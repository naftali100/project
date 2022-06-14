#include "Game/States/setState.h"
#include "Game/States/GameState.h"

void setState::initBackground()
{
    m_background.setTexture(&TextureHolder::get(Textures::SkyBackground));
    m_background.setSize({ float(m_stateManager.getWin().getSize().x),
        float(m_stateManager.getWin().getSize().y) });
}

void setState::handleEvent(const sf::Event& e)
{
    LOGV << "start";
    m_OKBtn.handleEvent(e);
    LOGV << "finish";
}

void setState::init() {
    initBackground();
    LOGV;
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();

    auto btnXPos = ((float)m_stateManager.getWin().getSize().x / 2) - sf::util::getGlobalCenter(m_OKBtn).x;
    m_OKBtn.setPosition({ btnXPos, 100 });
    m_OKBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager, m_params)); });
    LOGV;
}


void setState::update(const sf::Time& dt)
{
    LOGV;
    m_OKBtn.update();
    imGui();
    
    LOGV;

}

void setState::imGui() {

    ImGui::ShowDemoWindow();
    //if (ImGui::Button("close state")) {
    //    m_stateManager.popState();
    //    return;
    //}
        ImGui::StyleColorsLight();
        auto& style = ImGui::GetStyle();
        style.WindowBorderSize = 0;
        style.FramePadding = { 20, 20 };
        style.ItemSpacing = { 20, 20 };
        style.ScrollbarSize = 20;
    if (ImGui::Begin("level params", nullptr, ImGuiWindowFlags_NoResize         |
                                              ImGuiWindowFlags_AlwaysAutoResize |
                                              ImGuiWindowFlags_NoBackground     |
                                              ImGuiWindowFlags_NoResize         |
                                              ImGuiWindowFlags_NoMove           |
                                              ImGuiWindowFlags_NoCollapse)) {

        ImGui::SliderFloat("bomb speed", &m_params.m_speed, 100, 1000);
        ImGui::SliderFloat("spawn rate", &m_params.m_spawnRate, 1, 10);
        ImGui::SliderFloat("bomb timeout", &m_params.m_bombTime, 1, 10);
        ImGui::SliderInt("color amount", &m_params.m_colors, 1, 3);
        ImGui::SliderInt("non jailed at same time", &m_params.m_maxBomb, 5, 10);
        ImGui::SliderInt("bomb to score", &m_params.m_bombToScore, 3, 10);
    }
    ImGui::End();
}

void setState::draw(sf::RenderTarget& win) const {
    win.draw(m_background);
    m_cam.draw(win);
    m_OKBtn.draw(win);
}