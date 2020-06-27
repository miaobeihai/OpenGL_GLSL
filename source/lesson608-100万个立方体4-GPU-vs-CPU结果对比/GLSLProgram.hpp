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
};

class   GLSLProgram
{
public:
    typedef GLint   attribute;
    typedef GLint   uniform;
public:
    GLint           _vHandle;
    GLint           _pHandle;
    GLint           _program;
public:
    GLSLProgram()
    {
        _vHandle    =   -1;
        _pHandle    =   -1;
        _program    =   -1;
    }
    virtual ~GLSLProgram()
    {
    }

    virtual void    initialize()
    {
        const char* vs = "";
        const char* ps = "";
        createProgram(vs, ps);
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
     *	创建shader
     */
    GLuint    createShader(const char* source, ShaderType types)
    {
        GLuint  handle = 0;
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
    GLuint  createProgramOnly()
    {
        return  glCreateProgram();
    }
    bool linkProgram()
    {
        GLint   status = 0;
        char    compileLog[1024] = { 0 };
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
     *	根据输入的源代码编译shader
     */
    bool    createProgram(const char* vs, const char* ps)
    {
        bool    result = false;
        do
        {
            //! 创建shader
            _vHandle = glCreateShader(GL_VERTEX_SHADER);
            _pHandle = glCreateShader(GL_FRAGMENT_SHADER);

            //! 指定shader的源代码
            glShaderSource(_vHandle, 1, (const GLchar**)&vs, 0);
            glShaderSource(_pHandle, 1, (const GLchar**)&ps, 0);

            //! 编译shader
            glCompileShader(_vHandle);
            GLint   status = 0;
            char    compileLog[1024] = { 0 };
            glGetShaderiv(_vHandle, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetShaderInfoLog(_vHandle, sizeof(compileLog), 0, compileLog);
                assert(false && compileLog);
                break;
            }

            glCompileShader(_pHandle);

            glGetShaderiv(_pHandle, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetShaderInfoLog(_pHandle, sizeof(compileLog), 0, compileLog);
                assert(false && compileLog);
                break;
            }
            //! 4创建程序
            _program = glCreateProgram();
            glAttachShader(_program, _vHandle);
            glAttachShader(_program, _pHandle);

            //! 5链接程序
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
            if (_program != -1)
            {
                glDeleteProgram(_program);
            }
            _vHandle = -1;
            _pHandle = -1;
            _program = -1;
        }
        return  result;
    }
};
