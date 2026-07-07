"""Import selected KitBash3D Outlaw assets into UE content.

Run through UnrealEditor-Cmd from the project root:

  UnrealEditor-Cmd.exe ForbocAIDemo.uproject -run=pythonscript -script=Scripts/Tools/import_outlaw_assets.py

The native kit is too large for direct Runtime use. This script splits selected
OBJ object groups into standalone staging meshes, writes per-group material files,
rebuilds the expected texture folder layout, and imports only the curated pieces
needed for French Gulch.
"""

from __future__ import annotations

import re
import shutil
import zipfile
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

import unreal


PROJECT_ROOT = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))
ASSET_ROOT = PROJECT_ROOT / "pre-game-assets"
MODEL_ZIP = ASSET_ROOT / "kb3d_outlaw.fbxobj.native.zip"
TEXTURE_ZIP = ASSET_ROOT / "kb3d_outlaw.png.2k.zip"
OBJ_NAME = "kb3d_outlaw-native.obj"
MTL_NAME = "kb3d_outlaw-native.mtl"
STAGE_DIR = PROJECT_ROOT / "Intermediate" / "MapOutlawImport"
OUTLAW_ASSETS_ROOT = "/Game/Environment/KitBash/Outlaw"


SELECTED_GROUPS = [
    "KB3D_OUT_BldgSMPostOffice",
    "KB3D_OUT_BldgMDGeneralStore",
    "KB3D_OUT_BldgMDSaloon",
    "KB3D_OUT_BldgMDChurch",
    "KB3D_OUT_BldgSMApothecary",
    "KB3D_OUT_BldgSMShoeStore",
    "KB3D_OUT_BldgSMMiningSupplies",
    "KB3D_OUT_BldgMDSherifStation",
    "KB3D_OUT_BldgMDBlacksmith",
    "KB3D_OUT_BldgSMMineEntrance",
    "KB3D_OUT_PropWagon",
    "KB3D_OUT_PropTrainCart",
    "KB3D_OUT_PropTrainTracks",
    "KB3D_OUT_PropBarrel",
    "KB3D_OUT_PropWoodCrate",
    "KB3D_OUT_PropLanternPost",
    "KB3D_OUT_PropWantedPoster",
    "KB3D_OUT_StrctrBridge",
]


INTERCHANGE_IMPORT_CVARS = [
    "Interchange.FeatureFlags.Import.FBX 0",
    "Interchange.FeatureFlags.Import.FBX.ToLevel 0",
    "Interchange.FeatureFlags.Import.PSD 0",
    "Interchange.FeatureFlags.Import.SyncToBrowser 0",
]


@dataclass
class FaceEvent:
    kind: str
    value: str


@dataclass
class GroupSelection:
    group: str
    events: list[FaceEvent] = field(default_factory=list)
    materials: set[str] = field(default_factory=set)
    used_vertices: set[int] = field(default_factory=set)
    used_uvs: set[int] = field(default_factory=set)
    used_normals: set[int] = field(default_factory=set)


def log(message: str) -> None:
    unreal.log(f"MapOutlawImport: {message}")


def base_group(name: str) -> str:
    stripped = re.sub(r"_[A-Z]_[^_].*$", "", name)
    return re.sub(r"_[A-Z]$", "", stripped)


def sanitize_asset_name(name: str) -> str:
    cleaned = re.sub(r"[^A-Za-z0-9_]+", "_", name)
    return cleaned.strip("_")


def parse_index(value: str) -> int | None:
    return int(value) if value else None


def collect_face_indices(
    target: GroupSelection, face: str
) -> None:
    for token in face.split()[1:]:
        pieces = token.split("/")
        vertex = parse_index(pieces[0]) if len(pieces) >= 1 else None
        uv = parse_index(pieces[1]) if len(pieces) >= 2 else None
        normal = parse_index(pieces[2]) if len(pieces) >= 3 else None
        vertex and target.used_vertices.add(vertex)
        uv and target.used_uvs.add(uv)
        normal and target.used_normals.add(normal)


