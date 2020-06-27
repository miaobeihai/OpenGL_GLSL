#pragma once

#include "GLSLProgram.hpp"

class   ShaderNormalCubeMap :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _model;
    uniform     _invModel;
    uniform     _lightPos;
    uniform     _cameraPos;
    uniform     _texture;
    uniform     _textureCube;
    uniform     _height;
    attribute   _position;
    attribute   _normal;
    attribute   _uv;
    attribute   _tagent;
    attribute   _biTagent;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            uniform mat4    _mvp; \n\
                            uniform mat4    _model;\n\
                            uniform mat4    _invModel;\n\
                            uniform vec3    _cameraPos;\n\
                            in      vec3    _position;\n\
                            in      vec3    _normal;\n\
                            in      vec2    _uv;\n\
                            in      vec3    _tagent;\n\
                            in      vec3    _biTagent;\n\
                            out     vec2    _outUV;\n\
                            out     vec3    _eyeDir;\n\
                            out     mat3    _tbn;\n\
                            void main(void) {\n\
                                vec3    pos     =   (_model * vec4(_position, 0.0)).xyz;\n\
                                vec3    invEye  =   (_invModel * vec4(_cameraPos, 0.0)).xyz;\n\
                                vec3    eye     =   invEye - pos;\n\
                                vec3    n       =   normalize(_normal);\n\
                                vec3    t       =   normalize(_tagent);\n\
                                vec3    b       =   normalize(_biTagent);\n\
                                _tbn        =   mat3(t,b,n);\n\
                                _eyeDir.x   =   dot(t, eye);\n\
                                _eyeDir.y   =   dot(b, eye);\n\
                                _eyeDir.z   =   dot(n, eye);\n\
                                _eyeDir     =   normalize(_eyeDir);\n\
                                _outUV      =   _uv;\n\
                                gl_Position =   _mvp * vec4(_position, 1.0);\n\
                            }";
        const char* ps =    "#version 120 \n\
                            uniform sampler2D   _texture;\n\
                            uniform samplerCube _cubeTexture;\n\
                            in      vec2        _outUV;\n\
                            in      vec3        _eyeDir;\n\
                            in      mat3        _tbn;\n\
                            void main(void) {\n\
                                vec3    normal      =   _tbn * (texture2D(_texture, _outUV) * 2.0 - 1.0).rgb;\n\
                                vec3    ref         =   reflect(_eyeDir, normal);\n\
                                vec4    envColor    =   textureCube(_cubeTexture, ref);\n\
                                gl_FragColor        =   envColor;\n\
                            }";
        createProgram(vs, ps);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _model      =   glGetUniformLocation(_program,  "_model");
        _invModel   =   glGetUniformLocation(_program,  "_invModel");
        _cameraPos  =   glGetUniformLocation(_program,  "_cameraPos");
        _texture    =   glGetUniformLocation(_program,  "_texture");
        _textureCube=   glGetUniformLocation(_program,  "_cubeTexture");

        _position   =   glGetAttribLocation(_program,   "_position");
        _normal     =   glGetAttribLocation(_program,   "_normal");
        _uv         =   glGetAttribLocation(_program,   "_uv");
        _tagent     =   glGetAttribLocation(_program,   "_tagent");
        _biTagent   =   glGetAttribLocation(_program,   "_biTagent");
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
        glEnableVertexAttribArray(_biTagent);
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
        glDisableVertexAttribArray(_biTagent);
        glUseProgram(0);
    }

};