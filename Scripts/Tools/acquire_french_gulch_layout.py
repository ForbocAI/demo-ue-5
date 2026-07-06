#!/usr/bin/env python3
"""Acquire OSM layout data for the French Gulch runtime map."""

from __future__ import annotations

import argparse
import json
import math
from pathlib import Path
import re
import urllib.request


ROOT = Path(__file__).resolve().parents[2]
DATA_DIR = ROOT / "Content" / "Data"
SOURCE_DIR = DATA_DIR / "Source"
OVERPASS_PATH = SOURCE_DIR / "french_gulch_osm_overpass.json"
METADATA_PATH = SOURCE_DIR / "french_gulch_layout_metadata.json"
RUNTIME_LEVEL_PATH = DATA_DIR / "french_gulch_runtime_level.json"
LANDMARKS_PATH = DATA_DIR / "french_gulch_landmarks.json"
NATURE_PATH = DATA_DIR / "french_gulch_nature.json"
LEVEL_SETTINGS_PATH = DATA_DIR / "runtime_settings_level.json"

ANCHOR_LAT = 40.7011876
ANCHOR_LON = -122.6382527
QUERY_RADIUS_METERS = 450.0
TERRAIN_EXTENT_METERS = 3200.0
UNREAL_UNITS_PER_METER = 100.0
FEET_PER_METER = 3.280839895
LOT_SIZE_FEET = 10.0
EARTH_RADIUS_METERS = 6378137.0
MIN_BUILDING_DIMENSION_FEET = 8.0
REGULAR_BUILDING_MAX_FRONTAGE_FEET = 72.0
REGULAR_BUILDING_MAX_DEPTH_FEET = 48.0
POST_OFFICE_MAX_FRONTAGE_FEET = 68.0
POST_OFFICE_MAX_DEPTH_FEET = 46.0
NAMED_BUILDING_MAX_FRONTAGE_FEET = 116.0
NAMED_BUILDING_MAX_DEPTH_FEET = 64.0


def snake_id(text: str) -> str:
    value = re.sub(r"[^a-zA-Z0-9]+", "-", text.strip().lower()).strip("-")
    return value or "osm-feature"


def load_json(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def write_json(path: Path, data: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, indent=2) + "\n", encoding="utf-8")


def overpass_query() -> str:
    return f"""[out:json][timeout:60];
(
  nwr(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["amenity"];
  nwr(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["shop"];
  nwr(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["tourism"];
  way(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["building"];
  way(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["highway"];
  way(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["waterway"];
  way(around:{QUERY_RADIUS_METERS},{ANCHOR_LAT},{ANCHOR_LON})["natural"];
);
out body geom;"""


def fetch_overpass() -> dict:
    request = urllib.request.Request(
        "https://overpass-api.de/api/interpreter",
        data=overpass_query().encode("utf-8"),
        headers={"User-Agent": "ForbocAIDemoLayoutAcquisition/1.0"},
    )
    with urllib.request.urlopen(request, timeout=90) as response:
        return json.load(response)


def acquire_overpass(force: bool) -> dict:
    if OVERPASS_PATH.exists() and not force:
        return load_json(OVERPASS_PATH)
    data = fetch_overpass()
    write_json(OVERPASS_PATH, data)
    return data


def local_meters(lat: float, lon: float, origin_lat: float, origin_lon: float) -> tuple[float, float]:
    lat_radians = math.radians(origin_lat)
    east = math.radians(lon - origin_lon) * math.cos(lat_radians) * EARTH_RADIUS_METERS
    north = math.radians(lat - origin_lat) * EARTH_RADIUS_METERS
    return east, north


def lots_from_meters(value: float) -> float:
    return value * FEET_PER_METER / LOT_SIZE_FEET


def feet(value_meters: float) -> float:
    return value_meters * FEET_PER_METER


def clamp_feet(value_meters: float, maximum_feet: float) -> float:
    return min(max(MIN_BUILDING_DIMENSION_FEET, feet(value_meters)), maximum_feet)


