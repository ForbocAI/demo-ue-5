param(
  [string] $UnrealEditor = "",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $Width = 1280,
  [int] $Height = 720,
  [int] $TimeoutSeconds = 180,
  [double] $InitialDelaySeconds = 8.0,
  [double] $SettleSeconds = 1.0,
  [double] $BetweenSeconds = 1.0,
  [double] $WholeOrthoWidth = 336000.0,
  [double] $TownOrthoWidth = 70000.0,
  [double] $ActorsOrthoWidth = 24000.0,
  [double] $WholeCaptureHeight = 320000.0,
  [double] $TownCaptureHeight = 90000.0,
  [double] $ActorsCaptureHeight = 45000.0
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"
$OutputDir = Join-Path $ProjectRoot "screenshots"
$ExpectedNames = @(
  "scale-audit-ingame-whole-level.png",
  "scale-audit-ingame-town.png",
  "scale-audit-ingame-people-horses.png"
)

function Quote-ProcessArgument {
  param([string] $Value)

  '"' + ($Value -replace '"', '\"') + '"'
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

function Test-ExpectedScreenshots {
  param(
    [string] $Directory,
    [string[]] $Names,
    [datetime] $StartedAt
  )

  $Ready = $Names | Where-Object {
    $Path = Join-Path $Directory $_
    (Test-Path -LiteralPath $Path) -and
      ((Get-Item -LiteralPath $Path).LastWriteTime -ge $StartedAt)
  }

  return $Ready.Count -eq $Names.Count
}

$UnrealEditor = Resolve-UnrealEditorPath -ExplicitPath $UnrealEditor

if (-not (Test-Path -LiteralPath $UnrealEditor)) {
  throw "UnrealEditor was not found at $UnrealEditor"
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$StartedAt = (Get-Date).AddSeconds(-2)
$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  "-game",
  "-log",
  "-stdout",
  "-FullStdOutLogOutput",
  "-unattended",
  "-nop4",
  "-nosplash",
  "-NoLiveCoding",
  "-nosound",
  "-d3d11",
  "-windowed",
  "-ResX=$Width",
  "-ResY=$Height",
  "-forbocScaleAuditCapture",
  "-forbocScaleAuditQuitWhenDone",
  (Quote-ProcessArgument "-forbocScaleAuditOutputDir=$OutputDir"),
  "-forbocScaleAuditInitialDelaySeconds=$InitialDelaySeconds",
  "-forbocScaleAuditSettleSeconds=$SettleSeconds",
  "-forbocScaleAuditBetweenSeconds=$BetweenSeconds",
  "-forbocScaleAuditWholeOrthoWidth=$WholeOrthoWidth",
  "-forbocScaleAuditTownOrthoWidth=$TownOrthoWidth",
  "-forbocScaleAuditActorsOrthoWidth=$ActorsOrthoWidth",
  "-forbocScaleAuditWholeCaptureHeight=$WholeCaptureHeight",
  "-forbocScaleAuditTownCaptureHeight=$TownCaptureHeight",
  "-forbocScaleAuditActorsCaptureHeight=$ActorsCaptureHeight"
)

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru
$Deadline = (Get-Date).AddSeconds($TimeoutSeconds)

Write-Host "Started scale audit capture in real game mode."
Write-Host "Process id: $($Process.Id)"
Write-Host "Output: $OutputDir"

while ((-not (Test-ExpectedScreenshots -Directory $OutputDir -Names $ExpectedNames -StartedAt $StartedAt)) -and ((Get-Date) -lt $Deadline) -and (-not $Process.HasExited)) {
  Start-Sleep -Seconds 2
  $Process.Refresh()
}

$Process.Refresh()
$Ready = Test-ExpectedScreenshots -Directory $OutputDir -Names $ExpectedNames -StartedAt $StartedAt

if (-not $Process.HasExited) {
  Stop-Process -Id $Process.Id -Force
  $Process.WaitForExit()
}

if (-not $Ready) {
  throw "Scale audit screenshots were not all refreshed within $TimeoutSeconds seconds."
}

$ExpectedNames | ForEach-Object {
  Write-Host "Saved screenshot: $(Join-Path $OutputDir $_)"
}
