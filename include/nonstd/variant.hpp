// ToDo
// visit
// compare
// get<N> at same time as get<type>
// some emplace
// some forwarding ctors (std::initializer_list<U>)
// get<N> at same time as get<type> (temporarily gett<type>)

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

#include <cassert>
#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>
#include <typeinfo>
#include <utility>

#define  variant_lite_VERSION "0.0.0"

// variant-lite feature selection:

#ifndef  variant_FEATURE_MAX_ALIGN_HACK
# define variant_FEATURE_MAX_ALIGN_HACK  0
#endif

#ifndef variant_FEATURE_ALIGN_AS
// used in #if defined(), so no default...
#endif

#ifndef  variant_FEATURE_ALIGN_AS_FALLBACK
# define variant_FEATURE_ALIGN_AS_FALLBACK  double
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

#if variant_CPP11_OR_GREATER
# include <type_traits>
#endif

// Presence of C++11 language features:

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 10
# define variant_HAVE_AUTO  1
# define variant_HAVE_NULLPTR  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 12
# define variant_HAVE_DEFAULT_FUNCTION_TEMPLATE_ARG  1
#endif

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14
# define variant_HAVE_ALIAS_TEMPLATE  1
# define variant_HAVE_CONSTEXPR_11  1
# define variant_HAVE_ENUM_CLASS  1
# define variant_HAVE_EXPLICIT_CONVERSION  1
# define variant_HAVE_INITIALIZER_LIST  1
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

#if variant_CPP11_OR_GREATER || variant_COMPILER_MSVC_VERSION >= 14 || (variant_COMPILER_MSVC_VERSION >= 9 && _HAS_CPP0X)
# define variant_HAVE_CONTAINER_DATA_METHOD  1
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

namespace nonstd {

namespace variant_detail {

// typelist:

#define variant_TL1( T1                         ) variant_detail::typelist< T1, variant_detail::nulltype >
#define variant_TL2( T1, T2                     ) variant_detail::typelist< T1, variant_TL1( T2) >
#define variant_TL3( T1, T2, T3                 ) variant_detail::typelist< T1, variant_TL2( T2, T3 ) >
#define variant_TL4( T1, T2, T3, T4             ) variant_detail::typelist< T1, variant_TL3( T2, T3, T4 ) >
#define variant_TL5( T1, T2, T3, T4, T5         ) variant_detail::typelist< T1, variant_TL4( T2, T3, T4, T5 ) >
#define variant_TL6( T1, T2, T3, T4, T5, T6     ) variant_detail::typelist< T1, variant_TL5( T2, T3, T4, T5, T6 ) >
#define variant_TL7( T1, T2, T3, T4, T5, T6, T7 ) variant_detail::typelist< T1, variant_TL6( T2, T3, T4, T5, T6, T7 ) >

template< bool condition, class Then, class Else > 
struct select;

template< class Then, class Else > 
struct select< true , Then, Else > { typedef Then type; };

template< class Then, class Else > 
struct select< false, Then, Else > { typedef Else type; };

// variant parameter unused type tags:

struct T0{};
struct T1{}; 
struct T2{}; 
struct T3{}; 
struct T4{}; 
struct T5{}; 
struct T6{}; 

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
    enum { value = sizeof( Head ) > tail_value ? sizeof( Head ) : tail_value } ;

