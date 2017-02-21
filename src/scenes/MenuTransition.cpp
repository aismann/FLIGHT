#include "../Scene.hpp"
#include "../Game.hpp"

MenuTransitionIn::MenuTransitionIn() : m_transitionTimer(0) {}

void MenuTransitionIn::UpdateLogic(const Time dt) {
    World::UpdateLogic(dt);
    m_transitionTimer += dt;
}

void MenuTransitionIn::UpdateState(SceneStack & state) {
    if (m_transitionTimer > TRANSITION_TIME) {
	state.pop();
	state.push(std::make_unique<Menu>());
    }
}

void MenuTransitionIn::Display() {
    World::Display();
    const float overlayDarkness =
	glm::smoothstep(0.f, static_cast<float>(TRANSITION_TIME),
			static_cast<float>(m_transitionTimer) / 2.f);
    DisplayShadowOverlay(overlayDarkness);
}

MenuTransitionOut::MenuTransitionOut() : m_transitionTimer(0) {}

void MenuTransitionOut::UpdateLogic(const Time dt) {
    World::UpdateLogic(dt);
    m_transitionTimer += dt;
}

void MenuTransitionOut::UpdateState(SceneStack & state) {
    if (m_transitionTimer > TRANSITION_TIME) {
	state.pop();
    }
}

void MenuTransitionOut::Display() {
    World::Display();
    const float overlayDarkness =
	0.5f - glm::smoothstep(0.f, static_cast<float>(TRANSITION_TIME),
			       static_cast<float>(m_transitionTimer) / 2.f);
    DisplayShadowOverlay(overlayDarkness);
}