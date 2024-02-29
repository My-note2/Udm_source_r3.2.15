@echo off

: Register UML2XML and GmeOclExprChecker
: Parameter 1 should be the VC version, VC7.1, VC8, or VC9
: Parameter 2 should be the build type, Debug or Release

setlocal

set _UDM_BIN_PATH=%~dp0..\Build\Win32\%1\%2

if /i "%2"=="Debug" (goto :register_debug) else (goto :register_release)

:register_debug
regsvr32 /s /c "%_UDM_BIN_PATH%\UML2XMLD.dll"
regsvr32 /s /c "%_UDM_BIN_PATH%\GmeOclExprCheckerD.dll"
goto :end

:register_release
regsvr32 /s /c "%_UDM_BIN_PATH%\UML2XML.dll"
regsvr32 /s /c "%_UDM_BIN_PATH%\GmeOclExprChecker.dll"

:end