def building_dimension_caps(tags: dict) -> tuple[float, float]:
    if tags.get("amenity") == "post_office":
        return POST_OFFICE_MAX_FRONTAGE_FEET, POST_OFFICE_MAX_DEPTH_FEET
    if tags.get("name") in {"E. Franck & Co.", "French Gulch Hotel"}:
        return NAMED_BUILDING_MAX_FRONTAGE_FEET, NAMED_BUILDING_MAX_DEPTH_FEET
    return REGULAR_BUILDING_MAX_FRONTAGE_FEET, REGULAR_BUILDING_MAX_DEPTH_FEET


def way_points(element: dict, origin_lat: float, origin_lon: float) -> list[tuple[float, float]]:
    return [
        local_meters(point["lat"], point["lon"], origin_lat, origin_lon)
        for point in element.get("geometry", [])
    ]


def polygon_centroid(points: list[tuple[float, float]]) -> tuple[float, float]:
    if len(points) < 3:
        return average_point(points)
    area = 0.0
    cx = 0.0
    cy = 0.0
    pairs = zip(points, points[1:] + points[:1])
    for (x0, y0), (x1, y1) in pairs:
        cross = x0 * y1 - x1 * y0
        area += cross
        cx += (x0 + x1) * cross
        cy += (y0 + y1) * cross
    if abs(area) < 0.000001:
        return average_point(points)
    area *= 0.5
    return cx / (6.0 * area), cy / (6.0 * area)


def average_point(points: list[tuple[float, float]]) -> tuple[float, float]:
    if not points:
        return 0.0, 0.0
    return (
        sum(point[0] for point in points) / len(points),
        sum(point[1] for point in points) / len(points),
    )


def rotate(point: tuple[float, float], radians: float) -> tuple[float, float]:
    x, y = point
    c = math.cos(radians)
    s = math.sin(radians)
    return x * c - y * s, x * s + y * c


def oriented_bounds(points: list[tuple[float, float]]) -> tuple[tuple[float, float], float, float, float]:
    unique = points[:-1] if len(points) > 1 and points[0] == points[-1] else points
    if len(unique) < 2:
        return average_point(unique), 0.0, 0.0, 0.0
    edge_angles = [
        math.atan2(y1 - y0, x1 - x0)
        for (x0, y0), (x1, y1) in zip(unique, unique[1:] + unique[:1])
        if math.hypot(x1 - x0, y1 - y0) > 0.25
    ]
    candidates = edge_angles or [0.0]
    best = None
    for angle in candidates:
        rotated = [rotate(point, -angle) for point in unique]
        min_x = min(point[0] for point in rotated)
        max_x = max(point[0] for point in rotated)
        min_y = min(point[1] for point in rotated)
        max_y = max(point[1] for point in rotated)
        width = max_x - min_x
        depth = max_y - min_y
        area = width * depth
        center = rotate(((min_x + max_x) * 0.5, (min_y + max_y) * 0.5), angle)
        candidate = (area, center, angle, width, depth)
        best = candidate if best is None or candidate[0] < best[0] else best
    _, center, angle, width, depth = best
    if depth > width:
        angle += math.pi * 0.5
        width, depth = depth, width
    return center, normalize_yaw(math.degrees(angle)), width, depth


def normalize_yaw(degrees: float) -> float:
    value = ((degrees + 180.0) % 360.0) - 180.0
    return 0.0 if abs(value) < 0.0001 else value


def element_name(element: dict) -> str:
    tags = element.get("tags", {})
    return (
        tags.get("name")
        or tags.get("operator")
        or tags.get("amenity")
        or tags.get("building")
        or f"osm {element['type']} {element['id']}"
    )


def is_named_landmark(element: dict) -> bool:
    tags = element.get("tags", {})
    name = tags.get("name", "")
    return (
        tags.get("amenity") == "post_office"
        or name in {"E. Franck & Co.", "French Gulch Hotel"}
    )


def landmark_label(element: dict) -> str:
    tags = element.get("tags", {})
    if tags.get("amenity") == "post_office":
        return "U.S. Post Office / 14200 Trinity Mountain Rd"
    return element_name(element)


