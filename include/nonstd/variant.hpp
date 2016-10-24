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

#pragma once

#ifndef NONSTD_VARIANT_LITE_HPP
#define NONSTD_VARIANT_LITE_HPP

#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

#define  variant_lite_VERSION "0.0.0"

// variant-lite feature selection:

#ifndef  variant_FEATURE_HAVE_VARIANT_SIZE_V_MACRO
# define variant_FEATURE_HAVE_VARIANT_SIZE_V_MACRO  1
#endif

#ifndef  variant_FEATURE_HAVE_VARIANT_ALTERNATIVE_T_MACRO
# define variant_FEATURE_HAVE_VARIANT_ALTERNATIVE_T_MACRO  1
#endif

// variant-lite configuration:

#ifndef  variant_CONFIG_MAX_ALIGN_HACK
# define variant_CONFIG_MAX_ALIGN_HACK  0
#endif

#ifndef  variant_CONFIG_ALIGN_AS
// used in #if defined(), so no default...
#endif

#ifndef  variant_CONFIG_ALIGN_AS_FALLBACK
# define variant_CONFIG_ALIGN_AS_FALLBACK  double
#endif

// Compiler detection (C++17 is speculative):

#define variant_CPP11_OR_GREATER  ( __cplusplus >= 201103L )
#define variant_CPP14_OR_GREATER  ( __cplusplus >= 201402L )
#define variant_CPP17_OR_GREATER  ( __cplusplus >= 201700L )

// half-open range [lo..hi):
#define variant_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define variant_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define variant_COMPILER_MSVC_VERSION   0
# define variant_COMPILER_NON_MSVC       1
#endif

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 7, 14 )
# pragma warning( push )
# pragma warning( disable: 4345 )   // initialization behavior changed
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# include <initializer_list>
# include <type_traits>
#endif

// Presence of C++11 language features:

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 10
# define variant_HAVE_AUTO  1
# define variant_HAVE_NULLPTR  1
# define variant_HAVE_STATIC_ASSERT  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
# define variant_HAVE_INITIALIZER_LIST  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14
# define variant_HAVE_ALIAS_TEMPLATE  1
# define variant_HAVE_CONSTEXPR_11  1
# define variant_HAVE_ENUM_CLASS  1
# define variant_HAVE_EXPLICIT_CONVERSION  1
# define variant_HAVE_IS_DEFAULT  1
# define variant_HAVE_IS_DELETE  1
# define variant_HAVE_NOEXCEPT  1
#endif

// Presence of C++14 language features:

#if variant_CPP14_OR_GREATER
# define variant_HAVE_CONSTEXPR_14  1
#endif

// Presence of C++17 language features:

#if variant_CPP17_OR_GREATER
# define variant_HAVE_ENUM_CLASS_CONSTRUCTION_FROM_UNDERLYING_TYPE  1
#endif

// Presence of C++ library features:

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 11
# define variant_HAVE_ARRAY  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_CONDITIONAL  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14 || (variant_COMPILER_MSVC_VERSION >= 9 && _HAS_CPP0X)
# define variant_HAVE_CONTAINER_DATA_METHOD  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_REMOVE_CV  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14
# define variant_HAVE_SIZED_TYPES  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 11
# define variant_HAVE_TYPE_TRAITS  1
#endif

// For the rest, consider VC14 as C++11 for variant-lite:

#if variant_COMPILER_MSVC_VERSION >= 14
# undef  variant_CPP11_OR_GREATER
# define variant_CPP11_OR_GREATER  1
#endif

// C++ feature usage:

#if variant_HAVE_CONSTEXPR_11
# define variant_constexpr constexpr
#else
# define variant_constexpr /*constexpr*/
#endif

#if variant_HAVE_CONSTEXPR_14
# define variant_constexpr14 constexpr
#else
# define variant_constexpr14 /*constexpr*/
#endif

#if variant_HAVE_NOEXCEPT
# define variant_noexcept noexcept
#else
# define variant_noexcept /*noexcept*/
#endif

#if variant_HAVE_NULLPTR
# define variant_nullptr nullptr
#else
# define variant_nullptr NULL
#endif

