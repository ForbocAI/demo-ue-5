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

function Get-OptionalBudgetValue {
  param(
    [object] $Budget,
    [string] $Name,
    [object] $DefaultValue
  )

  $Property = $Budget.runtime_budget.PSObject.Properties[$Name]
  if ($null -eq $Property) {
    return $DefaultValue
  }
  return $Property.Value
}

function Get-RuntimeBudgetSamples {
  param([string] $Path)

  $Pattern = "runtime-budget sample fps=(?<fps>-?\d+) stack_depth=(?<stack_depth>-?\d+) poly_count=(?<poly_count>-?\d+) memory_mib=(?<memory_mib>-?\d+) peak_memory_mib=(?<peak_memory_mib>-?\d+) virtual_memory_mib=(?<virtual_memory_mib>-?\d+) game_ms=(?<game_ms>-?\d+(?:\.\d+)?) render_ms=(?<render_ms>-?\d+(?:\.\d+)?) rhi_ms=(?<rhi_ms>-?\d+(?:\.\d+)?) gpu_ms=(?<gpu_ms>-?\d+(?:\.\d+)?) draw_calls=(?<draw_calls>-?\d+) rhi_primitives=(?<rhi_primitives>-?\d+) wall_ms=(?<wall_ms>-?\d+(?:\.\d+)?) input_delta_ms=(?<input_delta_ms>-?\d+(?:\.\d+)?) stats_select_ms=(?<stats_select_ms>-?\d+(?:\.\d+)?) poly_count_ms=(?<poly_count_ms>-?\d+(?:\.\d+)?) engine_idle_ms=(?<engine_idle_ms>-?\d+(?:\.\d+)?) engine_idle_overshoot_ms=(?<engine_idle_overshoot_ms>-?\d+(?:\.\d+)?) max_fps=(?<max_fps>-?\d+(?:\.\d+)?) frame_rate_limit=(?<frame_rate_limit>-?\d+(?:\.\d+)?) effective_max_tick_rate=(?<effective_max_tick_rate>-?\d+(?:\.\d+)?) fixed_frame_rate_enabled=(?<fixed_frame_rate_enabled>-?\d+) fixed_frame_rate=(?<fixed_frame_rate>-?\d+(?:\.\d+)?) fixed_time_step_enabled=(?<fixed_time_step_enabled>-?\d+) fixed_delta_ms=(?<fixed_delta_ms>-?\d+(?:\.\d+)?) vsync=(?<vsync>-?\d+) idle_when_not_foreground=(?<idle_when_not_foreground>-?\d+) app_has_focus=(?<app_has_focus>-?\d+) cpu_throttle=(?<cpu_throttle>-?\d+) all_windows_hidden=(?<all_windows_hidden>-?\d+) root_reducer_ms=(?<root_reducer_ms>-?\d+(?:\.\d+)?) combined_reducer_ms=(?<combined_reducer_ms>-?\d+(?:\.\d+)?) ecs_projection_ms=(?<ecs_projection_ms>-?\d+(?:\.\d+)?) ecs_entities=(?<ecs_entities>-?\d+) ecs_component_types=(?<ecs_component_types>-?\d+)"
  Get-Content -LiteralPath $Path |
    ForEach-Object {
      $Match = [regex]::Match($_, $Pattern)
      if ($Match.Success) {
        [pscustomobject]@{
          Fps = [int] $Match.Groups["fps"].Value
          StackDepth = [int] $Match.Groups["stack_depth"].Value
          PolyCount = [int64] $Match.Groups["poly_count"].Value
          MemoryMiB = [int64] $Match.Groups["memory_mib"].Value
          PeakMemoryMiB = [int64] $Match.Groups["peak_memory_mib"].Value
          VirtualMemoryMiB = [int64] $Match.Groups["virtual_memory_mib"].Value
          GameMs = [double] $Match.Groups["game_ms"].Value
          RenderMs = [double] $Match.Groups["render_ms"].Value
          RhiMs = [double] $Match.Groups["rhi_ms"].Value
          GpuMs = [double] $Match.Groups["gpu_ms"].Value
          DrawCalls = [int64] $Match.Groups["draw_calls"].Value
          RhiPrimitives = [int64] $Match.Groups["rhi_primitives"].Value
          WallMs = [double] $Match.Groups["wall_ms"].Value
          InputDeltaMs = [double] $Match.Groups["input_delta_ms"].Value
          StatsSelectMs = [double] $Match.Groups["stats_select_ms"].Value
          PolyCountMs = [double] $Match.Groups["poly_count_ms"].Value
          EngineIdleMs = [double] $Match.Groups["engine_idle_ms"].Value
          EngineIdleOvershootMs = [double] $Match.Groups["engine_idle_overshoot_ms"].Value
          MaxFps = [double] $Match.Groups["max_fps"].Value
          FrameRateLimit = [double] $Match.Groups["frame_rate_limit"].Value
          EffectiveMaxTickRate = [double] $Match.Groups["effective_max_tick_rate"].Value
          FixedFrameRateEnabled = [int] $Match.Groups["fixed_frame_rate_enabled"].Value
          FixedFrameRate = [double] $Match.Groups["fixed_frame_rate"].Value
          FixedTimeStepEnabled = [int] $Match.Groups["fixed_time_step_enabled"].Value
          FixedDeltaMs = [double] $Match.Groups["fixed_delta_ms"].Value
          Vsync = [int] $Match.Groups["vsync"].Value
          IdleWhenNotForeground = [int] $Match.Groups["idle_when_not_foreground"].Value
          AppHasFocus = [int] $Match.Groups["app_has_focus"].Value
          CpuThrottle = [int] $Match.Groups["cpu_throttle"].Value
          AllWindowsHidden = [int] $Match.Groups["all_windows_hidden"].Value
          RootReducerMs = [double] $Match.Groups["root_reducer_ms"].Value
          CombinedReducerMs = [double] $Match.Groups["combined_reducer_ms"].Value
          EcsProjectionMs = [double] $Match.Groups["ecs_projection_ms"].Value
          EcsEntities = [int] $Match.Groups["ecs_entities"].Value
          EcsComponentTypes = [int] $Match.Groups["ecs_component_types"].Value
        }
      }
    } |
    Where-Object { $_.Fps -gt 0 }
}

