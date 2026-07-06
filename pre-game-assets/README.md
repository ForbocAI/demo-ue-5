# UE Runtime Pre-Game Assets

This folder stages source/reference assets before they are converted into runtime
Unreal assets. It follows the Therapy 4150 pattern: keep acquisition proof,
source packages, and import notes in version control, then produce runtime
proof through editor automation and screenshots.

## Classic Horse Animated Asset

- Source package: `uploads_files_4102381_Classic_Horse.zip`
- FBX package: `uploads_files_4102381_animated_meshes_fbx.zip`
- Acquisition proof: `Order #17413382 _ CGTrader - Google Chrome 2026-06.png`
- Store/reference image: `3D model Classic Horse Animated Asset VR _ AR _ lo.png`

The current French Gulch prototype uses primitive `AWalkingHorse` actors so the
1899 town can show horses moving around Main Street before the UE SDK and asset
import pipeline are ready. The staged horse packages are reserved for the later
editor-import pass that should convert the model, animation clips, materials,
and screenshot proof into committed UE runtime assets.
