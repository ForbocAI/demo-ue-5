#!/usr/bin/env bash
# run-tests.sh
# Runs ForbocAI automation tests for the UE 5.8 runtime project.
# Saves logs and distinguishes real failures from offline skips.

set -uo pipefail

# Locate project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
PROJECT_FILE_ARG="$PROJECT_FILE"
RUNTIME_BUDGET_SCRIPT="$PROJECT_ROOT/Scripts/Run/run-runtime-budget.sh"
RUNTIME_BUDGET_POWERSHELL_SCRIPT="$PROJECT_ROOT/Scripts/Run/run-runtime-budget.ps1"
RUNTIME_BUDGET_POWERSHELL_ARG="$RUNTIME_BUDGET_POWERSHELL_SCRIPT"
RUNTIME_BUDGET_VIA_POWERSHELL=0
UNREAL_BUILD=""
UNREAL_BUILD_ARG=""
BUILD_VIA_CMD=0

if [ ! -f "$PROJECT_FILE" ]; then
  echo "Error: ForbocAIDemo.uproject not found at $PROJECT_FILE"
  exit 1
fi

# Detect OS and UE path (same logic as build_plugin.sh)
if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]] || [[ "$OSTYPE" == "win32"* ]]; then
    UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.8}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Build.bat"
    UNREAL_BUILD_ARG="$UNREAL_BUILD"
    RUNTIME_BUDGET_VIA_POWERSHELL=1
    if command -v cygpath >/dev/null 2>&1; then
      RUNTIME_BUDGET_POWERSHELL_ARG="$(cygpath -w "$RUNTIME_BUDGET_POWERSHELL_SCRIPT")"
    fi
elif grep -qi microsoft /proc/version 2>/dev/null; then
    UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.8}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Build.bat"
    PROJECT_FILE_ARG="$(wslpath -w "$PROJECT_FILE")"
    UNREAL_BUILD_ARG="$(wslpath -w "$UNREAL_BUILD")"
    RUNTIME_BUDGET_POWERSHELL_ARG="$(wslpath -w "$RUNTIME_BUDGET_POWERSHELL_SCRIPT")"
    RUNTIME_BUDGET_VIA_POWERSHELL=1
    BUILD_VIA_CMD=1
else
    UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.8}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Mac/Build.sh"
    UNREAL_BUILD_ARG="$UNREAL_BUILD"
fi

LOG_FILE="$PROJECT_ROOT/Saved/Automation/AutomationLog.txt"
mkdir -p "$PROJECT_ROOT/Saved/Automation"

CHECK_FAILURES=0
FAILED_CHECKS=()

record_check_failure() {
  local label="$1"
  local exit_code="$2"

  CHECK_FAILURES=$((CHECK_FAILURES + 1))
  FAILED_CHECKS+=("${label%...} (exit $exit_code)")
  echo "✗ ${label%...} failed with exit code: $exit_code"
}

run_check() {
  local label="$1"
  shift

  echo "$label"
  "$@"
  local exit_code=$?
  if [ "$exit_code" -ne 0 ]; then
    record_check_failure "$label" "$exit_code"
  fi
  return 0
}

validate_authored_data_json() {
  local failures=0
  while IFS= read -r -d '' JSON_FILE; do
    if ! python3 -m json.tool "$JSON_FILE" >/dev/null; then
      echo "Invalid JSON: $JSON_FILE"
      failures=$((failures + 1))
    fi
  done < <(find "$PROJECT_ROOT/Content/Data" -type f -name '*.json' -print0)

  if [ "$failures" -ne 0 ]; then
    echo "Found $failures invalid authored JSON file(s)."
    return 1
  fi
  return 0
}

echo "=== ForbocAI UE Automation Runner ==="
echo "Project: $PROJECT_FILE"
echo "Editor:  $UNREAL_EDITOR"
echo "Log:     $LOG_FILE"
if [ -n "$UNREAL_BUILD" ]; then
  echo "Build:   $UNREAL_BUILD"
fi
echo "Running ForbocAI.* tests..."

run_check "Locking SDK submodule read-only..." bash "$PROJECT_ROOT/Scripts/SDK/lock_sdk_submodule.sh" --lock
run_check "Checking SDK submodule immutability..." bash "$PROJECT_ROOT/Scripts/Checks/check_sdk_submodule_guard.sh"
run_check "Checking feature C++ parameter discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/fp/param_count.py"
run_check "Checking function/data composition naming discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/fp/function_composition.py"
run_check "Checking branchless FP source discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/fp/branchless_source.py"
run_check "Checking RTK/ECS boundary discipline (Features + Views)..." python3 "$PROJECT_ROOT/Scripts/Checks/check_redux.py"
run_check "Checking runtime rendering JSON tuning discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/check_source_for_data.py"
run_check "Checking authored data naming discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/ecs/data_naming.py"
run_check "Checking domain/subdomain path ownership..." python3 "$PROJECT_ROOT/Scripts/Checks/ecs/domain_boundaries.py"
run_check "Checking Source/Content file size discipline..." python3 "$PROJECT_ROOT/Scripts/Checks/check_line_count.py"
run_check "Checking for stale dead code/data (orphan files)..." python3 "$PROJECT_ROOT/Scripts/Checks/check_dead_code.py"
run_check "Validating authored data JSON..." validate_authored_data_json
run_check "Updating file line count documentation..." python3 "$PROJECT_ROOT/Scripts/Docs/count_project_lines.py"
run_check "Checking diff whitespace..." git -C "$PROJECT_ROOT" diff --check

