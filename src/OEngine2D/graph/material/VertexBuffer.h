#pragma once

#include "util.h"
#include "VertexDescription.h"

namespace oengine2d {
	class VertexBuffer {
	public:
		enum VertexBufferMemoryType {
			VBMT_GENERAL,
			VBMT_VIDEO,
			VBMT_TRANSMIT,
		};

		VertexBuffer(std::shared_ptr<VertexDescription>& ptr, VertexBufferMemoryType type, uint32_t count) : _description(ptr), _type(type), _count(count) {
			_size = _description->GetVertexSize() * _count;
		}
		~VertexBuffer();

		bool Create();
		void Update();

		bool WriteBuffer(const void* p);
		bool WriteBuffer(uint32_t offset, uint32_t size, const void* p);
		bool CopyBuffer();

	private:
		bool CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory);

	private:
		std::shared_ptr<VertexDescription> _description = nullptr;
		VertexBufferMemoryType _type;
		uint32_t _count = 0;
		uint32_t _size = 0;

		VkBuffer _vertexBuffer = nullptr;
		VkDeviceMemory _vertexBufferMemory = nullptr;

		VkBuffer _stagingBuffer = nullptr;
		VkDeviceMemory _stagingBufferMemory = nullptr;

		bool _needCopy = false;
	};
}
