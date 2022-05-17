#include "Game.h"


#include "Colors.h"
#include "Config.h"
#include "SfmlUtil.h"
#include "States/MainMenu.h"
#include "Resources.h"

Game::Game() : m_win(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "World"), m_stateManager(m_win) {}

void Game::loadResources() {
    TextureHolder::load(Textures::Player, "textures/player.png");
    TextureHolder::load(Textures::FireAnimationSheet, "textures/fireSheet.png");
    TextureHolder::load(Textures::Run, "textures/run.png");
    TextureHolder::load(Textures::Map, "textures/map1.png");
    FontHolder::load(Fonts::Main, FONT_PATH);
    FontHolder::load(Fonts::Test, "fonts/ttf/KlokanTechNotoSans-Regular.ttf");
    SoundBufferHolder::load(SoundEffect::Main, "music/background.ogg");
}

void Game::initImGui() {
    // init ImGui
    bool ImGuiInit = ImGui::SFML::Init(m_win, false);
    ImGui::GetIO().Fonts->Clear();
    // TODO: check imgui font range and load hebrew range 0x0590 - 0x05FF
    ImGui::GetIO().Fonts->AddFontFromFileTTF("fonts/ttf/KlokanTechNotoSans-Regular.ttf", 25.f);
    bool i = ImGui::SFML::UpdateFontTexture();
    // set my style for ImGui
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 12;
    style.FrameRounding = 4;
    style.ScrollbarRounding = 12;
    style.GrabRounding = 12;
}

void Game::run() {
    LOGV << "game::run - start";

    loadResources();
    initImGui();

    m_win.setFramerateLimit(FPS);
    m_win.setKeyRepeatEnabled(false);

    // initial state
    m_stateManager.pushState(std::make_unique<MainMenu>(m_stateManager));

    sf::Clock clock;
    while (m_stateManager.isRunning()) {
        processEvents();
        sf::Time deltaTime = clock.restart();
        update(deltaTime);
        showStatWin();
        draw();
    }
    if (m_win.isOpen())
        m_win.close();

    ImGui::SFML::Shutdown();

    LOGV << "game::run - finish";
}

void Game::processEvents() {
    sf::Event event;
    while (m_win.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(m_win, event);
        m_stateManager.handleEvent(event);

        switch (event.type) {
            case sf::Event::Resized:
                {  // update the view to the new size of the window
                    sf::FloatRect visibleArea(0.f, 0.f, event.size.width, event.size.height);
                    m_win.setView(sf::View(visibleArea));
                    break;
                }
            case sf::Event::Closed:
                m_win.close();
                m_stateManager.stop();
                break;
                // case sf::Event::LostFocus:
                //     m_stateManager.pause();
                //     break;
                // case sf::Event::GainedFocus:
                //     m_stateManager.resume();
                //     break;
        }
    }
}

void Game::update(sf::Time deltaTime) {
    ImGui::SFML::Update(m_win, deltaTime);

    m_stateManager.update(deltaTime);
}

void Game::showStatWin() {
    static bool open = true;
    if (open) {
        if (ImGui::Begin("stat window", &open)) {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}

void Game::draw() {
    LOGV << "game render - start";

    m_win.clear(Colors::Gray);
    m_stateManager.draw(m_win);

    ImGui::SFML::Render(m_win);

    m_win.display();

    LOGV << "game render - finish";
}
