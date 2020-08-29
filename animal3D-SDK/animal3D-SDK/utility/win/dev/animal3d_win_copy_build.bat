::Utility script to copy build files.
::By Dan Buckstein
@echo off

rem echo A3: ENTER "animal3d_win_copy_build.bat"

set "libpath=%1"
set "binpath=%2"
set "libname=%3"
set "binname=%4"
set "datapath=%5"

rem echo p1  libpath: %libpath%
rem echo p2  binpath: %binpath%
rem echo p3  libname: %libname%
rem echo p4  binname: %binname%
rem echo p5 datapath: %datapath%

:: create output directory in case it does not exist yet
:: write to dummy file and perform copy
:: also create asset data directory
cd "%ANIMAL3D_SDK%"
mkdir %binpath%
mkdir %datapath%
set dstfile="%~2\%~4"
@echo ANIMAL3D_SDK>%dstfile%
call ".\utility\win\dev\animal3d_win_copy.bat" %libpath% %binpath% %libname% %binname%

rem echo A3: EXIT "animal3d_win_copy_build.bat"