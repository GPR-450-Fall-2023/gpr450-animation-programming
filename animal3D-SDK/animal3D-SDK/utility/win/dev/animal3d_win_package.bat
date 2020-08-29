::Utility script to help you prepare a clean project package.
::By Dan Buckstein
@echo off
echo Preparing project package...
set /p outputdir=" Please type output folder name: "
set rootpath=..\..
set outputpath=_PACKAGE\%outputdir%_VisualStudio
set projectname=animal3D-DemoPlugin
mkdir %outputpath%
cd %outputpath%

::build directory tree
::DO NOT ADD TO THIS LIST
set includedst=include
set resourcedst=resource
set sourcedst=source\%projectname%
set projectdst=project\VisualStudio\%projectname%
mkdir %includedst%
mkdir %resourcedst%
mkdir %sourcedst%
mkdir %projectdst%

::COPY RELEVANT FILES ONLY
::DO NOT ADD TO THIS LIST
set includedir=%rootpath%\%includedst%
set resourcedir=%rootpath%\%resourcedst%
set sourcedir=%rootpath%\%sourcedst%
set projectdir=%rootpath%\%projectdst%

rmdir /s /q %projectdir%\build
xcopy /y /s %includedir%\* 				.\%includedst%\
xcopy /y /s %resourcedir%\* 			.\%resourcedst%\
xcopy /y /s %sourcedir%\*				.\%sourcedst%\
xcopy /y /s %projectdir%\*				.\%projectdst%\

::done
