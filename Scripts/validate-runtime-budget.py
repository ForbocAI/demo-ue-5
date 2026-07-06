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
    r"poly_count=(?P<poly_count>-?\d+)"
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
    average_fps = statistics.fmean(fps_values)
    lowest_fps = min(fps_values)
    highest_stack_depth = max(stack_values)
    highest_poly_count = max(poly_values)

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
    return failures


def print_summary(budget: dict, samples: list[dict]) -> None:
    if not samples:
        print(f"Runtime budget '{budget['id']}': no valid samples captured")
        return

    fps_values = [sample["fps"] for sample in samples]
    print(
        "Runtime budget '{}': samples={} average_fps={:.2f} lowest_fps={} "
        "max_stack_depth={} max_poly_count={}".format(
            budget["id"],
            len(samples),
            statistics.fmean(fps_values),
            min(fps_values),
            max(sample["stack_depth"] for sample in samples),
            max(sample["poly_count"] for sample in samples),
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
