// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_VARIANT_LITE_HPP
#define NONSTD_VARIANT_LITE_HPP

#define variant_lite_MAJOR  1
#define variant_lite_MINOR  2
#define variant_lite_PATCH  1

#define variant_lite_VERSION  variant_STRINGIFY(variant_lite_MAJOR) "." variant_STRINGIFY(variant_lite_MINOR) "." variant_STRINGIFY(variant_lite_PATCH)

#define variant_STRINGIFY(  x )  variant_STRINGIFY_( x )
#define variant_STRINGIFY_( x )  #x

// variant-lite configuration:

#define variant_VARIANT_DEFAULT  0
#define variant_VARIANT_NONSTD   1
#define variant_VARIANT_STD      2

#if !defined( variant_CONFIG_SELECT_VARIANT )
# define variant_CONFIG_SELECT_VARIANT  ( variant_HAVE_STD_VARIANT ? variant_VARIANT_STD : variant_VARIANT_NONSTD )
#endif

#ifndef  variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO  0
#endif

#ifndef  variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO  0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef variant_CONFIG_NO_EXCEPTIONS
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#  define variant_CONFIG_NO_EXCEPTIONS  0
# else
#  define variant_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   variant_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define variant_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define variant_CPLUSPLUS  __cplusplus
# endif
#endif

#define variant_CPP98_OR_GREATER  ( variant_CPLUSPLUS >= 199711L )
#define variant_CPP11_OR_GREATER  ( variant_CPLUSPLUS >= 201103L )
#define variant_CPP11_OR_GREATER_ ( variant_CPLUSPLUS >= 201103L )
#define variant_CPP14_OR_GREATER  ( variant_CPLUSPLUS >= 201402L )
#define variant_CPP17_OR_GREATER  ( variant_CPLUSPLUS >= 201703L )
#define variant_CPP20_OR_GREATER  ( variant_CPLUSPLUS >= 202000L )

// Use C++17 std::variant if available and requested:

#if variant_CPP17_OR_GREATER && defined(__has_include )
# if __has_include( <variant> )
#  define variant_HAVE_STD_VARIANT  1
# else
#  define variant_HAVE_STD_VARIANT  0
# endif
#else
# define  variant_HAVE_STD_VARIANT  0
#endif

#define  variant_USES_STD_VARIANT  ( (variant_CONFIG_SELECT_VARIANT == variant_VARIANT_STD) || ((variant_CONFIG_SELECT_VARIANT == variant_VARIANT_DEFAULT) && variant_HAVE_STD_VARIANT) )

//
// in_place: code duplicated in any-lite, expected-lite, optional-lite, value-ptr-lite, variant-lite:
//

#ifndef nonstd_lite_HAVE_IN_PLACE_TYPES
#define nonstd_lite_HAVE_IN_PLACE_TYPES  1

// C++17 std::in_place in <utility>:

#if variant_CPP17_OR_GREATER

#include <utility>

namespace nonstd {

using std::in_place;
using std::in_place_type;
using std::in_place_index;
using std::in_place_t;
using std::in_place_type_t;
using std::in_place_index_t;

#define nonstd_lite_in_place_t(      T)  std::in_place_t
#define nonstd_lite_in_place_type_t( T)  std::in_place_type_t<T>
#define nonstd_lite_in_place_index_t(K)  std::in_place_index_t<K>

#define nonstd_lite_in_place(      T)    std::in_place_t{}
#define nonstd_lite_in_place_type( T)    std::in_place_type_t<T>{}
#define nonstd_lite_in_place_index(K)    std::in_place_index_t<K>{}

} // namespace nonstd

#else // variant_CPP17_OR_GREATER

#include <cstddef>

