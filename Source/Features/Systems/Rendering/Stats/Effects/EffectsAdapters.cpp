#include "Features/Systems/Rendering/Stats/Effects/EffectsAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"
#include "Features/Systems/Rendering/Stats/RenderingStatsSelectors.h"

#include "Engine/Engine.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsAdapters {
namespace {

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeBudget, Log, All);

/**
 * User Story: As a runtime budget reader, I need sampled diagnostics logged with the authored format so capture output remains data-driven.
 * @fn void LogRuntimeBudgetSample( const FRuntimeStatsViewModel &Stats, const ForbocAI::Game::Data::FOverlaySettings &Settings)
 */
void LogRuntimeBudgetSample(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  const FString Message =
      RenderingStatsSelectors::FormatRuntimeStatsBudgetLogMessage(Stats,
                                                                  Settings);
  UE_LOG(LogForbocRuntimeBudget, Display,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *Message);
}

/**
 * User Story: As a runtime diagnostics reader, I need refreshed stats presented through the engine debug adapter so pure selectors remain free of Unreal effects.
 * @fn void PresentRuntimeStatsDebugMessage( const FRuntimeStatsViewModel &Stats, const ForbocAI::Game::Data::FOverlaySettings &Settings)
 */
void PresentRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  GEngine ? (GEngine->AddOnScreenDebugMessage(
                 Settings.Presentation.Message.DebugMessageKey,
                 Settings.Presentation.Message.DebugMessageDurationSeconds,
                 Settings.Presentation.Colors.TextColor.ToFColor(true),
                 RenderingStatsSelectors::FormatRuntimeStatsDebugMessage(
                     Stats, Settings)),
             void())
          : void();
}

/** User Story: As a stats effect executor, I need one key-to-effect catalog so effect routing is declarative and exhaustively testable. @fn func::Dispatcher<ERuntimeStatsEffect, bool> RuntimeStatsEffectCatalog(const FRuntimeStatsEffectRequest &Request) */
func::Dispatcher<ERuntimeStatsEffect, bool>
RuntimeStatsEffectCatalog(const FRuntimeStatsEffectRequest &Request) {
  return func::createDispatcher<ERuntimeStatsEffect, bool>(
      {{ERuntimeStatsEffect::Present,
        [&Request]() {
          PresentRuntimeStatsDebugMessage(*Request.Stats, *Request.Settings);
          return true;
        }},
       {ERuntimeStatsEffect::BudgetLog,
        [&Request]() {
          LogRuntimeBudgetSample(*Request.Stats, *Request.Settings);
          return true;
        }},
       {ERuntimeStatsEffect::BudgetScreenshot,
        [&Request]() {
          RenderingAdapters::RequestRuntimeBudgetScreenshot(
              *Request.Settings, Request.ScreenshotIndex);
          return true;
        }}});
}

} // namespace

/** User Story: As a rendering stats thunk, I need declared effects routed through one registered catalog so Unreal presentation, logging, and screenshots remain outside pure planning. @fn bool ExecuteRuntimeStatsEffects(const FRuntimeStatsEffectRequest &Request) */
bool ExecuteRuntimeStatsEffects(const FRuntimeStatsEffectRequest &Request) {
  const func::Dispatcher<ERuntimeStatsEffect, bool> Dispatcher =
      RuntimeStatsEffectCatalog(Request);
  return func::all_array(
      *Request.Effects, [&Dispatcher](ERuntimeStatsEffect Effect) {
        return func::match(
            func::dispatch(Dispatcher, Effect),
            [](bool bExecuted) { return bExecuted; }, []() { return false; });
      });
}

} // namespace RenderingStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
