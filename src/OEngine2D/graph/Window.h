#pragma once

#include "util.h"
#include "singleton.h"

namespace oengine2d {
	class Window : public Singleton<Window> {
	public:
		Window() {}
		~Window() {}

		bool Init();
		void Release();

		bool ShouldClose();
		void Update();

		inline GLFWwindow* Get() { return _window; }

		std::vector<const char*> GetRequiredExtensions();
		const glm::uvec2& GetSize() const { return _size; }

#ifdef WIN32
		HWND GetWin32Window();
#endif

	private:
		bool CreateGlfwWindow();
		void DestroyWindow();

	private:
		glm::uvec2 _size;

		GLFWwindow* _window = nullptr;
	};
}
