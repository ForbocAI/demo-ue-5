#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
RUNNER_PATH="$SCRIPT_DIR/run-game-mac.sh"

cat > "$RUNNER_PATH" <<'MAC_RUNNER'
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/ForbocAIDemo.uproject"
MAP_PATH="/Game/Map/Maps/Runtime"
UE_ROOT="${UE_ROOT:-/Users/Shared/Epic Games/UE_5.7}"
UNREAL_EDITOR="$UE_ROOT/Engine/Binaries/Mac/UnrealEditor.app/Contents/MacOS/UnrealEditor"

if [[ ! -x "$UNREAL_EDITOR" ]]; then
  echo "UnrealEditor was not found at:"
  echo "  $UNREAL_EDITOR"
  echo
  echo "Set UE_ROOT to your Unreal Engine 5.7 directory, then run this again."
  exit 1
fi

if [[ ! -f "$PROJECT_FILE" ]]; then
  echo "ForbocAIDemo.uproject was not found at:"
  echo "  $PROJECT_FILE"
  exit 1
fi

echo "Launching Frontier of Jefferson runtime map..."
echo "Project:     $PROJECT_FILE"
echo "Map:         $MAP_PATH"
echo "Editor:      $UNREAL_EDITOR"
echo "Rendering:   authored JSON profile applied by game features"
echo

exec "$UNREAL_EDITOR" "$PROJECT_FILE" "$MAP_PATH" -game -log -NoLiveCoding "$@"
MAC_RUNNER

chmod +x "$RUNNER_PATH"
echo "Created $RUNNER_PATH"

