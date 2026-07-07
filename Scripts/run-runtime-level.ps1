param(
  [string] $UnrealEditor = "",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [bool] $Game = $true,
  [switch] $Wait,
  [string[]] $ExtraArgs = @()
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"

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

function Resolve-UnrealEditorPath {
  param([string] $ExplicitPath)

  if (-not [string]::IsNullOrWhiteSpace($ExplicitPath)) {
    return $ExplicitPath
  }

  $Root = $env:UE_ROOT
  if ([string]::IsNullOrWhiteSpace($Root)) {
    $Root = "C:\Program Files\Epic Games\UE_5.8"
  }

  return Join-Path $Root "Engine\Binaries\Win64\UnrealEditor.exe"
}

$UnrealEditor = Resolve-UnrealEditorPath -ExplicitPath $UnrealEditor

if (-not (Test-Path -LiteralPath $UnrealEditor)) {
  throw "UnrealEditor was not found at $UnrealEditor"
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}

$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  (Optional-GameArgument $Game),
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
