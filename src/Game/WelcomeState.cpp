#include "Game/States/WelcomeState.h"

#include "EnumPrint.h"

void WelcomeState::init() {
	LOGV;
	static bool firstTime = true;
	if(firstTime){
		firstTime = false;
		if(MusicHolder::get(Music::Background1).getStatus() != sf::Music::Playing){
			MusicHolder::get(Music::Background1).play();
		}
	}

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
	m_background.setSize((sf::Vector2f)TextureHolder::get(Textures::SoldierBackground).getSize());
	m_background.setTexture(&TextureHolder::get(Textures::SoldierBackground));
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
	m_gravityChangeTimer.update(dt);
	m_btn.update();
	m_setBtn.update();

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

	// change music
	static int selected = 0;
	for(int i = 0; i < Music::Count; i++){
		std::string label = std::string(magic_enum::enum_name((Music::ID)i));
		int oldSelected = selected;
		if(ImGui::Selectable(label.c_str(), selected == i)){
			selected = i;
			MusicHolder::get((Music::ID)oldSelected).stop();
			MusicHolder::get((Music::ID)selected).play();
		}
	}
	static int musicVolume = 50;
	ImGui::SliderInt("music volume", &musicVolume, 1, 100);
	MusicHolder::get((Music::ID)selected).setVolume(musicVolume);
	if(ImGui::Button("exit")){
		m_stateManager.stop();
	}

	LOGV;
}

void WelcomeState::draw(sf::RenderTarget& win) const {
	win.draw(m_background);
	m_btn.draw(win);
	m_setBtn.draw(win);
	m_ps.draw(win);
	m_ps1.draw(win);
}