namespace nonstd {
namespace detail {

template< class T >
struct in_place_type_tag {};

template< std::size_t K >
struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template< class T >
inline in_place_t in_place( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t K >
inline in_place_t in_place( detail::in_place_index_tag<K> = detail::in_place_index_tag<K>() )
{
    return in_place_t();
}

template< class T >
inline in_place_t in_place_type( detail::in_place_type_tag<T> = detail::in_place_type_tag<T>() )
{
    return in_place_t();
}

template< std::size_t K >
inline in_place_t in_place_index( detail::in_place_index_tag<K> = detail::in_place_index_tag<K>() )
{
    return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_t(      T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_type_t( T)  nonstd::in_place_t(&)( nonstd::detail::in_place_type_tag<T>  )
#define nonstd_lite_in_place_index_t(K)  nonstd::in_place_t(&)( nonstd::detail::in_place_index_tag<K> )

#define nonstd_lite_in_place(      T)    nonstd::in_place_type<T>
#define nonstd_lite_in_place_type( T)    nonstd::in_place_type<T>
#define nonstd_lite_in_place_index(K)    nonstd::in_place_index<K>

} // namespace nonstd

#endif // variant_CPP17_OR_GREATER
#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// Use C++17 std::variant:
//

#if variant_USES_STD_VARIANT

#include <functional>   // std::hash<>
#include <variant>

#if ! variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
# define variant_size_V(T)  nonstd::variant_size<T>::value
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(K,T)  typename nonstd::variant_alternative<K,T >::type
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

#else // variant_USES_STD_VARIANT

#include <cstddef>
#include <limits>
#include <new>
#include <utility>

#if variant_CONFIG_NO_EXCEPTIONS
# include <cassert>
#else
# include <stdexcept>
#endif

// variant-lite type and visitor argument count configuration (script/generate_header.py):

#define variant_CONFIG_MAX_TYPE_COUNT  {{NumParams}}
#define variant_CONFIG_MAX_VISITOR_ARG_COUNT  {{VisitorArgs}}

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
#define variant_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Compiler versions:
//
// MSVC++ 6.0  _MSC_VER == 1200 (Visual Studio 6.0)
// MSVC++ 7.0  _MSC_VER == 1300 (Visual Studio .NET 2002)
// MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio .NET 2003)
// MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
// MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 14.1 _MSC_VER >= 1910 (Visual Studio 2017)

#if defined(_MSC_VER ) && !defined(__clang__)
# define variant_COMPILER_MSVC_VER      (_MSC_VER )
# define variant_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define variant_COMPILER_MSVC_VER      0
# define variant_COMPILER_MSVC_VERSION  0
#endif

#define variant_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined(__clang__)
# define variant_COMPILER_CLANG_VERSION  variant_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define variant_COMPILER_CLANG_VERSION  0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define variant_COMPILER_GNUC_VERSION  variant_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define variant_COMPILER_GNUC_VERSION  0
#endif

#if variant_BETWEEN( variant_COMPILER_MSVC_VER, 1300, 1900 )
# pragma warning( push )
# pragma warning( disable: 4345 )   // initialization behavior changed
#endif

// Presence of language and library features:

#define variant_HAVE( feature )  ( variant_HAVE_##feature )

#ifdef _HAS_CPP0X
# define variant_HAS_CPP0X  _HAS_CPP0X
#else
# define variant_HAS_CPP0X  0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for variant-lite:

#if variant_COMPILER_MSVC_VER >= 1900
# undef  variant_CPP11_OR_GREATER
# define variant_CPP11_OR_GREATER  1
#endif

#define variant_CPP11_90   (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1500)
#define variant_CPP11_100  (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1600)
#define variant_CPP11_110  (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1700)
#define variant_CPP11_120  (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1800)
#define variant_CPP11_140  (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1900)
#define variant_CPP11_141  (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1910)

#define variant_CPP14_000  (variant_CPP14_OR_GREATER)
#define variant_CPP17_000  (variant_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define variant_HAVE_CONSTEXPR_11       variant_CPP11_140
#define variant_HAVE_INITIALIZER_LIST   variant_CPP11_120
#define variant_HAVE_NOEXCEPT           variant_CPP11_140
#define variant_HAVE_NULLPTR            variant_CPP11_100
#define variant_HAVE_OVERRIDE           variant_CPP11_140

// Presence of C++14 language features:

#define variant_HAVE_CONSTEXPR_14       variant_CPP14_000

// Presence of C++17 language features:

// no flag

// Presence of C++ library features:

#define variant_HAVE_CONDITIONAL        variant_CPP11_120
#define variant_HAVE_REMOVE_CV          variant_CPP11_120
#define variant_HAVE_STD_ADD_POINTER    variant_CPP11_90
#define variant_HAVE_TYPE_TRAITS        variant_CPP11_90

#define variant_HAVE_TR1_TYPE_TRAITS    (!! variant_COMPILER_GNUC_VERSION )
#define variant_HAVE_TR1_ADD_POINTER    (!! variant_COMPILER_GNUC_VERSION )

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

#if variant_CPP11_OR_GREATER
# include <functional>      // std::hash
#endif

#if variant_HAVE_INITIALIZER_LIST
# include <initializer_list>
#endif

#if variant_HAVE_TYPE_TRAITS
# include <type_traits>
#elif variant_HAVE_TR1_TYPE_TRAITS
# include <tr1/type_traits>
#endif

// Method enabling

#if variant_CPP11_OR_GREATER

#define variant_REQUIRES_0(...) \
    template< bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = 0 >

#define variant_REQUIRES_T(...) \
    , typename = typename std::enable_if< (__VA_ARGS__), nonstd::variants::detail::enabler >::type

#define variant_REQUIRES_R(R, ...) \
    typename std::enable_if< (__VA_ARGS__), R>::type

#define variant_REQUIRES_A(...) \
    , typename std::enable_if< (__VA_ARGS__), void*>::type = nullptr

#endif

//
// variant:
//

namespace nonstd { namespace variants {

// C++11 emulation:

namespace std11 {

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

} // namespace std11

/// type traits C++17:

namespace std17 {

#if variant_CPP17_OR_GREATER

using std::is_swappable;
using std::is_nothrow_swappable;

#elif variant_CPP11_OR_GREATER

namespace detail {

using std::swap;

struct is_swappable
{
    template< typename T, typename = decltype( swap( std::declval<T&>(), std::declval<T&>() ) ) >
    static std::true_type test( int );

    template< typename >
    static std::false_type test(...);
};

struct is_nothrow_swappable
{
    // wrap noexcept(epr) in separate function as work-around for VC140 (VS2015):

    template< typename T >
    static constexpr bool test()
    {
        return noexcept( swap( std::declval<T&>(), std::declval<T&>() ) );
    }

    template< typename T >
    static auto test( int ) -> std::integral_constant<bool, test<T>()>{}

    template< typename >
    static std::false_type test(...);
};

} // namespace detail

// is [nothow] swappable:

template< typename T >
struct is_swappable : decltype( detail::is_swappable::test<T>(0) ){};

template< typename T >
struct is_nothrow_swappable : decltype( detail::is_nothrow_swappable::test<T>(0) ){};

#endif // variant_CPP17_OR_GREATER

} // namespace std17

// detail:

namespace detail {

// for variant_REQUIRES_T():

/*enum*/ class enabler{};

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

template<>
struct typelist_max< nulltype >
{
    enum V { value = 0 } ;
    typedef void type;
};

template< class Head, class Tail >
struct typelist_max< typelist<Head, Tail> >
{
private:
    enum TV { tail_value = size_t( typelist_max<Tail>::value ) };

    typedef typename typelist_max<Tail>::type tail_type;

public:
    enum V { value = (sizeof( Head ) > tail_value) ? sizeof( Head ) : std::size_t( tail_value ) } ;

    typedef typename std11::conditional< (sizeof( Head ) > tail_value), Head, tail_type>::type type;
};

#if variant_CPP11_OR_GREATER

// typelist max alignof element type:

template< class List >
struct typelist_max_alignof;

template<>
struct typelist_max_alignof< nulltype >
{
    enum V { value = 0 } ;
};

template< class Head, class Tail >
struct typelist_max_alignof< typelist<Head, Tail> >
{
private:
    enum TV { tail_value = size_t( typelist_max_alignof<Tail>::value ) };

public:
    enum V { value = (alignof( Head ) > tail_value) ? alignof( Head ) : std::size_t( tail_value ) };
};

#endif

// typelist size (length):

template< class List >
struct typelist_size
{
   enum V { value = 1 };
};

{% for n in range(NumParams) -%}
template<> struct typelist_size< T{{n}} > { enum V { value = 0 }; };
{% endfor %}

template<> struct typelist_size< nulltype > { enum V { value = 0 } ; };

template< class Head, class Tail >
struct typelist_size< typelist<Head, Tail> >
{
    enum V { value = typelist_size<Head>::value + typelist_size<Tail>::value };
};

// typelist index of type:

template< class List, class T >
struct typelist_index_of;

template< class T >
struct typelist_index_of< nulltype, T >
{
    enum V { value = -1 };
};

template< class Tail, class T >
struct typelist_index_of< typelist<T, Tail>, T >
{
    enum V { value = 0 };
};

template< class Head, class Tail, class T >
struct typelist_index_of< typelist<Head, Tail>, T >
{
private:
    enum TV { nextVal = typelist_index_of<Tail, T>::value };

public:
    enum V { value = nextVal == -1 ? -1 : 1 + nextVal } ;
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

template< typename T >
struct alignment_of;

template< typename T >
struct alignment_of_hack
{
    char c;
    T t;
    alignment_of_hack();
};

template< size_t A, size_t S >
struct alignment_logic
{
    enum V { value = A < S ? A : S };
};

template< typename T >
struct alignment_of
{
    enum V { value = alignment_logic<
        sizeof( alignment_of_hack<T> ) - sizeof(T), sizeof(T) >::value };
};

template< typename List, size_t N >
struct type_of_size
{
    typedef typename std11::conditional<
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

#if variant_CPP11_OR_GREATER

template< typename T>
inline std::size_t hash( T const & v )
{
    return std::hash<T>()( v );
}

{% for n in range(NumParams) -%}
inline std::size_t hash( T{{n}} const & ) { return 0; }
{% endfor %}

#endif // variant_CPP11_OR_GREATER

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

    static void destroy( type_index_t index, void * data )
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

    template< std::size_t K, class... Args >
    static type_index_t construct_i( void * data, Args&&... args )
    {
        using type = typename detail::typelist_type_at< variant_types, K >::type;

        construct_t< type >( data, std::forward<Args>(args)... );

        return to_index_t( K );
    }

    static type_index_t move_construct( type_index_t const from_index, void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: new( to_value ) T{{n}}( std::move( *as<T{{n}}>( from_value ) ) ); break;
        {% endfor %}
        }
        return from_index;
    }

    static type_index_t move_assign( type_index_t const from_index, void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: *as<T{{n}}>( to_value ) = std::move( *as<T{{n}}>( from_value ) ); break;
        {% endfor %}
        }
        return from_index;
    }
#endif

    static type_index_t copy_construct( type_index_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: new( to_value ) T{{n}}( *as<T{{n}}>( from_value ) ); break;
        {% endfor %}
        }
        return from_index;
    }

    static type_index_t copy_assign( type_index_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
        {% for n in range(NumParams) -%}
            case {{n}}: *as<T{{n}}>( to_value ) = *as<T{{n}}>( from_value ); break;
        {% endfor %}
        }
        return from_index;
    }
};

} // namespace detail

//
// Variant:
//

template< {{TplParamsList}} >
class variant;

// 19.7.8 Class monostate

class monostate{};

// 19.7.9 monostate relational operators

inline variant_constexpr bool operator< ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator> ( monostate, monostate ) variant_noexcept { return false; }
inline variant_constexpr bool operator<=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator>=( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator==( monostate, monostate ) variant_noexcept { return true;  }
inline variant_constexpr bool operator!=( monostate, monostate ) variant_noexcept { return false; }

// 19.7.4 variant helper classes

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

template< std::size_t K, class T >
struct variant_alternative; /* undefined */

template< std::size_t K, {{TplParamsList}} >
struct variant_alternative< K, variant<{{TplArgsList}}> >
{
    typedef typename detail::typelist_type_at<{{TLMacroName}}({{TplArgsList}}), K>::type type;
};

#if variant_CPP11_OR_GREATER
template< std::size_t K, class T >
using variant_alternative_t = typename variant_alternative<K, T>::type;
#endif

#if ! variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
# define variant_alternative_T(K,T)  typename nonstd::variant_alternative<K,T >::type
#endif

// NTS:implement specializes the std::uses_allocator type trait
// std::uses_allocator<nonstd::variant>

// index of the variant in the invalid state (constant)

#if variant_CPP11_OR_GREATER
variant_constexpr std::size_t variant_npos = static_cast<std::size_t>( -1 );
#else
static const std::size_t variant_npos = static_cast<std::size_t>( -1 );
#endif

#if ! variant_CONFIG_NO_EXCEPTIONS

// 19.7.11 Class bad_variant_access

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

#endif // variant_CONFIG_NO_EXCEPTIONS

// 19.7.3 Class template variant

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
    // 19.7.3.1 Constructors
    
    variant() : type_index( 0 ) { new( ptr() ) T0(); }

    {% for n in range(NumParams) -%}
    variant( T{{n}} const & t{{n}} ) : type_index( {{n}} ) { new( ptr() ) T{{n}}( t{{n}} ); }
    {% endfor %}

#if variant_CPP11_OR_GREATER
    {% for n in range(NumParams) -%}
    variant( T{{n}} && t{{n}} ) : type_index( {{n}} ) { new( ptr() ) T{{n}}( std::move(t{{n}}) ); }
    {% endfor %}
#endif

    variant(variant const & other)
    : type_index( other.type_index )
    {
        (void) helper_type::copy_construct( other.type_index, other.ptr(), ptr() );
    }

#if variant_CPP11_OR_GREATER

    variant( variant && other ) noexcept(
        {% for n in range(NumParams) -%}
        std::is_nothrow_move_constructible<T{{n}}>::value{{')' if loop.last else ' &&'}}
        {% endfor -%}
    : type_index( other.type_index )
    {
        (void) helper_type::move_construct( other.type_index, other.ptr(), ptr() );
    }

    template< std::size_t K >
    using type_at_t = typename detail::typelist_type_at< variant_types, K >::type;

    template< class T, class... Args
        variant_REQUIRES_T( std::is_constructible< T, Args...>::value )
    >
    explicit variant( nonstd_lite_in_place_type_t(T), Args&&... args)
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );
    }

    template< class T, class U, class... Args
        variant_REQUIRES_T( std::is_constructible< T, std::initializer_list<U>&, Args...>::value )
    >
    explicit variant( nonstd_lite_in_place_type_t(T), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );
    }

