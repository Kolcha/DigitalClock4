@echo off
del *.wixobj
del *.wixpdb
del *.msi
call candle.exe *.wxs
call light.exe *.wixobj -out digital_clock_4-x64.msi
