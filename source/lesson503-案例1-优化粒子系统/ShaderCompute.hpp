#pragma once

#include "GLSLProgram.hpp"

class   ShaderCompute :public GLSLProgram
{
public:
    uniform _maxSize;
    uniform _allElapsed;
    uniform _frameElapsed;
public:
    virtual void    initialize()
    {
        GLSLProgram::initialize();
        const char* cs  =   "#version 430 core\n\
                            layout(local_size_x = 64,local_size_y = 1,local_size_z = 1) in;\n\
                            struct  DataIn\n\
                            {\n\
                                vec4    pos;\n\
                                vec4    vel;\n\
                            };\n\
                            layout(binding = 0) buffer calcBuf\n\
                            {\n\
                                DataIn datas[];\n\
                            }In;\n\
                            uniform uint    maxSize;\n\
                            uniform float   allElapsed;\n\
                            uniform float   frameElapsed;\n\
                            void main()\n\
                            {\n\
                                uint index  = gl_GlobalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y + gl_GlobalInvocationID.y * gl_WorkGroupSize.x +gl_GlobalInvocationID.x;\n\
                                if(index < maxSize)\n\
                                if(allElapsed >=2.0)\n\
                                {\n\
                                    In.datas[index].pos = vec4(0,0,0,0);\n\
                                }\n\
                                else\n\
                                {\n\
                                    In.datas[index].pos+= In.datas[index].vel * frameElapsed;\n\
                                }\n\
                            }";
        GLuint  csHandle    =   createShader(cs,SHADER_CS);
        attachShader(csHandle);
        linkProgram();
        _maxSize        = glGetUniformLocation(_program, "maxSize");
        _allElapsed     = glGetUniformLocation(_program, "allElapsed");
        _frameElapsed   = glGetUniformLocation(_program, "frameElapsed");
    }
};
