@echo off

: Read UDM_3RDPARTY_PATH and JAVA_HOME from script with local settings
if exist "%~dp0"vc9-env-local.bat call "%~dp0"vc9-env-local.bat

if not defined UDM_3RDPARTY_PATH echo Set UDM_3RDPARTY_PATH & exit /b 1
if not defined JAVA_HOME echo Set JAVA_HOME & exit /b 1
if not defined GME_ROOT echo Set GME_ROOT & exit /b 1

if defined UDM_PATH goto :udm_path_set

rem Determine UDM_PATH
pushd %CD%
rem cd to the parent directory of this script
cd /d %~dp0..
set UDM_PATH=%CD%
popd

:udm_path_set

set PATH=%UDM_PATH%\bin;^
%UDM_3RDPARTY_PATH%\cppunit-1.12.1\bin;^
%UDM_3RDPARTY_PATH%\info-zip;^
%UDM_3RDPARTY_PATH%\apache-ant-1.7.1\bin;^
%PATH%

set _CLASSPATH=%UDM_3RDPARTY_PATH%\junit\junit.jar;%UDM_3RDPARTY_PATH%\antlr-2.7.7\antlr.jar
if defined CLASSPATH (set CLASSPATH=%_CLASSPATH%;%CLASSPATH%) else (set CLASSPATH=%_CLASSPATH%)

