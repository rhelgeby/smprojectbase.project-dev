@echo off

:: Update these with your project's name.
set SOURCEFILE=src\project.sp
set PLUGIN=project.smx

set SOURCEDIR=src
set SMINCLUDES=env\include
set BUILDDIR=build
set SPCOMP=env\win32\bin\spcomp-1.3.7-3113.exe
set VERSIONDUMP=updateversion.bat

:: Dump version and revision information first.
echo Updating version and revision info...
start /wait %VERSIONDUMP%

:: Make build directory.
if not exist "%BUILDDIR%" (
    mkdir %BUILDDIR%
)

:: Compile.
echo Starting compiler:
%SPCOMP% -i%SOURCEDIR% -i%SMINCLUDES% -o%BUILDDIR%/%PLUGIN% %SOURCEFILE%

echo Compiling done. This script is looped, close if you're done.
pause

compile.bat