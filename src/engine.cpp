
// System headers
#include <fmt/core.h>

// Local headers
#include "editor.hpp"
#include "engine.hpp"
#include "graphics.hpp"
#include "input.hpp"
#include "profiler.hpp"
#include "trace.hpp"

#include "computetest.hpp"

constexpr int WIDTH = 1000;
constexpr int HEIGHT = 1000;

static std::unique_ptr< CTest > test;

Engine::Engine() {}

bool Engine::initialize() {
    if ( !Graphics::instance().initialize( WIDTH, HEIGHT ) ) {
        Trace::message( "Graphics falied to initialize." );
        return false;
    }

    if ( !Editor::instance().initialize( Graphics::instance().getWindow() ) ) {
        Trace::message( "Editor failed to initialize." );
    }

    test = std::make_unique< CTest >();

    test->init();

    Time = std::make_unique< TimeManager >();

    Graphics::instance().addRenderCallback(
        std::bind( &CTest::render, test.get(), Time.get() ) );

    IsRunning = true;

    return true;
}

void Engine::update() {
    while ( IsRunning ) {
        Time->update();

        glfwSetWindowTitle( Graphics::instance().getWindow(),
                            fmt::format( "basic window: FPS: {:0.2f}",
                                         1.f / Time->getDeltaTime() )
                                .c_str() );

        // Non-fixed time step update calls
        Input::instance().update();

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
