#
# Copyright 2020 Toyota Connected North America
# @copyright Copyright (c) 2022 Woven Alpha, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# repo tags

if (NOT CMAKE_APPS_MODULE_TAG)
    set(CMAKE_APPS_MODULE_TAG master)
endif ()

# determine if CI or local build

if (DEFINED ENV{CI})
    MESSAGE(STATUS "Build Type ............. CI - $ENV{CI_JOB_NAME}")
    set(BUILD_TYPE_LOCAL OFF)
    set(BUILD_TYPE_CI ON)
else ()
    MESSAGE(STATUS "Build Type ............. LOCAL")
    set(BUILD_TYPE_CI OFF)
    set(BUILD_TYPE_LOCAL ON)
endif ()

#
# variables
#
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third_party)

#
# LTO
#
option(ENABLE_LTO "Enable Link Time optimization" OFF)

#
# DLT
#
option(ENABLE_DLT "Enable DLT logging" ON)
if (ENABLE_DLT)
    add_compile_definitions(ENABLE_DLT)
endif ()

#
# backend selection
#
option(BUILD_BACKEND_WAYLAND_EGL "Build Backend for EGL" ON)
if (BUILD_BACKEND_WAYLAND_EGL)
    add_compile_definitions(BUILD_BACKEND_WAYLAND_EGL)
    option(BUILD_EGL_TRANSPARENCY "Build with EGL Transparency Enabled" ON)
    if (BUILD_EGL_TRANSPARENCY)
        add_compile_definitions(BUILD_EGL_ENABLE_TRANSPARENCY)
    endif ()
    option(BUILD_EGL_ENABLE_3D "Build with EGL Stencil, Depth, and Stencil config Enabled" ON)
    if (BUILD_EGL_ENABLE_3D)
        add_compile_definitions(BUILD_EGL_ENABLE_3D)
    endif ()
    option(BUILD_EGL_ENABLE_MULTISAMPLE "Build with EGL Sample set to 4" OFF)
    if (BUILD_EGL_ENABLE_MULTISAMPLE)
        add_compile_definitions(BUILD_EGL_ENABLE_MULTISAMPLE)
    endif ()
else ()
    option(BUILD_BACKEND_WAYLAND_VULKAN "Build Backend for Vulkan" ON)
    if (BUILD_BACKEND_WAYLAND_VULKAN)
        add_compile_definitions(BUILD_BACKEND_WAYLAND_VULKAN)
    endif ()
endif ()

option(BUILD_BACKEND_WAYLAND_DRM "Build Backend Wayland DRM" OFF)
if (BUILD_BACKEND_WAYLAND_DRM)
    add_compile_definitions(BUILD_BACKEND_WAYLAND_DRM)
endif ()

option(BUILD_BACKEND_HEADLESS "Build Headless Backend" OFF)
if (BUILD_BACKEND_HEADLESS)
    find_package(PkgConfig)
    pkg_check_modules(OSMESA osmesa glesv2 egl IMPORTED_TARGET REQUIRED)
    add_compile_definitions(BUILD_BACKEND_HEADLESS)
endif ()

option(DEBUG_PLATFORM_MESSAGES "Debug platform messages" OFF)
if (DEBUG_PLATFORM_MESSAGES)
    add_compile_definitions(DEBUG_PLATFORM_MESSAGES)
endif ()

#
# Crash Handler
#
option(BUILD_CRASH_HANDLER "Build Crash Handler" OFF)
if (BUILD_CRASH_HANDLER)
    if (NOT EXISTS ${SENTRY_NATIVE_LIBDIR}/cmake/sentry/sentry-config.cmake)
        message(FATAL_ERROR "${SENTRY_NATIVE_LIBDIR}/cmake/sentry/sentry-config.cmake does not exist")
    endif ()
    set(sentry_DIR ${SENTRY_NATIVE_LIBDIR}/cmake/sentry)
    find_package(sentry REQUIRED)
    find_package(PkgConfig)
    pkg_check_modules(UNWIND REQUIRED IMPORTED_TARGET libunwind)
    add_compile_definitions(
            BUILD_CRASH_HANDLER
            "CRASH_HANDLER_DSN=\"${CRASH_HANDLER_DSN}\""
            "CRASH_HANDLER_RELEASE=\"${PROJECT_NAME}@${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}\"")
else ()
    add_compile_definitions(
            "CRASH_HANDLER_DSN=\"\""
            "CRASH_HANDLER_RELEASE=\"\"")
endif ()

#
# Docs
#
option(BUILD_DOCS "Build documentation" ON)
MESSAGE(STATUS "Build Documentation .... ${BUILD_DOCS}")

#
# Unit Tests
#
option(BUILD_UNIT_TESTS "Build Unit Tests" OFF)
MESSAGE(STATUS "Build Unit Tests ....... ${BUILD_UNIT_TESTS}")
option(UNIT_TEST_SAVE_GOLDENS "Generate Golden Images" OFF)
MESSAGE(STATUS "Generate Golden Images.. ${UNIT_TEST_SAVE_GOLDENS}")

#
# Sanitizers
#
find_package(Sanitizers)

#
# Executable Name
#
if(NOT EXE_OUTPUT_NAME)
    set(EXE_OUTPUT_NAME "homescreen")
endif ()
add_definitions("-DEXE_OUTPUT_NAME=\"${EXE_OUTPUT_NAME}\"")
