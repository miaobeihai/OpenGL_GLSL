#pragma once

#include "GLSLProgram.hpp"

/// gl_Vertex,gl_Normal,
class   ShaderLightNM :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _model;
    uniform     _view;
    uniform     _noMatrix;
    uniform     _lightPos;
    uniform     _lightColor;
    uniform     _lightDiffuse;
    uniform     _texture;
    uniform     _texture1;
    attribute   _position;
    attribute   _normal;
    attribute   _uv;
    attribute   _tagent;
    attribute   _binoraml;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            in      vec3    _position;\n\
                            in      vec3    _normal;\n\
                            in      vec2    _uv;\n\
                            in      vec3    _tagent;\n\
                            in      vec3    _biTagent;\n\
                            out     vec2    _outUV;\n\
                            out     vec3    _outPos;\n\
                            out     vec3    _outNormal;\n\
                            out     vec3    _outTagent;\n\
                            out     vec3    _outBiTagent;\n\
                            uniform mat4    _mvp;\n\
                            uniform mat4    _matWorld;\n\
                            void main()\n\
                            {\n\
                                _outUV          =   _uv;\n\
                                vec4    pos     =   _matWorld*vec4(_position,1);\n\
                                _outPos         =   pos.xyz;\n\
                                _outNormal      =   normalize(_normal);\n\
                                _outTagent      =   normalize(_tagent);\n\
                                _outBiTagent    =   normalize(_biTagent);\n\
                                gl_Position     =   _mvp * vec4(_position,1.0);\n\
                            }";
        const char* ps =    "#version 330 \n\
                            in      vec2        _outUV;\n\
                            in      vec3        _outPos;\n\
                            in      vec3        _outNormal;\n\
                            in      vec3        _outTagent;\n\
                            in      vec3        _outBiTagent;\n\
                            uniform sampler2D   _texture;\n\
                            uniform sampler2D   _texNormal;\n\
                            uniform mat3        _normalMatrix;;\n\
                            uniform vec3        _lightPos;\n\
                            void main()\n\
                            {\n\
                                gl_FragColor.rgb    =  texture2D(_texture, _outUV).rgb;\n\
                                mat3    TBN         =   mat3x3(normalize(_outTagent), normalize(_outBiTagent), normalize(_outNormal));\n\
                                mat3    norMapT     =   transpose(_normalMatrix);\n\
                                vec3    normal      =   normalize( norMapT * (TBN * (texture2D(_texNormal, _outUV).rgb * 2.0 - 1.0)));\n\
                                vec3    lightDir    =   normalize(_lightPos - _outPos);\n\
                                float   NdotLD      =   max(dot(normal, _lightPos), 0.0);\n\
                                gl_FragColor.rgb    *=  0.1 + 0.7 * NdotLD;\n\
                            }";
        createProgram(vs, ps);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _model      =   glGetUniformLocation(_program,  "_matWorld");
        _noMatrix   =   glGetUniformLocation(_program,  "_normalMatrix");
        _lightPos   =   glGetUniformLocation(_program,  "_lightPos");
        _texture    =   glGetUniformLocation(_program,  "_texture");
        _texture1   =   glGetUniformLocation(_program,  "_texNormal");

        _position   =   glGetAttribLocation(_program,   "_position");
        _normal     =   glGetAttribLocation(_program,   "_normal");
        _uv         =   glGetAttribLocation(_program,   "_uv");
        _tagent     =   glGetAttribLocation(_program,   "_tagent");
        _binoraml   =   glGetAttribLocation(_program,   "_biTagent");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        glEnableVertexAttribArray(_normal);
        glEnableVertexAttribArray(_uv);
        glEnableVertexAttribArray(_tagent);
        glEnableVertexAttribArray(_binoraml);
        
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_normal);
        glDisableVertexAttribArray(_uv);
        glDisableVertexAttribArray(_tagent);
        glDisableVertexAttribArray(_binoraml);
        glUseProgram(0);
    }


};