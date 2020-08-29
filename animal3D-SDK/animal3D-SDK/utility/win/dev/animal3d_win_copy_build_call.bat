::Utility script to call build copy.
::By Dan Buckstein
@echo off

rem echo A3: ENTER "animal3d_win_copy_build_call.bat"

set "calldir=%~dp0"
set "calllibpath=%1"
set "callbinpath=%2"
set "calllibname=%3"
set "callbinname=%4"
set "calldatapath=%5"

rem echo dp      calldir: %calldir%
rem echo p1  calllibpath: %calllibpath%
rem echo p2  callbinpath: %callbinpath%
rem echo p3  calllibname: %calllibname%
rem echo p4  callbinname: %callbinname%
rem echo p5 calldatapath: %calldatapath%

if not "%stopcopy%"=="YES" (
	call "%calldir%animal3d_win_copy_build.bat" %calllibpath% %callbinpath% %calllibname% %callbinname% %calldatapath%
) else (
	echo A3: DEFERRED COPY BUILD...
)

rem echo A3: EXIT "animal3d_win_copy_build_call.bat"