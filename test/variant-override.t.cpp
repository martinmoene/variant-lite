// Copyright 2016-2025 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// mix C++17 std::monostate and std::bad_variant_access with nonstd::variant with configurable such types.
// msvc: dumpbin /symbols variant-override.t.obj |grep -i bad_variant_access: std::bad_variant_access
// msvc: dumpbin /symbols variant.t.obj          |grep -i bad_variant_access: nonstd::variants::bad_variant_access

// make sure to only override with std::monostate for C++17 and later:

#ifndef   variant_ovr_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define variant_ovr_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define variant_ovr_CPLUSPLUS  __cplusplus
# endif
#endif

// Requires C++17:

#if variant_ovr_CPLUSPLUS >= 201703L

#define variant_CONFIG_SELECT_VARIANT               variant_VARIANT_NONSTD
#define variant_CONFIG_OVERRIDE_MONOSTATE           std::monostate
#define variant_CONFIG_OVERRIDE_BAD_VARIANT_ACCESS  std::bad_variant_access

#include <string>
#include <utility>      // std::monostate
#include <variant>

#include "variant-main.t.hpp"

namespace std_compat
{
    using std::monostate;
    using std::bad_variant_access;
}

namespace std_compat
{
    template <class... Types>
    using variant = nonstd::variant<Types...>;

    // using nonstd::bad_variant_access;
    // using nonstd::monostate;
    using nonstd::in_place_type_t;
    using nonstd::in_place_type;
    using nonstd::in_place_index_t;
    using nonstd::in_place_index;
    using nonstd::get;
    using nonstd::get_if;
    using nonstd::holds_alternative;
    using nonstd::visit;
    using nonstd::variant_npos;
    using nonstd::variant_size;
    using nonstd::variant_size_v;
    using nonstd::variant_alternative;
    using nonstd::variant_alternative_t;
}

#else // variant_ovr_CPLUSPLUS >= 201703L

// configuration without override of monostate, bad_variant access:
#include "variant-main.t.hpp"

#endif // variant_ovr_CPLUSPLUS >= 201703L

CASE("bad_variant_access: Allows to override nonstd::bad_variant_access via variant_CONFIG_OVERRIDE_MONOSTATE")
{
#if variant_ovr_CPLUSPLUS >= 201703L
    // using namespace std_compat;
    // use namespace std_compat explicitly, as to avoid 'invisible' parameter type-dependent (koenig) lookup

    auto var = std_compat::variant<int, std::string>(std_compat::in_place_type<std::string>, std::string("std::variant") );

    EXPECT_THROWS_AS(std_compat::get<int>(var), std_compat::bad_variant_access);
#else
    EXPECT("bad_variant_access: no override using std::bad_variant_access (no C++17)");
#endif // variant_ovr_CPLUSPLUS >= 201703L
}

// end of file
