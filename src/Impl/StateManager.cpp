#include "StateManager.h"

void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("what is this");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

StateManager::StateManager(sf::RenderWindow& win) : m_win(win) {
    LOGV << "state manager constructor - start";
    // registerState<DemoState>(States::Main);

    LOGV << "state manager constructor - finish";
}

void StateManager::pushState(StatePtr ptr, bool pauseCurrent) {
    if (!pauseCurrent && !m_states.empty()) {
        m_states.pop();
    }
    m_states.push(std::move(ptr));
    m_states.top()->init();
}

void StateManager::replaceState(StatePtr ptr){
    m_states.pop();
    m_states.push(std::move(ptr));
    m_states.top()->init();
}

void StateManager::popState() {
    if (!m_states.empty())
        m_states.pop();
}

void StateManager::handleEvent(const sf::Event& e) {
    if (m_paused)
        return;

    if (!m_states.empty()) {
        m_states.top()->handleEvent(e);
    }
    else {
        LOGI << "empty states stack";
    }
}

void StateManager::update(const sf::Time& td) {
    LOGV << "stateManage update - start";

    if (m_paused)
        return;

    // create window with my title and flags
    // MAYBE: check how to change debug window flags
    // to use the defult Debug window of ImGui use imGui::End in the begining of your stat's update method
    // and ImGui::Begin in the end of the update
    static bool shouldEnd = m_showImGuiGameWindow;
    if (m_showImGuiGameWindow) {
        shouldEnd = true;
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoResize;
        window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
        // ImGui::SetNextWindowPos(sf::Vector2f(10, 10));
        if (ImGui::Begin("game window", &m_showImGuiGameWindow, window_flags)) {
            HelpMarker("my game state manager's default window\nyou can close this by setting show to false\nyou can add stuff to the window for consistant\nif you close this and add stuff they go to debug window");
        }
    }

    if (!m_states.empty()) {
        m_states.top()->update(td);
    }
    else
        LOGD << "empty states stack";

    if (shouldEnd) {
        ImGui::End();  // end "game window"
        shouldEnd = false;
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
    
    while(!m_states.empty()) m_states.pop();
    m_states = {};

    LOGV << "state Manager stop - finish";
}

bool StateManager::isRunning() const {
    return !m_states.empty();
}

sf::RenderWindow& StateManager::getWin() {
    return m_win;
}

// StatePtr StateManager::createState(States::ID stateID) {
//     auto found = m_factories.find(stateID);
//     assert(found != m_factories.end());

//     return m_factories[stateID]();
// }
