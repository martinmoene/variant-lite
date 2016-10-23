# variant-lite: A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++03 and later

[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)  [![Standard](https://img.shields.io/badge/c%2B%2B-98-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-11-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-14-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![Standard](https://img.shields.io/badge/c%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization) [![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT) [![Build Status](https://travis-ci.org/martinmoene/variant-lite.svg?branch=master)](https://travis-ci.org/martinmoene/variant-lite) [![Build status](https://ci.appveyor.com/api/projects/status/w2dgn3fxyrd6vcq8?svg=true)](https://ci.appveyor.com/project/martinmoene/variant-lite) [![Version](https://badge.fury.io/gh/martinmoene%2Fvariant-lite.svg)](https://github.com/martinmoene/variant-lite/releases) [![download](https://img.shields.io/badge/latest%20version%20%20-download-blue.svg)](https://raw.githubusercontent.com/martinmoene/variant-lite/master/include/nonstd/variant.hpp)

Please note that this is a very early version of variant-lite. Remarks for implementation/simplification welcome

ToDo
----
- Complete implementation of features
- Write test specification
- Travis
- Write documentation in this Readme.md

variant-lite is inspired on
-  Tibor Djurica Potpara. [Implementing a variant type from scratch in C++](http://www.ojdip.net/2013/10/implementing-a-variant-type-in-cpp/) ([code](https://gist.github.com/tibordp/6909880)).
- [Mapbox variant](https://github.com/mapbox/variant).
- StackOverflow. [variant_modern.hpp](http://stackoverflow.com/a/3552166/437272) by vvenedik.

References
----------
Ben Deane. [Using Types Effectively](https://youtu.be/ojZbFIQSdl8?list=PLHTh1InhhwT7J5jl4vAhO1WvGHUUFgUQH). CppCon 2016.  
CppReference. [variant](http://en.cppreference.com/w/cpp/utility/variant).  

### A.1 Variant Lite test specification

```
variant: Disallows non-default constructible as first type (non-standard)
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
variant: (monostate)
variant: (bad_variant_access)
variant: Allows to obtain number of element types (non-standard: max 7)
variant: (variant_alternative)
variant: Allows to obtain hash (C++11)
```
