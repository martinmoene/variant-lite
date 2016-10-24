#include "variant.hpp"

#include <iostream>
#include <string>

namespace nonstd {

template< class  T >
inline std::ostream & operator<<( std::ostream & os, nonstd::detail::TX<T> const & v )
{
    return os << "[variant:tx]";
}

template< class T0, class T1, class T2, class T3, class T4, class T5, class T6 >
inline std::ostream & operator<<( std::ostream & os, nonstd::variant<T0, T1, T2, T3, T4, T5, T6> const & v )
{
    os << "[variant:";
    switch( v.index() )
    {
        case 0: os << get<0>( v ); break;
        case 1: os << get<1>( v ); break;
        case 2: os << get<2>( v ); break;
        case 3: os << get<3>( v ); break;
        case 4: os << get<4>( v ); break;
        case 5: os << get<5>( v ); break;
        case 6: os << get<6>( v ); break;
        default: os << "unexpected index";
    }
    return os << "]";
}

} // namespace nonstd

using namespace nonstd;

int main()
{
    variant< char, int, long, std::string > var;
    
    var = 'v'; std::cout << "char:" << var << "\n";
    var =  7 ; std::cout << "int:"  << var << "\n";
    var = 42L; std::cout << "long:" << var << "\n";
    var = std::string("hello, world"); std::cout << "std::string:" << var << "\n";
}

#if 0
cl -EHsc -I../include/nonstd 02-iostream.cpp && 02-iostream.exe
g++ -std=c++03 -Wall -I../include/nonstd -o 02-iostream.exe 02-iostream.cpp && 02-iostream.exe
#endif 