    typedef typename select< (sizeof( Head ) > tail_value), Head, tail_type>::type type;
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
    enum { value = alignof( Head ) > tail_value ? alignof( Head ) : tail_value };
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

#if variant_FEATURE_MAX_ALIGN_HACK

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

#elif defined( variant_FEATURE_ALIGN_AS ) // variant_FEATURE_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define variant_ALIGN_AS( unused ) \
    variant_FEATURE_ALIGN_AS

#else // variant_FEATURE_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define variant_ALIGN_AS( to_align ) \
    typename variant_detail::type_of_size< variant_detail::alignment_types, variant_detail::alignment_of< to_align >::value >::type

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
    typedef typename select<
        N == sizeof( typename List::head ),
            typename List::head,
            typename type_of_size<typename List::tail, N >::type >::type type;
};

template< size_t N >
struct type_of_size< nulltype, N >
{
    typedef variant_FEATURE_ALIGN_AS_FALLBACK type;
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

#endif // variant_FEATURE_MAX_ALIGN_HACK

template< typename T>
inline std::size_t hash_code( T const & v )
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
    static int construct_t( void * data, Args&&... args )
    {
        new( data ) T( std::forward<Args...>( args... ) ); 
        
        return variant_detail::typelist_index_of< variant_types, T>::value;
    }

    template< std::size_t I, class... Args >
    static int construct_i( void * data, Args&&... args )
    {
        using type = typename variant_detail::typelist_type_at< variant_types, I >::type;
        
        construct_t< type >( data, std::forward<Args...>( args... ) );
        
        return I;
    }

    static int move( std::size_t const from_index, const void * from_value, void * to_value )
    {
        switch ( from_index )
        {
            case 0: new( to_value ) T0( std::move( *as<T0>( from_value ) ) ); break;
            case 1: new( to_value ) T1( std::move( *as<T1>( from_value ) ) ); break;
            case 2: new( to_value ) T2( std::move( *as<T2>( from_value ) ) ); break;
            case 3: new( to_value ) T3( std::move( *as<T3>( from_value ) ) ); break;
            case 4: new( to_value ) T4( std::move( *as<T4>( from_value ) ) ); break;
            case 5: new( to_value ) T5( std::move( *as<T5>( from_value ) ) ); break;
            case 6: new( to_value ) T6( std::move( *as<T6>( from_value ) ) ); break;
        }
        return from_index;
    }
#endif

    static int copy( std::size_t const from_index, const void * from_value, void * to_value )
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
        return from_index;
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

#if variant_CPP11_OR_GREATER

struct in_place_tag { in_place_tag() = delete; };

in_place_tag in_place( /* unspecified */ );

template < class T >
in_place_tag in_place( /* unspecified */ /*<T>*/ );

template < std::size_t I >
in_place_tag in_place( /* unspecified */ /*<I>*/ );

using in_place_t = in_place_tag (&)( /* unspecified */ );

template < class T >
using in_place_type_t = in_place_tag (&)( /* unspecified */ /*<T>*/ );

template < std::size_t I >
using in_place_index_t = in_place_tag (&)( /* unspecified */ /*<I>*/ );

#endif

// obtain the size of the variant's list of alternatives at compile time

template< class T >
struct variant_size; /* undefined */

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_size< variant<T0, T1, T2, T3, T4, T5, T6> >
{
    enum { value = variant_detail::typelist_size< variant_TL7(T0, T1, T2, T3, T4, T5, T6) >::value };
};

#if variant_CPP14_OR_GREATER
template< class T >
constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

// obtain the type of the alternative specified by its index, at compile time:

template< std::size_t I, class T >
struct variant_alternative; /* undefined */

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
struct variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >
{
    typedef typename variant_detail::typelist_type_at<variant_TL7(T0, T1, T2, T3, T4, T5, T6), I>::type type;
};

#if variant_CPP11_OR_GREATER
template< std::size_t I, class T >
using variant_alternative_t = typename variant_alternative<I, T>::type;
#endif

// NTS:implement specializes the std::uses_allocator type trait
// std::uses_allocator<nonstd::variant>

// index of the variant in the invalid state (constant)

#if variant_CPP11_OR_GREATER
variant_constexpr std::size_t variant_npos = -1;
#else
enum { variant_npos = -1 };
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
    class T1 = variant_detail::T1,
    class T2 = variant_detail::T2,
    class T3 = variant_detail::T3,
    class T4 = variant_detail::T4,
    class T5 = variant_detail::T5,
    class T6 = variant_detail::T6
>
class variant
{
    typedef variant_detail::helper<T0, T1, T2, T3, T4, T5, T6> helper_type;

public:
    variant() { new( ptr() ) T0(); type_index = 0; }

