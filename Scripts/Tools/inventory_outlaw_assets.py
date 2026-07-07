"""Inventory the staged KitBash3D Outlaw source package.

Run from the project root:

  python3 Scripts/Tools/inventory_outlaw_assets.py

The generated files are source-asset review evidence only. Runtime import stays
separate so the playable map can use selected, optimized assets instead of the
whole native kit.
"""

from __future__ import annotations

import json
import re
import struct
import zipfile
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[2]
ASSET_ROOT = PROJECT_ROOT / "pre-game-assets"
MODEL_ZIP = ASSET_ROOT / "kb3d_outlaw.fbxobj.native.zip"
TEXTURE_ZIP = ASSET_ROOT / "kb3d_outlaw.png.2k.zip"
OBJ_NAME = "kb3d_outlaw-native.obj"
MTL_NAME = "kb3d_outlaw-native.mtl"
JSON_OUTPUT = ASSET_ROOT / "kb3d_outlaw_inventory.json"
MARKDOWN_OUTPUT = ASSET_ROOT / "kb3d_outlaw_inventory.md"


@dataclass
class ObjectInventory:
    name: str
    vertex_lines: int = 0
    face_lines: int = 0
    materials: set[str] = field(default_factory=set)
    min_x: float | None = None
    min_y: float | None = None
    min_z: float | None = None
    max_x: float | None = None
    max_y: float | None = None
    max_z: float | None = None


def base_group(name: str) -> str:
    stripped = re.sub(r"_[A-Z]_[^_].*$", "", name)
    return re.sub(r"_[A-Z]$", "", stripped)


def extend_bounds(item: ObjectInventory, point: tuple[float, float, float]) -> None:
    x, y, z = point
    item.min_x = x if item.min_x is None else min(item.min_x, x)
    item.min_y = y if item.min_y is None else min(item.min_y, y)
    item.min_z = z if item.min_z is None else min(item.min_z, z)
    item.max_x = x if item.max_x is None else max(item.max_x, x)
    item.max_y = y if item.max_y is None else max(item.max_y, y)
    item.max_z = z if item.max_z is None else max(item.max_z, z)


def parse_obj_inventory() -> list[ObjectInventory]:
    objects: list[ObjectInventory] = []
    current: ObjectInventory | None = None
    active_material = ""
    with zipfile.ZipFile(MODEL_ZIP) as package:
        with package.open(OBJ_NAME) as handle:
            for raw_line in handle:
                line = raw_line.decode("utf-8", "ignore").strip()
                if line.startswith("o "):
                    current = ObjectInventory(line[2:].strip())
                    objects.append(current)
                elif line.startswith("usemtl "):
                    active_material = line.split(maxsplit=1)[1]
                    current and current.materials.add(active_material)
                elif line.startswith("v "):
                    parts = line.split()
                    point = (float(parts[1]), float(parts[2]), float(parts[3]))
                    current and setattr(
                        current, "vertex_lines", current.vertex_lines + 1
                    )
                    current and extend_bounds(current, point)
                elif line.startswith("f "):
                    current and setattr(current, "face_lines", current.face_lines + 1)
                    current and active_material and current.materials.add(active_material)
    return objects


def parse_material_textures() -> dict[str, list[str]]:
    materials: dict[str, list[str]] = defaultdict(list)
    current = ""
    with zipfile.ZipFile(MODEL_ZIP) as package:
        with package.open(MTL_NAME) as handle:
            for raw_line in handle:
                line = raw_line.decode("utf-8", "ignore").strip()
                if line.startswith("newmtl "):
                    current = line.split(maxsplit=1)[1]
                elif current and (line.startswith("map_") or line.startswith("refl ")):
                    materials[current].append(line.split(maxsplit=1)[1].replace("\\", "/"))
    return {key: sorted(set(value)) for key, value in sorted(materials.items())}


def png_header(package: zipfile.ZipFile, name: str) -> dict[str, object]:
    with package.open(name) as handle:
        header = handle.read(33)
    width, height = struct.unpack(">II", header[16:24])
    return {
        "width": width,
        "height": height,
        "bit_depth": header[24],
        "color_type": header[25],
    }


