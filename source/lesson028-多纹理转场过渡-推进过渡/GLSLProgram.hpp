#pragma once

#include <assert.h>
#include "glew/glew.h"

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

/// gl_Vertex,gl_Normal,
class   ShaderVertex :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _texture0;
    uniform     _texture1;
    uniform     _offset;
    attribute   _pos;
    attribute   _uv;
    
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 130\n\
                            uniform mat4    _mvp;\n\
                            uniform float   _uvOffset; \n\
                            in  vec3    _pos;\n\
                            in  vec2    _uv;\n\
                            out vec2    _outUV;\n\
                            out vec2    _outUV1;\n\
                            void main()\n\
                            {\n\
                                _outUV      =   _uv;\n\
                                gl_Position =   _mvp * vec4(_pos, 1.0); \n\
                            }";
        const char* ps  =   "uniform sampler2D  _texture0;\n\
                            uniform sampler2D   _texture1;\n\
                            uniform float       _offset;\n\
                            in      vec2        _outUV;\n\
                            void main()\n\
                            {\n\
                                vec4 color0 = vec4(0,0,0,1);\n\
                                if(_outUV.x <=_offset )\n\
                                    color0 = texture2D(_texture0,_outUV);\n\
                                else\n\
                                    color0 = texture2D(_texture1,_outUV);\n\
                                gl_FragColor    =   color0;\n\
                            }";
        createProgram(vs, ps);

        _mvp        =   glGetUniformLocation(_program,"_mvp");
        _texture0   =   glGetUniformLocation(_program,"_texture0");
        _texture1   =   glGetUniformLocation(_program,"_texture1");
        _offset     =   glGetUniformLocation(_program,"_offset");
        _pos        =   glGetAttribLocation(_program,"_pos");
        _uv         =   glGetAttribLocation(_program,"_uv");
    }

    /**
    *   使用shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_pos);
        glEnableVertexAttribArray(_uv);
    }
    /**
    *   结束
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_pos);
        glDisableVertexAttribArray(_uv);
        glUseProgram(0);
    }


};