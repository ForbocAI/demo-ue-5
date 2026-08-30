#!/usr/bin/env bash

forbocai_find_data_json_by_key() {
  local data_root="$1"
  local key="$2"
  python3 - "$data_root" "$key" <<'PY'
import json
import sys
from pathlib import Path

root = Path(sys.argv[1])
key = sys.argv[2]
matches = []
for path in sorted(root.rglob("*.json")):
    with path.open(encoding="utf-8") as source:
        document = json.load(source)
    if isinstance(document, dict) and isinstance(document.get(key), dict):
        matches.append(path)
if len(matches) != 1:
    discovered = ", ".join(str(path) for path in matches)
    raise SystemExit(
        f"Expected exactly one data JSON document with top-level key {key!r}; "
        f"found {len(matches)}: {discovered}"
    )
print(matches[0])
PY
}

forbocai_load_build_runner_policy() {
  local data_root="$1"
  local policy_file
  local policy_values
  policy_file="$(forbocai_find_data_json_by_key "$data_root" build_runner)" ||
    return 1
  policy_values="$(python3 - "$policy_file" <<'PY'
import json
import sys

with open(sys.argv[1], encoding="utf-8") as source:
    policy = json.load(source)["build_runner"]

max_parallel_actions = policy.get("max_parallel_actions")
disable_uba = policy.get("disable_uba")
if isinstance(max_parallel_actions, bool) or not isinstance(max_parallel_actions, int):
    raise SystemExit("build_runner.max_parallel_actions must be an integer")
if max_parallel_actions < 1:
    raise SystemExit("build_runner.max_parallel_actions must be positive")
if not isinstance(disable_uba, bool):
    raise SystemExit("build_runner.disable_uba must be a boolean")

print(max_parallel_actions)
print(1 if disable_uba else 0)
PY
)" || return 1
  mapfile -t FORBOCAI_BUILD_RUNNER_POLICY <<<"$policy_values"
  FORBOCAI_BUILD_MAX_PARALLEL_ACTIONS="${FORBOCAI_BUILD_RUNNER_POLICY[0]}"
  FORBOCAI_BUILD_DISABLE_UBA="${FORBOCAI_BUILD_RUNNER_POLICY[1]}"
}

forbocai_cleanup_windows_build_host() {
  local build_via_cmd="$1"
  local build_host_pid_file="$2"
  local build_host_pid
  if [ "$build_via_cmd" -ne 1 ] || [ ! -f "$build_host_pid_file" ]; then
    return 0
  fi

  build_host_pid="$(tr -cd '0-9' < "$build_host_pid_file")"
  rm -f "$build_host_pid_file"
  if [[ "$build_host_pid" =~ ^[0-9]+$ ]]; then
    cmd.exe /c "taskkill /PID $build_host_pid /T /F" >/dev/null 2>&1 || true
  fi
}
