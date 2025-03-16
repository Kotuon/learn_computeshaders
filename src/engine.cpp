
// System headers
#include <fmt/core.h>

// Local headers
#include "engine.hpp"
#include "graphics.hpp"
#include "profiler.hpp"
#include "trace.hpp"

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

Engine::Engine() {}

bool Engine::initialize() {
    if ( !Graphics::instance().initialize( WIDTH, HEIGHT ) ) {
        Trace::message( "Graphics falied to initialize." );
        return false;
    }

    // if ( !Camera::instance().initialize( glm::vec3( 0.f, 0.f, 1600.f ) ) ) {
    //     Trace::message( "Camera falied to initialize." );
    // }

    // if ( !Editor::instance().initialize( Graphics::instance().getWindow() ) )
    // {
    //     Trace::message( "Editor failed to initialize." );
    // }

    Time = std::make_unique< TimeManager >();

    IsRunning = true;

    return true;
}

void Engine::update() {
    // Profiler ProfilerInstance( 100000 );

    while ( IsRunning ) {
        Time->update();

        glfwSetWindowTitle( Graphics::instance().getWindow(),
                            fmt::format( "basic window: FPS: {:0.2f}",
                                         1.f / Time->getDeltaTime() )
                                .c_str() );

        // Non-fixed time step update calls
        // Input::instance().update();

        // Fixed time step update calls
        while ( Time->needsFixedUpdate() ) {
            // Call fixed updates here

            for ( auto& Func : FixedUpdateCallbacks ) {
                Func();
            }
        }

        // Non-fixed time step update calls
        // TODO: will be moved around

        for ( auto& Func : UpdateCallbacks ) {
            Func();
        }

        // Camera::instance().update();
        Graphics::instance().update();
    }
}

void Engine::shutdown() { Graphics::instance().shutdown(); }

void Engine::triggerShutdown() { IsRunning = false; }

Engine& Engine::instance() {
    static Engine EngineInstance;
    return EngineInstance;
}
