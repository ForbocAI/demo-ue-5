#!/usr/bin/env bash
# run-tests.sh
# Runs ForbocAI automation tests for the UE 5.7 demo project.
# Saves logs and distinguishes real failures from offline skips.

set -euo pipefail

# Locate project root
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_FILE="$PROJECT_ROOT/DemoProject.uproject"

if [ ! -f "$PROJECT_FILE" ]; then
  echo "Error: DemoProject.uproject not found at $PROJECT_FILE"
  exit 1
fi

# Detect OS and UE path (same logic as build_plugin.sh)
if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]] || [[ "$OSTYPE" == "win32"* ]]; then
    UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
elif grep -qi microsoft /proc/version 2>/dev/null; then
    UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
else
    UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.7}"
    UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor-Cmd not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to your Unreal Engine 5.7 installation directory."
  exit 1
fi

LOG_FILE="$PROJECT_ROOT/Saved/Automation/AutomationLog.txt"
mkdir -p "$PROJECT_ROOT/Saved/Automation"

echo "=== ForbocAI UE Automation Runner ==="
echo "Project: $PROJECT_FILE"
echo "Editor:  $UNREAL_EDITOR"
echo "Log:     $LOG_FILE"
echo "Running ForbocAI.* tests..."

# Run UE Automation tests
# We use -ExecCmds to run tests and then quit.
# We pipe both stdout and stderr to tee so we can parse it.
set +e
"$UNREAL_EDITOR" "$PROJECT_FILE" -ExecCmds="Automation RunTests ForbocAI; Quit" -log -NoUI -stdout -FullStdOutLogOutput | tee "$LOG_FILE"
UE_EXIT=$?
set -e

echo ""
echo "=== Test Results Analysis ==="

# Parse the log for actual errors vs skips
# Skips typically appear as Warnings or LogAutomationCommandLine: Warning: Test ... skipped
# Failures appear as Error: or Test Failed

FAILURES=$(grep -c "Error:" "$LOG_FILE" || true)
FAILED_TESTS=$(grep -c "Test Failed" "$LOG_FILE" || true)
SKIPS=$(grep -c "skipped" "$LOG_FILE" || grep -c "Skipped" "$LOG_FILE" || true)

TOTAL_FAILURES=$((FAILURES + FAILED_TESTS))

if [ "$TOTAL_FAILURES" -gt 0 ]; then
  echo "✗ Tests failed. Found $TOTAL_FAILURES real failure(s)."
  echo "  (Found $SKIPS offline skip(s)/warning(s))"
  exit 1
else
  if [ "$UE_EXIT" -ne 0 ]; then
    echo "✗ Editor crashed or returned non-zero exit code: $UE_EXIT"
    exit 1
  fi
  echo "✓ All tests passed."
  echo "  (Found $SKIPS offline skip(s)/warning(s) which are treated as successful skips)"
  exit 0
fi