function Get-MeasuredRuntimeBudgetSamples {
  param(
    [object] $Budget,
    [object[]] $Samples
  )

  $WarmupCount = [int] (Get-OptionalBudgetValue -Budget $Budget -Name "measurement_warmup_sample_count" -DefaultValue 0)
  $ScreenshotStallWallMs = [double] (Get-OptionalBudgetValue -Budget $Budget -Name "screenshot_stall_wall_ms" -DefaultValue ([double]::PositiveInfinity))
  $ScreenshotStallInputDeltaMs = [double] (Get-OptionalBudgetValue -Budget $Budget -Name "screenshot_stall_input_delta_ms" -DefaultValue 0.0)
  return @($Samples |
      Select-Object -Skip $WarmupCount |
      Where-Object {
        !(($_.WallMs -ge $ScreenshotStallWallMs) -and
          ($_.InputDeltaMs -le $ScreenshotStallInputDeltaMs))
      })
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
  $MemoryStats = $Samples | Measure-Object -Property MemoryMiB -Maximum
  $PeakMemoryStats = $Samples | Measure-Object -Property PeakMemoryMiB -Maximum
  $VirtualMemoryStats = $Samples | Measure-Object -Property VirtualMemoryMiB -Maximum
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
  if ($MemoryStats.Maximum -gt $Budget.runtime_budget.maximum_memory_mib) {
    $Failures += "memory_mib=$($MemoryStats.Maximum) exceeds $($Budget.runtime_budget.maximum_memory_mib)"
  }
  if ($PeakMemoryStats.Maximum -gt $Budget.runtime_budget.maximum_peak_memory_mib) {
    $Failures += "peak_memory_mib=$($PeakMemoryStats.Maximum) exceeds $($Budget.runtime_budget.maximum_peak_memory_mib)"
  }
  if ($VirtualMemoryStats.Maximum -gt $Budget.runtime_budget.maximum_virtual_memory_mib) {
    $Failures += "virtual_memory_mib=$($VirtualMemoryStats.Maximum) exceeds $($Budget.runtime_budget.maximum_virtual_memory_mib)"
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
  $RawSamples = @(Get-RuntimeBudgetSamples -Path $LogPath)
  $Samples = @(Get-MeasuredRuntimeBudgetSamples -Budget $Budget -Samples $RawSamples)
  $Failures = @()
  $Failures += Test-ProfileBudget -Profile $Profile -Budget $Budget
  $Failures += Test-MeasuredBudget -Budget $Budget -Samples $Samples

  if ($Samples.Count -gt 0) {
    $FpsStats = $Samples | Measure-Object -Property Fps -Average -Minimum
    $StackStats = $Samples | Measure-Object -Property StackDepth -Maximum
    $PolyStats = $Samples | Measure-Object -Property PolyCount -Maximum
    $MemoryStats = $Samples | Measure-Object -Property MemoryMiB -Maximum
    $PeakMemoryStats = $Samples | Measure-Object -Property PeakMemoryMiB -Maximum
    $VirtualMemoryStats = $Samples | Measure-Object -Property VirtualMemoryMiB -Maximum
    $GameStats = $Samples | Measure-Object -Property GameMs -Maximum
    $RenderStats = $Samples | Measure-Object -Property RenderMs -Maximum
    $RhiStats = $Samples | Measure-Object -Property RhiMs -Maximum
    $GpuStats = $Samples | Measure-Object -Property GpuMs -Maximum
    $DrawStats = $Samples | Measure-Object -Property DrawCalls -Maximum
    $RhiPrimitiveStats = $Samples | Measure-Object -Property RhiPrimitives -Maximum
    $WallStats = $Samples | Measure-Object -Property WallMs -Maximum
    $InputDeltaStats = $Samples | Measure-Object -Property InputDeltaMs -Maximum
    $StatsSelectStats = $Samples | Measure-Object -Property StatsSelectMs -Maximum
    $PolyCountMsStats = $Samples | Measure-Object -Property PolyCountMs -Maximum
    $EngineIdleStats = $Samples | Measure-Object -Property EngineIdleMs -Maximum
    $EngineIdleOvershootStats = $Samples | Measure-Object -Property EngineIdleOvershootMs -Maximum
    $MaxFpsStats = $Samples | Measure-Object -Property MaxFps -Maximum
    $FrameRateLimitStats = $Samples | Measure-Object -Property FrameRateLimit -Maximum
    $EffectiveMaxTickRateStats = $Samples | Measure-Object -Property EffectiveMaxTickRate -Maximum
    $FixedFrameRateEnabledStats = $Samples | Measure-Object -Property FixedFrameRateEnabled -Maximum
    $FixedFrameRateStats = $Samples | Measure-Object -Property FixedFrameRate -Maximum
    $FixedTimeStepEnabledStats = $Samples | Measure-Object -Property FixedTimeStepEnabled -Maximum
    $FixedDeltaMsStats = $Samples | Measure-Object -Property FixedDeltaMs -Maximum
    $VsyncStats = $Samples | Measure-Object -Property Vsync -Maximum
    $IdleWhenNotForegroundStats = $Samples | Measure-Object -Property IdleWhenNotForeground -Maximum
    $AppHasFocusStats = $Samples | Measure-Object -Property AppHasFocus -Minimum
    $CpuThrottleStats = $Samples | Measure-Object -Property CpuThrottle -Maximum
    $AllWindowsHiddenStats = $Samples | Measure-Object -Property AllWindowsHidden -Maximum
    $RootReducerStats = $Samples | Measure-Object -Property RootReducerMs -Maximum
    $CombinedReducerStats = $Samples | Measure-Object -Property CombinedReducerMs -Maximum
    $EcsProjectionStats = $Samples | Measure-Object -Property EcsProjectionMs -Maximum
    $EcsEntityStats = $Samples | Measure-Object -Property EcsEntities -Maximum
    $EcsComponentTypeStats = $Samples | Measure-Object -Property EcsComponentTypes -Maximum
    Write-Host ("Runtime budget '{0}': samples={1} average_fps={2:N2} lowest_fps={3} max_stack_depth={4} max_poly_count={5} max_memory_mib={6} max_peak_memory_mib={7} max_virtual_memory_mib={8} max_game_ms={9:N2} max_render_ms={10:N2} max_rhi_ms={11:N2} max_gpu_ms={12:N2} max_draw_calls={13} max_rhi_primitives={14} max_wall_ms={15:N2} max_input_delta_ms={16:N2} max_stats_select_ms={17:N2} max_poly_count_ms={18:N2} max_engine_idle_ms={19:N2} max_engine_idle_overshoot_ms={20:N2} max_fps_cap={21:N2} max_frame_rate_limit={22:N2} max_effective_tick_rate={23:N2} max_fixed_frame_rate_enabled={24} max_fixed_frame_rate={25:N2} max_fixed_time_step_enabled={26} max_fixed_delta_ms={27:N2} max_vsync={28} max_idle_when_not_foreground={29} min_app_has_focus={30} max_cpu_throttle={31} max_all_windows_hidden={32} max_root_reducer_ms={33:N2} max_combined_reducer_ms={34:N2} max_ecs_projection_ms={35:N2} max_ecs_entities={36} max_ecs_component_types={37}" -f $Budget.runtime_budget.id, $Samples.Count, $FpsStats.Average, $FpsStats.Minimum, $StackStats.Maximum, $PolyStats.Maximum, $MemoryStats.Maximum, $PeakMemoryStats.Maximum, $VirtualMemoryStats.Maximum, $GameStats.Maximum, $RenderStats.Maximum, $RhiStats.Maximum, $GpuStats.Maximum, $DrawStats.Maximum, $RhiPrimitiveStats.Maximum, $WallStats.Maximum, $InputDeltaStats.Maximum, $StatsSelectStats.Maximum, $PolyCountMsStats.Maximum, $EngineIdleStats.Maximum, $EngineIdleOvershootStats.Maximum, $MaxFpsStats.Maximum, $FrameRateLimitStats.Maximum, $EffectiveMaxTickRateStats.Maximum, $FixedFrameRateEnabledStats.Maximum, $FixedFrameRateStats.Maximum, $FixedTimeStepEnabledStats.Maximum, $FixedDeltaMsStats.Maximum, $VsyncStats.Maximum, $IdleWhenNotForegroundStats.Maximum, $AppHasFocusStats.Minimum, $CpuThrottleStats.Maximum, $AllWindowsHiddenStats.Maximum, $RootReducerStats.Maximum, $CombinedReducerStats.Maximum, $EcsProjectionStats.Maximum, $EcsEntityStats.Maximum, $EcsComponentTypeStats.Maximum)
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
