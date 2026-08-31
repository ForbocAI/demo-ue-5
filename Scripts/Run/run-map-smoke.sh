#!/usr/bin/env bash
# Builds, inspects, and opens the authored default map without live API calls.

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
PROJECT_FILE_ARG="$PROJECT_FILE"
DATA_ROOT="$PROJECT_ROOT/Content/Data"
BUILD_RUNNER_LIB="$PROJECT_ROOT/Scripts/Run/Compilation/build-runner.sh"
RUN_TESTS_SCRIPT="$PROJECT_ROOT/Scripts/Run/run-tests.sh"
UNREAL_EDITOR=""

# shellcheck source=Compilation/build-runner.sh
source "$BUILD_RUNNER_LIB"
forbocai_load_build_runner_policy "$DATA_ROOT"
forbocai_load_map_smoke_policy "$DATA_ROOT"

AUTOMATION_LOG="$PROJECT_ROOT/$FORBOCAI_MAP_SMOKE_AUTOMATION_LOG_PATH"
RUNTIME_LOG="$PROJECT_ROOT/$FORBOCAI_MAP_SMOKE_RUNTIME_LOG_PATH"

if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]] || [[ "$OSTYPE" == "win32"* ]]; then
  UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/$FORBOCAI_ENGINE_DIRECTORY_NAME}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
elif grep -qi microsoft /proc/version 2>/dev/null; then
  UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/$FORBOCAI_ENGINE_DIRECTORY_NAME}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
  PROJECT_FILE_ARG="$(wslpath -w "$PROJECT_FILE")"
else
  UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/$FORBOCAI_ENGINE_DIRECTORY_NAME}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
fi

if [ ! -f "$PROJECT_FILE" ]; then
  echo "Error: ForbocAIDemo.uproject not found at $PROJECT_FILE"
  exit 1
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor-Cmd not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to the Unreal Engine installation directory."
  exit 1
fi

mkdir -p "$PROJECT_ROOT/Saved/Automation"
rm -f "$AUTOMATION_LOG" "$RUNTIME_LOG"

echo "=== ForbocAI Map Smoke ==="
echo "Project: $PROJECT_FILE"
echo "Map:     $FORBOCAI_MAP_SMOKE_MAP_PATH"
echo "Editor:  $UNREAL_EDITOR"
echo "Logs:    $AUTOMATION_LOG and $RUNTIME_LOG"

FORBOC_TEST_ENVIRONMENT_REQUIREMENT="$FORBOCAI_MAP_SMOKE_ENVIRONMENT_REQUIREMENT" \
FORBOC_AUTOMATION_FILTER="$FORBOCAI_MAP_SMOKE_AUTOMATION_TEST" \
FORBOC_AUTOMATION_LOG_FILE="$AUTOMATION_LOG" \
FORBOC_RUN_RUNTIME_BUDGET="$FORBOCAI_MAP_SMOKE_RUN_RUNTIME_BUDGET" \
  bash "$RUN_TESTS_SCRIPT"

python3 - "$FORBOCAI_MAP_SMOKE_POLICY_DIRECTORY" "$AUTOMATION_LOG" <<'PY'
import json
import sys
from pathlib import Path

policy_directory = Path(sys.argv[1])
log_path = Path(sys.argv[2])
with (policy_directory / "config.json").open(encoding="utf-8") as source:
    config = json.load(source)
with (policy_directory / "automation" / "failure-markers.json").open(encoding="utf-8") as source:
    failure_markers = json.load(source)
log = log_path.read_text(encoding="utf-8", errors="replace")
success_marker = config["automation_success_marker"]
failures = tuple(marker for marker in failure_markers if marker in log)
if success_marker not in log:
    raise SystemExit("Map automation missing marker: " + success_marker)
if failures:
    raise SystemExit(
        "Map automation observed failure markers: " + ", ".join(failures)
    )
PY

SECONDS=0
(
  "$UNREAL_EDITOR" "$PROJECT_FILE_ARG" "$FORBOCAI_MAP_SMOKE_MAP_PATH" \
    -game \
    -ExecCmds="Quit" \
    -log -stdout -FullStdOutLogOutput \
    -unattended -nop4 -nosplash -nullrhi -nosound -NoLiveCoding \
    2>&1 | tee "$RUNTIME_LOG"
) &
SMOKE_PID=$!
SMOKE_EXIT=0

while kill -0 "$SMOKE_PID" 2>/dev/null; do
  if [ "$SECONDS" -ge "$FORBOCAI_MAP_SMOKE_TIMEOUT_SECONDS" ]; then
    kill "$SMOKE_PID" 2>/dev/null || true
    wait "$SMOKE_PID" 2>/dev/null || true
    echo "Map smoke timed out after $FORBOCAI_MAP_SMOKE_TIMEOUT_SECONDS seconds."
    exit 1
  fi
  sleep 2
done

wait "$SMOKE_PID" || SMOKE_EXIT=$?
if [ "$SMOKE_EXIT" -ne 0 ]; then
  echo "Map smoke failed with exit code: $SMOKE_EXIT"
  exit 1
fi

python3 - "$FORBOCAI_MAP_SMOKE_POLICY_DIRECTORY" "$RUNTIME_LOG" "$FORBOCAI_MAP_SMOKE_MAP_PATH" <<'PY'
import json
import sys
from pathlib import Path

policy_directory = Path(sys.argv[1])
log_path = Path(sys.argv[2])
map_path = sys.argv[3]
with (policy_directory / "runtime" / "required-markers.json").open(encoding="utf-8") as source:
    required_markers = json.load(source)
with (policy_directory / "runtime" / "failure-markers.json").open(encoding="utf-8") as source:
    failure_markers = json.load(source)
log = log_path.read_text(encoding="utf-8", errors="replace")
required = (f"LogLoad: LoadMap: {map_path}", *required_markers)
missing = tuple(marker for marker in required if marker not in log)
failures = tuple(
    marker for marker in failure_markers if marker in log
)
if missing:
    raise SystemExit("Map smoke missing markers: " + ", ".join(missing))
if failures:
    raise SystemExit("Map smoke observed failure markers: " + ", ".join(failures))
PY

echo "Map smoke qualified $FORBOCAI_MAP_SMOKE_MAP_PATH."
