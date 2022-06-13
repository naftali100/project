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
    m_OKBtn.setFunction([this]() { m_stateManager.pushState(std::make_unique<GameState>(m_stateManager)); });
    LOGV;
}


void setState::update(const sf::Time& dt)
{
    LOGV;
    m_OKBtn.update();
    ImGui::ShowDemoWindow();
    if (ImGui::Button("close state")) {
        m_stateManager.popState();
        return;
    }
    LOGV;

}

void setState::draw(sf::RenderTarget& win) const {
    win.draw(m_background);
    m_cam.draw(win);
    m_OKBtn.draw(win);
}