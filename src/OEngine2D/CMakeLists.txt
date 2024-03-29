INCLUDE_DIRECTORIES(
	${Vulkan_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/thirdparty/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/"
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)

IF (NOT glfw_FOUND)
	INCLUDE_DIRECTORIES("${glfw3_SOURCE_DIR}/include")
ENDIF()

SET(KERNEL
    "${CMAKE_CURRENT_SOURCE_DIR}/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

SET(UTIL
    "${CMAKE_CURRENT_SOURCE_DIR}/util/util.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/util.cpp"
)

SET(TINYXML
    "${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinystr.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinystr.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinyxml.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinyxml.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinyxmlerror.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/util/tinyxml/tinyxmlparser.cpp"
)

IF(WIN32)
SET(DETAIL
	"${CMAKE_CURRENT_SOURCE_DIR}/util/detail/windows/win32.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/detail/windows/win32.cpp"
)
ENDIF(WIN32)

IF(UNIX)
SET(DETAIL
	"${CMAKE_CURRENT_SOURCE_DIR}/util/detail/linux/linux.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/detail/linux/linux.cpp"

)
ENDIF(UNIX)

SET(GRAPH
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Graph.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Graph.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/DrawAble.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/DrawAble.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Window.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Instance.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Instance.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/PhysicalDevice.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/PhysicalDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/LogicalDevice.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/LogicalDevice.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Surface.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Surface.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/SwapChain.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/SwapChain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/CommandPool.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/CommandPool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/CommandBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/CommandBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/RenderStage.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/RenderStage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/RenderPass.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/RenderPass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/FrameBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/FrameBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/Image.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Image.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/ImageView.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/ImageView.cpp"
)

SET(GRAPH_VERTEX_DATA
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/vertexdata/VertexDescription.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/vertexdata/VertexBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/vertexdata/VertexBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/vertexdata/IndexBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/vertexdata/IndexBuffer.cpp"
)

SET(GRAPH_TEXTURE
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Bitmap.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Bitmap.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Texture2D.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Texture2D.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Sampler.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/texture/Sampler.cpp"
)

SET(GRAPH_MATERIAL
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Material.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Material.cpp"
)

SET(GRAPH_SHADER
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Pipeline.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Pipeline.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/DescriptorSet.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/DescriptorSet.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Pass.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Pass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/SubShader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/SubShader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Shader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Shader.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/ShaderProperty.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/ShaderProperty.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/UniformBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/UniformBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/PropertyHandler.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/PropertyHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/RenderState.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/RenderState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Renderable.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/shader/Renderable.cpp"
)

SET(GRAPH_STAGE
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/PreDepthStage.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/PreDepthStage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/LightCullStage.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/LightCullStage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/ForwardStage.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/ForwardStage.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/RenderStageBuilder.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/stage/RenderStageBuilder.cpp"
)

SET(GRAPH_RESOURCE
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/resource/Resource.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/resource/ResourceMgr.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/resource/ResourceMgr.cpp"
)

SET(SYS
    "${CMAKE_CURRENT_SOURCE_DIR}/system/FileSystem.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/system/FileSystem.cpp"
)

SET(RESOURCES
	
)

SET(RESOURCES_SHADER
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/shader/oengine_in.glsl"
)

SOURCE_GROUP(src\\util FILES ${UTIL})
SOURCE_GROUP(src\\util\\detail FILES ${DETAIL})
SOURCE_GROUP(src\\util\\tinyxml FILES ${TINYXML})
SOURCE_GROUP(src\\graph FILES ${GRAPH})
SOURCE_GROUP(src\\graph\\shader FILES ${GRAPH_SHADER})
SOURCE_GROUP(src\\graph\\material FILES ${GRAPH_MATERIAL})
SOURCE_GROUP(src\\graph\\vertex_data FILES ${GRAPH_VERTEX_DATA})
SOURCE_GROUP(src\\graph\\texture FILES ${GRAPH_TEXTURE})
SOURCE_GROUP(src\\graph\\stage FILES ${GRAPH_STAGE})
SOURCE_GROUP(src\\graph\\resource FILES ${GRAPH_RESOURCE})
SOURCE_GROUP(src\\system FILES ${SYS})
SOURCE_GROUP(src FILES ${KERNEL})
SOURCE_GROUP(resources\\shader FILES ${RESOURCES_SHADER})
SOURCE_GROUP(resources FILES ${RESOURCES})

ADD_EXECUTABLE(OEngine2D
    ${UTIL}
	${DETAIL}
	${TINYXML}
	${GRAPH}
	${GRAPH_SHADER}
	${GRAPH_MATERIAL}
	${GRAPH_VERTEX_DATA}
	${GRAPH_TEXTURE}
	${GRAPH_STAGE}
	${GRAPH_RESOURCE}
	${SYS}
    ${KERNEL}
	${RESOURCES_SHADER}
	${RESOURCES}
)

IF(WIN32)
TARGET_LINK_LIBRARIES(OEngine2D 
	debug "${PROJECT_SOURCE_DIR}/thirdparty/libs/debug/FreeImaged.lib"
	optimized "${PROJECT_SOURCE_DIR}/thirdparty/libs/release/FreeImage.lib"
	glfw
	${SPIRV_LIBRARIES}
	${Vulkan_LIBRARY}
)
ENDIF(WIN32)

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")