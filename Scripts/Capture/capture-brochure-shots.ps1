param(
  [string] $UnrealEditor = "",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $Width = 1920,
  [int] $Height = 1080,
  [int] $TimeoutSeconds = 180,
  [double] $InitialDelaySeconds = 8.0,
  [double] $SettleSeconds = 1.0,
  [double] $BetweenSeconds = 1.0,
  [string] $OutputDir = ""
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ScriptsRoot = Split-Path -Parent $ScriptRoot
$ProjectRoot = Split-Path -Parent $ScriptsRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"

if ([string]::IsNullOrWhiteSpace($OutputDir)) {
  $OutputDir = Join-Path $ProjectRoot "screenshots\brochure\french-gulch"
}

$ExpectedNames = @(
  "french-gulch-brochure-hero.png",
  "french-gulch-brochure-main-street.png",
  "french-gulch-brochure-people-horses.png"
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
  "-forbocMarketingCapture",
  "-forbocMarketingQuitWhenDone",
  (Quote-ProcessArgument "-forbocMarketingOutputDir=$OutputDir"),
  "-forbocMarketingInitialDelaySeconds=$InitialDelaySeconds",
  "-forbocMarketingSettleSeconds=$SettleSeconds",
  "-forbocMarketingBetweenSeconds=$BetweenSeconds"
)

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru
$Deadline = (Get-Date).AddSeconds($TimeoutSeconds)

Write-Host "Started French Gulch brochure capture in real game mode."
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
  throw "French Gulch brochure screenshots were not all refreshed within $TimeoutSeconds seconds."
}

$ExpectedNames | ForEach-Object {
  Write-Host "Saved screenshot: $(Join-Path $OutputDir $_)"
}
