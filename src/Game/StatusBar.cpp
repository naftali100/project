#include "Game/StatusBar.h"

void StatusBar::draw(sf::RenderTarget& win) const
{
    sf::Text m_status;
    m_status.setFont(FontHolder::get(Fonts::Main));
    sf::String m_text{ "lives: " };
    m_text += std::to_string(m_lives);
    m_text += " score: " + std::to_string(m_score);
    m_status.setString(m_text);
    m_status.setPosition(win.getView().getSize().x / 2, 0);
    win.draw(m_status);
}
