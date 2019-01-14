#include "nonstd/variant.hpp"

using namespace nonstd;

int main()
{
    variant<char,int,long> v;

    int x = get<int>( v );  // asserts (normally throws)
}

// cl -nologo -I../include 03-no-exceptions.cpp && 03-no-exceptions
// g++ -Wall -fno-exceptions -I../include -o 03-no-exceptions 03-no-exceptions.cpp && 03-no-exceptions
