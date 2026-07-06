#!/usr/bin/env bash
# Launches the Runtime map in game mode and verifies runtime map actions.

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
PROJECT_FILE_ARG="$PROJECT_FILE"
MAP_PATH="${FORBOC_RUNTIME_SMOKE_MAP:-/Game/Map/Maps/Runtime}"
LOG_FILE="$PROJECT_ROOT/Saved/Automation/RuntimeSmokeLog.txt"
MAP_MARKER="LogLoad: LoadMap: /Game/Map/Maps/Runtime"
TERRAIN_MARKER="LogForbocRuntimeRedux: Display: action terrain/loaded"
TIMEOUT_SECONDS="${FORBOC_RUNTIME_SMOKE_TIMEOUT_SECONDS:-120}"
UNREAL_EDITOR=""

if [ ! -f "$PROJECT_FILE" ]; then
  echo "Error: ForbocAIDemo.uproject not found at $PROJECT_FILE"
  exit 1
fi

if [[ "$OSTYPE" == "msys"* ]] || [[ "$OSTYPE" == "cygwin"* ]] || [[ "$OSTYPE" == "win32"* ]]; then
  UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.7}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
elif grep -qi microsoft /proc/version 2>/dev/null; then
  UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.7}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
  PROJECT_FILE_ARG="$(wslpath -w "$PROJECT_FILE")"
else
  UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.7}"
  UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
fi

if [ ! -f "$UNREAL_EDITOR" ]; then
  echo "Error: UnrealEditor-Cmd not found at $UNREAL_EDITOR"
  echo "Please set UE_ROOT to your Unreal Engine 5.7 installation directory."
  exit 1
fi

mkdir -p "$PROJECT_ROOT/Saved/Automation"
rm -f "$LOG_FILE"

echo "=== ForbocAI Runtime Smoke ==="
echo "Project: $PROJECT_FILE"
echo "Map:     $MAP_PATH"
echo "Editor:  $UNREAL_EDITOR"
echo "Log:     $LOG_FILE"

SECONDS=0
(
  "$UNREAL_EDITOR" "$PROJECT_FILE_ARG" "$MAP_PATH" \
    -game \
    -ExecCmds="Quit" \
    -log -stdout -FullStdOutLogOutput \
    -unattended -nop4 -nosplash -nullrhi -nosound -NoLiveCoding \
    2>&1 | tee "$LOG_FILE"
) &
SMOKE_PID=$!
SMOKE_EXIT=0

while kill -0 "$SMOKE_PID" 2>/dev/null; do
  if [ "$SECONDS" -ge "$TIMEOUT_SECONDS" ]; then
    kill "$SMOKE_PID" 2>/dev/null || true
    wait "$SMOKE_PID" 2>/dev/null || true
    echo "✗ Runtime smoke timed out after $TIMEOUT_SECONDS seconds."
    exit 1
  fi
  sleep 2
done

wait "$SMOKE_PID" || SMOKE_EXIT=$?

if [ "$SMOKE_EXIT" -ne 0 ]; then
  echo "✗ Runtime smoke failed with exit code: $SMOKE_EXIT"
  exit 1
fi

if ! grep -Fq "$MAP_MARKER" "$LOG_FILE"; then
  echo "✗ Runtime smoke did not observe Runtime map load."
  echo "  Missing marker: $MAP_MARKER"
  exit 1
fi

if ! grep -Fq "$TERRAIN_MARKER" "$LOG_FILE"; then
  echo "✗ Runtime smoke did not observe runtime terrain dispatch."
  echo "  Missing marker: $TERRAIN_MARKER"
  exit 1
fi

if grep -Fq "Fatal error" "$LOG_FILE"; then
  echo "✗ Runtime smoke log contains a fatal error."
  exit 1
fi

echo "✓ Runtime smoke loaded $MAP_PATH in game mode."
