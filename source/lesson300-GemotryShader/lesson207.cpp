
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "ShaderTessllation.hpp"
#include "CELLMath.hpp"
#include "stb_image.h"

class   SamplerExt :public OpenGLWindow
{
public:
    ShaderTessllation   _shader;
    GLuint              _vbo;
    GLuint              _texId;
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
        glewInit();
        _shader.initialize();

        _texId  =   createTextureFromeFile("image/2.jpg");

        Vertex  quad[] =
        {
            { 10,   10, 0,  0,  1},
            { 210,  10, 0,  1,  1},
            { 210,  210,0,  1,  0},
            { 10,   210,0,  0,  0},


            { 210,  210, 0,  0,  1 },
            { 410,  210, 0,  1,  1 },
            { 410,  410,0,  1,  0 },
            { 210,  410,0,  0,  0 },

        };
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    }
    virtual void    render()
    {

        //! 指定以下的操作针对投影矩阵
        glMatrixMode(GL_PROJECTION);
        //! 将投影举证清空成单位矩阵
        glLoadIdentity();
        glOrtho(0,_width,_height,0,-100,100);

        CELL::matrix4   mat = CELL::ortho<float>(0, _width, _height, 0, -100, 100);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glBindTexture(GL_TEXTURE_2D, _texId);
        glEnable(GL_TEXTURE_2D);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        _shader.begin();
            glUniformMatrix4fv(_shader._mvp, 1, 0, mat.data());
            glUniform1i(_shader._texture,0);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
            glPatchParameteri(GL_PATCH_VERTICES, 4);
            glDrawArrays(GL_PATCHES, 0, 8);

        _shader.end();
    }


protected:
    GLuint  createTextureFromeFile(const char* fileName)
    {
        int     width, height, n;
        GLuint  texId   =   0; 
        stbi_uc*    pixels = stbi_load(fileName, &width, &height, &n, 0);
        if (n == 3)
        {
            texId   =  createTexture(width, height, pixels, GL_RGB);
        }
        else if (n == 4)
        {
            texId = createTexture(width, height, pixels, GL_RGB);
        }
        free(pixels);
        return  texId;
    }

    GLuint  createTexture(int w, int h, const void* data, int fmt)
    {
        GLuint  texId;
        glGenTextures(1, &texId);
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);

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