#pragma once

#include "GLSLProgram.hpp"

class   ShaderBillboard :public GLSLProgram
{
public:
    attribute   _position;
    attribute   _uv;
    uniform     _MVP;
    uniform     _texture;
public:
    ShaderBillboard()
    {
    }
    ~ShaderBillboard()
    {
    }

    /// 初始化函数
    virtual void    initialize()
    {
        const char* vs =
        {   
            "#version 330\n\
            uniform    mat4    _MVP;\n\
            in          vec3    _position; \n\
            in          vec2    _uv; \n\
            out         vec2    _outUV;\n\
            void main()\n\
            {\n\
               vec4    pos =   vec4(_position.x,_position.y,_position.z,1);\n\
               gl_Position =   _MVP * pos; \n\
               _outUV      =   _uv;\n\
            }"
        };
        const char* ps =
        {
            "uniform    sampler2D   _texture;\n\
            in          vec2        _outUV;\n\
            void main()\n\
            {\n\
               vec4   color   =   texture2D(_texture,_outUV); \n\
               if(color.a < 0.2) discard;\n\
               gl_FragColor   =   color;\n\
            }"
        };

        createProgram(vs, ps);
        _position   =   glGetAttribLocation(_program, "_position");
        _uv         =   glGetAttribLocation(_program, "_uv");
        _MVP        =   glGetUniformLocation(_program, "_MVP");
        _texture    =   glGetUniformLocation(_program, "_texture");
        
    }

    /**
    *   使用程序
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_uv);
    }
    /**
    *   使用完成
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_uv);
        glUseProgram(0);
    }
};
