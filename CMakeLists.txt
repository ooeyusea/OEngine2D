#cmake file for project kernel
#author:ooeyusea
#created:2015-07-08 09:38:00

cmake_minimum_required(VERSION 2.8)
PROJECT(OEngine2D)
IF(MSVC)
	SET(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /Zm8192000 /MTD /Zi /Ob0 /Od /RTC1")
	SET(CMAKE_C_FLAGS_MINSIZEREL_INIT "/MT /Zm8192000 /O1 /Ob1 /D NDEBUG")
	SET(CMAKE_C_FLAGS_RELEASE_INIT "/MT /Zm8192000 /O2 /Ob2 /D NDEBUG")
	SET(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zm8192000 /Zi /O2 /Ob1 /D NDEBUG")
	MESSAGE(STATUS "WINDOWS")
	add_compile_options("/std:c++17")
ENDIF(MSVC)
 
IF(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WIN32_KHR")
ELSE(WIN32)

IF(USE_D2D_WSI)
    MESSAGE("Using direct to display extension...")
    add_definitions(-D_DIRECT2DISPLAY)
ELSEIF(USE_WAYLAND_WSI)
    find_package(Wayland REQUIRED)
    if (NOT WAYLAND_FOUND)
            message(FATAL_ERROR "Wayland development package not found")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_WAYLAND_KHR")
    include_directories(${WAYLAND_INCLUDE_DIR})
ELSE(USE_D2D_WSI)
    find_package(XCB REQUIRED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_USE_PLATFORM_XCB_KHR")
ENDIF(USE_D2D_WSI)

ENDIF(WIN32)

IF(MSVC)
	SET(VULKAN_DIR "" CACHE PATH "Vulkan directory")
	IF (NOT VULKAN_DIR)
		message(FATAL_ERROR "Could not find Vulkan library!")
	ENDIF()

	SET(Vulkan_INCLUDE_DIRS 
		"${VULKAN_DIR}/Include" 
		"${VULKAN_DIR}/Third-Party/Include"
	)

	SET(Vulkan_LIBRARY 
		${VULKAN_DIR}/Lib/vulkan-1.lib
	)
ELSE(MSVC)
	IF (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
		MESSAGE(STATUS "Using module to find Vulkan")
		FIND_PACKAGE(Vulkan REQUIRED)
	ENDIF()

	IF (NOT Vulkan_FOUND)
		message(FATAL_ERROR "Could not find Vulkan library!")
	ELSE()
		message(STATUS ${Vulkan_LIBRARY})
	ENDIF()
ENDIF(MSVC)

include(FetchContent)

find_package(glfw3 QUIET)
if(NOT TARGET glfw)
	FetchContent_Declare(glfw3
		URL https://github.com/glfw/glfw/archive/3.3.tar.gz
		URL_MD5 5be03812f5d109817e6558c3fab7bbe1
	)
	FetchContent_GetProperties(glfw3)
	if(NOT glfw3_POPULATED)
		foreach(_glfw3_option "GLFW_BUILD_TESTS" "GLFW_BUILD_EXAMPLES" "GLFW_BUILD_DOCS")
			set(${_glfw3_option} OFF CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glfw3)
		add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR})
	endif()

	# Used later to define as a pre-build dependency
	# The glfw3Config.cmake file doesn't define it automatically
	set(glfw_FOUND false)
else()
	set(glfw_FOUND true)
endif()

# SPIRV and other GLSLang libraries are needed.
# NOTE: End-users can pass -DSPIRV_ROOT=/some/path to find the lib
set(SPIRV_ROOT CACHE PATH "An optional path to the system's SPIRV root dir to help find it. Ignore if building Glslang locally.")
find_library(SPIRV_LIBRARY
	NAMES "SPIRV" "libSPIRV"
	HINTS "${SPIRV_ROOT}"
)
find_library(GLSLANG_LIBRARY
	NAMES "glslang" "libglslang"
	HINTS "${SPIRV_ROOT}"
)
find_library(OSDEPENDENT_LIBRARY
	NAMES "OSDependent" "libOSDependent"
	HINTS "${SPIRV_ROOT}"
)
find_library(OGLCOMPILER_LIBRARY
	NAMES "OGLCompiler" "libOGLCompiler"
	HINTS "${SPIRV_ROOT}"
)
find_library(HLSL_LIBRARY
	NAMES "HLSL" "libHLSL"
	HINTS "${SPIRV_ROOT}"
)
find_path(SPIRV_INCLUDE_DIR
	NAMES "GlslangToSpv.h"
	PATH_SUFFIXES "SPIRV"
	HINTS "${SPIRV_ROOT}"
)

if(NOT SPIRV_LIBRARY
	OR NOT GLSLANG_LIBRARY
	OR NOT OSDEPENDENT_LIBRARY
	OR NOT OGLCOMPILER_LIBRARY
	OR NOT HLSL_LIBRARY
	OR NOT SPIRV_INCLUDE_DIR)
	set(_ACID_ALL_SYSTEM_LIBS false)
	FetchContent_Declare(glslang
		URL https://github.com/KhronosGroup/glslang/archive/7.12.3352.tar.gz
		URL_MD5 b31bc89ffa86dbb5ab638c9c2a412302
	)
	FetchContent_GetProperties(glslang)
	if(NOT glslang_POPULATED)
		foreach(_glslang_option "BUILD_TESTING" "ENABLE_GLSLANG_BINARIES" "ENABLE_SPVREMAPPER" "ENABLE_HLSL" "ENABLE_AMD_EXTENSIONS" "ENABLE_NV_EXTENSIONS")
			set(${_glslang_option} OFF CACHE INTERNAL "")
		endforeach()
		foreach(_glslang_option "ENABLE_OPT")
			set(${_glslang_option} ON CACHE INTERNAL "")
		endforeach()
		FetchContent_Populate(glslang)
		add_subdirectory(${glslang_SOURCE_DIR} ${glslang_BINARY_DIR})
	endif()

	# Used later to define as a pre-build dependencies
	# Have to manually define because we manually searched for SPIRV
	set(SPIRV_FOUND false)
	# Used in target_link_libraries()
	# Please note that SPIRV is now a CMake target, which means transitive dependencies are taken into account.
	set(SPIRV_LIBRARIES "SPIRV")
else()
	set(SPIRV_FOUND true)
	# glslang, hlsl and the others are transitive dependencies of libSPIRV, which are not detected
	# during linking (because the project might be a shared object).
	set(SPIRV_LIBRARIES
		"${SPIRV_LIBRARY}"
		"${GLSLANG_LIBRARY}"
		"${OSDEPENDENT_LIBRARY}"
		"${OGLCOMPILER_LIBRARY}"
		"${HLSL_LIBRARY}"
	)
endif()

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/OEngine2D)
