#pragma once
#include "GLSLProgram.hpp"

class   ShaderDepth :public GLSLProgram
{
public:
    attribute   _position;
    attribute   _uv;
    uniform     _mvp;
    uniform     _tex;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 430 \n\
                            layout(location = 0) in     vec3    _position;\n\
                            layout(location = 1) in     vec2    _uv;\n\
                            layout(location = 0) uniform mat4   _mvp;\n\
                            layout(location = 0) out    vec2    _outUV;\n\
                            void main()\n\
                            {\n\
                                _outUV      =   _uv;\n\
                                gl_Position =   _mvp * vec4(_position,1.0);\n\
                            }";
        const char* ps =    "#version 430 \n\
            float restDepth(vec4 RGBA){\n\
            const float rMask = 1.0;\n\
            const float gMask = 1.0 / 255.0;\n\
            const float bMask = 1.0 / (255.0 * 255.0);\n\
            const float aMask = 1.0 / (255.0 * 255.0 * 255.0);\n\
            float depth = dot(RGBA, vec4(rMask, gMask, bMask, aMask));\n\
            return depth;\n\
        }\n\
                            layout(location = 0) in    vec2         _inUV;\n\
                            layout(location = 1) uniform sampler2D  _tex;\n\
                            void main()\n\
                            {\n\
                                vec4    color = texture2D(_tex,_inUV);\n\
                                float   dd = restDepth(color);\n\
                                gl_FragColor = vec4(dd,dd,dd,1);\n\
                            }";
        createProgram(vs, ps);

        _mvp        =   glGetUniformLocation(_program,"_mvp");
        _tex        =   glGetUniformLocation(_program,"_tex");
        _position   =   glGetAttribLocation(_program, "_position");
        _uv         =   glGetAttribLocation(_program, "_uv");
    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        glDisableVertexAttribArray(_uv);
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glDisableVertexAttribArray(_uv);
        glUseProgram(0);
    }


};
