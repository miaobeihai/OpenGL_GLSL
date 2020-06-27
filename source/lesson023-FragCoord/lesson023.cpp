
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
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
    }
    virtual void    render()
    {
    static  float   angle   =   0;
    angle   +=  0.1f;
        CELL::matrix4   matRot;
        matRot.rotateYXZ(angle,angle,angle);
        CELL::matrix4   prj =   CELL::ortho<float>(0,_width,_height,0,-100,100);
        CELL::matrix4   mvp =   prj * matRot;
        _shader.begin();
        glUniformMatrix4fv(_shader._mvp,1,GL_FALSE,mvp.data());
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