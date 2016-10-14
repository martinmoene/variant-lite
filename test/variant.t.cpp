//
// Copyright (c) 2016 Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// This code is licensed under the MIT License (MIT).
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "variant-lite.t.h"

namespace {

struct S
{
    enum State
    {
        default_constructed,
        copy_constructed,
        move_constructed,
        value_constructed,
    };

    State state;
    int   value;

    static int deflt() { return 3; }

    S(             ) : state( default_constructed ), value( deflt() ){}
    S( S const & s ) : state( copy_constructed    ), value( s.value ){}
#if variant_CPP11_OR_GREATER
    S( S &&      s ) : state( move_constructed    ), value( std::move(s.value) ){ s.value = -1; }
#endif
    S( int       v ) : state( value_constructed   ), value( v ){}
};

//inline bool operator==( S const & a, S const & b )
//{
//    return a.value == b.value;
//}
//
//inline std::ostream & operator<<( std::ostream & os, S const & s )
//{
//    return os << "S{" << s.value << "}";
//}

}

//
// variant member operations:
//

CASE( "variant: Disallows non-default constructible as first type (non-standard)" )
{
    class S { S(){} };
//  variant<S> var;

    EXPECT( true );
}

CASE( "variant: Allows non-default constructible as second and later type" )
{
    class S { S(){} };
    variant<int, S> var;

    EXPECT( true );
}

CASE( "variant: Allows variant default construction" )
{
    variant<S> var;

    EXPECT( get<S>(var).state == S::default_constructed );
    EXPECT( get<0>(var).state == S::default_constructed );
}

CASE( "variant: Allows variant copy-construction" )
{
    S s( 7 );
    variant<S> var1( s );

    variant<S> var2( var1 );

    EXPECT( get<S>(var2).value == 7 );
}

