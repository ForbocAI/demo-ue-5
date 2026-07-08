#pragma once

#include "Features/Components/Level/Data/Csv/CsvParseAdapters.h"

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

bool FLevelOrthoData::IsLoaded() const {
  return GridSize > MinimumGridSize &&
         Colors.Num() == GridSize * GridSize;
}

FColor FLevelOrthoData::GetColorAt(int32 Row, int32 Column) const {
  check(IsLoaded());

  const int32 ClampedRow =
      FMath::Clamp(Row, MinimumGridIndex, GridSize - GridTerminalOffset);
  const int32 ClampedColumn =
      FMath::Clamp(Column, MinimumGridIndex, GridSize - GridTerminalOffset);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

int32 FLevelOrthoData::GetGridSize() const { return GridSize; }

FString FLevelOrthoData::GetSourcePath() const { return SourcePath; }
