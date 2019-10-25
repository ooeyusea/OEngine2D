#pragma once

#include "util.h"
#include "singleton.h"

namespace oengine2d {
	class Config : public Singleton<Config> {
	public:
		Config() {}
		~Config() {}

		bool Init();
		void Release();

		inline uint32_t GetWindowWidth() const { return _width; }
		inline uint32_t GetWindowHeight() const { return _height; }

		inline uint32_t GetMaxFramesInFlight() const { return _maxFramesInFlight; }

	private:
		uint32_t _width = 800;
		uint32_t _height = 600;

		uint32_t _maxFramesInFlight = 3;
	};
}
