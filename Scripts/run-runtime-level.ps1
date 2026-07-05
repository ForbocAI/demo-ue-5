param(
  [string] $UnrealEditor = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $Width = 1280,
  [int] $Height = 720,
  [bool] $Game = $true,
  [switch] $Wait,
  [string[]] $ExtraArgs = @()
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "DemoProject.uproject"

function Quote-ProcessArgument {
  param([string] $Value)

  '"' + ($Value -replace '"', '\"') + '"'
}

function Optional-GameArgument {
  param([bool] $Enabled)

  if ($Enabled) {
    @("-game")
  } else {
    @()
  }
}

if (-not (Test-Path -LiteralPath $UnrealEditor)) {
  throw "UnrealEditor was not found at $UnrealEditor"
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "DemoProject.uproject was not found at $ProjectPath"
}

$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  (Optional-GameArgument $Game),
  "-windowed",
  "-ResX=$Width",
  "-ResY=$Height",
  "-log",
  "-stdout",
  "-FullStdOutLogOutput",
  "-nop4",
  "-nosplash",
  "-NoLiveCoding"
) + $ExtraArgs

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru
Write-Host "Started runtime level: $MapPath"
Write-Host "Process id: $($Process.Id)"

if ($Wait.IsPresent) {
  $Process.WaitForExit()
  exit $Process.ExitCode
}
