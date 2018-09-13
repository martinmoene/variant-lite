@setlocal
@set std=%1
@if not "%std%"=="" set std=-std:%std%
cl -W3 -EHsc %std% -Dvariant_CONFIG_SELECT_VARIANT=variant_VARIANT_NONSTD  -I../include/nonstd variant-main.t.cpp variant.t.cpp optional.t.cpp && variant-main.t.exe 
@endlocal

