@echo on

setlocal

set _UDM_PATH=%~dp0..
set _VC=%1%
shift
set _UDM_BUILD=%1%
shift

rem Set environment
call "%_UDM_PATH%\scripts\%_VC%-env.bat"
if ERRORLEVEL 1 echo Environment not set & exit /b 1

if "%_VC%" == "VC10" (
xcopy /y/d tests\test_crosslinks\testdata\CL_udm.udm tests\test_crosslinks\ || exit /b 2
xcopy /y/d samples\cross_links\CL_udm.udm samples\ || exit /b 2
xcopy /y/d samples\CreateLampModel\LampDiagram_udm.xml samples\ || exit /b 2
zip "%_UDM_PATH%\model-files-%_VC%-%_UDM_BUILD%.zip" samples/CL_udm.udm samples/LampDiagram_udm.xml tests/test_crosslinks/CL_udm.udm tests/testGetAdjacentObjects/testAssociationScenarios_udm.udm tests/testOCL/SBML2_udm.xml tests/testUdmCint/GeneTF_udm.xml tests/testUdmCint/SBML2Ex_udm.xml tests/test_assocInheritanceMGA/SignalFlow_udm.xml tests/test_ns/test_ns_udm.xml tests/test_ns2/CHost_udm.xml tests/test_refports/RefPort_udm.xml judm/samples/GeneTF/meta/GeneTF_udm.xml judm/samples/GeneTRE/meta/GeneTRE_udm.xml judm/samples/SBML2Ex/meta/SBML2Ex_udm.xml judm/samples/TimeSeries/meta/TimeSeries_udm.xml || exit /b 3
) else (
pushd %_UDM_PATH%\Build\Win32\%_VC%\%_UDM_BUILD%\Gen || exit /b 2
zip -r "%_UDM_PATH%\model-files-%_VC%-%_UDM_BUILD%.zip" . -i *_udm.xml -i *_udm.udm || exit /b 3
popd

pushd %_UDM_PATH%
zip -r "%_UDM_PATH%\model-files-%_VC%-%_UDM_BUILD%.zip" judm -i *_udm.xml -i *_udm.udm -x judm/build/* || exit /b 4
popd
)

exit /b 0
