#include "ResourceMgr.h"
#include "../shader/Shader.h"

namespace oengine2d {
	bool ResourceMgr::Initialize() {
		_queue.SetProcFunc(std::bind(&ResourceMgr::DoLoad, *this, std::placeholders::_1));
		_queue.SetCompleteFunc(std::bind(&ResourceMgr::CompleteLoad, *this, std::placeholders::_1));
		_queue.Start();
		return true;
	}

	void ResourceMgr::Destroy() {
	}

	void ResourceMgr::Loop() {
		_queue.Complete(1000);
	}

	ResourcePtr ResourceMgr::SyncLoad(const char* path, const ResourceType type) {
		{
			auto itr = _asyncloadListeners.find(path);
			if (itr != _asyncloadListeners.end()) {
				return ResourcePtr::Nil();
			}
		}

		auto itr = _resources.find(path);
		if (itr != _resources.end()) {
			return ResourcePtr(itr->second);
		}

		return ResourcePtr::Nil();
	}

	void ResourceMgr::AsyncLoad(const char* path, const ResourceType type, IResourceLoadListener* listener) {
		{
			auto itr = _resources.find(path);
			if (itr != _resources.end()) {
				if (itr->second->GetType() == type)
					listener->OnLoadRes(path, ResourcePtr(itr->second));
				else
					listener->OnFailed(path);
				return;
			}
		}

		auto itr = _asyncloadListeners.find(path);
		if (itr != _asyncloadListeners.end()) {
			if (itr->second.iscacel)
				itr->second.iscacel = false;

			itr->second.listeners.emplace_back(listener);
		}
		else {
			_asyncloadListeners[path].listeners.emplace_back(listener);

			_queue.Do({ path, _asyncloadListeners[path].iscacel, false });
		}
	}

	ResourcePtr ResourceMgr::Find(const char* path, const ResourceType type) {
		auto itr = _resources.find(path);
		if (itr != _resources.end() && itr->second->GetType() == type) {
			return ResourcePtr(itr->second);
		}
		return ResourcePtr();
	}

	void ResourceMgr::DoLoad(AsyncLoadTask& task) {

	}

	void ResourceMgr::CompleteLoad(AsyncLoadTask& task) {
		if (!task.resource.isNull()) {
			
		}
	}
}
