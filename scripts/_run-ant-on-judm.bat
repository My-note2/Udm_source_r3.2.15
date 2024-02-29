@echo off

setlocal

set _UDM_PATH=%~dp0..
set _VC=%1%
shift
set _UDM_BUILD=%1%
shift

rem Set environment
call "%_UDM_PATH%\scripts\%_VC%-env.bat"
if ERRORLEVEL 1 echo Environment not set & exit /b

rem Register UML2XML and GmeOclExprChecker
call "%_UDM_PATH%\scripts\_register-gme-components.bat" %_VC% %_UDM_BUILD%

rem Add the rest of parameters to _CMD_LINE
set _CMD_LINE=%1
if ""%1""=="""" goto :start
shift
:setup_args
if ""%1""=="""" goto :start
set _CMD_LINE=%_CMD_LINE% %1
shift
goto setup_args


:start
pushd %_UDM_PATH%\judm
ant "-Dudm.bin.dir=%_UDM_PATH%\bin" %_CMD_LINE%
popd

exit /b
