param(
  [string] $UnrealEditor = "C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $Width = 1280,
  [int] $Height = 720,
  [string] $OutputName = "ue-runtime-first-run.png",
  [int] $TimeoutSeconds = 120
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ScriptsRoot = Split-Path -Parent $ScriptRoot
$ProjectRoot = Split-Path -Parent $ScriptsRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"
$OutputDir = Join-Path $ProjectRoot "screenshots"
$OutputPath = Join-Path $OutputDir $OutputName
$GeneratedScreenshotRoot = Join-Path $ProjectRoot "Saved\Screenshots"
$ExecCmds = "HighResShot ${Width}x${Height}"

function Latest-GeneratedScreenshot {
  param(
    [string] $Root,
    [datetime] $StartedAt
  )

  Get-ChildItem -LiteralPath $Root -Filter "*.png" -File -Recurse -ErrorAction SilentlyContinue |
    Where-Object { $_.LastWriteTime -ge $StartedAt } |
    Sort-Object LastWriteTime -Descending |
    Select-Object -First 1
}

function Quote-ProcessArgument {
  param([string] $Value)

  '"' + ($Value -replace '"', '\"') + '"'
}

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$StartedAt = (Get-Date).AddSeconds(-2)
$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  "-game",
  (Quote-ProcessArgument "-ExecCmds=$ExecCmds"),
  "-stdout",
  "-FullStdOutLogOutput",
  "-unattended",
  "-nop4",
  "-nosplash",
  "-NoLiveCoding",
  "-nosound"
)

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru
$Deadline = (Get-Date).AddSeconds($TimeoutSeconds)
$Screenshot = $null

while (($null -eq $Screenshot) -and ((Get-Date) -lt $Deadline) -and (-not $Process.HasExited)) {
  Start-Sleep -Seconds 2
  $Process.Refresh()
  $Screenshot = Latest-GeneratedScreenshot -Root $GeneratedScreenshotRoot -StartedAt $StartedAt
}

$Process.Refresh()
$Screenshot = if ($null -ne $Screenshot) {
  $Screenshot
} else {
  Latest-GeneratedScreenshot -Root $GeneratedScreenshotRoot -StartedAt $StartedAt
}

if ($null -ne $Screenshot) {
  Copy-Item -LiteralPath $Screenshot.FullName -Destination $OutputPath -Force
  Write-Host "Saved screenshot: $OutputPath"
}

if (-not $Process.HasExited) {
  Stop-Process -Id $Process.Id -Force
  $Process.WaitForExit()
}

if (-not (Test-Path -LiteralPath $OutputPath)) {
  throw "No new screenshot was found under $GeneratedScreenshotRoot within $TimeoutSeconds seconds."
}
