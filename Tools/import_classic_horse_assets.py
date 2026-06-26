"""Import the staged Classic Horse source assets into UE content.

Run through UnrealEditor-Cmd from the project root:

  UnrealEditor-Cmd.exe DemoProject.uproject -run=pythonscript -script=Tools/import_classic_horse_assets.py

The source package stays in `pre-game-assets`; extracted import sources are
temporary files under `Intermediate`.
"""

from __future__ import annotations

import shutil
import zipfile
from dataclasses import dataclass
from pathlib import Path

import unreal


PROJECT_ROOT = Path(unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()))
SOURCE_ZIP = PROJECT_ROOT / "pre-game-assets" / "uploads_files_4102381_Classic_Horse.zip"
STAGE_DIR = PROJECT_ROOT / "Intermediate" / "FrenchGulchClassicHorseImport"


@dataclass(frozen=True)
class ZipImportSource:
    source: str
    staged_name: str
    destination: str


HORSE_SOURCES = [
    ZipImportSource(
        "ClassicHorse/1. Horse/Skeletal Mesh/ClassicHorse_Equipment_A.fbx",
        "ClassicHorse_Equipment_A.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/SkeletalMesh",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Animations/InPlace/3. walk.fbx",
        "Horse_Walk.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/Animations",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Animations/InPlace/1. idle_main.fbx",
        "Horse_Idle.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/Animations",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/brown/ch_brown_dirty_albedo.psd",
        "ch_brown_dirty_albedo.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Textures/Horse",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/brown/ch_brown_dirty_roughness.psd",
        "ch_brown_dirty_roughness.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Textures/Horse",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/body/ch_all_skins_dirty_normal.psd",
        "ch_all_skins_dirty_normal.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Textures/Horse",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/equip/ch_equip_dirty_albedo.psd",
        "ch_equip_dirty_albedo.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Textures/Horse",
    ),
    ZipImportSource(
        "ClassicHorse/1. Horse/Textures/dirty_horse/equip/ch_equip_dirty_normal.psd",
        "ch_equip_dirty_normal.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Textures/Horse",
    ),
]

RIDER_SOURCES = [
    ZipImportSource(
        "ClassicHorse/2. Rider/Skeletal Mesh/Rider_SkeletalMesh.fbx",
        "Rider_SkeletalMesh.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/SkeletalMesh",
    ),
    ZipImportSource(
        "ClassicHorse/2. Rider/Animations/InPlace/3. walk.fbx",
        "Rider_Walk.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/Animations",
    ),
    ZipImportSource(
        "ClassicHorse/2. Rider/Animations/InPlace/1. idle_main.fbx",
        "Rider_Idle.fbx",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/Animations",
    ),
    ZipImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Albedo.psd",
        "Rider_Albedo.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/Textures",
    ),
    ZipImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Normal.psd",
        "Rider_Normal.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/Textures",
    ),
    ZipImportSource(
        "ClassicHorse/2. Rider/Textures/Rider_Roughness.psd",
        "Rider_Roughness.psd",
        "/Game/FrenchGulch/Horses/ClassicHorse/Rider/Textures",
    ),
]


def log(message: str) -> None:
    unreal.log(f"FrenchGulchHorseImport: {message}")


def staged_path(source: ZipImportSource) -> Path:
    return STAGE_DIR / source.staged_name


def extract_sources(sources: list[ZipImportSource]) -> None:
    if STAGE_DIR.exists():
        shutil.rmtree(STAGE_DIR)
    STAGE_DIR.mkdir(parents=True, exist_ok=True)

    with zipfile.ZipFile(SOURCE_ZIP) as package:
        for source in sources:
            target = staged_path(source)
            with package.open(source.source) as src, target.open("wb") as dst:
                shutil.copyfileobj(src, dst)
            log(f"Staged {source.staged_name}")


def import_source(source: ZipImportSource) -> None:
    task = unreal.AssetImportTask()
    task.filename = str(staged_path(source))
    task.destination_path = source.destination
    task.automated = True
    task.replace_existing = True
    task.save = True
    task.options = None
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])


def import_sources(sources: list[ZipImportSource]) -> None:
    for source in sources:
        import_source(source)


def main() -> int:
    if not SOURCE_ZIP.exists():
        unreal.log_error(f"FrenchGulchHorseImport: missing {SOURCE_ZIP}")
        return 1

    sources = HORSE_SOURCES + RIDER_SOURCES
    extract_sources(sources)
    import_sources(sources)
    unreal.EditorAssetLibrary.save_directory("/Game/FrenchGulch/Horses", True)
    log("Imported Classic Horse, mounted rider, animations, and PSD textures.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
