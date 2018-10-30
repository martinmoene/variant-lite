@setlocal
@set std=%1
@if "%std%"=="" set std=c++11
g++ -std=%std% -O2 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wno-unused-parameter -o variant-main.t.exe -I../include/nonstd variant-main.t.cpp variant.t.cpp && variant-main.t.exe
@endlocal

