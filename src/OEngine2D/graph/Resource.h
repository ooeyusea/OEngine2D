#pragma once
#include "util.h"

namespace oengine2d {
	enum class ResourceType {
		RT_TEXTURE2D,
	};

	class Resource {
	public:
		Resource(ResourceType type, const std::string& path) : _type(type), _path(path) {}
		~Resource() {}

		inline ResourceType GetType() const { return _type; }

		inline const std::string& GetPath() const { return _path; }

		inline void AddRef() { ++_ref; }
		inline void DecRef() { --_ref; }
		inline bool IsNoUse() const { return _ref == 0; }

		virtual bool Load() = 0;

	protected:
		ResourceType _type;
		std::string _path;

		int32_t _ref = 0;
	};
}
