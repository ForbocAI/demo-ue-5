#!/usr/bin/env bash
# Locks the runtime's SDK submodule working tree. Direct SDK edits are forbidden.

set -euo pipefail

MODE="${1:---lock}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SDK_SUBMODULE="Plugins/ForbocAI_SDK"
SDK_PATH="$PROJECT_ROOT/$SDK_SUBMODULE"
SDK_HOOKS_PATH="$PROJECT_ROOT/.githooks/sdk-submodule"
SDK_ACL_HELPER="$PROJECT_ROOT/Scripts/sdk_submodule_acl.ps1"

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
SDK SUBMODULE OFF LIMITS IN THIS RUNTIME CHECKOUT

Do not edit, chmod, rename, stage, or commit SDK source files under:
  Plugins/ForbocAI_SDK

Unreal may create generated build artifacts only under:
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

Then commit there and update this runtime from here with:
  bash Scripts/update-sdk.sh

This runtime path is a locked submodule consumer, not the SDK source checkout.
========================================================================
POLICY
}

git_submodule_env() {
  env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git "$@"
}

is_wsl_windows() {
  grep -qi microsoft /proc/version 2>/dev/null
}

is_windows_acl_available() {
  is_wsl_windows && command -v powershell.exe >/dev/null 2>&1
}

sdk_lock_windows_path() {
  command -v wslpath >/dev/null 2>&1 || {
    printf '%s\n' "$SDK_PATH"
    return 0
  }
  wslpath -w "$SDK_PATH" 2>/dev/null || printf '%s\n' "$SDK_PATH"
}

sdk_acl_helper_windows_path() {
  command -v wslpath >/dev/null 2>&1 || {
    printf '%s\n' "$SDK_ACL_HELPER"
    return 0
  }
  wslpath -w "$SDK_ACL_HELPER" 2>/dev/null || printf '%s\n' "$SDK_ACL_HELPER"
}

run_windows_acl_helper() {
  local Mode="$1"
  local LockPath
  local HelperPath
  local EscapedHelperPath
  local EscapedLockPath
  is_windows_acl_available || return 0

  LockPath="$(sdk_lock_windows_path)"
  HelperPath="$(sdk_acl_helper_windows_path)"
  EscapedHelperPath="${HelperPath//\"/\\\"}"
  EscapedLockPath="${LockPath//\"/\\\"}"
  powershell.exe -NoProfile -ExecutionPolicy Bypass -Command \
    "\$Helper = (Resolve-Path -LiteralPath \"$EscapedHelperPath\").Path; \$Target = (Resolve-Path -LiteralPath \"$EscapedLockPath\").Path; & \$Helper -Mode \"$Mode\" -Path \$Target"
}

remove_windows_write_deny() {
  run_windows_acl_helper remove
}

apply_windows_write_deny() {
  run_windows_acl_helper apply
}

case "$MODE" in
  --lock|--unlock)
    ;;
  --help|-h)
    echo "Usage: Scripts/lock_sdk_submodule.sh [--lock|--unlock]"
    print_sdk_policy
    exit 0
    ;;
  *)
    echo "Error: Expected --lock or --unlock."
    print_sdk_policy
    exit 2
    ;;
esac

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

if [ "$MODE" = "--unlock" ]; then
  case "${FORBOC_ALLOW_SDK_SUBMODULE_UNLOCK:-}" in
    setup-dev|update-sdk)
      ;;
    *)
      echo "Error: Refusing direct SDK submodule unlock."
      print_sdk_policy
      echo "Approved unlock paths only:"
      echo "  bash Scripts/setup-dev.sh"
      echo "  bash Scripts/update-sdk.sh"
      exit 1
      ;;
  esac
  case "${FORBOC_ENFORCE_SDK_ACL:-0}" in
    1|true|TRUE|yes|YES)
      remove_windows_write_deny
      ;;
  esac
  git_submodule_env -C "$SDK_PATH" config core.hooksPath "$SDK_HOOKS_PATH" || true
  echo "SDK submodule guard unlocked for approved ${FORBOC_ALLOW_SDK_SUBMODULE_UNLOCK} automation only."
  exit 0
fi

bash "$PROJECT_ROOT/Scripts/check_sdk_submodule_guard.sh" --allow-gitlink-change
git_submodule_env -C "$SDK_PATH" config core.hooksPath "$SDK_HOOKS_PATH" || true
case "${FORBOC_ENFORCE_SDK_ACL:-0}" in
  1|true|TRUE|yes|YES)
    apply_windows_write_deny
    echo "SDK submodule source guarded by hooks/status checks and Windows ACLs; Unreal generated output dirs remain writable."
    ;;
  *)
    echo "SDK submodule source guarded by hooks/status checks. Optional ACL helper: Scripts/sdk_submodule_acl.ps1."
    ;;
esac
print_sdk_policy
