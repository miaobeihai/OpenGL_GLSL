#pragma once

#include "GLSLProgram.hpp"


/// gl_Vertex,gl_Normal,
class   ShaderCULL :public GLSLProgram
{
public:
    uniform     _FRUSTUM_LEFT;
    uniform     _FRUSTUM_RIGHT;
    uniform     _FRUSTUM_TOP;
    uniform     _FRUSTUM_BOTTOM;
    uniform     _FRUSTUM_FAR;
    uniform     _FRUSTUM_NEAR;
    attribute   _sphere;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 150 core \n\
                            uniform vec4    _FRUSTUM_LEFT;\n\
                            uniform vec4    _FRUSTUM_RIGHT;\n\
                            uniform vec4    _FRUSTUM_TOP;\n\
                            uniform vec4    _FRUSTUM_BOTTOM;\n\
                            uniform vec4    _FRUSTUM_FAR;\n\
                            uniform vec4    _FRUSTUM_NEAR;\n\
                            in      vec4    _sphere;\n\
                            out     float   _outValue;\n\
                            bool    dist(vec4 plan,vec4 pos)\n\
                            {\n\
                                return  dot(plan.xyz,pos.xyz) + plan.w <= -pos.w;\n\
                            }\n\
                            void main(void )\n\
                            {\n\
                                _outValue   =   1.0f;\n\
                                if (dist(_FRUSTUM_LEFT,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                else if (dist(_FRUSTUM_RIGHT,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                else if (dist(_FRUSTUM_TOP,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                else if (dist(_FRUSTUM_BOTTOM,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                else if (dist(_FRUSTUM_FAR,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                else if (dist(_FRUSTUM_NEAR,_sphere))\n\
                                   _outValue   =   0.0;\n\
                                   _outValue   =   1.0f;\n\
                            }";
        GLchar* fbv[]   =   { "_outValue" };
        GLuint  shader  =   createShader(vs,SHADER_VERTEX);
        _program        =   glCreateProgram();
        
        glAttachShader(_program,shader);
        glTransformFeedbackVaryings(_program, 1, fbv, GL_INTERLEAVED_ATTRIBS);
        GLint   status  =   0;
        char    compileLog[1024] = { 0 };
        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetProgramInfoLog(_program,sizeof(compileLog),0,compileLog);
        }
        glUseProgram(_program);
        glTransformFeedbackVaryings(_program, 0, 0, GL_INTERLEAVED_ATTRIBS);

        _FRUSTUM_LEFT   =   glGetUniformLocation(_program, "_FRUSTUM_LEFT");
        _FRUSTUM_RIGHT  =   glGetUniformLocation(_program, "_FRUSTUM_RIGHT");
        _FRUSTUM_TOP    =   glGetUniformLocation(_program, "_FRUSTUM_TOP");
        _FRUSTUM_BOTTOM =   glGetUniformLocation(_program, "_FRUSTUM_BOTTOM");
        _FRUSTUM_FAR    =   glGetUniformLocation(_program, "_FRUSTUM_FAR");
        _FRUSTUM_NEAR   =   glGetUniformLocation(_program, "_FRUSTUM_NEAR");
        _sphere         =   glGetAttribLocation(_program,"_sphere");
    }

    /**
    *   Ê¹ÓÃshader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_sphere);
        
    }
    /**
    *   ½áÊø
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_sphere);
        glUseProgram(0);
    }


};