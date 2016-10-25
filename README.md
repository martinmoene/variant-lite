# variant lite: A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++11 and later

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)  [![Standard](https://img.shields.io/badge/c%2B%2B-98-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-14-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.org/martinmoene/variant-lite.svg?branch=master)](https://travis-ci.org/martinmoene/variant-lite) [![Build status](https://ci.appveyor.com/api/projects/status/w2dgn3fxyrd6vcq8?svg=true)](https://ci.appveyor.com/project/martinmoene/variant-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fvariant-lite.svg)](https://github.com/martinmoene/variant-lite/releases) [![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/variant-lite/master/include/nonstd/variant.hpp)

**Contents**  
- [Example usage](#example-usage)
- [In a nutshell](#in-a-nutshell)
- [License](#license)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Building the tests](#building-the-tests)
- [Synopsis](#synopsis)
- [Features](#features)
- [Reported to work with](#reported-to-work-with)
- [Other variant implementations](#other-variant-implementations)
- [Notes and references](#notes-and-references)
- [Appendix](#appendix)


Example usage
-------------

```Cpp
#include "variant.hpp"

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
prompt>g++ -std=c++98 -Wall -I../include/nonstd -o 01-basic.exe 01-basic.cpp && 01-basic.exe
```

In a nutshell
-------------
**variant lite** is a single-file header-only library to represent a type-safe union. The library aims to provide a [C++17-like variant](http://en.cppreference.com/w/cpp/utility/variant) for use with C++98 and later.

**Features and properties of variant lite** are ease of installation (single header), ...

*variant lite* may provide class template `optional` for nullable objects that is based on `variant`. 

**Not provided** are construction or emplacement via `initializer_list`s.


License
-------
*variant lite* uses the [MIT](LICENSE) license.
 

Dependencies
------------
*variant lite* has no other dependencies than the [C++ standard library](http://en.cppreference.com/w/cpp/header).


Installation
------------
*variant lite* is a single-file header-only library. Put `variant.hpp` in the [include](include) folder directly into the project source tree or somewhere reachable from your project.


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


Synopsis
--------

**Contents**  
- [Types in namespace nonstd](#types-in-namespace-nonstd)  
- [Interface of *variant lite*](#interface-of-variant-lite)  
- [Algorithms for *variant lite*](#algorithms-for-variant-lite)  
- [Feature selection macros](#feature-selection-macros)
- [Macros to control alignment](#macros-to-control-alignment)  

### Types in namespace nonstd

| Purpose               | Type | Notes |
|-----------------------|------|-------|
| Type-safe union       | template< class T0...T6 ><br>class variant | non-standard: may hold up to seven types |
| Error reporting       | class bad_variant_access         |&nbsp; |
| In-place construction | struct in_place_tag              |&nbsp;             |
| &nbsp;                | in_place_type_t( T)              | macro for alias template in_place_type_t&lt;T>  |
| &nbsp;                | in_place_index_t( T )            | macro for alias template in_place_index_t&lt;T> |
| Variant size          | template<...><br>struct variant_size< variant<...> > | &nbsp;|
| &nbsp;                | variant_size_v&lt; T >           | C++14 |
| &nbsp;                | variant_size_V( T )              | macro for nonstd::variant_size<T>::value |
| Select variant type   | template< std::size_t I, ...><br>struct variant_alternative< I, variant<...> > | &nbsp; |
| &nbsp;                | variant_alternative_t&lt; I, T > | C++11 |
| &nbsp;                | variant_alternative_T( I, T )    | macro for typename nonstd::variant_alternative<I,T >::type |

### Interface of *variant lite*

| Kind         | Std  | Method                                       | Result |
|--------------|------|----------------------------------------------|--------|
| Construction |&nbsp;| variant()                                    | default-construct first type      |
| &nbsp;       |&nbsp;| variant( Tx const & x )                      | copy-construct with value type Tx |
| &nbsp;       |C++11 | variant( Tx && x )                           | move-construct with value type Tx |
| &nbsp;       |&nbsp;| variant(variant const & other)               | copy-construct from other variant |
| &nbsp;       |C++11 | variant( variant && other )                  | move-construct from other variant |
| &nbsp;       |C++11 | template< class T, class... Args ><br>explicit variant( in_place_type_t(T), Args&&... args) | in-place-construct type T |
| &nbsp;       |C++11 | template< class T, class U, class... Args ><br>explicit variant( in_place_type_t(T),<br>&emsp;std::initializer_list&lt;U> il, Args&&... args ) | in-place-construct type T<br>Note: not implemented |
| &nbsp;       |C++11 | template< std::size_t I, class... Args ><br>explicit variant( in_place_index_t(I), Args&&... args ) | in-place-construct type at index I | 
| &nbsp;       |C++11 | template< size_t I, class U, class... Args><br>explicit variant( in_place_index_t(I),<br>&emsp;std::initializer_list&lt;U> il, Args&&... args ) | in-place-construct type at index I<br>Note: not implemented |
| Destruction  |&nbsp;| ~variant()                                   | destruct current content |
| Assignment   |&nbsp;| variant & operator=( variant const & rhs )   | copy-assign from other;<br>destruct current content |
| &nbsp;       |C++11 | variant & operator=( variant && other )      | move-assign from other;<br>destruct current content |
| State        |&nbsp;| std::size_t index() const                    | index of current content's type |
| &nbsp;       |&nbsp;| bool valueless_by_exception() const          | true if no content is present |
| Emplace      |C++11 | template< class T, class... Args ><br>void emplace( Args&&... args ) | emplace type T |
| &nbsp;       |C++11 | template< class T, class U, class... Args ><br>void emplace( std::initializer_list&lt;U> il, Args&&... args ) | emplace type T;<br>Note: not implemented |
| &nbsp;       |C++11 | template< size_t I, class... Args ><br>void emplace( Args&&... args ); | emplace type at index I |
| Swap         |&nbsp;| void swap( variant & other );                | swap with other |
 
### Algorithms for *variant lite*

| Kind                      | Std  | Function |
|---------------------------|------|----------|
| Relational operators      |&nbsp;| &nbsp;   | 
| ==                        |&nbsp;| template<...><br>bool operator==( variant<...> const & v, variant<...> const & w ) |
| !=                        |&nbsp;| template<...><br>bool operator==( variant<...> const & v, variant<...> const & w ) |
| <                         |&nbsp;| template<...><br>bool operator< ( variant<...> const & v, variant<...> const & w ) |
| >                         |&nbsp;| template<...><br>bool operator> ( variant<...> const & v, variant<...> const & w ) |
| <=                        |&nbsp;| template<...><br>bool operator<=( variant<...> const & v, variant<...> const & w ) |
| >=                        |&nbsp;| template<...><br>bool operator>=( variant<...> const & v, variant<...> const & w ) |
| Content                   |&nbsp;| &nbsp;   |
| contains value of type T  |&nbsp;| template< class T, ...><br>bool holds_alternative( variant<...> const & v ) [noexcept] |
| get by type               |&nbsp;| template< class R, ...><br>R &<br>get( variant<...> & v, in_place_type_t(R) = in_place<R> ) |
| get by type (const)       |&nbsp;| template< class R, ...><br>R const &<br>get( variant<...> const & v, in_place_type_t(R) = in_place<R> ) |
| get by index              |&nbsp;| template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type &<br>get( variant<...> & v, in_place_index_t(I) = in_place<I> ) |
| get by index (const)      |&nbsp;| template< std::size_t I, ...><br>typename variant_alternative< I, variant<...> >::type const &<br>get( variant<...> const & v, in_place_index_t(I) = in_place<I> ) |
| get_if by type            |&nbsp;| template< class T, ...><br>typename detail::add_pointer&lt;T>::type<br>get_if( variant<...> * pv, in_place_type_t(T) = in_place<T> ) |
| get_if by type (const)    |&nbsp;| template< class T, ...><br>typename detail::add_pointer&lt;const T>::type<br>get_if( variant<...> const * pv, in_place_type_t(T) = in_place<T>) |
| get_if by index           |&nbsp;| template< std::size_t I, ...><br>typename detail::add_pointer< typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type<br>get_if( variant<...> * pv, in_place_index_t(I) = in_place<I> ) |
| get_if by index (const)   |&nbsp;| template< std::size_t I, ...><br>typename detail::add_pointer< const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type<br>get_if( variant<...> const * pv, in_place_index_t(I) = in_place<I> ) |
| swap                      |&nbsp;| template<...><br>void swap( variant<...> & x, variant<...> & y ) |
| visit                     |Note&nbsp;1| template< class Visitor, class Variant ><br>Variant visit( Visitor const & vis, Variant const & v ) |
| Hash support              |&nbsp;| &nbsp;   | 
| variant                   |C++11 | template<...> struct hash< variant<...> >; |
| monostate                 |C++11 | template<> struct hash< monostate >;       |

Note 1: visitor is limited to always return a Variant.

### Feature selection macros

\-D<b>variant\_FEATURE\_HAVE\_VARIANT\_SIZE\_V\_MACRO</b>=1  
Define this macro to 0 to omit the `variant_size_V(T)` macro. Default is 1.

\-D<b>variant\_FEATURE\_HAVE\_VARIANT\_ALTERNATIVE\_T\_MACRO</b>=1  
Define this macro to 0 to omit the `variant_alternative_T(I,T)` macro. Default is 1.

### Macros to control alignment

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


Other variant implementations
-----------------------------
- Anthony Williams. [C++ variant class, matching the C++17 draft]().
- Agustín Bergé. [Eggs.Variant, a C++11/14 generic, type-safe, discriminated union.](https://github.com/eggs-cpp/variant).
- Mapbox. [C++11/C++14 Variant](https://github.com/mapbox/variant).
- Michael Park. [Variant: A type-safe union (C++14)](https://github.com/mpark/variant).
- Gregor Burger. [Typesafe tagged union with "don't call us, we'll call you" feature (C++14)](https://github.com/gregorburger/variant).
- Anatoliy Tomilov. [C++1z variant, never empty, constexpr-enabled visit/multivisit, ... relational](https://github.com/tomilov/variant).


Implementation notes
--------------------

### Object allocation and alignment

*variant lite* reserves POD-type storage for an object of the underlying type inside a union to prevent unwanted construction and uses placement new to construct the object when required. Using non-placement new (malloc) to  obtain storage, ensures that the memory is properly aligned for the object's type, whereas that's not the case with placement new.

If you access data that's not properly aligned, it 1) may take longer than when it is properly aligned (on x86 processors), or 2) it may terminate the program immediately (many other processors).

Although the C++ standard does not guarantee that all user-defined types have the alignment of some POD type, in practice it's likely they do [7, part 2].

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

Note that the algorithm of 5. differs from the one Andrei Alexandrescu uses in [7, part 2].

The class template `alignment_of<>` is gleaned from [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) [11].

For more information on constructed unions and alignment, see [6-11].


Notes and References
--------------------
### References
[1] CppReference. [Variant](http://en.cppreference.com/w/cpp/utility/variant).  

[2] ISO/IEC WG21. [N4606, section 20.7 Variants](wg21.link/n4606). 12 July 2016.

[3] Andrzej Krzemieński. [Constexpr unions](https://akrzemi1.wordpress.com/2012/12/13/constexpr-unions/). December 2012.

[4] Agustín Bergé. [Eggs.Variant - Part I](http://talesofcpp.fusionfenix.com/post-17/eggs.variant---part-i). August 2014.

[5] Agustín Bergé. [Eggs.Variant - Part II (the constexpr experience)](http://talesofcpp.fusionfenix.com/post-20/eggs.variant---part-ii-the-constexpr-experience). March 2015.

[6] Andrei Alexandrescu. [An Implementation of Discriminated Unions in C++](https://www.researchgate.net/publication/2522635_An_Implementation_of_Discriminated_Unions_in_C). August 2002.

[7] Andrei Alexandrescu. [Generic<Programming>: Discriminated Unions part 1](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2004/alexandr/alexandr.htm), [part 2](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2006/alexandr/alexandr.htm), [part 3](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/cexp2008/alexandr/alexandr.htm). April 2002. 

[8] Herb Sutter. [Style Case Study #3: Construction Unions](http://www.gotw.ca/gotw/085.htm). GotW #85. 2009

[9] Kevin T. Manley. [Using Constructed Types in C++ Unions](http://collaboration.cmc.ec.gc.ca/science/rpn/biblio/ddj/Website/articles/CUJ/2002/0208/manley/manley.htm). C/C++ Users Journal, 20(8), August 2002.

[10] StackOverflow. [Determining maximum possible alignment in C++](http://stackoverflow.com/a/3126992).

[11] [Boost.TypeTraits, alignment_of](http://www.boost.org/doc/libs/1_57_0/libs/type_traits/doc/html/boost_typetraits/reference/alignment_of.html) ( [code](http://www.boost.org/doc/libs/1_57_0/boost/type_traits/alignment_of.hpp) ).

### Presentations
[12] Ben Deane. [Using Types Effectively](https://youtu.be/ojZbFIQSdl8?list=PLHTh1InhhwT7J5jl4vAhO1WvGHUUFgUQH). CppCon 2016.  


Appendix
--------
### A.1 Variant Lite test specification

```
variant: Disallows non-default constructible as first type
variant: Allows non-default constructible as second and later type (first: int)
variant: Allows non-default constructible as second and later type (first: monostate)
variant: Allows to default-construct variant
variant: Allows to copy-construct from variant
variant: Allows to move-construct from variant (C++11)
variant: Allows to copy-assign from variant
variant: Allows to move-assign from variant (C++11)
variant: Allows to construct from element value
variant: Allows to copy-construct from element
variant: Allows to move-construct from element (C++11)
variant: Allows to copy-assign from element
variant: Allows to move-assign from element (C++11)
variant: Allows to in-place construct element based on type (C++11)
variant: Allows to in-place construct element based on index (C++11)
variant: Allows to in-place construct element via intializer-list based on type (C++11, not implemented)
variant: Allows to in-place construct element via intializer-list based on index (C++11, not implemented)
variant: Allows to emplace element based on type (C++11)
variant: Allows to emplace element based on index (C++11)
variant: Allows to emplace element via intializer-list based on type (C++11, not implemented)
variant: Allows to emplace element via intializer-list based on index (C++11, not implemented)
variant: Allows to obtain the index of the current type
variant: Allows to inspect if variant is "valueless by exception"
variant: Allows to swap variants (member)
variant: Allows to visit contents (non-standard: always returning variant)
variant: Allows to check for content by type
variant: Allows to get element by type
variant: Allows to get element by index
variant: Allows to get pointer to element or NULL by type
variant: Allows to get pointer to element or NULL by index
variant: Allows to compare variants
variant: Allows to swap variants (non-member)
monostate: Allows to make variant default-constructible
bad_variant_access: Indicates invalid variant access
variant_size<>: Allows to obtain number of element types (non-standard: max 7)
variant_size_v<>: Allows to obtain number of element types (C++14, non-standard: max 7)
variant_size_V(): Allows to obtain number of element types (non-standard: max 7, macro)
variant_alternative<>: Allows to select type by index
variant_alternative_t<>: Allows to select type by index (C++11)
variant_alternative_T(): Allows to select type by index (non-standard: macro)
std::hash<>: Allows to obtain hash (C++11)
```
