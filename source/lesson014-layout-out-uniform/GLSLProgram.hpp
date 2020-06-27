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


class   HelloShader :public GLSLProgram
{
public:
    virtual void    initialize()
    {
        const char* vs = "void main()\
                         {\
                            gl_Position = ftransform();\
                         }";
        const char* ps = "void main()\
                         {\
                            gl_FragColor = vec4(1,0,0,1);\
                         }";
        createProgram(vs, ps);
    }

};




class   ShaderUniform :public GLSLProgram
{
public:
    uniform _color;
public:
    virtual void    initialize()
    {
        const char* vs  =   "void main()\
                            {\
                                gl_Position = ftransform();\
                            }";
        const char* ps  =   "uniform vec4 _color;\
                            void main()\
                            {\
                                gl_FragColor = _color;\
                            }";
        createProgram(vs, ps);

        _color  =   glGetUniformLocation(_program,"_color");
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

};


/// gl_Vertex,gl_Normal,
class   ShaderVertex :public GLSLProgram
{
public:
    attribute   _position;
    attribute   _color;
    uniform     _mvp;
    uniform     _tex;
public:
    virtual void    initialize()
    {
        /// more https://www.khronos.org/opengl/wiki/Layout_Qualifier_(GLSL)
        const char* vs  =   "#version 430 \n\
                            layout(location = 0) in     vec3    _position;\n\
                            layout(location = 1) in     vec3    _color;\n\
                            layout(location = 2) in     vec2    _uv;\n\
                            layout(location = 0) uniform mat4    _mvp;\n\
                            layout(location = 0) out    vec3    _outColor;\n\
                            layout(location = 1) out    vec2    _outUV;\n\
                            void main()\n\
                            {\n\
                                _outColor   =   _color;\n\
                                _outUV      =   _uv;\n\
                                gl_Position =   _mvp * vec4(_position,1.0);\n\
                            }";
        const char* ps =    "#version 430 \n\
                            layout(location = 0)  in    vec3    _inColor;\n\
                            layout(location = 1)  in    vec2    _inUV;\n\
                            layout(location = 1) uniform sampler2D   _tex;\n\
                            void main()\n\
                            {\n\
                                vec4    color = texture2D(_tex,_inUV);\n\
                                gl_FragColor = color * vec4(_inColor,1);\n\
                            }";
        createProgram(vs, ps);

        _mvp        =   glGetUniformLocation(_program,"_mvp");
        _tex        =   glGetUniformLocation(_program,"_tex");
        _color      =   glGetAttribLocation(_program,"_color");
        _position   =   glGetAttribLocation(_program, "_position");

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


};