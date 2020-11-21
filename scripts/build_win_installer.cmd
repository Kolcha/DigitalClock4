@echo off
setlocal

set src_root=%~dp0..

call C:\Qt\5.12.10\msvc2017_64\bin\qtenv2.bat
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
set PATH=%PATH%;C:\Qt\Tools\QtCreator\bin
set PATH=%PATH%;C:\Program Files (x86)\WiX Toolset v3.11\bin

where >nul 2>&1 jom
if %ERRORLEVEL% equ 0 (
  set make=jom
) else (
  set make=nmake
)

set data_dir=%src_root%\..\data
set qt_files=%src_root%\..\qt-files-x64
set archive_name=%src_root%\..\digital_clock_4-x64.msi

set variant=release

cd "%src_root%"

lupdate -no-obsolete DigitalClock.pro

set build_dir=%src_root%\..\build-msi-x64
mkdir "%build_dir%"
cd "%build_dir%"

qmake -config %variant% -r "%src_root%\DigitalClock.pro"
%make%

if %ERRORLEVEL% neq 0 (
  exit /b 1
)

set deploy_flags=--libdir "%qt_files%" --plugindir "%qt_files%\plugins" --no-system-d3d-compiler --no-compiler-runtime --no-opengl-sw

windeployqt %deploy_flags% "clock_common\%variant%\clock_common.dll"
windeployqt %deploy_flags% "digital_clock\%variant%\digital_clock.exe"
windeployqt %deploy_flags% "plugin_core\%variant%\plugin_core.dll"
windeployqt %deploy_flags% "skin_draw\%variant%\skin_draw.dll"

cd plugins
for /d %%g in (*) do (
  if not "%%g" == "win_on_top" (
    windeployqt %deploy_flags% --no-translations "%%g\%variant%\%%g.dll"
  )
)
cd ..

xcopy /s /y "digital_clock\%variant%\translations" "%qt_files%\translations\"

xcopy /y "C:\Qt\Tools\OpenSSL\Win_x64\bin\libcrypto-1_1-x64.dll" "%qt_files%\"
xcopy /y "C:\Qt\Tools\OpenSSL\Win_x64\bin\libssl-1_1-x64.dll" "%qt_files%\"

cd "%src_root%\installer\setup-x64"
nmake
if %ERRORLEVEL% neq 0 (
  exit /b 1
)

if exist "%archive_name%" del /f /q "%archive_name%"
move digital_clock_4-x64.msi "%archive_name%"
nmake clean

cd "%build_dir%\.."
rmdir /s /q "%build_dir%"
cd "%qt_files%\.."
rmdir /s /q "%qt_files%"

endlocal
