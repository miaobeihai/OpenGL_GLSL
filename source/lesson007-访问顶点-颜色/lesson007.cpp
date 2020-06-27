
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "ShaderColors.hpp"

class   SamplerExt :public OpenGLWindow
{
public:
    ShaderColors _shader;
public:

    struct  Vertex
    {
        float   x,y,z;
        unsigned char   r0,g0,b0,a0;
        unsigned char   r1,g1,b1,a1;
    };


    SamplerExt()
    {
        
    }
    virtual void    onInitGL()
    {
        _shader.initialize();
    }
    virtual void    render()
    {

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);



        Vertex  rect[]  =   
        {
            {10,    10,     0,  255,  0,  0,  255,  255,  0,  0,  255},
            {510,   10,     0,  0,  255,  0,  255,  255,  0,  0,  255},
            {10,    510,    0,  0,  0,  255,  255,  255,  0,  0,  255},
            {510,   510,    0,  255,  0,  255,  255,  255,  0,  0,  255},
        };


        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnableClientState(GL_SECONDARY_COLOR_ARRAY);

        _shader.begin();

        glVertexPointer(        3,  GL_FLOAT,   sizeof(Vertex),     rect);
        glColorPointer(         4,  GL_UNSIGNED_BYTE,   sizeof(Vertex),     &rect[0].r0);
        glSecondaryColorPointer(4,  GL_UNSIGNED_BYTE,   sizeof(Vertex),     &rect[0].r1);

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        _shader.end();

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_SECONDARY_COLOR_ARRAY);

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