@echo off

if /i not "%1"=="VC7.1" if /i not "%1"=="VC8" if /i not "%1"=="VC9" goto :usage
if /i not "%2"=="Debug" if /i not "%2"=="Release" goto :usage

rem Run GME
cmd /c "call "%~dp0_%~nx0" %*"

exit /b

:usage
echo Runs GME from specified UDM build
echo Usage: %0 ^<VC7.1^|VC8^|VC9^> ^<Debug^|Release^> ^<parameters to GME^>
exit /b
