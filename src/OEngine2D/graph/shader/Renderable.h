#pragma once
#include "util.h"

namespace oengine2d {
	class DescriptorSet;
	class PropertyHandler;
	class Pipeline;
	class CommandBuffer;
	class UniformBuffer;
	class VertexBuffer;
	class IndexBuffer;
	class Renderable {
	public:
		Renderable() {}
		virtual ~Renderable() {}

		CommandBuffer* GetCommandBuffer(uint32_t frame, const std::string& name, const std::string& stage);
		CommandBuffer* CreateCommandBuffer(uint32_t frame, const std::string& name, const std::string& stage);

		DescriptorSet* GetDescriptorSet(const std::string& name, const PropertyHandler& handler, const Pipeline& pipeline);

		inline bool IsChanged() const { return _changed; }

		inline void SetLod(uint32_t lod) { _lod = lod; }
		inline uint32_t GetLod() const { return _lod; }

		inline void SetVertexBuffer(VertexBuffer * vb) { _vertexBuffer = vb; }
		inline VertexBuffer * GetVertexBuffer() const { return _vertexBuffer; }

		inline void SetIndexBuffer(IndexBuffer * ib) { _indexBuffer = ib; }
		inline IndexBuffer* GetIndexBuffer() const { return _indexBuffer; }

		inline void SetLod(uint32_t lod) { _lod = lod; }
		inline uint32_t GetLod() const { return _lod; }

	protected:
		std::map<std::string, std::map<std::string, std::map<uint32_t, CommandBuffer*>>> _commandBuffers;
		std::map<std::string, DescriptorSet*> _descriptorSets;

		bool _changed = false;
		UniformBuffer* _uniformBuffer = nullptr;
		uint32_t _lod = 0;

		VertexBuffer * _vertexBuffer = nullptr;
		IndexBuffer * _indexBuffer = nullptr;
	};
}
