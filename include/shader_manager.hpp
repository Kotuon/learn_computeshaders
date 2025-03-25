
#ifndef SHADER_MANAGER_HPP
#define SHADER_MANAGER_HPP
#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderBase {
public:
    // Shader( const std::string& VertexFile, const std::string& FragmentFile );

    void use() const;
    void deleteProgram();

    void setBool( const std::string& Name, const bool Value ) const;
    void setInt( const std::string& Name, const int Value ) const;
    void setFloat( const std::string& Name, const float Value ) const;

    void setVec2( const std::string& Name, const glm::vec2& Value ) const;
    void setVec2( const std::string& Name, const float x, const float y ) const;
    void setVec3( const std::string& Name, const glm::vec3& Value ) const;
    void setVec3( const std::string& Name, const float x, const float y,
                  const float z ) const;
    void setVec4( const std::string& Name, const glm::vec4& Value ) const;
    void setVec4( const std::string& Name, const float x, const float y,
                  const float z, const float w ) const;

    void setMat2( const std::string& Name, const glm::mat2& Value ) const;
    void setMat3( const std::string& Name, const glm::mat3& Value ) const;
    void setMat4( const std::string& Name, const glm::mat4& Value ) const;

    unsigned getId() const;

protected:
    const std::string readFile( const std::string& FileName );
    void checkCompileErrors( GLuint Shader, std::string Type,
                             std::string FileName = "" );

    unsigned Id;
};

class Shader : public ShaderBase {
public:
    Shader( const std::string& VertexFile, const std::string& FragmentFile );
};

class ComputeShader : public ShaderBase {
public:
    ComputeShader( const std::string& ComputeFile );
};

#endif
