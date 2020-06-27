#pragma once

#include "GLSLProgram.hpp"

class   ShaderParticle :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _texture0;
    attribute   _pos;
public:
    virtual void    initialize()
    {
        GLSLProgram::initialize();
        const char* vs  =   "#version 130\n\
                            uniform mat4    _mvp;\n\
                            in  vec3        _pos;\n\
                            void main()\n\
                            {\n\
                                gl_PointSize    =   64;\n\
                                gl_Position     =   _mvp * vec4(_pos, 1.0); \n\
                            }";
        const char* ps  =   "uniform sampler2D  _texture0;\n\
                            void main()\n\
                            {\n\
                                gl_FragColor    =   texture2D(_texture0 ,gl_PointCoord);\n\
                            }";

        GLuint  vsHandle =   createShader(vs,SHADER_VERTEX);
        GLuint  psHandle =   createShader(ps,SHADER_FRAGMENT);
        attachShader(vsHandle);
        attachShader(psHandle);
        linkProgram();
        _mvp        =   glGetUniformLocation(_program,"_mvp");
        _texture0   =   glGetUniformLocation(_program,"_texture0");
        _pos        =   glGetAttribLocation(_program,"_pos");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_pos);
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_pos);
        glUseProgram(0);
    }


};
