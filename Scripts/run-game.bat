@echo off
setlocal

set "SCRIPT_ROOT=%~dp0"
if "%SCRIPT_ROOT:~-1%"=="\" set "SCRIPT_ROOT=%SCRIPT_ROOT:~0,-1%"

for %%I in ("%SCRIPT_ROOT%\..") do set "PROJECT_ROOT=%%~fI"
set "PROJECT_FILE=%PROJECT_ROOT%\ForbocAIDemo.uproject"
set "MAP_PATH=/Game/Map/Maps/Runtime"

if "%UE_ROOT%"=="" (
  set "UE_ROOT=C:\Program Files\Epic Games\UE_5.8"
)

set "UNREAL_EDITOR=%UE_ROOT%\Engine\Binaries\Win64\UnrealEditor.exe"

if not exist "%UNREAL_EDITOR%" (
  echo UnrealEditor.exe was not found at:
  echo   %UNREAL_EDITOR%
  echo.
  echo Set UE_ROOT to your Unreal Engine 5.8 directory, then run this again.
  exit /b 1
)

if not exist "%PROJECT_FILE%" (
  echo ForbocAIDemo.uproject was not found at:
  echo   %PROJECT_FILE%
  exit /b 1
)

echo Launching Frontier of Jefferson runtime map...
echo Project:     %PROJECT_FILE%
echo Map:         %MAP_PATH%
echo Editor:      %UNREAL_EDITOR%
echo Rendering:   authored JSON profile applied by game features
echo.

start "ForbocAIDemo" "%UNREAL_EDITOR%" "%PROJECT_FILE%" "%MAP_PATH%" -game -log -NoLiveCoding %*

endlocal
