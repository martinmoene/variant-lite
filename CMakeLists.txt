# Copyright 2016-2018 by Martin Moene
#
# https://github.com/martinmoene/variant-lite
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required( VERSION 3.0 )

# At default, enable building and performing of tests and building of examples:

option( VARIANT_LITE_OPT_BUILD_TESTS    "Build and perform variant-lite tests" ON )
option( VARIANT_LITE_OPT_BUILD_EXAMPLES "Build variant-lite examples" OFF )

project( variant_lite )

set( package_name "variant-lite" )

# Interface library:

add_library(                
    ${package_name} INTERFACE )
    
target_include_directories( 
    ${package_name} INTERFACE "$<BUILD_INTERFACE:${include_source_dir}>" )

enable_testing()

if ( VARIANT_LITE_OPT_BUILD_TESTS )
    add_subdirectory( test )
endif()

if ( VARIANT_LITE_OPT_BUILD_EXAMPLES )
    add_subdirectory( example )
endif()

# end of file