def texture_inventory() -> dict[str, object]:
    suffix_counts: Counter[str] = Counter()
    dimension_counts: Counter[str] = Counter()
    texture_files: list[dict[str, object]] = []
    with zipfile.ZipFile(TEXTURE_ZIP) as package:
        for info in package.infolist():
            if not info.filename.lower().endswith(".png"):
                continue
            header = png_header(package, info.filename)
            suffix = info.filename.rsplit("_", 1)[-1]
            suffix_counts[suffix] += 1
            dimension_counts[f"{header['width']}x{header['height']}"] += 1
            texture_files.append(
                {
                    "name": Path(info.filename).name,
                    "suffix": suffix,
                    "compressed_bytes": info.compress_size,
                    "uncompressed_bytes": info.file_size,
                    **header,
                }
            )
    return {
        "count": len(texture_files),
        "suffix_counts": dict(sorted(suffix_counts.items())),
        "dimension_counts": dict(sorted(dimension_counts.items())),
        "files": texture_files,
    }


def object_record(item: ObjectInventory) -> dict[str, object]:
    return {
        "name": item.name,
        "group": base_group(item.name),
        "vertex_lines": item.vertex_lines,
        "face_lines": item.face_lines,
        "materials": sorted(item.materials),
        "bounds": {
            "min": [item.min_x, item.min_y, item.min_z],
            "max": [item.max_x, item.max_y, item.max_z],
        },
    }


def write_markdown(payload: dict[str, object]) -> None:
    objects = payload["objects"]
    groups = payload["groups"]
    textures = payload["textures"]
    largest = sorted(objects, key=lambda item: item["face_lines"], reverse=True)[:30]
    candidates = [
        item
        for item in objects
        if re.search(
            r"General|Store|Farm|Church|Shoe|Apothecary|Saloon|"
            r"Hotel|Office|Wagon|Bridge|Track|Mine|Sheriff|Jail|"
            r"Bank|Stable|WaterTower|Barrel|Crate",
            item["name"],
            re.IGNORECASE,
        )
    ][:80]
    lines = [
        "# KitBash3D Outlaw Asset Inventory",
        "",
        "Generated from `kb3d_outlaw.fbxobj.native.zip` and "
        "`kb3d_outlaw.png.2k.zip`.",
        "",
        "## Summary",
        "",
        f"- Objects: {len(objects)}",
        f"- Groups: {len(groups)}",
        f"- Materials: {len(payload['materials'])}",
        f"- Texture PNGs: {textures['count']}",
        f"- Total OBJ vertices: {payload['totals']['vertices']}",
        f"- Total OBJ faces: {payload['totals']['faces']}",
        "",
        "## Texture Map Counts",
        "",
    ]
    lines.extend(
        f"- `{name}`: {count}" for name, count in textures["suffix_counts"].items()
    )
    lines.extend(["", "## Largest Objects", "", "| Object | Faces | Materials |", "|---|---:|---|"])
    lines.extend(
        f"| `{item['name']}` | {item['face_lines']} | "
        f"{', '.join(item['materials'][:3])} |"
        for item in largest
    )
    lines.extend(
        [
            "",
            "## French Gulch Candidate Objects",
            "",
            "| Object | Faces | Reason |",
            "|---|---:|---|",
        ]
    )
    lines.extend(
        f"| `{item['name']}` | {item['face_lines']} | town dressing candidate |"
        for item in candidates
    )
    MARKDOWN_OUTPUT.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    objects = parse_obj_inventory()
    materials = parse_material_textures()
    records = [object_record(item) for item in objects]
    group_counts = Counter(item["group"] for item in records)
    payload = {
        "source": {
            "model_zip": MODEL_ZIP.name,
            "texture_zip": TEXTURE_ZIP.name,
            "obj": OBJ_NAME,
            "mtl": MTL_NAME,
        },
        "totals": {
            "vertices": sum(item["vertex_lines"] for item in records),
            "faces": sum(item["face_lines"] for item in records),
        },
        "groups": dict(sorted(group_counts.items())),
        "materials": materials,
        "textures": texture_inventory(),
        "objects": records,
    }
    JSON_OUTPUT.write_text(json.dumps(payload, indent=2), encoding="utf-8")
    write_markdown(payload)
    print(f"Wrote {JSON_OUTPUT}")
    print(f"Wrote {MARKDOWN_OUTPUT}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
