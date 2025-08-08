cmake_minimum_required(VERSION 3.24)

project( tp_raytracing LANGUAGES CXX VERSION 2026.1.0)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED_LIBS "Build shared libs" ON)
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

#########################################################
# SET COMPILATION FLAGS FOR C++17
#########################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(BUILD_SHARED_LIBS)
    if(WIN32)
        # export all symbols on a windows build of dynamic libs
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS  ON)
    endif()
endif()

# if no build type is set, default to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

set(CMAKE_DEBUG_POSTFIX d)

#include(CheckIPOSupported)
#check_ipo_supported(RESULT result)
#message(STATUS "IPO supported: ${result}")
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ${result})

# compile options for all targets depending on the compiler and the system
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(MY_COMPILE_OPTIONS "-Wall;-Wextra;-pedantic;-Wno-comment;-Wshadow;-Wnon-virtual-dtor;-Wpedantic;-Wconversion;-Wmisleading-indentation;-Wsign-conversion;-Wfloat-equal")
    if(ENABLE_WARNINGS_AS_ERRORS)
        list(APPEND MY_COMPILE_OPTIONS "-Werror")
    endif()
elseif(MSVC)
    set(MY_COMPILE_OPTIONS "/W4;/permissive-")
    if(ENABLE_WARNINGS_AS_ERRORS)
        list(APPEND MY_COMPILE_OPTIONS "/WX")
    endif()
endif()
if(APPLE)
    list(APPEND MY_COMPILE_OPTIONS "-Wno-deprecated-declarations")
endif()

# compile definitions for all targets depending on the compiler and the system
set(MY_COMPILE_DEFINITIONS "")
if(MSVC)
    list(APPEND MY_COMPILE_DEFINITIONS "-DNOMINMAX;-D_USE_MATH_DEFINES")
endif()

#########################################################
#
# EXTERNAL LIBRARIES
#
#########################################################

#########################################################
# FIND OPENMP
#########################################################
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found")
    list(APPEND MY_COMPILE_DEFINITIONS "TRACER_WITH_OPENMP")
else()
    message(STATUS "OpenMP not found, parallelization will not be used")
endif()

#########################################################
# FIND glm
#########################################################

include(FetchContent)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    # this allow to first check if find_package(glm) can find it
    FIND_PACKAGE_ARGS CONFIG
)
FetchContent_MakeAvailable(glm)

#########################################################
# FIND OPENGL
#########################################################
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)
message(STATUS "OPENGL_gl_LIBRARY: ${OPENGL_gl_LIBRARY}")

#########################################################
# FIND GLUT
#########################################################
if(MSVC)
    set(GLUT_ROOT_PATH "${CMAKE_SOURCE_DIR}/freeglut")
    message(STATUS "GLUT_ROOT_PATH: ${GLUT_ROOT_PATH}")
endif()
find_package(GLUT REQUIRED)
message(STATUS "GLUT_FOUND: ${GLUT_FOUND}")
message(STATUS "GLUT_INCLUDE_DIR: ${GLUT_INCLUDE_DIR}")
message(STATUS "GLUT_LIBRARIES: ${GLUT_LIBRARIES}")

#########################################################
# FIND IMGUI
#########################################################
set(IMGUI_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/)
set(IMGUI_DIR ${IMGUI_INCLUDE_DIR}/imgui)
FetchContent_Populate(imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
    SOURCE_DIR ${IMGUI_DIR}
)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glut.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl2.cpp
)

add_library(imgui ${IMGUI_SOURCES})
target_include_directories(
    imgui
    PUBLIC
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>
        # this because internally they do not prefix the include with imgui/
        $<BUILD_INTERFACE:${IMGUI_INCLUDE_DIR}>/imgui
        $<INSTALL_INTERFACE:include>
)
target_compile_definitions(imgui PUBLIC IMGUI_DISABLE_DEMO_WINDOWS)
target_link_libraries(imgui PUBLIC OpenGL::GL OpenGL::GLU GLUT::GLUT)

#########################################################
# BUILD RAYTRACER LIBRARIES AND APPLICATION
#########################################################
set(RAYTRACER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/)
set(RAYTRACER_CORE_SOURCES
        src/raytracing/core/Camera.cpp
        src/raytracing/core/Scene.cpp
        src/raytracing/core/hittableObject/Sphere.cpp)

add_library(core ${RAYTRACER_CORE_SOURCES})
target_include_directories(core PUBLIC $<BUILD_INTERFACE:${RAYTRACER_INCLUDE_DIR}>)
target_link_libraries(core glm::glm)
target_compile_options(core PRIVATE ${MY_COMPILE_OPTIONS})
target_compile_definitions(core PUBLIC ${MY_COMPILE_DEFINITIONS})

set(RAYTRACER_CONTROLLER_SOURCES
        src/raytracing/controller/CameraFactory.cpp
        src/raytracing/controller/ImageWrapper.cpp
        src/raytracing/controller/Renderer.cpp
        src/raytracing/controller/SceneFactory.cpp)

add_library(controller ${RAYTRACER_CONTROLLER_SOURCES})
target_include_directories(controller PUBLIC $<BUILD_INTERFACE:${RAYTRACER_INCLUDE_DIR}>)
target_link_libraries(controller core glm::glm imgui OpenGL::GL OpenGL::GLU GLUT::GLUT)
if(OpenMP_CXX_FOUND)
    target_link_libraries(controller OpenMP::OpenMP_CXX)
endif()
target_compile_options(controller PRIVATE ${MY_COMPILE_OPTIONS})
target_compile_definitions(controller PUBLIC ${MY_COMPILE_DEFINITIONS})

set(RAYTRACER_GUI_SOURCES src/raytracing/gui/App.cpp)
add_library(gui ${RAYTRACER_GUI_SOURCES})
target_include_directories(gui PUBLIC $<BUILD_INTERFACE:${RAYTRACER_INCLUDE_DIR}>)
target_link_libraries(gui core controller glm::glm imgui)
target_compile_options(gui PRIVATE ${MY_COMPILE_OPTIONS})
target_compile_definitions(gui PUBLIC ${MY_COMPILE_DEFINITIONS})


add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer gui controller core imgui OpenGL::GL OpenGL::GLU GLUT::GLUT)
target_compile_options(raytracer PRIVATE ${MY_COMPILE_OPTIONS})
target_compile_definitions(raytracer PUBLIC ${MY_COMPILE_DEFINITIONS})

