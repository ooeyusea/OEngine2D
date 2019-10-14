#pragma once
#include "util.h"
#include "singleton.h"

namespace oengine2d {
	class FileSystem : public Singleton<FileSystem> {
	public:
		FileSystem() {}
		~FileSystem() {}

		bool Init();
		void Release();

		bool ReadFile(const std::string& path, std::string& buffer);
	private:
	};
}
