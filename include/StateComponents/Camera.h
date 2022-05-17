#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <imgui.h>
#include <SFML/Graphics/View.hpp>

enum winResizeStrategy
{
    LatterBox,  // change the viewport to math new window size
    FixRatio,   // change the view size while keeping the original ration
    ResizeView  // reset the view to match the new window size as is
};

// kinda sub state that update the view
class Camera {
public:
    /////////////////////////////////////
    // SETTERS
    // - view
    // - reset strategy
    // - window ratio
    /////////////////////////////////////

    // save the current view as default fallback size
    void setInitialView() {
        m_initialView = m_view;
    }
    void setView(const sf::View& view) {
        m_view = view;
    }
    void setView(const sf::Vector2u& size) {
        m_view = sf::View(sf::FloatRect(0.0f, 0.0f, (float)size.x, (float)size.y));
    }
    void setView(const float x, const float y) {
        m_view = sf::View(sf::FloatRect(0.0f, 0.0f, x, y));
    }
    void setView(const sf::FloatRect& rect) {
        m_view = sf::View(rect);
    }

    void setWinRatio(const sf::Vector2u& winSize) {
        m_windowRatio = (float)winSize.x / (float)winSize.y;
    }
    void setWinRatio(const float ratio) {
        m_windowRatio = ratio;
    }
    void setResizeStrategy(const winResizeStrategy& s) {
        m_resizeStrategy = s;
    }

    // reset view to default - manually set by setInitialView
    // @see setInitialView
    void resetView() {
        m_view = m_initialView;
    }

    //////////////////////////////////
    // GETERS
    //////////////////////////////////

    winResizeStrategy getResizeStrategy() const {
        return m_resizeStrategy;
    }

    sf::View& getView() {
        return m_view;
    }
    const sf::View& getView() const {
        return m_view;
    }

    //////////////////////////////////////////////
    // handle events, update, draw
    //////////////////////////////////////////////

    virtual void handleEvent(const sf::Event& e) {
        switch (e.type) {
            case sf::Event::Resized:
                m_windowRatio = (float)e.size.width / (float)e.size.height;

                switch (m_resizeStrategy) {
                    // crop viewport to match view ratio
                    case LatterBox:
                        resetView();
                        m_view = getLetterboxView(m_view);
                        break;
                    // fix view width to match window's ration
                    case FixRatio:
                        fixRatio();
                        break;
                    // strictly resize the view size to new window size
                    case ResizeView:
                        m_view.setSize(e.size.width, e.size.height);
                }

                break;
        }
    }

    virtual void update(const sf::Time& dt) {
        float cameraSpeed = 100 * dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            m_view.move(0, cameraSpeed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            m_view.move(0, -cameraSpeed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            m_view.move(cameraSpeed, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            m_view.move(-cameraSpeed, 0);
        }

        float zoomSpeed = 1 * dt.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            m_view.zoom(1 - zoomSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            m_view.zoom(1 + zoomSpeed);
        }

        ImGui::Text("camera view size: %f, %f", m_view.getSize().x, m_view.getSize().y);
        float sizeX, sizeY;
        sizeX = m_view.getSize().x;
        sizeY = m_view.getSize().y;
        ImGui::SliderFloat("camera size x", &sizeX, 0, 1000, NULL);
        ImGui::SliderFloat("camera size y", &sizeY, 0, 1000, NULL);
        m_view.setSize(sizeX, sizeY);

        ImGui::Text("camera view center: %f, %f", m_view.getCenter().x, m_view.getCenter().y);
        float centerX, centerY;
        centerX = m_view.getCenter().x;
        centerY = m_view.getCenter().y;
        ImGui::SliderFloat("camera center x", &centerX, 0, 1000, NULL);
        ImGui::SliderFloat("camera center y", &centerY, 0, 1000, NULL);
        m_view.setCenter(centerX, centerY);

        ImGui::Separator();

        static bool pinViewToWindowCorenr = true;
        ImGui::Checkbox("pin view up-left corner to window's 0,0", &pinViewToWindowCorenr);
        if (pinViewToWindowCorenr) {
            auto size = m_view.getSize();
            sf::Vector2f halfSize{size.x / 2, size.y / 2};
            m_view.setCenter(halfSize);
        }

        ImGui::Text("win ratio: %f", m_windowRatio);
    }

    // set the view to window
    virtual void draw(sf::RenderTarget& win) const {
        win.setView(m_view);
    }

    ////////////////////////////////////////
    // UTILITES
    // for varios operations on views
    ////////////////////////////////////////

    void zoomViewAt(sf::Vector2i pixel, sf::RenderTarget& window, float zoom) {
        const sf::Vector2f beforeCoord{window.mapPixelToCoords(pixel)};
        sf::View view{window.getView()};
        view.zoom(zoom);
        window.setView(view);
        const sf::Vector2f afterCoord{window.mapPixelToCoords(pixel)};
        const sf::Vector2f offsetCoords{beforeCoord - afterCoord};
        view.move(offsetCoords);
        window.setView(view);
        m_view = window.getView();
    }

    ///////////////////////////////////////////
    // resize strategy functions
    ///////////////////////////////////////////

    void fixRatio() {
        resetView();
        auto size = m_view.getSize();
        m_view.setSize(size.x * m_windowRatio, size.y);
    }

    sf::View getLetterboxView(sf::View view) {
        // Compares the aspect ratio of the window to the aspect ratio of the view,
        // and sets the view's viewport accordingly in order to archieve a letterbox effect.
        // A new view (with a new viewport set) is returned.

        float windowRatio = m_windowRatio;
        float viewRatio = view.getSize().x / (float)view.getSize().y;
        float sizeX = 1, sizeY = 1, posX = 0, posY = 0;

        bool horizontalSpacing = true;
        if (windowRatio < viewRatio)
            horizontalSpacing = false;

        // If horizontalSpacing is true, the black bars will appear on the left and right side.
        // Otherwise, the black bars will appear on the top and bottom.

        if (horizontalSpacing) {
            sizeX = viewRatio / windowRatio;
            posX = (1 - sizeX) / 2.f;
        }
        else {
            sizeY = windowRatio / viewRatio;
            posY = (1 - sizeY) / 2.f;
        }

        view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
        return view;
    }

    // WIP
    void setViewPosition(const sf::Vector2f& pos) {
        auto size = m_view.getSize();
        m_view.setCenter(size.x / 2, size.y / 2);
        m_view.move(pos);
    }

private:
    sf::View m_initialView;
    sf::View m_view;
    winResizeStrategy m_resizeStrategy;
    float m_windowRatio;
};

#endif