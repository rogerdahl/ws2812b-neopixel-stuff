cmake_minimum_required(VERSION 3.5)
project(emulator)

# https://cmake.org/cmake/help/v3.0/policy/CMP0015.html
cmake_policy(SET "CMP0015" "NEW")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    # Handling of compiler warnings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wno-deprecated-declarations")
    # Add relative ./lib to dynamic linker search path
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--enable-new-dtags,--rpath=${ORIGIN}./lib")
endif ()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/linux)
else ()
    set(LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/win64)
endif ()

set(EFFECTS_DIR ${CMAKE_SOURCE_DIR}/../../effects)
set(INTERFACE_DIR ${CMAKE_SOURCE_DIR}/../../interface)

## glm
#if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#else ()
#    set(GLM_INCLUDE_DIR ${LIBRARIES}/glm-0.9.7.4/glm)
#endif ()

# OpenGL and GLU
find_package(OpenGL REQUIRED)

## X
#if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
#    set(X_LIBRARIES Xrandr X11 Xxf86vm Xinerama Xcursor Xi)
#else ()
#    set(X_LIBRARIES "")
#endif ()

# GCC
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GCC_LIBRARIES m pthread dl)
    # -lglut -lGLU
    # -fstack-protector-all
else ()
    set(GCC_LIBRARIES "")
endif ()

# GLFW
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(GLFW_LIBRARIES glfw)
else ()
    set(GLFW_LIBRARIES glfw3.lib)
    set(GLFW_INCLUDE_DIRS ${LIBRARIES}/glfw-3.2.1.bin.WIN64/include)
    set(GLFW_LINK_DIRS ${LIBRARIES}/glfw-3.2.1.bin.WIN64/lib-vc2015)
endif ()

# fmt
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(FMT_LIBRARIES fmt)
    set(FMT_INCLUDE_DIR ${LIBRARIES}/fmt-3.0.1)
    set(FMT_LINK_DIR ${LIBRARIES}/fmt-3.0.1/fmt)
else ()
    set(FMT_LIBRARIES fmt.lib)
    set(FMT_INCLUDE_DIR ${LIBRARIES}/fmt-3.0.1)
    set(FMT_LINK_DIR ${LIBRARIES}/fmt-3.0.1/builds/fmt/Release)
    #set(FMT_LINK_DIR ${LIBRARIES}/fmt-3.0.1/builds/fmt/Debug)
endif ()

set(SOURCE_FILES
    main.cpp
    emu.cpp
    ${INTERFACE_DIR}/shim.cpp
    ${EFFECTS_DIR}/color_fade.cpp
    ${EFFECTS_DIR}/emergency.cpp
    ${EFFECTS_DIR}/hsv_scroll.cpp
    ${EFFECTS_DIR}/shimmer.cpp
    ${EFFECTS_DIR}/smoothrunners.cpp
    ${EFFECTS_DIR}/util.cpp
    ${EFFECTS_DIR}/xmas_twinkles.cpp
    glad.c
)

include_directories(
    ${GLFW_INCLUDE_DIRS}
    ${FMT_INCLUDE_DIR}
    ../../interface/
    ../../effects/
    .
)

link_directories(
    ${GLFW_LINK_DIRS}
    ${FMT_LINK_DIR}
)

link_libraries(
    ${GLFW_LIBRARIES}
    ${FMT_LIBRARIES}
    -ldl
)

add_executable(emulator ${SOURCE_FILES})
