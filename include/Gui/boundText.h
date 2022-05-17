#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Resources.h"
#include "RoundedRectangle.h"

namespace gui {
class BoundText {
public:
    BoundText(const sf::String& text = "", const sf::Vector2f& pos = sf::Vector2f(),
              const sf::Color& txt_color = sf::Color::Black, const sf::Color& bound_color = sf::Color::White,
              const int bound_outline_size = 0, const sf::Color& bound_outline_color = sf::Color::Blue) :
        m_text(text),
        m_txt_color(txt_color),
        m_bound_color(bound_color),
        m_bound_outline_color(bound_outline_color),
        m_bound_outline_size(bound_outline_size) {
        // setup rec
        m_background.setCornerPointCount(30);
        m_background.setCornersRadius(15);
        m_background.setFillColor(m_bound_color);
        m_background.setOutlineColor(m_bound_outline_color);
        m_background.setOutlineThickness(m_bound_outline_size);
        m_background.setPosition(pos);

        // setup text
        m_btnText.setString(text);
        m_btnText.setFont(FontHolder::get(Fonts::Main));
        
        updateBound();
    }
    virtual ~BoundText() = default;

    void setPosition(const sf::Vector2f& pos) {
        m_background.setPosition(pos);
        updateBound();
    }

    sf::FloatRect getGlobalBounds() const {
        return m_background.getGlobalBounds();
    };

    void setOutline(int size, const sf::Color& color) {
        m_bound_outline_size = size;
        m_bound_outline_color = color;
    }

    void setText(const sf::String& s) {
        m_btnText.setString(s);
        updateBound();
    }

    void handleChanges() {
        updateBound();
    }

    void draw(sf::RenderTarget& win) const {
        win.draw(m_background);
        win.draw(m_btnText);
    }

protected:
    virtual void updateBound() {
        m_btnText.setString(m_text);
        m_btnText.setFillColor(m_txt_color);
        sf::FloatRect text_bound = m_btnText.getLocalBounds();
        float char_size = m_btnText.getCharacterSize();
        //                          weird fix text wired offset - make sure is in (0,0)
        //                          add nice padding from up-right corner
        m_btnText.setPosition(m_background.getPosition() + sf::Vector2f(char_size / 5, (char_size / text_bound.height)));
        
        int linesInText = std::count(m_text.begin(), m_text.end(), '\n') + 1;
        auto boundHeight = linesInText * (m_btnText.getCharacterSize() + 10.5);

        // set btn size relative to text size // text size x // add padding
        m_background.setSize(sf::Vector2f(m_btnText.getGlobalBounds().width + ((m_btnText.getCharacterSize() / 3) * 2), boundHeight));
    }

    sf::String m_text;
    sf::FloatRect m_bound;
    sf::Color m_txt_color;
    sf::Color m_bound_color;
    sf::Color m_bound_outline_color;
    int m_bound_outline_size;

    sf::RoundedRectangleShape m_background;
    sf::Text m_btnText;
};

}  // namespace gui