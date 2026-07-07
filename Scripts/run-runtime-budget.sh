#!/usr/bin/env bash
# Runs the Runtime map for the authored performance budget window.

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
PROJECT_FILE_ARG="$PROJECT_FILE"
MAP_PATH="${FORBOC_RUNTIME_BUDGET_MAP:-/Game/Map/Maps/Runtime}"
BUDGET_FILE="${FORBOC_RUNTIME_BUDGET_FILE:-$PROJECT_ROOT/Content/Data/runtime_performance_budget_ps3_baseline.json}"
PROFILE_FILE="$PROJECT_ROOT/Content/Data/runtime_settings_rendering_profile.json"
LOG_FILE="$PROJECT_ROOT/Saved/Automation/RuntimeBudgetLog.txt"
LOG_FILE_ARG="$LOG_FILE"
STDOUT_FILE="$PROJECT_ROOT/Saved/Automation/RuntimeBudgetStdout.txt"
VALIDATOR="$PROJECT_ROOT/Scripts/validate-runtime-budget.py"
UNREAL_EDITOR=""
RUNTIME_PID=""
RUNNING_UNDER_WSL=0
RUNTIME_RENDER_ARGS=()

stop_runtime() {
  if [ -n "${RUNTIME_PID:-}" ]; then
    kill "$RUNTIME_PID" 2>/dev/null || true
    wait "$RUNTIME_PID" 2>/dev/null || true
  fi

  if [ "$RUNNING_UNDER_WSL" -eq 1 ]; then
    powershell.exe -NoProfile -Command "& {
      param([string]\$Project, [string]\$Log)
      Get-CimInstance Win32_Process -Filter \"Name='UnrealEditor.exe'\" |
        Where-Object {
          \$_.CommandLine -like \"*\$Project*\" -and
          \$_.CommandLine -like \"*\$Log*\"
        } |
        ForEach-Object { Stop-Process -Id \$_.ProcessId -Force }
    }" "$PROJECT_FILE_ARG" "$LOG_FILE_ARG" >/dev/null 2>&1 || true
  fi
}

trap stop_runtime EXIT

print_normal_game_failure_message() {
  python3 - "$BUDGET_FILE" <<'PY'
import json
import sys
with open(sys.argv[1], encoding="utf-8") as source:
    budget = json.load(source)["runtime_budget"]
for line in budget["failure_guidance"]:
    print(line)
PY
}

fail_runtime_budget() {
  echo "✗ $1"
  print_normal_game_failure_message
  exit 1
}

if [ ! -f "$PROJECT_FILE" ]; then
  echo "Error: ForbocAIDemo.uproject not found at $PROJECT_FILE"
  exit 1
fi

if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]] || [[ "$OSTYPE" == "win32"* ]]; then
  UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.8}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor.exe"
  RUNTIME_RENDER_ARGS=(-d3d11)
elif grep -qi microsoft /proc/version 2>/dev/null; then
  UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.8}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor.exe"
  PROJECT_FILE_ARG="$(wslpath -w "$PROJECT_FILE")"
  LOG_FILE_ARG="$(wslpath -w "$LOG_FILE")"
  RUNNING_UNDER_WSL=1
  RUNTIME_RENDER_ARGS=(-d3d11)
else
  UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.8}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor"
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to your Unreal Engine 5.8 installation directory."
  exit 1
fi

