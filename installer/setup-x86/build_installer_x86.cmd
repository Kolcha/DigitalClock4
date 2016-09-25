@echo off
del *.wixobj
del *.wixpdb
del *.msi
call candle.exe -ext WixUtilExtension *.wxs
call light.exe -ext WixUtilExtension *.wixobj -out digital_clock_4-x86.msi
