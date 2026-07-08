#!/usr/bin/env bash
set -euo pipefail

SCRIPT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
POWERSHELL_SCRIPT="$SCRIPT_ROOT/capture-scale-audit.ps1"
POWERSHELL_SCRIPT_ARG="$POWERSHELL_SCRIPT"

if grep -qi microsoft /proc/version 2>/dev/null; then
  POWERSHELL_SCRIPT_ARG="$(wslpath -w "$POWERSHELL_SCRIPT")"
fi

powershell.exe -NoProfile -NonInteractive -InputFormat None -ExecutionPolicy Bypass -File "$POWERSHELL_SCRIPT_ARG" "$@" < /dev/null
