param(
  [string] $UnrealEditor = "",
  [string] $MapPath = "/Game/Map/Maps/Runtime",
  [int] $StartTimeoutSeconds = 0,
  [string] $BudgetFile = ""
)

$ErrorActionPreference = "Stop"

$ScriptRoot = Split-Path -Parent $PSCommandPath
$ProjectRoot = Split-Path -Parent $ScriptRoot
$ProjectPath = Join-Path $ProjectRoot "ForbocAIDemo.uproject"
$LogDir = Join-Path $ProjectRoot "Saved\Automation"
$LogPath = Join-Path $LogDir "RuntimeBudgetLog.txt"
$ProfilePath = Join-Path $ProjectRoot "Content\Data\runtime_settings_rendering_profile.json"
$RuntimeBudgetFailureGuidance = @()

function Throw-RuntimeBudgetFailure {
  param([string] $Message)

  foreach ($Line in $RuntimeBudgetFailureGuidance) {
    Write-Host $Line
  }
  throw $Message
}

function Resolve-UnrealEditorPath {
  param([string] $ExplicitPath)

  if (-not [string]::IsNullOrWhiteSpace($ExplicitPath)) {
    return $ExplicitPath
  }

  $Root = $env:UE_ROOT
  if ([string]::IsNullOrWhiteSpace($Root)) {
    $Root = "C:\Program Files\Epic Games\UE_5.7"
  }

  return Join-Path $Root "Engine\Binaries\Win64\UnrealEditor.exe"
}

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

function Test-LogMatches {
  param(
    [string] $Path,
    [string] $Pattern
  )

  $Match = Select-String -LiteralPath $Path -Pattern $Pattern -List -ErrorAction SilentlyContinue
  return $null -ne $Match
}

function Wait-ForLogMarker {
  param(
    [System.Diagnostics.Process] $Process,
    [string] $Path,
    [string] $Marker,
    [int] $TimeoutSeconds
  )

  $Deadline = (Get-Date).AddSeconds($TimeoutSeconds)
  while ((-not $Process.HasExited) -and ((Get-Date) -lt $Deadline)) {
    if (Test-LogContains -Path $Path -Marker $Marker) {
      return
    }
    Start-Sleep -Seconds 1
    $Process.Refresh()
  }

  Throw-RuntimeBudgetFailure "Runtime budget test timed out before observing log marker: $Marker"
}

function Wait-ForLogPattern {
  param(
    [System.Diagnostics.Process] $Process,
    [string] $Path,
    [string] $Pattern,
    [int] $TimeoutSeconds
  )

  $Deadline = (Get-Date).AddSeconds($TimeoutSeconds)
  while ((-not $Process.HasExited) -and ((Get-Date) -lt $Deadline)) {
    if (Test-LogMatches -Path $Path -Pattern $Pattern) {
      return
    }
    Start-Sleep -Seconds 1
    $Process.Refresh()
  }

  Throw-RuntimeBudgetFailure "Runtime budget test timed out before observing a real nonzero FPS sample."
}

function Get-ScreenshotCount {
  param(
    [string] $Path,
    [string] $Filter,
    [int] $Depth
  )

  @(Get-ChildItem -LiteralPath $Path -Filter $Filter -File -Recurse -Depth $Depth -ErrorAction SilentlyContinue).Count
}

function Wait-ForScreenshots {
  param(
    [System.Diagnostics.Process] $Process,
    [string] $Path,
    [string] $Filter,
    [int] $Depth,
    [int] $ExpectedCount,
    [int] $TimeoutSeconds
  )

  $Deadline = (Get-Date).AddSeconds($TimeoutSeconds)
  while ((-not $Process.HasExited) -and ((Get-Date) -lt $Deadline)) {
    $Count = Get-ScreenshotCount -Path $Path -Filter $Filter -Depth $Depth
    if ($Count -ge $ExpectedCount) {
      return
    }
    Start-Sleep -Seconds 1
    $Process.Refresh()
  }

  $FinalCount = Get-ScreenshotCount -Path $Path -Filter $Filter -Depth $Depth
  Throw-RuntimeBudgetFailure "Runtime budget captured $FinalCount screenshot(s), expected $ExpectedCount."
}

function Get-NamedJsonProperty {
  param(
    [object] $Object,
    [string] $Name
  )

  return $Object.PSObject.Properties[$Name].Value
}

function Get-LimitFieldName {
  param([string] $LimitName)

  return ($LimitName -replace "^max_", "") -replace "^min_", ""
}

function Get-RuntimeBudgetSamples {
  param([string] $Path)

  $Pattern = "runtime-budget sample fps=(?<fps>-?\d+) stack_depth=(?<stack_depth>-?\d+) poly_count=(?<poly_count>-?\d+)"
  Get-Content -LiteralPath $Path |
    ForEach-Object {
      $Match = [regex]::Match($_, $Pattern)
      if ($Match.Success) {
        [pscustomobject]@{
          Fps = [int] $Match.Groups["fps"].Value
          StackDepth = [int] $Match.Groups["stack_depth"].Value
          PolyCount = [int64] $Match.Groups["poly_count"].Value
        }
      }
    } |
    Where-Object { $_.Fps -gt 0 }
}

