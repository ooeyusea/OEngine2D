#pragma once

#include "util.h"
#include "WriteDescriptorSet.h"

namespace oengine2d {
	class Pipeline;
	class UniformBuffer {
		struct UniformDescription {
			int32_t offset;
			int32_t size;
		};

		struct Buffer {
			VkBuffer uniformBuffer = nullptr;
			VkDeviceMemory uniformBufferMemory = nullptr;
		};

	public:
		UniformBuffer() {}
		~UniformBuffer();

		bool Create();
		
		template <typename T>
		inline void AddUniform(const std::string& name) {
			if (_descriptions.find(name) == _descriptions.end()) {
				_descriptions[name] = { _size, sizeof(T) };
				_size += sizeof(T);
			}
		}

		template <typename T>
		inline void Update(uint32_t frame, const std::string& name, const T& t) {
			Update(frame, name, &t, sizeof(t));
		}

		void GetDescriptorBuffInfo(uint32_t frame, const Pipeline& pipeline, std::vector<WriteDescriptorSet>& writes);

		inline bool IsCreated() const { return _created; }

	private:
		bool CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags flags, VkBuffer& buffer, VkDeviceMemory& memory);
		void Update(uint32_t frame, const std::string& name, const char* data, const int32_t size);

	private:
		uint32_t _size = 0;
		std::map<std::string, UniformDescription> _descriptions;

		std::vector<Buffer> _buffers;
		bool _created = false;
	};
}
