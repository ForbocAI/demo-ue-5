# ForbocAI SDK Demo Project (UE 5.7)

A working Unreal Engine 5.7 project for the ForbocAI game demo. It boots into
the tracked French Gulch runtime prototype while the UE SDK integration is
feature-gated closed by default.

Use it as a reference for project layout, runtime state flow, gameplay
presentation boundaries, and SDK-gated integration. Copy the patterns into your
own project rather than treating every demo-specific file as reusable SDK code.

---

## Prerequisites

| Tool | Version |
|---|---|
| Unreal Engine | 5.7 |
| Visual Studio | 2022+ (Windows, with C++ Build Tools + Windows 11 SDK) |
| Xcode | 15+ (macOS) |
| Clang | 16+ (Linux) |

The SDK/API path talks to the ForbocAI API over HTTP, but that path is opt-in.
The default build does not compile or load `ForbocAI_SDK`.

---

## UE SDK feature gate

The UE SDK feature is closed by default so the playable demo can move forward
before the next API, TS SDK, and UE SDK PRs land.

To keep the door closed, build normally:

```bash
bash run-tests.sh
```

To reopen the SDK path for validation work, set `FORBOC_DEMO_WITH_SDK=1` before
generating project files or building:

```bash
FORBOC_DEMO_WITH_SDK=1 bash run-tests.sh
```

On Windows PowerShell:

```powershell
$env:FORBOC_DEMO_WITH_SDK = "1"
```

Only use the open-gate path after the UE SDK is synced with the TS SDK/API map.

---

## Setup

1. **Clone** the demo project. The SDK submodule is present for integration
   work, but it is not required for the default gate-closed prototype build.

   ```bash
   git clone --recurse-submodules https://github.com/ForbocAI/demo-ue-5.7.git
   ```

   If you cloned without `--recurse-submodules`:

   ```bash
   git submodule update --init --recursive
   ```

2. **Generate project files**
   - **Windows**: right-click `DemoProject.uproject` → *Generate Visual Studio Project Files*.
   - **macOS**: right-click `DemoProject.uproject` → *Generate Xcode Project*.
   - **Linux**: run `UnrealEditor.sh -projectfiles -project="$PWD/DemoProject.uproject"`.

3. **Build** the `Development Editor` configuration.

4. **Launch** the editor from your IDE or by double-clicking `DemoProject.uproject`.
   The project opens `/Game/Map/Maps/Runtime`, the default French Gulch demo
   map configured in `Config/DefaultEngine.ini`.

5. **Press Play** to enter the runtime prototype. The player and townspeople use
   the project-owned Manny assets under `Content/Characters/Mannequins`; mounted
   encounters use the committed Classic Horse and rider assets under
   `Content/Characters/Horses/ClassicHorse`.

---

## What's in the box

| Scene / Component | What it shows |
|---|---|
| `/Game/Map/Maps/Runtime` | First-run 1899 French Gulch runtime map with talkable townspeople |
| `Source/Core/ecs.hpp` | Neutral ECS primitives and request-object transforms |
| `Source/Store.*` | Single configured runtime store for the playable demo |
| `Source/Features` | RTK-style feature domains: actions, reducers, selectors, thunks, adapters, factories, and slices |
| `Source/Views` | Unreal display/effect boundary for actors, widgets, input binding, and mesh application |
| `Content/Characters/Mannequins` | Project-owned UE mannequin meshes, materials, rigs, and animation assets |
| `Content/Characters/Horses/ClassicHorse` | Project-owned horse, mounted rider, animation, and texture assets |
| `Systems/Bots` | Multi-bot feature state backed by normalized RTK-style reducers/selectors |
| `Systems/Dialogue` | Dialogue actions, reducers, selectors, and thunks for gate-closed runtime interaction |
| `SpeechComponent` | TTS + viseme blending hooks |
| `/Game/UI/WBP_Chat` | Source-controlled UMG conversation UI parented to `RuntimeChatWidget` |

## Runtime architecture

The demo uses one unidirectional runtime state path:

```text
UE event / tick / overlap
  -> RTK-style action or thunk
  -> reducer-owned state transition
  -> selector
  -> UE view applies selected state
```

`Source/Features` owns durable gameplay state and semantics. Feature code uses
Redux Toolkit-style terms from the SDK `Core/rtk.hpp`: actions describe events,
reducers own transitions, selectors own reads, thunks own imperative workflows,
entity adapters own normalized collections, and `Source/Store.*` owns the single
configured store.

`Source/Core/ecs.hpp` is the neutral ECS layer for entity IDs, component/tag
projection, and request-object transforms. `Core/functional_core.hpp` is used
for pure composition, lazy values, and small unary transforms; it is not used as
a substitute action/store model.

`Source/Views` is display/effect code. Views may bind Unreal input, spawn or
attach actors/widgets, create procedural meshes through `ProceduralMeshElement`,
and apply selected transforms/text. Decisions belong in reducers, selectors,
thunks, or ECS helpers.

---

## Blueprint and UMG usage

The happy-path demo runs from source-controlled content. Do not create
local-only Blueprint or UMG assets for the first-run experience.

The shipped runtime map uses `/Game/Blueprints/BP_LevelGameMode`, which wires
`/Game/Blueprints/BP_PlayerRuntimeController` to `/Game/UI/WBP_Chat`.
`/Game/Blueprints/BP_RuntimeLevelView`, `/Game/Blueprints/BP_TownspersonView`,
and `/Game/Blueprints/BP_SpeechPresenter` provide the tracked editor-facing
presentation assets for the level, dialogue actors, and speech component.

`WBP_Chat` receives selected view models from the single runtime store through
`RuntimeChatWidget`. Dialogue, speech, and interaction decisions belong in
actions, reducers, thunks, selectors, and ECS helpers, not in widget graphs.

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `Plugin 'ForbocAI_SDK' failed to load` | Only relevant with `FORBOC_DEMO_WITH_SDK=1`; sync the SDK submodule and rebuild |
| Linker errors after C++ changes | Regenerate project files from `.uproject` |

---

## Going further

- SDK reference, tutorials, protocol docs: <https://docs.forboc.ai>
- SDK source: <https://github.com/ForbocAI/sdk-ue-5.7>
- Issues: <https://github.com/ForbocAI/demo-ue-5.7/issues>

---

## License

© 2026 ForbocAI, Inc. All rights reserved. See [LICENSE](./LICENSE) for full terms.
