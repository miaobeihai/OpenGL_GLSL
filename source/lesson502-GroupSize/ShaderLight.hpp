#pragma once

#include "GLSLProgram.hpp"

/// gl_Vertex,gl_Normal,
class   ShaderLight :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _lightDir;
    attribute   _position;
    attribute   _normal;
public:
    virtual void    initialize()
    {
        GLSLProgram::initialize();
        const char* vs  =   "#version 330 \n\
                            uniform mat4   _mvp;\n\
                            uniform vec3   _lightDir;\n\
                            in     vec3    _position;\n\
                            in     vec3    _normal;\n\
                            out    vec4    _outColor;\n\
                            void main()\n\
                            {\n\
                                float NDotL =   max(dot(_normal, _lightDir), 0.1); \n\
                                _outColor   =   vec4(NDotL,NDotL,NDotL,1.0);\n\
                                gl_Position =   _mvp * vec4(_position,1.0);\n\
                            }";
        const char* ps =    "#version 330 \n\
                            in    vec4    _outColor;\n\
                            void main()\n\
                            {\n\
                                gl_FragColor = _outColor;\n\
                            }";
        GLuint  vsHandle    =   createShader(vs,SHADER_VERTEX);
        GLuint  psHandle    =   createShader(ps,SHADER_FRAGMENT);
        attachShader(vsHandle);
        attachShader(psHandle);
        linkProgram();
        glUseProgram(_program);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _lightDir   =   glGetUniformLocation(_program,  "_lightDir");
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