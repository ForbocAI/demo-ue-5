#!/usr/bin/env python3
"""Validate real Runtime map performance samples against an authored budget."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
import re
import statistics
import sys


SAMPLE_PATTERN = re.compile(
    r"runtime-budget sample fps=(?P<fps>-?\d+) "
    r"stack_depth=(?P<stack_depth>-?\d+) "
    r"poly_count=(?P<poly_count>-?\d+) "
    r"memory_mib=(?P<memory_mib>-?\d+) "
    r"peak_memory_mib=(?P<peak_memory_mib>-?\d+) "
    r"virtual_memory_mib=(?P<virtual_memory_mib>-?\d+) "
    r"game_ms=(?P<game_ms>-?\d+(?:\.\d+)?) "
    r"render_ms=(?P<render_ms>-?\d+(?:\.\d+)?) "
    r"rhi_ms=(?P<rhi_ms>-?\d+(?:\.\d+)?) "
    r"gpu_ms=(?P<gpu_ms>-?\d+(?:\.\d+)?) "
    r"draw_calls=(?P<draw_calls>-?\d+) "
    r"rhi_primitives=(?P<rhi_primitives>-?\d+) "
    r"wall_ms=(?P<wall_ms>-?\d+(?:\.\d+)?) "
    r"input_delta_ms=(?P<input_delta_ms>-?\d+(?:\.\d+)?) "
    r"stats_select_ms=(?P<stats_select_ms>-?\d+(?:\.\d+)?) "
    r"poly_count_ms=(?P<poly_count_ms>-?\d+(?:\.\d+)?) "
    r"engine_idle_ms=(?P<engine_idle_ms>-?\d+(?:\.\d+)?) "
    r"engine_idle_overshoot_ms=(?P<engine_idle_overshoot_ms>-?\d+(?:\.\d+)?) "
    r"max_fps=(?P<max_fps>-?\d+(?:\.\d+)?) "
    r"frame_rate_limit=(?P<frame_rate_limit>-?\d+(?:\.\d+)?) "
    r"effective_max_tick_rate=(?P<effective_max_tick_rate>-?\d+(?:\.\d+)?) "
    r"fixed_frame_rate_enabled=(?P<fixed_frame_rate_enabled>-?\d+) "
    r"fixed_frame_rate=(?P<fixed_frame_rate>-?\d+(?:\.\d+)?) "
    r"fixed_time_step_enabled=(?P<fixed_time_step_enabled>-?\d+) "
    r"fixed_delta_ms=(?P<fixed_delta_ms>-?\d+(?:\.\d+)?) "
    r"vsync=(?P<vsync>-?\d+) "
    r"idle_when_not_foreground=(?P<idle_when_not_foreground>-?\d+) "
    r"app_has_focus=(?P<app_has_focus>-?\d+) "
    r"cpu_throttle=(?P<cpu_throttle>-?\d+) "
    r"all_windows_hidden=(?P<all_windows_hidden>-?\d+) "
    r"root_reducer_ms=(?P<root_reducer_ms>-?\d+(?:\.\d+)?) "
    r"combined_reducer_ms=(?P<combined_reducer_ms>-?\d+(?:\.\d+)?) "
    r"ecs_projection_ms=(?P<ecs_projection_ms>-?\d+(?:\.\d+)?) "
    r"ecs_entities=(?P<ecs_entities>-?\d+) "
    r"ecs_component_types=(?P<ecs_component_types>-?\d+)"
)
def load_json(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def runtime_budget(path: Path) -> dict:
    return load_json(path)["runtime_budget"]


def rendering_profile(path: Path) -> dict:
    return load_json(path)["rendering_profile"]


def limit_field_name(limit_name: str) -> str:
    return limit_name.removeprefix("max_").removeprefix("min_")


def profile_limit_failures(profile: dict, limits: dict) -> list[str]:
    failures: list[str] = []
    for limit_name, limit_value in limits.items():
        field_name = limit_field_name(limit_name)
        profile_value = profile[field_name]
        if limit_name.startswith("max_") and profile_value > limit_value:
            failures.append(f"{field_name}={profile_value} exceeds {limit_value}")
        if limit_name.startswith("min_") and profile_value < limit_value:
            failures.append(f"{field_name}={profile_value} is below {limit_value}")
    return failures


def sample_from_line(line: str) -> dict | None:
    match = SAMPLE_PATTERN.search(line)
    if match is None:
        return None
    return {
        "fps": int(match.group("fps")),
        "stack_depth": int(match.group("stack_depth")),
        "poly_count": int(match.group("poly_count")),
        "memory_mib": int(match.group("memory_mib")),
        "peak_memory_mib": int(match.group("peak_memory_mib")),
        "virtual_memory_mib": int(match.group("virtual_memory_mib")),
        "game_ms": float(match.group("game_ms")),
        "render_ms": float(match.group("render_ms")),
        "rhi_ms": float(match.group("rhi_ms")),
        "gpu_ms": float(match.group("gpu_ms")),
        "draw_calls": int(match.group("draw_calls")),
        "rhi_primitives": int(match.group("rhi_primitives")),
        "wall_ms": float(match.group("wall_ms")),
        "input_delta_ms": float(match.group("input_delta_ms")),
        "stats_select_ms": float(match.group("stats_select_ms")),
        "poly_count_ms": float(match.group("poly_count_ms")),
        "engine_idle_ms": float(match.group("engine_idle_ms")),
        "engine_idle_overshoot_ms": float(match.group("engine_idle_overshoot_ms")),
        "max_fps": float(match.group("max_fps")),
        "frame_rate_limit": float(match.group("frame_rate_limit")),
        "effective_max_tick_rate": float(match.group("effective_max_tick_rate")),
        "fixed_frame_rate_enabled": int(match.group("fixed_frame_rate_enabled")),
        "fixed_frame_rate": float(match.group("fixed_frame_rate")),
        "fixed_time_step_enabled": int(match.group("fixed_time_step_enabled")),
        "fixed_delta_ms": float(match.group("fixed_delta_ms")),
        "vsync": int(match.group("vsync")),
        "idle_when_not_foreground": int(match.group("idle_when_not_foreground")),
        "app_has_focus": int(match.group("app_has_focus")),
        "cpu_throttle": int(match.group("cpu_throttle")),
        "all_windows_hidden": int(match.group("all_windows_hidden")),
        "root_reducer_ms": float(match.group("root_reducer_ms")),
        "combined_reducer_ms": float(match.group("combined_reducer_ms")),
        "ecs_projection_ms": float(match.group("ecs_projection_ms")),
        "ecs_entities": int(match.group("ecs_entities")),
        "ecs_component_types": int(match.group("ecs_component_types")),
    }


def runtime_samples(path: Path) -> list[dict]:
    return [
        sample
        for sample in (sample_from_line(line) for line in path.read_text(encoding="utf-8", errors="ignore").splitlines())
        if sample is not None and sample["fps"] > 0
    ]


def measured_failures(budget: dict, samples: list[dict]) -> list[str]:
    if len(samples) < budget["minimum_sample_count"]:
        return [f"only {len(samples)} valid runtime samples captured"]

    fps_values = [sample["fps"] for sample in samples]
    stack_values = [sample["stack_depth"] for sample in samples]
    poly_values = [sample["poly_count"] for sample in samples]
    memory_values = [sample["memory_mib"] for sample in samples]
    peak_memory_values = [sample["peak_memory_mib"] for sample in samples]
    virtual_memory_values = [sample["virtual_memory_mib"] for sample in samples]
    average_fps = statistics.fmean(fps_values)
    lowest_fps = min(fps_values)
    highest_stack_depth = max(stack_values)
    highest_poly_count = max(poly_values)
    highest_memory_mib = max(memory_values)
    highest_peak_memory_mib = max(peak_memory_values)
    highest_virtual_memory_mib = max(virtual_memory_values)

    failures: list[str] = []
    if average_fps < budget["minimum_average_fps"]:
        failures.append(
            f"average_fps={average_fps:.2f} is below {budget['minimum_average_fps']}"
        )
    if lowest_fps < budget["minimum_lowest_fps"]:
        failures.append(
            f"lowest_fps={lowest_fps} is below {budget['minimum_lowest_fps']}"
        )
    if highest_stack_depth > budget["maximum_stack_depth"]:
        failures.append(
            f"stack_depth={highest_stack_depth} exceeds {budget['maximum_stack_depth']}"
        )
    if highest_poly_count > budget["maximum_poly_count"]:
        failures.append(
            f"poly_count={highest_poly_count} exceeds {budget['maximum_poly_count']}"
        )
    if highest_memory_mib > budget["maximum_memory_mib"]:
        failures.append(
            f"memory_mib={highest_memory_mib} exceeds {budget['maximum_memory_mib']}"
        )
    if highest_peak_memory_mib > budget["maximum_peak_memory_mib"]:
        failures.append(
            f"peak_memory_mib={highest_peak_memory_mib} exceeds {budget['maximum_peak_memory_mib']}"
        )
    if highest_virtual_memory_mib > budget["maximum_virtual_memory_mib"]:
        failures.append(
            f"virtual_memory_mib={highest_virtual_memory_mib} exceeds {budget['maximum_virtual_memory_mib']}"
        )
    return failures


def print_summary(budget: dict, samples: list[dict]) -> None:
    if not samples:
        print(f"Runtime budget '{budget['id']}': no valid samples captured")
        return

    fps_values = [sample["fps"] for sample in samples]
    print(
        "Runtime budget '{}': samples={} average_fps={:.2f} lowest_fps={} "
        "max_stack_depth={} max_poly_count={} max_memory_mib={} "
        "max_peak_memory_mib={} max_virtual_memory_mib={} "
        "max_game_ms={:.2f} max_render_ms={:.2f} max_rhi_ms={:.2f} "
        "max_gpu_ms={:.2f} max_draw_calls={} max_rhi_primitives={} "
        "max_wall_ms={:.2f} max_input_delta_ms={:.2f} "
        "max_stats_select_ms={:.2f} max_poly_count_ms={:.2f} "
        "max_engine_idle_ms={:.2f} max_engine_idle_overshoot_ms={:.2f} "
        "max_fps_cap={:.2f} max_frame_rate_limit={:.2f} "
        "max_effective_tick_rate={:.2f} max_fixed_frame_rate_enabled={} "
        "max_fixed_frame_rate={:.2f} max_fixed_time_step_enabled={} "
        "max_fixed_delta_ms={:.2f} max_vsync={} "
        "max_idle_when_not_foreground={} min_app_has_focus={} "
        "max_cpu_throttle={} max_all_windows_hidden={} "
        "max_root_reducer_ms={:.2f} max_combined_reducer_ms={:.2f} "
        "max_ecs_projection_ms={:.2f} max_ecs_entities={} "
        "max_ecs_component_types={}".format(
            budget["id"],
            len(samples),
            statistics.fmean(fps_values),
            min(fps_values),
            max(sample["stack_depth"] for sample in samples),
            max(sample["poly_count"] for sample in samples),
            max(sample["memory_mib"] for sample in samples),
            max(sample["peak_memory_mib"] for sample in samples),
            max(sample["virtual_memory_mib"] for sample in samples),
            max(sample["game_ms"] for sample in samples),
            max(sample["render_ms"] for sample in samples),
            max(sample["rhi_ms"] for sample in samples),
            max(sample["gpu_ms"] for sample in samples),
            max(sample["draw_calls"] for sample in samples),
            max(sample["rhi_primitives"] for sample in samples),
            max(sample["wall_ms"] for sample in samples),
            max(sample["input_delta_ms"] for sample in samples),
            max(sample["stats_select_ms"] for sample in samples),
            max(sample["poly_count_ms"] for sample in samples),
            max(sample["engine_idle_ms"] for sample in samples),
            max(sample["engine_idle_overshoot_ms"] for sample in samples),
            max(sample["max_fps"] for sample in samples),
            max(sample["frame_rate_limit"] for sample in samples),
            max(sample["effective_max_tick_rate"] for sample in samples),
            max(sample["fixed_frame_rate_enabled"] for sample in samples),
            max(sample["fixed_frame_rate"] for sample in samples),
            max(sample["fixed_time_step_enabled"] for sample in samples),
            max(sample["fixed_delta_ms"] for sample in samples),
            max(sample["vsync"] for sample in samples),
            max(sample["idle_when_not_foreground"] for sample in samples),
            min(sample["app_has_focus"] for sample in samples),
            max(sample["cpu_throttle"] for sample in samples),
            max(sample["all_windows_hidden"] for sample in samples),
            max(sample["root_reducer_ms"] for sample in samples),
            max(sample["combined_reducer_ms"] for sample in samples),
            max(sample["ecs_projection_ms"] for sample in samples),
            max(sample["ecs_entities"] for sample in samples),
            max(sample["ecs_component_types"] for sample in samples),
        )
    )


def print_failure_guidance(budget: dict) -> None:
    for line in budget["failure_guidance"]:
        print(line)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--budget", required=True, type=Path)
    parser.add_argument("--profile", required=True, type=Path)
    parser.add_argument("--log", required=True, type=Path)
    args = parser.parse_args()

    budget = runtime_budget(args.budget)
    profile = rendering_profile(args.profile)
    samples = runtime_samples(args.log)
    failures = profile_limit_failures(profile, budget["profile_limits"])
    failures.extend(measured_failures(budget, samples))

    print_summary(budget, samples)
    if failures:
        print("Runtime budget check failed:")
        for failure in failures:
            print(f"  - {failure}")
        print_failure_guidance(budget)
        return 1

    print("Runtime budget check passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
