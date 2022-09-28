#include "Game/States/LoadingState.h"

#include "Config.h"
#include "Game/States/WelcomeState.h"
#include "Resources.h"

LoadingState::LoadingState(StateManager& sm) : State(sm), m_loadingThread(&LoadingState::loadResources, this) {
    m_loadingThread.launch();
    FontHolder::load(Fonts::Main, FONT_PATH);

    m_timer.setInterval(
        [&]() {
            static int dots = 0;
            std::string dotsString((dots % 4), '.');
            m_text.setString("Loading" + dotsString);
            dots++;
        },
        0.2);
}

void loadMusic(Music::ID id, const std::string& path) {
    std::unique_ptr<sf::Music> music = std::make_unique<sf::Music>();
    music->openFromFile(path);
    music->setLoop(true);
    MusicHolder::set(id, std::move(music));
}

void LoadingState::loadResources() {
    TextureHolder::load(Textures::Terrorist, "textures/bomb.png");
    TextureHolder::load(Textures::Door, "textures/door-open.png");
    TextureHolder::load(Textures::Explosion, "textures/explosion.png");
    TextureHolder::load(Textures::GameOver, "textures/gameover.jpg");
    TextureHolder::load(Textures::Gift, "textures/gift.png");
    TextureHolder::load(Textures::Jail, "textures/jail.png");
    TextureHolder::load(Textures::SkyBackground, "textures/sky.jpg");
    TextureHolder::load(Textures::level1, "textures/level1.png");
    TextureHolder::load(Textures::level2, "textures/level2.png");
    TextureHolder::load(Textures::level3, "textures/level3.png");
    TextureHolder::load(Textures::level4, "textures/level4.png");
    TextureHolder::load(Textures::level5, "textures/level5.png");
    TextureHolder::load(Textures::level1Background, "textures/level1Background.jpg");
    TextureHolder::load(Textures::level2Background, "textures/level2Background.jpg");
    TextureHolder::load(Textures::level3Background, "textures/level3Background.jpg");
    TextureHolder::load(Textures::level4Background, "textures/level4Background.jpg");
    TextureHolder::load(Textures::level5Background, "textures/level5Background.jpg");
    TextureHolder::load(Textures::Smoke, "textures/smoke.png");
    TextureHolder::load(Textures::SoldierBackground, "textures/bg-1.jpg");
    TextureHolder::load(Textures::Stars, "textures/stars.png");
    TextureHolder::load(Textures::TerroristRunLeft, "textures/trrorist-final-left-run.png");
    TextureHolder::load(Textures::TerroristRunRight, "textures/trrorist-final-1.png");
    TextureHolder::load(Textures::Wall, "textures/wall-final.png");
    TextureHolder::get(Textures::Wall).setRepeated(true);

    // FontHolder::load(Fonts::Main, FONT_PATH);
    FontHolder::load(Fonts::Test, "fonts/ttf/KlokanTechNotoSans-Regular.ttf");

    loadMusic(Music::Background1, "music/b1.ogg");
    loadMusic(Music::Background2, "music/b2.ogg");
    loadMusic(Music::Background3, "music/b3.ogg");
    loadMusic(Music::Background4, "music/b4.ogg");

    SoundBufferHolder::load(SoundEffect::Explosion, "soundEffects/explosion-1.ogg");
    SoundBufferHolder::load(SoundEffect::Gift, "soundEffects/gift1.ogg");
    SoundBufferHolder::load(SoundEffect::Jailed, "soundEffects/jail-sound.ogg");
    SoundBufferHolder::load(SoundEffect::Lose, "soundEffects/lose1.ogg");

    sf::Lock lock(m_mutex);
    m_threadFinished = true;
}

void LoadingState::update(const sf::Time& dt) {
    // all update stuff
    m_timer.update(dt);
    // m_text.move(dt.asSeconds() * 100, 0);

    // check if load finished
    sf::Lock lock(m_mutex);
    if (m_threadFinished) {
        m_stateManager.replaceState(std::make_unique<WelcomeState>(m_stateManager));
    }
}

void LoadingState::init() {
    m_text.setFont(FontHolder::get(Fonts::Main));
    m_text.setString("Loading");
    m_text.setPosition(500, 500);
}

void LoadingState::draw(sf::RenderTarget& win) const {
    win.draw(m_text);
}
