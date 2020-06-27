
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"



class   SamplerExt :public OpenGLWindow
{
public:
    ShaderVertex _shader;
public:

    struct  Vertex
    {
        float   x,y,z;
        unsigned char   r,g,b;
    };

    char*   _pixel;

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

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,0,1);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  255,  0,  0},
            {110,   10,     0,  0,  255,  0},
            {10,    110,    0,  0,  0,  255},
            {110,   110,    0,  255,  0,  255},
        };

        _shader.begin();
        glUniform4f(_shader._ucolor,0,1,0,1);
        glEnableVertexAttribArray(_shader._position);
        glVertexAttribPointer(_shader._position,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),rect);

        glEnableVertexAttribArray(_shader._color);
        glVertexAttribPointer(_shader._color,3,GL_UNSIGNED_BYTE,GL_TRUE,sizeof(Vertex),&rect[0].r);
        
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        glDisableVertexAttribArray(_shader._position);
        _shader.end();


        Vertex  rect1[]  =   
        {
            {10,    10,     0,  1,  0,  0},
            {110,   10,     0,  0,  1,  0},
            {10,    110,    0,  0,  0,  1},
            {110,   110,    0,  1,  0,  1},
        };

        for (int i = 0 ;i < 4 ; ++ i )
        {
            rect1[i].x += 100;
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),rect1);
        glColorPointer(3,GL_FLOAT,sizeof(Vertex),&rect1[0].r);
        glColor3f(0,0,1);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);


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