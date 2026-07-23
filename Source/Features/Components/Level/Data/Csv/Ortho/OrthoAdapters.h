#pragma once

#include "Features/Components/Level/Data/Csv/Parsing/ParsingAdapters.h"

/** User Story: As a data csv ortho consumer, I need to invoke load from content through a stable signature so the data csv ortho workflow remains explicit and composable. @fn bool FLevelOrthoData::LoadFromContent( const FOrthoDataLoadRequest &Request) */
bool FLevelOrthoData::LoadFromContent(
    const FOrthoDataLoadRequest &Request) {
  MinimumGridSize = Request.Csv.Grid.MinimumGridSize;
  MinimumGridIndex = Request.Csv.Grid.EmptyCount;
  GridTerminalOffset = Request.Csv.Grid.TerminalOffset;
  SourcePath = FPaths::ProjectContentDir() / Request.Sources.OrthoCsvPath;

  TArray<FString> Lines;
  const bool bLoaded = FFileHelper::LoadFileToStringArray(Lines, *SourcePath);
  check(bLoaded);

  const FOrthoCsvParseState Parsed =
      func::fold_indexed(Lines, static_cast<size_t>(Lines.Num()),
	                         FOrthoCsvParseState{},
	                         [&Request](const FOrthoCsvParseState &State,
	                                    const FString &Line) {
	                           return ReduceOrthoLine({&Request.Csv, State, Line});
	                         });
  check(Parsed.bValid);
  check(Parsed.GridSize > Request.Csv.Grid.MinimumGridSize);
  check(Parsed.Colors.Num() == Parsed.GridSize * Parsed.GridSize);

  GridSize = Parsed.GridSize;
  Colors = Parsed.Colors;
  const FString LoadedMessage =
      CsvMessage(Request.Csv.Messages.OrthoLoadedFormat,
                 {FStringFormatArg(GridSize), FStringFormatArg(GridSize),
                  FStringFormatArg(SourcePath)});
  UE_LOG(LogTemp, Display,
         TEXT("%s"), *LoadedMessage);
  return true;
}

/** User Story: As a data csv ortho consumer, I need to invoke is loaded through a stable signature so the data csv ortho workflow remains explicit and composable. @fn bool FLevelOrthoData::IsLoaded() const */
bool FLevelOrthoData::IsLoaded() const {
  return GridSize > MinimumGridSize &&
         Colors.Num() == GridSize * GridSize;
}

/** User Story: As a data csv ortho consumer, I need to invoke get color at through a stable signature so the data csv ortho workflow remains explicit and composable. @fn FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const */
FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow =
      FMath::Clamp(Row, MinimumGridIndex, GridSize - GridTerminalOffset);
  const int32 ClampedColumn =
      FMath::Clamp(Column, MinimumGridIndex, GridSize - GridTerminalOffset);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

/** User Story: As a data csv ortho consumer, I need to invoke get grid size through a stable signature so the data csv ortho workflow remains explicit and composable. @fn int32 FLevelOrthoData::GetGridSize() const */
int32 FLevelOrthoData::GetGridSize() const { return GridSize; }

/** User Story: As a data csv ortho consumer, I need to invoke get source path through a stable signature so the data csv ortho workflow remains explicit and composable. @fn FString FLevelOrthoData::GetSourcePath() const */
FString FLevelOrthoData::GetSourcePath() const { return SourcePath; }
