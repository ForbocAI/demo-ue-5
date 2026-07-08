#include "Features/Systems/Terrain/Slice.h"

#include "Core/ecs.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainReducers {
namespace {

struct FTerrainMeshBuildContext {
  const FLevelTerrainData &TerrainData;
  const FLevelOrthoData &OrthoData;
  int32 SourceGridSize;
  int32 SourceGridLastIndex;
  int32 LodGridSize;
  int32 LodGridLastIndex;
  int32 LodStep;
  int32 QuadColumnStep;
  int32 QuadRowStep;
  float HalfSize;
  float Step;
  float VertexHeightOffset;
};

struct FTerrainMeshReserveRequest {
  int32 LodGridSize;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FTerrainLodGridSizeRequest {
  int32 SourceGridSize;
  int32 LodStep;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FTerrainGridIndex {
  int32 Row;
  int32 Column;
};

FTerrainMeshPayload
ReserveTerrainMeshPayload(const FTerrainMeshReserveRequest &Request) {
  return (func::pipe(FTerrainMeshPayload{}) |
          [&Request](FTerrainMeshPayload Payload) {
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
    const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry) {
  return FMath::Max(Geometry.TerrainLodStep, Geometry.TerrainMinimumLodStep);
}

int32 SelectTerrainLodGridSize(const FTerrainLodGridSizeRequest &Request) {
  return ((Request.SourceGridSize -
           Request.Geometry.TerrainGridTerminalOffset) /
          Request.LodStep) +
         Request.Geometry.TerrainLodGridPadding;
}

int32 SelectTerrainSourceGridIndex(const FTerrainMeshBuildContext &Context,
                                   int32 LodIndex) {
  return LodIndex >= Context.LodGridLastIndex
             ? Context.SourceGridLastIndex
             : LodIndex * Context.LodStep;
}

FColor TerrainVertexColor(const FTerrainMeshBuildContext &Context,
                          const FTerrainGridIndex &GridIndex) {
  return Context.OrthoData.GetColorAt(GridIndex.Row, GridIndex.Column);
}

FTerrainMeshPayload ReduceTerrainVertex(FTerrainMeshPayload Payload,
                                        const FTerrainMeshBuildContext &Context,
                                     const FTerrainGridIndex &GridIndex) {
  const int32 SourceRow = SelectTerrainSourceGridIndex(Context, GridIndex.Row);
  const int32 SourceColumn = SelectTerrainSourceGridIndex(Context, GridIndex.Column);
  const float NorthSouth = Context.HalfSize - Context.Step * SourceRow;
  const float EastWest = -Context.HalfSize + Context.Step * SourceColumn;
  Payload.Vertices.Add(Context.TerrainData.ToWorld(FVector2D(EastWest, NorthSouth), Context.VertexHeightOffset));
  Payload.Normals.Add(FVector::UpVector);
  Payload.UVs.Add(FVector2D(
      static_cast<float>(SourceColumn) / Context.SourceGridLastIndex,
      static_cast<float>(SourceRow) / Context.SourceGridLastIndex));
  Payload.VertexColors.Add(TerrainVertexColor(Context,
                                              {SourceRow, SourceColumn}));
  return Payload;
}

FTerrainMeshPayload ReduceTerrainQuadTriangles(
    FTerrainMeshPayload Payload,
    const FTerrainMeshBuildContext &Context,
    const FTerrainGridIndex &GridIndex) {
  const int32 A = GridIndex.Row * Context.LodGridSize + GridIndex.Column;
  const int32 B = A + Context.QuadColumnStep;
  const int32 C = A + Context.LodGridSize * Context.QuadRowStep;
  const int32 D = C + Context.QuadColumnStep;
  Payload.Triangles.Add(A);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(B);
  Payload.Triangles.Add(C);
  Payload.Triangles.Add(D);
  return Payload;
}


FTerrainMeshPayload BuildLoadedTerrainMeshPayload(
    const FTerrainMeshBuildRequest &Request) {
  const FLevelTerrainData &TerrainData = Request.TerrainData;
  const FLevelOrthoData &OrthoData = Request.OrthoData;
  const ForbocAI::Game::Data::FLevelGeometrySettings &Geometry = Request.Geometry;
  const int32 SourceGridSize = TerrainData.GetGridSize();
  const int32 LodStep = SelectTerrainLodStep(Geometry);
  const int32 LodGridSize =
      SelectTerrainLodGridSize({SourceGridSize, LodStep, Geometry});
  const FTerrainMeshBuildContext Context{
      TerrainData, OrthoData, SourceGridSize,
      SourceGridSize - Geometry.TerrainGridTerminalOffset, LodGridSize,
      LodGridSize - Geometry.TerrainGridTerminalOffset, LodStep,
      Geometry.TerrainQuadColumnStep, Geometry.TerrainQuadRowStep,
      TerrainData.GetTerrainWorldSize() * Geometry.TerrainHalfWorldSizeScale,
      TerrainData.GetTerrainWorldSize() /
          static_cast<float>(SourceGridSize -
                             Geometry.TerrainGridTerminalOffset),
      Geometry.TerrainVertexHeightOffset};
  const FTerrainMeshPayload WithVertices =
      func::fold_grid_range<FTerrainMeshPayload>(
          LodGridSize, LodGridSize,
          ReserveTerrainMeshPayload({LodGridSize, Geometry}),
          [&Context](const FTerrainMeshPayload &Payload,
                     const func::GridIndex &Index) {
            return ReduceTerrainVertex(Payload, Context,
                                       {static_cast<int32>(Index.Row),
                                        static_cast<int32>(Index.Column)});
          });
  return func::fold_grid_range<FTerrainMeshPayload>(
      Context.LodGridLastIndex, Context.LodGridLastIndex, WithVertices,
      [&Context](const FTerrainMeshPayload &Payload,
                 const func::GridIndex &Index) {
        return ReduceTerrainQuadTriangles(Payload, Context,
                                          {static_cast<int32>(Index.Row),
                                           static_cast<int32>(Index.Column)});
      });
}

} // namespace

FTerrainState
ReduceTerrainLoaded(const FTerrainState &State,
                    const rtk::PayloadAction<FTerrainLoadedPayload> &Action) {
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

FTerrainMeshPayload
BuildTerrainMeshPayload(const FTerrainMeshBuildRequest &Request) {
  return Request.TerrainData.IsLoaded()
             ? (Request.OrthoData.IsLoaded()
                    ? BuildLoadedTerrainMeshPayload(Request)
                    : FTerrainMeshPayload{})
             : FTerrainMeshPayload{};
}

} // namespace TerrainReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Terrain/Actions.h"

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

FTerrainLoadedPayload LoadedPayload(const FTerrainLoadedSource &Source) {
  FTerrainLoadedPayload Payload;
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
