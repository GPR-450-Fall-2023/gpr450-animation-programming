::Utility script to copy file.
::By Dan Buckstein
@echo off

rem echo A3: ENTER "animal3d_win_copy.bat"

set srcpath=%~1
set dstpath=%~2
set srcname=%~3
set dstname=%~4

rem echo p1 srcpath: %srcpath%
rem echo p2 dstpath: %dstpath%
rem echo p3 srcname: %srcname%
rem echo p4 dstname: %dstname%

xcopy /y "%srcpath%%srcname%" "%dstpath%%dstname%"

rem echo A3: EXIT "animal3d_win_copy.bat"