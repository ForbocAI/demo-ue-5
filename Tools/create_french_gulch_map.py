"""Create the project-owned runtime startup map.

Run from the project root through UnrealEditor-Cmd:

  UnrealEditor-Cmd.exe DemoProject.uproject -run=pythonscript -script=Tools/create_french_gulch_map.py

Run Tools/create_demo_blueprint_assets.py first so the map can use the tracked
Blueprint game mode and presentation actors.
"""

import unreal


MAP_PACKAGE = "/Game/Map/Maps/Runtime"
MAP_FOLDER = "/Game/Map/Maps"
GAME_MODE_CLASS = "/Game/Blueprints/BP_LevelGameMode.BP_LevelGameMode_C"
RUNTIME_LEVEL_CLASS = "/Game/Blueprints/BP_RuntimeLevelView.BP_RuntimeLevelView_C"
SPEECH_PRESENTER_CLASS = "/Game/Blueprints/BP_SpeechPresenter.BP_SpeechPresenter_C"


def log(message: str) -> None:
    unreal.log(f"RuntimeMap: {message}")


def load_required_class(path: str):
    loaded_class = unreal.load_class(None, path)
    if loaded_class is None:
        raise RuntimeError(f"Unable to load Unreal class: {path}")
    return loaded_class


def actor_class_path(actor) -> str:
    actor_class = actor.get_class()
    return actor_class.get_path_name() if actor_class else ""


def editor_actor_subsystem():
    return unreal.get_editor_subsystem(unreal.EditorActorSubsystem)


def editor_world():
    return unreal.get_editor_subsystem(
        unreal.UnrealEditorSubsystem
    ).get_editor_world()


def level_editor_subsystem():
    return unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)


def actor_exists(class_path: str) -> bool:
    for actor in editor_actor_subsystem().get_all_level_actors():
        if actor_class_path(actor) == class_path:
            return True
    return False


def spawn_once(class_path: str, label: str, location, rotation):
    actor_class = load_required_class(class_path)
    if actor_exists(class_path):
        log(f"{label} already exists")
        return None
    actor = editor_actor_subsystem().spawn_actor_from_class(
        actor_class, location, rotation
    )
    if actor is None:
        raise RuntimeError(f"Unable to spawn {label}")
    actor.set_actor_label(label)
    return actor


def configure_world_settings() -> None:
    world = editor_world()
    if world is None:
        raise RuntimeError("No editor world is loaded")
    world_settings = world.get_world_settings()
    game_mode_class = load_required_class(GAME_MODE_CLASS)
    world_settings.set_editor_property("default_game_mode", game_mode_class)


def ensure_lighting() -> None:
    spawn_once(
        "/Script/Engine.DirectionalLight",
        "Runtime_DirectionalLight",
        unreal.Vector(-2600.0, -1800.0, 4200.0),
        unreal.Rotator(-42.0, -30.0, 0.0),
    )
    spawn_once(
        "/Script/Engine.SkyAtmosphere",
        "Runtime_SkyAtmosphere",
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    spawn_once(
        "/Script/Engine.SkyLight",
        "Runtime_SkyLight",
        unreal.Vector(0.0, 0.0, 1800.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    spawn_once(
        "/Script/Engine.ExponentialHeightFog",
        "Runtime_HeightFog",
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )


def main() -> int:
    if not unreal.EditorAssetLibrary.does_directory_exist(MAP_FOLDER):
        unreal.EditorAssetLibrary.make_directory(MAP_FOLDER)

    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PACKAGE):
        log(f"Loading {MAP_PACKAGE}")
        level_editor_subsystem().load_level(MAP_PACKAGE)
    else:
        log(f"Creating {MAP_PACKAGE}")
        unreal.EditorLoadingAndSavingUtils.new_blank_map(False)

    configure_world_settings()
    ensure_lighting()
    spawn_once(
        RUNTIME_LEVEL_CLASS,
        "RuntimeLevel",
        unreal.Vector(0.0, 0.0, 0.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    spawn_once(
        SPEECH_PRESENTER_CLASS,
        "RuntimeSpeechPresenter",
        unreal.Vector(260.0, 120.0, 120.0),
        unreal.Rotator(0.0, 180.0, 0.0),
    )

    world = editor_world()
    if not unreal.EditorLoadingAndSavingUtils.save_map(world, MAP_PACKAGE):
        raise RuntimeError(f"Failed to save {MAP_PACKAGE}")

    log(f"Saved {MAP_PACKAGE}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        unreal.log_error(f"RuntimeMap: {exc}")
        raise SystemExit(1)