    template< std::size_t K, class... Args
        variant_REQUIRES_T( std::is_constructible< type_at_t<K>, Args...>::value )
    >
    explicit variant( nonstd_lite_in_place_index_t(K), Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<K>( ptr(), std::forward<Args>(args)... );
    }

    template< size_t K, class U, class... Args
        variant_REQUIRES_T( std::is_constructible< type_at_t<K>, std::initializer_list<U>&, Args...>::value )
    >
    explicit variant( nonstd_lite_in_place_index_t(K), std::initializer_list<U> il, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<K>( ptr(), il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    // 19.7.3.2 Destructor
    
    ~variant()
    {
        if ( ! valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
        }
    }

    // 19.7.3.3 Assignment
    
    variant & operator=( variant const & other )
    {
        return copy_assign( other );
    }

#if variant_CPP11_OR_GREATER

    variant & operator=( variant && other ) noexcept(
        {% for n in range(NumParams) -%}
        std::is_nothrow_move_assignable<T{{n}}>::value{{')' if loop.last else ' &&'}}
        {% endfor -%}
    {
        return move_assign( std::move( other ) );
    }

    {% for n in range(NumParams) -%}
    variant & operator=( T{{n}} &&      t{{n}} ) { return assign_value<{{n}}>( std::move( t{{n}} ) ); }
    {% endfor %}

#endif

    {% for n in range(NumParams) -%}
    variant & operator=( T{{n}} const & t{{n}} ) { return assign_value<{{n}}>( t{{n}} ); }
    {% endfor %}

    std::size_t index() const
    {
        return variant_npos_internal() == type_index ? variant_npos : static_cast<std::size_t>( type_index );
    }

    // 19.7.3.4 Modifiers
    
#if variant_CPP11_OR_GREATER
    template< class T, class... Args
        variant_REQUIRES_T( std::is_constructible< T, Args...>::value )
    >
    T& emplace( Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args>(args)... );

        return *as<T>();
    }

    template< class T, class U, class... Args
        variant_REQUIRES_T( std::is_constructible< T, std::initializer_list<U>&, Args...>::value )
    >
    T& emplace( std::initializer_list<U> il, Args&&... args )
    {
        helper_type::destroy( type_index, ptr() );
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), il, std::forward<Args>(args)... );

        return *as<T>();
    }

