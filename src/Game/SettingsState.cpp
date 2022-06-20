#include "Game/States/SettingsState.h"

#include "Game/States/GameState.h"
#include "Game/States/WelcomeState.h"

void SettingsState::init() {
    LOGV;
    initBackground();
    auto btnXPos = ((float)m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_OKBtn).x;
    m_OKBtn.setPosition({btnXPos, 100});

    m_OKBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager, m_params, m_texture)); });
    LOGV;
}

void SettingsState::initBackground() {
    m_background.setTexture(&TextureHolder::get(Textures::SkyBackground));
    m_background.setSize({float(m_stateManager.getWin().getSize().x), float(m_stateManager.getWin().getSize().y)});
}

void SettingsState::handleEvent(const sf::Event& e) {
    LOGV << "start";
    m_OKBtn.handleEvent(e);
    LOGV << "finish";
}

void SettingsState::update(const sf::Time& dt) {
    LOGV;
    m_OKBtn.update();
    imGui();

    LOGV;
}

void SettingsState::setting(const std::string& title, const std::string& explanation, int* p, float min, float max){
    ImGui::BeginGroup();
    ImGui::Text(title.c_str());
    auto id = "##" + title;
    ImGui::SliderInt(id.c_str(), p, min, max);
    ImGui::EndGroup();
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(explanation.c_str());
    }
}


void SettingsState::level(const sf::Sprite& s, LevelParams p, const Textures::ID& texture){
    if (ImGui::ImageButton(s, ImVec2({ 100, 100 }))) {
        m_params = p;
        m_texture = texture;;
    }
}

void SettingsState::imGui() {
    if (ImGui::Begin("level params##settingsState", nullptr, ImGuiWindowFlags_AlwaysAutoResize |
                                              ImGuiWindowFlags_NoBackground     |
                                              ImGuiWindowFlags_NoCollapse       |
                                              ImGuiWindowFlags_NoMove           |
                                              ImGuiWindowFlags_NoTitleBar       |
                                              ImGuiWindowFlags_NoResize))
    {
        sf::Sprite level1, level2, level3, level4, level5;
        level1.setTexture(TextureHolder::get(Textures::level1));
        level2.setTexture(TextureHolder::get(Textures::level2));
        level3.setTexture(TextureHolder::get(Textures::level3));
        level4.setTexture(TextureHolder::get(Textures::level4));
        level5.setTexture(TextureHolder::get(Textures::level5));

        level(level1, LevelParams(300, 4, 15, 1, 0, 10, 10, 10, 3, 3), Textures::level1Background);
        ImGui::SameLine();
        level(level2, LevelParams(500, 4, 20, 2, 0, 9, 9, 8, 5, 5), Textures::level2Background);
        ImGui::SameLine();
        level(level3, LevelParams(600, 3, 20, 2, 0, 8, 8, 6, 7, 7), Textures::level3Background);
        ImGui::SameLine();
        level(level4, LevelParams(600, 3, 25, 3, 0, 7, 7, 4, 9, 9), Textures::level4Background);
        ImGui::SameLine();
        level(level5, LevelParams(800, 2, 25, 3, 0, 6, 6, 3, 10, 10), Textures::level5Background);

        setting("bomb speed", "this is the speed in which the bomb will move", &m_params.m_speed, 100, 1000);
        setting("spawn rate", "the maximum possible amount of time to pass before new bomb is spawned", &m_params.m_bombSpawnRate, 1, 10);
        setting("bomb timeout", "bomb inital timeout timer", &m_params.m_bombTime, 1, 10);
        setting("color amount", "amount of different bomb and jail colors", &m_params.m_colors, 2, 4);
        setting("non jailed at same time", "max amount of non jailed bomb on the board at the same time", &m_params.m_maxTerrorist, 5, 10);
        setting("bomb to score", "how much bomb you need to jail before they clean and you get the score", &m_params.m_bombToScore, 3, 10);

        auto size = ImGui::GetWindowSize();
        ImGui::SetWindowPos({m_stateManager.getWin().getView().getSize().x / 2 - size.x / 2, 300});
    }
    ImGui::End();
    if (ImGui::Button("go to welcome state")) {
        m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
    }
}

void SettingsState::draw(sf::RenderTarget& win) const {
    win.draw(m_background);
    m_OKBtn.draw(win);
}