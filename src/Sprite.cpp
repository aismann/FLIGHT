#include "Sprite.hpp"

#include <glm/gtc/type_ptr.hpp>
#ifdef FLIGHT_MAC
#include <OpenGL/gl3.h>
#elif FLIGHT_WINDOWS
#include <GL/glew.h>
#endif
#include <glm/gtc/matrix_transform.hpp>

Sprite::Sprite() : m_position{}, m_rotation{}, m_scale{1, 1, 1}, m_material{} {}

void Sprite::SetMaterial(std::shared_ptr<Material> material) {
    m_material = material;
}

void Sprite::SetTexture(std::shared_ptr<Texture> texture) {
    m_texture = texture;
}

void Sprite::SetModel(std::shared_ptr<Model> model) { m_model = model; }

void Sprite::Display(const glm::mat4 & parentContext, ShaderProgram & shader) {
    if (auto texSp = m_texture.lock()) {
        shader.SetUniformInt("tex", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texSp->GetId());
    }
    auto modSp = m_model.lock();
    if (!modSp) {
        throw std::runtime_error("Sprite missing model data");
    }
    const size_t numVertices = modSp->Bind(shader);
    auto model = glm::scale(parentContext, m_scale);
    model = glm::rotate(model, m_rotation.y, {0, 1, 0});
    model = glm::rotate(model, m_rotation.z, {0, 0, 1});
    model = glm::rotate(model, m_rotation.x, {1, 0, 0});
    model = glm::translate(model, m_position);
    glm::mat4 invTransModel = glm::transpose(glm::inverse(model));
    shader.SetUniformMat4("invTransModel", invTransModel);
    shader.SetUniformMat4("model", model);
    if (auto mat = m_material.lock()) {
        shader.SetUniformFloat("material.diffuse", mat->diffuse);
        shader.SetUniformFloat("material.specular", mat->specular);
        shader.SetUniformFloat("material.shininess", mat->shininess);
    }
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const glm::vec3 & Sprite::GetRotation() const { return m_rotation; }

void Sprite::SetPosition(const glm::vec3 & position) { m_position = position; }

void Sprite::SetScale(const glm::vec3 & scale) { m_scale = scale; }

void Sprite::SetRotation(const glm::vec3 & vec) { m_rotation = vec; }

std::shared_ptr<Model> Sprite::GetModel() const { return m_model.lock(); }

AABB Sprite::GetAABB() {
    throw std::runtime_error("TODO: implement AABBs");
    return {};
}
