@echo off

cd %5

echo Generate domain-specific %1 data type ...
%UDM_3RDPARTY_PATH%\ant-1.6.2\bin\ant -f %UDM_PATH%\etc\java\bs_type\build_data_type.xml -Dbasedir="%5" -Ddspackage=%1 -Ddspackage_lower=%2 -Dversion=%3 -Ddashboard_home="C:\Program Files\Bio-SPICE\Dashboard-6.0\Dashboard" -quiet nbm

:usage
echo .
echo biospiceApi: Wrong parameter!
echo Usage:   biospiceApi [project_name] [package_name_name] [version] [dashboard_home]
echo Example: biospiceApi GeneTF genetf 1.0.0 "C:\Program Files\Bio-SPICE\Dashboard-6.0\Dashboard"
goto end
:end