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
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/Resource.h"
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

SET(GRAPH_MATERIAL_UNIFORM
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float1Uniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float1Uniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float2Uniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float2Uniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float3Uniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float3Uniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float4Uniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Float4Uniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/MatrixUniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/MatrixUniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Sampler2DUniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/uniform/Sampler2DUniform.cpp"
)

SET(GRAPH_MATERIAL
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Pipeline.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Pipeline.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Pass.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Pass.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Technique.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Technique.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Material.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Material.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Uniform.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Uniform.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/RenderState.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/RenderState.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/RenderOption.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/RenderOption.cpp"
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
SOURCE_GROUP(src\\graph\\material FILES ${GRAPH_MATERIAL})
SOURCE_GROUP(src\\graph\\material\\uniform FILES ${GRAPH_MATERIAL_UNIFORM})
SOURCE_GROUP(src\\graph\\vertex_data FILES ${GRAPH_VERTEX_DATA})
SOURCE_GROUP(src\\graph\\texture FILES ${GRAPH_TEXTURE})
SOURCE_GROUP(src\\graph\\stage FILES ${GRAPH_STAGE})
SOURCE_GROUP(src\\system FILES ${SYS})
SOURCE_GROUP(src FILES ${KERNEL})
SOURCE_GROUP(resources\\shader FILES ${RESOURCES_SHADER})
SOURCE_GROUP(resources FILES ${RESOURCES})

ADD_EXECUTABLE(OEngine2D
    ${UTIL}
	${DETAIL}
	${TINYXML}
	${GRAPH}
	${GRAPH_MATERIAL}
	${GRAPH_VERTEX_DATA}
	${GRAPH_TEXTURE}
	${GRAPH_STAGE}
	${GRAPH_MATERIAL_UNIFORM}
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