def building_record(element: dict, origin_lat: float, origin_lon: float) -> dict | None:
    points = way_points(element, origin_lat, origin_lon)
    if len(points) < 3:
        return None
    center, yaw, width_m, depth_m = oriented_bounds(points)
    if width_m * depth_m < 10.0:
        return None
    tags = element.get("tags", {})
    max_frontage, max_depth = building_dimension_caps(tags)
    stories = 1.6 if tags.get("name") in {"E. Franck & Co.", "French Gulch Hotel"} else 1.35
    return {
        "id": f"osm-building-{element['id']}",
        "name": element_name(element),
        "label": landmark_label(element),
        "east_lots": lots_from_meters(center[0]),
        "north_lots": lots_from_meters(center[1]),
        "yaw_degrees": yaw,
        "frontage_feet": clamp_feet(width_m, max_frontage),
        "depth_feet": clamp_feet(depth_m, max_depth),
        "stories": stories,
        "distance_m": math.hypot(center[0], center[1]),
        "named_landmark": is_named_landmark(element),
    }


def road_segment_blocks(elements: list[dict], origin_lat: float, origin_lon: float) -> list[dict]:
    blocks: list[dict] = []
    for element in elements:
        tags = element.get("tags", {})
        names = {tags.get("name", ""), tags.get("name_1", "")}
        if "Main Street" not in names and "Trinity Mountain Road" not in names:
            continue
        points = way_points(element, origin_lat, origin_lon)
        for index, (start, end) in enumerate(zip(points, points[1:])):
            dx = end[0] - start[0]
            dy = end[1] - start[1]
            length_m = math.hypot(dx, dy)
            if length_m < 8.0:
                continue
            center = ((start[0] + end[0]) * 0.5, (start[1] + end[1]) * 0.5)
            if math.hypot(center[0], center[1]) > 430.0:
                continue
            heading = math.degrees(math.atan2(dy, dx))
            blocks.append(
                runtime_block(
                    f"osm-road-{element['id']}-{index}",
                    "Trinity Mountain Road / Main Street",
                    center,
                    normalize_yaw(heading - 90.0),
                    {
                        "mode": "long_feature",
                        "width_feet": 24.0,
                        "length_lots": feet(length_m) / LOT_SIZE_FEET,
                        "height_feet": 0.35,
                    },
                    "road_dust",
                )
            )
    return blocks


def creek_segment_blocks(elements: list[dict], origin_lat: float, origin_lon: float) -> list[dict]:
    blocks: list[dict] = []
    for element in elements:
        tags = element.get("tags", {})
        if tags.get("name") != "Clear Creek" and tags.get("waterway") != "stream":
            continue
        points = way_points(element, origin_lat, origin_lon)
        for index, (start, end) in enumerate(zip(points, points[1:])):
            dx = end[0] - start[0]
            dy = end[1] - start[1]
            length_m = math.hypot(dx, dy)
            center = ((start[0] + end[0]) * 0.5, (start[1] + end[1]) * 0.5)
            if length_m < 10.0 or math.hypot(center[0], center[1]) > 500.0:
                continue
            heading = math.degrees(math.atan2(dy, dx))
            blocks.append(
                runtime_block(
                    f"osm-clear-creek-{element['id']}-{index}",
                    "Clear Creek",
                    center,
                    normalize_yaw(heading - 90.0),
                    {
                        "mode": "long_feature",
                        "width_feet": 12.0,
                        "length_lots": feet(length_m) / LOT_SIZE_FEET,
                        "height_feet": 0.4,
                    },
                    "water_creek",
                )
            )
    return blocks[:18]


def runtime_block(
    block_id: str,
    name: str,
    center_meters: tuple[float, float],
    yaw_degrees: float,
    scale: dict,
    texture: str,
) -> dict:
    return {
        "id": block_id,
        "name": name,
        "anchor": "post_office_lots",
        "east_lots": round(lots_from_meters(center_meters[0]), 3),
        "north_lots": round(lots_from_meters(center_meters[1]), 3),
        "yaw_degrees": round(yaw_degrees, 3),
        "scale": round_scale(scale),
        "texture": texture,
    }


