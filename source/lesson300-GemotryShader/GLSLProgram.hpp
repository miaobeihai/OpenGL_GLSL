#pragma once

#include <assert.h>
#include "glew/glew.h"

enum    ShaderType
{
    SHADER_NONE = 0,
    SHADER_VERTEX = GL_VERTEX_SHADER,
    SHADER_FRAGMENT = GL_FRAGMENT_SHADER,
    SHADER_TES = GL_TESS_EVALUATION_SHADER,
    SHADER_TCS = GL_TESS_CONTROL_SHADER,
};

class   GLSLProgram
{
public:
    typedef GLint   attribute;
    typedef GLint   uniform;
public:
    GLint           _vHandle;
    GLint           _pHandle;
    GLint           _pTES;
    GLint           _pTCS;
    GLint           _program;
public:
    GLSLProgram()
    {
        _vHandle = -1;
        _pHandle = -1;
        _pTES = -1;
        _pTCS = -1;
        _program = -1;
    }
    virtual ~GLSLProgram()
    {
    }

    virtual void    initialize()
    {
        createProgram(0, 0, 0, 0);
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
protected:
    /**
    *	根据输入的源代码编译shader
    */
    bool    createProgram(
          const char* vs
        , const char* ps
        , const char* tcs
        , const char* tes)
    {
        bool    result = false;
        do
        {
            if (vs)
            {
                _vHandle = createShader(vs, SHADER_VERTEX);
            }
            if (ps)
            {
                _pHandle = createShader(ps, SHADER_FRAGMENT);
            }
            if (tcs)
            {
                _pTCS = createShader(tcs, SHADER_TCS);
            }
            if (tes)
            {
                _pTES = createShader(tes, SHADER_TES);
            }
            _program = glCreateProgram();
            if (_vHandle != -1)
            {
                glAttachShader(_program, _vHandle);
            }
            if (_pHandle != -1)
            {
                glAttachShader(_program, _pHandle);
            }
            if (_pTCS != -1)
            {
                glAttachShader(_program, _pTCS);
            }
            if (_pTES != -1)
            {
                glAttachShader(_program, _pTES);
            }
            GLint   status = 0;
            char    compileLog[1024] = { 0 };
            /// 链接程序
            glLinkProgram(_program);
            glGetProgramiv(_program, GL_LINK_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
                break;
            }
            result = true;

        } while (false);

        if (!result)
        {
            if (_vHandle != -1)
            {
                glDeleteShader(_vHandle);
            }
            if (_pHandle != -1)
            {
                glDeleteShader(_pHandle);
            }
            if (_pTCS != -1)
            {
                glDeleteShader(_pTCS);
            }
            if (_pTES != -1)
            {
                glDeleteShader(_pTES);
            }
            if (_program != -1)
            {
                glDeleteProgram(_program);
            }
            _vHandle = -1;
            _pHandle = -1;
            _pTES = -1;
            _pTCS = -1;
            _program = -1;
        }
        return  result;
    }
protected:
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
