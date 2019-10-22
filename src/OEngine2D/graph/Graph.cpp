#include "Graph.h"
#include "Surface.h"
#include "Instance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include "SwapChain.h"

namespace oengine2d {
	bool Graph::Init() {
		_instance = new Instance;
		CHECK(_instance->Create(), "create instance failed");

		_physicalDevice = new PhysicalDevice(*_instance);
		CHECK(_physicalDevice->Create(), "pick physical device failed");

		_surface = new Surface(*_instance, *_physicalDevice);
		CHECK(_surface->Create(), "create surface failed");

		_logicalDevice = new LogicalDevice(*_instance, *_physicalDevice, *_surface);
		CHECK(_logicalDevice->Create(), "create logical device failed");

		_swapChain = new SwapChain(*_surface, *_logicalDevice);
		CHECK(_swapChain->Create(), "create swap chain failed");
		
		return true;
	}

	void Graph::Release() {
		SAFE_DELETE(_swapChain);
		SAFE_DELETE(_logicalDevice);
		SAFE_DELETE(_surface);
		SAFE_DELETE(_physicalDevice);
		SAFE_DELETE(_instance);
	}

	void Graph::Update() {

	}
}
