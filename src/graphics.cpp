
// std includes
#include <ranges>

// System headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_float4x4.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

// Local headers
#include "graphics.hpp"
#include "trace.hpp"
#include "engine.hpp"

static const char* castToString( const unsigned char* Input ) {
    return reinterpret_cast< const char* >( Input );
}

Graphics::Graphics() {}

bool Graphics::initialize( const int Width, const int Height ) {
    if ( !glfwInit() ) {
        Trace::message( "Could not start GLFW." );
        return false;
    }

    // Set core Window options (adjust version numbers if needed)
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    // Enable the GLFW runtime error callback function defined previously.
    glfwSetErrorCallback( Graphics::GLFWErrorCallback );

    // Set additional Window options
    glfwWindowHint( GLFW_RESIZABLE, false );
    // glfwWindowHint( GLFW_SAMPLES, 4 ); // MSAA

    // Create Window using GLFW
    Window = glfwCreateWindow( Width, Height, "", nullptr, nullptr );

    // Ensure the Window is set up correctly
    if ( !Window ) {
        Trace::message( "Failed to create GLFW Window." );

        glfwTerminate();
        return false;
    }

    // Let the Window be the current OpenGL context and initialise glad
    glfwMakeContextCurrent( Window );
    glfwSwapInterval( 0 );

    if ( !gladLoadGLLoader( ( GLADloadproc )glfwGetProcAddress ) ) {
        Trace::message( "Failed to initialize GLAD." );
        return false;
    }

    Trace::message( fmt::format( "{}: {}",
                                 castToString( glGetString( GL_VENDOR ) ),
                                 castToString( glGetString( GL_RENDERER ) ) ) );
    Trace::message( fmt::format( "GLFW\t {}", glfwGetVersionString() ) );
    Trace::message( fmt::format( "OpenGL\t {}",
                                 castToString( glGetString( GL_VERSION ) ) ) );
    Trace::message( fmt::format(
        "GLSL\t {}",
        castToString( glGetString( GL_SHADING_LANGUAGE_VERSION ) ) ) );

    // Set callbacks
    glfwSetFramebufferSizeCallback( Window, Graphics::frameBufferSizeCallback );
    glfwSetCursorEnterCallback( Window, Graphics::cursorEnterCallback );
    glfwSetWindowCloseCallback( Window, Graphics::closeWindowCallback );

    return true;
}

void Graphics::update() {
    // Draw your scene here
    for ( auto& Func : RenderCallbacks | std::views::reverse ) {
        Func();
    }

    // Flip buffers
    glfwSwapBuffers( Window );

    // Handle other events
    glfwPollEvents();
}

void Graphics::shutdown() {
    // Terminate GLFW (no need to call glfwDestroyWindow)
    glfwTerminate();
}

GLFWwindow* Graphics::getWindow() const { return Window; }

void Graphics::frameBufferSizeCallback( GLFWwindow*, int Width, int Height ) {
    glViewport( 0, 0, Width, Height );
}

void Graphics::cursorEnterCallback( GLFWwindow*, int Entered ) {
    if ( Entered ) {
        // The cursor entered the content area of the Window
        // glfwSetInputMode( Graphics::Instance().Window, GLFW_CURSOR,
        // GLFW_CURSOR_DISABLED ); cursorEntered = true;
    } else {
        // The cursor left the content area of the Window
    }
}

void Graphics::GLFWErrorCallback( int Error, const char* Description ) {
    std::string message =
        "GLFW returned an error: " + std::string( Description ) +
        std::to_string( Error );

    Trace::message( message );
}

void Graphics::closeWindowCallback( GLFWwindow* Window ) {
    glfwSetWindowShouldClose( Window, GL_TRUE );
    Engine::instance().triggerShutdown();
}

Graphics& Graphics::instance() {
    static Graphics graphicsInstance;
    return graphicsInstance;
}

void handleKeyboardInput( GLFWwindow* Window ) {
    // Use escape key for terminating the GLFW Window
    if ( glfwGetKey( Window, GLFW_KEY_ESCAPE ) == GLFW_PRESS ) {
        glfwSetWindowShouldClose( Window, GL_TRUE );
        Engine::instance().triggerShutdown();
    }
}
