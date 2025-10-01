// Copyright 2016-2025 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "variant-main.t.hpp"

#define variant_PRESENT( x ) \
    std::cout << #x << ": " << x << "\n"

#define variant_ABSENT( x ) \
    std::cout << #x << ": (undefined)\n"

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

CASE( "variant-lite version" "[.variant][.version]" )
{
    variant_PRESENT( variant_lite_MAJOR );
    variant_PRESENT( variant_lite_MINOR );
    variant_PRESENT( variant_lite_PATCH );
    variant_PRESENT( variant_lite_VERSION );
}

CASE( "variant configuration" "[.variant][.config]" )
{
    variant_PRESENT( variant_HAVE_STD_VARIANT );
    variant_PRESENT( variant_USES_STD_VARIANT );
    variant_PRESENT( variant_VARIANT_DEFAULT );
    variant_PRESENT( variant_VARIANT_NONSTD );
    variant_PRESENT( variant_VARIANT_STD );
    variant_PRESENT( variant_CONFIG_SELECT_VARIANT );
    variant_PRESENT( variant_CONFIG_NO_EXCEPTIONS );
#if !variant_USES_STD_VARIANT
    variant_PRESENT( variant_CONFIG_MAX_TYPE_COUNT );
    variant_PRESENT( variant_CONFIG_MAX_VISITOR_ARG_COUNT );
#endif
    variant_PRESENT( variant_CPLUSPLUS );
}

CASE( "__cplusplus" "[.stdc++]" )
{
    variant_PRESENT( __cplusplus );
}

CASE( "Compiler version" "[.compiler]" )
{
#if variant_USES_STD_VARIANT
    std::cout << "(Compiler version not available: using std::variant)\n";
#else
    variant_PRESENT( variant_COMPILER_CLANG_VERSION );
    variant_PRESENT( variant_COMPILER_GNUC_VERSION );
    variant_PRESENT( variant_COMPILER_MSVC_VERSION );
#endif
}

CASE( "Presence of C++ language features" "[.stdlanguage]" )
{
#if variant_USES_STD_VARIANT
    std::cout << "(Presence of C++ language features not available: using std::variant)\n";
#else
    variant_PRESENT( variant_HAVE_CONSTEXPR_11 );
    variant_PRESENT( variant_HAVE_CONSTEXPR_14 );
    variant_PRESENT( variant_HAVE_NOEXCEPT );
    variant_PRESENT( variant_HAVE_NULLPTR );
    variant_PRESENT( variant_HAVE_OVERRIDE );
#endif
}

CASE( "Presence of C++ library features" "[.stdlibrary]" )
{
#if variant_USES_STD_VARIANT
    std::cout << "(Presence of C++ library features not available: using std::variant)\n";
#else
    variant_PRESENT( variant_HAVE_CONDITIONAL );
    variant_PRESENT( variant_HAVE_REMOVE_CV );
    variant_PRESENT( variant_HAVE_ENABLE_IF );
    variant_PRESENT( variant_HAVE_IS_SAME );
    variant_PRESENT( variant_HAVE_TYPE_TRAITS );
    variant_PRESENT( variant_HAVE_TR1_TYPE_TRAITS );
    variant_PRESENT( variant_HAVE_TR1_ADD_POINTER );
    variant_PRESENT( variant_HAVE_STD_ADD_POINTER );
    variant_PRESENT( variant_HAS_CPP0X );
#endif

#if defined _HAS_CPP0X
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
g++            -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++98 -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++03 -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++0x -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++11 -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++14 -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass
g++ -std=c++17 -I../include/nonstd -o variant-main.t.exe variant-main.t.cpp && variant-main.t.exe --pass

cl -EHsc -I../include/nonstd variant-main.t.cpp && variant-main.t.exe --pass
#endif

// end of file
