param()

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"
$PythonScript = Join-Path $ProjectRoot "Scripts\Tools\generate_skeletal_lods.py"

if ($env:UE_ROOT) {
  $UERoot = $env:UE_ROOT
} else {
  $UERoot = "C:\Program Files\Epic Games\UE_5.8"
}

$UnrealEditorCmd = Join-Path $UERoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"

if (!(Test-Path $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}

if (!(Test-Path $PythonScript)) {
  throw "Skeletal LOD generation script was not found at $PythonScript"
}

if (!(Test-Path $UnrealEditorCmd)) {
  throw "UnrealEditor-Cmd.exe was not found at $UnrealEditorCmd. Set UE_ROOT to your Unreal Engine 5.8 directory."
}

& $UnrealEditorCmd `
  $ProjectPath `
  -run=pythonscript `
  "-script=$PythonScript" `
  -unattended `
  -nop4 `
  -nosplash `
  -NoLiveCoding

exit $LASTEXITCODE
