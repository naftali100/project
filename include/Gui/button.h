#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "Colors.h"
#include "boundText.h"

namespace gui {
// enum class ButtonSize
// {
//     Small,
//     Wide,
// };

class Button : public BoundText {
public:
    Button(const sf::String& text = "btn text") : BoundText(text, {}, Colors::White, Colors::Blue) {}

    void setFunction(std::function<void(void)> func) {
        m_cbFunction = func;
    }

    void update() {
        if (m_isHovered) {
            m_background.setFillColor(Colors::Blue);
        }
        else {
            m_background.setFillColor(Colors::Primary);
        }
    }

    void handleEvent(const sf::Event& e) {
        switch (e.type) {
            case sf::Event::MouseMoved:
                m_isHovered = m_background.getGlobalBounds().contains(e.mouseMove.x, e.mouseMove.y);
                break;
            case sf::Event::MouseButtonReleased:
                if (m_isHovered) {
                    m_cbFunction();
                }
                break;
            default:
                break;
        }
    }

    operator bool() {
        return m_isClicked;
    }

private:
    sf::Text m_text;
    std::function<void(void)> m_cbFunction = []() {};
    bool m_isHovered = false;
    bool m_isClicked = false;
};
}  // namespace gui