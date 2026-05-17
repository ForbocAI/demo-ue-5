# ForbocAI SDK Demo Project (UE 5.7)

A working Unreal Engine 5.7 project that shows how to integrate the
[**ForbocAI SDK**](https://github.com/ForbocAI/sdk-ue-5.7) into a game:
spawn agents, drive dialogue, route decisions through your game's
ruleset, and manage multi-bot encounters from C++ or Blueprints.

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

The demo talks to the ForbocAI API over HTTP. By default it uses production routing; for local-dev overrides, use `SDKConfig::SetApiConfig(TEXT("http://localhost:8080"), TEXT(""))` globally as an internal escape hatch.

---

## Setup

1. **Clone with submodules** — the SDK is a submodule pointing at
   [`ForbocAI/sdk-ue-5.7`](https://github.com/ForbocAI/sdk-ue-5.7).

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

---

## What's in the box

| Scene / Component | What it shows |
|---|---|
| `NPCDialogueDemo` | A persona-driven NPC conversation with persistent memory recall |
| `CombatEncounterDemo` | Bridge validation — agent proposes actions, your ruleset accepts or rejects |
| `MemoryDemo` | Memory store/recall round-trip |
| `BotOrchestrator` | Multi-bot manager backed by an immutable store; spawn, route, and tear down agents |
| `DialogueComponent` | Reusable actor component for chat-driven NPCs |
| `SpeechComponent` | TTS + viseme blending hooks |
| `ChatWidget` (UMG) | Drop-in dialogue UI |

`SDKTestActor` is the minimal "hello world" entry point — start there.

---

## C++ usage

`SDKTestActor.h` / `SDKTestActor.cpp` walk through the three core operations:

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

## Blueprint usage

1. **Create Blueprint**: Content Browser → New Blueprint Class →
   parent `SDKTestActor` → name it `BP_SDKTestAgent`.
2. **Configure**: open `BP_SDKTestAgent` → Class Defaults.
   - **Persona**: e.g. `"Cyber-Merchant"`
   - **Api Url**: leave blank — the SDK defaults to `https://api.forboc.ai`. Set it only when pointing at a non-production API instance.
3. **Implement events**: in the Event Graph, implement
   **Event On Agent Response** and wire it to **Print String**
   (or your dialogue widget).
4. **Test**:
   - `Initialize Agent` fires automatically on `BeginPlay`.
   - Call **Process Input** on a key press or UI event:
     `"Hello, who are you?"`
   - Call **Update Agent State** to change mood / context.

---

## Troubleshooting

| Problem | Fix |
|---|---|
| `Plugin 'ForbocAI_SDK' failed to load` | Submodule missing or unbuilt — run `git submodule update --init --recursive`, then rebuild |
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
