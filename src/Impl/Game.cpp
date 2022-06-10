#include "Game.h"

#include "Colors.h"
#include "Config.h"
#include "Game/States/WelcomeState.h"
#include "Resources.h"
#include "SfmlUtil.h"

//Game::Game() : m_win(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "World", sf::Style::Fullscreen), m_stateManager(m_win) {}
 Game::Game() : m_win(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "World"), m_stateManager(m_win) {}

void Game::loadResources() const {
    TextureHolder::load(Textures::Bomb, "textures/bomb.png");
    TextureHolder::load(Textures::Stars, "textures/stars.png");
    TextureHolder::load(Textures::Gift, "textures/gift.png");
    TextureHolder::load(Textures::Jail, "textures/jail.png");
    TextureHolder::load(Textures::Door, "textures/door.png");
    TextureHolder::load(Textures::Terrorist, "textures/trrorist-final-1.png");
    TextureHolder::load(Textures::Gift2, "textures/gift4.png");
    // TextureHolder::load(Textures::Explosion, "textures/explosion.png");
    TextureHolder::load(Textures::Explosion, "textures/explosion.png");
    FontHolder::load(Fonts::Main, FONT_PATH);
    FontHolder::load(Fonts::Test, "fonts/ttf/KlokanTechNotoSans-Regular.ttf");
    // SoundBufferHolder::load(SoundEffect::Main, "music/background.ogg");
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
    m_stateManager.pushState(std::make_unique<WelcomeState>(m_stateManager));

    sf::Clock clock;
    while (m_stateManager.isRunning()) {
        LOGV << "game loop - start";
        processEvents();
        update(clock.restart());
        showStatWin();
        draw();
        LOGV << "game loop - finish";
    }
    if (m_win.isOpen())
        m_win.close();

    ImGui::SFML::Shutdown();

    LOGV << "game::run - finish";
}

void Game::processEvents() {
    LOGV << "handleEvent - start";
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
            default:
                break;
        }
    }
    LOGV << "handleEvent - finish";
}

void Game::update(sf::Time deltaTime) {
    LOGV << "start";
    ImGui::SFML::Update(m_win, deltaTime);

    m_stateManager.update(deltaTime);
    LOGV << "finish";
}

void Game::showStatWin() const {
    static bool open = true;
    // IF_PLOG(plog::verbose) {
        if (open) {
            if (ImGui::Begin("stat window", &open)) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
                ImGui::Text("mouse position: %d, %d", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
                // add more useful debug info
            }
            ImGui::End();
        }
    // }
}

void Game::draw() {
    LOGV << "game render - start";

    m_win.clear(Colors::Gray);
    m_stateManager.draw(m_win);

    ImGui::SFML::Render(m_win);

    m_win.display();

    LOGV << "game render - finish";
}
