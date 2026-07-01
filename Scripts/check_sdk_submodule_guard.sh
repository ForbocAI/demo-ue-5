#!/usr/bin/env bash
# Enforces that the demo consumes ForbocAI_SDK as an immutable submodule.

set -euo pipefail

ALLOW_GITLINK_CHANGE=0

for ARG in "$@"; do
  case "$ARG" in
    --allow-gitlink-change)
      ALLOW_GITLINK_CHANGE=1
      ;;
    *)
      echo "Error: Unknown argument: $ARG"
      exit 2
      ;;
  esac
done

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SDK_SUBMODULE="Plugins/ForbocAI_SDK"
SDK_PATH="$PROJECT_ROOT/$SDK_SUBMODULE"

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

print_sdk_policy() {
  local SourcePath
  local SourceWindowsPath
  SourcePath="$(sdk_source_path)"
  SourceWindowsPath="$(sdk_source_windows_path)"
  cat <<POLICY
========================================================================
SDK SUBMODULE OFF LIMITS IN THIS DEMO CHECKOUT

Do not edit, chmod, rename, stage, or commit SDK source files under:
  Plugins/ForbocAI_SDK

Unreal-generated build artifacts are allowed only under:
  Plugins/ForbocAI_SDK/Binaries
  Plugins/ForbocAI_SDK/Intermediate
  Plugins/ForbocAI_SDK/Saved

Only edit SDK source in the sibling SDK source checkout:
  $SourcePath
POLICY
  if [ -n "$SourceWindowsPath" ] && [ "$SourceWindowsPath" != "$SourcePath" ]; then
    printf '  %s\n' "$SourceWindowsPath"
  fi
  cat <<'POLICY'

For a nonstandard layout, set FORBOC_SDK_SOURCE_PATH to the real SDK source checkout.

Then commit there and update this demo from here with:
  bash Scripts/update-sdk.sh

This demo path is a locked submodule consumer, not the SDK source checkout.
========================================================================
POLICY
}

git_submodule_env() {
  env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git "$@"
}

if [ ! -d "$SDK_PATH" ]; then
  echo "Error: SDK submodule directory is missing: $SDK_SUBMODULE"
  echo "Run: git submodule update --init --recursive"
  echo "First-time setup: bash Scripts/setup-dev.sh"
  exit 1
fi

if [ ! -d "$SDK_PATH/.git" ] && [ ! -f "$SDK_PATH/.git" ]; then
  echo "Error: $SDK_SUBMODULE is not an initialized git submodule."
  echo "Run: git submodule update --init --recursive"
  echo "First-time setup: bash Scripts/setup-dev.sh"
  exit 1
fi

SDK_STATUS="$(
  git_submodule_env -C "$SDK_PATH" status --porcelain=v1 --untracked-files=all
)"
if [ -n "$SDK_STATUS" ]; then
  echo "Error: OFF LIMITS: direct edits inside $SDK_SUBMODULE are not allowed."
  print_sdk_policy
  echo "Dirty SDK submodule status:"
  echo "$SDK_STATUS"
  exit 1
fi

if [ "$ALLOW_GITLINK_CHANGE" -eq 0 ]; then
  if ! git -C "$PROJECT_ROOT" diff --quiet -- "$SDK_SUBMODULE"; then
    echo "Error: The SDK submodule pointer changed outside bash Scripts/update-sdk.sh."
    print_sdk_policy
    exit 1
  fi

  if ! git -C "$PROJECT_ROOT" diff --cached --quiet -- "$SDK_SUBMODULE"; then
    echo "Error: Staged SDK submodule pointer changes must be created by bash Scripts/update-sdk.sh."
    print_sdk_policy
    exit 1
  fi
fi

echo "SDK submodule guard passed."
