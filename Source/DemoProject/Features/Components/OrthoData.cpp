#include "Features/Components/OrthoData.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

bool FMapOrthoData::LoadFromContent() {
  SourcePath = FPaths::ProjectContentDir() /
               TEXT("Map/Heightmaps/french_gulch_ca_ortho_65.csv");

  TArray<FString> Lines;
  if (!FFileHelper::LoadFileToStringArray(Lines, *SourcePath)) {
    UE_LOG(LogTemp, Warning, TEXT("Map: Failed to load ortho CSV: %s"),
           *SourcePath);
    return false;
  }

  TArray<FColor> ParsedColors;
  int32 ParsedGridSize = 0;

  for (const FString &RawLine : Lines) {
    const FString Line = RawLine.TrimStartAndEnd();
    if (Line.IsEmpty() || Line.StartsWith(TEXT("#"))) {
      continue;
    }

    TArray<FString> Cells;
    Line.ParseIntoArray(Cells, TEXT(","), true);
    if (Cells.Num() == 0) {
      continue;
    }

    if (ParsedGridSize == 0) {
      ParsedGridSize = Cells.Num();
    } else if (Cells.Num() != ParsedGridSize) {
      UE_LOG(LogTemp, Warning,
             TEXT("Map: Ortho row width mismatch in %s"), *SourcePath);
      return false;
    }

    for (const FString &Cell : Cells) {
      TArray<FString> Channels;
      Cell.ParseIntoArray(Channels, TEXT(":"), true);
      if (Channels.Num() != 3) {
        UE_LOG(LogTemp, Warning,
               TEXT("Map: Bad ortho color cell '%s' in %s"), *Cell,
               *SourcePath);
        return false;
      }

      ParsedColors.Add(FColor(static_cast<uint8>(FCString::Atoi(*Channels[0])),
                              static_cast<uint8>(FCString::Atoi(*Channels[1])),
                              static_cast<uint8>(FCString::Atoi(*Channels[2])),
                              255));
    }
  }

  if (ParsedGridSize <= 1 || ParsedColors.Num() != ParsedGridSize * ParsedGridSize) {
    UE_LOG(LogTemp, Warning, TEXT("Map: Invalid ortho CSV grid: %s"),
           *SourcePath);
    return false;
  }

  GridSize = ParsedGridSize;
  Colors = MoveTemp(ParsedColors);
  UE_LOG(LogTemp, Display,
         TEXT("Map: Loaded %dx%d USGS ortho color CSV from %s"),
         GridSize, GridSize, *SourcePath);
  return true;
}

bool FMapOrthoData::IsLoaded() const {
  return GridSize > 1 && Colors.Num() == GridSize * GridSize;
}

FColor FMapOrthoData::GetColorAt(int32 Row, int32 Column) const {
  if (!IsLoaded()) {
    return FColor(89, 116, 73);
  }

  const int32 ClampedRow = FMath::Clamp(Row, 0, GridSize - 1);
  const int32 ClampedColumn = FMath::Clamp(Column, 0, GridSize - 1);
  return Colors[ClampedRow * GridSize + ClampedColumn];
}

int32 FMapOrthoData::GetGridSize() const { return GridSize; }

FString FMapOrthoData::GetSourcePath() const { return SourcePath; }
