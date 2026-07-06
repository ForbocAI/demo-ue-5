"""Import Classic Horse source assets into UE content.

Run through UnrealEditor-Cmd from the project root:

  UnrealEditor-Cmd.exe ForbocAIDemo.uproject -run=pythonscript -script=Scripts/Tools/import_classic_horse_assets.py

The extracted source package stays in `pre-game-assets`; selected import sources
are copied to temporary, normalized filenames under `Intermediate`.
"""

from __future__ import annotations

import shutil
from dataclasses import dataclass
from pathlib import Path

import unreal


PROJECT_ROOT = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))
SOURCE_ROOT = (
    PROJECT_ROOT
    / "pre-game-assets"
    / "uploads_files_4102381_Classic_Horse"
)
STAGE_DIR = PROJECT_ROOT / "Intermediate" / "MapClassicHorseImport"


@dataclass(frozen=True)
class ImportSource:
    source: str
    staged_name: str
    destination: str
    asset_name: str


HORSE_SKELETON = (
    "/Game/Characters/Horses/ClassicHorse/SkeletalMesh/"
    "ClassicHorse_Equipment_A_Skeleton"
)
RIDER_SKELETON = (
    "/Game/Characters/Horses/ClassicHorse/Rider/SkeletalMesh/"
    "Rider_SkeletalMesh_Skeleton"
)
HORSE_ASSETS_ROOT = "/Game/Characters/Horses"
INTERCHANGE_IMPORT_CVARS = [
    "Interchange.FeatureFlags.Import.FBX 0",
    "Interchange.FeatureFlags.Import.FBX.ToLevel 0",
    "Interchange.FeatureFlags.Import.PSD 0",
    "Interchange.FeatureFlags.Import.SyncToBrowser 0",
]


HORSE_MESH_SOURCES = [
    ImportSource(
        "ClassicHorse/1. Horse/Skeletal Mesh/ClassicHorse_Equipment_A.fbx",
        "ClassicHorse_Equipment_A.fbx",
        "/Game/Characters/Horses/ClassicHorse/SkeletalMesh",
        "ClassicHorse_Equipment_A",
    ),
]

HORSE_ANIMATION_SOURCES = [
    ImportSource(
        "ClassicHorse/1. Horse/Animations/InPlace/3. walk.fbx",
        "Horse_Walk.fbx",
        "/Game/Characters/Horses/ClassicHorse/Animations",
        "Horse_Walk",
    ),
    ImportSource(
        "ClassicHorse/1. Horse/Animations/InPlace/1. idle_main.fbx",
        "Horse_Idle.fbx",
        "/Game/Characters/Horses/ClassicHorse/Animations",
        "Horse_Idle",
    ),
]

HORSE_TEXTURE_SOURCES = [
    ImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/brown/"
        "ch_brown_dirty_albedo.psd",
        "ch_brown_dirty_albedo.psd",
        "/Game/Characters/Horses/ClassicHorse/Textures/Horse",
        "ch_brown_dirty_albedo",
    ),
    ImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/brown/"
        "ch_brown_dirty_roughness.psd",
        "ch_brown_dirty_roughness.psd",
        "/Game/Characters/Horses/ClassicHorse/Textures/Horse",
        "ch_brown_dirty_roughness",
    ),
    ImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/"
        "ch_all_skins_dirty_normal.psd",
        "ch_all_skins_dirty_normal.psd",
        "/Game/Characters/Horses/ClassicHorse/Textures/Horse",
        "ch_all_skins_dirty_normal",
    ),
    ImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/equip/"
        "ch_equip_dirty_albedo.psd",
        "ch_equip_dirty_albedo.psd",
        "/Game/Characters/Horses/ClassicHorse/Textures/Horse",
        "ch_equip_dirty_albedo",
    ),
    ImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/equip/"
        "ch_equip_dirty_normal.psd",
        "ch_equip_dirty_normal.psd",
        "/Game/Characters/Horses/ClassicHorse/Textures/Horse",
        "ch_equip_dirty_normal",
    ),
]

