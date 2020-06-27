#pragma once

#include "GLSLProgram.hpp"

class   ShaderCompute :public GLSLProgram
{
public:
    virtual void    initialize()
    {
        GLSLProgram::initialize();
        const char* cs  =   "#version 430 core\n\
                            void main()\n\
                            {\n\
                            }";
        GLuint  csHandle    =   createShader(cs,SHADER_CS);
        attachShader(csHandle);
        linkProgram();
    }
};
