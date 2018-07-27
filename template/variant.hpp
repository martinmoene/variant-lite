// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_VARIANT_LITE_HPP
#define NONSTD_VARIANT_LITE_HPP

#define  variant_lite_VERSION "1.0.0"

// variant-lite configuration:

#ifndef  variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO  0
#endif

#ifndef  variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO  0
#endif

// Compiler detection:

#ifdef _MSVC_LANG
# define variant_MSVC_LANG  _MSVC_LANG
#else
# define variant_MSVC_LANG  0
#endif

#define variant_CPP11_OR_GREATER  ( __cplusplus >= 201103L || variant_MSVC_LANG >= 201103L )
#define variant_CPP14_OR_GREATER  ( __cplusplus >= 201402L || variant_MSVC_LANG >= 201703L )
#define variant_CPP17_OR_GREATER  ( __cplusplus >= 201703L || variant_MSVC_LANG >= 201703L )

// use C++17 std::variant if available:

#if defined( __has_include )
# define variant_HAS_INCLUDE( arg )  __has_include( arg )
#else
# define variant_HAS_INCLUDE( arg )  0
#endif

#if variant_HAS_INCLUDE( <variant> ) && variant_CPP17_OR_GREATER

#define variant_HAVE_STD_VARIANT  1

#include <variant>

#if ! variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(I,T)  typename nonstd::variant_alternative<I,T >::type
#endif

namespace nonstd {

    using std::variant;
    using std::monostate;
    using std::bad_variant_access;
    using std::variant_size;
    using std::variant_size_v;
    using std::variant_alternative;
    using std::variant_alternative_t;
    using std::hash;

    using std::in_place;
    using std::in_place_type;
    using std::in_place_index;
    using std::in_place_t;
    using std::in_place_type_t;
    using std::in_place_index_t;

    using std::visit;
    using std::holds_alternative;
    using std::get;
    using std::get_if;
    using std::operator==;
    using std::operator!=;
    using std::operator<;
    using std::operator<=;
    using std::operator>;
    using std::operator>=;
    using std::swap;

    constexpr auto variant_npos = std::variant_npos;

}

#else // C++17 std::variant

#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>
#include <utility>

// variant-lite alignment configuration:

#ifndef  variant_CONFIG_MAX_ALIGN_HACK
# define variant_CONFIG_MAX_ALIGN_HACK  0
#endif

#ifndef  variant_CONFIG_ALIGN_AS
// no default, used in #if defined()
#endif

#ifndef  variant_CONFIG_ALIGN_AS_FALLBACK
# define variant_CONFIG_ALIGN_AS_FALLBACK  double
#endif

// half-open range [lo..hi):
#define variant_BETWEEN( v, lo, hi ) ( lo <= v && v < hi )

#if defined(_MSC_VER) && !defined(__clang__)
# define variant_COMPILER_MSVC_VERSION   (_MSC_VER / 100 - 5 - (_MSC_VER < 1900))
#else
# define variant_COMPILER_MSVC_VERSION   0
#endif

#if defined __GNUC__
# define variant_COMPILER_GNUC_VERSION  __GNUC__
#else
# define variant_COMPILER_GNUC_VERSION    0
#endif

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 7, 14 )
# pragma warning( push )
# pragma warning( disable: 4345 )   // initialization behavior changed
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
# define variant_HAVE_OVERRIDE  1
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

#if variant_COMPILER_GNUC_VERSION
# define variant_HAVE_TR1_TYPE_TRAITS  1
# define variant_HAVE_TR1_ADD_POINTER  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 9
# define variant_HAVE_TYPE_TRAITS  1
# define variant_HAVE_STD_ADD_POINTER  1
#endif

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

#if variant_HAVE_OVERRIDE
# define variant_override override
#else
# define variant_override /*override*/
#endif

// additional includes:

#if variant_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if variant_HAVE_TYPE_TRAITS
# include <type_traits>
#elif variant_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

//
// in_place: code duplicated in any-lite, optional-lite, variant-lite:
//

#if ! nonstd_lite_HAVE_IN_PLACE_TYPES

