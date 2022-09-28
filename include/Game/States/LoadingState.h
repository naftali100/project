#include "State.h"

#include "Game/Timer.h"
#include "Animation.h"

class LoadingState : public State {
public:
    LoadingState(StateManager& sm);
    void init() override;

    virtual void handleEvent(const sf::Event&) override {}
    virtual void update(const sf::Time&) override;
    virtual void draw(sf::RenderTarget&) const override;

    void loadResources();

private:
    // sf::Sprite m_loading;
    // Animation m_loadingAnimation{m_loading};

    sf::Thread m_loadingThread;

    bool m_threadFinished = false;
    sf::Mutex m_mutex;

    sf::Text m_text;

    Timer m_timer;
    sf::Sprite m_spinnerSprite;
    Animation m_spinnerAnimation{m_spinnerSprite};
};
