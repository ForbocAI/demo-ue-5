#!/usr/bin/env bash
# update-sdk.sh
# Fetches the latest ForbocAI SDK submodule from origin/main, updates the local pointer,
# and creates a commit with the short SHA and subject in the message to ensure traceability.

set -euo pipefail

SUBMODULE_DIR="Plugins/ForbocAI_SDK"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
GUARD_SCRIPT="$PROJECT_ROOT/Scripts/Checks/check_sdk_submodule_guard.sh"
LOCK_SCRIPT="$PROJECT_ROOT/Scripts/SDK/lock_sdk_submodule.sh"
SUBMODULE_PATH="$PROJECT_ROOT/$SUBMODULE_DIR"

if [ ! -d "$SUBMODULE_PATH/.git" ] && [ ! -f "$SUBMODULE_PATH/.git" ]; then
  echo "Error: Submodule directory $SUBMODULE_DIR not initialized."
  echo "Run: git submodule update --init --recursive"
  exit 1
fi

echo "Checking SDK submodule immutability guard..."
bash "$GUARD_SCRIPT" --allow-gitlink-change

echo "Unlocking SDK submodule for the predefined update flow..."
FORBOC_ALLOW_SDK_SUBMODULE_UNLOCK=update-sdk bash "$LOCK_SCRIPT" --unlock
trap 'bash "$LOCK_SCRIPT" --lock' EXIT

echo "Fetching latest SDK submodule from origin/main..."
cd "$SUBMODULE_PATH"
git fetch origin main
git checkout main
git pull --rebase origin main

SHORT_SHA=$(git rev-parse --short HEAD)
SUBJECT=$(git log -1 --format="%s")
cd - > /dev/null

if git -C "$PROJECT_ROOT" diff --quiet -- "$SUBMODULE_DIR" &&
   git -C "$PROJECT_ROOT" diff --cached --quiet -- "$SUBMODULE_DIR"; then
  echo "Submodule is already at the latest commit ($SHORT_SHA)."
  exit 0
fi

echo "Staging submodule bump..."
git -C "$PROJECT_ROOT" add "$SUBMODULE_DIR"

COMMIT_MSG="chore: bump SDK submodule to $SHORT_SHA

Submodule bumped to $SHORT_SHA.
Latest SDK commit: $SUBJECT"

echo "Creating commit..."
FORBOC_ALLOW_SDK_SUBMODULE_BUMP=1 git -C "$PROJECT_ROOT" commit -m "$COMMIT_MSG"

echo "Success! Submodule bumped to $SHORT_SHA."
