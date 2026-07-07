# Forboc AI — UE 5.7 Demo Project

A working Unreal Engine 5.7 project that demonstrates how to integrate
**Forboc AI NPCs** into your game. Clone it, press Play, and talk to AI-driven
townspeople — then copy the integration patterns into your own project.

---

## Prerequisites

| Tool | Version |
|---|---|
| Unreal Engine | 5.7 |
| Visual Studio | 2022+ (Windows, with C++ Build Tools + Windows 11 SDK) |
| Xcode | 15+ (macOS) |
| Clang | 16+ (Linux) |

---

## Quick Start

1. **Clone** (include the SDK submodule):

   ```bash
   git clone --recurse-submodules https://github.com/ForbocAI/demo-ue-5.7.git
   ```

   If you cloned without `--recurse-submodules`:

   ```bash
   git submodule update --init --recursive
   ```

2. **Run first-time setup:**

   ```bash
   bash Scripts/setup-dev.sh
   ```

3. **Generate project files:**
   - **Windows** — right-click `ForbocAIDemo.uproject` → *Generate Visual Studio Project Files*.
   - **macOS** — right-click `ForbocAIDemo.uproject` → *Generate Xcode Project*.
   - **Linux** — `UnrealEditor.sh -projectfiles -project="$PWD/ForbocAIDemo.uproject"`.

4. **Build** the `Development Editor` configuration.

5. **Launch** the editor and **Press Play**. You'll spawn into a small town with
   AI-driven NPCs you can walk up to and talk with.

---

## What's in the Box

This demo is a complete, playable project. The parts most relevant to your own
Forboc AI integration are:

| Component | Path | What It Shows |
|---|---|---|
| **SDK plugin** | `Plugins/ForbocAI_SDK` | The Forboc AI SDK, consumed as a Git submodule |
| **Dialogue system** | `Source/Features/Systems/Dialogue` | Actions, reducers, selectors, and thunks that manage NPC conversation state |
| **Bot management** | `Source/Features/Systems/Bots` | Multi-bot orchestration — spawning, tracking, and updating many NPCs at once |
| **Speech component** | `Source/Features/Systems/Speech` | Text-to-speech playback with viseme-driven facial blending |
| **Interaction system** | `Source/Features/Systems/Interaction` | Proximity triggers and player-to-NPC interaction flow |
| **Conversation UI** | `Content/UI/WBP_Chat` | A ready-made UMG chat widget driven by the dialogue state |
| **Store** | `Source/Store.*` | Single runtime store that wires all feature slices together |
| **ECS layer** | `Source/Core/ecs.hpp` | Lightweight ECS primitives used for entity/component management |
| **RTK core** | via `Plugins/ForbocAI_SDK` | Redux Toolkit–style state management: actions, reducers, selectors, thunks |

---

## SDK Integration Pattern

The demo follows a strict unidirectional data flow. Every NPC interaction
follows this path:

```text
UE event (overlap, input, tick)
  → action or thunk          (describe what happened)
  → reducer                  (update state)
  → selector                 (read derived state)
  → UE view                  (render the result)
```

**In practice, a dialogue interaction looks like this:**

1. Player overlaps an NPC trigger → dispatches a `StartDialogue` action.
2. A thunk calls the Forboc AI service and dispatches response actions.
3. The dialogue reducer updates conversation state.
4. Selectors derive the current dialogue text, speech audio, and viseme data.
5. `WBP_Chat` and the speech component read selected state and update the UI
   and facial animation.

Gameplay decisions live in **reducers and thunks** (`Source/Features`).
Display and effects live in **views** (`Source/Views`). This separation keeps
your AI integration testable and portable.

---

## SDK Submodule

`Plugins/ForbocAI_SDK` is consumed as a **read-only Git submodule**. Do not edit
SDK files from inside this repo.

To pull the latest SDK version:

```bash
bash Scripts/update-sdk.sh
```

To activate the submodule guard hooks (recommended):

```bash
git config core.hooksPath .githooks
bash Scripts/lock_sdk_submodule.sh --lock
```

These hooks prevent accidental edits inside the submodule directory.

---

## Running Tests

```bash
bash Scripts/run-tests.sh
```

This runs the project's C++ test suite, including store/reducer round-trip
tests and SDK integration checks.

## Runtime Map Tools

Press `F` during play to toggle fly mode for scale and layout inspection.

To capture reusable in-game scale evidence with the real player camera, run:

```bash
bash Scripts/capture-scale-audit.sh
```

The script launches `/Game/Map/Maps/Runtime` in game mode and writes:

- `screenshots/scale-audit-ingame-whole-level.png`
- `screenshots/scale-audit-ingame-town.png`
- `screenshots/scale-audit-ingame-people-horses.png`

Distance LOD should reduce detail and runtime cost, not hide the level. Distant
assets stay visible while lower LODs disable labels, animation, patrol,
collision, shadows, and other expensive behavior.

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `Plugin 'ForbocAI_SDK' failed to load` | Run `git submodule update --init --recursive`, then rebuild |
| Linker errors after C++ changes | Regenerate project files from `.uproject` |
| NPCs don't respond to dialogue | Check the editor Output Log for SDK connection errors; verify your API key in project settings |
| Speech audio plays but no lip sync | Ensure the NPC skeletal mesh has morph targets for the viseme set defined in `SpeechComponent` |

---

## Going Further

- **SDK reference & tutorials:** <https://docs.forboc.ai>
- **SDK source:** <https://github.com/ForbocAI/sdk-ue-5.7>
- **Issues & questions:** <https://github.com/ForbocAI/demo-ue-5.7/issues>

---

## License

© 2026 ForbocAI, Inc. All rights reserved. See [LICENSE](./LICENSE) for full terms.
