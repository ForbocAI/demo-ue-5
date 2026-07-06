#!/usr/bin/env bash
# First-run setup for runtime contributors.

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

sdk_source_path() {
  if [ -n "${FORBOC_SDK_SOURCE_PATH:-}" ]; then
    printf '%s\n' "$FORBOC_SDK_SOURCE_PATH"
  else
    printf '%s\n' "$(cd "$PROJECT_ROOT/.." && pwd)/sdk-ue-5.7"
  fi
}

sdk_source_windows_path() {
  local SourcePath
  SourcePath="$(sdk_source_path)"
  command -v wslpath >/dev/null 2>&1 || return 0
  wslpath -w "$SourcePath" 2>/dev/null || true
}

echo "=== ForbocAI UE runtime setup ==="
if [ -d "$PROJECT_ROOT/Plugins/ForbocAI_SDK" ]; then
  echo "Unlocking SDK submodule for setup sync..."
  FORBOC_ALLOW_SDK_SUBMODULE_UNLOCK=setup-dev \
    bash "$PROJECT_ROOT/Scripts/lock_sdk_submodule.sh" --unlock || true
fi

echo "Initializing SDK submodule..."
git -C "$PROJECT_ROOT" submodule update --init --recursive

echo "Activating runtime git hooks..."
git -C "$PROJECT_ROOT" config core.hooksPath .githooks

echo "Making guard scripts executable..."
chmod +x "$PROJECT_ROOT/Scripts/check_sdk_submodule_guard.sh"
chmod +x "$PROJECT_ROOT/Scripts/lock_sdk_submodule.sh"
chmod +x "$PROJECT_ROOT/.githooks/pre-commit"
chmod +x "$PROJECT_ROOT/.githooks/post-checkout"
chmod +x "$PROJECT_ROOT/.githooks/post-merge"
chmod +x "$PROJECT_ROOT/.githooks/post-rewrite"
chmod +x "$PROJECT_ROOT/.githooks/sdk-submodule/pre-commit"

echo "Locking SDK submodule read-only..."
bash "$PROJECT_ROOT/Scripts/lock_sdk_submodule.sh" --lock

SDK_SOURCE_PATH="$(sdk_source_path)"
SDK_SOURCE_WINDOWS_PATH="$(sdk_source_windows_path)"

cat <<MSG

Setup complete.

SDK rule:
  Plugins/ForbocAI_SDK is OFF LIMITS and read-only in this runtime checkout.

To change SDK code:
  1. Edit and commit in $SDK_SOURCE_PATH
MSG
if [ -n "$SDK_SOURCE_WINDOWS_PATH" ] && [ "$SDK_SOURCE_WINDOWS_PATH" != "$SDK_SOURCE_PATH" ]; then
  printf '     Windows path: %s\n' "$SDK_SOURCE_WINDOWS_PATH"
fi
cat <<'MSG'
  2. Return here and run bash Scripts/update-sdk.sh

For a nonstandard layout, set FORBOC_SDK_SOURCE_PATH to the real SDK source checkout.

To work on runtime code:
  Edit Source/, Content/, Config/, or Scripts/ as usual.

MSG
