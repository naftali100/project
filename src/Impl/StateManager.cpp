#include "StateManager.h"

StateManager::StateManager(sf::RenderWindow& win) : m_win(win) {
    LOGV << "state manager constructor - start";
    // registerState<DemoState>(States::Main);

    LOGV << "state manager constructor - finish";
}

void StateManager::pushState(StatePtr ptr) {
    m_states.push(std::move(ptr));
    m_states.top()->init();
}

void StateManager::replaceState(StatePtr ptr) {
    m_states.pop();
    m_states.push(std::move(ptr));
    m_states.top()->init();
}

void StateManager::popState() {
    m_shouldPop = true;
}

void StateManager::handleEvent(const sf::Event& e) {
    LOGV << "start";
    if (m_paused)
        return;

    if (!m_states.empty()) {
        m_states.top()->handleEvent(e);
    }
    else {
        LOGI << "empty states stack";
    }
    LOGV << "finish";
}

void StateManager::update(const sf::Time& td) {
    LOGV << "stateManage update - start";

    if (m_paused)
        return;

    setupImGuiWindow();

    if (!m_states.empty() && m_shouldPop) {
        m_states.pop();
        m_shouldPop = false;
    }
    if (!m_states.empty()) {
        static bool noDraw = false;
        ImGui::Checkbox("pause", &noDraw);
        if(!noDraw) m_states.top()->update(td);
    }
    else
        LOGD << "empty states stack";

    if (m_showImGuiGameWindow) {
        ImGui::End();  // end "game window"
    }

    LOGV << "stateManage update - finish";
}

void StateManager::draw(sf::RenderTarget& win) {
    if (m_paused)
        return;

    if (!m_states.empty()) {
        m_states.top()->draw(win);
    }
    else {
        LOGD << "empty states stack";
    }
}

void StateManager::pause() {
    m_paused = true;
};

void StateManager::resume() {
    m_paused = false;
};

void StateManager::stop() {
    LOGV << "state Manager stop - start";

    while (!m_states.empty()) m_states.pop();

    LOGV << "state Manager stop - finish";
}

bool StateManager::isRunning() const {
    return !m_states.empty();
}

void StateManager::setupImGuiWindow() {
    // create window with my title and flags
    // MAYBE: check how to change debug window flags
    // to use the default Debug window of ImGui use imGui::End in the beginning of your stat's update method
    // and ImGui::Begin in the end of the update
    if (m_showImGuiGameWindow) {
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
        window_flags |= ImGuiWindowFlags_NoCollapse;
        window_flags |= ImGuiWindowFlags_NoTitleBar;
        ImGui::SetNextWindowPos(sf::Vector2f(10, 10));
        if (ImGui::Begin("game window", &m_showImGuiGameWindow, window_flags)) {
            // ImGui::TextDisabled("what is this?");
            // if(ImGui::IsItemHovered())
            //     ImGui::SetTooltip(
            //         "my game state manager's default window\nyou can close this by setting show to false\nyou can add "
            //         "stuff to the window for consistant\nif you close this and add stuff they go to debug window");
        }
    }
}

sf::RenderWindow& StateManager::getWin() {
    return m_win;
}

// StatePtr StateManager::createState(States::ID stateID) {
//     auto found = m_factories.find(stateID);
//     assert(found != m_factories.end());

//     return m_factories[stateID]();
// }
