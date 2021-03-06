#pragma once

#include <string>
#include <SFML/Window.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <glm/glm.hpp>
#include <stack>
#include <stdio.h>
#include <tuple>

#include "AssetManager.hpp"
#include "Camera.hpp"
#include "CollisionManager.hpp"
#include "ConfigData.hpp"
#include <FLIGHT/Entity/Entity.hpp>
#include "InputModes.hpp"
#include "LoadBlueprints.hpp"
#include "Player.hpp"
#include <FLIGHT/Scene/Scene.hpp>
#include "Sky.hpp"
#include "SmoothDTProvider.hpp"
#include <FLIGHT/Graphics/DisplayImpl.hpp>
#include "TerrainManager.hpp"

#include "UpdateCap.hpp"

#include <FLIGHT/Util/Singleton.hpp>
#ifndef ALERT
#define ALERT(str) Singleton<Game>::Instance().PushAlert(str);
#endif

namespace FLIGHT {
class Game {
    friend struct Patch;
    ConfigData m_conf;
    sf::Window m_window;
    bool m_running;
    std::unique_ptr<Camera> m_camera;
    AssetManager m_assetManager;
    CollisionManager m_collisionManager;
    Player m_player1;
    std::unique_ptr<TerrainManager> m_terrainManager;
    SkyManager m_skyManager;
    SmoothDTProvider m_smoothDTProv;
    std::unique_ptr<DisplayImpl> m_renderer;
    struct {
        std::mutex mutex;
        std::stack<std::shared_ptr<Scene>> stack;
    } m_sceneStack;
    void PollEvents();
    std::vector<std::unique_ptr<Controller>> m_unassignedGamepads;
    std::unique_ptr<Controller> m_unassignedMouseJSController;
    PlaneRegistry m_planesRegistry;
    struct {
        std::mutex mutex;
        std::vector<std::exception_ptr> excepts;
    } m_threadExceptions;
    void LogicLoop();
    void TryBindGamepad(const sf::Joystick::Identification & ident,
			const unsigned id);
    struct {
        std::recursive_mutex mutex;
        std::list<std::shared_ptr<Entity>> list;
    } m_entityList;
    time_t m_seed;
    void AutoAssignController(Player & player);
    void InitJoysticks();
    bool m_restartRequested;
    void Restart();
    struct {
        std::mutex mutex;
        std::vector<std::string> messages;
    } m_alerts;

public:
    Game();
    void Configure(const ConfigData & conf);
    void ShowAlerts();
    void SetSeed(const time_t seed);
    time_t GetSeed() const;
    void Run();
    bool IsRunning() const;
    void RequestRestart();
    AssetManager & GetAssetMgr();
    void Save();
    void SaveAndQuit();
    void RestoreFromSave();
    void RemoveSaveData();
    ConfigData & GetConf();
    PlaneRegistry & GetPlaneRegistry();
    TerrainManager & GetTerrainMgr();
    std::unique_ptr<TerrainManager> TakeTerrainManager();
    void SetTerrainManager(std::unique_ptr<TerrainManager> mgr);
    SkyManager & GetSkyMgr();
    DisplayImpl & GetRenderer();
    CollisionManager & GetCollisionMgr();
    Camera & GetCamera();
    void SetCamera(std::unique_ptr<Camera> camera);
    Player & GetPlayer1();
    void UpdateEntities(const Time dt);
    void NotifyThreadExceptionOccurred(std::exception_ptr ex);
    sf::Vector2<unsigned> GetSubwindowSize() const;
    Game(const Game &) = delete;
    template <typename T, typename... Args>
    std::shared_ptr<T> CreateSolid(Args &&... args) {
        auto solid = std::make_shared<T>(args...);
        std::lock_guard<std::recursive_mutex> lk(m_entityList.mutex);
        m_entityList.list.push_back(solid);
        m_collisionManager.AddSolid(solid);
        return solid;
    }
    void PushAlert(const std::string & message);
};

struct Patch {
    static void SubvertMacOSKernelPanics(Game & game);
    static void FixMysteriousStateGlitch(Game & game);
};
}
