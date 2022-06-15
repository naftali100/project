#include "Game/StatusBar.h"


void StatusBar::update(const sf::Time& dt){
    m_starAnimation.update(dt);
}

void StatusBar::draw(sf::RenderTarget& win) const
{

    sf::Text m_status;
    m_status.setFont(FontHolder::get(Fonts::Main));
    sf::String m_text{ "score: " + std::to_string(m_score) + " lives: " };
    m_status.setString(m_text);
    m_status.setOutlineColor(sf::Color::Black);
    m_status.setOutlineThickness(2);
    m_status.setPosition(win.getView().getSize().x / 2, 30);
    win.draw(m_status);

    auto starPos = m_status.getPosition().x + m_status.getGlobalBounds().width;

    auto localStars = m_stars;
    for (int i = 0; i < m_lives; i++) {
        localStars.setPosition(starPos + (100 * (float)i), 0);
        win.draw(localStars);
    }
}
