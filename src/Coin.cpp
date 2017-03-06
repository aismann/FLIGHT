#include "Coin.hpp"
#include "Plane.hpp"
#include "Game.hpp"

namespace FLIGHT {
Coin::Coin(const glm::vec3 & position) : m_timer(0) {
    m_position = position;
    m_model = GetGame().GetAssetMgr().GetModel<ModelId::Box>();
}
    
void Coin::Display(ShaderProgram & shader) {
    if (auto modelSp = m_model.lock()) {
	auto binding = modelSp->Bind(shader);
	glm::mat4 model;
	model = glm::translate(model, m_position);
	shader.SetUniformMat4("model", model);
	shader.SetUniformVec4("color", {0.949f, 0.765f, 0.027f, 1.f});
	glDrawArrays(GL_TRIANGLES, 0, binding.numVertices);
    }
}

void Coin::Update(const Time dt) {
    m_timer += dt;
    static const double PI = 3.1415926535;
    const float offset = 0.0025 * sinf(2 * PI * 0.0000005 * m_timer);
    m_position.y = m_position.y + offset;
}

void Coin::OnCollide(Solid & other) {
    if (dynamic_cast<Plane *>(&other)) {
        SetDeallocFlag();
    }
}

AABB Coin::GetAABB() { return {m_position, {
	    m_position.x + 1, m_position.y + 1, m_position.z + 1
	}}; }
}
