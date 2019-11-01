INCLUDE_DIRECTORIES(
	${Vulkan_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/thirdparty/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/"
	"${CMAKE_CURRENT_SOURCE_DIR}/"
)

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
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Pipeline.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/Pipeline.cpp"
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
)

SET(GRAPH_MATERIAL
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/VertexBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/VertexBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/IndexBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/IndexBuffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Image.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Image.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Texture2D.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/graph/material/Texture2D.cpp"
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

SOURCE_GROUP(src\\util FILES ${UTIL})
SOURCE_GROUP(src\\util\\detail FILES ${DETAIL})
SOURCE_GROUP(src\\util\\tinyxml FILES ${TINYXML})
SOURCE_GROUP(src\\graph FILES ${GRAPH})
SOURCE_GROUP(src\\graph\\material FILES ${GRAPH_MATERIAL})
SOURCE_GROUP(src\\graph\\stage FILES ${GRAPH_STAGE})
SOURCE_GROUP(src\\system FILES ${SYS})
SOURCE_GROUP(src FILES ${KERNEL})

ADD_EXECUTABLE(OEngine2D
    ${UTIL}
	${DETAIL}
	${TINYXML}
	${GRAPH}
	${GRAPH_MATERIAL}
	${GRAPH_STAGE}
	${SYS}
    ${KERNEL}
)

IF(WIN32)
TARGET_LINK_LIBRARIES(OEngine2D 
	debug "${PROJECT_SOURCE_DIR}/thirdparty/libs/debug/glfw3dll.lib"
	optimized "${PROJECT_SOURCE_DIR}/thirdparty/libs/release/glfw3dll.lib"
	debug "${PROJECT_SOURCE_DIR}/thirdparty/libs/debug/shaderc_combined.lib"
	optimized "${PROJECT_SOURCE_DIR}/thirdparty/libs/release/shaderc_combined.lib"
	debug "${PROJECT_SOURCE_DIR}/thirdparty/libs/debug/FreeImaged.lib"
	optimized "${PROJECT_SOURCE_DIR}/thirdparty/libs/release/FreeImage.lib"
	${Vulkan_LIBRARY}
)
ENDIF(WIN32)

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")