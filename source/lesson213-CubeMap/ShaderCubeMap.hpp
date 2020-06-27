#pragma once

#include "GLSLProgram.hpp"

class   ShaderCubeMap :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _model;
    uniform     _cameraPos;
    uniform     _texture;
    attribute   _position;
    attribute   _normal;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            uniform mat4    _model;\n\
                            uniform mat4    _mvp;\n\
                            in      vec3    _position; \n\
                            in      vec3    _normal; \n\
                            out     vec3    vPosition;\n\
                            out     vec3    vNormal;\n\
                            void main(void) \n\
                            {\n\
                                vPosition   =   (_model * vec4(_position, 1.0)).xyz;\n\
                                vNormal     =   (_model * vec4(_normal, 0.0)).xyz;\n\
                                gl_Position =   _mvp * vec4(_position, 1.0);\n\
                            }";
        const char* ps =    "//#version 120 \n\
                            #version 330 \n\
                            uniform vec3        _cameraPos;\n\
                            uniform samplerCube _cubeTexture;\n\
                            in      vec3        vPosition;\n\
                            in      vec3        vNormal;\n\
                            void main(void)\n\
                            {\n\
                                vec3 ref        =   reflect(vPosition - _cameraPos, vNormal);\n\
                                vec4 envColor   =   texture(_cubeTexture, ref);\n\
                                gl_FragColor    =   envColor;\n\
                            }";
        createProgram(vs, ps);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _model      =   glGetUniformLocation(_program,  "_model");
        _cameraPos  =   glGetUniformLocation(_program,  "_cameraPos");
        _texture    =   glGetUniformLocation(_program,  "_cubeTexture");

        _position   =   glGetAttribLocation(_program,   "_position");
        _normal     =   glGetAttribLocation(_program,   "_normal");
    }
    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_normal);
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_normal);
        glUseProgram(0);
    }

};