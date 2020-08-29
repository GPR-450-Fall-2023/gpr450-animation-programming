::Utility script to build project.
::By Dan Buckstein
@echo off

rem echo A3: ENTER "animal3d_win_build_demo.bat"

set launchpath=%1
set buildpath=%2
set buildconfig=%3
set buildswitch=%~4
set projpath=%~5
set stopcopy=%~6

rem echo p1  launchpath: %launchpath%
rem echo p2   buildpath: %buildpath%
rem echo p3 buildconfig: %buildconfig%
rem echo p4 buildswitch: %buildswitch%
rem echo p5    projpath: %projpath%
rem echo p6    stopcopy: %stopcopy%

call %launchpath% %buildpath% %buildswitch% %buildconfig%
rem /Project %projpath% /ProjectConfig %buildconfig%

rem echo A3: EXIT "animal3d_win_build_demo.bat"

exit