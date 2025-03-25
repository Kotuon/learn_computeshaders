
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "computetest.hpp"
#include "time_manager.hpp"

#include "editor.hpp"

void CTest::init() {
    Editor::instance().addDisplayMenuCallback(
        std::bind( &CTest::displayEditorWindow, this ) );

    VFileNamePos = getFileNamePos( VertexFile );
    FFileNamePos = getFileNamePos( FragmentFile );
    CFileNamePos = getFileNamePos( ComputeFile );

    loadShaders();

    glGenTextures( 1, &texture );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, TextureWidth, TextureHeight, 0,
                  GL_RGBA, GL_FLOAT, NULL );

    glBindImageTexture( 0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
}

void CTest::render( const TimeManager* Time ) {
    Compute->use();
    Compute->setFloat( "t", Time->getTime() );
    Compute->setFloat( "speed", speed );
    glDispatchCompute( TextureWidth / 10, TextureHeight / 10, 1 );

    // make sure writing to image has finished before read
    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );

    // render image to quad
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // Screen->setFloat( "t", Time->getTime() );
    Screen->use();

    renderQuad();
}

void CTest::loadShaders() {
    Screen = std::make_unique< Shader >( VertexFile, FragmentFile );
    Compute = std::make_unique< ComputeShader >( ComputeFile );

    Screen->use();
    Screen->setInt( "tex", 0 );
}

void CTest::renderQuad() {
    if ( quadVAO == 0 ) {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays( 1, &quadVAO );
        glGenBuffers( 1, &quadVBO );
        glBindVertexArray( quadVAO );
        glBindBuffer( GL_ARRAY_BUFFER, quadVBO );
        glBufferData( GL_ARRAY_BUFFER, sizeof( quadVertices ), &quadVertices,
                      GL_STATIC_DRAW );
        glEnableVertexAttribArray( 0 );
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ),
                               ( void* )0 );
        glEnableVertexAttribArray( 1 );
        glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ),
                               ( void* )( 3 * sizeof( float ) ) );
    }
    glBindVertexArray( quadVAO );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    glBindVertexArray( 0 );
}

#include "imgui.h"
#include "ImGuiFileDialog.h"

#include <string>

#include "trace.hpp"
#include <fmt/core.h>

void CTest::displayEditorWindow() {
    ImGui::Begin( "ShaderManager##1" );

    // Shader file select

    ImGui::SeparatorText( "Shader file settings" );

    if ( ImGui::Button( "Reload shaders" ) ) {
        loadShaders();
    }

    // Vertex dialog

    ImGui::Text( "Vertex Shader:" );
    ImGui::SameLine();

    if ( ImGui::Button( VertexFile.substr( VFileNamePos ).c_str() ) ) {
        IGFD::FileDialogConfig config;
        config.path = "shaders/";

        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseFileDlgKey##1", "Choose File", ".vs", config );
    }

    if ( ImGuiFileDialog::Instance()->Display( "ChooseFileDlgKey##1" ) ) {
        if ( ImGuiFileDialog::Instance()->IsOk() ) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();

            Trace::message( fmt::format( "FilePathName: {}", filePathName ) );
            Trace::message( fmt::format( "FilePath: {}", filePath ) );

            VertexFile = filePathName;
            VFileNamePos = getFileNamePos( VertexFile );
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Fragment dialog
    ImGui::Text( "Fragment Shader:" );
    ImGui::SameLine();

    if ( ImGui::Button( FragmentFile.substr( FFileNamePos ).c_str() ) ) {
        IGFD::FileDialogConfig config;
        config.path = "shaders/";

        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseFileDlgKey##2", "Choose File", ".fs", config );
    }

    if ( ImGuiFileDialog::Instance()->Display( "ChooseFileDlgKey##2" ) ) {
        if ( ImGuiFileDialog::Instance()->IsOk() ) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();

            Trace::message( fmt::format( "FilePathName: {}", filePathName ) );
            Trace::message( fmt::format( "FilePath: {}", filePath ) );

            FragmentFile = filePathName;
            FFileNamePos = getFileNamePos( FragmentFile );
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Compute dialog
    ImGui::Text( "Compute Shader:" );
    ImGui::SameLine();

    if ( ImGui::Button( ComputeFile.substr( CFileNamePos ).c_str() ) ) {
        IGFD::FileDialogConfig config;
        config.path = "shaders/";

        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseFileDlgKey##3", "Choose File", ".cs", config );
    }

    if ( ImGuiFileDialog::Instance()->Display( "ChooseFileDlgKey##3" ) ) {
        if ( ImGuiFileDialog::Instance()->IsOk() ) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath =
                ImGuiFileDialog::Instance()->GetCurrentPath();

            Trace::message( fmt::format( "FilePathName: {}", filePathName ) );
            Trace::message( fmt::format( "FilePath: {}", filePath ) );

            ComputeFile = filePathName;
            CFileNamePos = getFileNamePos( ComputeFile );
        }
        ImGuiFileDialog::Instance()->Close();
    }

    // Compute shader settings

    ImGui::SeparatorText( "Compute shader settings" );

    if ( ImGui::SliderFloat( "Speed##1", &speed, -2000.f, 2000.f ) ) {
    }

    ImGui::End();
}

size_t CTest::getFileNamePos( const std::string& fileName ) {
    size_t fileNamePos = fileName.find_last_of( "\\" );
    if ( fileNamePos == std::string::npos ) {
        fileNamePos = 0;
    } else {
        fileNamePos += 1;
    }

    return fileNamePos;
}
