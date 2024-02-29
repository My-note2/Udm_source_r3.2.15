@echo on

if /i not "%1"=="VC10" if /i not "%1"=="VC8" if /i not "%1"=="VC9" goto :usage
if /i not "%2"=="Debug" if /i not "%2"=="Release" goto :usage

cmd /c "call "%~dp0_%~nx0" %*"

exit /b %ERRORLEVEL%

:usage
echo zips model files necessary for Linux build.
echo Usage: %0 ^<VC7.1^|VC8^|VC9^> ^<Debug^|Release^>
exit /b
