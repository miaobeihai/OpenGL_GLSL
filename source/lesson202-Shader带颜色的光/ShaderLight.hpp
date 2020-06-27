#pragma once

#include "GLSLProgram.hpp"

/// gl_Vertex,gl_Normal,
class   ShaderLight :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _lightDir;
    uniform     _lightColor;
    uniform     _lightDiffuse;
    attribute   _position;
    attribute   _normal;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            uniform mat4   _mvp;\n\
                            uniform vec3   _lightDir;\n\
                            uniform vec3   _lightColor;\n\
                            uniform vec3   _lightDiffuse;\n\
                            in     vec3    _position;\n\
                            in     vec3    _normal;\n\
                            out    vec4    _outColor;\n\
                            void main()\n\
                            {\n\
                                float NDotL =   max(dot(_normal, _lightDir), 0.0); \n\
                                _outColor   =   vec4(_lightColor * NDotL + _lightDiffuse,1);\n\
                                gl_Position =   _mvp * vec4(_position,1.0);\n\
                            }";
        const char* ps =    "#version 330 \n\
                            in    vec4    _outColor;\n\
                            void main()\n\
                            {\n\
                                gl_FragColor = _outColor;\n\
                            }";
        createProgram(vs, ps);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _lightDir   =   glGetUniformLocation(_program,  "_lightDir");
        _lightColor =   glGetUniformLocation(_program,  "_lightColor");
        _lightDiffuse = glGetUniformLocation(_program, "_lightDiffuse");
        _position   =   glGetAttribLocation(_program,   "_position");
        _normal     =   glGetAttribLocation(_program,   "_normal");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_normal);
        glUseProgram(_program);
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