function Test-ProfileBudget {
  param(
    [object] $Profile,
    [object] $Budget
  )

  $Failures = @()
  foreach ($Limit in $Budget.runtime_budget.profile_limits.PSObject.Properties) {
    $FieldName = Get-LimitFieldName -LimitName $Limit.Name
    $ProfileValue = Get-NamedJsonProperty -Object $Profile.rendering_profile -Name $FieldName
    if (($Limit.Name.StartsWith("max_")) -and ($ProfileValue -gt $Limit.Value)) {
      $Failures += "$FieldName=$ProfileValue exceeds $($Limit.Value)"
    }
    if (($Limit.Name.StartsWith("min_")) -and ($ProfileValue -lt $Limit.Value)) {
      $Failures += "$FieldName=$ProfileValue is below $($Limit.Value)"
    }
  }
  return $Failures
}

function Test-MeasuredBudget {
  param(
    [object] $Budget,
    [object[]] $Samples
  )

  if ($Samples.Count -lt $Budget.runtime_budget.minimum_sample_count) {
    return @("only $($Samples.Count) valid runtime samples captured")
  }

  $FpsStats = $Samples | Measure-Object -Property Fps -Average -Minimum
  $StackStats = $Samples | Measure-Object -Property StackDepth -Maximum
  $PolyStats = $Samples | Measure-Object -Property PolyCount -Maximum
  $Failures = @()

  if ($FpsStats.Average -lt $Budget.runtime_budget.minimum_average_fps) {
    $Failures += ("average_fps={0:N2} is below {1}" -f $FpsStats.Average, $Budget.runtime_budget.minimum_average_fps)
  }
  if ($FpsStats.Minimum -lt $Budget.runtime_budget.minimum_lowest_fps) {
    $Failures += "lowest_fps=$($FpsStats.Minimum) is below $($Budget.runtime_budget.minimum_lowest_fps)"
  }
  if ($StackStats.Maximum -gt $Budget.runtime_budget.maximum_stack_depth) {
    $Failures += "stack_depth=$($StackStats.Maximum) exceeds $($Budget.runtime_budget.maximum_stack_depth)"
  }
  if ($PolyStats.Maximum -gt $Budget.runtime_budget.maximum_poly_count) {
    $Failures += "poly_count=$($PolyStats.Maximum) exceeds $($Budget.runtime_budget.maximum_poly_count)"
  }

  return $Failures
}

function Invoke-RuntimeBudgetValidation {
  param(
    [string] $BudgetPath,
    [string] $ProfilePath,
    [string] $LogPath
  )

  $Budget = Get-Content -LiteralPath $BudgetPath -Raw | ConvertFrom-Json
  $Profile = Get-Content -LiteralPath $ProfilePath -Raw | ConvertFrom-Json
  $Samples = @(Get-RuntimeBudgetSamples -Path $LogPath)
  $Failures = @()
  $Failures += Test-ProfileBudget -Profile $Profile -Budget $Budget
  $Failures += Test-MeasuredBudget -Budget $Budget -Samples $Samples

  if ($Samples.Count -gt 0) {
    $FpsStats = $Samples | Measure-Object -Property Fps -Average -Minimum
    $StackStats = $Samples | Measure-Object -Property StackDepth -Maximum
    $PolyStats = $Samples | Measure-Object -Property PolyCount -Maximum
    Write-Host ("Runtime budget '{0}': samples={1} average_fps={2:N2} lowest_fps={3} max_stack_depth={4} max_poly_count={5}" -f $Budget.runtime_budget.id, $Samples.Count, $FpsStats.Average, $FpsStats.Minimum, $StackStats.Maximum, $PolyStats.Maximum)
  } else {
    Write-Host "Runtime budget '$($Budget.runtime_budget.id)': no valid samples captured"
  }

  if ($Failures.Count -gt 0) {
    Write-Host "Runtime budget check failed:"
    foreach ($Failure in $Failures) {
      Write-Host "  - $Failure"
    }
    Throw-RuntimeBudgetFailure "Runtime budget validation failed."
  }

  Write-Host "Runtime budget check passed."
}

$UnrealEditor = Resolve-UnrealEditorPath -ExplicitPath $UnrealEditor
$BudgetPath = if ([string]::IsNullOrWhiteSpace($BudgetFile)) {
  Join-Path $ProjectRoot "Content\Data\runtime_performance_budget_ps3_baseline.json"
} else {
  $BudgetFile
}

if (-not (Test-Path -LiteralPath $UnrealEditor)) {
  throw "UnrealEditor was not found at $UnrealEditor"
}

if (-not (Test-Path -LiteralPath $ProjectPath)) {
  throw "ForbocAIDemo.uproject was not found at $ProjectPath"
}

if (-not (Test-Path -LiteralPath $BudgetPath)) {
  throw "Runtime budget file was not found at $BudgetPath"
}