RIDER_MESH_SOURCES = [
    ImportSource(
        "ClassicHorse/2. Rider/Skeletal Mesh/Rider_SkeletalMesh.fbx",
        "Rider_SkeletalMesh.fbx",
        "/Game/Characters/Horses/ClassicHorse/Rider/SkeletalMesh",
        "Rider_SkeletalMesh",
    ),
]

RIDER_ANIMATION_SOURCES = [
    ImportSource(
        "ClassicHorse/2. Rider/Animations/InPlace/3. walk.fbx",
        "Rider_Walk.fbx",
        "/Game/Characters/Horses/ClassicHorse/Rider/Animations",
        "Rider_Walk",
    ),
    ImportSource(
        "ClassicHorse/2. Rider/Animations/InPlace/1. idle_main.fbx",
        "Rider_Idle.fbx",
        "/Game/Characters/Horses/ClassicHorse/Rider/Animations",
        "Rider_Idle",
    ),
]

RIDER_TEXTURE_SOURCES = [
    ImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Albedo.psd",
        "Rider_Albedo.psd",
        "/Game/Characters/Horses/ClassicHorse/Rider/Textures",
        "Rider_Albedo",
    ),
    ImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Normal.psd",
        "Rider_Normal.psd",
        "/Game/Characters/Horses/ClassicHorse/Rider/Textures",
        "Rider_Normal",
    ),
    ImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Roughness.psd",
        "Rider_Roughness.psd",
        "/Game/Characters/Horses/ClassicHorse/Rider/Textures",
        "Rider_Roughness",
    ),
]


def log(message: str) -> None:
    unreal.log(f"MapHorseImport: {message}")


def all_sources() -> list[ImportSource]:
    return (
        HORSE_MESH_SOURCES
        + RIDER_MESH_SOURCES
        + HORSE_ANIMATION_SOURCES
        + RIDER_ANIMATION_SOURCES
        + HORSE_TEXTURE_SOURCES
        + RIDER_TEXTURE_SOURCES
    )


def staged_path(source: ImportSource) -> Path:
    return STAGE_DIR / source.staged_name


def imported_asset_path(source: ImportSource) -> str:
    return f"{source.destination}/{source.asset_name}.{source.asset_name}"


def disable_interchange_import_routes() -> None:
    for command in INTERCHANGE_IMPORT_CVARS:
        try:
            unreal.SystemLibrary.execute_console_command(None, command)
        except Exception as exc:
            unreal.log_warning(
                f"MapHorseImport: could not execute '{command}': {exc}"
            )


def extract_sources(sources: list[ImportSource]) -> None:
    if STAGE_DIR.exists():
        shutil.rmtree(STAGE_DIR)
    STAGE_DIR.mkdir(parents=True, exist_ok=True)

    for source in sources:
        source_path = SOURCE_ROOT / source.source
        target = staged_path(source)
        if not source_path.exists():
            unreal.log_error(f"MapHorseImport: missing {source_path}")
            raise FileNotFoundError(source_path)
        shutil.copyfile(source_path, target)
        log(f"Staged {source.staged_name}")


def task_for(source: ImportSource) -> unreal.AssetImportTask:
    task = unreal.AssetImportTask()
    task.filename = str(staged_path(source))
    task.destination_path = source.destination
    task.destination_name = source.asset_name
    task.automated = True
    task.replace_existing = True
    task.replace_existing_settings = True
    task.save = True
    task.async_ = False
    staged_name = source.staged_name.lower()
    if staged_name.endswith(".fbx"):
        task.factory = unreal.FbxFactory()
    elif staged_name.endswith(".psd"):
        task.factory = unreal.TextureFactory()
    return task


def set_optional_property(target: object, name: str, value: object) -> None:
    try:
        target.set_editor_property(name, value)
    except Exception as exc:
        unreal.log_warning(
            f"MapHorseImport: could not set {target}.{name}: {exc}"
        )


