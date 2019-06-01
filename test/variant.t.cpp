// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "variant-main.t.hpp"

#include <memory>

using namespace nonstd;

namespace {
    
using lest::to_string;

// ensure comparison of pointers for lest:

#if variant_USES_STD_VARIANT
#define variant_noexcept  noexcept
const void * test_nullptr = nullptr;
#else
const void * test_nullptr = variant_nullptr;
#endif

// The following tracer code originates as Oracle from Optional by
// Andrzej Krzemienski, https://github.com/akrzemi1/Optional.

enum State
{
    /* 0 */ default_constructed,
    /* 1 */ value_copy_constructed,
    /* 2 */ value_move_constructed,
    /* 3 */ copy_constructed,
    /* 4 */ move_constructed,
    /* 5 */ move_assigned,
    /* 6 */ copy_assigned,
    /* 7 */ value_copy_assigned,
    /* 8 */ value_move_assigned,
    /* 9 */ moved_from,
    /*10 */ value_constructed
};

struct Tracer
{
    State state;
    static int instances;

    static void reset() variant_noexcept { instances = 0; }

    ~Tracer() variant_noexcept { --instances; }
    Tracer()  variant_noexcept { ++instances; state = default_constructed; }

    Tracer( const Tracer & ) variant_noexcept  { ++instances; state = copy_constructed; }
    Tracer & operator= ( const Tracer & ) variant_noexcept  { state = copy_assigned; return *this; }

#if variant_CPP11_OR_GREATER
    Tracer( Tracer && ) variant_noexcept  { ++instances; state = move_constructed; }
    Tracer &  operator= ( Tracer && ) variant_noexcept  { state = move_assigned; return *this; }
#endif
};

int Tracer::instances = 0;

struct TracerX : Tracer{};
struct TracerY : Tracer{};

struct V
{
    State state;
    int   value;

    V(             ) : state( default_constructed ), value( deflt() ) {}
    V( int       v ) : state( value_constructed   ), value( v       ) {}
    V( V const & v ) : state( copy_constructed    ), value( v.value ) {}

    V & operator=( int       v ) { state = value_copy_assigned; value = v; return *this; }
    V & operator=( V const & v ) { state = copy_assigned      ; value = v.value; return *this; }

#if variant_CPP11_OR_GREATER
    V(             V && v ) : state( move_constructed   ), value(  std::move( v.value ) ) {}
    V & operator=( V && v ) { state = move_assigned      ; value = std::move( v.value ); return *this; }
#endif

    static int deflt() { return 42; }

    bool operator==( V const & other ) const { return state == other.state && value == other.value; }
};

struct S
{
    State state;
    V     value;

    S(             ) : state( default_constructed    ) {}
    S( V const & v ) : state( value_copy_constructed ), value( v ) {}
    S( S const & s ) : state( copy_constructed       ), value( s.value        ) {}

    S & operator=( V const & v ) { state = value_copy_assigned; value = v; return *this; }
    S & operator=( const S & s ) { state = copy_assigned      ; value = s.value; return *this; }

#if variant_CPP11_OR_GREATER
    S(             V && v ) : state(  value_move_constructed ), value(  std::move( v ) ) { v.state = moved_from; }
    S(             S && s ) : state(  move_constructed       ), value(  std::move( s.value ) ) { s.state = moved_from; }

    S & operator=( V && v ) { state = value_move_assigned     ; value = std::move( v ); v.state = moved_from; return *this; }
    S & operator=( S && s ) { state = move_assigned           ; value = std::move( s.value ); s.state = moved_from; return *this; }
#endif

    bool operator==( S const & other ) const { return state == other.state && value == other.value; }
};

inline std::ostream & operator<<( std::ostream & os, V const & v )
{
    using lest::to_string;
    return os << "[V:" << to_string( v.value ) << "]";
}

class NoDefaultConstruct { NoDefaultConstruct(){} };

struct BlowCopyMoveConstruct
{
    static bool blow;
    static void enable_blow() { blow = true; }
    static void disable_blow() { blow = false; }

    ~BlowCopyMoveConstruct() {}
    BlowCopyMoveConstruct( bool blow_ = true ) { blow = blow_; }
    BlowCopyMoveConstruct( BlowCopyMoveConstruct const & ) { if ( blow ) throw 42; }
    BlowCopyMoveConstruct & operator=( BlowCopyMoveConstruct const & ) { return *this; }
#if variant_CPP11_OR_GREATER
    BlowCopyMoveConstruct( BlowCopyMoveConstruct && ) { if ( blow ) throw 42; }
    BlowCopyMoveConstruct & operator=( BlowCopyMoveConstruct && ) = default;
#endif
};

bool BlowCopyMoveConstruct::blow = true;

struct BlowCopyMoveAssign
{
    ~BlowCopyMoveAssign() {};
    BlowCopyMoveAssign() {}
    BlowCopyMoveAssign( BlowCopyMoveAssign const & ) {}
    BlowCopyMoveAssign & operator=( BlowCopyMoveAssign const & ) { throw 42; return *this; }
#if variant_CPP11_OR_GREATER
    BlowCopyMoveAssign( BlowCopyMoveAssign && ) = default;
    BlowCopyMoveAssign & operator=( BlowCopyMoveAssign && ) { throw 42; return *this; }
#endif
};

typedef variant<char, BlowCopyMoveConstruct> empty_variant_t;

empty_variant_t make_empty_variant()
{
    empty_variant_t var = 'a';

    try { var = BlowCopyMoveConstruct(); } catch(...) {}

    return var;
}

empty_variant_t make_non_empty_variant()
{
    return empty_variant_t( 'a' );
}

template< typename T >
std::size_t to_size_t( T v )
{
    return v;
}

} // anonymous namespace

