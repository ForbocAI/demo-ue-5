# ForbocAI SDK Demo Project (UE 5.7)

A working Unreal Engine 5.7 project for the ForbocAI game demo. It boots into
the tracked French Gulch runtime prototype while the UE SDK integration is
feature-gated closed by default.

Use it as a reference, copy code into your own project, or open it
in-editor to see each scene wired end-to-end.

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
| `Content/Characters/Mannequins` | Project-owned UE mannequin meshes, materials, rigs, and animation assets |
| `Content/Characters/Horses/ClassicHorse` | Project-owned horse, mounted rider, animation, and texture assets |
| `NPCDialogueDemo` | A persona-driven NPC conversation, backed by SDK calls only when the gate is open |
| `CombatEncounterDemo` | Local validation while gate-closed; Bridge validation when SDK is enabled |
| `MemoryDemo` | Local memory list while gate-closed; SDK memory store/recall when enabled |
| `BotOrchestrator` | Multi-bot manager backed by an immutable store; SDK agent routing when enabled |
| `DialogueComponent` | Reusable actor component for chat-driven NPCs with local fallback |
| `SpeechComponent` | TTS + viseme blending hooks |
| `RuntimeChatWidget` | Source-built runtime conversation UI for the French Gulch prototype |
| `ChatWidget` | UMG binding contract for SDK-backed dialogue surfaces |

`SDKTestActor` is compiled only when `FORBOC_DEMO_WITH_SDK=1`.

---

## C++ usage

When the SDK gate is open, `SDKTestActor.h` / `SDKTestActor.cpp` walk through
the three core operations:

### 1. Create an agent

```cpp
// Zero-config: SDKConfig targets api.forboc.ai by default. ApiUrl is an
// internal escape hatch — set it only for non-production API instances.
FAgentConfig Config;
Config.Persona = TEXT("Cyber-Merchant");

CurrentAgent = MakeShared<const FAgent>(AgentFactory::Create(Config));
```

### 2. Process player input

```cpp
const FAgentResponse Response =
    AgentOps::Process(*CurrentAgent, InputText, {});

OnAgentResponse(Response.Dialogue);
```

### 3. Update agent state

```cpp
const FAgentState NewState = TypeFactory::AgentState(TEXT("Alert"));

// AgentOps::WithState returns a NEW agent — original stays untouched.
CurrentAgent =
    MakeShared<const FAgent>(AgentOps::WithState(*CurrentAgent, NewState));
```

> `FAgent` and friends are immutable: assign the result of any `AgentOps::*`
> call back to your `TSharedPtr<const FAgent>` to retain it.

---

## Blueprint and UMG usage

The happy-path demo should run from source-controlled content. Do not create
local-only Blueprint or UMG assets for the first-run experience.

For runtime play, use the shipped `/Game/Map/Maps/Runtime` map and the C++
`PrototypeGameMode`, `ThirdPersonCharacter`, `RuntimePlayerController`,
`RuntimeLevel`, and `RuntimeChatWidget` classes.

For SDK/API validation with `FORBOC_DEMO_WITH_SDK=1`, source-controlled
Blueprint and UMG assets should bind to `SDKTestActor`, `DialogueComponent`,
`SpeechComponent`, and `ChatWidget`. `ChatWidget` expects UMG widget bindings
named `ChatInputBox`, `ChatScrollBox`, and `NPCNameLabel`.

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `Plugin 'ForbocAI_SDK' failed to load` | Only relevant with `FORBOC_DEMO_WITH_SDK=1`; sync the SDK submodule and rebuild |
| Linker errors after C++ changes | Regenerate project files from `.uproject` |
| `FAgent` assignment errors | Use `TSharedPtr<const FAgent>` — `FAgent` has `const` members |
| No response from agent | Run the SDK CLI `doctor` command to check API connectivity |
| Missing `Response.Content` field | The field is `Response.Dialogue` |

---

## Going further

- SDK reference, tutorials, protocol docs: <https://docs.forboc.ai>
- SDK source: <https://github.com/ForbocAI/sdk-ue-5.7>
- Issues: <https://github.com/ForbocAI/demo-ue-5.7/issues>

---

## License

© 2026 ForbocAI, Inc. All rights reserved. See [LICENSE](./LICENSE) for full terms.
