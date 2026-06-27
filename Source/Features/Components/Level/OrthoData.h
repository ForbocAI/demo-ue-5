#pragma once

#include "CoreMinimal.h"

class DEMOPROJECT_API FMapOrthoData {
public:
  bool LoadFromContent();
  bool IsLoaded() const;

  FColor GetColorAt(int32 Row, int32 Column) const;
  int32 GetGridSize() const;
  FString GetSourcePath() const;

private:
  int32 GridSize = 0;
  FString SourcePath;
  TArray<FColor> Colors;
};
