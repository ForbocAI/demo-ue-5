#!/usr/bin/env bash
# update-sdk.sh
# Fetches the latest ForbocAI SDK submodule from origin/main, updates the local pointer,
# and creates a commit with the short SHA and subject in the message to ensure traceability.

set -euo pipefail

SUBMODULE_DIR="Plugins/ForbocAI_SDK"

if [ ! -d "$SUBMODULE_DIR/.git" ] && [ ! -f "$SUBMODULE_DIR/.git" ]; then
  echo "Error: Submodule directory $SUBMODULE_DIR not initialized."
  echo "Run: git submodule update --init --recursive"
  exit 1
fi

echo "Fetching latest SDK submodule from origin/main..."
cd "$SUBMODULE_DIR"
git fetch origin main
git checkout main
git pull --rebase origin main

SHORT_SHA=$(git rev-parse --short HEAD)
SUBJECT=$(git log -1 --format="%s")
cd ..

if git diff --quiet "$SUBMODULE_DIR"; then
  echo "Submodule is already at the latest commit ($SHORT_SHA)."
  exit 0
fi

echo "Staging submodule bump..."
git add "$SUBMODULE_DIR"

COMMIT_MSG="chore: bump SDK submodule to $SHORT_SHA

Submodule bumped to $SHORT_SHA.
Latest SDK commit: $SUBJECT"

echo "Creating commit..."
git commit -m "$COMMIT_MSG"

echo "Success! Submodule bumped to $SHORT_SHA."