mapfile -t BUDGET_CONFIG < <(python3 - "$BUDGET_FILE" <<'PY'
import json
import sys
with open(sys.argv[1], encoding="utf-8") as source:
    budget = json.load(source)["runtime_budget"]
    markers = budget["markers"]
    screenshot_interval = int(budget["screenshot_interval_seconds"])
    expected_screenshots = int(budget["measurement_seconds"] // screenshot_interval) if screenshot_interval > 0 else 0
    values = [
        int(budget["measurement_seconds"]),
        int(budget["start_timeout_seconds"]),
        screenshot_interval,
        int(budget["screenshot_timeout_seconds"]),
        expected_screenshots,
        budget["screenshot_directory"],
        budget["screenshot_file_glob"],
        int(budget["screenshot_search_max_depth"]),
        budget["screenshot_interval_command_line_key"],
        markers["map"],
        markers["terrain"],
        markers["profile"],
        markers["sample"],
        markers["valid_sample_pattern"],
        markers["fatal_error"],
    ]
    for value in values:
        print(value)
PY
)
MEASUREMENT_SECONDS="${BUDGET_CONFIG[0]}"
START_TIMEOUT_SECONDS="${FORBOC_RUNTIME_BUDGET_START_TIMEOUT_SECONDS:-${BUDGET_CONFIG[1]}}"
SCREENSHOT_INTERVAL_SECONDS="${BUDGET_CONFIG[2]}"
SCREENSHOT_TIMEOUT_SECONDS="${FORBOC_RUNTIME_BUDGET_SCREENSHOT_TIMEOUT_SECONDS:-${BUDGET_CONFIG[3]}}"
EXPECTED_SCREENSHOT_COUNT="${BUDGET_CONFIG[4]}"
SCREENSHOT_DIR="$PROJECT_ROOT/${BUDGET_CONFIG[5]}"
SCREENSHOT_FILE_GLOB="${BUDGET_CONFIG[6]}"
SCREENSHOT_SEARCH_MAX_DEPTH="${BUDGET_CONFIG[7]}"
SCREENSHOT_INTERVAL_COMMAND_LINE_KEY="${BUDGET_CONFIG[8]}"
SCREENSHOT_RUN_MARKER="$PROJECT_ROOT/Saved/Automation/RuntimeBudgetScreenshotStart.marker"
MAP_MARKER="${BUDGET_CONFIG[9]}"
TERRAIN_MARKER="${BUDGET_CONFIG[10]}"
PROFILE_MARKER="${BUDGET_CONFIG[11]}"
SAMPLE_MARKER="${BUDGET_CONFIG[12]}"
VALID_SAMPLE_PATTERN="${BUDGET_CONFIG[13]}"
FATAL_ERROR_MARKER="${BUDGET_CONFIG[14]}"

mkdir -p "$PROJECT_ROOT/Saved/Automation"
rm -f "$LOG_FILE" "$STDOUT_FILE"
mkdir -p "$SCREENSHOT_DIR"
touch "$LOG_FILE"
touch "$SCREENSHOT_RUN_MARKER"

echo "=== ForbocAI Runtime Budget ==="
echo "Project:     $PROJECT_FILE"
echo "Map:         $MAP_PATH"
echo "Editor:      $UNREAL_EDITOR"
echo "Budget:      $BUDGET_FILE"
echo "Window:      ${MEASUREMENT_SECONDS}s"
echo "Screenshots: every ${SCREENSHOT_INTERVAL_SECONDS}s into $SCREENSHOT_DIR"
echo "Mode:        real UnrealEditor game process; visible rendering; no null RHI/offscreen render path"
echo "Rendering:   authored JSON profile applied by game features"
echo "Log:         $LOG_FILE"

(
  "$UNREAL_EDITOR" "$PROJECT_FILE_ARG" "$MAP_PATH" \
    -game \
    "-AbsLog=$LOG_FILE_ARG" \
    "${SCREENSHOT_INTERVAL_COMMAND_LINE_KEY}${SCREENSHOT_INTERVAL_SECONDS}" \
    -log -stdout -FullStdOutLogOutput \
    -unattended -nop4 -nosplash -nosound -NoLiveCoding \
    "${RUNTIME_RENDER_ARGS[@]}" \
    2>&1 | tee "$STDOUT_FILE"
) &
RUNTIME_PID=$!

wait_for_fixed_log_marker() {
  local marker="$1"
  local timeout_message="$2"
  SECONDS=0
  while kill -0 "$RUNTIME_PID" 2>/dev/null; do
    if grep -Fq "$marker" "$LOG_FILE"; then
      return 0
    fi
    if [ "$SECONDS" -ge "$START_TIMEOUT_SECONDS" ]; then
      stop_runtime
      fail_runtime_budget "$timeout_message"
    fi
    sleep 1
  done
  stop_runtime
  fail_runtime_budget "Runtime budget test exited before observing log marker: $marker"
}

wait_for_log_pattern() {
  local pattern="$1"
  local timeout_message="$2"
  SECONDS=0
  while kill -0 "$RUNTIME_PID" 2>/dev/null; do
    if grep -Eq "$pattern" "$LOG_FILE"; then
      return 0
    fi
    if [ "$SECONDS" -ge "$START_TIMEOUT_SECONDS" ]; then
      stop_runtime
      fail_runtime_budget "$timeout_message"
    fi
    sleep 1
  done
  stop_runtime
  fail_runtime_budget "Runtime budget test exited before observing a real runtime sample."
}

screenshot_count() {
  find "$SCREENSHOT_DIR" -maxdepth "$SCREENSHOT_SEARCH_MAX_DEPTH" -type f -name "$SCREENSHOT_FILE_GLOB" -newer "$SCREENSHOT_RUN_MARKER" 2>/dev/null | wc -l | tr -d ' '
}

wait_for_screenshots() {
  local expected_count="$1"
  SECONDS=0
  while kill -0 "$RUNTIME_PID" 2>/dev/null; do
    if [ "$(screenshot_count)" -ge "$expected_count" ]; then
      return 0
    fi
    if [ "$SECONDS" -ge "$SCREENSHOT_TIMEOUT_SECONDS" ]; then
      fail_runtime_budget "Runtime budget captured $(screenshot_count) screenshot(s), expected $expected_count."
    fi
    sleep 1
  done
  stop_runtime
  fail_runtime_budget "Runtime budget test exited before capturing $expected_count screenshot(s)."
}

wait_for_fixed_log_marker \
  "$PROFILE_MARKER" \
  "Runtime budget test timed out before the selected profile was observed."

wait_for_log_pattern \
  "$VALID_SAMPLE_PATTERN" \
  "Runtime budget test timed out before a real nonzero FPS sample was observed."

if ! kill -0 "$RUNTIME_PID" 2>/dev/null; then
  stop_runtime
  fail_runtime_budget "Runtime budget test exited before the measurement window."
fi

echo "Measuring real Runtime map performance for ${MEASUREMENT_SECONDS}s..."
sleep "$MEASUREMENT_SECONDS"

if ! kill -0 "$RUNTIME_PID" 2>/dev/null; then
  stop_runtime
  fail_runtime_budget "Runtime budget test exited before completing ${MEASUREMENT_SECONDS}s."
fi

if [ "$EXPECTED_SCREENSHOT_COUNT" -gt 0 ]; then
  echo "Waiting for ${EXPECTED_SCREENSHOT_COUNT} review screenshot(s)..."
  wait_for_screenshots "$EXPECTED_SCREENSHOT_COUNT"
fi

stop_runtime

for MARKER in "$MAP_MARKER" "$TERRAIN_MARKER" "$PROFILE_MARKER" "$SAMPLE_MARKER"; do
  if ! grep -Fq "$MARKER" "$LOG_FILE"; then
    fail_runtime_budget "Runtime budget log did not contain required marker: $MARKER"
  fi
done

if grep -Fq "$FATAL_ERROR_MARKER" "$LOG_FILE"; then
  fail_runtime_budget "Runtime budget log contains a fatal error."
fi

if ! python3 "$VALIDATOR" --budget "$BUDGET_FILE" --profile "$PROFILE_FILE" --log "$LOG_FILE"; then
  fail_runtime_budget "Runtime budget validation failed."
fi