$Budget = Get-Content -LiteralPath $BudgetPath -Raw | ConvertFrom-Json
$RuntimeBudgetFailureGuidance = @($Budget.runtime_budget.failure_guidance)
$MeasurementSeconds = [int] $Budget.runtime_budget.measurement_seconds
$StartTimeoutSeconds = if ($StartTimeoutSeconds -gt 0) { $StartTimeoutSeconds } else { [int] $Budget.runtime_budget.start_timeout_seconds }
$ScreenshotIntervalSeconds = [int] $Budget.runtime_budget.screenshot_interval_seconds
$ScreenshotTimeoutSeconds = [int] $Budget.runtime_budget.screenshot_timeout_seconds
$ExpectedScreenshotCount = if ($ScreenshotIntervalSeconds -gt 0) { [int] [Math]::Floor($MeasurementSeconds / $ScreenshotIntervalSeconds) } else { 0 }
$ScreenshotDir = Join-Path $ProjectRoot $Budget.runtime_budget.screenshot_directory
$ScreenshotFileGlob = [string] $Budget.runtime_budget.screenshot_file_glob
$ScreenshotSearchMaxDepth = [int] $Budget.runtime_budget.screenshot_search_max_depth
$ScreenshotIntervalCommandLineKey = [string] $Budget.runtime_budget.screenshot_interval_command_line_key
$MapMarker = [string] $Budget.runtime_budget.markers.map
$TerrainMarker = [string] $Budget.runtime_budget.markers.terrain
$ProfileMarker = [string] $Budget.runtime_budget.markers.profile
$SampleMarker = [string] $Budget.runtime_budget.markers.sample
$ValidSamplePattern = [string] $Budget.runtime_budget.markers.valid_sample_pattern
$FatalErrorMarker = [string] $Budget.runtime_budget.markers.fatal_error

New-Item -ItemType Directory -Force -Path $LogDir | Out-Null
Remove-Item -LiteralPath $LogPath -Force -ErrorAction SilentlyContinue
New-Item -ItemType File -Force -Path $LogPath | Out-Null
Remove-Item -LiteralPath $ScreenshotDir -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $ScreenshotDir | Out-Null

$Arguments = @(
  (Quote-ProcessArgument $ProjectPath),
  $MapPath,
  "-game",
  (Quote-ProcessArgument "-AbsLog=$LogPath"),
  "${ScreenshotIntervalCommandLineKey}${ScreenshotIntervalSeconds}",
  "-log",
  "-stdout",
  "-FullStdOutLogOutput",
  "-unattended",
  "-nop4",
  "-nosplash",
  "-nosound",
  "-NoLiveCoding"
)

Write-Host "=== ForbocAI Runtime Budget ==="
Write-Host "Project:     $ProjectPath"
Write-Host "Map:         $MapPath"
Write-Host "Editor:      $UnrealEditor"
Write-Host "Budget:      $BudgetPath"
Write-Host "Window:      ${MeasurementSeconds}s"
Write-Host "Screenshots: every ${ScreenshotIntervalSeconds}s into $ScreenshotDir"
Write-Host "Rendering:   authored JSON profile applied by game features"
Write-Host "Log:         $LogPath"

$Process = Start-Process -FilePath $UnrealEditor -ArgumentList ($Arguments -join " ") -PassThru

try {
  Wait-ForLogMarker -Process $Process -Path $LogPath -Marker $ProfileMarker -TimeoutSeconds $StartTimeoutSeconds
  Wait-ForLogPattern -Process $Process -Path $LogPath -Pattern $ValidSamplePattern -TimeoutSeconds $StartTimeoutSeconds

  Write-Host "Measuring real Runtime map performance for ${MeasurementSeconds}s..."
  Start-Sleep -Seconds $MeasurementSeconds
  $Process.Refresh()

  if ($Process.HasExited) {
    Throw-RuntimeBudgetFailure "Runtime budget test exited before completing ${MeasurementSeconds}s."
  }

  if ($ExpectedScreenshotCount -gt 0) {
    Write-Host "Waiting for ${ExpectedScreenshotCount} review screenshot(s)..."
    Wait-ForScreenshots -Process $Process -Path $ScreenshotDir -Filter $ScreenshotFileGlob -Depth $ScreenshotSearchMaxDepth -ExpectedCount $ExpectedScreenshotCount -TimeoutSeconds $ScreenshotTimeoutSeconds
  }
}
finally {
  $Process.Refresh()
  if (-not $Process.HasExited) {
    Stop-Process -Id $Process.Id -Force
    $Process.WaitForExit()
  }
}

foreach ($Marker in @($MapMarker, $TerrainMarker, $ProfileMarker, $SampleMarker)) {
  if (-not (Test-LogContains -Path $LogPath -Marker $Marker)) {
    Throw-RuntimeBudgetFailure "Runtime budget log did not contain required marker: $Marker"
  }
}

if (Test-LogContains -Path $LogPath -Marker $FatalErrorMarker) {
  Throw-RuntimeBudgetFailure "Runtime budget log contains a fatal error."
}

Invoke-RuntimeBudgetValidation -BudgetPath $BudgetPath -ProfilePath $ProfilePath -LogPath $LogPath
