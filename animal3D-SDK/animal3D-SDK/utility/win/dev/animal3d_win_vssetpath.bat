::Utility to set a missing VS tools path variable.
::By Dan Buckstein, (c) 2018
::Problem: 
:: VS150COMNTOOLS not set with VS2017 (dumb)...
:: No quick fix, so let's have VS2017 do it for us!
@echo off
setlocal enableDelayedExpansion
set ver=%~1
set var=VS%ver%COMNTOOLS
set "loc=!%var%!"
cd /d "%~2..\Tools"
if not exist "%loc%" (
	echo "%var%" does not exist
	echo setting %var% to "%cd%\"
	setx /M %var% "%cd%"\
) else (
	echo "%var%" exists
)
