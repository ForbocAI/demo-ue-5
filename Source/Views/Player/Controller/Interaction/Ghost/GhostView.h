#pragma once

/** User Story: As a level designer, I need a playable Ghost command that returns API-owned level-quality analysis. @fn void APlayerRuntimeControllerView::AnalyzeLevelWithGhost() */
void APlayerRuntimeControllerView::AnalyzeLevelWithGhost() {
  func::AsyncResult<FG::FForbocAIGhostResult> Result =
      FG::ForbocAIProtocolActions::DispatchGhostAnalysis();
  PresentMissingInteraction(
      FG::ForbocAIProtocolSelectors::SelectGhostPresentation().Message);
  Result
      .then([this](const FG::FForbocAIGhostResult &) {
        PresentGhostAnalysisResult();
      })
      .catch_([this](const std::string &) {
        PresentGhostAnalysisFailure();
      })
      .execute();
}

/** User Story: As Ghost presentation, I need fulfilled API diagnostics rendered from root-state evidence. @fn void APlayerRuntimeControllerView::PresentGhostAnalysisResult() */
void APlayerRuntimeControllerView::PresentGhostAnalysisResult() {
  PresentMissingInteraction(
      FG::ForbocAIProtocolSelectors::SelectGhostPresentation().Message);
}

/** User Story: As Ghost presentation, I need rejected root-state evidence rendered without masking the failure. @fn void APlayerRuntimeControllerView::PresentGhostAnalysisFailure() */
void APlayerRuntimeControllerView::PresentGhostAnalysisFailure() {
  PresentMissingInteraction(
      FG::ForbocAIProtocolSelectors::SelectGhostPresentation().Message);
}
