#include "Features/Systems/Terrain/TerrainSlice.h"

#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {
namespace {

struct FMeshSourceGrid {
  int32 SourceGridSize;
  int32 SourceGridLastIndex;
};

struct FMeshLodGrid {
  int32 LodGridSize;
  int32 LodGridLastIndex;
  int32 LodStep;
};

struct FMeshQuadStep {
  int32 QuadColumnStep;
  int32 QuadRowStep;
};

struct FMeshSpace {
  float HalfSize;
  float Step;
  float VertexHeightOffset;
};

struct FMeshBuildContext {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  FMeshSourceGrid Source;
  FMeshLodGrid Lod;
  FMeshQuadStep Quad;
  FMeshSpace Space;
};

struct FMeshReserveRequest {
  int32 LodGridSize;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FLodGridSizeRequest {
  int32 SourceGridSize;
  int32 LodStep;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FGridIndex {
  int32 Row;
  int32 Column;
};

FMeshPayload
ReserveTerrainMeshPayload(const FMeshReserveRequest &Request) {
  return (func::pipe(FMeshPayload{}) |
          [&Request](FMeshPayload Payload) {
            Payload.bLoaded = true;
            Payload.MaterialSlotIndex =
                Request.Geometry.TerrainMaterialSlotIndex;
            Payload.MeshSectionIndex = Request.Geometry.TerrainMeshSectionIndex;
            Payload.bCreateCollision = Request.Geometry.bTerrainCreateCollision;
            Payload.Vertices.Reserve(Request.LodGridSize *
                                     Request.LodGridSize);
            Payload.Normals.Reserve(Request.LodGridSize *
                                    Request.LodGridSize);
            Payload.UVs.Reserve(Request.LodGridSize * Request.LodGridSize);
            Payload.VertexColors.Reserve(Request.LodGridSize *
                                         Request.LodGridSize);
            Payload.Triangles.Reserve(
                (Request.LodGridSize -
                 Request.Geometry.TerrainGridTerminalOffset) *
                (Request.LodGridSize -
                 Request.Geometry.TerrainGridTerminalOffset) *
                Request.Geometry.TerrainQuadIndexReserveCount);
            return Payload;
          })
      .val;
}

int32 SelectTerrainLodStep(
    const ForbocAI::Game::Data::FGeometrySettings &Geometry) {
  return FMath::Max(Geometry.TerrainLodStep, Geometry.TerrainMinimumLodStep);
}

int32 SelectTerrainLodGridSize(const FLodGridSizeRequest &Request) {
  return ((Request.SourceGridSize -
           Request.Geometry.TerrainGridTerminalOffset) /
          Request.LodStep) +
         Request.Geometry.TerrainLodGridPadding;
}

int32 SelectTerrainSourceGridIndex(const FMeshBuildContext &Context,
                                   int32 LodIndex) {
  return LodIndex >= Context.Lod.LodGridLastIndex
             ? Context.Source.SourceGridLastIndex
             : LodIndex * Context.Lod.LodStep;
}

FColor TerrainVertexColor(const FMeshBuildContext &Context,
                          const FGridIndex &GridIndex) {
  return Context.OrthoData.GetColorAt(GridIndex.Row, GridIndex.Column);
}

FMeshPayload ReduceTerrainVertex(FMeshPayload Payload,
                                        const FMeshBuildContext &Context,
                                     const FGridIndex &GridIndex) {
  const int32 SourceRow = SelectTerrainSourceGridIndex(Context, GridIndex.Row);
  const int32 SourceColumn = SelectTerrainSourceGridIndex(Context, GridIndex.Column);
  const float NorthSouth = Context.Space.HalfSize - Context.Space.Step * SourceRow;
  const float EastWest = -Context.Space.HalfSize + Context.Space.Step * SourceColumn;
  Payload.Vertices.Add(Context.TerrainData.ToWorld(FVector2D(EastWest, NorthSouth), Context.Space.VertexHeightOffset));
  Payload.Normals.Add(FVector::UpVector);
  Payload.UVs.Add(FVector2D(
      static_cast<float>(SourceColumn) / Context.Source.SourceGridLastIndex,
      static_cast<float>(SourceRow) / Context.Source.SourceGridLastIndex));
  Payload.VertexColors.Add(TerrainVertexColor(Context,
                                              {SourceRow, SourceColumn}));
  return Payload;
}

FMeshPayload ReduceTerrainQuadTriangles(
    FMeshPayload Payload,
    const FMeshBuildContext &Context,
    const FGridIndex &GridIndex) {
  const int32 A = GridIndex.Row * Context.Lod.LodGridSize + GridIndex.Column;
  const int32 B = A + Context.Quad.QuadColumnStep;
  const int32 C = A + Context.Lod.LodGridSize * Context.Quad.QuadRowStep;
  const int32 D = C + Context.Quad.QuadColumnStep;
  Payload.Triangles.Add(A);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(D);
  return Payload;
}


FMeshPayload BuildLoadedTerrainMeshPayload(
    const FMeshBuildRequest &Request) {
  const FLevelTerrainData &TerrainData = Request.TerrainData;
  const FLevelOrthoData &OrthoData = Request.OrthoData;
  const ForbocAI::Game::Data::FGeometrySettings &Geometry = Request.Geometry;
  const int32 SourceGridSize = TerrainData.GetGridSize();
  const int32 LodStep = SelectTerrainLodStep(Geometry);
  const int32 LodGridSize =
      SelectTerrainLodGridSize({SourceGridSize, LodStep, Geometry});
  const FMeshBuildContext Context{
      TerrainData,
      OrthoData,
      {SourceGridSize, SourceGridSize - Geometry.TerrainGridTerminalOffset},
      {LodGridSize, LodGridSize - Geometry.TerrainGridTerminalOffset,
       LodStep},
      {Geometry.TerrainQuadColumnStep, Geometry.TerrainQuadRowStep},
      {TerrainData.GetTerrainWorldSize() * Geometry.TerrainHalfWorldSizeScale,
       TerrainData.GetTerrainWorldSize() /
           static_cast<float>(SourceGridSize -
                              Geometry.TerrainGridTerminalOffset),
       Geometry.TerrainVertexHeightOffset}};
  const FMeshPayload WithVertices =
      func::fold_grid_range<FMeshPayload>(
          LodGridSize, LodGridSize,
          ReserveTerrainMeshPayload({LodGridSize, Geometry}),
          [&Context](const FMeshPayload &Payload,
                     const func::GridIndex &Index) {
            return ReduceTerrainVertex(Payload, Context,
                                       {static_cast<int32>(Index.Row),
                                        static_cast<int32>(Index.Column)});
          });
  return func::fold_grid_range<FMeshPayload>(
      Context.Lod.LodGridLastIndex, Context.Lod.LodGridLastIndex, WithVertices,
      [&Context](const FMeshPayload &Payload,
                 const func::GridIndex &Index) {
        return ReduceTerrainQuadTriangles(Payload, Context,
                                          {static_cast<int32>(Index.Row),
                                           static_cast<int32>(Index.Column)});
      });
}

} // namespace

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FLoadedPayload> &Action) {
  return (func::pipe(State) | [&](FTerrainState Next) -> FTerrainState {
  Next.bTerrainLoaded = true;
  Next.TerrainSource = Action.PayloadValue.TerrainSource;
  Next.OrthoSource = Action.PayloadValue.OrthoSource;
  Next.GridSize = Action.PayloadValue.GridSize;
  Next.MinElevationMeters = Action.PayloadValue.MinElevationMeters;
  Next.MaxElevationMeters = Action.PayloadValue.MaxElevationMeters;
  return Next;
  }).val;
}

FMeshPayload
BuildTerrainMeshPayload(const FMeshBuildRequest &Request) {
  return Request.TerrainData.IsLoaded()
             ? (Request.OrthoData.IsLoaded()
                    ? BuildLoadedTerrainMeshPayload(Request)
                    : FMeshPayload{})
             : FMeshPayload{};
}

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Terrain/TerrainActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FTerrainState>> Slice =
      func::lazy([]() -> rtk::Slice<FTerrainState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FTerrainState>(
          TEXT("terrain"), TerrainFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTerrainState> &Builder) {
    Builder.addCase(TerrainActions::TerrainLoaded(),
                                TerrainReducers::ReduceTerrainLoaded);
  });
      });
  return func::eval(Slice);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI


namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainFactories {

FTerrainState CreateInitialState() {
  return FTerrainState{};
}

FLoadedPayload LoadedPayload(const FLoadedSource &Source) {
  FLoadedPayload Payload;
  Payload.TerrainSource = Source.TerrainSource;
  Payload.OrthoSource = Source.OrthoSource;
  Payload.GridSize = Source.GridSize;
  Payload.MinElevationMeters = Source.MinElevationMeters;
  Payload.MaxElevationMeters = Source.MaxElevationMeters;
  return Payload;
}

} // namespace TerrainFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
