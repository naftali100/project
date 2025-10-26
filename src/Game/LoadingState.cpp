#include "Game/States/LoadingState.h"

#include "Config.h"
#include "Game/States/WelcomeState.h"
#include "Resources.h"

#include <SFML/Audio/Music.hpp>

LoadingState::LoadingState(StateManager& sm) : State(sm), m_loadingThread(&LoadingState::loadResources, this) {
    m_loadingThread.launch();
    FontHolder::load(Fonts::Main, "resources/fonts/ttf/KlokanTechNotoSans-Regular.ttf");
    TextureHolder::load(Textures::Loading, "resources/textures/loading.png");

    m_spinnerSprite.setTexture(TextureHolder::get(Textures::Loading));
    m_spinnerSprite.scale(2, 2);
    m_spinnerSprite.setColor(sf::Color::Black);

    m_spinnerAnimation.initFramesWithFixedSize(TextureHolder::get(Textures::Loading).getSize(), 4, 12, 0.05);
    m_spinnerAnimation.setFrame(0);
    m_spinnerSprite.setPosition((m_stateManager.getWin().getSize().x - m_spinnerSprite.getGlobalBounds().width) / 2, 200);

    m_timer.setInterval(
        [&]() {
            static int dots = 1;
            std::string dotsString((dots % 4), '.');
            m_text.setString("Loading" + dotsString);
            dots++;
        },
        0.05 * 6);
}

void LoadingState::init() {
    m_text.setFont(FontHolder::get(Fonts::Main));
    m_text.setString("Loading");
    m_text.setOrigin(sf::util::getLocalCenter(m_text));
}

void loadMusic(Music::ID id, const std::string& path) {
    std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
    music->openFromFile(path);
    music->setLoop(true);
    MusicHolder::set(id, std::move(music));
}

void LoadingState::loadResources() {
    sf::Lock lock(m_mutex);

    TextureHolder::load(Textures::Terrorist, "resources/textures/bomb.png");
    TextureHolder::load(Textures::Door, "resources/textures/door-open.png");
    TextureHolder::load(Textures::Explosion, "resources/textures/explosion.png");
    TextureHolder::load(Textures::GameOver, "resources/textures/gameover.jpg");
    TextureHolder::load(Textures::Gift, "resources/textures/gift.png");
    TextureHolder::load(Textures::Jail, "resources/textures/jail.png");
    TextureHolder::load(Textures::SkyBackground, "resources/textures/sky.jpg");
    TextureHolder::load(Textures::level1, "resources/textures/level1.png");
    TextureHolder::load(Textures::level2, "resources/textures/level2.png");
    TextureHolder::load(Textures::level3, "resources/textures/level3.png");
    TextureHolder::load(Textures::level4, "resources/textures/level4.png");
    TextureHolder::load(Textures::level5, "resources/textures/level5.png");
    TextureHolder::load(Textures::level1Background, "resources/textures/level1Background.jpg");
    TextureHolder::load(Textures::level2Background, "resources/textures/level2Background.jpg");
    TextureHolder::load(Textures::level3Background, "resources/textures/level3Background.jpg");
    TextureHolder::load(Textures::level4Background, "resources/textures/level4Background.jpg");
    TextureHolder::load(Textures::level5Background, "resources/textures/level5Background.jpg");
    TextureHolder::load(Textures::Smoke, "resources/textures/smoke.png");
    TextureHolder::load(Textures::SoldierBackground, "resources/textures/bg-1.jpg");
    TextureHolder::load(Textures::Stars, "resources/textures/stars.png");
    TextureHolder::load(Textures::TerroristRunLeft, "resources/textures/trrorist-final-left-run.png");
    TextureHolder::load(Textures::TerroristRunRight, "resources/textures/trrorist-final-1.png");
    TextureHolder::load(Textures::Wall, "resources/textures/wall-final.png");
    TextureHolder::get(Textures::Wall).setRepeated(true);

    // FontHolder::load(Fonts::Main, FONT_PATH);
    FontHolder::load(Fonts::Test, "resources/fonts/ttf/KlokanTechNotoSans-Regular.ttf");

    loadMusic(Music::Background1, "resources/music/b1.ogg");
    loadMusic(Music::Background2, "resources/music/b2.ogg");
    loadMusic(Music::Background3, "resources/music/b3.ogg");
    loadMusic(Music::Background4, "resources/music/b4.ogg");

    SoundBufferHolder::load(SoundEffect::Explosion, "resources/soundEffects/explosion-1.ogg");
    SoundBufferHolder::load(SoundEffect::Gift, "resources/soundEffects/gift1.ogg");
    SoundBufferHolder::load(SoundEffect::Jailed, "resources/soundEffects/jail-sound.ogg");
    SoundBufferHolder::load(SoundEffect::Lose, "resources/soundEffects/lose1.ogg");

    m_threadFinished = true;
}

void LoadingState::update(const sf::Time& dt) {
    // all update stuff
    m_text.setPosition(sf::util::getGlobalCenter(m_spinnerSprite));
    m_timer.update(dt);
    m_spinnerAnimation.update(dt);
    // m_text.move(dt.asSeconds() * 100, 0);

    // check if load finished
    // sf::Lock lock(m_mutex);
    if (m_threadFinished) {
        m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
    }
}

void LoadingState::draw(sf::RenderTarget& win) const {
    win.draw(m_text);
    win.draw(m_spinnerSprite);
}
