#pragma once
#include "util.h"
#include "singleton.h"
#include "Resource.h"
#include "AsyncTaskQueue.h"

namespace oengine2d {
	class IResourceLoadListener {
	public:
		virtual ~IResourceLoadListener() {}

		virtual void OnLoadRes(const char* path, ResourcePtr resource) = 0;
		virtual void OnFailed(const char* path) = 0;
	};

	class ResourceMgr : public Singleton<ResourceMgr> {
		struct AsyncLoadTask {
			std::string path;
			bool& iscacel;
			ResourcePtr resource;
		};

		struct AsyncLoadInfo {
			std::vector<IResourceLoadListener*> listeners;
			bool iscacel = false;
		};

	public:
		ResourceMgr() {}
		~ResourceMgr() {}

		bool Initialize();
		void Destroy();
		void Loop();

		ResourcePtr SyncLoad(const char* path, const ResourceType type);
		void AsyncLoad(const char* path, const ResourceType type, IResourceLoadListener* listener);

		ResourcePtr Find(const char* path, const ResourceType type);

		void DoLoad(AsyncLoadTask& task);
		void CompleteLoad(AsyncLoadTask& task);

	protected:
		std::unordered_map<std::string, Resource*> _resources;

		AsyncTaskQueue<AsyncLoadTask> _queue;
		std::unordered_map<std::string, AsyncLoadInfo> _asyncloadListeners;
	};
}
