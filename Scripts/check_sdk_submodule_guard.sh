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
SDK_POLICY="Plugins/ForbocAI_SDK is read-only in the demo. Edit ../sdk-ue-5.7, commit there, then run ./update-sdk.sh here."

git_submodule_env() {
  env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git "$@"
}

if [ ! -d "$SDK_PATH" ]; then
  echo "Error: SDK submodule directory is missing: $SDK_SUBMODULE"
  echo "Run: git submodule update --init --recursive"
  echo "First-time setup: ./setup-dev.sh"
  exit 1
fi

if [ ! -d "$SDK_PATH/.git" ] && [ ! -f "$SDK_PATH/.git" ]; then
  echo "Error: $SDK_SUBMODULE is not an initialized git submodule."
  echo "Run: git submodule update --init --recursive"
  echo "First-time setup: ./setup-dev.sh"
  exit 1
fi

SDK_STATUS="$(
  git_submodule_env -C "$SDK_PATH" status --porcelain=v1 --untracked-files=all
)"
if [ -n "$SDK_STATUS" ]; then
  echo "Error: Direct edits inside $SDK_SUBMODULE are not allowed."
  echo "$SDK_POLICY"
  echo "$SDK_STATUS"
  exit 1
fi

if [ "$ALLOW_GITLINK_CHANGE" -eq 0 ]; then
  if ! git -C "$PROJECT_ROOT" diff --quiet -- "$SDK_SUBMODULE"; then
    echo "Error: The SDK submodule pointer changed outside ./update-sdk.sh."
    echo "$SDK_POLICY"
    exit 1
  fi

  if ! git -C "$PROJECT_ROOT" diff --cached --quiet -- "$SDK_SUBMODULE"; then
    echo "Error: Staged SDK submodule pointer changes must be created by ./update-sdk.sh."
    echo "$SDK_POLICY"
    exit 1
  fi
fi

echo "SDK submodule guard passed."
