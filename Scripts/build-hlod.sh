#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
MAP_PATH="${1:-/Game/Map/Maps/Runtime}"

case "$(uname -s)" in
  Darwin)
    UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.8}"
    UNREAL_EDITOR_CMD="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor-Cmd"
    ;;
  MINGW*|MSYS*|CYGWIN*)
    UE_ROOT="${UE_ROOT:-C:/Program Files/Epic Games/UE_5.8}"
    UNREAL_EDITOR_CMD="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    ;;
  *)
    UE_ROOT="${UE_ROOT:-/mnt/c/Program Files/Epic Games/UE_5.8}"
    UNREAL_EDITOR_CMD="$UE_ROOT/Engine/Binaries/Win64/UnrealEditor-Cmd.exe"
    ;;
esac

if [[ ! -f "$PROJECT_FILE" ]]; then
  echo "ForbocAIDemo.uproject was not found at $PROJECT_FILE" >&2
  exit 1
fi

if [[ ! -f "$UNREAL_EDITOR_CMD" ]]; then
  echo "UnrealEditor-Cmd was not found at $UNREAL_EDITOR_CMD. Set UE_ROOT to your Unreal Engine 5.8 directory." >&2
  exit 1
fi

echo "Building native World Partition HLODs for $MAP_PATH"
"$UNREAL_EDITOR_CMD" \
  "$PROJECT_FILE" \
  "$MAP_PATH" \
  -run=WorldPartitionBuilderCommandlet \
  -AllowCommandletRendering \
  -builder=WorldPartitionHLODsBuilder \
  -SetupHLODs \
  -BuildHLODs \
  -unattended \
  -nop4 \
  -NoLiveCoding