def collect_group_selections() -> dict[str, GroupSelection]:
    selected = {name: GroupSelection(name) for name in SELECTED_GROUPS}
    current: GroupSelection | None = None
    active_material = ""
    with zipfile.ZipFile(MODEL_ZIP) as package:
        with package.open(OBJ_NAME) as handle:
            for raw_line in handle:
                line = raw_line.decode("utf-8", "ignore").strip()
                if line.startswith("o "):
                    group = base_group(line[2:].strip())
                    current = selected.get(group)
                    current and current.events.append(FaceEvent("object", line))
                elif current and line.startswith("usemtl "):
                    active_material = line.split(maxsplit=1)[1]
                    current.materials.add(active_material)
                    current.events.append(FaceEvent("material", line))
                elif current and line.startswith("f "):
                    active_material and current.materials.add(active_material)
                    collect_face_indices(current, line)
                    current.events.append(FaceEvent("face", line))
    return {key: value for key, value in selected.items() if value.events}


def union_indices(
    selections: dict[str, GroupSelection], attr: str
) -> set[int]:
    output: set[int] = set()
    for selection in selections.values():
        output.update(getattr(selection, attr))
    return output


def collect_index_lines(
    vertex_indices: set[int],
    uv_indices: set[int],
    normal_indices: set[int],
) -> tuple[dict[int, str], dict[int, str], dict[int, str]]:
    vertices: dict[int, str] = {}
    uvs: dict[int, str] = {}
    normals: dict[int, str] = {}
    vertex_cursor = 0
    uv_cursor = 0
    normal_cursor = 0
    with zipfile.ZipFile(MODEL_ZIP) as package:
        with package.open(OBJ_NAME) as handle:
            for raw_line in handle:
                line = raw_line.decode("utf-8", "ignore").strip()
                if line.startswith("v "):
                    vertex_cursor += 1
                    vertex_cursor in vertex_indices and vertices.__setitem__(
                        vertex_cursor, line
                    )
                elif line.startswith("vt "):
                    uv_cursor += 1
                    uv_cursor in uv_indices and uvs.__setitem__(uv_cursor, line)
                elif line.startswith("vn "):
                    normal_cursor += 1
                    normal_cursor in normal_indices and normals.__setitem__(
                        normal_cursor, line
                    )
    return vertices, uvs, normals


def material_blocks() -> dict[str, list[str]]:
    blocks: dict[str, list[str]] = {}
    current = ""
    with zipfile.ZipFile(MODEL_ZIP) as package:
        with package.open(MTL_NAME) as handle:
            for raw_line in handle:
                line = raw_line.decode("utf-8", "ignore").rstrip()
                if line.startswith("newmtl "):
                    current = line.split(maxsplit=1)[1]
                    blocks[current] = [line]
                elif current:
                    blocks[current].append(line)
    return blocks


def material_texture_names(blocks: dict[str, list[str]], names: set[str]) -> set[str]:
    textures: set[str] = set()
    for name in names:
        for line in blocks.get(name, []):
            if line.startswith("map_"):
                path = line.split(maxsplit=1)[1].replace("\\", "/")
                textures.add(Path(path).name)
    return textures


def staged_mtl_name(group: str) -> str:
    return f"{group}.mtl"


def write_selected_mtl(
    output_path: Path, blocks: dict[str, list[str]], names: set[str]
) -> None:
    lines: list[str] = []
    for name in sorted(names):
        lines.extend(line for line in blocks.get(name, []) if not line.startswith("refl "))
        lines.append("")
    output_path.write_text("\n".join(lines), encoding="utf-8")


def remap_face_token(
    token: str,
    vertex_map: dict[int, int],
    uv_map: dict[int, int],
    normal_map: dict[int, int],
) -> str:
    pieces = token.split("/")
    vertex = parse_index(pieces[0]) if len(pieces) >= 1 else None
    uv = parse_index(pieces[1]) if len(pieces) >= 2 else None
    normal = parse_index(pieces[2]) if len(pieces) >= 3 else None
    remapped_vertex = str(vertex_map[vertex]) if vertex else ""
    remapped_uv = str(uv_map[uv]) if uv else ""
    remapped_normal = str(normal_map[normal]) if normal else ""
    return "/".join([remapped_vertex, remapped_uv, remapped_normal])


def remap_face(
    line: str,
    vertex_map: dict[int, int],
    uv_map: dict[int, int],
    normal_map: dict[int, int],
) -> str:
    tokens = line.split()
    return " ".join(
        [tokens[0]]
        + [remap_face_token(token, vertex_map, uv_map, normal_map) for token in tokens[1:]]
    )