namespace nonstd { namespace variants {

namespace detail {

// C++11 emulation:

#if variant_HAVE_CONDITIONAL

template< bool Cond, class Then, class Else >
using conditional = std::conditional<Cond, Then, Else>;

#else

template< bool Cond, class Then, class Else >
struct conditional;

template< class Then, class Else >
struct conditional< true , Then, Else > { typedef Then type; };

template< class Then, class Else >
struct conditional< false, Then, Else > { typedef Else type; };

#endif // variant_HAVE_CONDITIONAL

#if variant_HAVE_REMOVE_CV

template< class T >
using remove_cv = std::remove_cv<T>;

#else

template< class T > struct remove_const          { typedef T type; };
template< class T > struct remove_const<const T> { typedef T type; };

template< class T > struct remove_volatile             { typedef T type; };
template< class T > struct remove_volatile<volatile T> { typedef T type; };

template< class T >
struct remove_cv
{
    typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

#endif // variant_HAVE_REMOVE_CV

// typelist:

#define variant_TL1( T1                         ) detail::typelist< T1, detail::nulltype >
#define variant_TL2( T1, T2                     ) detail::typelist< T1, variant_TL1( T2) >
#define variant_TL3( T1, T2, T3                 ) detail::typelist< T1, variant_TL2( T2, T3 ) >
#define variant_TL4( T1, T2, T3, T4             ) detail::typelist< T1, variant_TL3( T2, T3, T4 ) >
#define variant_TL5( T1, T2, T3, T4, T5         ) detail::typelist< T1, variant_TL4( T2, T3, T4, T5 ) >
#define variant_TL6( T1, T2, T3, T4, T5, T6     ) detail::typelist< T1, variant_TL5( T2, T3, T4, T5, T6 ) >
#define variant_TL7( T1, T2, T3, T4, T5, T6, T7 ) detail::typelist< T1, variant_TL6( T2, T3, T4, T5, T6, T7 ) >

// variant parameter unused type tags:

template< class T >
struct TX : T
{
                        inline TX<T> operator+ (           ) const { return TX<T>();  }
                        inline TX<T> operator- (           ) const { return TX<T>();  }

                        inline TX<T> operator! (           ) const { return TX<T>();  }
                        inline TX<T> operator~ (           ) const { return TX<T>();  }

                        inline TX<T>*operator& (           ) const { return variant_nullptr; }