if [ "$CHECK_FAILURES" -ne 0 ]; then
  echo ""
  echo "=== Preflight Failure Summary ==="
  printf ' - %s\n' "${FAILED_CHECKS[@]}"
  echo "✗ Preflight failed after running all checks."
  exit 1
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor-Cmd not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to your Unreal Engine 5.8 installation directory."
  exit 1
fi

if [ -n "$UNREAL_BUILD" ] && [ ! -f "$UNREAL_BUILD" ]; then
  echo "Error: Unreal build script not found at $UNREAL_BUILD"
  echo "Please set UE_ROOT to your Unreal Engine 5.8 installation directory."
  exit 1
fi

if [ -n "$UNREAL_BUILD" ]; then
  echo "Ensuring ForbocAIDemoEditor is built..."
  if [ "$BUILD_VIA_CMD" -eq 1 ]; then
    powershell.exe -NoProfile -NonInteractive -InputFormat None -Command "& { & '$UNREAL_BUILD_ARG' 'ForbocAIDemoEditor' 'Win64' 'Development' '-Project=$PROJECT_FILE_ARG' '-WaitMutex' '-NoHotReloadFromIDE'; exit \$LASTEXITCODE }" < /dev/null
  else
    "$UNREAL_BUILD" ForbocAIDemoEditor Win64 Development "-Project=$PROJECT_FILE_ARG" -WaitMutex -NoHotReloadFromIDE
  fi
  BUILD_EXIT=$?
  if [ "$BUILD_EXIT" -ne 0 ]; then
    echo "✗ Build failed with exit code: $BUILD_EXIT"
    exit 1
  fi
fi

# Run UE Automation tests
# We use -ExecCmds to run tests and then quit.
# We pipe both stdout and stderr to tee so we can parse it.
"$UNREAL_EDITOR" "$PROJECT_FILE_ARG" \
  -ExecCmds="Automation RunTests ForbocAI; Quit" \
  -log -NoUI -stdout -FullStdOutLogOutput \
  -unattended -nop4 -nosplash -nullrhi -nosound -NoLiveCoding \
  | tee "$LOG_FILE"
UE_EXIT=$?

echo ""
echo "=== Test Results Analysis ==="

# Parse the log for actual test failures vs skips. Unreal startup logs can
# contain benign Error: lines, and test names may contain words like "Failed".
FAILURES=$(grep -Fc "Test Completed. Result={Fail" "$LOG_FILE" || true)
CONTROLLER_ERRORS=$(grep -Fc "LogAutomationController: Error:" "$LOG_FILE" || true)
COMMAND_ERRORS=$(grep -Fc "LogAutomationCommandLine: Error:" "$LOG_FILE" || true)
WARNINGS=$(grep -Eic '(^|[[:space:]])Warning:' "$LOG_FILE" || true)
SKIPS=$(grep -Eic "skipp(ed|ing)" "$LOG_FILE" || true)

TOTAL_FAILURES=$((FAILURES + CONTROLLER_ERRORS + COMMAND_ERRORS + WARNINGS))

if [ "$TOTAL_FAILURES" -gt 0 ]; then
  echo "✗ Tests failed. Found $TOTAL_FAILURES failure(s), including $WARNINGS warning(s)."
  echo "  (Found $SKIPS offline skip(s))"
  exit 1
else
  if [ "$UE_EXIT" -ne 0 ]; then
    echo "✗ Editor crashed or returned non-zero exit code: $UE_EXIT"
    exit 1
  fi
  echo "Running 30-second Runtime map performance budget test in real UnrealEditor game mode..."
  if [ "$RUNTIME_BUDGET_VIA_POWERSHELL" -eq 1 ]; then
    powershell.exe -NoProfile -NonInteractive -InputFormat None -ExecutionPolicy Bypass -File "$RUNTIME_BUDGET_POWERSHELL_ARG" < /dev/null
  else
    bash "$RUNTIME_BUDGET_SCRIPT"
  fi
  echo "✓ All tests passed."
  echo "  (Found $SKIPS offline skip(s))"
  exit 0
fi