namespace nonstd {

namespace detail {

template< class T >
struct in_place_type_tag {};

template< std::size_t I >
struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template< class T >
inline in_place_t in_place( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t I >
inline in_place_t in_place( detail::in_place_index_tag<I> = detail::in_place_index_tag<I>() )
{
    return in_place_t();
}

template< class T >
inline in_place_t in_place_type( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t I >
inline in_place_t in_place_index( detail::in_place_index_tag<I> = detail::in_place_index_tag<I>() )
{
    return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_type_t( T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_index_t(T)  nonstd::in_place_t(&)( nonstd::detail::in_place_index_tag<I> )

#define nonstd_lite_HAVE_IN_PLACE_TYPES  1

} // namespace nonstd

#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// variant:
//

namespace nonstd { namespace variants {

namespace detail {

// C++11 emulation:

#if variant_HAVE_STD_ADD_POINTER

using std::add_pointer;

#elif variant_HAVE_TR1_ADD_POINTER

using std::tr1::add_pointer;

#else

template< class T > struct remove_reference     { typedef T type; };
template< class T > struct remove_reference<T&> { typedef T type; };

template< class T > struct add_pointer
{
    typedef typename remove_reference<T>::type * type;
};

#endif // variant_HAVE_STD_ADD_POINTER

#if variant_HAVE_REMOVE_CV

using std::remove_cv;

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

#if variant_HAVE_CONDITIONAL

using std::conditional;

#else

template< bool Cond, class Then, class Else >
struct conditional;

template< class Then, class Else >
struct conditional< true , Then, Else > { typedef Then type; };

template< class Then, class Else >
struct conditional< false, Then, Else > { typedef Else type; };

#endif // variant_HAVE_CONDITIONAL

// typelist:

#define variant_TL1( T1 ) detail::typelist< T1, detail::nulltype >
{% for n in range(2, NumParams + 1) -%}
#define variant_TL{{n}}( {% for i in range(n) %}T{{i + 1}}{{', ' if not loop.last}}{%endfor%}) detail::typelist< T1, variant_TL{{n - 1}}( {% for i in range(2, n + 1) %}T{{i}}{{', ' if not loop.last}}{%endfor%}) >
{% endfor %}

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

{% for n in range(NumParams) -%}
struct S{{n}}{}; typedef TX<S{{n}}> T{{n}};
{% endfor %}

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

// typelist size (length):

template< class List >
struct typelist_size
{
   enum { value = 1 };
};

{% for n in range(NumParams) -%}
template<> struct typelist_size< T{{n}} > { enum { value = 0 }; };
{% endfor %}

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

{% set TplArgsList %}{% for n in range(NumParams) %}T{{n ~ (', ' if not loop.last)}}{% endfor %}{% endset %}
{% set TplParamsList %}{% for n in range(NumParams) %}class T{{n ~ (', ' if not loop.last)}}{% endfor %}{% endset %}
{% set TLMacroName = 'variant_TL' ~ NumParams %}

template< {{TplParamsList}} >
struct helper
{
    typedef signed char type_index_t;
    typedef {{TLMacroName}}( {{TplArgsList}} ) variant_types;

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
        {% for n in range(NumParams) -%}
            case {{n}}: as<T{{n}}>( data )->~T{{n}}(); break;
        {% endfor %}
        }
    }

#if variant_CPP11_OR_GREATER
    template< class T, class... Args >
    static type_index_t construct_t( void * data, Args&&... args )
    {
        new( data ) T( std::forward<Args>(args)... );

        return to_index_t( detail::typelist_index_of< variant_types, T>::value );
    }

    template< std::size_t I, class... Args >
    static type_index_t construct_i( void * data, Args&&... args )
    {
        using type = typename detail::typelist_type_at< variant_types, I >::type;

        construct_t< type >( data, std::forward<Args>(args)... );

        return to_index_t( I );
    }

    static type_index_t move( std::size_t const from_index, void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: new( to_value ) T{{n}}( std::forward<T{{n}}>( *as<T{{n}}>( from_value ) ) ); break;
        {% endfor %}
        }
        return to_index_t( from_index );
    }
#endif

    static type_index_t copy( std::size_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: new( to_value ) T{{n}}( *as<T{{n}}>( from_value ) ); break;
        {% endfor %}
        }
        return to_index_t( from_index );
    }
};

} // namespace detail

//
// Variant:
//

template< {{TplParamsList}} >
class variant;

class monostate{};

inline variant_constexpr bool operator< ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator> ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator<=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator>=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator==( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator!=( monostate, monostate ) variant_noexcept { return false; }

// obtain the size of the variant's list of alternatives at compile time

template< class T >
struct variant_size; /* undefined */

template< {{TplParamsList}} >
struct variant_size< variant<{{TplArgsList}}> >
{
    enum _ { value = detail::typelist_size< {{TLMacroName}}({{TplArgsList}}) >::value };
};

#if variant_CPP14_OR_GREATER
template< class T >
constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

#if ! variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

// obtain the type of the alternative specified by its index, at compile time:

template< std::size_t I, class T >
struct variant_alternative; /* undefined */

template< std::size_t I, {{TplParamsList}} >
struct variant_alternative< I, variant<{{TplArgsList}}> >
{
    typedef typename detail::typelist_type_at<{{TLMacroName}}({{TplArgsList}}), I>::type type;
};

#if variant_CPP11_OR_GREATER
template< std::size_t I, class T >
using variant_alternative_t = typename variant_alternative<I, T>::type;
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
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
public:
#if variant_CPP11_OR_GREATER
    virtual const char* what() const variant_noexcept variant_override
#else
    virtual const char* what() const throw()
#endif
    {
        return "bad variant access";
    }
};

template<
    class T0,
    {% for n in range(1, NumParams) -%}
    class T{{n}} = detail::T{{n ~ (',' if not loop.last)}}
    {% endfor -%}
>
class variant
{
    typedef detail::helper< {{TplArgsList}} > helper_type;
    typedef {{TLMacroName}}( {{TplArgsList}} ) variant_types;

public:
    variant() : type_index( 0 ) { new( ptr() ) T0(); }

    {% for n in range(NumParams) -%}
    variant( T{{n}} const & t{{n}} ) : type_index( {{n}} ) { new( ptr() ) T{{n}}( t{{n}} ); }
    {% endfor %}

#if variant_CPP11_OR_GREATER
    {% for n in range(NumParams) -%}
    variant( T{{n}} && t{{n}} ) : type_index( {{n}} ) { new( ptr() ) T{{n}}( std::move(t{{n}}) ); }
    {% endfor %}
#endif

    variant(variant const & rhs)
    : type_index( rhs.type_index )
    {
        (void) helper_type::copy( rhs.type_index, rhs.ptr(), ptr() );
    }

#if variant_CPP11_OR_GREATER

    variant( variant && rhs )
    : type_index( rhs.type_index )
    {
        (void) helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
    }

    template< std::size_t I >
    using type_at_t = typename detail::typelist_type_at< variant_types, I >::type;

    template< class T, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_type_t(T), Args&&... args)
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );
    }

