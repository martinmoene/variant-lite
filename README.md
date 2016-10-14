# variant-lite
A single-file header-only version of a C++17-like variant, a type-safe union for C++98, C++03 and later

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
variant: Allows non-default constructible as second and later type
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
variant: Allows to in-place construct element via intializer-list based on type (C++11)
variant: Allows to in-place construct element via intializer-list based on index (C++11)
variant: Allows to emplace element based on type (C++11)
variant: Allows to emplace element based on index (C++11)
variant: Allows to emplace element via intializer-list based on type (C++11)
variant: Allows to emplace element via intializer-list based on index (C++11)
variant: Allows to obtain the index of the current type
variant: Allows to inspect if variant is "valueless by exception"
variant: Allows to compare variants
variant: Allows to swap variants (member)
variant: Allows to obtain number of element types (non-standard: max 7)
variant: Allows to get element by type
variant: Allows to get element by index
variant: Allows to get pointer to element or NULL by type
variant: Allows to get pointer to element or NULL by index
variant: Allows to swap variants (non-member)
variant: Allows to obtain hash (C++11)
```
