#pragma once

#include "GLSLProgram.hpp"

/// gl_Vertex,gl_Normal,
class   ShaderCULL :public GLSLProgram
{
public:
    uniform     _mvp;
    attribute   _position;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 330 \n\
                            in     vec3     _position;\n\
                            out    uint      _outVal;\n\
                            void main()\n\
                            {\n\
                                _outVal =   uint(1<<3);\n\
                            }";
        GLchar* fbv[]   =   { "_outVal" };
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
        _position   =   glGetAttribLocation(_program,   "_position");
    }

    /**
    *   Ê¹ÓÃshader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
        
    }
    /**
    *   ½áÊø
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glUseProgram(0);
    }


};