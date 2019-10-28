#include "Application.h"
#include "system/FileSystem.h"
#include "graph/Window.h"
#include "graph/Graph.h"
#include "graph/stage/RenderStageBuilder.h"
#include "Config.h"

namespace oengine2d {
	void Application::Run() {
		do {
			INIT_STEP(Config::GetInstance().Init(), "load configure");
			INIT_STEP(FileSystem::GetInstance().Init(), "init file system");
			INIT_STEP(Window::GetInstance().Init(), "init window");
			INIT_STEP(Graph::GetInstance().Init(), "init graph");
			INIT_STEP(RenderStageBuilder().Build(), "build render stage");
			
			MainLoop();
		} while (false);

		Graph::GetInstance().Release();
		FileSystem::GetInstance().Release();
		Window::GetInstance().Release();
	}

	void Application::MainLoop() {
		while (!Window::GetInstance().ShouldClose()) {
			Window::GetInstance().Update();
			Graph::GetInstance().Update();
		}
	}
}
