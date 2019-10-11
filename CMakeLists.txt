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

ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/src/OEngine2D)