    template< class T, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, std::initializer_list<U>&, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_type_t(T), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );
    }

    template< std::size_t I, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, Args...>::value>::type >
    explicit variant( nonstd_lite_in_place_index_t(I), Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), std::forward<Args>(args)... );
    }

    template <size_t I, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, std::initializer_list<U>&, Args...>::value >::type >
    explicit variant( nonstd_lite_in_place_index_t(I), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    ~variant()
    {
        helper_type::destroy( index(), ptr() );
    }

    variant & operator=( variant const & rhs )
    {
        return copy_assign( rhs );
    }

#if variant_CPP11_OR_GREATER

    variant & operator=( variant && rhs )
    {
        return move_assign( std::forward<variant>( rhs ) );
    }

    {% for n in range(NumParams) -%}
    variant & operator=( T{{n}} &&      t{{n}} ) { return move_assign_value<T{{n}},{{n}}>( std::forward<T{{n}}>( t{{n}} ) ); }
    {% endfor %}
#else
    {% for n in range(NumParams) -%}
    variant & operator=( T{{n}} const & t{{n}} ) { return copy_assign_value<T{{n}},{{n}}>( t{{n}} ); }
    {% endfor %}
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
    template< class T, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, Args...>::value>::type >
    void emplace( Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );
    }

    template< class T, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< T, std::initializer_list<U>&, Args...>::value>::type >
    void emplace( std::initializer_list<U> il, Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );
    }

    template< size_t I, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, Args...>::value>::type >
    void emplace( Args&&... args )
    {
        this->template emplace< type_at_t<I> >( std::forward<Args>(args)... );
    }

    template< size_t I, class U, class... Args,
        typename = typename std::enable_if< std::is_constructible< type_at_t<I>, std::initializer_list<U>&, Args...>::value >::type >
    void emplace( std::initializer_list<U> il, Args&&... args )
    {
        this->template emplace< type_at_t<I> >( il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    void swap( variant & rhs ) variant_noexcept
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( index() == rhs.index() )
        {
            this->swap_value( index(), rhs );
        }
        else
        {
            variant tmp( *this );
            *this = rhs;
            rhs = tmp;
        }
    }

    //
    // non-standard:
    //

    template< class T >
    variant_constexpr std::size_t index_of() const variant_noexcept
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

    template< std::size_t I >
    typename variant_alternative< I, variant >::type &
    get()
    {
        return this->template get< typename detail::typelist_type_at< variant_types, I >::type >();
    }

    template< std::size_t I >
    typename variant_alternative< I, variant >::type const &
    get() const
    {
        return this->template get< typename detail::typelist_type_at< variant_types, I >::type >();
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

    variant & copy_assign( variant const & rhs )
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && rhs.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == rhs.index() )
        {
            type_index = helper_type::copy( rhs.type_index, rhs.ptr(), ptr() );
        }
        else
        {
            // alas exception safety with pre-C++11 needs an extra copy:

            variant tmp( rhs );
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
#if variant_CPP11_OR_GREATER
            type_index = helper_type::move( rhs.type_index, tmp.ptr(), ptr() );
#else
            type_index = helper_type::copy( rhs.type_index, tmp.ptr(), ptr() );
#endif
        }
        return *this;
    }