    template< size_t K, class... Args
        variant_REQUIRES_T( std::is_constructible< type_at_t<K>, Args...>::value )
    >
    variant_alternative_t<K, variant> & emplace( Args&&... args )
    {
        return this->template emplace< type_at_t<K> >( std::forward<Args>(args)... );
    }

    template< size_t K, class U, class... Args
        variant_REQUIRES_T( std::is_constructible< type_at_t<K>, std::initializer_list<U>&, Args...>::value )
    >
    variant_alternative_t<K, variant> & emplace( std::initializer_list<U> il, Args&&... args )
    {
        return this->template emplace< type_at_t<K> >( il, std::forward<Args>(args)... );
    }

#endif // variant_CPP11_OR_GREATER

    // 19.7.3.5 Value status
    
    bool valueless_by_exception() const
    {
        return type_index == variant_npos_internal();
    }

    // 19.7.3.6 Swap
    
    void swap( variant & other )
#if variant_CPP11_OR_GREATER
        noexcept(
            {% for n in range(NumParams) -%}
            std::is_nothrow_move_constructible<T{{n}}>::value && std17::is_nothrow_swappable<T{{n}}>::value {{('&&' if not loop.last)}}
            {% endfor %}
        )
#endif
    {
        if ( valueless_by_exception() && other.valueless_by_exception() )
        {
            // no effect
        }
        else if ( type_index == other.type_index )
        {
            this->swap_value( type_index, other );
        }
        else
        {
#if variant_CPP11_OR_GREATER
            variant tmp( std::move( *this ) );
            *this = std::move( other );
            other = std::move( tmp );
#else
            variant tmp( *this );
            *this = other;
            other = tmp;
#endif
        }
    }

