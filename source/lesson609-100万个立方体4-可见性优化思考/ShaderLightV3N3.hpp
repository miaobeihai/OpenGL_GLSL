#pragma once

#include "GLSLProgram.hpp"

/// gl_Vertex,gl_Normal,
class   ShaderLightV3N3 :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _lightDir;
    uniform     _lightColor;
    attribute   _position;
    attribute   _normal;
    attribute   _local;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            uniform mat4   _mvp;\n\
                            uniform vec3   _lightDir;\n\
                            uniform vec3   _lightColor;\n\
                            in     mat4    _local;\n\
                            in     vec3    _position;\n\
                            in     vec3    _normal;\n\
                            out    vec4    _outColor;\n\
                            void main()\n\
                            {\n\
                                float NDotL =   max(dot(_normal, _lightDir), 0.0); \n\
                                _outColor   =   vec4(_lightColor * NDotL ,1);\n\
                                gl_Position =   (_mvp * _local) * vec4(_position,1.0);\n\
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
        _position   =   glGetAttribLocation(_program,   "_position");
        _normal     =   glGetAttribLocation(_program,   "_normal");
        _local      =   glGetAttribLocation(_program,   "_local");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_normal);
        glEnableVertexAttribArray(_local + 0);
        glEnableVertexAttribArray(_local + 1);
        glEnableVertexAttribArray(_local + 2);
        glEnableVertexAttribArray(_local + 3);
        
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_normal);
        glDisableVertexAttribArray(_local + 0);
        glDisableVertexAttribArray(_local + 1);
        glDisableVertexAttribArray(_local + 2);
        glDisableVertexAttribArray(_local + 3);
        glUseProgram(0);
    }


};