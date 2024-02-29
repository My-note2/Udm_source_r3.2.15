
PATH=$PATH:"$(cygpath -pa 'C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE\;C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\BIN;C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools;C:\Windows\Microsoft.NET\Framework\v4.0.30319;C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\bin')"

DUMPBIN="$(cygpath -a 'C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\dumpbin.exe')"

for D in '' D; do
 DEF=xerces-c_2_8$D.def

 echo "LIBRARY xerces-c_2_8_x64$D" > "$DEF"
 echo "EXPORTS" >> "$DEF"
 "$DUMPBIN" /exports xerces-c_2_8$D.dll | tail -n+20 | head -n-8 | awk '{print $4}' >> "$DEF"
 u2d "$DEF"

 lib /MACHINE:x64 /def:"$DEF" /out:..\\libx64\\xerces-c_2_x64$D.lib
done

