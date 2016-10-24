#include "variant.hpp"

#include <cassert>
#include <string>

using namespace nonstd;

int main()
{
    std::string hello = "hello, world"; 

    variant< char, int, long, std::string > var;
    
    var =  'v' ; assert( get<  0 >( var ) == 'v' );
    var =  'v' ; assert( get<char>( var ) == 'v' );
    var =   7  ; assert( get<int >( var ) ==  7  );
    var =  42L ; assert( get<long>( var ) == 42L );    
    var = hello; assert( get<std::string>( var ) == hello );
}

#if 0
cl -EHsc -I../include/nonstd 01-basic.cpp && 01-basic.exe
g++ -std=c++98 -Wall -I../include/nonstd -o 01-basic.exe 01-basic.cpp && 01-basic.exe
#endif 
