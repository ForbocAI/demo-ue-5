#include "Features/Systems/Terrain/TerrainSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainSelectors {

bool SelectLoaded(const FTerrainState &State) {
  return (func::pipe(State.bTerrainLoaded) |
          [](bool bTerrainLoaded) -> bool { return bTerrainLoaded; })
      .val;
}

FTerrainMeshSectionViewModel
SelectTerrainMeshSectionViewModel(const FTerrainMeshPayload &Payload) {
  return (func::pipe(FTerrainMeshSectionViewModel{}) |
          [&Payload](FTerrainMeshSectionViewModel Model) {
            Model.bLoaded = Payload.bLoaded;
            Model.Vertices = Payload.Vertices;
            Model.Triangles = Payload.Triangles;
            Model.Normals = Payload.Normals;
            Model.UVs = Payload.UVs;
            Model.VertexColors = Payload.VertexColors;
            return Model;
          })
      .val;
}

} // namespace TerrainSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
