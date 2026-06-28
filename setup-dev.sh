#!/usr/bin/env bash
# First-run setup for demo contributors.

set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== ForbocAI UE demo setup ==="
if [ -d "$PROJECT_ROOT/Plugins/ForbocAI_SDK" ]; then
  echo "Unlocking SDK submodule for setup sync..."
  bash "$PROJECT_ROOT/Scripts/lock_sdk_submodule.sh" --unlock || true
fi

echo "Initializing SDK submodule..."
git -C "$PROJECT_ROOT" submodule update --init --recursive

echo "Activating demo git hooks..."
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

cat <<'MSG'

Setup complete.

SDK rule:
  Plugins/ForbocAI_SDK is read-only in this demo checkout.

To change SDK code:
  1. Edit and commit in ../sdk-ue-5.7
  2. Return here and run ./update-sdk.sh

To work on demo code:
  Edit Source/, Content/, Config/, Tools/, or Scripts/ as usual.

MSG
