# Copyright 2016-2018 by Martin Moene
#
# https://github.com/martinmoene/variant-lite
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.5 )

# variant-lite project and version, updated by script/update-version.py:

project( variant_lite LANGUAGES CXX )

set( variant_lite_version "1.1.0" )

# At default, enable building and performing of tests and b# Toplevel or subproject:

if ( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    set( variant_IS_TOPLEVEL_PROJECT TRUE )
else()
    set( variant_IS_TOPLEVEL_PROJECT FALSE )
endif()

# At default, enable building and performing of tests and building of examples:

option( VARIANT_LITE_OPT_BUILD_TESTS    "Build and perform variant-lite tests" ${variant_IS_TOPLEVEL_PROJECT} )
option( VARIANT_LITE_OPT_BUILD_EXAMPLES "Build variant-lite examples" OFF )

option( VARIANT_LITE_OPT_SELECT_STD     "Select std::variant"    OFF )
option( VARIANT_LITE_OPT_SELECT_NONSTD  "Select nonstd::variant" OFF )

include( GNUInstallDirs )

set( package_name "variant-lite" )
set( include_source_dir "${PROJECT_SOURCE_DIR}/include" )

# Interface library:

add_library(
    ${package_name} INTERFACE )

target_include_directories(
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>" )

# Installation:

install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

# If requested, build and perform tests, build examples:

enable_testing()

if ( VARIANT_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( VARIANT_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
