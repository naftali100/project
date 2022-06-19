#include "Game/States/SettingsState.h"

#include "Game/States/GameState.h"
#include "Game/States/WelcomeState.h"

void SettingsState::init() {
    LOGV;
    initBackground();
    auto btnXPos = ((float)m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_OKBtn).x;
    m_OKBtn.setPosition({btnXPos, 100});

    m_OKBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager, m_params)); });
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

void SettingsState::imGui() {
    ImGui::ShowDemoWindow();
    // if (ImGui::Button("close state")) {
    //     m_stateManager.popState();
    //     return;
    // }
    //  ImGui::SetNextWindowPos({500, 500});
    if (ImGui::Begin("level params##settingsState", nullptr, ImGuiWindowFlags_AlwaysAutoResize |
                                              ImGuiWindowFlags_NoBackground     |
                                              ImGuiWindowFlags_NoCollapse       |
                                              ImGuiWindowFlags_NoMove           |
                                              ImGuiWindowFlags_NoTitleBar       |
                                              ImGuiWindowFlags_NoResize))
    {
        setting("bomb speed", "this is the speed in which the bomb will move", &m_params.m_speed, 100, 1000);
        setting("spawn rate", "the maximum possible amount of time to pass before new bomb is spawned", &m_params.m_bombSpawnRate, 1, 10);
        setting("bomb timeout", "bomb inital timeout timer", &m_params.m_bombTime, 1, 10);
        setting("color amount", "amount of different bomb and jail colors", &m_params.m_colors, 1, 3);
        setting("non jailed at same time", "max amount of non jailed bomb on the board at the same time", &m_params.m_maxBomb, 5, 10);
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