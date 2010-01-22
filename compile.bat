@echo off

set SOURCEDIR=src
set SMINCLUDES=env\include
set BUILDDIR=build
set SPCOMP=env\win32\bin\spcomp.exe
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
%SPCOMP% -i%SOURCEDIR% -i%SOURCEDIR%/include -i%SMINCLUDES% -o%BUILDDIR%/project.smx %SOURCEDIR%\project.sp

pause