
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "ShaderTexcoord.hpp"
#include "GLFreeImage.hpp"



class   SamplerExt :public OpenGLWindow
{
public:
    ShaderTexcoord  _shader;
    GLuint          _texure;
public:

    struct  Vertex
    {
        float   x,y,z;
        float   u,v;
    };

    SamplerExt()
    {
        
    }
    virtual void    onInitGL()
    {
        char    szResourcePath[1024];
        char    szImage[1024];
        getResourcePath(szResourcePath);
        sprintf(szImage,"%s/../data/%s",szResourcePath,"Terrain.bmp");
        _texure =   GLFreeImage::createTextureFromImage(szImage);
        glEnable(GL_TEXTURE_2D);

        _shader.initialize();
    }
    virtual void    render()
    {

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);


        glColor3f(1,1,1);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  0,  1},
            {410,   10,     0,  1,  1},
            {10,    410,    0,  0,  0},
            {410,   410,    0,  1,  0},
        };

        /// 邦定纹理
        glBindTexture(GL_TEXTURE_2D,_texure);

        _shader.begin();
        glUniform4f(_shader._color,1,1,1,1);
        glUniform1i(_shader._texture,0);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glVertexPointer(3,  GL_FLOAT,   sizeof(Vertex), rect);
        glTexCoordPointer(2,GL_FLOAT,   sizeof(Vertex), &rect[0].u);
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