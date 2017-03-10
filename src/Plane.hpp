#pragma once

#include "AssetManager.hpp"
#include "Blueprint.hpp"
#include "ColorMix.hpp"
#include "Entity.hpp"
#include "GameMath.hpp"
#include "Sprite.hpp"
#include "TerrainManager.hpp"
#include "Time.hpp"

namespace FLIGHT {
class Plane : public DynamicSolidPreallocMBS, public ColorMixDecay {
protected:
    std::vector<Sprite> m_parts;
    glm::vec3 m_direction;
    float m_pitch;
    float m_roll;
    float m_thrust;
    float m_yVelocity;
    void MessageLoop();

public:
    constexpr static float GetElevationLimit() { return 50.f; }
    Plane();
    Plane(const Blueprint & blueprint);
    void Display(ShaderProgram & shader) override;
    void Update(const Time dt) override;
    const glm::vec3 & GetDirection() const;
    void SetDirection(const glm::vec3 & direction);
    void SetPitch(const float);
    float GetPitch() const;
    void SetRoll(const float);
    float GetRoll() const;
    void SetThrust(const float);
    float GetThrust() const;
    AABB GetAABB() override;
    OBB GetOBB() override;
};
}
