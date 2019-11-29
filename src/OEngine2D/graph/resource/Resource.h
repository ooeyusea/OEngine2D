#pragma once
#include "util.h"

namespace oengine2d {
	enum class ResourceType {
		RT_TEXTURE2D,
		RT_MATERIAL,
		RT_SHADER,
	};

	class Resource {
	public:
		Resource(ResourceType type, const std::string& path) : _type(type), _path(path) {}
		virtual ~Resource() {}

		inline ResourceType GetType() const { return _type; }

		inline const std::string& GetPath() const { return _path; }

		inline void IncRef() const { ++_ref; }
		inline void DecRef() const { --_ref; }
		inline bool IsNoUse() const { return _ref == 0; }

		virtual bool Load() = 0;

	protected:
		ResourceType _type;
		std::string _path;

		mutable int32_t _ref = 0;
	};

	class ResourcePtr;
	template <typename T>
	class ResFunctionPtr {
		friend class ResourcePtr;
	public:
		~ResFunctionPtr() {}

		inline T* operator->() const { return _res; }
		inline bool operator==(const ResFunctionPtr& rhs) const { return _res == rhs._res; }
		inline bool operator!=(const ResFunctionPtr& rhs) const { return _res != rhs._res; }
		inline bool operator()() const { return _res != nullptr; }

	private:
		ResFunctionPtr(T* res) : _res(res) {}

		ResFunctionPtr(const ResFunctionPtr&);
		ResFunctionPtr(const ResFunctionPtr&&);
		ResFunctionPtr& operator=(const ResFunctionPtr&);
		ResFunctionPtr& operator=(const ResFunctionPtr&&);

		T* _res;
	};

	class ResourceMgr;
	class ResourcePtr {
		friend class ResourceMgr;
	public:
		ResourcePtr() : _res(nullptr) {}
		ResourcePtr(const ResourcePtr& rhs)
			: _res(rhs._res) {
			if (rhs._res)
				rhs._res->IncRef();
		}

		~ResourcePtr() {
			if (_res)
				_res->DecRef();
			_res = nullptr;
		}

		bool isNull() const { return _res == nullptr; }
		inline bool operator==(const ResourcePtr& rhs) const { return _res == rhs._res; }
		inline bool operator!=(const ResourcePtr& rhs) const { return _res != rhs._res; }
		inline bool operator()() const { return _res != nullptr; }

		inline ResourcePtr& operator=(const ResourcePtr& rhs) {
			if (this != &rhs) {
				if (_res)
					_res->DecRef();
				_res = rhs._res;
				if (_res)
					_res->IncRef();
			}
			return *this;
		}

		template<typename T>
		inline ResFunctionPtr<T> cast(const ResourceType type) const {
			OASSERT(type == _res->GetType(), "wtf");
			return ResFunctionPtr<T>((T*)_res);
		}

		static ResourcePtr Nil() { return ResourcePtr(); }

	private:
		ResourcePtr(Resource* res)
			: _res(res) {
			if (res)
				res->IncRef();
		}

	private:
		Resource* _res;
	};
}
