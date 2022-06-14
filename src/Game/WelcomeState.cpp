#include "Game/States/WelcomeState.h"

void WelcomeState::init() {
	LOGV;
	m_cam.setView(m_stateManager.getWin().getView());
	initBackground();

	auto btnXPos = (((float)m_stateManager.getWin().getSize().x / 3) * 2.5f) - sf::util::getGlobalCenter(m_btn).x;
	auto btnYPos = (((float)m_stateManager.getWin().getSize().y / 2)) - sf::util::getGlobalCenter(m_btn).y;

	m_btn.setPosition({ btnXPos, btnYPos + 100 });
	m_setBtn.setPosition({ btnXPos, btnYPos + 200 });
	m_setBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<SettingsState>(m_stateManager)); });
	m_btn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager)); });

	m_ps.setDissolutionRate(0.8);
	m_ps.setShape(Shape::CIRCLE);
	m_ps.setParticleSpeed(50.0f);
	m_ps.setGravity( -0.5f, -1.0f );            
	m_ps.setPosition(m_stateManager.getWin().getSize().x / 2, m_stateManager.getWin().getSize().y + 200);

	m_ps1 = m_ps;
	m_ps1.setPosition(m_stateManager.getWin().getSize().x / 3, m_stateManager.getWin().getSize().y + 200);

	m_gravityChangeTimer.setInterval([this](){
		m_ps.setGravity(Random::rnd<float>(-0.5, 0.5), -1);
		m_ps1.setGravity(Random::rnd<float>(-0.5, 0.5), -1);
	}, 1);
	LOGV;
}

void WelcomeState::initBackground(){
	m_background.setSize((sf::Vector2f)TextureHolder::get(Textures::Test).getSize());
	m_background.setTexture(&TextureHolder::get(Textures::Test));
	// scale to window size
	auto oldSize = m_background.getSize();
	auto newSize = (sf::Vector2f)m_stateManager.getWin().getView().getSize();
	m_background.scale(newSize.y / oldSize.y, newSize.y / oldSize.y);
}

void WelcomeState::handleEvent(const sf::Event& e) {
	LOGV << "start";
	m_btn.handleEvent(e);
	m_setBtn.handleEvent(e);
	LOGV << "finish";
}

void WelcomeState::update(const sf::Time& dt) {
	LOGV;
	m_cam.update(dt);
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
		m_ps1.fuel<spriteParticle>(Random::rnd(3,8));
		deffer = defferAmount;
	}
	m_ps.update(dt);
	m_ps1.update(dt);
	LOGV;
}

void WelcomeState::draw(sf::RenderTarget& win) const {
	m_cam.draw(win);
	win.draw(m_background);
	m_btn.draw(win);
	m_setBtn.draw(win);
	m_ps.draw(win);
	m_ps1.draw(win);
}