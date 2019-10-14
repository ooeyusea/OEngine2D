#include "FileSystem.h"
#include <fstream>

namespace oengine2d {
	bool FileSystem::Init() {
		return true;
	}

	void FileSystem::Release() {

	}

	bool FileSystem::ReadFile(const std::string& path, std::string& buffer) {
		std::ifstream file(path, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			return false;
		}

		size_t fileSize = (size_t)file.tellg();
		buffer.resize(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return true;
	}
}