    variant( T0 const & t0 ) { new( ptr() ) T0( t0 ); type_index = 0; }
    variant( T1 const & t1 ) { new( ptr() ) T1( t1 ); type_index = 1; }
    variant( T2 const & t2 ) { new( ptr() ) T2( t2 ); type_index = 2; }
    variant( T3 const & t3 ) { new( ptr() ) T3( t3 ); type_index = 3; }
    variant( T4 const & t4 ) { new( ptr() ) T4( t4 ); type_index = 4; }
    variant( T5 const & t5 ) { new( ptr() ) T5( t5 ); type_index = 5; }
    variant( T6 const & t6 ) { new( ptr() ) T6( t6 ); type_index = 6; }

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
    variant_constexpr14 explicit variant( in_place_type_t<T>, Args&&... args)
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_t<T>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template< class T, class U, class... Args >
    variant_constexpr explicit variant( in_place_type_t<T>, std::initializer_list<U> il, Args&&... args );

    template< std::size_t I, class... Args >
    variant_constexpr14 explicit variant( in_place_index_t<I>, Args&&... args )
    {
        type_index = variant_npos_internal();
        type_index = helper_type::template construct_i<I>( ptr(), std::forward<Args...>( args... ) );
    }

    // NTS:implement
    template <size_t I, class U, class... Args>
    variant_constexpr explicit variant( in_place_index_t<I>, std::initializer_list<U> il, Args&&... args );

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

    variant_constexpr std::size_t index() const
    {
        return variant_npos_internal() == type_index ? variant_npos : type_index;
    }

    variant_constexpr bool valueless_by_exception() const
    {
        return type_index == variant_npos_internal();
    }

    // NTS:implement
    bool operator==( variant const & rhs ) const
    {
        assert( valid() && rhs.valid() );

        if ( this->index() != rhs.index() )
        {
            return false;
        }
//        variant_detail::comparer<variant, variant_detail::equal_comp> visitor( *this );
//
//        return visit( rhs, visitor );
        return false; // NTS:implement
    }

    bool operator!=( variant const & rhs ) const
    {
        return ! ( *this == rhs );
    }

    // NTS:implement
    bool operator<(variant const& rhs) const
    {
        assert( valid() && rhs.valid() );

        if ( this->index() != rhs.index() )
        {
            return this->index() < rhs.index();
        }
//        variant_detail::comparer<variant, variant_detail::less_comp> visitor( *this );
//
//        return visit( rhs, visitor );
        return false; // NTS:implement
    }

    bool operator>( variant const & rhs ) const
    {
        return rhs < *this;
    }

    bool operator<=( variant const & rhs ) const
    {
        return ! ( *this > rhs );
    }