def round_scale(scale: dict) -> dict:
    return {
        key: (round(value, 3) if isinstance(value, float) else value)
        for key, value in scale.items()
    }


def landmark_from_building(building: dict) -> dict:
    return {
        "id": "post-office" if building["label"].startswith("U.S. Post Office") else snake_id(building["label"]),
        "label": building["label"],
        "kind": "building",
        "east_lots": round(building["east_lots"], 3),
        "north_lots": round(building["north_lots"], 3),
        "yaw_degrees": round(building["yaw_degrees"], 3),
        "frontage_feet": round(building["frontage_feet"], 3),
        "depth_feet": round(building["depth_feet"], 3),
        "stories": round(building["stories"], 3),
    }


def runtime_building_block(building: dict) -> dict:
    return {
        "id": building["id"],
        "name": building["name"],
        "anchor": "post_office_lots",
        "east_lots": round(building["east_lots"], 3),
        "north_lots": round(building["north_lots"], 3),
        "yaw_degrees": round(building["yaw_degrees"], 3),
        "scale": {
            "mode": "building",
            "frontage_feet": round(building["frontage_feet"], 3),
            "depth_feet": round(building["depth_feet"], 3),
            "stories": round(building["stories"], 3),
        },
        "texture": "building_timber",
    }


def find_origin(elements: list[dict]) -> tuple[float, float, str]:
    for element in elements:
        if element.get("tags", {}).get("amenity") == "post_office":
            geometry = element.get("geometry", [])
            if geometry:
                lat = sum(point["lat"] for point in geometry) / len(geometry)
                lon = sum(point["lon"] for point in geometry) / len(geometry)
                return lat, lon, f"osm:{element['type']}/{element['id']}"
            return element["lat"], element["lon"], f"osm:{element['type']}/{element['id']}"
    return ANCHOR_LAT, ANCHOR_LON, "google-street-view-url-anchor"


def player_spawn_from_road_blocks(blocks: list[dict]) -> dict:
    roads = [block for block in blocks if "Trinity Mountain Road" in block["name"]]
    if not roads:
        return {"east_lots": 0.0, "north_lots": -8.0, "yaw_degrees": 64.0}
    road = min(
        roads,
        key=lambda block: math.hypot(block["east_lots"], block["north_lots"]),
    )
    return {
        "east_lots": road["east_lots"],
        "north_lots": road["north_lots"],
        "yaw_degrees": normalize_yaw(road["yaw_degrees"] + 90.0),
    }


