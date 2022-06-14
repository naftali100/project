#include "Game/States/WelcomeState.h"

void WelcomeState::init() {
	LOGV;
	m_cam.setView(m_stateManager.getWin().getDefaultView());
	m_cam.setInitialView();

	auto btnXPos = ((float)m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_btn).x;
	m_setBtn.setPosition({ btnXPos, 100 });
	m_btn.setPosition({ btnXPos, 200 });
	m_setBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<setState>(m_stateManager)); });
	m_btn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager)); });

	m_ps.setDissolutionRate(0.8);
	m_ps.setShape(Shape::CIRCLE);
	m_ps.setParticleSpeed(50.0f);
	m_ps.setGravity( -0.5f, -1.0f );            
	m_ps.setPosition(m_stateManager.getWin().getSize().x /2, m_stateManager.getWin().getSize().y + 200);

	m_gravityChangeTimer.setInterval([this](){
		m_ps.setGravity(Random::rnd<float>(-0.5, 0.5), -1);
	}, 1);
	LOGV;
}

void WelcomeState::handleEvent(const sf::Event& e) {
	LOGV << "start";
	m_btn.handleEvent(e);
	m_setBtn.handleEvent(e);
	LOGV << "finish";
}

void WelcomeState::update(const sf::Time& dt) {
	LOGV;
	m_gravityChangeTimer.update(dt);
	m_btn.update();
	m_setBtn.update();
	ImGui::ShowDemoWindow();
	if (ImGui::Button("close state")) {
		m_stateManager.popState();
		return;
	}

	int defferAmount = 10;
	static int deffer = defferAmount;
	deffer--;
	if(deffer < 0){
		m_ps.fuel<spriteParticle>(Random::rnd(3,8));
		deffer = defferAmount;
	}
	m_ps.update(dt);
	LOGV;
}

void WelcomeState::draw(sf::RenderTarget& win) const {
	m_cam.draw(win);
	auto newSize = (sf::Vector2f)m_cam.getView().getSize();
	sf::RectangleShape rec{ (sf::Vector2f)TextureHolder::get(Textures::Test).getSize() };
	rec.setTexture(&TextureHolder::get(Textures::Test));
	auto oldSize = rec.getSize();
	rec.scale(newSize.y / oldSize.y, newSize.y / oldSize.y);
	win.draw(rec);
	m_btn.draw(win);
	m_setBtn.draw(win);
	m_ps.draw(win);
}