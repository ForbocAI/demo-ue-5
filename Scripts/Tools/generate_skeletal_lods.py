"""Generate UE-native skeletal mesh LODs from authored JSON.

Run with UnrealEditor-Cmd:
  UnrealEditor-Cmd.exe ForbocAIDemo.uproject -run=pythonscript -script=Scripts/Tools/generate_skeletal_lods.py
"""

import json
from pathlib import Path

import unreal


PROJECT_ROOT = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))


def load_data_json(content_relative_path):
    path = PROJECT_ROOT / "Content" / content_relative_path
    with path.open("r", encoding="utf-8") as source:
        return json.load(source)


def load_settings():
    rendering_manifest = load_data_json("Data/settings/rendering.json")
    pipeline_manifest = load_data_json(rendering_manifest["rendering_pipeline"])
    return load_data_json(pipeline_manifest["skeletal_lod_generation"])["skeletal_lod_generation"]


def require_skeletal_mesh(asset_path):
    asset = unreal.EditorAssetLibrary.load_asset(asset_path)
    if not isinstance(asset, unreal.SkeletalMesh):
        raise RuntimeError(f"Expected SkeletalMesh at {asset_path}, found {type(asset).__name__}")
    return asset


def generate_asset_lods(subsystem, settings, asset_settings):
    asset_path = asset_settings["asset_path"]
    mesh = require_skeletal_mesh(asset_path)
    lod_count = int(asset_settings["lod_count"])
    regenerate_even_if_imported = bool(settings["regenerate_even_if_imported"])
    generate_base_lod = bool(settings["generate_base_lod"])
    unreal.log(
        "Generating native skeletal LODs id={id} asset={asset} lod_count={lod_count}".format(
            id=asset_settings["id"], asset=asset_path, lod_count=lod_count
        )
    )
    generated = subsystem.regenerate_lod(
        mesh, lod_count, regenerate_even_if_imported, generate_base_lod
    )
    if not generated:
        raise RuntimeError(f"SkeletalMeshEditorSubsystem.regenerate_lod failed for {asset_path}")
    if not unreal.EditorAssetLibrary.save_loaded_asset(mesh):
        raise RuntimeError(f"Failed to save generated skeletal LODs for {asset_path}")
    unreal.log(f"Saved native skeletal LODs for {asset_path}")


def main():
    settings = load_settings()
    subsystem = unreal.get_editor_subsystem(unreal.SkeletalMeshEditorSubsystem)
    if subsystem is None:
        raise RuntimeError("SkeletalMeshEditorSubsystem is unavailable")
    for asset_settings in settings["assets"]:
        generate_asset_lods(subsystem, settings, asset_settings)


if __name__ == "__main__":
    main()
