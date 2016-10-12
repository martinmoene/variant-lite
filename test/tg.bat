@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -O2 -Wall -Wextra -Wno-unused-parameter -o variant-lite.t.exe -I../include/nonstd variant-lite.t.cpp variant.t.cpp optional.t.cpp && variant-lite.t.exe
@endlocal