    template< class U > inline TX<T> operator* ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator/ ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator% ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator+ ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator- ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator<<( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator>>( U const & ) const { return TX<T>();  }

                        inline bool  operator==( T const & ) const { return false; }
                        inline bool  operator< ( T const & ) const { return false; }

    template< class U > inline TX<T> operator& ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator| ( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator^ ( U const & ) const { return TX<T>();  }

    template< class U > inline TX<T> operator&&( U const & ) const { return TX<T>();  }
    template< class U > inline TX<T> operator||( U const & ) const { return TX<T>();  }
};

struct S0{}; typedef TX<S0> T0;
struct S1{}; typedef TX<S1> T1;
struct S2{}; typedef TX<S2> T2;
struct S3{}; typedef TX<S3> T3;
struct S4{}; typedef TX<S4> T4;
struct S5{}; typedef TX<S5> T5;
struct S6{}; typedef TX<S6> T6;

struct nulltype{};

template< class Head, class Tail >
struct typelist
{
    typedef Head head;
    typedef Tail tail;
};

// typelist max element size:

template< class List >
struct typelist_max;

template <>
struct typelist_max< nulltype >
{
    enum { value = 0 } ;
    typedef void type;
};

template< class Head, class Tail >
struct typelist_max< typelist<Head, Tail> >
{
private:
    enum { tail_value = size_t( typelist_max<Tail>::value ) };

    typedef typename typelist_max<Tail>::type tail_type;

public:
    enum { value = (sizeof( Head ) > tail_value) ? sizeof( Head ) : std::size_t( tail_value ) } ;

    typedef typename conditional< (sizeof( Head ) > tail_value), Head, tail_type>::type type;
};

#if variant_CPP11_OR_GREATER

// typelist max alignof element type:

template< class List >
struct typelist_max_alignof;

template <>
struct typelist_max_alignof< nulltype >
{
    enum { value = 0 } ;
};

template< class Head, class Tail >
struct typelist_max_alignof< typelist<Head, Tail> >
{
private:
    enum { tail_value = size_t( typelist_max_alignof<Tail>::value ) };

public:
    enum { value = (alignof( Head ) > tail_value) ? alignof( Head ) : std::size_t( tail_value ) };
};

#endif

// typelist size (lenth):

template< class List >
struct typelist_size
{
   enum { value = 1 };
};

template<> struct typelist_size< T0 > { enum { value = 0 }; };
template<> struct typelist_size< T1 > { enum { value = 0 }; };
template<> struct typelist_size< T2 > { enum { value = 0 }; };
template<> struct typelist_size< T3 > { enum { value = 0 }; };
template<> struct typelist_size< T4 > { enum { value = 0 }; };
template<> struct typelist_size< T5 > { enum { value = 0 }; };
template<> struct typelist_size< T6 > { enum { value = 0 }; };

template<> struct typelist_size< nulltype > { enum { value = 0 } ; };

template< class Head, class Tail >
struct typelist_size< typelist<Head, Tail> >
{
    enum { value = typelist_size<Head>::value + typelist_size<Tail>::value };
};

// typelist index of type:

template< class List, class T >
struct typelist_index_of;

template< class T >
struct typelist_index_of< nulltype, T >
{
    enum { value = -1 };
};

template< class Tail, class T >
struct typelist_index_of< typelist<T, Tail>, T >
{
    enum { value = 0 };
};

template< class Head, class Tail, class T >
struct typelist_index_of< typelist<Head, Tail>, T >
{
private:
    enum { nextVal = typelist_index_of<Tail, T>::value };
public:
    enum { value = nextVal == -1 ? -1 : 1 + nextVal } ;
};

// typelist type at index:

template< class List, std::size_t i>
struct typelist_type_at;

template< class Head, class Tail >
struct typelist_type_at< typelist<Head, Tail>, 0 >
{
    typedef Head type;
};

template< class Head, class Tail, std::size_t i >
struct typelist_type_at< typelist<Head, Tail>, i >
{
    typedef typename typelist_type_at<Tail, i - 1>::type type;
};

#if variant_CONFIG_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define variant_UNIQUE(  name )       variant_UNIQUE2( name, __LINE__ )
#define variant_UNIQUE2( name, line ) variant_UNIQUE3( name, line )
#define variant_UNIQUE3( name, line ) name ## line

#define variant_ALIGN_TYPE( type ) \
    type variant_UNIQUE( _t ); struct_t< type > variant_UNIQUE( _st )

template< class T >
struct struct_t { T _; };

union max_align_t
{
    variant_ALIGN_TYPE( char );
    variant_ALIGN_TYPE( short int );
    variant_ALIGN_TYPE( int );
    variant_ALIGN_TYPE( long int  );
    variant_ALIGN_TYPE( float  );
    variant_ALIGN_TYPE( double );
    variant_ALIGN_TYPE( long double );
    variant_ALIGN_TYPE( char * );
    variant_ALIGN_TYPE( short int * );
    variant_ALIGN_TYPE( int *  );
    variant_ALIGN_TYPE( long int * );
    variant_ALIGN_TYPE( float * );
    variant_ALIGN_TYPE( double * );
    variant_ALIGN_TYPE( long double * );
    variant_ALIGN_TYPE( void * );

#ifdef HAVE_LONG_LONG
    variant_ALIGN_TYPE( long long );
#endif

    struct Unknown;

    Unknown ( * variant_UNIQUE(_) )( Unknown );
    Unknown * Unknown::* variant_UNIQUE(_);
    Unknown ( Unknown::* variant_UNIQUE(_) )( Unknown );

    struct_t< Unknown ( * )( Unknown)         > variant_UNIQUE(_);
    struct_t< Unknown * Unknown::*            > variant_UNIQUE(_);
    struct_t< Unknown ( Unknown::* )(Unknown) > variant_UNIQUE(_);
};

#undef variant_UNIQUE
#undef variant_UNIQUE2
#undef variant_UNIQUE3

#undef variant_ALIGN_TYPE

#elif defined( variant_CONFIG_ALIGN_AS ) // variant_CONFIG_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define variant_ALIGN_AS( unused ) \
    variant_CONFIG_ALIGN_AS

#else // variant_CONFIG_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define variant_ALIGN_AS( to_align ) \
    typename detail::type_of_size< detail::alignment_types, detail::alignment_of< to_align >::value >::type

template <typename T>
struct alignment_of;

template <typename T>
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template <unsigned A, unsigned S>
struct alignment_logic
{
    enum { value = A < S ? A : S };
};

template< typename T >
struct alignment_of
{
    enum { value = alignment_logic<
        sizeof( alignment_of_hack<T> ) - sizeof(T), sizeof(T) >::value, };
};

template< typename List, size_t N >
struct type_of_size
{
    typedef typename conditional<
        N == sizeof( typename List::head ),
            typename List::head,
            typename type_of_size<typename List::tail, N >::type >::type type;
};

template< size_t N >
struct type_of_size< nulltype, N >
{
    typedef variant_CONFIG_ALIGN_AS_FALLBACK type;
};

template< typename T>
struct struct_t { T _; };

#define variant_ALIGN_TYPE( type ) \
    typelist< type , typelist< struct_t< type >

struct Unknown;

typedef
    variant_ALIGN_TYPE( char ),
    variant_ALIGN_TYPE( short ),
    variant_ALIGN_TYPE( int ),
    variant_ALIGN_TYPE( long ),
    variant_ALIGN_TYPE( float ),
    variant_ALIGN_TYPE( double ),
    variant_ALIGN_TYPE( long double ),

    variant_ALIGN_TYPE( char *),
    variant_ALIGN_TYPE( short * ),
    variant_ALIGN_TYPE( int * ),
    variant_ALIGN_TYPE( long * ),
    variant_ALIGN_TYPE( float * ),
    variant_ALIGN_TYPE( double * ),
    variant_ALIGN_TYPE( long double * ),

    variant_ALIGN_TYPE( Unknown ( * )( Unknown ) ),
    variant_ALIGN_TYPE( Unknown * Unknown::*     ),
    variant_ALIGN_TYPE( Unknown ( Unknown::* )( Unknown ) ),

    nulltype
    > > > > > > >    > > > > > > >
    > > > > > > >    > > > > > > >
    > > > > > >
    alignment_types;

#undef variant_ALIGN_TYPE

#endif // variant_CONFIG_MAX_ALIGN_HACK

template< typename T>
inline std::size_t hash( T const & v )
{
    // primes:
    unsigned const int a  = 54059;
    unsigned const int b  = 76963;
    unsigned const int h0 = 37;

    unsigned int h = h0;
    unsigned char const * s = reinterpret_cast<unsigned char const *>( &v );

    for ( std::size_t i = 0; i < sizeof(v); ++i, ++s )
    {
        h = (h * a) ^ (*s * b);
    }
    return h;
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct helper
{
    typedef signed char type_index_t;
    typedef variant_TL7( T0, T1, T2, T3, T4, T5, T6 ) variant_types;

    template< class U >
    static U * as( void * data )
    {
        return reinterpret_cast<U*>( data );
    }

    template< class U >
    static U const * as( void const * data )
    {
        return reinterpret_cast<const U*>( data );
    }

    static type_index_t to_index_t( std::size_t index )
    {
        return static_cast<type_index_t>( index );
    }

    static void destroy( std::size_t index, void * data )
    {
        switch ( index )
        {
            case 0: as<T0>( data )->~T0(); break;
            case 1: as<T1>( data )->~T1(); break;
            case 2: as<T2>( data )->~T2(); break;
            case 3: as<T3>( data )->~T3(); break;
            case 4: as<T4>( data )->~T4(); break;
            case 5: as<T5>( data )->~T5(); break;
            case 6: as<T6>( data )->~T6(); break;
        }
    }

#if variant_CPP11_OR_GREATER
    template< class T, class... Args >
    static type_index_t construct_t( void * data, Args&&... args )
    {
        new( data ) T( std::forward<Args...>( args... ) );

        return to_index_t( detail::typelist_index_of< variant_types, T>::value );
    }

    template< std::size_t I, class... Args >
    static type_index_t construct_i( void * data, Args&&... args )
    {
        using type = typename detail::typelist_type_at< variant_types, I >::type;

        construct_t< type >( data, std::forward<Args...>( args... ) );

        return to_index_t( I );
    }

    static type_index_t move( std::size_t const from_index, void * from_value, void * to_value )
    {
        switch ( from_index )
        {
            case 0: new( to_value ) T0( std::forward<T0>( *as<T0>( from_value ) ) ); break;
            case 1: new( to_value ) T1( std::forward<T1>( *as<T1>( from_value ) ) ); break;
            case 2: new( to_value ) T2( std::forward<T2>( *as<T2>( from_value ) ) ); break;
            case 3: new( to_value ) T3( std::forward<T3>( *as<T3>( from_value ) ) ); break;
            case 4: new( to_value ) T4( std::forward<T4>( *as<T4>( from_value ) ) ); break;
            case 5: new( to_value ) T5( std::forward<T5>( *as<T5>( from_value ) ) ); break;
            case 6: new( to_value ) T6( std::forward<T6>( *as<T6>( from_value ) ) ); break;
        }
        return to_index_t( from_index );
    }
#endif

    static type_index_t copy( std::size_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
            case 0: new( to_value ) T0( *as<T0>( from_value ) ); break;
            case 1: new( to_value ) T1( *as<T1>( from_value ) ); break;
            case 2: new( to_value ) T2( *as<T2>( from_value ) ); break;
            case 3: new( to_value ) T3( *as<T3>( from_value ) ); break;
            case 4: new( to_value ) T4( *as<T4>( from_value ) ); break;
            case 5: new( to_value ) T5( *as<T5>( from_value ) ); break;
            case 6: new( to_value ) T6( *as<T6>( from_value ) ); break;
        }
        return to_index_t( from_index );
    }
};

} // namespace detail

//
// Variant:
//

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
class variant;

class monostate{};

inline variant_constexpr bool operator< ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator> ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator<=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator>=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator==( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator!=( monostate, monostate ) variant_noexcept { return false; }

namespace detail {

template< class T >
struct in_place_type_tag {};

template< std::size_t I >
struct in_place_index_tag {};

} // namespace detail

struct in_place_tag {};

template< class T >
inline in_place_tag in_place( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_tag();
}

template< std::size_t I >
inline in_place_tag in_place( detail::in_place_index_tag<I> = detail::in_place_index_tag<I>() )
{
    return in_place_tag();
}

// mimic templated typedef:

#define in_place_type_t( T)  in_place_tag(&)( detail::in_place_type_tag<T>  )
#define in_place_index_t(T)  in_place_tag(&)( detail::in_place_index_tag<I> )

// obtain the size of the variant's list of alternatives at compile time

template< class T >
struct variant_size; /* undefined */

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_size< variant<T0, T1, T2, T3, T4, T5, T6> >
{
    enum { value = detail::typelist_size< variant_TL7(T0, T1, T2, T3, T4, T5, T6) >::value };
};

#if variant_CPP14_OR_GREATER
template< class T >
constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

#if variant_FEATURE_HAVE_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

// obtain the type of the alternative specified by its index, at compile time:

template< std::size_t I, class T >
struct variant_alternative; /* undefined */

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >
{
    typedef typename detail::typelist_type_at<variant_TL7(T0, T1, T2, T3, T4, T5, T6), I>::type type;
};

#if variant_CPP11_OR_GREATER
template< std::size_t I, class T >
using variant_alternative_t = typename variant_alternative<I, T>::type;
#endif

#if variant_FEATURE_HAVE_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(I,T)  typename nonstd::variant_alternative<I,T >::type
#endif

// NTS:implement specializes the std::uses_allocator type trait
// std::uses_allocator<nonstd::variant>

// index of the variant in the invalid state (constant)

#if variant_CPP11_OR_GREATER
variant_constexpr std::size_t variant_npos = static_cast<std::size_t>( -1 );
#else
static const std::size_t variant_npos = static_cast<std::size_t>( -1 );
#endif

class bad_variant_access : public std::exception
{
#if variant_CPP11_OR_GREATER
    virtual const char* what() const variant_noexcept
#else
    virtual const char* what() const throw()
#endif
	{
        return "bad variant access";
    }
};

template<
    class T0,
    class T1 = detail::T1,
    class T2 = detail::T2,
    class T3 = detail::T3,
    class T4 = detail::T4,
    class T5 = detail::T5,
    class T6 = detail::T6
>
class variant
{
    typedef detail::helper<T0, T1, T2, T3, T4, T5, T6> helper_type;

public:
    variant() : type_index( 0 ) { new( ptr() ) T0(); }

    variant( T0 const & t0 ) : type_index( 0 ) { new( ptr() ) T0( t0 ); }
    variant( T1 const & t1 ) : type_index( 1 ) { new( ptr() ) T1( t1 ); }
    variant( T2 const & t2 ) : type_index( 2 ) { new( ptr() ) T2( t2 ); }
    variant( T3 const & t3 ) : type_index( 3 ) { new( ptr() ) T3( t3 ); }
    variant( T4 const & t4 ) : type_index( 4 ) { new( ptr() ) T4( t4 ); }
    variant( T5 const & t5 ) : type_index( 5 ) { new( ptr() ) T5( t5 ); }
    variant( T6 const & t6 ) : type_index( 6 ) { new( ptr() ) T6( t6 ); }

#if variant_CPP11_OR_GREATER
    variant( T0 &&      t0 ) : type_index( 0 ) { new( ptr() ) T0( std::move( t0 ) ); }
    variant( T1 &&      t1 ) : type_index( 1 ) { new( ptr() ) T1( std::move( t1 ) ); }
    variant( T2 &&      t2 ) : type_index( 2 ) { new( ptr() ) T2( std::move( t2 ) ); }
    variant( T3 &&      t3 ) : type_index( 3 ) { new( ptr() ) T3( std::move( t3 ) ); }
    variant( T4 &&      t4 ) : type_index( 4 ) { new( ptr() ) T4( std::move( t4 ) ); }
    variant( T5 &&      t5 ) : type_index( 5 ) { new( ptr() ) T5( std::move( t5 ) ); }
    variant( T6 &&      t6 ) : type_index( 6 ) { new( ptr() ) T6( std::move( t6 ) ); }
#endif

    variant(variant const & other)
    : type_index( other.type_index )
    {
        helper_type::copy( other.type_index, other.ptr(), ptr() );
    }

#if variant_CPP11_OR_GREATER

    variant( variant && other )
    : type_index( other.type_index )
    {
        helper_type::move( other.type_index, other.ptr(), ptr() );
    }

    template< class T, class... Args >
    explicit variant( in_place_type_t(T), Args&&... args)
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template< class T, class U, class... Args >
    explicit variant( in_place_type_t(T), std::initializer_list<U> il, Args&&... args );

    template< std::size_t I, class... Args >
    explicit variant( in_place_index_t(I), Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template <size_t I, class U, class... Args>
    explicit variant( in_place_index_t(I), std::initializer_list<U> il, Args&&... args );

#endif // variant_CPP11_OR_GREATER

    ~variant()
    {
        helper_type::destroy( index(), ptr() );
    }

    variant & operator=( variant const & other )
    {
        copy_assign( other );
        return *this;
    }

#if variant_CPP11_OR_GREATER
    variant & operator=( variant && other )
    {
        move_assign( std::forward<variant>( other ) );
        return *this;
    }
#endif

    std::size_t index() const
    {
        return variant_npos_internal() == type_index ? variant_npos : type_index;
    }

    bool valueless_by_exception() const
    {
        return type_index == variant_npos_internal();
    }

#if variant_CPP11_OR_GREATER
    template< class T, class... Args >
    void emplace( Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template< class T, class U, class... Args >
    void emplace( std::initializer_list<U> il, Args&&... args );

    template< size_t I, class... Args >
    void emplace( Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template< size_t I, class U, class... Args >
    void emplace( std::initializer_list<U> il, Args&&... args );

#endif // variant_CPP11_OR_GREATER

    void swap( variant & other ) variant_noexcept
    {
        variant temp = *this;
        *this = other;
        other = temp;
    }

    //
    // non-standard:
    //

    template< class T >
    std::size_t index_of() const variant_noexcept
    {
        return detail::typelist_index_of<variant_types, typename detail::remove_cv<T>::type >::value;
    }

    template< class T >
    T & get()
    {
        const std::size_t i = index_of<T>();

        if ( i != index() || i == max_index() )
        {
            throw bad_variant_access();
        }

        return *as<T>();
    }

    template< class T >
    T const & get() const
    {
        const std::size_t i = index_of<T>();

        if ( i != index() || i == max_index() )
        {
            throw bad_variant_access();
        }

        return *as<const T>();
    }

private:
    typedef typename helper_type::type_index_t type_index_t;

    void * ptr() variant_noexcept
    {
        return &data;
    }

    void const * ptr() const variant_noexcept
    {
        return &data;
    }

    template< class U >
    U * as()
    {
        return reinterpret_cast<U*>( ptr() );
    }

    template< class U >
    U const * as() const
    {
        return reinterpret_cast<U const *>( ptr() );
    }

    variant_constexpr std::size_t max_index() const variant_noexcept
    {
        return data_size;
    }

    variant_constexpr type_index_t variant_npos_internal() const variant_noexcept
    {
        return static_cast<type_index_t>( -1 );
    }

    void copy_assign( variant const & rhs )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::copy( rhs.type_index, rhs.ptr(), ptr() );
    }

#if variant_CPP11_OR_GREATER
    void move_assign( variant && rhs )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
    }
#endif

private:
    typedef variant_TL7( T0, T1, T2, T3, T4, T5, T6 ) variant_types;

    enum { data_size  = detail::typelist_max< variant_types >::value };

#if variant_CPP11_OR_GREATER

    enum { data_align = detail::typelist_max_alignof< variant_types >::value };

    using aligned_storage_t = typename std::aligned_storage< data_size, data_align >::type;
    aligned_storage_t data;

#elif variant_CONFIG_MAX_ALIGN_HACK

    typedef struct { unsigned char data[ data_size ]; } aligned_storage_t;

    detail::max_align_t hack;
    aligned_storage_t data;

#else
    typedef typename detail::typelist_max< variant_types >::type max_type;

    typedef variant_ALIGN_AS( max_type ) align_as_type;

    typedef struct { align_as_type data[ 1 + ( data_size - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t data;

// #   undef variant_ALIGN_AS

#endif // variant_CONFIG_MAX_ALIGN_HACK

    type_index_t type_index;
};

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool holds_alternative( variant<T0, T1, T2, T3, T4, T5, T6> const & v ) variant_noexcept
{
    return v.index() == v.template index_of<T>();
}

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline R & get( variant<T0, T1, T2, T3, T4, T5, T6> & v, in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6  >
inline R const & get( variant<T0, T1, T2, T3, T4, T5, T6> const & v, in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type &
get( variant<T0, T1, T2, T3, T4, T5, T6> & v, in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    typedef variant_TL7(T0, T1, T2, T3, T4, T5, T6) variant_types;

    return v.template get< typename detail::typelist_type_at< variant_types, I >::type >();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type const &
get( variant<T0, T1, T2, T3, T4, T5, T6> const & v, in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    typedef variant_TL7(T0, T1, T2, T3, T4, T5, T6) variant_types;

    return v.template get< typename detail::typelist_type_at< variant_types, I >::type >();
}

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
#if variant_CPP11_OR_GREATER
inline typename std::add_pointer<T>::type
#else
inline T *
#endif
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv, in_place_type_t(T) = in_place<T> )
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
#if variant_CPP11_OR_GREATER
inline typename std::add_pointer< const T >::type
#else
inline const T *
#endif
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv, in_place_type_t(T) = in_place<T>)
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
#if variant_CPP11_OR_GREATER
inline typename std::add_pointer< typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
#else
inline typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type *
#endif
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv, in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv ) : variant_nullptr;
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
#if variant_CPP11_OR_GREATER
inline typename std::add_pointer< const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
#else
inline const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type *
#endif
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv, in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv )  : variant_nullptr;
}

template< class T0, class T1, class T2, class T3, class T4, class T5 >
inline void swap(
    variant<T0, T1, T2, T3, T4, T5> & a,
    variant<T0, T1, T2, T3, T4, T5> & b ) variant_noexcept
{
    a.swap( b );
}

// template <class Visitor, class... Variants>
// visit( Visitor&& vis, Variants&&... vars );

// The following visit is restricted  with respect to the standard.
// It uses the common idiom is to return another variant:

template< class Visitor, class Variant >
inline Variant visit( Visitor const & vis, Variant const & v )
{
    if ( v.valueless_by_exception() )
    {
        throw bad_variant_access();
    }

    switch( v.index() )
    {
        case 0: return vis( get<0>( v ) );
        case 1: return vis( get<1>( v ) );
        case 2: return vis( get<2>( v ) );
        case 3: return vis( get<3>( v ) );
        case 4: return vis( get<4>( v ) );
        case 5: return vis( get<5>( v ) );
        case 6: return vis( get<6>( v ) );
        default: return Variant();
    }
}

namespace detail {

template< class Variant >
struct Comparator
{
    static inline bool equal( Variant const & v, Variant const & w )
    {
        switch( v.index() )
        {
            case 0: return get<0>( v ) == get<0>( w );
            case 1: return get<1>( v ) == get<1>( w );
            case 2: return get<2>( v ) == get<2>( w );
            case 3: return get<3>( v ) == get<3>( w );
            case 4: return get<4>( v ) == get<4>( w );
            case 5: return get<5>( v ) == get<5>( w );
            case 6: return get<6>( v ) == get<6>( w );
            default: return false;
        }
    }

    static inline bool less_than( Variant const & v, Variant const & w )
    {
        switch( v.index() )
        {
            case 0: return get<0>( v ) < get<0>( w );
            case 1: return get<1>( v ) < get<1>( w );
            case 2: return get<2>( v ) < get<2>( w );
            case 3: return get<3>( v ) < get<3>( w );
            case 4: return get<4>( v ) < get<4>( w );
            case 5: return get<5>( v ) < get<5>( w );
            case 6: return get<6>( v ) < get<6>( w );
            default: return false;
        }
    }
};

} //namespace detail

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator==(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    if      ( v.index() != w.index()     ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else                                   return detail::Comparator< variant<T0, T1, T2, T3, T4, T5, T6> >::equal( v, w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator!=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v == w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator<(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    if      ( w.valueless_by_exception() ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else if ( v.index() < w.index()      ) return true;
    else if ( v.index() > w.index()      ) return false;
    else                                   return detail::Comparator< variant<T0, T1, T2, T3, T4, T5, T6> >::less_than( v, w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator>(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return w < v;
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator<=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v > w );
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline bool operator>=(
    variant<T0, T1, T2, T3, T4, T5, T6> const & v,
    variant<T0, T1, T2, T3, T4, T5, T6> const & w )
{
    return ! ( v < w );
}

} // namespace variants

using namespace variants;

} // namespace nonstd

#if variant_CPP11_OR_GREATER

// specialize the std::hash algorithm:

namespace std {

template<>
class hash< nonstd::monostate >
{
public:
    std::size_t operator()( nonstd::monostate ) const variant_noexcept
    {
        return 42;
    }
};

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
class hash< nonstd::variant<T0, T1, T2, T3, T4, T5, T6> >
{
public:
    std::size_t operator()( nonstd::variant<T0, T1, T2, T3, T4, T5, T6> const & v ) const variant_noexcept
    {
        return nonstd::variants::detail::hash( v );
    }
};

} //namespace std

#endif // variant_CPP11_OR_GREATER

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 10, 14 )
# pragma warning( pop )
#endif

#endif // NONSTD_VARIANT_LITE_HPP
