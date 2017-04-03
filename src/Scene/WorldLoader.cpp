#include <FLIGHT/Core/Game.hpp>
#include <FLIGHT/Scene/Scene.hpp>

namespace FLIGHT {
WorldLoader::WorldLoader()
    : m_active(true), m_terrainThread([this] {
          // TODO: intially request chunk at player's plane's position.
          GetGame().GetTerrainMgr().RequestChunk(0, -3);
          try {
              while (this->m_active) {
                  auto & game = GetGame();
                  if (game.GetTerrainMgr().HasWork()) {
                      game.GetTerrainMgr().UpdateTerrainGen();
                  } else {
                      std::this_thread::sleep_for(std::chrono::milliseconds(2));
                  }
              }
          } catch (const std::exception &) {
              GetGame().NotifyThreadExceptionOccurred(std::current_exception());
          }
      }) {}

void WorldLoader::UpdateLogic(const Time dt) {
    auto & camera = GetGame().GetCamera();
    camera.Update(dt);
    const auto view = camera.GetWorldView();
    auto invView = glm::inverse(view);
    glm::vec3 eyePos = invView * glm::vec4(0, 0, 0, 1);
    auto & terrain = GetGame().GetTerrainMgr();
    terrain.UpdateChunkLOD(eyePos, camera.GetViewDir());
}

void WorldLoader::UpdateState(SceneStack & state) {
    if (GetGame().GetTerrainMgr().HasWork()) {
        state.push(std::make_shared<WorldTransitionIn>());
    }
}

void WorldLoader::Display(DisplayImpl & gfx) {
    gfx.Dispatch(*this);
}
}
