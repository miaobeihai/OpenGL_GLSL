#pragma once

#include "GLSLProgram.hpp"


class   ShaderTexcoord:public GLSLProgram
{
public:
    uniform _color;
    uniform _texture;
public:
    virtual void    initialize()
    {
        const char* vs  =   "uniform    vec4    _color;\
                            varying     vec4    outColor;\
                            void main()\
                            {\
                                outColor        =   _color;\
                                gl_TexCoord[0]  =   gl_MultiTexCoord0;\
                                gl_Position     =   gl_ModelViewProjectionMatrix * gl_Vertex;\
                            }";
        const char* ps  =   "varying    vec4        outColor;\
                            uniform     sampler2D   _texture;\
                            void main()\
                            {\
                                vec4    color   =   texture2D(_texture,gl_TexCoord[0].st);\
                                gl_FragColor    =   color;\
                            }";
        createProgram(vs, ps);

        _color  =   glGetUniformLocation(_program,"_color");
        _texture=   glGetUniformLocation(_program,"_texture");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glUseProgram(0);
    }

};   