def write_group_obj(
    selection: GroupSelection,
    vertices: dict[int, str],
    uvs: dict[int, str],
    normals: dict[int, str],
) -> Path:
    output_path = STAGE_DIR / f"{selection.group}.obj"
    ordered_vertices = sorted(selection.used_vertices)
    ordered_uvs = sorted(selection.used_uvs)
    ordered_normals = sorted(selection.used_normals)
    vertex_map = {value: index + 1 for index, value in enumerate(ordered_vertices)}
    uv_map = {value: index + 1 for index, value in enumerate(ordered_uvs)}
    normal_map = {value: index + 1 for index, value in enumerate(ordered_normals)}
    lines = [f"mtllib {staged_mtl_name(selection.group)}"]
    lines.extend(vertices[index] for index in ordered_vertices)
    lines.extend(uvs[index] for index in ordered_uvs)
    lines.extend(normals[index] for index in ordered_normals)
    for event in selection.events:
        if event.kind == "face":
            lines.append(remap_face(event.value, vertex_map, uv_map, normal_map))
        else:
            lines.append(event.value)
    output_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return output_path


def extract_textures(texture_names: set[str]) -> None:
    texture_target = STAGE_DIR / "KB3DTextures" / "4k"
    texture_target.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(TEXTURE_ZIP) as package:
        archive_by_name = {Path(info.filename).name: info for info in package.infolist()}
        for texture_name in sorted(texture_names):
            info = archive_by_name.get(texture_name)
            if info:
                with package.open(info) as source:
                    (texture_target / texture_name).write_bytes(source.read())
            else:
                unreal.log_warning(f"MapOutlawImport: missing texture {texture_name}")


def disable_interchange_import_routes() -> None:
    for command in INTERCHANGE_IMPORT_CVARS:
        try:
            unreal.SystemLibrary.execute_console_command(None, command)
        except Exception as exc:
            unreal.log_warning(
                f"MapOutlawImport: could not execute '{command}': {exc}"
            )


def prepare_stage() -> dict[str, Path]:
    if STAGE_DIR.exists():
        shutil.rmtree(STAGE_DIR)
    STAGE_DIR.mkdir(parents=True, exist_ok=True)
    selections = collect_group_selections()
    vertex_indices = union_indices(selections, "used_vertices")
    uv_indices = union_indices(selections, "used_uvs")
    normal_indices = union_indices(selections, "used_normals")
    vertices, uvs, normals = collect_index_lines(vertex_indices, uv_indices, normal_indices)
    blocks = material_blocks()
    texture_names: set[str] = set()
    for selection in selections.values():
        write_selected_mtl(
            STAGE_DIR / staged_mtl_name(selection.group), blocks, selection.materials
        )
        texture_names.update(material_texture_names(blocks, selection.materials))
    extract_textures(texture_names)
    return {
        key: write_group_obj(selection, vertices, uvs, normals)
        for key, selection in sorted(selections.items())
    }


def import_task(source_path: Path, asset_name: str) -> unreal.AssetImportTask:
    task = unreal.AssetImportTask()
    task.filename = str(source_path)
    task.destination_path = OUTLAW_ASSETS_ROOT
    task.destination_name = asset_name
    task.automated = True
    task.replace_existing = True
    task.replace_existing_settings = True
    task.save = True
    task.async_ = False
    return task


def run_imports(staged_objects: dict[str, Path]) -> None:
    tasks = [
        import_task(path, sanitize_asset_name(group.replace("KB3D_OUT_", "")))
        for group, path in staged_objects.items()
    ]
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task in tasks:
        imported = ", ".join(task.imported_object_paths)
        log(f"Imported {task.destination_name}: {imported}")


def main() -> int:
    if not MODEL_ZIP.exists() or not TEXTURE_ZIP.exists():
        unreal.log_error("MapOutlawImport: missing KitBash3D Outlaw source zips")
        return 1
    disable_interchange_import_routes()
    staged_objects = prepare_stage()
    run_imports(staged_objects)
    unreal.EditorAssetLibrary.save_directory(OUTLAW_ASSETS_ROOT, True)
    log(f"Imported {len(staged_objects)} selected Outlaw groups.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