    bool operator>=( variant const & rhs ) const
    {
        return ! ( *this < rhs );
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

    // non-standard:

    std::size_t hash() const
    {
        return hash_code( *this );
    }
    
    template< class T >
    variant_constexpr std::size_t index_of() const variant_noexcept
    {
        return variant_detail::typelist_index_of<variant_types, T>::value;
    }

    // NTS:remove
    variant_constexpr int variant_types_max_f() const variant_noexcept
    {
        return data_size;
    }

private:
    typedef signed char type_index_t;

    void * ptr() variant_noexcept
    { 
        return &data; 
    }

    void const * ptr() const variant_noexcept
    { 
        return &data; 
    }

    bool valid() const variant_noexcept
    { 
        return type_index != variant_npos_internal(); 
    }

    template< class U >
    U * as()
    {
        return reinterpret_cast<U*>( ptr() );
    }

    variant_constexpr std::size_t max_index() const variant_noexcept
    {
        return data_size;
    }

    type_index_t variant_npos_internal() const variant_noexcept
    {
        return std::numeric_limits<type_index_t>::min();
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

    enum { data_size  = variant_detail::typelist_max< variant_types >::value };

#if variant_CPP11_OR_GREATER

    enum { data_align = variant_detail::typelist_max_alignof< variant_types >::value };
    
    using aligned_storage_t = typename std::aligned_storage< data_size, data_align >::type;
    aligned_storage_t data;

#elif variant_FEATURE_MAX_ALIGN_HACK

    typedef struct { unsigned char data[ data_size ]; } aligned_storage_t;

    variant_detail::max_align_t hack;
    aligned_storage_t data;

#else
    typedef typename variant_detail::typelist_max< variant_types >::type max_type;
    
    typedef variant_ALIGN_AS( max_type ) align_as_type;

    typedef struct { align_as_type data[ 1 + ( data_size - 1 ) / sizeof(align_as_type) ]; } aligned_storage_t;
    aligned_storage_t data;

// #   undef variant_ALIGN_AS

#endif // variant_FEATURE_MAX_ALIGN_HACK

    type_index_t type_index;
};


// NTS:implement
//template <class Visitor, class... Variants>
//variant_constexpr visit( Visitor&& vis, Variants&&... vars );

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline variant_constexpr bool holds_alternative( variant<T0, T1, T2, T3, T4, T5, T6> const & v ) variant_noexcept
{
    return v.index() == v.template index_of<T>();
}

// NTS:select R/I <=================================

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline R & gett( variant<T0, T1, T2, T3, T4, T5, T6> & v )
{
    return v.template get<R>();
}

template< class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6  >
inline R const & gett( variant<T0, T1, T2, T3, T4, T5, T6> const & v )
{
    return v.template get<R>();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type & 
get( variant<T0, T1, T2, T3, T4, T5, T6> & v )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    typedef variant_TL7(T0, T1, T2, T3, T4, T5, T6) variant_types;
    
    return v.template get< typename variant_detail::typelist_type_at< variant_types, I >::type >();
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline typename variant_alternative< I, variant<T0, T1, T2, T3, T4, T5, T6> >::type const & 
get( variant<T0, T1, T2, T3, T4, T5, T6> const & v )
{
    if ( I != v.index() )
    {
        throw bad_variant_access();
    }

    typedef variant_TL7(T0, T1, T2, T3, T4, T5, T6) variant_types;
    
    return v.template get< typename variant_detail::typelist_type_at< variant_types, I >::type >();
}

// NTS:implement

#if variant_CPP11_OR_GREATER

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline variant_constexpr typename std::add_pointer< typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv )
{
    return ( pv->index() == I ) ? &pv-get<I>() : NULL;
}

template< std::size_t I, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline variant_constexpr typename std::add_pointer< const typename variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6> >::type >::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv )
{
    return ( pv->index() == I ) ? &pv-get<I>() : NULL;
}

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline variant_constexpr typename std::add_pointer<T>::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> * pv );

template< class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline variant_constexpr typename std::add_pointer< const T >::type
get_if( variant<T0, T1, T2, T3, T4, T5, T6> const * pv );

#endif // variant_CPP11_OR_GREATER

template< class T0, class T1, class T2, class T3, class T4, class T5 >
inline void swap(
    variant<T0, T1, T2, T3, T4, T5> & a,
    variant<T0, T1, T2, T3, T4, T5> & b ) variant_noexcept
{
    a.swap( b );
}

} // namespace nonstd

#if variant_CPP11_OR_GREATER

// specialize the std::hash algorithm:

namespace std {

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
class hash< nonstd::variant<T0, T1, T2, T3, T4, T5, T6> >
{
    std::size_t operator()( nonstd::variant<T0, T1, T2, T3, T4, T5, T6> const & v ) const
    {
        return v.hash();
    }
};
}
#endif // variant_CPP11_OR_GREATER

#endif // NONSTD_VARIANT_LITE_HPP
