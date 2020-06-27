#pragma once

#include <assert.h>
#include "glew/glew.h"


enum    ShaderType
{
    SHADER_NONE     =   0,
    SHADER_VERTEX   =   GL_VERTEX_SHADER,
    SHADER_FRAGMENT =   GL_FRAGMENT_SHADER,
    SHADER_TES      =   GL_TESS_EVALUATION_SHADER,
    SHADER_TCS      =   GL_TESS_CONTROL_SHADER,
    SHADER_CS       =   GL_COMPUTE_SHADER,
};

class   GLSLProgram
{
public:
    typedef GLint   attribute;
    typedef GLint   uniform;
public:
    GLint           _program;
public:
    GLSLProgram()
    {
        _program    =   -1;
    }
    virtual ~GLSLProgram()
    {
        if (_program != -1)
        {
            glDeleteProgram(_program);
        }
    }

    virtual void    initialize()
    {
        createProgram();
    }

    virtual void    createProgram()
    {
        _program = glCreateProgram();
        
    }
    
    virtual void    attachShader(GLuint shader)
    {
        glAttachShader(_program,shader);
    }
    virtual void    dettachShader(GLuint shader)
    {
        glDetachShader(_program,shader);
    }

    virtual bool    linkProgram()
    {
        GLint   status = 0;
        char    compileLog[1024] = { 0 };
        /// 链接程序
        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
            return  false;
        }
        return  true;
    }
    
    /**
    *   使用shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
    }
    /**
    *   结束
    */
    virtual void    end()
    {
        glUseProgram(0);
    }

    unsigned    createShader(const char* source, ShaderType types)
    {
        unsigned    handle = 0;
        switch (types)
        {
        case SHADER_VERTEX:
            handle = glCreateShader(GL_VERTEX_SHADER);
            break;
        case SHADER_FRAGMENT:
            handle = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case SHADER_TES:
            handle = glCreateShader(GL_TESS_EVALUATION_SHADER);
            break;
        case SHADER_TCS:
            handle = glCreateShader(GL_TESS_CONTROL_SHADER);
            break;
        case SHADER_CS:
            handle = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            break;
        }

        GLint   status = 0;
        char    compileLog[1024] = { 0 };

        glShaderSource(handle, 1, (const GLchar**)&source, 0);
        glCompileShader(handle);
        glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
        glGetShaderInfoLog(handle, sizeof(compileLog), 0, compileLog);
        if (status == GL_FALSE)
        {
            assert(false && compileLog);
            glDeleteShader(handle);
            return  -1;
        }
        return  handle;
    }
};
