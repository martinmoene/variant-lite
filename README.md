# variant lite: A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++11 and later

[![Language](https://img.shields.io/badge/C%2B%2B-98/11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-BSL-blue.svg)](https://opensource.org/licenses/BSL-1.0) [![Build Status](https://travis-ci.org/martinmoene/variant-lite.svg?branch=master)](https://travis-ci.org/martinmoene/variant-lite) [![Build status](https://ci.appveyor.com/api/projects/status/w2dgn3fxyrd6vcq8?svg=true)](https://ci.appveyor.com/project/martinmoene/variant-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fvariant-lite.svg)](https://github.com/martinmoene/variant-lite/releases) [![Latest download](https://img.shields.io/badge/latest-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/variant-lite/master/include/nonstd/variant.hpp) [![Conan](https://img.shields.io/badge/on-conan-blue.svg)](https://bintray.com/martinmoene/nonstd-lite/variant-lite%3Anonstd-lite/_latestVersion) [![Try it online](https://img.shields.io/badge/on-wandbox-blue.svg)](https://wandbox.org/permlink/tpG9nmapo2mUKUCo)

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Synopsis](#synopsis)
- [Features](#features)
- [Reported to work with](#reported-to-work-with)
- [Building the tests](#building-the-tests)
- [Other implementations of variant](#other-implementations-of-variant)
- [Notes and references](#notes-and-references)
- [Appendix](#appendix)


Example usage
-------------

```Cpp
#include "nonstd/variant.hpp"

#include <cassert>
#include <string>

using namespace nonstd;

int main()
{
    std::string hello = "hello, world"; 

    variant< char, int, long, std::string > var;
    
    var =  'v' ; assert( get<  0 >( var ) == 'v' );
                 assert( get<char>( var ) == 'v' );
    var =   7  ; assert( get<int >( var ) ==  7  );
    var =  42L ; assert( get<long>( var ) == 42L );    
    var = hello; assert( get<std::string>( var ) == hello );
}
```

### Compile and run

```
prompt>g++ -std=c++98 -Wall -I../include -o 01-basic.exe 01-basic.cpp && 01-basic.exe
```

In a nutshell
-------------
**variant lite** is a single-file header-only library to represent a type-safe union. The library aims to provide a [C++17-like variant](http://en.cppreference.com/w/cpp/utility/variant) for use with C++98 and later. If available, std::variant is used. 

**Features and properties of variant lite** are ease of installation (single header), freedom of dependencies other than the standard library and control over object alignment (if needed).  *variant lite* shares the approach to in-place tags with [any-lite](https://github.com/martinmoene/any-lite), [expected-lite](https://github.com/martinmoene/expected-lite)  and with [optional-lite](https://github.com/martinmoene/optional-lite) and these libraries can be used together.

**Limitations of variant lite** are the requirement for the alternative types to be of different types and the limit on the number of alternative types and the number of visitor arguments. The maximum number of types and visitor arguments are configurable via [script generate_header.py](script/generate_header.py) (default: 16 types, 5 visitor arguments). Move construction, move assignment and emplacement require C++11 and are not supported when compiling under C++98. *variant lite* does not provide allocator-extended constructors.


License
-------
*variant lite* is distributed under the [Boost Software License](LICENSE.txt).
 

Dependencies
------------
*variant lite* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation
------------
*variant lite* is a single-file header-only library. Put `variant.hpp` in the [include](include) folder directly into the project source tree or somewhere reachable from your project.

Or, if you use the [conan package manager](https://www.conan.io/), follow these steps:

1. Add *nonstd-lite* to the conan remotes:

        conan remote add nonstd-lite https://api.bintray.com/conan/martinmoene/nonstd-lite

2. Add a reference to *variant-lite* to the *requires* section of your project's `conanfile.txt` file:

        [requires]
        variant-lite/[~=0]@nonstd-lite/testing

3. Run conan's install command:

        conan install


Synopsis
--------

**Contents**  
- [Types in namespace nonstd](#types-in-namespace-nonstd)  
- [Interface of *variant lite*](#interface-of-variant-lite)  
- [Algorithms for *variant lite*](#algorithms-for-variant-lite)  
- [Information macros](#information-macros)
- [Configuration macros](#configuration-macros)
- [Macros to control alignment](#macros-to-control-alignment)  

### Types in namespace nonstd

| Purpose               | Type | Notes |
|-----------------------|------|-------|
| Type-safe union       | template&lt; class T0...T6 ><br>class **variant** | non-standard: may hold up to seven types |
| Default constructible | class **monostate**                  |Type to make variant default constructible |
| Error reporting       | class **bad_variant_access**         |&nbsp; |
| In-place construction | struct **in_place_tag**              |&nbsp;             |
| &nbsp;                | **in_place**                         | select type or index for in-place construction |
| &nbsp;                | **in_place_type**                    | select type for in-place construction |
| &nbsp;                | **in_place_index**                   | select index for in-place construction |
| &nbsp;                | **nonstd_lite_in_place_type_t**( T)  | macro for alias template in_place_type_t&lt;T>  |
| &nbsp;                | **nonstd_lite_in_place_index_t**( T )| macro for alias template in_place_index_t&lt;T> |
| Variant size          | template&lt;...><br>struct **variant_size**&lt; variant&lt;...> > | &nbsp;|
| &nbsp;                | **variant_size_v**&lt; T >           | C++14 |
| &nbsp;                | **variant_size_V**( T )              | macro for nonstd::variant_size&lt;T>::value |
| Select variant type   | template&lt; std::size_t I, ...><br>struct **variant_alternative**&lt; I, variant&lt;...> > | &nbsp; |
| &nbsp;                | **variant_alternative_t**&lt; I, T > | C++11 |
| &nbsp;                | **variant_alternative_T**( I, T )    | macro for typename nonstd::variant_alternative&lt;I,T >::type |

### Interface of *variant lite*

| Kind         | Std  | Method                                       | Result |
|--------------|------|----------------------------------------------|--------|
| Construction |&nbsp;| **variant**()                                    | default-construct first type      |
| &nbsp;       |&nbsp;| **variant**( Tx const & x )                      | copy-construct with value type Tx |
| &nbsp;       |C++11 | **variant**( Tx && x )                           | move-construct with value type Tx |
| &nbsp;       |&nbsp;| **variant**( variant const & rhs )               | copy-construct from other variant |
| &nbsp;       |C++11 | **variant**( variant && rhs )                    | move-construct from other variant |
| &nbsp;       |C++11 | template&lt; class T, class... Args ><br>explicit **variant**( in_place_type_t(T), Args&&... args) | in-place-construct type T |
| &nbsp;       |C++11 | template&lt; class T, class U, class... Args ><br>explicit **variant**( in_place_type_t(T),<br>&emsp;std::initializer_list&lt;U> il, Args&&... args ) | in-place-construct type T|
| &nbsp;       |C++11 | template&lt; std::size_t I, class... Args ><br>explicit **variant**( in_place_index_t(I), Args&&... args ) | in-place-construct type at index I | 
| &nbsp;       |C++11 | template&lt; size_t I, class U, class... Args><br>explicit **variant**( in_place_index_t(I),<br>&emsp;std::initializer_list&lt;U> il, Args&&... args ) | in-place-construct type at index I|
| Destruction  |&nbsp;| **~variant**()                                   | destruct current content |
| Assignment   |&nbsp;| variant & **operator=**( variant const & rhs )   | copy-assign from other |
| &nbsp;       |C++11 | variant & **operator=**( variant && rhs )        | move-assign from other |
| &nbsp;       |C++11 | template&lt; class Tx ><br>variant & **operator=**( Tx && t ) | move-assign from value |
| &nbsp;       |< C++11 | template&lt; class Tx ><br>variant & **operator=**( Tx const & t ) | copy-assign from value;<br>non-standard |
| State        |&nbsp;| std::size_t **index**() const                    | index of current content's type |
| &nbsp;       |&nbsp;| bool **valueless_by_exception**() const          | true if no content is present |
| Emplace      |C++11 | template&lt; class T, class... Args ><br>T & **emplace**( Args&&... args ) | emplace type T |
| &nbsp;       |C++11 | template&lt; class T, class U, class... Args ><br>T & **emplace**( std::initializer_list&lt;U> il, Args&&... args ) | emplace type T |
| &nbsp;       |C++11 | template&lt; size_t I, class... Args ><br>variant_alternative_t&lt;I,variant> &<br>**emplace**( Args&&... args ); | emplace type at index I |
| &nbsp;       |C++11 | template&lt; size_t I, class U, class... Args ><br>variant_alternative_t&lt;I,variant> &<br>**emplace**( std::initializer_list&lt;U> il, Args&&... args ) | emplace type at index I |
| Swap         |&nbsp;| void **swap**( variant & other );                | swap with other |
 

### Algorithms for *variant lite*

| Kind                        | Std  | Function |
|-----------------------------|------|----------|
| **Relational operators**    |&nbsp;| &nbsp;   |
| ==                          |&nbsp;| template<...><br>bool **operator==**( variant<...> const & v, variant&lt;...> const & w ) |
| !=                          |&nbsp;| template<...><br>bool **operator==**( variant<...> const & v, variant&lt;...> const & w ) |
| <                           |&nbsp;| template<...><br>bool **operator<**( variant<...> const & v, variant<...> const & w ) |
| >                           |&nbsp;| template<...><br>bool **operator>**( variant<...> const & v, variant<...> const & w ) |
| <=                          |&nbsp;| template<...><br>bool **operator<=**( variant<...> const & v, variant<...> const & w ) |
| >=                          |&nbsp;| template<...><br>bool **operator>=**( variant<...> const & v, variant<...> const & w ) |
| **Content**                 |&nbsp;| &nbsp;   |
| contains value of type T    |&nbsp;| template< class T, ...><br>bool **holds_alternative**( variant<...> const & v ) [noexcept] |
| get by type                 |&nbsp;| template< class R, ...><br>R &<br>**get**( variant<...> & v, in_place_type_t(R) = in_place<R> ) |
| get by type (rvalue)        |C++11 | template< class R, ...><br>R &&<br>**get**( variant<...> && v, in_place_type_t(R) = in_place<R> ) |
| get by type (const)         |&nbsp;| template< class R, ...><br>R const &<br>**get**( variant<...> const & v, in_place_type_t(R) = in_place<R> ) |
| get by type (const rvalue)  |C++11 | template< class R, ...><br>R const &&<br>**get**( variant<...> const && v, in_place_type_t(R) = in_place<R> ) |
| get by index                |&nbsp;| template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type &<br>**get**( variant<...> & v, in_place_index_t(I) = in_place<I> ) |
| get by index (rvalue)       |C++11 | template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type &&<br>**get**( variant<...> && v, in_place_index_t(I) = in_place<I> ) |
| get by index (const)        |&nbsp;| template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type const &<br>**get**( variant<...> const & v, in_place_index_t(I) = in_place<I> ) |
| get by index (const rvalue) |C++11 | template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type const &&<br>**get**( variant<...> const && v, in_place_index_t(I) = in_place<I> ) |
| get_if by type              |&nbsp;| template< class T, ...><br>typename detail::add_pointer&lt;T>::type<br>**get_if**( variant<...> * pv, in_place_type_t(T) = in_place<T> ) |
| get_if by type (const)      |&nbsp;| template< class T, ...><br>typename detail::add_pointer&lt;const T>::type<br>**get_if**( variant<...> const * pv, in_place_type_t(T) = in_place<T>) |
| get_if by index             |&nbsp;| template< std::size_t I, ...><br>typename detail::add_pointer< typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type<br>**get_if**( variant<...> * pv, in_place_index_t(I) = in_place<I> ) |
| get_if by index (const)     |&nbsp;| template< std::size_t I, ...><br>typename detail::add_pointer< const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type<br>**get_if**( variant<...> const * pv, in_place_index_t(I) = in_place<I> ) |
| swap                        |&nbsp;| template<...><br>void **swap**( variant<...> & x, variant<...> & y ) |
| visit                         |Note&nbsp;1| template< class Visitor, class Variant ><br>Variant **visit**( Visitor const & vis, Variant const & v ) |
| **Hash support**            |&nbsp;| &nbsp;   |
| variant                     |C++11 | template<...> struct **hash**< variant<...> >; |
| monostate                   |C++11 | template<> struct **hash**< monostate >;       |

Note 1: visitor is limited to always return a Variant.

### Information macros

<b>variant_CONFIG_MAX_TYPE_COUNT</b>  
The maximum number of types thevariant can hold as configured via script [generate_header.py](script/generate_header.py).

<b>variant_CONFIG_MAX_VISITOR_ARG_COUNT</b>  
The maximum number of visitor arguments as configured via script [generate_header.py](script/generate_header.py).

### Configuration macros

#### Standard selection macro
\-D<b>variant\_CPLUSPLUS</b>=199711L  
Define this macro to override the auto-detection of the supported C++ standard, or if your compiler does not set the `__cplusplus` macro correctly.

#### Select `std::variant` or `nonstd::variant`
At default, *variant lite* uses `std::variant` if it is available and lets you use it via namespace `nonstd`. You can however override this default and explicitly request to use `std::variant` or variant lite's `nonstd::variant` as `nonstd::variant` via the following macros.

\-D<b>variant\_CONFIG\_SELECT\_VARIANT</b>=variant_VARIANT_DEFAULT  
Define this to `variant_VARIANT_STD` to select `std::variant` as `nonstd::variant`. Define this to `variant_VARIANT_NONSTD` to select `nonstd::variant` as `nonstd::variant`. Default is undefined, which has the same effect as defining to `variant_VARIANT_DEFAULT`.

#### Disable exceptions
-D<b>variant_CONFIG_NO_EXCEPTIONS</b>=0  
Define this to 1 if you want to compile without exceptions. If not defined, the header tries and detect if exceptions have been disabled (e.g. via `-fno-exceptions`). Default is undefined.

#### Presence of `variant_size_V()` simulation macro
\-D<b>variant\_CONFIG\_OMIT\_VARIANT\_SIZE\_V\_MACRO</b>=0  
Define this macro to 1 to omit the `variant_size_V(T)` macro. Default is 0.

#### Presence of `variant_alternative_T()` simulation macro
\-D<b>variant\_CONFIG\_OMIT\_VARIANT\_ALTERNATIVE\_T\_MACRO</b>=0  
Define this macro to 1 to omit the `variant_alternative_T(I,T)` macro. Default is 0.

#### Macros to control alignment

If *variant lite* is compiled as C++11 or later, C++11 alignment facilities are used for storage of the underlying object. When compiled as pre-C++11, *variant lite* tries to determine proper alignment itself. If this doesn't work out, you can control alignment via the following macros. See also section [Implementation notes](#implementation-notes).

\-D<b>variant\_CONFIG\_MAX\_ALIGN\_HACK</b>=0  
Define this to 1 to use the max align hack for alignment. Default is 0.

\-D<b>variant\_CONFIG\_ALIGN\_AS</b>=*pod_type*  
Define this to the pod-type you want to align to (no default).

\-D<b>variant\_CONFIG\_ALIGN\_AS\_FALLBACK</b>=*pod_type*  
Define this to the pod-type to use for alignment if the algorithm of *variant lite* cannot find a suitable POD type to use for alignment. Default is `double`.


Reported to work with
---------------------
The table below mentions the compiler versions *variant lite* is reported to work with.

OS        | Compiler   | Versions |
---------:|:-----------|:---------|
Windows   | Clang/LLVM | ? |
&nbsp;    | GCC        | 5.2.0 |
&nbsp;    | Visual C++<br>(Visual Studio)| 8 (2005), 10 (2010), 11 (2012),<br>12 (2013), 14 (2015) |
GNU/Linux | Clang/LLVM | 3.5.0 |
&nbsp;    | GCC        | 4.8.4 |
OS X      | ?          | ?   |


Building the tests
------------------
To build the tests you need:

- [CMake](http://cmake.org), version 2.8.12 or later to be installed and in your PATH.
- A [suitable compiler](#reported-to-work-with).

The [*lest* test framework](https://github.com/martinmoene/lest)  is included in the [test folder](test).

The following steps assume that the [*variant lite* source code](https://github.com/martinmoene/variant-lite) has been cloned into a directory named `c:\variant-lite`.

1. Create a directory for the build outputs for a particular architecture.
Here we use c:\variant-lite\build-win-x86-vc10.

        cd c:\variant-lite
        md build-win-x86-vc10
        cd build-win-x86-vc10

2. Configure CMake to use the compiler of your choice (run `cmake --help` for a list).

        cmake -G "Visual Studio 10 2010" ..

3. Build the test suite in the Debug configuration (alternatively use Release).    

        cmake --build . --config Debug

4. Run the test suite.    

        ctest -V -C Debug

All tests should pass, indicating your platform is supported and you are ready to use *variant lite*.


Implementation notes
--------------------

### Object allocation and alignment

*variant lite* reserves POD-type storage for an object of the underlying type inside a union to prevent unwanted construction and uses placement new to construct the object when required. Using non-placement new (malloc) to  obtain storage, ensures that the memory is properly aligned for the object's type, whereas that's not the case with placement new.

If you access data that's not properly aligned, it 1) may take longer than when it is properly aligned (on x86 processors), or 2) it may terminate the program immediately (many other processors).

Although the C++ standard does not guarantee that all user-defined types have the alignment of some POD type, in practice it's likely they do [10, part 2].

If *variant lite* is compiled as C++11 or later, C++11 alignment facilities are used for storage of the underlying object. When compiling as pre-C++11, *variant lite* tries to determine proper alignment using meta programming. If this doesn't work out, you can control alignment via three macros.

*variant lite* uses the following rules for alignment:

1. If the program compiles as C++11 or later, C++11 alignment facilities  are used.

2. If you define -D<b>variant_CONFIG_MAX_ALIGN_HACK</b>=1 the underlying type is aligned as the most restricted type in `struct max_align_t`. This potentially wastes many bytes per variant if the actually required alignment is much less, e.g. 24 bytes used instead of the 2 bytes required.

3. If you define -D<b>variant_CONFIG_ALIGN_AS</b>=*pod-type* the underlying type is aligned as *pod-type*. It's your obligation to specify a type with proper alignment.

4. If you define -D<b>variant_CONFIG_ALIGN_AS_FALLBACK</b>=*pod-type* the fallback type for alignment of rule 5 below becomes *pod-type*. It's your obligation to specify a type with proper alignment.

5. At default, *variant lite* tries to find a POD type with the same alignment as the underlying type.

	The algorithm for alignment of 5. is:
	- Determine the alignment A of the underlying type using `alignment_of<>`.
	- Find a POD type from the list `alignment_types` with exactly alignment A.
	- If no such POD type is found, use a type with a relatively strict alignment requirement such as double; this type is specified in  `variant_CONFIG_ALIGN_AS_FALLBACK` (default double).

Note that the algorithm of 5. differs from the one Andrei Alexandrescu uses in [10, part 2].

The class template `alignment_of<>` is gleaned from [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) [14].

For more information on constructed unions and alignment, see [10-14].


Other implementations of variant
--------------------------------
- Isabella Muerte. [MNMLSTC Core](https://github.com/mnmlstc/core) (C++11).
- Anthony Williams. [C++ variant class, matching the C++17 draft](https://bitbucket.org/anthonyw/variant).
- Agustín Bergé. [Eggs.Variant, a C++11/14 generic, type-safe, discriminated union](https://github.com/eggs-cpp/variant).
- Chris Beck. [strict_variant, a simpler type-safe union for C++11/C++14](https://github.com/cbeck88/strict-variant).
- Mapbox. [C++11/C++14 Variant](https://github.com/mapbox/variant).
- Michael Park. [Variant: A type-safe union (C++14)](https://github.com/mpark/variant).
- Gregor Burger. [Typesafe tagged union with "don't call us, we'll call you" feature (C++14)](https://github.com/gregorburger/variant).
- Anatoliy Tomilov. [C++1z variant, never empty, constexpr-enabled visit/multivisit, ... relational](https://github.com/tomilov/variant).


Notes and References
--------------------

### Acknowledgments

Thanks to @flexferrum for making the number of variant types and visitor arguments [configurable](#in-a-nutshell).
Thanks to @siffiejoe for contributing to fixing [lifetime](#31), [noexcept](34) and [hash](#32) issues. 


### References

[1] CppReference. [Variant](http://en.cppreference.com/w/cpp/utility/variant).  

[2] ISO/IEC WG21. [N4606, section 20.7 Variants](http://wg21.link/n4606). July 2016.

[3] Axel Naumann. [Variant: a type-safe union for C++17 (v8)](http://wg21.link/p0088r3). June 2016.

[4] Peter Dimov. [Valueless Variants Considered Harmful](http://wg21.link/p0308r0.html). March 2016.

[5] Anthony Williams. [Standardizing Variant: Difficult Decisions](https://www.justsoftwaresolutions.co.uk/cplusplus/standardizing-variant.html). June 2015

[6] Andrzej Krzemieński. [Constexpr unions](https://akrzemi1.wordpress.com/2012/12/13/constexpr-unions/). December 2012.

[7] Agustín Bergé. [Eggs.Variant - Part I](http://talesofcpp.fusionfenix.com/post-17/eggs.variant---part-i). August 2014.

[8] Agustín Bergé. [Eggs.Variant - Part II (the constexpr experience)](http://talesofcpp.fusionfenix.com/post-20/eggs.variant---part-ii-the-constexpr-experience). March 2015.

[9] Andrei Alexandrescu. [An Implementation of Discriminated Unions in C++](https://www.researchgate.net/publication/2522635_An_Implementation_of_Discriminated_Unions_in_C). August 2002.

[10] Andrei Alexandrescu. [Generic<Programming>: Discriminated Unions part 1](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2004/alexandr/alexandr.htm), [part 2](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2006/alexandr/alexandr.htm), [part 3](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2008/alexandr/alexandr.htm). April 2002.

[11] Herb Sutter. [Style Case Study #3: Construction Unions](http://www.gotw.ca/gotw/085.htm). GotW #85. 2009

[12] Kevin T. Manley. [Using Constructed Types in C++ Unions](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/0208/manley/manley.htm). C/C++ Users Journal, 20(8), August 2002.

[13] StackOverflow. [Determining maximum possible alignment in C++](http://stackoverflow.com/a/3126992).

[14] [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) ( [code](http://www.boost.org/doc/libs/1_57_0/boost/type_traits/alignment_of.hpp) ).

### Presentations
[15] Ben Deane. [Using Types Effectively](https://youtu.be/ojZbFIQSdl8?list=PLHTh1InhhwT7J5jl4vAhO1WvGHUUFgUQH). CppCon 2016.


Appendix
--------

### A.1 Compile-time information

The version of *variant lite* is available via tag `[.version]`. The following tags are available for information on the compiler and on the C++ standard library used: `[.compiler]`, `[.stdc++]`, `[.stdlanguage]` and `[.stdlibrary]`.

### A.2 Variant lite test specification

```
variant: Disallows non-default constructible as first type
variant: Allows non-default constructible as second and later type (first: int)
variant: Allows non-default constructible as second and later type (first: monostate)
variant: Allows to default-construct variant
variant: Allows to copy-construct from variant
variant: Allows to move-construct from variant (C++11)
variant: Allows to move-construct if-noexcept from variant (C++11)
variant: Allows to obtain the index of the current type
variant: Allows to inspect if variant is "valueless by exception"
variant: Allows to copy-assign from variant
variant: Allows to copy-assign mutually empty variant
variant: Allows to copy-assign from empty variant
variant: Allows to copy-assign to empty variant
variant: Allows to move-assign from variant (C++11)
variant: Allows to move-assign mutually empty variant (C++11)
variant: Allows to move-assign from empty variant (C++11)
variant: Allows to move-assign to empty variant (C++11)
variant: Allows to construct from element value
variant: Allows to copy-construct from element
variant: Allows to move-construct from element (C++11)
variant: Allows to convert-copy-construct from element
variant: Allows to convert-move-construct from element (C++11)
variant: Allows to copy-assign from element value
variant: Allows to move-assign from element value
variant: Allows to copy-assign from element
variant: Allows to move-assign from element (C++11)
variant: Allows to convert-copy-assign from element value
variant: Allows to copy-construct from elements in intializer-list based on type (C++11)
variant: Allows to move-construct from elements in intializer-list based on type (C++11)
variant: Allows to in-place copy-construct element based on type (C++11)
variant: Allows to in-place move-construct element based on type (C++11)
variant: Allows to in-place copy-construct element based on index (C++11)
variant: Allows to in-place move-construct element based on index (C++11)
variant: Allows to in-place copy-construct elements from intializer-list based on type (C++11)
variant: Allows to in-place move-construct elements from intializer-list based on type (C++11)
variant: Allows to in-place copy-construct elements from intializer-list based on index (C++11)
variant: Allows to in-place move-construct elements from intializer-list based on index (C++11)
variant: Allows to copy-emplace element based on type (C++11)
variant: Allows to move-emplace element based on type (C++11)
variant: Allows to copy-emplace element based on index (C++11)
variant: Allows to move-emplace element based on index (C++11)
variant: Allows to copy-emplace elements from intializer-list based on type (C++11)
variant: Allows to move-emplace elements from intializer-list based on type (C++11)
variant: Allows to copy-emplace elements from intializer-list based on index (C++11)
variant: Allows to move-emplace elements from intializer-list based on index (C++11)
variant: Allows to swap variants, same index (member)
variant: Allows to swap variants, different index (member)
variant: Allows to visit contents (args: 1; configured max args: 5)
variant: Allows to visit contents (args: 2; configured max args: 5)
variant: Allows to visit contents (args: 3; configured max args: 5)
variant: Allows to visit contents, rvalue reference (args: 1; configured max args: 5)
variant: Allows to check for content by type
variant: Allows to get element by type
variant: Allows to get element by index
variant: Allows to get pointer to element or NULL by type
variant: Allows to get pointer to element or NULL by index
variant: Allows to compare variants
variant: Allows to swap variants, same index (non-member)
variant: Allows to swap variants, different index (non-member)
monostate: Allows to make variant default-constructible
bad_variant_access: Indicates invalid variant access
variant_size<>: Allows to obtain number of element types (configured max types: 16)
variant_size_v<>: Allows to obtain number of element types (C++14; configured max types: 16)
variant_size_V(): Allows to obtain number of element types (non-standard: macro; configured max types: 16)
variant_alternative<>: Allows to select type by index
variant_alternative_t<>: Allows to select type by index (C++11)
variant_alternative_T(): Allows to select type by index (non-standard: macro)
std::hash<>: Allows to obtain hash (C++11)
```