//
// variant member operations:
//

CASE( "variant: Disallows non-default constructible as first type" )
{
//  variant<NoDefaultConstruct> var;

    EXPECT( true );
}

CASE( "variant: Allows non-default constructible as second and later type (first: int)" )
{
    variant<int, NoDefaultConstruct> var;

    EXPECT( true );
}

CASE( "variant: Allows non-default constructible as second and later type (first: monostate)" )
{
    variant<monostate, NoDefaultConstruct> var;

    EXPECT( true );
}

CASE( "variant: Allows to default-construct variant" )
{
    variant<S> var;

    EXPECT( get<S>(var).value.value == V::deflt() );
    EXPECT( get<S>(var).value.state == default_constructed );
    EXPECT( get<S>(var).state       == default_constructed );
}

CASE( "variant: Allows to copy-construct from variant" )
{
    S s( 7 );
    variant<S> var1( s );

    variant<S> var2( var1 );

    EXPECT( get<S>(var2).value.value == 7 );
    EXPECT( get<S>(var2).state       == copy_constructed );
}

CASE( "variant: Allows to move-construct from variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<S> var{ variant<S>{ S{} } };

    EXPECT( get<S>(var).value.value == V::deflt()  );
    EXPECT( get<S>(var).value.state == move_constructed );
    EXPECT( get<S>(var).state       == move_constructed );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-construct if-noexcept from variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    std::vector< variant<Tracer> > vec(1);

    EXPECT( get<Tracer>( vec[0] ).state == default_constructed );

    vec.resize( 7 );

    EXPECT( get<Tracer>( vec[0] ).state == move_constructed );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to obtain the index of the current type" )
{
    variant<int, S> vari(   3  );
    variant<int, S> vars( S(7) );

    EXPECT( 0u == vari.index() );
    EXPECT( 1u == vars.index() );
}

CASE( "variant: Allows to inspect if variant is \"valueless by exception\"" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var{ make_empty_variant() };

    EXPECT( var.valueless_by_exception() );
#else
    EXPECT( !!"variant: emplace is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-assign from variant" )
{
    SETUP("") {
    SECTION("Before assignment, assignee is destructed")
    {
        Tracer::reset();
        {
            TracerY y;
            variant<TracerX, TracerY> var1;
            variant<TracerX, TracerY> var2;
            variant<TracerX, TracerY> var3(y);

            EXPECT( Tracer::instances == 4 );

            var1 = var2; EXPECT( Tracer::instances == 4 );
            var1 = var3; EXPECT( Tracer::instances == 4 );
        }
        EXPECT( Tracer::instances == 0 );
    }
    SECTION("On same-alternative assignment, assignee is copied-to")
    {
        variant<Tracer> var1;
        variant<Tracer> var2;

        var1 = var2;

        EXPECT( get<Tracer>(var1).state == copy_assigned );
    }
    SECTION("On same-alternative assignment, assignee does not become valueless-by-exception")
    {
        variant<BlowCopyMoveAssign> var1;
        variant<BlowCopyMoveAssign> var2;

        try { var1 = var2; } catch (...) {}

        EXPECT_NOT( var1.valueless_by_exception() );
    }
    SECTION("On non-same-alternative assignment, assignee may become valueless-by-exception")
    {
        variant<int, BlowCopyMoveConstruct> var1;
        variant<int, BlowCopyMoveConstruct> var2( BlowCopyMoveConstruct( false ) );

        BlowCopyMoveConstruct::enable_blow();

        EXPECT_NOT( var1.valueless_by_exception() );

        try { var1 = var2; } catch (...) {}

#if     variant_USES_STD_VARIANT
# if    variant_COMPILER_CLANG_VERSION
        EXPECT( var1.valueless_by_exception() );
# elif  variant_COMPILER_GNU_VERSION
        EXPECT( var1.valueless_by_exception() );
# elif  variant_COMPILER_MSVC_VERSION
        EXPECT_NOT( var1.valueless_by_exception() );
# endif
#else // variant_USES_STD_VARIANT
        EXPECT( var1.valueless_by_exception() );
#endif // variant_USES_STD_VARIANT
    }
    }
}

CASE( "variant: Allows to copy-assign mutually empty variant" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var1{ make_empty_variant() };
    empty_variant_t var2{ make_empty_variant() };

    var1 = var2;

    EXPECT( var1.valueless_by_exception() );
    EXPECT( var2.valueless_by_exception() );
#else
    EXPECT( !!"variant: make_empty_variant requires C++11 (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-assign from empty variant" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var1{ make_non_empty_variant() };
    empty_variant_t var2{ make_empty_variant()     };

    var1 = var2;

    EXPECT( var1.valueless_by_exception() );
    EXPECT( var2.valueless_by_exception() );
#else
    EXPECT( !!"variant: make_empty_variant requires C++11 (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-assign to empty variant" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var1{ make_empty_variant()     };
    empty_variant_t var2{ make_non_empty_variant() };

    var1 = var2;

    EXPECT_NOT( var1.valueless_by_exception() );
    EXPECT_NOT( var2.valueless_by_exception() );
#else
    EXPECT( !!"variant: make_empty_variant requires C++11 (no C++11)" );
#endif
}

CASE( "variant: Allows to move-assign from variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    SETUP("") {
    SECTION("Before assignment, assignee is destructed")
    {
        Tracer::reset();
        {
            TracerY y;
            variant<TracerX, TracerY> var1;
            variant<TracerX, TracerY> var2;
            variant<TracerX, TracerY> var3(y);

            EXPECT( Tracer::instances == 4 );

            var1 = std::move(var2); EXPECT( Tracer::instances == 4 );
            var1 = std::move(var3); EXPECT( Tracer::instances == 4 );
        }
        EXPECT( Tracer::instances == 0 );
    }
    SECTION("On same-alternative assignment, assignee is moved-to")
    {
        variant<Tracer> var1;
        variant<Tracer> var2;

        var1 = std::move(var2);

        EXPECT( get<Tracer>(var1).state == move_assigned );
    }
    SECTION("On same-alternative assignment, assignee does not become valueless-by-exception")
    {
        variant<BlowCopyMoveAssign> var1;
        variant<BlowCopyMoveAssign> var2;

        try { var1 = std::move(var2); } catch (...) {}

        EXPECT_NOT( var1.valueless_by_exception() );
    }
    SECTION("On non-same-alternative assignment, assignee may become valueless-by-exception")
    {
        variant<int, BlowCopyMoveConstruct> var1;

        EXPECT_NOT( var1.valueless_by_exception() );

        try { var1 = BlowCopyMoveConstruct{}; } catch (...) {}

        EXPECT( var1.valueless_by_exception() );
    }
    }
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-assign mutually empty variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var{ make_empty_variant() };

    var = make_empty_variant() ;

    EXPECT( var.valueless_by_exception() );
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-assign from empty variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var{ make_non_empty_variant() };

    var = make_empty_variant() ;

    EXPECT( var.valueless_by_exception() );
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-assign to empty variant (C++11)" )
{
#if variant_CPP11_OR_GREATER
    empty_variant_t var{ make_empty_variant() };

    var = make_non_empty_variant();

    EXPECT_NOT( var.valueless_by_exception() );
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to construct from element value" )
{
    V v(7);

    variant<S> var( v );

    EXPECT( get<S>(var).value.value == 7 );
    EXPECT(                 v.state != moved_from );
}

CASE( "variant: Allows to copy-construct from element" )
{
    S s(7);

    variant<S> var( s );

    EXPECT( get<S>(var).value.value == 7 );
    EXPECT( get<S>(var).state       == copy_constructed );
}

CASE( "variant: Allows to move-construct from element (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<S> var{ S{7} };

    EXPECT( get<S>(var).value.value == 7 );
    EXPECT( get<S>(var).state       == move_constructed );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to convert-copy-construct from element" )
{
    int i = 7;

    variant<double, std::string> var1(  i );
    variant<double, std::string> var2(  7 );

    EXPECT( var1.index() == 0u              );
    EXPECT( get<0>(var1) == lest::approx(7) );

    EXPECT( var2.index() == 0u              );
    EXPECT( get<0>(var2) == lest::approx(7) );
}

CASE( "variant: Allows to convert-move-construct from element (C++11)" )
{
#if variant_CPP11_OR_GREATER
    struct Int { operator int() { return 7; } };

    variant<double, std::string> var( Int{} );

    EXPECT( var.index() == 0u              );
    EXPECT( get<0>(var) == lest::approx(7) );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-assign from element value" )
{
    SETUP("") {
    SECTION("same-alternative copy-assignment")
    {
        int seven = 7;
        variant<int> var = 42;

        var = seven;

        EXPECT( get<int>(var) == seven );
    }
    SECTION("non-same-alternative copy-assignment")
    {
        V v( 7 );
        variant<int, S> var1;
        variant<int, S> var2;

        var1 = v;
        var2 = V( 7 );  // copy for pre-C++11

        EXPECT( get<S>(var1).value.value == 7 );
        EXPECT(                 v.state != moved_from );

        EXPECT( get<S>(var1).value.value == 7 );
    }
    }
}

CASE( "variant: Allows to move-assign from element value" )
{
#if variant_CPP11_OR_GREATER
    SETUP("") {
    SECTION("same-alternative move-assignment")
    {
        variant<int> var = 42;

        var = 7;

        EXPECT( get<int>(var) == 7 );
    }
    SECTION("non-same-alternative move-assignment")
    {
        variant<int, S> var;

        var = V( 7 );

        EXPECT( get<S>(var).value.value == 7 );
        EXPECT( get<S>(var).value.state == move_constructed );
        EXPECT( get<S>(var).value.state == move_constructed );
#if variant_USES_STD_VARIANT
        EXPECT( get<S>(var).state       == value_move_constructed );
#else
        EXPECT( get<S>(var).state       == move_constructed );
#endif
    }
    }
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-assign from element" )
{
    S s( 7 );
    variant<int, S> var1;
    variant<int, S> var2;

    var1 = s;
    var2 = S( 7 );  // copy for pre-C++11

    EXPECT( get<S>(var1).value.value == 7 );
    EXPECT(                  s.state != moved_from );

    EXPECT( get<S>(var2).value.value == 7 );
}

CASE( "variant: Allows to move-assign from element (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, S> var;

    var = S{ 7 };

    EXPECT( get<S>(var).value.value == 7 );
    EXPECT( get<S>(var).state       == move_constructed );
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to convert-copy-assign from element value" )
{
    char const * hello = "hello, world";
    variant<int, std::string> var1;
    variant<int, std::string> var2;

    var1 = hello;
    var2 = "hello, world";

    EXPECT( get<1>(var1) ==  hello         );
    EXPECT( get<1>(var2) == "hello, world" );
}

CASE( "variant: Allows to copy-construct from elements in intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s{ 7 };
    std::string hello = "hello";
    using var_t = variant< int, long, double, std::string, S >;

    std::vector<var_t> vec = { 10, 15L, 1.5, hello, s };

    EXPECT( get<0>( vec[0] ) == 10    );
    EXPECT( get<1>( vec[1] ) == 15L   );
    EXPECT( get<2>( vec[2] ) == lest::approx(1.5) );
    EXPECT( get<3>( vec[3] ) == hello );
    EXPECT( get<4>( vec[4] ).value.value == 7 );
    EXPECT( get<4>( vec[4] ).state       == copy_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-construct from elements in intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    using var_t = variant<S>;

    std::vector<var_t> vec = { S{7} };

    EXPECT( get<0>( vec[0] ).value.value == 7 );
    EXPECT( get<0>( vec[0] ).state       == copy_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

#if variant_CPP11_OR_GREATER

namespace {

struct NoCopyMove
{
    S s; int  value;

    NoCopyMove( S const& t, int v  ) : s( t ), value( v ) {}
    NoCopyMove( S && t    , int v  ) : s( std::move(t) ), value( v ) {}
    NoCopyMove(                    ) = delete;
    NoCopyMove( NoCopyMove const & ) = delete;
    NoCopyMove( NoCopyMove &&      ) = delete;
};
}
#endif

CASE( "variant: Allows to in-place copy-construct element based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant<int, NoCopyMove> var( in_place_type<NoCopyMove>, s, 7 );
//  variant<int, NoCopyMove> var( in_place<     NoCopyMove>, s, 7 );
#else
    variant<int, NoCopyMove> var( in_place_type<NoCopyMove>, s, 7 );
#endif
    EXPECT( get<NoCopyMove>( var ).value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value .value == 7  );
    EXPECT( get<NoCopyMove>( var ).s.state == copy_constructed );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place move-construct element based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant<int, NoCopyMove> var( in_place_type<NoCopyMove>, S( 7 ), 7 );
//  variant<int, NoCopyMove> var( in_place<     NoCopyMove>, S( 7 ), 7 );
#else
    variant<int, NoCopyMove> var( in_place_type<NoCopyMove>, S( 7 ), 7 );
#endif

    EXPECT( get<NoCopyMove>( var ).value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value .value == 7  );
    EXPECT( get<NoCopyMove>( var ).s.state == move_constructed );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place copy-construct element based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant<int, NoCopyMove> var( in_place_index<1>, s, 7 );
//  variant<int, NoCopyMove> var( in_place<      1>, s, 7 );
#else
    variant<int, NoCopyMove> var( in_place_index<1>, s, 7 );
#endif

    EXPECT( get<1>( var ).value == 7 );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == copy_constructed );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place move-construct element based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant<int, NoCopyMove> var( in_place_index<1>, S( 7 ), 7 );
//  variant<int, NoCopyMove> var( in_place<      1>, S( 7 ), 7 );
#else
    variant<int, NoCopyMove> var( in_place_index<1>, S( 7 ), 7 );
#endif

    EXPECT( get<1>( var ).value == 7 );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == move_constructed );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

#if variant_CPP11_OR_GREATER
struct InitList
{
    std::vector<int> vec;
    char c;
    S s;

    InitList( std::initializer_list<int> il, char k, S const & t)
    : vec( il ), c( k ), s( t ) {}

    InitList( std::initializer_list<int> il, char k, S && t)
    : vec( il ), c( k ), s( std::forward<S>(t) ) {}
};
#endif

CASE( "variant: Allows to in-place copy-construct elements from intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );

#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant< int, InitList> var( in_place_type<InitList>, { 7, 8, 9, }, 'a', s );
//  variant< int, InitList> var( in_place<     InitList>, { 7, 8, 9, }, 'a', s );
#else
    variant< int, InitList> var( in_place_type<InitList>, { 7, 8, 9, }, 'a', s );
#endif

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == copy_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place move-construct elements from intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant< int, InitList> var( in_place_type<InitList>, { 7, 8, 9, }, 'a', S( 7 ) );
//  variant< int, InitList> var( in_place<     InitList>, { 7, 8, 9, }, 'a', S( 7 ) );
#else
    variant< int, InitList> var( in_place_type<InitList>, { 7, 8, 9, }, 'a', S( 7 ) );
#endif

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == move_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place copy-construct elements from intializer-list based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );

#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant< int, InitList> var( in_place_index<1>, { 7, 8, 9, }, 'a', s );
//  variant< int, InitList> var( in_place<      1>, { 7, 8, 9, }, 'a', s );
#else
    variant< int, InitList> var( in_place_index<1>, { 7, 8, 9, }, 'a', s );
#endif

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == copy_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to in-place move-construct elements from intializer-list based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
#if nonstd_lite_HAVE_IN_PLACE_TYPES // or variant-lite
    variant< int, InitList> var( in_place_index<1>, { 7, 8, 9, }, 'a', S( 7 ) );
//  variant< int, InitList> var( in_place<      1>, { 7, 8, 9, }, 'a', S( 7 ) );
#else
    variant< int, InitList> var( in_place_index<1>, { 7, 8, 9, }, 'a', S( 7 ) );
#endif

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == move_constructed );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-emplace element based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
    variant<int, NoCopyMove> var;

    auto & result = var.emplace<NoCopyMove>( s, 7 );

    EXPECT( get<NoCopyMove>( var ).value         == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value.value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.state       == copy_constructed );

    EXPECT(                                    7 ==  result.value );
    EXPECT(&get<NoCopyMove>( var ).value         == &result.value );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-emplace element based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var;

    auto & result = var.emplace<NoCopyMove>( S( 7 ), 7 );

    EXPECT( get<NoCopyMove>( var ).value         == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value.value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.state       == move_constructed );

    EXPECT(                                    7 ==  result.value );
    EXPECT(&get<NoCopyMove>( var ).value         == &result.value );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-emplace element based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
    variant<int, NoCopyMove> var;

    auto & result = var.emplace<1>( s, 7 );

    EXPECT( get<NoCopyMove>( var ).value         == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value.value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.state       == copy_constructed );

    EXPECT(                                    7 ==  result.value );
    EXPECT(&get<NoCopyMove>( var ).value         == &result.value );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-emplace element based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var;

    auto & result = var.emplace<1>( S( 7 ), 7 );

    EXPECT( get<NoCopyMove>( var ).value         == 7 );
    EXPECT( get<NoCopyMove>( var ).s.value.value == 7 );
    EXPECT( get<NoCopyMove>( var ).s.state       == move_constructed );

    EXPECT(                                    7 ==  result.value );
    EXPECT(&get<NoCopyMove>( var ).value         == &result.value );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-emplace elements from intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
    variant< int, InitList> var;

    auto & result = var.emplace<InitList>( { 7, 8, 9, }, 'a', s );

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == copy_constructed );

    EXPECT(                     7 ==  result.vec[0] );
    EXPECT(& get<1>( var ).vec[0] == &result.vec[0] );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-emplace elements from intializer-list based on type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant< int, InitList> var;

    auto & result = var.emplace<InitList>( { 7, 8, 9, }, 'a', S( 7 ) );

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == move_constructed );

    EXPECT(                     7 ==  result.vec[0] );
    EXPECT(& get<1>( var ).vec[0] == &result.vec[0] );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to copy-emplace elements from intializer-list based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    S s( 7 );
    variant< int, InitList> var;

    auto & result = var.emplace<1>( { 7, 8, 9, }, 'a', s );

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == copy_constructed );

    EXPECT(                     7 ==  result.vec[0] );
    EXPECT(& get<1>( var ).vec[0] == &result.vec[0] );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to move-emplace elements from intializer-list based on index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant< int, InitList> var;

    auto & result = var.emplace<1>( { 7, 8, 9, }, 'a', S( 7 ) );

    EXPECT( get<1>( var ).vec[0]  ==  7  );
    EXPECT( get<1>( var ).vec[1]  ==  8  );
    EXPECT( get<1>( var ).vec[2]  ==  9  );
    EXPECT( get<1>( var ).c       == 'a' );
    EXPECT( get<1>( var ).s.value .value == 7  );
    EXPECT( get<1>( var ).s.state == move_constructed );

    EXPECT(                     7 ==  result.vec[0] );
    EXPECT(& get<1>( var ).vec[0] == &result.vec[0] );
#else
    EXPECT( !!"variant: initializer_list construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to swap variants, same index (member)" )
{
    variant<int, S> var1( 1 );
    variant<int, S> var2( 2 );

    var1.swap( var2 );

    EXPECT( get<int>( var1 ) == 2 );
    EXPECT( get<int>( var2 ) == 1 );
}

CASE( "variant: Allows to swap variants, different index (member)" )
{
    S s( 7 );
    variant<int, S> vari( 3 );
    variant<int, S> vars( s );

    vari.swap( vars );

    EXPECT( s.value.value == get< S >( vari ).value.value );
    EXPECT(             3 == get<int>( vars )             );
}

//
// variant non-member functions:
//

namespace {

struct Doubler
{
    template< class T >
    T operator()( T a ) const { return a + a; }
};
}

struct GenericVisitor1
{
    std::string operator()( int val ) const
    {
        std::ostringstream os;
        os << val;
        return os.str();
    }

    std::string operator()( std::string const & val ) const
    {
        std::ostringstream os;
        os << val;
        return os.str();
    }
};

CASE( "variant: Allows to visit contents (args: 1; configured max args: " + to_string(variant_CONFIG_MAX_VISITOR_ARG_COUNT) + ")" )
{
    typedef variant< int, std::string > var_t;
    var_t vi = 7;
    var_t vs = std::string("hello");
#if variant_CPP11_OR_GREATER
    std::string ri = visit(GenericVisitor1(), vi);
    std::string rs = visit(GenericVisitor1(), vs);
#else
    std::string ri = visit<std::string>(GenericVisitor1(), vi);
    std::string rs = visit<std::string>(GenericVisitor1(), vs);
#endif
    EXPECT( ri == "7" );
    EXPECT( rs == "hello" );
}

struct GenericVisitor2
{
    template< typename T1, typename T2 >
    std::string operator()( T1 const & v1, T2 const & v2 ) const
    {
        std::ostringstream os;
        os << v1 << v2;
        return os.str();
    }
};

CASE( "variant: Allows to visit contents (args: 2; configured max args: " + to_string(variant_CONFIG_MAX_VISITOR_ARG_COUNT) + ")" )
{
    typedef variant< int, std::string > var_t;
    var_t vi = 7;
    var_t vs = std::string("hello");

#if variant_CPP11_OR_GREATER
    std::string r = visit(GenericVisitor2(), vi, vs);
#else
    std::string r = visit<std::string>(GenericVisitor2(), vi, vs);
#endif

    EXPECT( r == "7hello" );
}

struct GenericVisitor3
{
    template<typename T1, typename T2, typename T3>
    std::string operator()(const T1& v1, const T2& v2, const T3& v3) const
    {
        std::ostringstream os;
        os << v1 << v2 << v3;
        return os.str();
    }
};

CASE( "variant: Allows to visit contents (args: 3; configured max args: " + to_string(variant_CONFIG_MAX_VISITOR_ARG_COUNT) + ")" )
{
    typedef variant< int, std::string, double > var_t;
    var_t vi = 7;
    var_t vs = std::string("hello");
    var_t vd = 0.5;

#if variant_CPP11_OR_GREATER
    std::string r = visit(GenericVisitor3(), vi, vs, vd);
#else
    std::string r = visit<std::string>(GenericVisitor3(), vi, vs, vd);
#endif

    EXPECT( r == "7hello0.5" );
}

#if variant_CPP14_OR_GREATER

struct RVRefTestVisitor
{
    std::string operator()( int val ) const
    {
        std::ostringstream os;
        os << val;
        return os.str();
    }
    std::string operator()( std::string const & val ) const
    {
        std::ostringstream os;
        os << val;
        return os.str();
    }

    template< typename ... Args >
    std::string operator()( variant<Args...> const & var ) const
    {
        return visit( RVRefTestVisitor(), var );
    }

    template< typename U >
    std::string operator()( U && ) const
    {
        static_assert( std::is_const<U>::value, "Wrong branch!" );
        return ">>> Broken branch! <<<";
    }
};

struct Unwrapper
{
    RVRefTestVisitor * m_v;

    Unwrapper( RVRefTestVisitor * v )
        : m_v( v )
    {}

    template< typename T >
    auto & Unwrap( T && val ) const
    {
        return std::forward<T>( val );
    }

    template< typename T >
    const auto & Unwrap( std::shared_ptr<T> val ) const
    {
        const auto & result = *val.get();
        return result;
    }

    template< typename ... Args >
    auto operator()( Args &&... args ) const
    {
        return (*m_v)( Unwrap( std::forward<Args>(args))...);
    }
};

#endif

CASE( "variant: Allows to visit contents, rvalue reference (args: 1; configured max args: " + to_string(variant_CONFIG_MAX_VISITOR_ARG_COUNT) + ")" )
{
#if variant_CPP14_OR_GREATER
    typedef std::shared_ptr< std::string > string_ptr_t;
    typedef variant< int, std::string, string_ptr_t > var_t;
    string_ptr_t inner = std::make_shared< std::string >("hello1");

    var_t vstr1 = inner;
    var_t vstr2 = std::string("hello2");
    RVRefTestVisitor visitor;

    std::string rs1 = visit( Unwrapper( &visitor ), vstr1 );
    std::string rs2 = visit( Unwrapper( &visitor ), vstr2 );

    EXPECT( rs1 == "hello1" );
    EXPECT( rs2 == "hello2" );
#else
    EXPECT( !!"variant: return type deduction is not available (no C++14)" );
#endif
}

CASE( "variant: Allows to check for content by type" )
{
    typedef variant< int, long, double, std::string > var_t;
    var_t vi = 7;
    var_t vl = 7L;
    var_t vd = 7.0;
    var_t vs = std::string("hello");

    EXPECT(     holds_alternative< int          >( vi ) );
    EXPECT(     holds_alternative< long         >( vl ) );
    EXPECT(     holds_alternative< double       >( vd ) );
    EXPECT(     holds_alternative< std::string  >( vs ) );

#if ! variant_USES_STD_VARIANT // fires static_assert with g++ (GCC) 7.2.0:
    EXPECT_NOT( holds_alternative< char         >( vi ) );
    EXPECT_NOT( holds_alternative< short        >( vi ) );
    EXPECT_NOT( holds_alternative< float        >( vd ) );

    EXPECT_NOT( holds_alternative< unsigned int >( vi ) );
#endif
}

CASE( "variant: Allows to get element by type" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( get<S>(var).value.value == 7 );
}

CASE( "variant: Allows to get element by index" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( get<1>(var).value.value == 7 );
}

CASE( "variant: Allows to get pointer to element or NULL by type" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( test_nullptr == get_if<int>( &var ) );

    EXPECT( test_nullptr != get_if< S >( &var ) );
    EXPECT(                 get_if< S >( &var )->value.value == 7 );
}

CASE( "variant: Allows to get pointer to element or NULL by index" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( test_nullptr == get_if<0>( &var ) );

    EXPECT( test_nullptr != get_if<1>( &var ) );
    EXPECT(                 get_if<1>( &var )->value.value == 7 );
}

CASE( "variant: Allows to compare variants" )
{
    variant<int, double> v = 3, w = 7;

    EXPECT( v == v );
    EXPECT( v != w );
    EXPECT( v <  w );
    EXPECT( w >  v );
    EXPECT( v <= v );
    EXPECT( v <= w );
    EXPECT( v >= v );
    EXPECT( w >= v );

    EXPECT_NOT( v == w );
    EXPECT_NOT( v != v );
    EXPECT_NOT( w <  v );
    EXPECT_NOT( v >  w );
    EXPECT_NOT( w <= v );
    EXPECT_NOT( v >= w );
}

CASE( "variant: Allows to swap variants, same index (non-member)" )
{
    variant<int, S> var1( 1 );
    variant<int, S> var2( 2 );

    swap( var1, var2 );

    EXPECT( get<int>( var1 ) == 2 );
    EXPECT( get<int>( var2 ) == 1 );
}

CASE( "variant: Allows to swap variants, different index (non-member)" )
{
    S s( 7 );
    variant<int, S> vari( 3 );
    variant<int, S> vars( s );

    swap( vari, vars );

    EXPECT( s.value.value == get< S >( vari ).value.value );
    EXPECT(             3 == get<int>( vars )             );
}

//
// variant helper classes:
//

CASE( "monostate: Allows to make variant default-constructible" )
{
    variant<monostate, NoDefaultConstruct> var;

    EXPECT( true );
}

CASE( "bad_variant_access: Indicates invalid variant access" )
{
    variant< char, int > v = 7;
    variant< char, int > const c = 7;

    EXPECT_THROWS_AS( get<  0 >( v ), bad_variant_access );
    EXPECT_THROWS_AS( get<  0 >( c ), bad_variant_access );
    EXPECT_THROWS_AS( get<char>( v ), bad_variant_access );
    EXPECT_THROWS_AS( get<char>( c ), bad_variant_access );
}

namespace {
    struct t1{};
    struct t2{};
    struct t3{};
    struct t4{};
    struct t5{};
    struct t6{};
    struct t7{};
    struct t8{};
}

CASE( "variant_size<>: Allows to obtain number of element types (configured max types: " + to_string(variant_CONFIG_MAX_TYPE_COUNT) + ")" )
{
    typedef variant<t1> var1;
    typedef variant<t1, t2> var2;
    typedef variant<t1, t2, t3> var3;
    typedef variant<t1, t2, t3, t4> var4;
    typedef variant<t1, t2, t3, t4, t5> var5;
    typedef variant<t1, t2, t3, t4, t5, t6> var6;
    typedef variant<t1, t2, t3, t4, t5, t6, t7> var7;
//  typedef variant<t1, t2, t3, t4, t5, t6, t7, t8> var8;

    EXPECT( 1u == to_size_t( variant_size<var1>::value ) );
    EXPECT( 2u == to_size_t( variant_size<var2>::value ) );
    EXPECT( 3u == to_size_t( variant_size<var3>::value ) );
    EXPECT( 4u == to_size_t( variant_size<var4>::value ) );
    EXPECT( 5u == to_size_t( variant_size<var5>::value ) );
    EXPECT( 6u == to_size_t( variant_size<var6>::value ) );
    EXPECT( 7u == to_size_t( variant_size<var7>::value ) );
//  EXPECT( 8u == to_size_t( variant_size<var8>::value ) );
}

CASE( "variant_size_v<>: Allows to obtain number of element types (C++14; configured max types: " + to_string(variant_CONFIG_MAX_TYPE_COUNT) + ")" )
{
#if variant_CPP14_OR_GREATER
    typedef variant<t1> var1;
    typedef variant<t1, t2> var2;
    typedef variant<t1, t2, t3> var3;
    typedef variant<t1, t2, t3, t4> var4;
    typedef variant<t1, t2, t3, t4, t5> var5;
    typedef variant<t1, t2, t3, t4, t5, t6> var6;
    typedef variant<t1, t2, t3, t4, t5, t6, t7> var7;
//  typedef variant<t1, t2, t3, t4, t5, t6, t7, t8> var8;

    EXPECT( 1u == variant_size_v<var1> );
    EXPECT( 2u == variant_size_v<var2> );
    EXPECT( 3u == variant_size_v<var3> );
    EXPECT( 4u == variant_size_v<var4> );
    EXPECT( 5u == variant_size_v<var5> );
    EXPECT( 6u == variant_size_v<var6> );
    EXPECT( 7u == variant_size_v<var7> );
//  EXPECT( 8u == variant_size_v<var8>::value );
#else
    EXPECT( !!"variant_size_v<>: variable templates is not available (no C++14)" );
#endif
}

CASE( "variant_size_V(): Allows to obtain number of element types (non-standard: macro; configured max types: " + to_string(variant_CONFIG_MAX_TYPE_COUNT) + ")" )
{
    typedef variant<t1> var1;
    typedef variant<t1, t2> var2;
    typedef variant<t1, t2, t3> var3;
    typedef variant<t1, t2, t3, t4> var4;
    typedef variant<t1, t2, t3, t4, t5> var5;
    typedef variant<t1, t2, t3, t4, t5, t6> var6;
    typedef variant<t1, t2, t3, t4, t5, t6, t7> var7;
//  typedef variant<t1, t2, t3, t4, t5, t6, t7, t8> var8;

    EXPECT( 1u == to_size_t( variant_size_V( var1 ) ) );
    EXPECT( 2u == to_size_t( variant_size_V( var2 ) ) );
    EXPECT( 3u == to_size_t( variant_size_V( var3 ) ) );
    EXPECT( 4u == to_size_t( variant_size_V( var4 ) ) );
    EXPECT( 5u == to_size_t( variant_size_V( var5 ) ) );
    EXPECT( 6u == to_size_t( variant_size_V( var6 ) ) );
    EXPECT( 7u == to_size_t( variant_size_V( var7 ) ) );
//  EXPECT( 8u == to_size_t( variant_size_V( var8 ) ) );
}

CASE( "variant_alternative<>: Allows to select type by index" )
{
#if variant_CPP11_OR_GREATER
    static_assert( std::is_same<char, typename variant_alternative< 0, variant<char> >::type >::value, "variant_alternative<0,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 1, variant<int, char> >::type >::value, "variant_alternative<1,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 2, variant<int, int, char> >::type >::value, "variant_alternative<2,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 3, variant<int, int, int, char> >::type >::value, "variant_alternative<3,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 4, variant<int, int, int, int, char> >::type >::value, "variant_alternative<4,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 5, variant<int, int, int, int, int, char> >::type >::value, "variant_alternative<5,...>" );
    static_assert( std::is_same<char, typename variant_alternative< 6, variant<int, int, int, int, int, int, char> >::type >::value, "variant_alternative<6,...>" );
#else
    EXPECT( !!"variant_alternative<>: static_assert is not available (no C++11)" );
#endif
}

CASE( "variant_alternative_t<>: Allows to select type by index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    static_assert( std::is_same<char, variant_alternative_t< 0, variant<char> > >::value, "variant_alternative_t<0,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 1, variant<int, char> > >::value, "variant_alternative_t<1,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 2, variant<int, int, char> > >::value, "variant_alternative_t<2,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 3, variant<int, int, int, char> > >::value, "variant_alternative_t<3,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 4, variant<int, int, int, int, char> > >::value, "variant_alternative_t<4,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 5, variant<int, int, int, int, int, char> > >::value, "variant_alternative_t<5,...>" );
    static_assert( std::is_same<char, variant_alternative_t< 6, variant<int, int, int, int, int, int, char> > >::value, "variant_alternative_t<6,...>" );
#else
    EXPECT( !!"variant_alternative_t<>: alias template is not available (no C++11)" );
#endif
}

CASE( "variant_alternative_T(): Allows to select type by index (non-standard: macro)" )
{
#if variant_CPP11_OR_GREATER
    // cannot use variant<int, char> in macro due to comma:

    typedef variant<char> var0;
    typedef variant<int, char> var1;
    typedef variant<int, int, char> var2;
    typedef variant<int, int, int, char> var3;
    typedef variant<int, int, int, int, char> var4;
    typedef variant<int, int, int, int, int, char> var5;
    typedef variant<int, int, int, int, int, int, char> var6;

    static_assert( std::is_same<char, variant_alternative_T( 0, var0 ) >::value, "variant_alternative_T(0, var0)" );
    static_assert( std::is_same<char, variant_alternative_T( 1, var1 ) >::value, "variant_alternative_T(1, var1)" );
    static_assert( std::is_same<char, variant_alternative_T( 2, var2 ) >::value, "variant_alternative_T(2, var2)" );
    static_assert( std::is_same<char, variant_alternative_T( 3, var3 ) >::value, "variant_alternative_T(3, var3)" );
    static_assert( std::is_same<char, variant_alternative_T( 4, var4 ) >::value, "variant_alternative_T(4, var4)" );
    static_assert( std::is_same<char, variant_alternative_T( 5, var5 ) >::value, "variant_alternative_T(5, var5)" );
    static_assert( std::is_same<char, variant_alternative_T( 6, var6 ) >::value, "variant_alternative_T(6, var6)" );
#else
    EXPECT( !!"variant_alternative_T(): static_assert is not available (no C++11)" );
#endif
}

#if variant_CPP11_OR_GREATER

namespace {

struct Pad
{
    union { char c; int  i; };
};

// ensure non-char bits differ:

Pad make_pad1() { Pad p; p.i =  0; p.c = 'x'; return p; }
Pad make_pad2() { Pad p; p.i = ~0; p.c = 'x'; return p; }
}

namespace std {

template<>
struct hash<Pad>
{
    std::size_t operator()( Pad const & v ) const variant_noexcept
    {
        return std::hash<char>{}( v.c );
    }
};
}

#endif

CASE( "std::hash<>: Allows to obtain hash (C++11)" )
{
#if variant_CPP11_OR_GREATER
    using variant_t = variant<char, Pad>;
    variant_t var1( make_pad1() );
    variant_t var2( make_pad2() );

    EXPECT( std::hash<variant_t>{}( var1 ) == std::hash<variant_t>{}( var2 ) );
#else
    EXPECT( !!"std::hash<>: std::hash<> is not available (no C++11)" );
#endif
}

CASE("index_of<>(): method should be static" "[.issue-30]")
{
#if variant_CPP11_OR_GREATER && ! variant_USES_STD_VARIANT
    typedef variant<int, double, std::string> Value;
    Value value("Some string");

    switch ( value.index() )
    {
    case Value::index_of<int>():
        // do something
        break;
    case Value::index_of<double>():
        // do something
        break;
    case Value::index_of<std::string>():
        // do something
        break;
    }

    EXPECT( !!"prevent warnings" );
#else
    EXPECT( !!"index_of<>(): test is not available (no C++11, or std::variant)" );
#endif
}

CASE("operator=(T const &): assignment from element lvalue must use copy-assignment in C++11 and later" "[.issue-31]")
{
    variant<V> var1;
    variant<V> var2;
    V seven = 7;

    var1 = 7;
    var2 = seven;

    EXPECT( get<V>(var1).state == (variant_CPP11_OR_GREATER ? move_assigned : copy_assigned) );
    EXPECT( get<V>(var2).state == copy_assigned );
}

namespace issue_31 {

    struct CopyOnly
    {
        CopyOnly() {}
        CopyOnly( CopyOnly const & ) {}
        CopyOnly & operator=( CopyOnly const & ) { return *this; }

#if variant_CPP11_OR_GREATER
        CopyOnly( CopyOnly && ) = delete;
        CopyOnly & operator=( CopyOnly && ) = delete;
#endif
    };
}

CASE("operator=(variant const &): copy-assignment from variant lvalue must not require element move-assignment in C++11 and later" "[.issue-31]")
{
    using namespace issue_31;

    variant<CopyOnly> var1;
    variant<CopyOnly> var2;

    var1 = var2;

    EXPECT( true );
}

namespace issue_33 {

    template< size_t i > class N { int k; };
}

CASE("max_index(): should not cause erroneous bad_variant_access in get()" "[.issue-33]")
{
    if ( 4 == sizeof(int) )
    {
        using issue_33::N;
        typedef variant<N<0>, N<1>, N<2>, N<3>, int> variant_t;

        EXPECT_NO_THROW( get<int>( variant_t( 13 ) ) );
    }
    else
    {
        EXPECT( !!"max_index(): test is not available (sizeof(int) != 4)" );
    }
}

// end of file
