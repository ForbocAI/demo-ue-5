param(
  [string] $UnrealEditor = "C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor.exe",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $Width = 1280,
  [int] $Height = 720,
  [int] $TimeoutSeconds = 120
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "DemoProject.uproject"
$LogDir = Join-Path $ProjectRoot "Saved\Automation"
$LogPath = Join-Path $LogDir "RuntimeSmokeLog.txt"
$RequiredMarkers = @(
  "LogLoad: LoadMap: /Game/Map/Maps/Runtime",
  "LogForbocDemoRedux: Display: action terrain/loaded"
)

function Quote-ProcessArgument {
  param([string] $Value)

  '"' + ($Value -replace '"', '\"') + '"'
}

function Test-LogContains {
  param(
    [string] $Path,
    [string] $Marker
  )

  $Match = Select-String -LiteralPath $Path -SimpleMatch -Pattern $Marker -List -ErrorAction SilentlyContinue
  return $null -ne $Match
}

if (-not (Test-Path -LiteralPath $UnrealEditor)) {
  throw "UnrealEditor was not found at $UnrealEditor"
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "DemoProject.uproject was not found at $ProjectPath"
}

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null
Remove-Item -LiteralPath $LogPath -Force -ErrorAction SilentlyContinue

$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  "-game",
  "-windowed",
  "-ResX=$Width",
  "-ResY=$Height",
  (Quote-ProcessArgument "-AbsLog=$LogPath"),
  (Quote-ProcessArgument "-ExecCmds=Quit"),
  "-log",
  "-stdout",
  "-FullStdOutLogOutput",
  "-unattended",
  "-nop4",
  "-nosplash",
  "-nullrhi",
  "-nosound",
  "-NoLiveCoding"
)

Write-Host "=== ForbocAI Runtime Smoke ==="
Write-Host "Project: $ProjectPath"
Write-Host "Map:     $MapPath"
Write-Host "Editor:  $UnrealEditor"
Write-Host "Log:     $LogPath"

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru
$Deadline = (Get-Date).AddSeconds($TimeoutSeconds)

while ((-not $Process.HasExited) -and ((Get-Date) -lt $Deadline)) {
  Start-Sleep -Seconds 2
  $Process.Refresh()
}

if (-not $Process.HasExited) {
  Stop-Process -Id $Process.Id -Force
  $Process.WaitForExit()
  throw "Runtime smoke timed out after $TimeoutSeconds seconds."
}

$Process.WaitForExit()

if ($Process.ExitCode -ne 0) {
  throw "Runtime smoke failed with exit code $($Process.ExitCode)."
}

if (-not (Test-Path -LiteralPath $LogPath)) {
  throw "Runtime smoke log was not written at $LogPath."
}

foreach ($Marker in $RequiredMarkers) {
  if (-not (Test-LogContains -Path $LogPath -Marker $Marker)) {
    throw "Runtime smoke did not observe required log marker: $Marker"
  }
}

if (Test-LogContains -Path $LogPath -Marker "Fatal error") {
  throw "Runtime smoke log contains a fatal error."
}

Write-Host "Runtime smoke loaded $MapPath in game mode."
