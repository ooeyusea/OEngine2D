#pragma once
#include "util.h"

namespace oengine2d {
	class Application {
	public:
		Application() {}
		~Application() {}

		void Run();

	private:
		void MainLoop();
	};
}