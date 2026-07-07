param(
  [string] $MapPath = "/Game/Map/Maps/Runtime"
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"

if ($env:UE_ROOT) {
  $UERoot = $env:UE_ROOT
} else {
  $UERoot = "C:\Program Files\Epic Games\UE_5.8"
}

$UnrealEditorCmd = Join-Path $UERoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"

if (!(Test-Path $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}

if (!(Test-Path $UnrealEditorCmd)) {
  throw "UnrealEditor-Cmd.exe was not found at $UnrealEditorCmd. Set UE_ROOT to your Unreal Engine 5.8 directory."
}

Write-Host "Building native World Partition HLODs for $MapPath"
& $UnrealEditorCmd `
  $ProjectPath `
  $MapPath `
  -run=WorldPartitionBuilderCommandlet `
  -AllowCommandletRendering `
  -builder=WorldPartitionHLODsBuilder `
  -SetupHLODs `
  -BuildHLODs `
  -unattended `
  -nop4 `
  -NoLiveCoding

exit $LASTEXITCODE
