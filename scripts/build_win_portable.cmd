@echo off
setlocal

set src_root=%~dp0..

call C:\Qt\5.12.7\mingw73_32\bin\qtenv2.bat
set PATH=%PATH%;C:\Program Files\7-Zip

set data_dir=%src_root%\..\data
set dst_path=%src_root%\..\Digital Clock 4

set archive_name=digital_clock_4-win_portable.zip

set variant=release

cd "%src_root%"

lupdate -no-obsolete DigitalClock.pro

set build_dir=%src_root%\..\build
mkdir "%build_dir%"
cd "%build_dir%"

qmake DEFINES+=PORTABLE_VERSION -config %variant% -r "%src_root%\DigitalClock.pro"
mingw32-make -j4

if exist "%dst_path%" rmdir /s /q "%dst_path%"
mkdir "%dst_path%"

xcopy /y "clock_common\%variant%\clock_common.dll" "%dst_path%\"
xcopy /y "digital_clock\%variant%\digital_clock.exe" "%dst_path%\"
xcopy /y "plugin_core\%variant%\plugin_core.dll" "%dst_path%\"
xcopy /y "skin_draw\%variant%\skin_draw.dll" "%dst_path%\"

mkdir "%dst_path%\plugins"

cd plugins
for /d %%g in (*) do (
  xcopy /y "%%g\%variant%\%%g.dll" "%dst_path%\plugins\"
)
cd ..

cd "%dst_path%"

set deploy_flags=--libdir . --plugindir "plugins" --no-system-d3d-compiler --no-opengl-sw

windeployqt %deploy_flags% "clock_common.dll"
windeployqt %deploy_flags% "digital_clock.exe"
windeployqt %deploy_flags% "plugin_core.dll"
windeployqt %deploy_flags% "skin_draw.dll"

for %%f in ("plugins\*.dll") do (
  windeployqt %deploy_flags% --no-translations "%%f"
)

xcopy /y "%build_dir%\paletteicon\%variant%\paletteicon.dll" "%dst_path%\plugins\iconengines\"

xcopy /s /y "%data_dir%\textures" "%dst_path%\textures\"
xcopy /s /y "%data_dir%\skins" "%dst_path%\skins\"

xcopy /y "C:\Qt\Tools\OpenSSL\Win_x86\bin\libcrypto-1_1.dll" "%dst_path%\"
xcopy /y "C:\Qt\Tools\OpenSSL\Win_x86\bin\libssl-1_1.dll" "%dst_path%\"

copy /y "%src_root%\installer\postinstall_readme.txt" "%dst_path%\README.txt"

cd "%build_dir%\.."
rmdir /s /q "%build_dir%"

cd "%dst_path%\.."
if exist "%archive_name%" del /f /q "%archive_name%"
7z a "%archive_name%" "%dst_path%"

endlocal
