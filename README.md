# Forboc AI - Unreal Engine Demo

A working Unreal Engine 5.8 project that shows how to put **Forboc AI NPCs**
inside a playable game loop. Clone it, run the setup script, press Play, and
study how the demo routes player interaction into ForbocAI-backed NPC state,
conversation UI, and speech presentation.

## Prerequisites

| Tool | Version |
| --- | --- |
| Unreal Engine | 5.8 |
| Visual Studio | 2022+ with C++ Build Tools and Windows 11 SDK |
| Xcode | 15+ on macOS |
| Clang | 16+ on Linux |

## Quick Start

Clone with the SDK submodule:

```bash
git clone --recurse-submodules https://github.com/ForbocAI/demo-ue-5.git
cd demo-ue-5
```

If you already cloned without submodules:

```bash
git submodule update --init --recursive
```

Run first-time setup:

```bash
bash Scripts/Setup/setup-dev.sh
```

Generate project files:

- Windows: right-click `ForbocAIDemo.uproject` and choose **Generate Visual Studio Project Files**.
- macOS: right-click `ForbocAIDemo.uproject` and choose **Generate Xcode Project**.
- Linux:

  ```bash
  UnrealEditor.sh -projectfiles -project="$PWD/ForbocAIDemo.uproject"
  ```

Build the `Development Editor` target, open the project, then press Play.

## What To Study

The demo is a game project, not the SDK itself. The useful ForbocAI integration
surfaces are:

| Area | Path | What It Shows |
| --- | --- | --- |
| SDK plugin boundary | `Plugins/ForbocAI_SDK` | Consuming the ForbocAI UE SDK as a submodule |
| Runtime store | `Source/Store.*` | One game-owned state boundary for NPC interaction results |
| NPC and bot state | `Source/Features/Systems/Bots` | Multi-NPC state, position, goals, AI facts, and orchestration |
| Interaction flow | `Source/Features/Systems/Interaction` | Turning player focus/input into NPC conversation events |
| Dialogue state | `Source/Features/Systems/Dialogue` | Conversation actions, selectors, and reducer-owned state |
| Conversation UI | `Source/Views/Chat` and `Source/Views/Player/Controller` | Rendering selected ForbocAI dialogue into the viewport |
| Speech presentation | `Source/Features/Systems/Speech` and `Source/Views/Speech/Presenter` | Routing selected response text into speech/lip-sync presentation |

## Integration Pattern

The demo keeps the AI turn path explicit:

```text
UE input / overlap
  -> interaction action or thunk
  -> ForbocAI dialogue backend or local demo fallback
  -> reducer-owned conversation state
  -> selectors
  -> chat widget and speech presenter
```

Game actors and widgets apply selected state. They do not own the ForbocAI
protocol, API calls, or durable NPC state directly.

## SDK Submodule

`Plugins/ForbocAI_SDK` is a read-only Git submodule in this checkout. Change
SDK code in the SDK repository, then update the submodule pointer here.

Update the SDK pointer:

```bash
bash Scripts/SDK/update-sdk.sh
```

Activate the submodule guard hooks:

```bash
git config core.hooksPath .githooks
bash Scripts/SDK/lock_sdk_submodule.sh --lock
```

## Verify The Demo

Run the project verification wrapper:

```bash
bash Scripts/Run/run-tests.sh
```

This builds the editor target, runs the ForbocAI automation tests, and checks
the project guardrails used by the demo.

## Troubleshooting

| Problem | Fix |
| --- | --- |
| `Plugin 'ForbocAI_SDK' failed to load` | Run `git submodule update --init --recursive`, then rebuild |
| Linker errors after changing branches | Delete `Intermediate/` and `Binaries/`, regenerate project files, then rebuild |
| NPC conversation does not update | Check the editor Output Log for SDK/API configuration errors and verify your API key |
| Speech plays without matching presentation | Check the speech settings and the NPC mesh/morph-target setup used by your project |

## Links

- SDK docs: <https://docs.forboc.ai>
- UE SDK source: <https://github.com/ForbocAI/sdk-ue-5>
- Demo issues: <https://github.com/ForbocAI/demo-ue-5/issues>

## License

(c) 2026 ForbocAI, Inc. All rights reserved. See [LICENSE](./LICENSE) for
terms.
