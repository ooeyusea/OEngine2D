#pragma once
#include "util.h"
#include <mutex>

namespace oengine2d {
	template <typename T>
	class AsyncTaskQueue {
	public:
		AsyncTaskQueue() {
			
		}

		~AsyncTaskQueue() {
			_terminate = true;
			_cond.notify_all();
			_thread.join();
		}

		inline void SetProcFunc(const std::function<void(T&)>& f) { _proc = f; }
		inline void SetCompleteFunc(const std::function<void(T&)>& f) { _completeFn = f; }

		void Start() {
			_thread = std::thread(&AsyncTaskQueue::ThreadProc, this);
		}

		void Do(const T& t) {
			if (!_proc)
				return;

			std::unique_lock<std::mutex> lock(_mutex);
			bool empty = _doingQueue.empty();
			_doingQueue.emplace_back(t);
			if (empty)
				_cond.notify_one();
		}

		void Complete(int32_t count = -1) {
			if (count == -1) {
				{
					std::unique_lock<std::mutex> lock(_doneMutex);
					_completeQueue.splice(_completeQueue.end(), _doneQueue);
				}

				while (!_completeQueue.empty()) {
					_completeFn(*_completeQueue.begin());
					_completeQueue.pop_front();
				}
			}
			else {
				while (count > 0) {
					if (_completeQueue.empty()) {
						std::unique_lock<std::mutex> lock(_doneMutex);
						_completeQueue.splice(_completeQueue.end(), _doneQueue);
					}

					if (_completeQueue.empty())
						break;

					_completeFn(*_completeQueue.begin());
					_completeQueue.pop_front();
					--count;
				}
			}
		}

		void ThreadProc() {
			while (!_terminate) {
				if (_doQueue.empty())
				{
					std::unique_lock<std::mutex> lock(_mutex);
					while (!_terminate && _doingQueue.empty())
						_cond.wait(lock);
					
					if (!_terminate)
						_doQueue.swap(_doingQueue);
				}

				if (!_doQueue.empty() && proc) {
					auto itr = _doQueue.begin();
					proc(*itr);

					if (_completeFn) {
						std::unique_lock<std::mutex> lock(_doneMutex);
						_doneQueue.splice(_doneQueue.end(), _doQueue, itr);
					}
					else
						_doQueue.pop_front();
				}
			}
		}

	private:
		bool _terminate = false;
		std::function<void(T&)> _proc;
		std::function<void(T&)> _completeFn;
		std::thread _thread;

		
		std::mutex _mutex;
		std::condition_variable _cond;
		std::list<T> _doingQueue;

		std::list<T> _doQueue;

		std::mutex _doneMutex;
		std::list<T> _doneQueue;

		std::list<T> _completeQueue;
	};
}