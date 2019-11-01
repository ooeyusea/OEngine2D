#pragma once

#include "util.h"

namespace oengine2d {
	class IndexBuffer {
	public:
		enum IndexFormatType {
			IFT_U16 = 0,
			IFT_U32,
		};

		enum IndexBufferMemoryType {
			IBMT_GENERAL,
			IBMT_VIDEO,
			IBMT_TRANSMIT,
		};

		IndexBuffer(IndexFormatType format, IndexBufferMemoryType type, s32 count) : _format(format), _type(type), _count(count) {
			_size = ((format == IndexFormatType::IFT_U16) ? sizeof(uint16_t) : sizeof(uint32_t)) * _count;
		}
		~IndexBuffer();

		bool Create();
		void Update();

		bool WriteBuffer(const void* p);
		bool WriteBuffer(uint32_t offset, uint32_t size, const void* p);
		bool CopyBuffer();

	private:
		bool CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory);

	private:
		IndexFormatType _format;
		IndexBufferMemoryType _type;
		uint32_t _count = 0;
		uint32_t _size = 0;

		VkBuffer _indexBuffer = nullptr;
		VkDeviceMemory _indexBufferMemory = nullptr;

		VkBuffer _stagingBuffer = nullptr;
		VkDeviceMemory _stagingBufferMemory = nullptr;

		bool _needCopy = false;
	};
}
