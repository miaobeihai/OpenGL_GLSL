#pragma once

#include "GLSLProgram.hpp"

class   ShaderLightNM3 :public GLSLProgram
{
public:
    uniform     _mvp;
    uniform     _model;
    uniform     _invModel;
    uniform     _lightPos;
    uniform     _cameraPos;
    uniform     _texture;
    uniform     _texture1;
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
                            uniform vec3    _lightPos;\n\
                            uniform vec3    _cameraPos;\n\
                            in      vec3    _position;\n\
                            in      vec3    _normal;\n\
                            in      vec2    _uv;\n\
                            in      vec3    _tagent;\n\
                            in      vec3    _biTagent;\n\
                            out     vec2    _outUV;\n\
                            out     vec3    _eyeDir;\n\
                            out     vec3    _lightDir;\n\
                            void main(void) {\n\
                                vec3    pos     =   (_model * vec4(_position, 0.0)).xyz;\n\
                                vec3    invEye  =   (_invModel * vec4(_cameraPos, 0.0)).xyz;\n\
                                vec3    invLight=   (_invModel * vec4(_lightPos, 0.0)).xyz;\n\
                                vec3    eye     =   invEye - pos;\n\
                                vec3    light   =   invLight - pos;\n\
                                vec3    n       =   normalize(_normal);\n\
                                vec3    t       =   normalize(_tagent);\n\
                                vec3    b       =   normalize(_biTagent);\n\
                                //mat3    tbn     =   transpose(mat3x3(t,b,n));\n\
                                _eyeDir.x   =   dot(t, eye);\n\
                                _eyeDir.y   =   dot(b, eye);\n\
                                _eyeDir.z   =   dot(n, eye);\n\
                                _eyeDir     =   normalize(_eyeDir);\n\
                                _lightDir.x =   dot(t, light);\n\
                                _lightDir.y =   dot(b, light);\n\
                                _lightDir.z =   dot(n, light);\n\
                                _lightDir   =   normalize(_lightDir);\n\
                                _outUV      =   _uv;\n\
                                gl_Position =   _mvp * vec4(_position, 1.0);\n\
                            }";
        const char* ps =    "#version 330 \n\
                            uniform sampler2D _texture;\n\
                            uniform sampler2D _texture1;\n\
                            uniform float     _height;\n\
                            in      vec2      _outUV;\n\
                            in      vec3      _eyeDir;\n\
                            in      vec3      _lightDir;\n\
                            void main(void) {\n\
                                float hScale    =   texture2D(_texture1, _outUV).r * _height;\n\
                                vec2  hTexCoord =   _outUV - hScale * _eyeDir.xy;\n\
                                vec3  normal    =   (texture2D(_texture, hTexCoord) * 2.0 - 1.0).rgb;\n\
                                vec3  lightDirRef =   reflect(-_lightDir, normal);\n\
                                float diffuse   =   max(dot(normal, _lightDir), 0.0);\n\
                                float specular  =   pow(max(dot(normal, lightDirRef), 0.0), 64.0);\n\
                                gl_FragColor    =   vec4(vec3(diffuse), 1.0) + vec4(vec3(specular), 1.0);\n\
                            }";
        createProgram(vs, ps);
        _mvp        =   glGetUniformLocation(_program,  "_mvp");
        _model      =   glGetUniformLocation(_program,  "_model");
        _invModel   =   glGetUniformLocation(_program,  "_invModel");
        _cameraPos  =   glGetUniformLocation(_program,  "_cameraPos");
        _lightPos   =   glGetUniformLocation(_program,  "_lightPos");
        _texture    =   glGetUniformLocation(_program,  "_texture");
        _texture1   =   glGetUniformLocation(_program,  "_texture1");
        _height     =   glGetUniformLocation(_program,  "_height");

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