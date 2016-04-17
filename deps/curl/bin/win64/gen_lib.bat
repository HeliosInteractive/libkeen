setlocal
@echo off
set target_dir=..\..\lib\win64
dumpbin /exports libcurl.dll > exports.txt
echo LIBRARY LIBCURL > %target_dir%\libcurl.def
echo EXPORTS >> %target_dir%\libcurl.def
for /f "skip=19 tokens=4" %%A in (exports.txt) do echo %%A >> %target_dir%\libcurl.def
lib /def:%target_dir%\libcurl.def /out:%target_dir%\libcurl.lib /machine:x64
del exports.txt
