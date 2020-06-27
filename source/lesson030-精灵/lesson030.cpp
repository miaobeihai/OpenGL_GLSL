
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "CELLMath.hpp"
#include "stb_image.h"

class   SamplerExt :public OpenGLWindow
{
public:
    struct  Vertex
    {
        float   x, y, z;
        float   u, v;
    };
public:
    ShaderVertex    _shader;
    GLuint          _texure0;
    CELL::float2    _uvOffset;
    double          _hasElasped;
    float           _dir;
public:
    SamplerExt()
    {  
        _dir        =   1.0f;
    }
    virtual void    onInitGL()
    {
        glewInit();
        _shader.initialize();
        glEnable(GL_TEXTURE_2D);

        char    binPath[1024];
        getResourcePath(0, binPath);
        char    resPath[1024];
        sprintf(resPath, "%s../data/grass.png", binPath);
        _texure0 =   createTextureFromImage(resPath);

        glEnable(GL_PROGRAM_POINT_SIZE);
        //glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
        //glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_UPPER_LEFT);
        glEnable(GL_POINT_SPRITE_ARB);

        //glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    }
    virtual void    render(double fElapsed)
    {
        using namespace CELL;
        Vertex  rect[] =
        {
            {10,    10,     0,  0,  1},
            {410,   10,     0,  1,  1},
            {10,    410,    0,  0,  0},
            {410,   410,    0,  1,  0},
        };
       
        CELL::matrix4   prj =   CELL::ortho<float>(0,_width,_height,0,-100,100);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,_texure0);

        _shader.begin();
        glUniformMatrix4fv(_shader._mvp,1,GL_FALSE,prj.data());
        glUniform1i(_shader._texture0,0);
        glVertexAttribPointer(_shader._pos,3,   GL_FLOAT,GL_FALSE,sizeof(Vertex),rect);
        
        glDrawArrays(GL_POINTS,0,4);
        _shader.end();
    }

    void        getResourcePath(HINSTANCE hInstance, char pPath[1024])
    {
        char    szPathName[1024];
        char    szDriver[64];
        char    szPath[1024];
        GetModuleFileNameA(hInstance, szPathName, sizeof(szPathName));
        _splitpath(szPathName, szDriver, szPath, 0, 0);
        sprintf(pPath, "%s%s", szDriver, szPath);
    }

    /**
    *	µ˜”√freeimageº”‘ÿÕº∆¨
    */
    GLuint      createTextureFromImage(const char* fileName)
    {
        int     width;
        int     height;
        int     chanel;
        GLuint  texId;
        stbi_uc*    pixels  =   stbi_load(fileName, &width, &height, &chanel, 0);
        if (chanel == 3)
        {
            texId   =   createTexture(width, height, pixels, GL_RGB);
        }
        else
        {
            texId   =   createTexture(width, height, pixels, GL_RGBA);
        }
        free(pixels);

        return  texId;
    }

    unsigned    createTexture(int w, int h, const void* data, GLenum type)
    {
        unsigned    texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, data);


        return  texId;
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