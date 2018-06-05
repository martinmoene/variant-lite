// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "variant-lite.t.h"

#define variant_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define variant_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

lest::tests & specification() 
{ 
    static lest::tests tests; 
    return tests; 
}

CASE( "__cplusplus" "[.stdc++]" )
{
    variant_PRESENT( __cplusplus );
}

CASE( "variant-lite version" "[.version]" )
{
    variant_PRESENT( variant_lite_VERSION );
}

CASE( "compiler version" "[.compiler]" )
{
#if variant_COMPILER_GNUC_VERSION 
    variant_PRESENT( variant_COMPILER_GNUC_VERSION );
#else
    variant_ABSENT(  variant_COMPILER_GNUC_VERSION );
#endif

#if variant_COMPILER_MSVC_VERSION 
    variant_PRESENT( variant_COMPILER_MSVC_VERSION );
#else
    variant_ABSENT(  variant_COMPILER_MSVC_VERSION );
#endif
}

CASE( "Presence of C++ language features" "[.stdlanguage]" )
{
#if variant_HAVE_AUTO 
    variant_PRESENT( variant_HAVE_AUTO );
#else    
    variant_ABSENT(  variant_HAVE_AUTO );
#endif

#if variant_HAVE_NULLPTR 
    variant_PRESENT( variant_HAVE_NULLPTR );
#else    
    variant_ABSENT(  variant_HAVE_NULLPTR );
#endif

#if variant_HAVE_STATIC_ASSERT
    variant_PRESENT( variant_HAVE_STATIC_ASSERT );
#else    
    variant_ABSENT(  variant_HAVE_STATIC_ASSERT );
#endif

#if variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG 
    variant_PRESENT( variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#else    
    variant_ABSENT(  variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG );
#endif

#if variant_HAVE_ALIAS_TEMPLATE 
    variant_PRESENT( variant_HAVE_ALIAS_TEMPLATE );
#else    
    variant_ABSENT(  variant_HAVE_ALIAS_TEMPLATE );
#endif

#if variant_HAVE_CONSTEXPR_11 
    variant_PRESENT( variant_HAVE_CONSTEXPR_11 );
#else    
    variant_ABSENT(  variant_HAVE_CONSTEXPR_11 );
#endif

#if variant_HAVE_CONSTEXPR_14
    variant_PRESENT( variant_HAVE_CONSTEXPR_14 );
#else    
    variant_ABSENT(  variant_HAVE_CONSTEXPR_14 );
#endif

#if variant_HAVE_ENUM_CLASS 
    variant_PRESENT( variant_HAVE_ENUM_CLASS );
#else    
    variant_ABSENT(  variant_HAVE_ENUM_CLASS );
#endif

#if variant_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE 
    variant_PRESENT( variant_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#else    
    variant_ABSENT(  variant_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE );
#endif

#if variant_HAVE_EXPLICIT_CONVERSION 
    variant_PRESENT( variant_HAVE_EXPLICIT_CONVERSION );
#else    
    variant_ABSENT(  variant_HAVE_EXPLICIT_CONVERSION );
#endif

#if variant_HAVE_INITIALIZER_LIST 
    variant_PRESENT( variant_HAVE_INITIALIZER_LIST );
#else    
    variant_ABSENT(  variant_HAVE_INITIALIZER_LIST );
#endif

#if variant_HAVE_IS_DEFAULT 
    variant_PRESENT( variant_HAVE_IS_DEFAULT );
#else    
    variant_ABSENT(  variant_HAVE_IS_DEFAULT );
#endif

#if variant_HAVE_IS_DELETE 
    variant_PRESENT( variant_HAVE_IS_DELETE );
#else    
    variant_ABSENT(  variant_HAVE_IS_DELETE );
#endif

#if variant_HAVE_NOEXCEPT 
    variant_PRESENT( variant_HAVE_NOEXCEPT );
#else    
    variant_ABSENT(  variant_HAVE_NOEXCEPT );
#endif
}

CASE( "Presence of C++ library features" "[.stdlibrary]" )
{
#if variant_HAVE_ARRAY
    variant_PRESENT( variant_HAVE_ARRAY );
#else    
    variant_ABSENT(  variant_HAVE_ARRAY );
#endif

#if variant_HAVE_CONDITIONAL
    variant_PRESENT( variant_HAVE_CONDITIONAL );
#else    
    variant_ABSENT(  variant_HAVE_CONDITIONAL );
#endif

#if variant_HAVE_CONTAINER_DATA_METHOD
    variant_PRESENT( variant_HAVE_CONTAINER_DATA_METHOD );
#else    
    variant_ABSENT(  variant_HAVE_CONTAINER_DATA_METHOD );
#endif

#if variant_HAVE_REMOVE_CV
    variant_PRESENT( variant_HAVE_REMOVE_CV );
#else    
    variant_ABSENT(  variant_HAVE_REMOVE_CV );
#endif

#if variant_HAVE_SIZED_TYPES
    variant_PRESENT( variant_HAVE_SIZED_TYPES );
#else    
    variant_ABSENT(  variant_HAVE_SIZED_TYPES );
#endif

#if variant_HAVE_TYPE_TRAITS
    variant_PRESENT( variant_HAVE_TYPE_TRAITS );
#else    
    variant_ABSENT(  variant_HAVE_TYPE_TRAITS );
#endif

#if _HAS_CPP0X
    variant_PRESENT( _HAS_CPP0X );
#else    
    variant_ABSENT(  _HAS_CPP0X );
#endif
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

#if 0
g++            -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++98 -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++03 -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++0x -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++11 -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++14 -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass
g++ -std=c++17 -I../include/nonstd -o variant-lite.t.exe variant-lite.t.cpp && variant-lite.t.exe --pass

cl -EHsc -I../include/nonstd variant-lite.t.cpp && variant-lite.t.exe --pass
#endif

// end of file