    //
    // non-standard:
    //

    template< class T >
    static variant_constexpr std::size_t index_of() variant_noexcept
    {
        return to_size_t( detail::typelist_index_of<variant_types, typename std11::remove_cv<T>::type >::value );
    }

    template< class T >
    T & get()
    {
        const std::size_t i = index_of<T>();

#if variant_CONFIG_NO_EXCEPTIONS
        assert( i == index() );
#else
        if ( i != index() )
        {
            throw bad_variant_access();
        }
#endif
        return *as<T>();
    }

    template< class T >
    T const & get() const
    {
        const std::size_t i = index_of<T>();

#if variant_CONFIG_NO_EXCEPTIONS
        assert( i == index() );
#else
        if ( i != index() )
        {
            throw bad_variant_access();
        }
#endif
        return *as<const T>();
    }

    template< std::size_t K >
    typename variant_alternative< K, variant >::type &
    get()
    {
        return this->template get< typename detail::typelist_type_at< variant_types, K >::type >();
    }

    template< std::size_t K >
    typename variant_alternative< K, variant >::type const &
    get() const
    {
        return this->template get< typename detail::typelist_type_at< variant_types, K >::type >();
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

    template< class U >
    static variant_constexpr std::size_t to_size_t( U index )
    {
        return static_cast<std::size_t>( index );
    }

    variant_constexpr type_index_t variant_npos_internal() const variant_noexcept
    {
        return static_cast<type_index_t>( -1 );
    }

    variant & copy_assign( variant const & other )
    {
        if ( valueless_by_exception() && other.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && other.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == other.index() )
        {
            type_index = helper_type::copy_assign( other.type_index, other.ptr(), ptr() );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            type_index = helper_type::copy_construct( other.type_index, other.ptr(), ptr() );
        }
        return *this;
    }

#if variant_CPP11_OR_GREATER

    variant & move_assign( variant && other )
    {
        if ( valueless_by_exception() && other.valueless_by_exception() )
        {
            // no effect
        }
        else if ( ! valueless_by_exception() && other.valueless_by_exception() )
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
        }
        else if ( index() == other.index() )
        {
            type_index = helper_type::move_assign( other.type_index, other.ptr(), ptr() );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            type_index = helper_type::move_construct( other.type_index, other.ptr(), ptr() );
        }
        return *this;
    }

