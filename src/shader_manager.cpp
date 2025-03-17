
#include <fmt/core.h>
#include <sstream>

#include "shader_manager.hpp"
#include "trace.hpp"

void ShaderBase::use() const { glUseProgram( Id ); }

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

void ShaderBase::checkCompileErrors( GLuint Shader, std::string Type ) {
    GLint Success;
    GLchar InfoLog[1024];
    if ( Type != "PROGRAM" ) {
        glGetShaderiv( Shader, GL_COMPILE_STATUS, &Success );
        if ( !Success ) {
            glGetShaderInfoLog( Shader, 1024, NULL, InfoLog );

            Trace::message( fmt::format(
                "ERROR::Shader_COMPILATION_ERROR of Type: {}\n", Type ) );
            Trace::message( fmt::format(
                "{}\n -- "
                "--------------------------------------------------- \n",
                InfoLog ) );
        }
    } else {
        glGetProgramiv( Shader, GL_LINK_STATUS, &Success );
        if ( !Success ) {
            glGetProgramInfoLog( Shader, 1024, NULL, InfoLog );

            Trace::message( fmt::format(
                "ERROR::PROGRAM_LINKING_ERROR of Type: {}\n", Type ) );

            Trace::message( fmt::format(
                "{}\n -- --------------------------------------------------- "
                "\n",
                InfoLog ) );
        }
    }
}

Shader::Shader( const std::string& VertexFile,
                const std::string& FragmentFile ) {
    const char* VertSource;
    const char* FragSource;

    std::ifstream VFile;
    std::ifstream FFile;

    VFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    FFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try {
        VFile.open( VertexFile );
        FFile.open( FragmentFile );

        std::stringstream VStream, FStream;

        VStream << VFile.rdbuf();
        FStream << FFile.rdbuf();

        VFile.close();
        FFile.close();

        VertSource = VStream.str().c_str();
        FragSource = FStream.str().c_str();
    } catch ( std::ifstream::failure& e ) {
        Trace::message( fmt::format(
            "ERROR::SHADER::FILE_NOT_SuccessFULLY_READ: {}\n", e.what() ) );
    }

    unsigned Vertex, Fragment;

    // Create vertex shader
    Vertex = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( Vertex, 1, &VertSource, nullptr );
    glCompileShader( Vertex );
    checkCompileErrors( Vertex, "VERTEX" );

    // Create fragment shader
    Fragment = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( Fragment, 1, &FragSource, nullptr );
    glCompileShader( Fragment );
    checkCompileErrors( Fragment, "FRAGMENT" );

    // Create program
    Id = glCreateProgram();
    glAttachShader( Id, Vertex );
    glAttachShader( Id, Fragment );
    glLinkProgram( Id );
    checkCompileErrors( Id, "PROGRAM" );

    glDeleteShader( Vertex );
    glDeleteShader( Fragment );
}

ComputeShader::ComputeShader( const char* ComputeFile ) {
    const char* ComputeSource;
    std::ifstream CFile;

    CFile.exceptions( std::ifstream::failbit | std::ifstream::badbit );

    try {
        CFile.open( ComputeFile );

        std::stringstream CStream;

        CStream << CFile.rdbuf();
        CFile.close();
        ComputeSource = CStream.str().c_str();
    } catch ( std::ifstream::failure& e ) {
        Trace::message( fmt::format(
            "ERROR::SHADER::FILE_NOT_SuccessFULLY_READ: {}\n", e.what() ) );
    }

    unsigned Compute;

    // Create compute shader
    Compute = glCreateShader( GL_COMPUTE_SHADER );
    glShaderSource( Compute, 1, &ComputeSource, nullptr );
    glCompileShader( Compute );
    checkCompileErrors( Compute, "COMPUTE" );

    // Create program
    Id = glCreateProgram();
    glAttachShader( Id, Compute );
    glLinkProgram( Id );
    checkCompileErrors( Id, "PROGRAM" );

    glDeleteShader( Compute );
}
