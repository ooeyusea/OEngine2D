#ifndef __RENDERSTATE_H__
#define __RENDERSTATE_H__
#include "util.h"
#include "../defines/RSDef.h"

namespace oengine2d {
	class RenderState {
	public:
		enum Mask {
			ENABLE_BLEND = 0x00000001,
			BLEND_FUNC_SRC = 0x00000002,
			BLEND_FUNC_DST = 0x00000004,

			ENABLE_ALPHA = 0x00000008,
			ALPHA_FUNC = 0x00000010,
			ALPHA_FACTOR = 0x00000020,

			ENABLE_DEPTH = 0x00000040,
			DEPTH_WRITE = 0x00000080,
			DEPTH_FUNC = 0x00000100,

			ENABLE_CULL_FACE = 0x00000200,
			CULL_FACE_SIDE = 0x00000400,
			FRONT_FACE = 0x00000800,

			ENABLE_STENCIL_TEST = 0x00001000,
			STENCIL_WRITE_MASK = 0x00002000,
			STENCIL_FUNC = 0x00004000,
			STENCIL_FUNC_REF = 0x00008000,
			STENCIL_FUNC_MASK = 0x00010000,
			STENCIL_OP_S_FAIL = 0x00020000,
			STENCIL_OP_D_FAIL = 0x00040000,
			STENCIL_OP_D_PASS = 0x00080000,
		};

		RenderState();
		~RenderState();

		inline void SetBlend(bool enable) { _enableBlend = enable; }
		inline void SetBlendFunc(rs::BlendFactor src, rs::BlendFactor dst) { _blendFunc = { src, dst }; }
		inline void SetAlphaTest(bool enable) { _enableAlphaTest = enable; }
		inline void SetAlphaTestFunc(rs::AlphaTestFunc func, float factor) { _alphaTestFunc = func; _alphaTestFactor = factor; }
		inline void SetDepthTest(bool enable) { _enableDepthTest = enable; }
		inline void SetDepthTestWrite(bool write) { _enableDepthWrite = write; }
		inline void SetDepthTestFunc(rs::DepthFunc func) { _depthFunc = func; }
		inline void SetCullFace(bool enable) { _enableCullFace = enable; }
		inline void SetCullFaceSide(rs::CullFaceSide side) { _cullFaceSide = side; }
		inline void SetFrontFace(rs::FrontFace front) { _frontFace = front; }
		inline void SetStencilTest(bool enable) { _enableStencilTest = enable; }
		inline void SetStencilMask(uint32_t writeMask) { _stencilWriteMask = writeMask; }
		inline void SetStencilFunc(rs::StencilFuncType func, int32_t ref, uint32_t mask) { _stencilFunc = { func, ref, mask }; }
		inline void SetStencilOp(rs::StencilOpType sf, rs::StencilOpType df, rs::StencilOpType dp) { _stencilOp = { sf, df, dp }; }
		inline void SetMask(uint32_t mask) { _mask = mask; }

		void Merge(const RenderState& rhs);
		void Build();

		inline const VkPipelineRasterizationStateCreateInfo& GetRasterizationState() const { return _rasterizationState; }
		inline const VkPipelineMultisampleStateCreateInfo& GetMultisampleState() const { return _multisampleState; }
		inline const VkPipelineDepthStencilStateCreateInfo& GetDepthStencilState() const { return _depthStencilState; }
		inline const VkPipelineColorBlendStateCreateInfo& GetColorBlendState() const { return _colorBlendState; }

	private:
		bool _enableBlend;
		rs::BlendFunc _blendFunc;

		bool _enableAlphaTest;
		rs::AlphaTestFunc _alphaTestFunc;
		float _alphaTestFactor;

		bool _enableDepthTest;
		bool _enableDepthWrite;
		rs::DepthFunc _depthFunc;

		bool _enableCullFace;
		rs::CullFaceSide _cullFaceSide;
		rs::FrontFace _frontFace;

		bool _enableStencilTest;
		uint32_t _stencilWriteMask;
		rs::StencilFunc _stencilFunc;
		rs::StencilOp _stencilOp;

		uint32_t _mask;

		VkPipelineRasterizationStateCreateInfo _rasterizationState;
		VkPipelineMultisampleStateCreateInfo _multisampleState;
		VkPipelineDepthStencilStateCreateInfo _depthStencilState;
		VkPipelineColorBlendAttachmentState _colorBlendAttachmentState;
		VkPipelineColorBlendStateCreateInfo _colorBlendState;
	};
}

#endif //__TECHNIQUE_H__

