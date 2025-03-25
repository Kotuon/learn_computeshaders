
#ifndef COMPUTETEST_HPP
#define COMPUTETEST_HPP
#pragma once

#include <memory>

#include "shader_manager.hpp"

class TimeManager;

class CTest {
public:
    void init();
    void render( const TimeManager* Time );

    void displayEditorWindow();

private:
    void loadShaders();
    void renderQuad();

    size_t getFileNamePos( const std::string& fileName );

    std::unique_ptr< Shader > Screen;
    std::unique_ptr< ComputeShader > Compute;

    std::string VertexFile = "shaders\\screenVert.vs";
    std::string FragmentFile = "shaders\\screenFrag.fs";
    std::string ComputeFile = "shaders\\compute.cs";

    float speed = 0.f;

    size_t VFileNamePos = 0;
    size_t FFileNamePos = 0;
    size_t CFileNamePos = 0;

    unsigned quadVAO = 0;
    unsigned quadVBO;

    unsigned texture;

    const unsigned TextureWidth = 1000;
    const unsigned TextureHeight = 1000;
};

#endif
