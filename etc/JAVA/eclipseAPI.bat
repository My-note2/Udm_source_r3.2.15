@echo off

rem if "%3"=="" goto usage

cd %4

echo Generate domain-specific %1 jUdm source ...
%UDM_PATH%\bin\udm.exe %2 -java %3
echo Done

echo Generate the domain-specific %1 eclipse project ...
%UDM_3RDPARTY_PATH%\ant-1.6.2\bin\ant -f %UDM_PATH%\etc\java\build.xml -Dbasedir="%4" -Ddspackage=%1 -Dout.dir="%5" -quiet eclipse
echo Done

goto end

:usage
echo .
echo eclipseApi: Wrong parameter!
echo Usage:   %%UDM_PATH%%\etc\java\eclipseAPI [eclipse_project_name] [UML_meta_model] [UDM_parameters] [working_dir]
echo Example: %%UDM_PATH%%\etc\java\eclipseAPI SBML2Ex SBML2Ex_udm.xml "-i jigcell -q jigcell -e" . 
goto end
:end