    template< std::size_t K, class T >
    variant & assign_value( T && value )
    {
        if( index() == K )
        {
            *as<T>() = std::forward<T>( value );
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( std::forward<T>( value ) );
            type_index = K;
        }
        return *this;
    }

#endif // variant_CPP11_OR_GREATER

    template< std::size_t K, class T >
    variant & assign_value( T const & value )
    {
        if( index() == K )
        {
            *as<T>() = value;
        }
        else
        {
            helper_type::destroy( type_index, ptr() );
            type_index = variant_npos_internal();
            new( ptr() ) T( value );
            type_index = K;
        }
        return *this;
    }

    void swap_value( type_index_t index, variant & other )
    {
        using std::swap;
        switch( index )
        {
            {% for n in range(NumParams) -%}
            case {{n}}: swap( this->get<{{n}}>(), other.get<{{n}}>() ); break;
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

// 19.7.5 Value access

template< class T, {{TplParamsList}} >
inline bool holds_alternative( variant<{{TplArgsList}}> const & v ) variant_noexcept
{
    return v.index() == variant<{{TplArgsList}}>::template index_of<T>();
}

template< class R, {{TplParamsList}} >
inline R & get( variant<{{TplArgsList}}> & v, nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R) )
{
    return v.template get<R>();
}

template< class R, {{TplParamsList}} >
inline R const & get( variant<{{TplArgsList}}> const & v, nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R) )
{
    return v.template get<R>();
}

template< std::size_t K, {{TplParamsList}} >
inline typename variant_alternative< K, variant<{{TplArgsList}}> >::type &
get( variant<{{TplArgsList}}> & v, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
#if variant_CONFIG_NO_EXCEPTIONS
    assert( K == v.index() );
#else
    if ( K != v.index() )
    {
        throw bad_variant_access();
    }
#endif
    return v.template get<K>();
}

template< std::size_t K, {{TplParamsList}} >
inline typename variant_alternative< K, variant<{{TplArgsList}}> >::type const &
get( variant<{{TplArgsList}}> const & v, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
#if variant_CONFIG_NO_EXCEPTIONS
    assert( K == v.index() );
#else
    if ( K != v.index() )
    {
        throw bad_variant_access();
    }
#endif
    return v.template get<K>();
}

#if variant_CPP11_OR_GREATER

template< class R, {{TplParamsList}} >
inline R && get( variant<{{TplArgsList}}> && v, nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R) )
{
    return std::move(v.template get<R>());
}

template< class R, {{TplParamsList}} >
inline R const && get( variant<{{TplArgsList}}> const && v, nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R) )
{
    return std::move(v.template get<R>());
}

template< std::size_t K, {{TplParamsList}} >
inline typename variant_alternative< K, variant<{{TplArgsList}}> >::type &&
get( variant<{{TplArgsList}}> && v, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
#if variant_CONFIG_NO_EXCEPTIONS
    assert( K == v.index() );
#else
    if ( K != v.index() )
    {
        throw bad_variant_access();
    }
#endif
    return std::move(v.template get<K>());
}

template< std::size_t K, {{TplParamsList}} >
inline typename variant_alternative< K, variant<{{TplArgsList}}> >::type const &&
get( variant<{{TplArgsList}}> const && v, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
#if variant_CONFIG_NO_EXCEPTIONS
    assert( K == v.index() );
#else
    if ( K != v.index() )
    {
        throw bad_variant_access();
    }
#endif
    return std::move(v.template get<K>());
}