def skeletal_mesh_options() -> unreal.FbxImportUI:
    options = unreal.FbxImportUI()
    options.set_editor_property(
        "mesh_type_to_import", unreal.FBXImportType.FBXIT_SKELETAL_MESH
    )
    options.set_editor_property("automated_import_should_detect_type", False)
    options.set_editor_property("import_as_skeletal", True)
    options.set_editor_property("import_mesh", True)
    options.set_editor_property("create_physics_asset", True)
    options.set_editor_property("import_animations", False)
    options.set_editor_property("import_materials", False)
    options.set_editor_property("import_textures", False)

    skeletal_data = options.get_editor_property("skeletal_mesh_import_data")
    if skeletal_data:
        set_optional_property(skeletal_data, "use_t0_as_ref_pose", True)
        set_optional_property(skeletal_data, "import_meshes_in_bone_hierarchy", True)
    return options


def animation_options(skeleton: unreal.Skeleton, asset_name: str) -> unreal.FbxImportUI:
    options = unreal.FbxImportUI()
    options.set_editor_property(
        "mesh_type_to_import", unreal.FBXImportType.FBXIT_ANIMATION
    )
    options.set_editor_property("automated_import_should_detect_type", False)
    options.set_editor_property("import_as_skeletal", True)
    options.set_editor_property("import_mesh", False)
    options.set_editor_property("skeleton", skeleton)
    options.set_editor_property("import_animations", True)
    options.set_editor_property("override_animation_name", asset_name)
    options.set_editor_property("import_materials", False)
    options.set_editor_property("import_textures", False)

    anim_data = options.get_editor_property("anim_sequence_import_data")
    if anim_data:
        set_optional_property(anim_data, "import_meshes_in_bone_hierarchy", True)
        set_optional_property(anim_data, "import_bone_tracks", True)
    return options


def run_import_task(source: ImportSource, options: object | None) -> None:
    if unreal.EditorAssetLibrary.does_asset_exist(imported_asset_path(source)):
        log(f"Skipped existing {source.asset_name}")
        return

    task = task_for(source)
    task.options = options
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    if task.imported_object_paths:
        log(f"Imported {source.asset_name}: {', '.join(task.imported_object_paths)}")
    else:
        log(f"Imported {source.asset_name}")


def import_skeletal_mesh_sources(sources: list[ImportSource]) -> None:
    for source in sources:
        run_import_task(source, skeletal_mesh_options())


def load_required_asset(asset_path: str) -> unreal.Object:
    asset = unreal.load_asset(asset_path)
    if not asset:
        unreal.log_error(f"MapHorseImport: required asset missing: {asset_path}")
        raise RuntimeError(f"Missing imported asset: {asset_path}")
    return asset


def import_animation_sources(
    sources: list[ImportSource], skeleton_path: str
) -> None:
    skeleton = load_required_asset(skeleton_path)
    for source in sources:
        run_import_task(source, animation_options(skeleton, source.asset_name))


def import_texture_sources(sources: list[ImportSource]) -> None:
    for source in sources:
        run_import_task(source, None)


def main() -> int:
    if not SOURCE_ROOT.exists():
        unreal.log_error(f"MapHorseImport: missing {SOURCE_ROOT}")
        return 1

    disable_interchange_import_routes()
    extract_sources(all_sources())
    import_skeletal_mesh_sources(HORSE_MESH_SOURCES)
    import_skeletal_mesh_sources(RIDER_MESH_SOURCES)
    import_animation_sources(HORSE_ANIMATION_SOURCES, HORSE_SKELETON)
    import_animation_sources(RIDER_ANIMATION_SOURCES, RIDER_SKELETON)
    import_texture_sources(HORSE_TEXTURE_SOURCES + RIDER_TEXTURE_SOURCES)
    unreal.EditorAssetLibrary.save_directory(HORSE_ASSETS_ROOT, True)
    log("Imported Classic Horse, mounted rider, animations, and PSD textures.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
