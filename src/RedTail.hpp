#pragma once

#include "Plane.hpp"

namespace FLIGHT {   
    class RedTail : public Plane {
	Sprite m_fuselage;
    public:
	RedTail();
	AABB GetAABB() override;
	void Display(ShaderProgram & shader) override;
    };
}
