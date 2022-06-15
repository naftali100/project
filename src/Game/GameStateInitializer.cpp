#include "Game/States/GameStateInitializer.h"

GameStateInitializer::GameStateInitializer(StateManager& stateManager, 
    std::vector<std::unique_ptr<MovingObjects>>& moving,
    std::vector<std::unique_ptr<Jail>>& jails,
    std::vector<std::unique_ptr<Entity>>& statics,
    std::vector<std::unique_ptr<Explosion>>& explosions,
    std::vector<std::unique_ptr<Door>>& doors,
    sf::Sprite& stars,
    Animation& starAnimation,

    // components
    //CollisionHandler m_col; // currently not in use
    Camera& cam,
    std::vector<Func>& subscription,

    // game data
    int& lives,
    int& score,
    int& nonJailedBomb,
    LevelParams& params,

    Timer& spawnTimer,
    StatusBar& sb,
    sf::RectangleShape& background) 
:m_moving(moving), m_jails(jails), m_static(statics), m_explosions(explosions), m_doors(doors), m_stars(stars),
m_starAnimation(starAnimation), m_cam(cam), m_subscription(subscription), m_lives(lives), m_score(score),
m_nonJailedBomb(nonJailedBomb), m_params(params), m_spawnTimer(spawnTimer), m_sb(sb), m_background(background),
m_stateManager(stateManager) {}

void GameStateInitializer::initBackground() {
    m_background.setTexture(&TextureHolder::get(Textures::SkyBackground));
    m_background.setSize((sf::Vector2f)getWinSize());
}

void GameStateInitializer::initCamera() {
    m_cam.setView(m_stateManager.getWin().getDefaultView());
    m_cam.setInitialView();
    m_cam.setWinRatio(m_stateManager.getWin().getSize());
    m_cam.setResizeStrategy(LatterBox);
}

void GameStateInitializer::initState() {
    m_stars.setTexture(TextureHolder::get(Textures::Stars));

    static float spawnInterval = 3;
    m_spawnTimer.set(
        [this]() {
            m_spawnTimer.setTime(sf::seconds(Random::rnd(0.01f, m_params.m_spawnRate)));
            spawnBomb();
        },
        m_params.m_spawnRate);
    m_starAnimation.initFramesWithFixedSize(m_stars.getTexture()->getSize(), 1, 9, 0.1f);
    for (int _ : rng::views::iota(0, m_params.m_maxBomb)) {
        spawnBomb();
        // spawnGift();
    }
}

void GameStateInitializer::initJail() {
    auto winSize = getWinSize();
    auto jailAmount = m_params.m_colors + 1;
    auto jailSize = Jail(m_params).getSize();
    // TODO: find the right position
    sf::Vector2f startPoint{ (winSize.x - (jailSize.x * (float)jailAmount)) / 2.f, winSize.y - (jailSize.y / 2.f) };

    for (auto i : std::views::iota(0, jailAmount)) {
        addJail({ ((i + 1) * jailSize.x) + startPoint.x + 10, startPoint.y }, Colors::STD_COLORS[i]);
    }
}

void GameStateInitializer::addJail(const sf::Vector2f& pos, const sf::Color& color) {
    auto j = std::make_unique<Jail>(m_params);
    //todo: delete?
    //j->setDirection({ Random::rnd(-1.0f, 1.0f), Random::rnd(-1.0f, 1.0f) });
    //j->setSpeed(100.f);
    j->setColor(color);
    j->setOrigin(sf::util::getGlobalCenter(*j));
    j->setPosition(pos);
    m_jails.push_back(std::move(j));
}

void GameStateInitializer::initWalls() {
    auto winSize = getWinSize();

    // left wall
    addWall({ -100, -100 }, { 100, (float)winSize.y + 200 });
    // bottom wall
    addWall({ -100, (float)winSize.y }, { (float)winSize.x + 200, 100 });
    // right wall
    addWall({ (float)winSize.x, -100 }, { 100, (float)winSize.y + 200 });
    // upper wall
    addWall({ -100, -100 }, { (float)winSize.x + 200, 100 });
}

void GameStateInitializer::addWall(const sf::Vector2f& pos, const sf::Vector2f& size) {
    auto wall = std::make_unique<Wall>();
    wall->setPosition(pos);
    wall->setSize(size);
    m_static.push_back(std::move(wall));
}

void GameStateInitializer::initDoors() {
    auto winSize = getWinSize();

    m_doors.clear();

    // doors in corners for now.
    auto d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition(0, 0);
    d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition(10, (float)winSize.y - 10);
    d = std::make_unique<Door>();
    m_doors.push_back(std::move(d));
    m_doors.back()->setPosition((float)winSize.x - 10, (float)winSize.y - 10);
}

void GameStateInitializer::spawnBomb() {
    auto winSize = getWinSize();
    auto b = std::make_unique<Bomb>(m_explosions, m_params);
    b->setDirection({ Random::rnd(-1.0f, 1.0f), Random::rnd(-1.0f, 1.0f) });
    if (!m_doors.empty())
        b->setPosition(m_doors.at(Random::rnd(1, (int)m_doors.size()) - 1)->getPosition());
    else
        b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
    m_moving.push_back(std::move(b));
    m_nonJailedBomb++;
}

void GameStateInitializer::spawnGift() {
    auto winSize = getWinSize();
    auto b = std::make_unique<Gift>();
    b->setDirection({ Random::rnd(-1.0f, 1.0f), Random::rnd(-1.0f, 1.0f) });
    b->setPosition((float)Random::rnd(10, winSize.x - 10), (float)Random::rnd(10, winSize.y - 10));
    m_moving.push_back(std::move(b));
}

sf::Vector2u GameStateInitializer::getWinSize() {
    return m_stateManager.getWin().getSize();
}

void GameStateInitializer::registerMessageHandlers() {
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::BombJailed, [this]() {
            m_nonJailedBomb--;
            })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::BombTimedout, [this]() {
            m_lives--;
            m_nonJailedBomb--;
            }));
    m_subscription.push_back(
        MessageBus::subscribe<Bomb*>(MessageType::BombRemoveFromVector, [this](auto bomb) {
            bomb->kill();
            m_score++;
            }));
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::ScoreGift, [this]() {
            m_score += 5;
            })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::LiveGift, [this]() {
            m_lives++;
            })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::RemoveTerroristsGift, [this]() {
            m_nonJailedBomb = 0;
            rng::for_each(m_moving, [](auto& i) {
                if (i->getCollisionTag() == CollisionTag::bomb)
                    i->kill();
                });
            })
    );
    m_subscription.push_back(
        MessageBus::subscribe(MessageType::FreeTerroristsGift, [this]() {
            freeTerrorists();
            })
    );
}

void GameStateInitializer::freeTerrorists() const {
    for (auto const& i : m_jails)
        i->freeAll();
}
