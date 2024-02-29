@echo off

cd %4

echo Generate domain-specific %1 source ...
%UDM_PATH%\bin\udm.exe %2 -j %3
echo Done

echo Generate the domain-specific %1 library ...
%UDM_3RDPARTY_PATH%\ant-1.6.2\bin\ant -f %UDM_PATH%\etc\java\build.xml -Dbasedir="%4" -Ddspackage=%1 -Dout.dir="%5" -quiet lib-zip
echo Done

goto end

:usage
echo .
echo judmApi: Wrong parameter!
echo Usage:   %%UDM_PATH%%\etc\java\judmAPI [library_name] [UML_meta_model] [UDM_parameters] [working_dir]
echo Example: %%UDM_PATH%%\etc\java\judmAPI SBML2Ex SBML2Ex_udm.xml "-i jigcell -q jigcell -e" .
goto end
:end