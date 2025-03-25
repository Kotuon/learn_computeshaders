
#include "crash_handler.hpp"
#include "engine.hpp"

int main( int, char** ) {
    setupDump();

    const bool Result = Engine::instance().initialize();
    if ( !Result ) {
        return EXIT_FAILURE;
    }

    // Main update loop
    Engine::instance().update();

    Engine::instance().shutdown();

    return 0;
}
