
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "../lesson400-ComputeShader/CELLMath.hpp"



class   SamplerExt :public OpenGLWindow
{
public:
    ShaderVertex _shader;
public:
    SamplerExt()
    {  
    }
    virtual void    onInitGL()
    {
        glewInit();
        _shader.initialize();
    }
    virtual void    render()
    {
        CELL::matrix4   prj =   CELL::ortho<float>(0,_width,_height,0,-100,100);
        _shader.begin();
        glUniformMatrix4fv(_shader._mvp,1,GL_FALSE,prj.data());
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        _shader.end();
    }
};

int __stdcall WinMain( 
                      HINSTANCE hInstance, 
                      HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, 
                      int nShowCmd
                      )
{
   
    SamplerExt    instance;
    instance.main(800,600);


    return  0;
}