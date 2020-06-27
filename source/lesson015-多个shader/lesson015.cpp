
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
    ShaderVertex    _shader;
    ShaderVertex1   _shader1;
    GLuint          _texture;
public:

    struct  Vertex
    {
        float   x,y,z;
        float   r,g,b;
        float   u,v;
    };

    char*   _pixel;

    SamplerExt()
    {
        
    }
    virtual void    onInitGL()
    {
        glewInit();
        _shader.initialize();
        _shader1.initialize();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        char    binPath[1024];
        getResourcePath(0,binPath);
        char    resPath[1024];
        sprintf(resPath,"%s../data/1.jpg",binPath);
       _texture =   createTextureFromImage(resPath);
    }
    virtual void    render()
    {
        CELL::matrix4   prj =   CELL::ortho<float>(0,_width,_height,0,-100,100);

        Vertex  rect[]  =   
        {
            {10,    10,     0,  1,  1,  1,  1,0},
            {210,   10,     0,  1,  1,  1,  1,1},
            {10,    210,    0,  1,  1,  1,  0,0},
            {210,   210,    0,  1,  1,  1,  0,1},
        };

        glBindTexture(GL_TEXTURE_2D,_texture);

        _shader.begin();
        {
            glUniform1i(_shader._tex, 0);
            glUniformMatrix4fv(0, 1, GL_FALSE, prj.data());

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), rect);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &rect[0].r);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &rect[0].u);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
        
        _shader.end();


        _shader1.begin();
        {
            glUniformMatrix4fv(0, 1, GL_FALSE, prj.data());

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), rect);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &rect[0].r);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &rect[0].u);

            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
        _shader1.end();


    }
protected:

    void  getResourcePath(HINSTANCE hInstance, char pPath[1024])
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
    GLuint  createTextureFromImage(const char* fileName)
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
