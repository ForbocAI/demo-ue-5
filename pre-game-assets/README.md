# UE Runtime Pre-Game Assets

This folder stages source/reference assets before they are converted into runtime
Unreal assets. It follows the Therapy 4150 pattern: keep acquisition proof,
source packages, and import notes in version control, then produce runtime
proof through editor automation and screenshots.

This public demo repo does not publish licensed third-party source archives,
order screenshots, product screenshots, or editable imported KitBash assets.
Keep those files in this folder locally, regenerate the inventory when needed,
and import them into the editor from a licensed workstation.

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

## KitBash3D Outlaw Kit

- Source model package: `kb3d_outlaw.fbxobj.native.zip`
- Texture package: `kb3d_outlaw.png.2k.zip`
- Product proof: `kitbash_outlaw_product_page.png`
- Product model-list proof: `kitbash_outlaw_product_models.png`
- Acquisition proof: `kitbash_outlaw_order_372529_redacted.png`
- Inventory JSON: `kb3d_outlaw_inventory.json`
- Inventory notes: `kb3d_outlaw_inventory.md`

Order proof shows KitBash3D order `#372529`, confirmed 2026-07-07, for the
Outlaw kit under the individual license tier. The local proof image should stay
redacted so browser tokens, contact details, billing address, phone, and payment
details are not stored in the repository.

Archive review:

- Model zip is about 770 MB compressed and expands to about 1.8 GB.
- Texture zip is about 2.0 GB compressed and expands to about 2.14 GB.
- OBJ inventory: 904 objects, 87 materials, about 7.5M vertices, and about
  6.8M faces.
- Texture inventory: 571 PNG files at 2048x2048, covering basecolor, normal,
  roughness, metallic, AO, height, opacity, emissive, and refraction maps.
- The OBJ material library references textures under `KB3DTextures/4k`, but
  the 2K texture archive stores PNGs at the archive root. Import automation must
  normalize the texture folder layout or rewrite material references.
- The native OBJ bounds are about 266.8 x 26.8 x 200.1 units, centered near
  (7.9, 12.1, -33.3), so import automation must normalize scale, origin, and
  orientation before these assets are placed into the Runtime map.

Current local UE import:

- The local runtime content imports 18 curated Outlaw groups into
  `/Game/Environment/KitBash/Outlaw`.
- The generated content currently contains 518 `.uasset` files and is about
  1.3 GB on disk.
- Imported groups cover French Gulch storefronts and dressing pieces: post
  office, general store, saloon, church, apothecary, shoe store, mining
  supplies, sheriff station, blacksmith, mine entrance, wagon, train cart,
  train tracks, barrels, crates, lantern posts, wanted posters, and bridge.
- The importer writes one filtered `.mtl` per group and imports into a shared
  Outlaw content folder so common materials/textures are reused instead of
  duplicated into one folder per group.
- The importer strips unsupported OBJ `refl` material records before UE import.

Reusable tooling:

- `python3 Scripts/Tools/inventory_outlaw_assets.py` regenerates the JSON and
  Markdown inventory from the source zips.
- `UnrealEditor-Cmd.exe ForbocAIDemo.uproject -run=pythonscript
  -script=Scripts/Tools/import_outlaw_assets.py` stages selected object groups
  under `Intermediate/MapOutlawImport`, writes per-group material files,
  rebuilds the expected `KB3DTextures/4k` texture layout from the 2K texture
  archive, and imports the curated groups under
  `/Game/Environment/KitBash/Outlaw`.
