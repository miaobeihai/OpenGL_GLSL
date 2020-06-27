#pragma once

#include "GLSLProgram.hpp"


class   ShaderColors: public GLSLProgram
{
public:
    virtual void    initialize()
    {
        const char* vs  =   "varying    vec4    outColor0;\
                            varying     vec4    outColor1;\
                            void main()\
                            {\
                                outColor0   =   gl_Color;\
                                outColor1   =   gl_SecondaryColor;\
                                gl_Position =   gl_ModelViewProjectionMatrix * gl_Vertex;\
                            }";
        const char* ps  =   "varying    vec4    outColor0;\
                            varying     vec4    outColor1;\
                            void main()\
                            {\
                                gl_FragColor    =   (outColor1  + outColor0);\
                            }";
        createProgram(vs, ps);

    }

    /**
    *    π”√shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
    }
    /**
    *   Ω· ¯
    */
    virtual void    end()
    {
        glUseProgram(0);
    }

};   