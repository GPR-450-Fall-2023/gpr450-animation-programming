::Utility script to launch SDK in latest version of Visual Studio.
::By Dan Buckstein

if exist "%VS170COMNTOOLS%" (
rem VISUAL STUDIO 2022 IF PATH EXISTS
	set a3vstoolsdir="%VS170COMNTOOLS%"
	set a3vslaunchpath="%VS170COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v142
	set a3sdkversion=10.0
) else if exist "%VS160COMNTOOLS%" (
rem VISUAL STUDIO 2019 IF PATH EXISTS
	set a3vstoolsdir="%VS160COMNTOOLS%"
	set a3vslaunchpath="%VS160COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v142
	set a3sdkversion=10.0
) else if exist "%VS150COMNTOOLS%" (
rem VISUAL STUDIO 2017 IF PATH EXISTS
	set a3vstoolsdir="%VS150COMNTOOLS%"
	set a3vslaunchpath="%VS150COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v141
	set a3sdkversion=8.1
) else if exist "%VS140COMNTOOLS%" (
rem VISUAL STUDIO 2015 IF PATH EXISTS
	set a3vstoolsdir="%VS140COMNTOOLS%"
	set a3vslaunchpath="%VS140COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v140
	set a3sdkversion=8.1
) else if exist "%VS120COMNTOOLS%" (
rem VISUAL STUDIO 2013 IF PATH EXISTS
	set a3vstoolsdir="%VS120COMNTOOLS%"
	set a3vslaunchpath="%VS120COMNTOOLS%..\IDE\devenv"
	set a3platformtoolset=v120
	set a3sdkversion=8.1
)

if exist %a3vstoolsdir% (
	start %a3vslaunchpath% "%ANIMAL3D_SDK%project\VisualStudio\_SLN\%~1\%~1.sln"
) else (
	echo A3 ERROR: Could not locate compatible version of Visual Studio.
)
