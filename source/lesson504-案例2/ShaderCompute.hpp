#pragma once

#include "GLSLProgram.hpp"

class   ShaderCompute :public GLSLProgram
{
public:
    virtual void    initialize()
    {
        GLSLProgram::initialize();
        const char* cs  =   "#version 430 core\n\
                            layout(local_size_x = 10,local_size_y = 1,local_size_z = 1) in;\n\
                            layout(binding = 1) buffer calcBuf\n\
                            {\n\
                                float datas[];\n\
                            }In;\n\
                            void main()\n\
                            {\n\
                                uint index  = gl_GlobalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y + gl_GlobalInvocationID.y * gl_WorkGroupSize.x +gl_GlobalInvocationID.x;\n\
                                In.datas[index]   =   In.datas[index] * In.datas[index];\n\
                            }";
        GLuint  csHandle    =   createShader(cs,SHADER_CS);
        attachShader(csHandle);
        linkProgram();
    }
};
