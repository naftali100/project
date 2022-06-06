
#include <plog/Formatters/FuncMessageFormatter.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Log.h>

#include "Game.h"

int main() {
    // init plog
    plog::init<plog::FuncMessageFormatter>(plog::info, plog::streamStdOut);

    Game().run();
    return 0;
}