def generated_layout(data: dict) -> tuple[dict, dict, dict, dict]:
    elements = data.get("elements", [])
    origin_lat, origin_lon, origin_source = find_origin(elements)
    building_records = [
        record
        for element in elements
        if element.get("type") == "way" and "building" in element.get("tags", {})
        for record in [building_record(element, origin_lat, origin_lon)]
        if record is not None and record["distance_m"] <= 360.0
    ]
    named = sorted(
        [record for record in building_records if record["named_landmark"]],
        key=lambda record: record["distance_m"],
    )
    unnamed = sorted(
        [record for record in building_records if not record["named_landmark"]],
        key=lambda record: record["distance_m"],
    )[:36]
    terrain_blocks = road_segment_blocks(elements, origin_lat, origin_lon)
    terrain_blocks.extend(creek_segment_blocks(elements, origin_lat, origin_lon))
    player_spawn = player_spawn_from_road_blocks(terrain_blocks)
    layout = {
        "terrain": {"blocks": terrain_blocks, "labels": []},
        "town": {
            "blocks": [runtime_building_block(record) for record in unnamed],
            "labels": [
                {
                    "id": "trinity-mountain-road-label",
                    "text": "Trinity Mountain Rd / Main St",
                    "anchor": "post_office_lots",
                    "height_mode": "explicit",
                    "east_lots": 0.0,
                    "north_lots": -8.0,
                    "height_offset": 650.0,
                    "world_size_scale": 4.2,
                }
            ],
        },
        "mine": {"blocks": [], "labels": []},
        "overlay_labels": [
            {
                "id": "french-gulch-layout-source-label",
                "text": "French Gulch OSM/USGS runtime layout",
                "anchor": "post_office_lots",
                "height_mode": "explicit",
                "east_lots": 0.0,
                "north_lots": 18.0,
                "height_offset": 1200.0,
                "world_size_scale": 5.0,
            }
        ],
    }
    landmarks = {"landmarks": [landmark_from_building(record) for record in named]}
    nature = {"features": []}
    metadata = {
        "generated_by": "Scripts/Tools/acquire_french_gulch_layout.py",
        "source": "OpenStreetMap Overpass API",
        "license": "ODbL-1.0; credit OpenStreetMap contributors",
        "query_radius_meters": QUERY_RADIUS_METERS,
        "anchor": {"lat": origin_lat, "lon": origin_lon, "source": origin_source},
        "terrain_extent_meters": TERRAIN_EXTENT_METERS,
        "unreal_units_per_meter": UNREAL_UNITS_PER_METER,
        "lot_size_feet": LOT_SIZE_FEET,
        "player_spawn_lots": player_spawn,
        "counts": {
            "raw_elements": len(elements),
            "runtime_terrain_blocks": len(layout["terrain"]["blocks"]),
            "runtime_building_blocks": len(layout["town"]["blocks"]),
            "named_landmarks": len(landmarks["landmarks"]),
        },
    }
    return layout, landmarks, nature, metadata


def update_level_settings(metadata: dict) -> None:
    settings = load_json(LEVEL_SETTINGS_PATH)
    geometry = settings["level_geometry"]
    extent_feet = TERRAIN_EXTENT_METERS * FEET_PER_METER
    player_spawn = metadata["player_spawn_lots"]
    geometry.update(
        {
            "terrain_world_size": TERRAIN_EXTENT_METERS * UNREAL_UNITS_PER_METER,
            "terrain_elevation_scale": UNREAL_UNITS_PER_METER,
            "terrain_lots_across": extent_feet / LOT_SIZE_FEET,
            "post_office_east_lots": 0.0,
            "post_office_north_lots": 0.0,
            "cube_mesh_size": 100.0,
            "block_scale_per_foot": 0.3048,
            "height_scale_per_story": 3.048,
            "character_height_ratio": 1.2,
            "label_clearance_ratio": 2.4,
            "actor_foot_to_terrain_ratio": 1.0,
            "actor_reference_feet_across": extent_feet,
            "player_spawn_east_lots": player_spawn["east_lots"],
            "player_spawn_north_lots": player_spawn["north_lots"],
            "player_spawn_extra_height_ratio": 1.4,
            "main_street_facing_yaw_degrees": player_spawn["yaw_degrees"],
            "player_spawn_anchor_label": "U.S. Post Office / 14200 Trinity Mountain Rd",
            "landmark_label_world_size_scale": 4.8,
            "nature_label_world_size_scale": 3.6,
            "terrain_lod_step": 1,
            "block_forced_lod_model": 0,
            "block_cull_distance": TERRAIN_EXTENT_METERS * UNREAL_UNITS_PER_METER,
        }
    )
    write_json(LEVEL_SETTINGS_PATH, settings)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--force-fetch", action="store_true")
    args = parser.parse_args()

    data = acquire_overpass(args.force_fetch)
    layout, landmarks, nature, metadata = generated_layout(data)
    write_json(RUNTIME_LEVEL_PATH, layout)
    write_json(LANDMARKS_PATH, landmarks)
    write_json(NATURE_PATH, nature)
    write_json(METADATA_PATH, metadata)
    update_level_settings(metadata)
    print(
        "Generated French Gulch layout: "
        f"{metadata['counts']['runtime_terrain_blocks']} terrain blocks, "
        f"{metadata['counts']['runtime_building_blocks']} building blocks, "
        f"{metadata['counts']['named_landmarks']} named landmarks"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
