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
generated_metadata = policy.get("generated_metadata")
target = policy.get("target")
platform = policy.get("platform")
configuration = policy.get("configuration")
engine_directory_name = policy.get("engine_directory_name")
if isinstance(max_parallel_actions, bool) or not isinstance(max_parallel_actions, int):
    raise SystemExit("build_runner.max_parallel_actions must be an integer")
if max_parallel_actions < 1:
    raise SystemExit("build_runner.max_parallel_actions must be positive")
if not isinstance(disable_uba, bool):
    raise SystemExit("build_runner.disable_uba must be a boolean")
if not isinstance(generated_metadata, list) or not generated_metadata:
    raise SystemExit("build_runner.generated_metadata must be a non-empty array")
if any(not isinstance(path, str) or not path for path in generated_metadata):
    raise SystemExit("build_runner.generated_metadata entries must be strings")
for key, value in (
    ("target", target),
    ("platform", platform),
    ("configuration", configuration),
    ("engine_directory_name", engine_directory_name),
):
    if not isinstance(value, str) or not value:
        raise SystemExit(f"build_runner.{key} must be a non-empty string")

print(max_parallel_actions)
print(1 if disable_uba else 0)
print(target)
print(platform)
print(configuration)
print(engine_directory_name)
print("\n".join(generated_metadata))
PY
)" || return 1
  mapfile -t FORBOCAI_BUILD_RUNNER_POLICY <<<"$policy_values"
  FORBOCAI_BUILD_MAX_PARALLEL_ACTIONS="${FORBOCAI_BUILD_RUNNER_POLICY[0]}"
  FORBOCAI_BUILD_DISABLE_UBA="${FORBOCAI_BUILD_RUNNER_POLICY[1]}"
  FORBOCAI_BUILD_TARGET="${FORBOCAI_BUILD_RUNNER_POLICY[2]}"
  FORBOCAI_BUILD_PLATFORM="${FORBOCAI_BUILD_RUNNER_POLICY[3]}"
  FORBOCAI_BUILD_CONFIGURATION="${FORBOCAI_BUILD_RUNNER_POLICY[4]}"
  FORBOCAI_ENGINE_DIRECTORY_NAME="${FORBOCAI_BUILD_RUNNER_POLICY[5]}"
  FORBOCAI_BUILD_GENERATED_METADATA=("${FORBOCAI_BUILD_RUNNER_POLICY[@]:6}")
}