CASE( "variant: Allows variant move-construction (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<S> var(( variant<S>() ));

    EXPECT( get<S>(var).value == S::deflt() );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows variant copy-assignment" )
{
    variant<S> var1;
    variant<S> var2;

    var2 = var1;

    EXPECT( get<S>(var2).value == S::deflt() );
}

CASE( "variant: Allows variant move-assignment (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<S> var;

    var = variant<S>();

    EXPECT( get<S>(var).value == S::deflt() );
#else
    EXPECT( !!"variant: move-assignment is not available (no C++11)" );
#endif
}

// NTS:fix
CASE( "variant: Allows element value-construction" )
{
    S s(7);

    variant<S> var( s );

    EXPECT( get<S>(var).state == S::copy_constructed );
    EXPECT( get<S>(var).value == 7 );
}

CASE( "variant: Allows element copy-construction" )
{
    S s(7);

    variant<S> var( s );

    EXPECT( get<S>(var).state == S::copy_constructed );
    EXPECT( get<S>(var).value == 7 );
}

CASE( "variant: Allows element move-construction (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<S> var( S(7) );

    EXPECT( get<S>(var).state == S::move_constructed );
    EXPECT( get<S>(var).value == 7 );
#else
    EXPECT( !!"variant: move-construction is not available (no C++11)" );
#endif
}

#if variant_CPP11_OR_GREATER

namespace
{

struct NoCopyMove
{
    int value;
    NoCopyMove( int v ) : value( v ) {}
    NoCopyMove(                    ) = delete;
    NoCopyMove( NoCopyMove const & ) = delete;
    NoCopyMove( NoCopyMove &&      ) = delete;
};
}
#endif

CASE( "variant: Allows element type-based in-place construction (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var( in_place<NoCopyMove>, 7 );

    EXPECT( get<NoCopyMove>( var ).value == 7 );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows element type-based in-place intializer-list construction (C++11)" )
{
}

CASE( "variant: Allows element index-based in-place construction (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var( in_place<1>, 7 );

    EXPECT( get<1>( var ).value == 7 );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows element index-based in-place intializer-list construction (C++11)" )
{
}

CASE( "variant: Allows element type-based emplacement (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var;

    var.emplace<NoCopyMove>( 7 );

    EXPECT( get<NoCopyMove>( var ).value == 7 );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows element type-based intializer-list emplacement (C++11)" )
{
}

CASE( "variant: Allows element index-based emplacement (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, NoCopyMove> var;

    var.emplace<1>( 7 );

    EXPECT( get<1>( var ).value == 7 );
#else
    EXPECT( !!"variant: in-place construction is not available (no C++11)" );
#endif
}

CASE( "variant: Allows element index-based intializer-list emplacement (C++11)" )
{
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
//    struct S { operator int() { throw 42; } };
//
//    variant<float, int> v{12.f}; // OK
//    v.emplace<1>(S()); // v may be valueless
//
//    EXPECT( v.valueless_by_exception() );
}

CASE( "variant: Allows to compare variants" )
{
}

CASE( "variant: Allows to swap variants (member)" )
{
    S s( 7 );
    variant<int, S> vari( 3 );
    variant<int, S> vars( s );

    vari.swap( vars );

    EXPECT( s.value == get< S >( vari ).value );
    EXPECT(       3 == get<int>( vars )       );
}

//
// variant non-member operations:
//

CASE( "variant: Allows to obtain number of types (non-standard: max 7)" )
{
    struct t1{};
    struct t2{};
    struct t3{};
    struct t4{};
    struct t5{};
    struct t6{};
    struct t7{};
    struct t8{};

    typedef variant<t1> var1;
    typedef variant<t1, t2> var2;
    typedef variant<t1, t2, t3> var3;
    typedef variant<t1, t2, t3, t4> var4;
    typedef variant<t1, t2, t3, t4, t5> var5;
    typedef variant<t1, t2, t3, t4, t5, t6> var6;
    typedef variant<t1, t2, t3, t4, t5, t6, t7> var7;
//  typedef variant<t1, t2, t3, t4, t5, t6, t7, t8> var8;

    EXPECT( 1 == variant_size<var1>::value );
    EXPECT( 2 == variant_size<var2>::value );
    EXPECT( 3 == variant_size<var3>::value );
    EXPECT( 4 == variant_size<var4>::value );
    EXPECT( 5 == variant_size<var5>::value );
    EXPECT( 6 == variant_size<var6>::value );
    EXPECT( 7 == variant_size<var7>::value );
//  EXPECT( 8 == variant_size<var8>::value );
}

CASE( "variant: Allows to get element by type" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( get<S>(var).value == 7 );
}

CASE( "variant: Allows to get element by index" )
{
    variant<int, S> var( S( 7 ) );

    EXPECT( get<1>(var).value == 7 );
}

CASE( "variant: Allows to get pointer to element or NULL by type (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, S> var( S( 7 ) );

    EXPECT( nullptr == get_if<int>( &var ) );

    EXPECT( nullptr != get_if< S >( &var ) );
    EXPECT(            get_if< S >( &var )->value == 7 );
#else
    EXPECT( !!"variant: get_if() is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to get pointer to element or NULL by index (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int, S> var( S( 7 ) );

    EXPECT( nullptr == get_if<0>( &var ) );

    EXPECT( nullptr != get_if<1>( &var ) );
    EXPECT(            get_if<1>( &var )->value == 7 );
#else
    EXPECT( !!"variant: get_if() is not available (no C++11)" );
#endif
}

CASE( "variant: Allows to swap variants (non-member)" )
{
    S s( 7 );
    variant<int, S> vari( 3 );
    variant<int, S> vars( s );

    swap( vari, vars );

    EXPECT( s.value == get< S >( vari ).value );
    EXPECT(       3 == get<int>( vars )       );
}

CASE( "variant: Allows to obtain hash (C++11)" )
{
#if variant_CPP11_OR_GREATER
    variant<int> var( 7 );

    EXPECT( std::hash<variant<int> >()( var ) == std::hash<variant<int> >()( var ) );
#else
    EXPECT( !!"variant: std::hash<> is not available (no C++11)" );
#endif
}
// end of file
