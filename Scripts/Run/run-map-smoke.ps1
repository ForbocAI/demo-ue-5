param(
  [string] $EngineRoot = "",
  [string] $UnrealEditor = "",
  [int] $TimeoutSeconds = 0
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent (Split-Path -Parent $ScriptRoot)
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"
$DataRoot = Join-Path $ProjectRoot "Content\Data"
$BuildPolicyPath = Join-Path $DataRoot "settings\core\automation\runner\build.json"
$MapPolicyDirectory = Join-Path $DataRoot "settings\core\automation\runner\map-smoke"
$MapPolicyPath = Join-Path $MapPolicyDirectory "config.json"
$AutomationFailureMarkersPath = Join-Path $MapPolicyDirectory "automation\failure-markers.json"
$RuntimeFailureMarkersPath = Join-Path $MapPolicyDirectory "runtime\failure-markers.json"
$RuntimeRequiredMarkersPath = Join-Path $MapPolicyDirectory "runtime\required-markers.json"
$AutomationManifestPath = Join-Path $DataRoot "settings\core\automation.json"

$BuildPolicy = (Get-Content -Raw -LiteralPath $BuildPolicyPath | ConvertFrom-Json).build_runner
$MapPolicy = Get-Content -Raw -LiteralPath $MapPolicyPath | ConvertFrom-Json
$AutomationFailureMarkers = [string[]](Get-Content -Raw -LiteralPath $AutomationFailureMarkersPath | ConvertFrom-Json)
$RuntimeFailureMarkers = [string[]](Get-Content -Raw -LiteralPath $RuntimeFailureMarkersPath | ConvertFrom-Json)
$RuntimeRequiredMarkers = [string[]](Get-Content -Raw -LiteralPath $RuntimeRequiredMarkersPath | ConvertFrom-Json)
$AutomationManifest = Get-Content -Raw -LiteralPath $AutomationManifestPath | ConvertFrom-Json
$ContentAssetsPath = Join-Path (Split-Path -Parent $AutomationManifestPath) $AutomationManifest.parts.content_assets
$ContentAssets = Get-Content -Raw -LiteralPath $ContentAssetsPath | ConvertFrom-Json
$MapPath = $ContentAssets.runtime_map.package_path
$EffectiveTimeout = if ($TimeoutSeconds -gt 0) { $TimeoutSeconds } else { [int] $MapPolicy.timeout_seconds }
$EffectiveEngineRoot = if ([string]::IsNullOrEmpty($EngineRoot)) {
  Join-Path "C:\Program Files\Epic Games" $BuildPolicy.engine_directory_name
} else {
  $EngineRoot
}
$EffectiveEditor = if ([string]::IsNullOrEmpty($UnrealEditor)) {
  Join-Path $EffectiveEngineRoot "Engine\Binaries\Win64\UnrealEditor-Cmd.exe"
} else {
  $UnrealEditor
}
$UnrealBuild = Join-Path $EffectiveEngineRoot "Engine\Build\BatchFiles\Build.bat"
$AutomationLog = Join-Path $ProjectRoot $MapPolicy.automation_log_path
$RuntimeLog = Join-Path $ProjectRoot $MapPolicy.runtime_log_path
$LogDirectory = Split-Path -Parent $AutomationLog

function Quote-ProcessArgument {
  param([string] $Value)

  '"' + ($Value -replace '"', '\"') + '"'
}

function Test-JsonDocument {
  param([string] $Path)

  try {
    Get-Content -Raw -LiteralPath $Path | ConvertFrom-Json | Out-Null
    $true
  } catch {
    $false
  }
}

function Invoke-EditorRun {
  param([hashtable] $Request)

  Remove-Item -LiteralPath $Request.Log -Force -ErrorAction SilentlyContinue
  $Process = Start-Process -FilePath $EffectiveEditor -ArgumentList (($Request.Arguments | ForEach-Object { Quote-ProcessArgument $_ }) -join " ") -PassThru
  $Deadline = (Get-Date).AddSeconds($EffectiveTimeout)
  while ((-not $Process.HasExited) -and ((Get-Date) -lt $Deadline)) {
    Start-Sleep -Seconds 2
    $Process.Refresh()
  }
  if (-not $Process.HasExited) {
    Stop-Process -Id $Process.Id -Force
    $Process.WaitForExit()
    throw "$($Request.Label) timed out after $EffectiveTimeout seconds."
  }
  $Process.WaitForExit()
  if ($Process.ExitCode -ne 0) {
    throw "$($Request.Label) failed with exit code $($Process.ExitCode)."
  }
  if (-not (Test-Path -LiteralPath $Request.Log)) {
    throw "$($Request.Label) did not write $($Request.Log)."
  }
}

function Assert-LogContract {
  param([hashtable] $Contract)

  $Text = Get-Content -Raw -LiteralPath $Contract.Log
  $Missing = @($Contract.Required | Where-Object { -not $Text.Contains($_) })
  $Failures = @($Contract.Forbidden | Where-Object { $Text.Contains($_) })
  if ($Missing.Count -gt 0) {
    throw "$($Contract.Label) missing markers: $($Missing -join ', ')"
  }
  if ($Failures.Count -gt 0) {
    throw "$($Contract.Label) observed failure markers: $($Failures -join ', ')"
  }
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}
if (-not (Test-Path -LiteralPath $EffectiveEditor)) {
  throw "UnrealEditor-Cmd was not found at $EffectiveEditor"
}
if (-not (Test-Path -LiteralPath $UnrealBuild)) {
  throw "Unreal build script was not found at $UnrealBuild"
}

New-Item -ItemType Directory -Force -Path $LogDirectory | Out-Null

$GeneratedMetadata = @($BuildPolicy.generated_metadata | ForEach-Object {
  Join-Path $ProjectRoot $_
})
$InvalidMetadata = @($GeneratedMetadata | Where-Object {
  (Test-Path -LiteralPath $_) -and -not (Test-JsonDocument $_)
})
if ($InvalidMetadata.Count -gt 0) {
  $GeneratedMetadata | ForEach-Object {
    Remove-Item -LiteralPath $_ -Force -ErrorAction SilentlyContinue
  }
}

Write-Host "=== ForbocAI Map Smoke ==="
Write-Host "Project: $ProjectPath"
Write-Host "Map:     $MapPath"
Write-Host "Editor:  $EffectiveEditor"
Write-Host "Logs:    $AutomationLog and $RuntimeLog"

$BuildArguments = @(
  $BuildPolicy.target,
  $BuildPolicy.platform,
  $BuildPolicy.configuration,
  "-Project=$ProjectPath",
  "-WaitMutex",
  "-NoHotReloadFromIDE",
  "-NoUBTMakefiles",
  "-MaxParallelActions=$($BuildPolicy.max_parallel_actions)"
)
if ($BuildPolicy.disable_uba) {
  $BuildArguments += "-NoUBA"
}
& $UnrealBuild @BuildArguments
if ($LASTEXITCODE -ne 0) {
  throw "Map smoke build failed with exit code $LASTEXITCODE."
}

$SharedEditorArguments = @(
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
Invoke-EditorRun @{
  Label = "Map wiring automation"
  Log = $AutomationLog
  Arguments = @(
    $ProjectPath,
    "-ExecCmds=Automation RunTests $($MapPolicy.automation_test); Quit",
    "-AbsLog=$AutomationLog",
    "-NoUI"
  ) + $SharedEditorArguments
}
Assert-LogContract @{
  Label = "Map wiring automation"
  Log = $AutomationLog
  Required = @($MapPolicy.automation_success_marker)
  Forbidden = $AutomationFailureMarkers
}

Invoke-EditorRun @{
  Label = "Map game-mode smoke"
  Log = $RuntimeLog
  Arguments = @(
    $ProjectPath,
    $MapPath,
    "-game",
    "-ExecCmds=Quit",
    "-AbsLog=$RuntimeLog"
  ) + $SharedEditorArguments
}
Assert-LogContract @{
  Label = "Map game-mode smoke"
  Log = $RuntimeLog
  Required = @("LogLoad: LoadMap: $MapPath") + $RuntimeRequiredMarkers
  Forbidden = $RuntimeFailureMarkers
}

Write-Host "Map smoke qualified $MapPath."