#if variant_CPP11_OR_GREATER

    variant & move_assign( variant && rhs )
    {
        if ( valueless_by_exception() && rhs.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && rhs.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == rhs.index() )
        {
            type_index = helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            type_index = helper_type::move( rhs.type_index, rhs.ptr(), ptr() );
        }
        return *this;
    }

    template< class T, std::size_t I >
    variant & move_assign_value( T && value )
    {
        if( index() == I )
        {
            *as<T>() = std::forward<T>( value );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( std::forward<T>( value ) );
            type_index = I;
        }
        return *this;
    }
#else
    template< class T, std::size_t I >
    variant & copy_assign_value( T const & value )
    {
        if( index() == I )
        {
            *as<T>() = value;
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( value );
            type_index = I;
        }
        return *this;
    }

#endif // variant_CPP11_OR_GREATER

    void swap_value( std::size_t index, variant & rhs )
    {
        using std::swap;
        switch( index )
        {
            {% for n in range(NumParams) -%}
            case {{n}}: swap( this->get<{{n}}>(), rhs.get<{{n}}>() ); break;
            {% endfor %}
        }
    }

private:
    enum { data_size  = detail::typelist_max< variant_types >::value };

#if variant_CPP11_OR_GREATER

    enum { data_align = detail::typelist_max_alignof< variant_types >::value };

    using aligned_storage_t = typename std::aligned_storage< data_size, data_align >::type;
    aligned_storage_t data;

#elif variant_CONFIG_MAX_ALIGN_HACK

    typedef union { unsigned char data[ data_size ]; } aligned_storage_t;

    detail::max_align_t hack;
    aligned_storage_t data;

