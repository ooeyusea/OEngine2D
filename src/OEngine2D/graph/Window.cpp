#include "Window.h"
#include "Config.h"

namespace oengine2d {
	bool Window::Init() {
		_size = { Config::GetInstance().GetWindowWidth(), Config::GetInstance().GetWindowHeight() };
		CHECK(CreateGlfwWindow(), "init window false");
		return true;
	}

	void Window::Release() {
		DestroyWindow();
	}

	bool Window::ShouldClose() {
		return glfwWindowShouldClose(_window);
	}

	void Window::Update() {
		glfwPollEvents();
	}

	std::vector<const char*> Window::GetRequiredExtensions() {
		std::vector<const char*> extensions;

		unsigned int glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (unsigned int i = 0; i < glfwExtensionCount; i++) {
			extensions.push_back(glfwExtensions[i]);
		}

		return extensions;
	}

#ifdef WIN32
	HWND Window::GetWin32Window() {
		return glfwGetWin32Window(_window);
	}
#endif

	bool Window::CreateGlfwWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		_window = glfwCreateWindow(_size.x, _size.y, "Vulkan window", nullptr, nullptr);

		if (!_window) {
			return false;
		}
		return true;
	}

	void Window::DestroyWindow() {
		if (_window)
			glfwDestroyWindow(_window);

		glfwTerminate();
	}
}