forbocai_load_map_smoke_policy() {
  local data_root="$1"
  local policy_directory="$data_root/settings/core/automation/runner/map-smoke"
  local config_file="$policy_directory/config.json"
  local automation_failure_file="$policy_directory/automation/failure-markers.json"
  local runtime_failure_file="$policy_directory/runtime/failure-markers.json"
  local runtime_required_file="$policy_directory/runtime/required-markers.json"
  local automation_manifest="$data_root/settings/core/automation.json"
  mapfile -d '' -t FORBOCAI_MAP_SMOKE_POLICY < <(
    python3 - "$config_file" "$automation_failure_file" \
      "$runtime_failure_file" "$runtime_required_file" \
      "$automation_manifest" <<'PY'
import json
import sys
from pathlib import Path

config_path = Path(sys.argv[1])
automation_failure_path = Path(sys.argv[2])
runtime_failure_path = Path(sys.argv[3])
runtime_required_path = Path(sys.argv[4])
manifest_path = Path(sys.argv[5])
with config_path.open(encoding="utf-8") as source:
    policy = json.load(source)
with automation_failure_path.open(encoding="utf-8") as source:
    automation_failure_markers = json.load(source)
with runtime_failure_path.open(encoding="utf-8") as source:
    failure_markers = json.load(source)
with runtime_required_path.open(encoding="utf-8") as source:
    required_markers = json.load(source)
with manifest_path.open(encoding="utf-8") as source:
    manifest = json.load(source)
content_path = manifest_path.parent / manifest["parts"]["content_assets"]
with content_path.open(encoding="utf-8") as source:
    content = json.load(source)

scalar_keys = (
    "timeout_seconds",
    "test_environment_requirement",
    "automation_test",
    "automation_log_path",
    "automation_success_marker",
    "runtime_log_path",
)
values = [policy.get(key) for key in scalar_keys]
timeout, requirement, automation_test, automation_log, automation_success, runtime_log = values
runtime_map = content.get("runtime_map", {})
map_path = runtime_map.get("package_path")
run_runtime_budget = policy.get("run_runtime_budget")

if isinstance(timeout, bool) or not isinstance(timeout, int) or timeout < 1:
    raise SystemExit("map_smoke.timeout_seconds must be a positive integer")
if requirement not in ("optional", "required"):
    raise SystemExit("map_smoke.test_environment_requirement must be optional or required")
for key, value in zip(scalar_keys[2:], values[2:]):
    if not isinstance(value, str) or not value:
        raise SystemExit(f"map_smoke.{key} must be a non-empty string")
for key, markers in (
    ("automation_failure_markers", automation_failure_markers),
    ("runtime_required_markers", required_markers),
    ("runtime_failure_markers", failure_markers),
):
    if not isinstance(markers, list) or not markers:
        raise SystemExit(f"map_smoke.{key} must be a non-empty array")
    if any(not isinstance(marker, str) or not marker for marker in markers):
        raise SystemExit(f"map_smoke.{key} entries must be non-empty strings")
if not isinstance(map_path, str) or not map_path:
    raise SystemExit("content_assets.runtime_map.package_path must be a non-empty string")
if not isinstance(run_runtime_budget, bool):
    raise SystemExit("map_smoke.run_runtime_budget must be a boolean")

output = [
    str(timeout),
    requirement,
    automation_test,
    automation_log,
    runtime_log,
    map_path,
    "1" if run_runtime_budget else "0",
]
sys.stdout.buffer.write("\0".join(output).encode("utf-8") + b"\0")
PY
  ) || return 1

  FORBOCAI_MAP_SMOKE_POLICY_DIRECTORY="$policy_directory"
  FORBOCAI_MAP_SMOKE_TIMEOUT_SECONDS="${FORBOCAI_MAP_SMOKE_POLICY[0]}"
  FORBOCAI_MAP_SMOKE_ENVIRONMENT_REQUIREMENT="${FORBOCAI_MAP_SMOKE_POLICY[1]}"
  FORBOCAI_MAP_SMOKE_AUTOMATION_TEST="${FORBOCAI_MAP_SMOKE_POLICY[2]}"
  FORBOCAI_MAP_SMOKE_AUTOMATION_LOG_PATH="${FORBOCAI_MAP_SMOKE_POLICY[3]}"
  FORBOCAI_MAP_SMOKE_RUNTIME_LOG_PATH="${FORBOCAI_MAP_SMOKE_POLICY[4]}"
  FORBOCAI_MAP_SMOKE_MAP_PATH="${FORBOCAI_MAP_SMOKE_POLICY[5]}"
  FORBOCAI_MAP_SMOKE_RUN_RUNTIME_BUDGET="${FORBOCAI_MAP_SMOKE_POLICY[6]}"
}

forbocai_remove_invalid_unreal_metadata() {
  local project_root="$1"
  shift
  python3 - "$project_root" "$@" <<'PY'
import json
import sys
from pathlib import Path

root = Path(sys.argv[1]).resolve()
metadata = tuple((root / relative).resolve() for relative in sys.argv[2:])

for path in metadata:
    try:
        path.relative_to(root)
    except ValueError as error:
        raise SystemExit(f"Generated metadata path escapes project root: {path}") from error

def is_valid_json(path):
    try:
        with path.open(encoding="utf-8") as source:
            json.load(source)
        return True
    except (OSError, UnicodeError, json.JSONDecodeError):
        return False

invalid = tuple(path for path in metadata if path.is_file() and not is_valid_json(path))
if invalid:
    for path in metadata:
        if path.is_file():
            path.unlink()
    print(
        "Removed malformed generated Unreal metadata: "
        + ", ".join(str(path.relative_to(root)) for path in invalid)
    )
PY
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