#else
    typedef typename detail::typelist_max< variant_types >::type max_type;

    typedef variant_ALIGN_AS( max_type ) align_as_type;

    typedef union { align_as_type data[ 1 + ( data_size - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t data;

// #   undef variant_ALIGN_AS

#endif // variant_CONFIG_MAX_ALIGN_HACK

    type_index_t type_index;
};

template <class T, {{TplParamsList}} >
inline bool holds_alternative( variant<{{TplArgsList}}> const & v ) variant_noexcept
{
    return v.index() == v.template index_of<T>();
}

template< class R, {{TplParamsList}} >
inline R & get( variant<{{TplArgsList}}> & v, nonstd_lite_in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< class R, {{TplParamsList}} >
inline R const & get( variant<{{TplArgsList}}> const & v, nonstd_lite_in_place_type_t(R) = in_place<R> )
{
    return v.template get<R>();
}

template< std::size_t I, {{TplParamsList}} >
inline typename variant_alternative< I, variant<{{TplArgsList}}> >::type &
get( variant<{{TplArgsList}}> & v, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    return v.template get<I>();
}

template< std::size_t I, {{TplParamsList}} >
inline typename variant_alternative< I, variant<{{TplArgsList}}> >::type const &
get( variant<{{TplArgsList}}> const & v, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    return v.template get<I>();
}

template< class T, {{TplParamsList}} >
inline typename detail::add_pointer<T>::type
get_if( variant<{{TplArgsList}}> * pv, nonstd_lite_in_place_type_t(T) = in_place<T> )
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< class T, {{TplParamsList}} >
inline typename detail::add_pointer<const T>::type
get_if( variant<{{TplArgsList}}> const * pv, nonstd_lite_in_place_type_t(T) = in_place<T>)
{
    return ( pv->index() == pv->template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< std::size_t I, {{TplParamsList}} >
inline typename detail::add_pointer< typename variant_alternative<I, variant<{{TplArgsList}}> >::type >::type
get_if( variant<{{TplArgsList}}> * pv, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv ) : variant_nullptr;
}

template< std::size_t I, {{TplParamsList}} >
inline typename detail::add_pointer< const typename variant_alternative<I, variant<{{TplArgsList}}> >::type >::type
get_if( variant<{{TplArgsList}}> const * pv, nonstd_lite_in_place_index_t(I) = in_place<I> )
{
    return ( pv->index() == I ) ? &get<I>( *pv )  : variant_nullptr;
}

template< {{TplParamsList}} >
inline void swap(
    variant<{{TplArgsList}}> & a,
    variant<{{TplArgsList}}> & b ) variant_noexcept
{
    a.swap( b );
}

// template <class Visitor, class... Variants>
// visit( Visitor&& vis, Variants&&... vars );

// The following visit is restricted  with respect to the standard.
// It uses the common idiom is to return anrhs variant:

template< class Visitor, class Variant >
inline Variant visit( Visitor const & vis, Variant const & v )
{
    if ( v.valueless_by_exception() )
    {
        throw bad_variant_access();
    }

    switch( v.index() )
    {
        {% for n in range(NumParams) -%}
        case {{n}}: return vis( get<{{n}}>( v ) );
        {% endfor %}
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
            {% for n in range(NumParams) -%}
            case {{n}}: return get<{{n}}>( v ) == get<{{n}}>( w );
            {% endfor %}
            default: return false;
        }
    }

    static inline bool less_than( Variant const & v, Variant const & w )
    {
        switch( v.index() )
        {
            {% for n in range(NumParams) -%}
            case {{n}}: return get<{{n}}>( v ) < get<{{n}}>( w );
            {% endfor %}
            default: return false;
        }
    }
};

} //namespace detail

template< {{TplParamsList}} >
inline bool operator==(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
{
    if      ( v.index() != w.index()     ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else                                   return detail::Comparator< variant<{{TplArgsList}}> >::equal( v, w );
}

template< {{TplParamsList}} >
inline bool operator!=(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
{
    return ! ( v == w );
}

template< {{TplParamsList}} >
inline bool operator<(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
{
    if      ( w.valueless_by_exception() ) return false;
    else if ( v.valueless_by_exception() ) return true;
    else if ( v.index() < w.index()      ) return true;
    else if ( v.index() > w.index()      ) return false;
    else                                   return detail::Comparator< variant<{{TplArgsList}}> >::less_than( v, w );
}

template< {{TplParamsList}} >
inline bool operator>(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
{
    return w < v;
}

template< {{TplParamsList}} >
inline bool operator<=(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
{
    return ! ( v > w );
}

template< {{TplParamsList}} >
inline bool operator>=(
    variant<{{TplArgsList}}> const & v,
    variant<{{TplArgsList}}> const & w )
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
struct hash< nonstd::monostate >
{
    std::size_t operator()( nonstd::monostate ) const variant_noexcept
    {
        return 42;
    }
};

template< {{TplParamsList}} >
struct hash< nonstd::variant<{{TplArgsList}}> >
{
    std::size_t operator()( nonstd::variant<{{TplArgsList}}> const & v ) const variant_noexcept
    {
        return nonstd::variants::detail::hash( v );
    }
};

} //namespace std

#endif // variant_CPP11_OR_GREATER

#if variant_BETWEEN(variant_COMPILER_MSVC_VERSION, 10, 14 )
# pragma warning( pop )
#endif

#endif // have C++17 std::variant

#endif // NONSTD_VARIANT_LITE_HPP
