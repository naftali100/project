
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Log.h>

#include "Game.h"

int main() {
    // init plog
    plog::ColorConsoleAppender<plog::TxtFormatter> appiarer;
    plog::init(plog::info, &appiarer);

    Game().run();
    return 0;
}