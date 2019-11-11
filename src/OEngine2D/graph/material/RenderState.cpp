#include "RenderState.h"

#define MERGE_RENDER_STATE(mask, mask_var, variable) \
		if ((rhs.##mask_var & Mask::##mask) > 0)\
		{\
			if ((mask_var & Mask::##mask) == 0)\
			{\
				variable = rhs.##variable;\
				mask_var |= Mask::##mask; \
			}\
		}

namespace oengine2d {
	RenderState::RenderState()
		: _enableBlend(false)
		, _blendFunc({ rs::BlendFactor::BF_ONE, rs::BlendFactor::BF_ONE })
		, _enableAlphaTest(false)
		, _alphaTestFunc(rs::AlphaTestFunc::ATF_ALWAYS)
		, _alphaTestFactor(0.0f)
		, _enableDepthTest(true)
		, _enableDepthWrite(true)
		, _depthFunc(rs::DepthFunc::DF_LEQUAL)
		, _enableCullFace(false)
		, _cullFaceSide(rs::CullFaceSide::BACK)
		, _frontFace(rs::FrontFace::CCW)
		, _enableStencilTest(false)
		, _stencilWriteMask(0xff)
		, _stencilFunc({ rs::StencilFuncType::SF_ALWAYS, 0, 0xff })
		, _stencilOp({ rs::StencilOpType::SO_KEEP, rs::StencilOpType::SO_KEEP, rs::StencilOpType::SO_KEEP }) {
	}


	RenderState::~RenderState() {
	}

	void RenderState::Merge(const RenderState& rhs)
	{
		MERGE_RENDER_STATE(ENABLE_BLEND, _mask, _enableBlend);
		MERGE_RENDER_STATE(BLEND_FUNC_SRC, _mask, _blendFunc.src);
		MERGE_RENDER_STATE(BLEND_FUNC_DST, _mask, _blendFunc.dst);
		MERGE_RENDER_STATE(BLEND_FUNC_DST, _mask, _blendFunc.dst);
		MERGE_RENDER_STATE(ENABLE_ALPHA, _mask, _enableAlphaTest);
		MERGE_RENDER_STATE(ALPHA_FUNC, _mask, _alphaTestFunc);
		MERGE_RENDER_STATE(ALPHA_FACTOR, _mask, _alphaTestFactor);
		MERGE_RENDER_STATE(ENABLE_DEPTH, _mask, _enableDepthTest);
		MERGE_RENDER_STATE(DEPTH_WRITE, _mask, _enableDepthWrite);
		MERGE_RENDER_STATE(DEPTH_FUNC, _mask, _depthFunc);
		MERGE_RENDER_STATE(ENABLE_CULL_FACE, _mask, _enableCullFace);
		MERGE_RENDER_STATE(CULL_FACE_SIDE, _mask, _cullFaceSide);
		MERGE_RENDER_STATE(FRONT_FACE, _mask, _frontFace);
		MERGE_RENDER_STATE(ENABLE_STENCIL_TEST, _mask, _enableStencilTest);
		MERGE_RENDER_STATE(STENCIL_WRITE_MASK, _mask, _stencilWriteMask);
		MERGE_RENDER_STATE(STENCIL_FUNC, _mask, _stencilFunc.func);
		MERGE_RENDER_STATE(STENCIL_FUNC_REF, _mask, _stencilFunc.ref);
		MERGE_RENDER_STATE(STENCIL_FUNC_MASK, _mask, _stencilFunc.mask);
		MERGE_RENDER_STATE(STENCIL_OP_S_FAIL, _mask, _stencilOp.s_fail);
		MERGE_RENDER_STATE(STENCIL_OP_D_FAIL, _mask, _stencilOp.d_fail);
		MERGE_RENDER_STATE(STENCIL_OP_D_PASS, _mask, _stencilOp.d_pass);
	}

	void RenderState::Build() {
		_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		_rasterizationState.depthClampEnable = _depthClamp ? VK_TRUE : VK_FALSE;
		_rasterizationState.rasterizerDiscardEnable = _rasterizerDiscard ? VK_TRUE : VK_FALSE;
		_rasterizationState.polygonMode = _polygonMode;
		_rasterizationState.lineWidth = _lineWidth;
		_rasterizationState.cullMode = _cullMode;
		_rasterizationState.frontFace = _frontFace;
		_rasterizationState.depthBiasEnable = _rasterizerDiscard ? VK_TRUE : VK_FALSE;
		_rasterizationState.depthBiasConstantFactor = _depthBiasConstantFactor;
		_rasterizationState.depthBiasClamp = _depthBiasClamp;
		_rasterizationState.depthBiasSlopeFactor = _depthBiasSlopeFactor;

		_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_multisampleState.sampleShadingEnable = VK_FALSE;
		_multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		_multisampleState.minSampleShading = 1.0f; // Optional
		_multisampleState.pSampleMask = nullptr; // Optional
		_multisampleState.alphaToCoverageEnable = VK_FALSE; // Optional
		_multisampleState.alphaToOneEnable = VK_FALSE; // Optional

		_colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		_colorBlendAttachmentState.blendEnable = VK_FALSE;
		_colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		_colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		_colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		_colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		_colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		_colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		_colorBlendState.logicOpEnable = VK_FALSE;
		_colorBlendState.logicOp = VK_LOGIC_OP_COPY; // Optional
		_colorBlendState.attachmentCount = 1;
		_colorBlendState.pAttachments = &colorBlendAttachment;
		_colorBlendState.blendConstants[0] = 0.0f; // Optional
		_colorBlendState.blendConstants[1] = 0.0f; // Optional
		_colorBlendState.blendConstants[2] = 0.0f; // Optional
		_colorBlendState.blendConstants[3] = 0.0f; // Optional
	}
}