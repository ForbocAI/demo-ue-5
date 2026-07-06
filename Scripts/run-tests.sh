#!/usr/bin/env bash
# run-tests.sh
# Runs ForbocAI automation tests for the UE 5.7 runtime project.
# Saves logs and distinguishes real failures from offline skips.

set -euo pipefail

# Locate project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
PROJECT_FILE_ARG="$PROJECT_FILE"
RUNTIME_BUDGET_SCRIPT="$PROJECT_ROOT/Scripts/run-runtime-budget.sh"
RUNTIME_BUDGET_POWERSHELL_SCRIPT="$PROJECT_ROOT/Scripts/run-runtime-budget.ps1"
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
    UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Build.bat"
    UNREAL_BUILD_ARG="$UNREAL_BUILD"
    RUNTIME_BUDGET_VIA_POWERSHELL=1
    if command -v cygpath >/dev/null 2>&1; then
      RUNTIME_BUDGET_POWERSHELL_ARG="$(cygpath -w "$RUNTIME_BUDGET_POWERSHELL_SCRIPT")"
    fi
elif grep -qi microsoft /proc/version 2>/dev/null; then
    UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Build.bat"
    PROJECT_FILE_ARG="$(wslpath -w "$PROJECT_FILE")"
    UNREAL_BUILD_ARG="$(wslpath -w "$UNREAL_BUILD")"
    RUNTIME_BUDGET_POWERSHELL_ARG="$(wslpath -w "$RUNTIME_BUDGET_POWERSHELL_SCRIPT")"
    RUNTIME_BUDGET_VIA_POWERSHELL=1
    BUILD_VIA_CMD=1
else
    UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
    UNREAL_BUILD="$UE_ROOT/Engine/Build/BatchFiles/Mac/Build.sh"
    UNREAL_BUILD_ARG="$UNREAL_BUILD"
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor-Cmd not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to your Unreal Engine 5.7 installation directory."
  exit 1
fi

if [ -n "$UNREAL_BUILD" ] && [ ! -f "$UNREAL_BUILD" ]; then
  echo "Error: Unreal build script not found at $UNREAL_BUILD"
  echo "Please set UE_ROOT to your Unreal Engine 5.7 installation directory."
  exit 1
fi

LOG_FILE="$PROJECT_ROOT/Saved/Automation/AutomationLog.txt"
mkdir -p "$PROJECT_ROOT/Saved/Automation"

echo "=== ForbocAI UE Automation Runner ==="
echo "Project: $PROJECT_FILE"
echo "Editor:  $UNREAL_EDITOR"
echo "Log:     $LOG_FILE"
if [ -n "$UNREAL_BUILD" ]; then
  echo "Build:   $UNREAL_BUILD"
fi
echo "Running ForbocAI.* tests..."

echo "Locking SDK submodule read-only..."
bash "$PROJECT_ROOT/Scripts/lock_sdk_submodule.sh" --lock

echo "Checking SDK submodule immutability..."
bash "$PROJECT_ROOT/Scripts/check_sdk_submodule_guard.sh"

echo "Checking feature C++ parameter discipline..."
python3 "$PROJECT_ROOT/Scripts/check_param_count.py" --strict

echo "Checking branchless FP source discipline..."
python3 "$PROJECT_ROOT/Scripts/check_branchless_source.py" --self-test
python3 "$PROJECT_ROOT/Scripts/check_branchless_source.py" "$PROJECT_ROOT/Source"

echo "Checking runtime rendering JSON tuning discipline..."
python3 "$PROJECT_ROOT/Scripts/check_runtime_rendering_json_tuning.py" --self-test
python3 "$PROJECT_ROOT/Scripts/check_runtime_rendering_json_tuning.py" \
  "$PROJECT_ROOT/Source/Features/Systems/Rendering/RenderingProfileThunks.cpp"

echo "Validating runtime settings JSON..."
for JSON_FILE in "$PROJECT_ROOT/Content/Data/runtime_settings.json" "$PROJECT_ROOT"/Content/Data/runtime_settings_*.json; do
  python3 -m json.tool "$JSON_FILE" >/dev/null
done

echo "Updating file line count documentation..."
python3 "$PROJECT_ROOT/Scripts/DOCS/count_project_lines.py"

echo "Checking diff whitespace..."
git -C "$PROJECT_ROOT" diff --check

if [ -n "$UNREAL_BUILD" ]; then
  echo "Ensuring ForbocAIDemoEditor is built..."
  set +e
  if [ "$BUILD_VIA_CMD" -eq 1 ]; then
    powershell.exe -NoProfile -Command "& { & '$UNREAL_BUILD_ARG' 'ForbocAIDemoEditor' 'Win64' 'Development' '-Project=$PROJECT_FILE_ARG' '-WaitMutex' '-NoHotReloadFromIDE'; exit \$LASTEXITCODE }"
  else
    "$UNREAL_BUILD" ForbocAIDemoEditor Win64 Development "-Project=$PROJECT_FILE_ARG" -WaitMutex -NoHotReloadFromIDE
  fi
  BUILD_EXIT=$?
  set -e
  if [ "$BUILD_EXIT" -ne 0 ]; then
    echo "✗ Build failed with exit code: $BUILD_EXIT"
    exit 1
  fi
fi

# Run UE Automation tests
# We use -ExecCmds to run tests and then quit.
# We pipe both stdout and stderr to tee so we can parse it.
set +e
"$UNREAL_EDITOR" "$PROJECT_FILE_ARG" \
  -ExecCmds="Automation RunTests ForbocAI; Quit" \
  -log -NoUI -stdout -FullStdOutLogOutput \
  -unattended -nop4 -nosplash -nullrhi -nosound -NoLiveCoding \
  | tee "$LOG_FILE"
UE_EXIT=$?
set -e

echo ""
echo "=== Test Results Analysis ==="

# Parse the log for actual test failures vs skips. Unreal startup logs can
# contain benign Error: lines, and test names may contain words like "Failed".
FAILURES=$(grep -Fc "Test Completed. Result={Fail" "$LOG_FILE" || true)
CONTROLLER_ERRORS=$(grep -Fc "LogAutomationController: Error:" "$LOG_FILE" || true)
COMMAND_ERRORS=$(grep -Fc "LogAutomationCommandLine: Error:" "$LOG_FILE" || true)
SKIPS=$(grep -Eic "skipp(ed|ing)" "$LOG_FILE" || true)

TOTAL_FAILURES=$((FAILURES + CONTROLLER_ERRORS + COMMAND_ERRORS))

if [ "$TOTAL_FAILURES" -gt 0 ]; then
  echo "✗ Tests failed. Found $TOTAL_FAILURES real failure(s)."
  echo "  (Found $SKIPS offline skip(s)/warning(s))"
  exit 1
else
  if [ "$UE_EXIT" -ne 0 ]; then
    echo "✗ Editor crashed or returned non-zero exit code: $UE_EXIT"
    exit 1
  fi
  echo "Running 30-second Runtime map performance budget test..."
  if [ "$RUNTIME_BUDGET_VIA_POWERSHELL" -eq 1 ]; then
    powershell.exe -NoProfile -ExecutionPolicy Bypass -File "$RUNTIME_BUDGET_POWERSHELL_ARG"
  else
    bash "$RUNTIME_BUDGET_SCRIPT"
  fi
  echo "✓ All tests passed."
  echo "  (Found $SKIPS offline skip(s)/warning(s) which are treated as successful skips)"
  exit 0
fi
