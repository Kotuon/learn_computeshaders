
#include <fmt/core.h>
#include <sstream>

#include "shader_manager.hpp"
#include "trace.hpp"

void ShaderBase::use() const { glUseProgram( Id ); }

void ShaderBase::deleteProgram() { glDeleteProgram( Id ); }

void ShaderBase::setBool( const std::string& Name, const bool Value ) const {
    glUniform1i( glGetUniformLocation( Id, Name.c_str() ), Value );
}
void ShaderBase::setInt( const std::string& Name, const int Value ) const {
    glUniform1i( glGetUniformLocation( Id, Name.c_str() ), Value );
}
void ShaderBase::setFloat( const std::string& Name, const float Value ) const {
    glUniform1f( glGetUniformLocation( Id, Name.c_str() ), Value );
}

void ShaderBase::setVec2( const std::string& Name,
                          const glm::vec2& Value ) const {
    glUniform2fv( glGetUniformLocation( Id, Name.c_str() ), 1, &Value[0] );
}
void ShaderBase::setVec2( const std::string& Name, const float x,
                          const float y ) const {
    glUniform2f( glGetUniformLocation( Id, Name.c_str() ), x, y );
}
void ShaderBase::setVec3( const std::string& Name,
                          const glm::vec3& Value ) const {
    glUniform3fv( glGetUniformLocation( Id, Name.c_str() ), 1, &Value[0] );
}
void ShaderBase::setVec3( const std::string& Name, const float x, const float y,
                          const float z ) const {
    glUniform3f( glGetUniformLocation( Id, Name.c_str() ), x, y, z );
}
void ShaderBase::setVec4( const std::string& Name,
                          const glm::vec4& Value ) const {
    glUniform4fv( glGetUniformLocation( Id, Name.c_str() ), 1, &Value[0] );
}
void ShaderBase::setVec4( const std::string& Name, const float x, const float y,
                          const float z, const float w ) const {
    glUniform4f( glGetUniformLocation( Id, Name.c_str() ), x, y, z, w );
}

void ShaderBase::setMat2( const std::string& Name,
                          const glm::mat2& Value ) const {
    glUniformMatrix2fv( glGetUniformLocation( Id, Name.c_str() ), 1, GL_FALSE,
                        &Value[0][0] );
}
void ShaderBase::setMat3( const std::string& Name,
                          const glm::mat3& Value ) const {
    glUniformMatrix3fv( glGetUniformLocation( Id, Name.c_str() ), 1, GL_FALSE,
                        &Value[0][0] );
}
void ShaderBase::setMat4( const std::string& Name,
                          const glm::mat4& Value ) const {
    glUniformMatrix4fv( glGetUniformLocation( Id, Name.c_str() ), 1, GL_FALSE,
                        &Value[0][0] );
}

void ShaderBase::checkCompileErrors( GLuint Shader, std::string Type,
                                     std::string FileName ) {
    GLint Success = 0;
    GLint LogSize = 0;

    if ( Type != "Program" ) {
        glGetShaderiv( Shader, GL_COMPILE_STATUS, &Success );
        if ( Success == GL_FALSE ) {
            glGetShaderiv( Shader, GL_INFO_LOG_LENGTH, &LogSize );
            GLchar* InfoLog = new GLchar[LogSize];
            glGetShaderInfoLog( Shader, LogSize, &LogSize, InfoLog );
            glDeleteShader( Shader );
            Trace::message(
                fmt::format( "{} Shader {}: {}\n", Type, FileName, InfoLog ) );
            delete[] InfoLog;
        }
    } else {
        glGetProgramiv( Shader, GL_LINK_STATUS, &Success );
        if ( !Success ) {
            glGetShaderiv( Shader, GL_INFO_LOG_LENGTH, &LogSize );
            GLchar* InfoLog = new GLchar[LogSize];
            glGetProgramInfoLog( Shader, 1024, NULL, InfoLog );

            Trace::message(
                fmt::format( "{} Shader {}: {}\n", Type, FileName, InfoLog ) );
        }
    }
}

const std::string ShaderBase::readFile( const std::string& FileName ) {
    std::string content;

    std::ifstream file( FileName );
    if ( !file.is_open() ) {
        Trace::message( fmt::format( "Failed to open shader {}.", FileName ) );

        return nullptr;
    }

    std::string line = "";
    while ( !file.eof() ) {
        getline( file, line );
        content.append( line + "\n" );
    }

    file.close();

    content.append( "\0" );

    return content;
}

unsigned ShaderBase::getId() const { return Id; }

#include <iostream>

Shader::Shader( const std::string& VertexFile,
                const std::string& FragmentFile ) {
    const std::string VertSource = readFile( VertexFile ).c_str();
    const std::string FragSource = readFile( FragmentFile ).c_str();

    const char* VSource = VertSource.c_str();
    const char* FSource = FragSource.c_str();

    unsigned Vertex, Fragment;

    // Create vertex shader
    Vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( Vertex, 1, &VSource, nullptr );
    glCompileShader( Vertex );
    checkCompileErrors( Vertex, "Vertex", VertexFile );

    // Create fragment shader
    Fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( Fragment, 1, &FSource, nullptr );
    glCompileShader( Fragment );
    checkCompileErrors( Fragment, "Fragment", FragmentFile );

    // Create program
    Id = glCreateProgram();
    glAttachShader( Id, Vertex );
    glAttachShader( Id, Fragment );
    glLinkProgram( Id );
    checkCompileErrors( Id, "Program" );

    glDeleteShader( Vertex );
    glDeleteShader( Fragment );
}

ComputeShader::ComputeShader( const std::string& ComputeFile ) {
    const std::string ComputeSource = readFile( ComputeFile );
    const char* CSource = ComputeSource.c_str();

    unsigned Compute;

    // Create compute shader
    Compute = glCreateShader( GL_COMPUTE_SHADER );
    glShaderSource( Compute, 1, &CSource, nullptr );
    glCompileShader( Compute );
    checkCompileErrors( Compute, "Compute", ComputeFile );

    // Create program
    Id = glCreateProgram();
    glAttachShader( Id, Compute );
    glLinkProgram( Id );
    checkCompileErrors( Id, "Program" );

    glDeleteShader( Compute );
}
