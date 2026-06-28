#!/usr/bin/env bash
# Locks or unlocks the demo's SDK submodule working tree.

set -euo pipefail

MODE="${1:---lock}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SDK_SUBMODULE="Plugins/ForbocAI_SDK"
SDK_PATH="$PROJECT_ROOT/$SDK_SUBMODULE"
SDK_HOOKS_PATH="$PROJECT_ROOT/.githooks/sdk-submodule"
SDK_POLICY="Plugins/ForbocAI_SDK is read-only in the demo. Edit ../sdk-ue-5.7, commit there, then run bash Scripts/update-sdk.sh here."

git_submodule_env() {
  env -u GIT_DIR -u GIT_WORK_TREE -u GIT_INDEX_FILE git "$@"
}

is_wsl_windows() {
  grep -qi microsoft /proc/version 2>/dev/null
}

is_windows_acl_available() {
  is_wsl_windows && command -v powershell.exe >/dev/null 2>&1
}

windows_acl_env() {
  local Existing="${WSLENV:-}"
  local LockPathSpec="FORBOC_SDK_LOCK_PATH/p"
  [ -n "$Existing" ] && printf '%s:%s' "$Existing" "$LockPathSpec" ||
    printf '%s' "$LockPathSpec"
}

run_windows_acl_command() {
  local Command="$1"
  is_windows_acl_available || return 0

  FORBOC_SDK_LOCK_PATH="$SDK_PATH" WSLENV="$(windows_acl_env)" \
    powershell.exe -NoProfile -Command "$Command"
}

remove_windows_write_deny() {
  run_windows_acl_command '& {
    $Path = $env:FORBOC_SDK_LOCK_PATH
    if ([string]::IsNullOrWhiteSpace($Path)) {
      Write-Error "FORBOC_SDK_LOCK_PATH was not provided."
      exit 2
    }
    $Identity = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name
    $Dirs = @($Path) + @(Get-ChildItem -LiteralPath $Path -Directory -Recurse -Force | ForEach-Object { $_.FullName })
    foreach ($Dir in $Dirs) {
      & icacls.exe $Dir /remove:d "$Identity" /C | Out-Null
      $Code = $LASTEXITCODE
      if (($Code -ne 0) -and ($Code -ne 2)) { exit $Code }
    }
    exit 0
  }'
}

apply_windows_write_deny() {
  run_windows_acl_command '& {
    $Path = $env:FORBOC_SDK_LOCK_PATH
    if ([string]::IsNullOrWhiteSpace($Path)) {
      Write-Error "FORBOC_SDK_LOCK_PATH was not provided."
      exit 2
    }
    $Identity = [System.Security.Principal.WindowsIdentity]::GetCurrent().Name
    $Rule = "${Identity}:(WD,AD,DC)"
    $Dirs = @($Path) + @(Get-ChildItem -LiteralPath $Path -Directory -Recurse -Force | ForEach-Object { $_.FullName })
    foreach ($Dir in $Dirs) {
      & icacls.exe $Dir /deny $Rule /C | Out-Null
      if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    }
    exit 0
  }'
}

case "$MODE" in
  --lock|--unlock)
    ;;
  --help|-h)
    echo "Usage: Scripts/lock_sdk_submodule.sh [--lock|--unlock]"
    echo "$SDK_POLICY"
    exit 0
    ;;
  *)
    echo "Error: Expected --lock or --unlock."
    echo "$SDK_POLICY"
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

git_submodule_env -C "$SDK_PATH" config core.hooksPath "$SDK_HOOKS_PATH"

if [ "$MODE" = "--unlock" ]; then
  remove_windows_write_deny
  find "$SDK_PATH" -path "$SDK_PATH/.git" -prune -o -type d -exec chmod u+w {} +
  find "$SDK_PATH" -path "$SDK_PATH/.git" -prune -o -type f -exec chmod u+w {} +
  echo "SDK submodule unlocked for bash Scripts/update-sdk.sh only."
  exit 0
fi

bash "$PROJECT_ROOT/Scripts/check_sdk_submodule_guard.sh" --allow-gitlink-change
find "$SDK_PATH" -path "$SDK_PATH/.git" -prune -o -type f -exec chmod a-w {} +
find "$SDK_PATH" -path "$SDK_PATH/.git" -prune -o -type d -exec chmod a-w {} +
apply_windows_write_deny
echo "SDK submodule locked read-only."
echo "$SDK_POLICY"