#endif // variant_CPP11_OR_GREATER

template< class T, {{TplParamsList}} >
inline typename std11::add_pointer<T>::type
get_if( variant<{{TplArgsList}}> * pv, nonstd_lite_in_place_type_t(T) = nonstd_lite_in_place_type(T) )
{
    return ( pv->index() == variant<{{TplArgsList}}>::template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< class T, {{TplParamsList}} >
inline typename std11::add_pointer<const T>::type
get_if( variant<{{TplArgsList}}> const * pv, nonstd_lite_in_place_type_t(T) = nonstd_lite_in_place_type(T))
{
    return ( pv->index() == variant<{{TplArgsList}}>::template index_of<T>() ) ? &get<T>( *pv ) : variant_nullptr;
}

template< std::size_t K, {{TplParamsList}} >
inline typename std11::add_pointer< typename variant_alternative<K, variant<{{TplArgsList}}> >::type >::type
get_if( variant<{{TplArgsList}}> * pv, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
    return ( pv->index() == K ) ? &get<K>( *pv ) : variant_nullptr;
}

template< std::size_t K, {{TplParamsList}} >
inline typename std11::add_pointer< const typename variant_alternative<K, variant<{{TplArgsList}}> >::type >::type
get_if( variant<{{TplArgsList}}> const * pv, nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K) )
{
    return ( pv->index() == K ) ? &get<K>( *pv )  : variant_nullptr;
}

// 19.7.10 Specialized algorithms

template< {{TplParamsList}}
#if variant_CPP11_OR_GREATER
    variant_REQUIRES_T(
        {% for n in range(NumParams) -%}
        std::is_move_constructible<T{{n}}>::value && std17::is_swappable<T{{n}}>::value {{('&&' if not loop.last)}}
        {% endfor %} )
#endif
>
inline void swap(
    variant<{{TplArgsList}}> & a,
    variant<{{TplArgsList}}> & b ) 
#if variant_CPP11_OR_GREATER
    noexcept( noexcept( a.swap( b ) ) )
#endif
{
    a.swap( b );
}

// 19.7.7 Visitation

// Variant 'visitor' implementation
{% macro SequenceGen(len, delim=', ') -%}
{%- for i in range(len) %}{{delim if not loop.first}}{{caller(i, i + 1)}}{% endfor -%}
{%- endmacro %}
namespace detail
{

template< typename R, typename VT >
struct VisitorApplicatorImpl
{
    template< typename Visitor, typename T >
    static R apply(Visitor const& v, T const& arg)
    {
        return v(arg);
    }
};

template< typename R, typename VT >
struct VisitorApplicatorImpl<R, TX<VT> >
{
    template< typename Visitor, typename T >
    static R apply(Visitor const&, T)
    {
        return R();
    }
};

template<typename R>
struct VisitorApplicator;

template< typename R, typename Visitor, typename V1 >
struct VisitorUnwrapper;

#if variant_CPP11_OR_GREATER
template< size_t NumVars, typename R, typename Visitor, typename ... T >
#else
template< size_t NumVars, typename R, typename Visitor, typename T1, {% call (i0, i1) SequenceGen(VisitorArgs - 1)%}typename T{{i1 + 1}} = S0{% endcall %} >
#endif
struct TypedVisitorUnwrapper;

{% for n in range(VisitorArgs - 1) -%}
template< typename R, typename Visitor, {% call (i0, i1) SequenceGen(n + 1)%}typename T{{i1 + 1}}{% endcall %} >
struct TypedVisitorUnwrapper<{{n + 2}}, R, Visitor, {% call (i0, i1) SequenceGen(n + 1)%}T{{i1 + 1}}{% endcall %}>
{
    const Visitor& visitor;
    {% call (i0, i1) SequenceGen(n + 1, '')%}T{{i1 + 1}} const& val{{i1 + 1}};
    {% endcall %}
    TypedVisitorUnwrapper(const Visitor& visitor_, {% call (i0, i1) SequenceGen(n + 1)%}T{{i1 + 1}} const& val{{i1 + 1}}_{% endcall %})
        : visitor(visitor_)
        {% call (i0, i1) SequenceGen(n + 1, '')%}, val{{i1 + 1}}(val{{i1 + 1}}_)
        {% endcall %}
    {
    }

    template<typename T>
    R operator()(const T& val1) const
    {
        return visitor(val1, {% call (i0, i1) SequenceGen(n + 1)%}val{{i1 + 1}}{% endcall %});
    }
};

{% endfor %}

template<typename R, typename Visitor, typename V2>
struct VisitorUnwrapper
{
    const Visitor& visitor;
    const V2& r;

    VisitorUnwrapper(const Visitor& visitor_, const V2& r_)
        : visitor(visitor_)
        , r(r_)
    {
    }

    {% for n in range(VisitorArgs) %}
    template< {% call (i0, i1) SequenceGen(n + 1)%}typename T{{i1}}{% endcall %} >
    R operator()({% call (i0, i1) SequenceGen(n + 1)%}T{{i1}} const& val{{i1}}{% endcall %}) const
    {
        typedef TypedVisitorUnwrapper<{{n + 2}}, R, Visitor, {% call (i0, i1) SequenceGen(n + 1)%}T{{i1}}{% endcall %}> visitor_type;
        return VisitorApplicator<R>::apply(visitor_type(visitor, {% call (i0, i1) SequenceGen(n + 1)%}val{{i1}}{% endcall %}), r);
    }
    {% endfor %}
};


template<typename R>
struct VisitorApplicator
{
    template<typename Visitor, typename V1>
    static R apply(const Visitor& v, const V1& arg)
    {
        switch( arg.index() )
        {
            {% for n in range(NumParams) -%}
            case {{n}}: return apply_visitor<{{n}}>(v, arg);
            {% endfor %}
            default: return R();
        }
    }

    template<size_t Idx, typename Visitor, typename V1>
    static R apply_visitor(const Visitor& v, const V1& arg)
    {

#if variant_CPP11_OR_GREATER
        typedef typename variant_alternative<Idx, typename std::decay<V1>::type>::type value_type;
#else
        typedef typename variant_alternative<Idx, V1>::type value_type;
#endif
        return VisitorApplicatorImpl<R, value_type>::apply(v, get<Idx>(arg));
    }

#if variant_CPP11_OR_GREATER
    template<typename Visitor, typename V1, typename V2, typename ... V>
    static R apply(const Visitor& v, const V1& arg1, const V2& arg2, const V ... args)
    {
        typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
        Unwrapper unwrapper(v, arg1);
        return apply(unwrapper, arg2, args ...);
    }
#else
    {% for n in range(VisitorArgs - 1) %}
    template< typename Visitor, {% call (i0, i1) SequenceGen(n + 2)%}typename V{{i1}}{% endcall %} >
    static R apply(const Visitor& v, {% call (i0, i1) SequenceGen(n + 2)%}V{{i1}} const& arg{{i1}}{% endcall %})
    {
        typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
        Unwrapper unwrapper(v, arg1);
        return apply(unwrapper, {% call (i0, i1) SequenceGen(n + 1)%}arg{{i1 + 1}}{% endcall %});
    }
    {% endfor %}
#endif
};

#if variant_CPP11_OR_GREATER
template< size_t NumVars, typename Visitor, typename ... V >
struct VisitorImpl
{
    typedef decltype(std::declval<Visitor>()(get<0>(static_cast<const V&>(std::declval<V>()))...)) result_type;
    typedef VisitorApplicator<result_type> applicator_type;
};
#endif
} // detail

#if variant_CPP11_OR_GREATER
// No perfect forwarding here in order to simplify code
template< typename Visitor, typename ... V >
inline auto visit(Visitor const& v, V const& ... vars) -> typename detail::VisitorImpl<sizeof ... (V), Visitor, V... > ::result_type
{
    typedef detail::VisitorImpl<sizeof ... (V), Visitor, V... > impl_type;
    return impl_type::applicator_type::apply(v, vars...);
}
#else
{% for n in range(VisitorArgs) %}
template< typename R, typename Visitor, {% call (i0, i1) SequenceGen(n + 1)%}typename V{{i1}}{% endcall %} >
inline R visit(const Visitor& v, {% call (i0, i1) SequenceGen(n + 1)%}V{{i1}} const& arg{{i1}}{% endcall %})
{
    return detail::VisitorApplicator<R>::apply(v, {% call (i0, i1) SequenceGen(n + 1)%}arg{{i1}}{% endcall %});
}
{% endfor %}
#endif

// 19.7.6 Relational operators

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

// 19.7.12 Hash support

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
        namespace nvd = nonstd::variants::detail;

        switch( v.index() )
        {
            {% for n in range(NumParams) -%}
            case {{n}}: return nvd::hash( {{n}} ) ^ nvd::hash( get<{{n}}>( v ) );
            {% endfor %}
            default: return false;
        }
    }
};

} //namespace std

#endif // variant_CPP11_OR_GREATER

#if variant_BETWEEN( variant_COMPILER_MSVC_VER, 1300, 1900 )
# pragma warning( pop )
#endif

#endif // variant_USES_STD_VARIANT

#endif // NONSTD_VARIANT_LITE_HPP
