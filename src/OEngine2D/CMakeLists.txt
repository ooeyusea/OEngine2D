INCLUDE_DIRECTORIES(
	${Vulkan_INCLUDE_DIRS}
	"${PROJECT_SOURCE_DIR}/thirdparty/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/"
)

SET(KERNEL
    "${CMAKE_CURRENT_SOURCE_DIR}/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

SET(UTIL
    "${CMAKE_CURRENT_SOURCE_DIR}/util/util.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/util.cpp"
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

SOURCE_GROUP(src\\util FILES ${UTIL})
SOURCE_GROUP(src\\util\\detail FILES ${DETAIL})
SOURCE_GROUP(src FILES ${KERNEL})

ADD_EXECUTABLE(OEngine2D
    ${UTIL}
	${DETAIL}
    ${KERNEL}
)

IF(WIN32)
TARGET_LINK_LIBRARIES(OEngine2D 
	debug "${PROJECT_SOURCE_DIR}/thirdparty/libs/debug/glfw3dll.lib"
	optimized "${PROJECT_SOURCE_DIR}/thirdparty/libs/release/glfw3dll.lib"
	${Vulkan_LIBRARY}
)
ENDIF(WIN32)

SET(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin")