
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "OpenGLWindow.h"

class   Sampler1 :public OpenGLWindow
{
public:

    struct  Vertex
    {
        float   x,y,z;
        float   r,g,b;
    };

    char*   _pixel;

    Sampler1()
    {
        
    }
    virtual void    onInitGL()
    {
        
    }
    virtual void    render()
    {

        char*   ext =   (char*)glGetString(GL_EXTENSIONS);
        #define M_PI (3.14159265358979323846)

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,0,1);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  1,  0,  0},
            {110,   10,     0,  0,  1,  0},
            {10,    110,    0,  0,  0,  1},
            {110,   110,    0,  1,  0,  1},
        };
        glColor3f(1,0,1);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3,GL_FLOAT,sizeof(Vertex),rect);
        glColorPointer(3,GL_FLOAT,sizeof(Vertex),&rect[0].r);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);


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
   
    Sampler1    instance;
    instance.main(800,600);
    return  0;
}