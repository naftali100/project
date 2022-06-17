#include "Game/States/SettingsState.h"

#include "Game/States/GameState.h"
#include "Game/States/WelcomeState.h"
#include "imgui/imgui_internal.h"

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
        // option A
        ImGui::BeginGroup();
        ImGui::Text("bomb speed");
        ImGui::SliderInt("##", &m_params.m_speed, 100, 1000);
        ImGui::EndGroup();
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("this is the speed in which the bomb will move");
        }

        // option B
        ImGui::Text("spawn rate:");
        ImGui::SameLine();
        ImGui::SliderInt("##spawn rate", &m_params.m_bombSpawnRate, 1, 10);
        if (ImGui::IsItemHovered()) {  // cloud be after text too
            ImGui::SetTooltip("the maximum possible amount of time to pass before new bomb is spawned");
        }

        // option C
        ImGui::SliderInt("bomb timeout", &m_params.m_bombTime, 1, 10);

        ImGui::Text("color amount");
        ImGui::SameLine();
        ImGui::SliderInt("##color amount", &m_params.m_colors, 1, 3);
        ImGui::Text("non jailed at same time");
        ImGui::SameLine();
        ImGui::SliderInt("##non jailed at same time", &m_params.m_maxBomb, 5, 10);
        ImGui::Text("bomb to score");
        ImGui::SameLine();
        ImGui::SliderInt("##bomb to score", &m_params.m_bombToScore, 3, 10);

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