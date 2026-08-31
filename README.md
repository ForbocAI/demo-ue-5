# ForbocAI Unreal Engine Demo

A playable Unreal Engine 5.8 reference project showing ForbocAI NPC dialogue,
memory-aware interactions, validated actions, Ghost level pre-playtesting,
chat presentation, and speech presentation in a game world.

## Requirements

- Unreal Engine 5.8
- Visual Studio 2022 C++ tools on Windows, Xcode 15 or newer on macOS, or
  Clang 16 or newer on Linux
- A ForbocAI API key from <https://account.forboc.ai>

## Install

Clone the project and its ForbocAI SDK plugin:

```bash
git clone --recurse-submodules https://github.com/ForbocAI/demo-ue-5.git
cd demo-ue-5
bash Scripts/Setup/setup-dev.sh
```

If the repository was cloned without submodules, run:

```bash
git submodule update --init --recursive
```

Set `FORBOCAI_API_KEY` in the environment used to launch Unreal Editor. The
hosted API URL is selected automatically unless `FORBOCAI_API_URL` is set.

Generate project files, build the `Development Editor` target, and open
`ForbocAIDemo.uproject`.

## Play

The project opens `/Game/Map/Maps/Runtime` by default.

1. Press Play.
2. Move near a townsperson.
3. Press `E` when the interaction prompt appears.
4. Submit text through the chat interface.
5. Observe the ForbocAI response in chat and speech presentation.
6. Press `G` to run Ghost pre-playtesting and view the returned level-quality
   analysis.

The demo keeps each NPC's identity and game context separate while routing all
ForbocAI interaction through the installed SDK plugin.

## Verify The Integration

Run the focused map smoke before integrating the demo pattern into another
project:

```bash
bash Scripts/Run/run-map-smoke.sh
```

On Windows, the equivalent command is:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File ".\Scripts\Run\run-map-smoke.ps1"
```

The smoke builds the editor target, verifies that the default map contains the
required ForbocAI runtime and speech wiring, then opens the map in unattended
game mode. It does not require an API interaction.

## ForbocAI Integration Points

| ForbocAI behavior | Demo surface |
| --- | --- |
| NPC identity and runtime context | `Content/Data/levels/french_gulch/townspeople.json` |
| Player-to-NPC interaction | `Source/Features/Systems/Interaction` |
| Dialogue state | `Source/Features/Systems/Dialogue` |
| Multi-NPC orchestration | `Source/Features/Systems/Bots` |
| NPC and Ghost SDK CLI orchestration | `Source/Features/Systems/ForbocAI/Protocol` |
| Chat presentation | `Source/Views/Chat` |
| Speech and lip-sync presentation | `Source/Views/Speech/Presenter` |
| SDK plugin | `Plugins/ForbocAI_SDK` |

Use these surfaces as examples of where to send game context to ForbocAI and
where to apply returned dialogue and validated actions. Keep your own game's
world rules, UI, animation, audio, and save data in the game project.

## Troubleshooting

| Problem | Resolution |
| --- | --- |
| The SDK plugin does not load | Initialize the submodule and rebuild the editor target |
| An NPC does not answer | Confirm `FORBOCAI_API_KEY` is available to Unreal Editor and inspect the Output Log for ForbocAI errors |
| Ghost analysis fails | Inspect the Output Log for the preserved Ghost API failure and correlation/session evidence |
| Chat updates without speech | Check the speech presenter and the selected NPC mesh configuration |
| Project files are stale | Regenerate project files, then rebuild the editor target |

## More

- Unreal SDK documentation: <https://docs.forboc.ai/ue/welcome>
- Unreal SDK source: <https://github.com/ForbocAI/sdk-ue-5>
- Demo issues: <https://github.com/ForbocAI/demo-ue-5/issues>

## License

All rights reserved. See [LICENSE](./LICENSE).
