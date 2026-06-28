"""Create the tracked Blueprint and UMG assets for the runtime demo.

Run from the project root through UnrealEditor-Cmd after building the editor
module:

  UnrealEditor-Cmd.exe DemoProject.uproject -run=pythonscript -script=Tools/create_demo_blueprint_assets.py
"""

import unreal


BLUEPRINT_ROOT = "/Game/Blueprints"
UI_ROOT = "/Game/UI"

LEVEL_GAME_MODE_CLASS = "/Script/DemoProject.LevelGameModeView"
PLAYER_CONTROLLER_CLASS = "/Script/DemoProject.PlayerRuntimeControllerView"
RUNTIME_LEVEL_CLASS = "/Script/DemoProject.RuntimeLevelView"
TOWNSPERSON_CLASS = "/Script/DemoProject.TownspersonView"
SPEECH_PRESENTER_CLASS = "/Script/DemoProject.RuntimeSpeechPresenterView"
RUNTIME_CHAT_WIDGET_CLASS = "/Script/DemoProject.RuntimeChatWidget"

BP_LEVEL_GAME_MODE = f"{BLUEPRINT_ROOT}/BP_LevelGameMode"
BP_PLAYER_CONTROLLER = f"{BLUEPRINT_ROOT}/BP_PlayerRuntimeController"
BP_RUNTIME_LEVEL = f"{BLUEPRINT_ROOT}/BP_RuntimeLevelView"
BP_TOWNSPERSON = f"{BLUEPRINT_ROOT}/BP_TownspersonView"
BP_SPEECH_PRESENTER = f"{BLUEPRINT_ROOT}/BP_SpeechPresenter"
WBP_CHAT = f"{UI_ROOT}/WBP_Chat"


def log(message: str) -> None:
    unreal.log(f"DemoBlueprintAssets: {message}")


def ensure_directory(path: str) -> None:
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)


def split_asset_path(path: str) -> tuple[str, str]:
    folder, asset_name = path.rsplit("/", 1)
    return folder, asset_name


def load_required_class(path: str):
    loaded_class = unreal.load_class(None, path)
    if loaded_class is None:
        raise RuntimeError(f"Unable to load Unreal class: {path}")
    return loaded_class


def set_editor_property(target: object, name: str, value: object) -> None:
    try:
        target.set_editor_property(name, value)
    except Exception as exc:
        raise RuntimeError(f"Unable to set {target}.{name}: {exc}") from exc


def set_optional_editor_property(
    target: object, names: tuple[str, ...], value: object
) -> None:
    errors: list[str] = []
    for name in names:
        try:
            target.set_editor_property(name, value)
            return
        except Exception as exc:
            errors.append(f"{name}: {exc}")
    raise RuntimeError(
        f"Unable to set any of {names} on {target}: {'; '.join(errors)}"
    )


def create_or_load_blueprint(path: str, parent_class_path: str):
    ensure_directory(BLUEPRINT_ROOT)
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        log(f"Loading {path}")
        return unreal.EditorAssetLibrary.load_asset(path)

    folder, asset_name = split_asset_path(path)
    parent_class = load_required_class(parent_class_path)
    factory = unreal.BlueprintFactory()
    set_optional_editor_property(
        factory, ("parent_class", "ParentClass"), parent_class
    )
    asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        asset_name, folder, unreal.Blueprint, factory
    )
    if asset is None:
        raise RuntimeError(f"Unable to create Blueprint asset {path}")
    log(f"Created {path}")
    return asset


def create_or_load_widget_blueprint(path: str, parent_class_path: str):
    ensure_directory(UI_ROOT)
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        log(f"Loading {path}")
        return unreal.EditorAssetLibrary.load_asset(path)

    folder, asset_name = split_asset_path(path)
    parent_class = load_required_class(parent_class_path)
    factory = unreal.WidgetBlueprintFactory()
    set_optional_editor_property(
        factory, ("parent_class", "ParentClass"), parent_class
    )
    asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        asset_name, folder, unreal.WidgetBlueprint, factory
    )
    if asset is None:
        raise RuntimeError(f"Unable to create Widget Blueprint asset {path}")
    log(f"Created {path}")
    return asset


def generated_class(asset):
    generated_class_method = getattr(asset, "generated_class", None)
    if callable(generated_class_method):
        loaded_class = generated_class_method()
        if loaded_class is not None:
            return loaded_class

    try:
        loaded_class = asset.get_editor_property("generated_class")
    except Exception:
        loaded_class = getattr(asset, "generated_class", None)
    if loaded_class is None:
        raise RuntimeError(f"Unable to read generated class for {asset}")
    return loaded_class


def default_object(asset):
    return unreal.get_default_object(generated_class(asset))


def save_asset(asset) -> None:
    if not unreal.EditorAssetLibrary.save_loaded_asset(asset):
        raise RuntimeError(f"Unable to save {asset.get_path_name()}")


def wire_player_controller(player_controller, chat_widget) -> None:
    controller_defaults = default_object(player_controller)
    set_editor_property(
        controller_defaults,
        "runtime_conversation_widget_class",
        generated_class(chat_widget),
    )


def wire_game_mode(game_mode, player_controller) -> None:
    game_mode_defaults = default_object(game_mode)
    set_editor_property(
        game_mode_defaults,
        "player_controller_class",
        generated_class(player_controller),
    )


def create_assets() -> list[object]:
    chat_widget = create_or_load_widget_blueprint(
        WBP_CHAT, RUNTIME_CHAT_WIDGET_CLASS
    )
    player_controller = create_or_load_blueprint(
        BP_PLAYER_CONTROLLER, PLAYER_CONTROLLER_CLASS
    )
    game_mode = create_or_load_blueprint(BP_LEVEL_GAME_MODE, LEVEL_GAME_MODE_CLASS)
    runtime_level = create_or_load_blueprint(BP_RUNTIME_LEVEL, RUNTIME_LEVEL_CLASS)
    townsperson = create_or_load_blueprint(BP_TOWNSPERSON, TOWNSPERSON_CLASS)
    speech_presenter = create_or_load_blueprint(
        BP_SPEECH_PRESENTER, SPEECH_PRESENTER_CLASS
    )

    assets = [
        chat_widget,
        player_controller,
        game_mode,
        runtime_level,
        townsperson,
        speech_presenter,
    ]
    wire_player_controller(player_controller, chat_widget)
    wire_game_mode(game_mode, player_controller)

    for asset in assets:
        save_asset(asset)

    return assets


def main() -> int:
    assets = create_assets()
    for asset in assets:
        log(f"Saved {asset.get_path_name()}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        unreal.log_error(f"DemoBlueprintAssets: {exc}")
        raise SystemExit(1)
