#pragma once
#ifndef MAP_H
#define MAP_H

#include "StateComponents/Camera.h"
#include "Log.h"
#include "Random.h"
#include "Resources.h"
#include "State.h"
#include "TileMap.h"

class MapState : public State {
public:
    using State::State;

    void init() override {
        m_map.setTexture(Textures::Map);
        m_map.setTileSize({10, 10});
        m_map.setLayoutWidth(10);
        m_map.generateRandomMap();

        m_camera.setView(m_map.getMapSize());
        m_camera.setInitialView();
        m_camera.setWinRatio(m_stateManager.getWin().getSize());
        m_camera.setResizeStrategy(LatterBox);
    }

    virtual void handleEvent(const sf::Event& e) override {
        m_camera.handleEvent(e);

        if (e.type == sf::Event::MouseWheelScrolled) {
            if (e.mouseWheelScroll.delta > 0) {
                m_camera.zoomViewAt({e.mouseWheelScroll.x, e.mouseWheelScroll.y}, m_stateManager.getWin(), 0.9);
            }
            else if (e.mouseWheelScroll.delta < 0) {
                m_camera.zoomViewAt({e.mouseWheelScroll.x, e.mouseWheelScroll.y}, m_stateManager.getWin(), 1.1);
            }
        }
    }
    virtual void update(const sf::Time& dt) override {
        ImGui::Text("mouse position: %d, %d", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);

        auto layoutSize = m_map.getLayout().size();
        auto tileSize = m_map.getTileSize();
        int layoutWidth = m_map.getLayoutWidth();

        if (ImGui::Button("reset corrent ratio")) {
            m_camera.resetView();
            m_camera.fixRatio();
        }

        if (ImGui::Button("set view postion")) {
            m_camera.setViewPosition(m_stateManager.getWin().mapPixelToCoords(sf::Vector2i(100, 100)));
        }

        static int selected_id = m_camera.getResizeStrategy();
        std::array<std::string, 3> item_names{"Latter box", "Fix ration", "Resize view"};
        ImGui::Text("select resize strategy");
        ImGui::SameLine();
        if(ImGui::BeginCombo("##", item_names.at(selected_id).c_str())){
            int counter = 0;
            for(auto i: item_names){
                if(ImGui::Selectable(i.c_str(), std::move(counter == selected_id))){
                    selected_id = counter;
                }
                counter++;
            }

            ImGui::EndCombo();
        }

        m_camera.setResizeStrategy((winResizeStrategy)selected_id);

        if (ImGui::Button("recreate map")) {
            m_map.generateRandomMap();
        }

        if (ImGui::Button("exit")) {
            m_stateManager.popState();
            return;
        }
        m_camera.update(dt);
    }
    virtual void draw(sf::RenderTarget& win) const override {
        m_camera.draw(win);
        m_map.draw(win);
    }

private:
    TileMap m_map;
    Camera m_camera;
};
#endif