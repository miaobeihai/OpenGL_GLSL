
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "glew/glew.h"
#include "OpenGLWindow.h"
#include "GLSLProgram.hpp"
#include "CELLMath.hpp"
#include "stb_image.h"
using namespace CELL;

struct Particle
{
    float3  _curPos;
    float3  _curVel;
    unsigned char   _r;
    unsigned char   _g;
    unsigned char   _b;
    unsigned char   _a;
};

const int MAX_PARTICLES = 100;
class   SamplerExt :public OpenGLWindow
{
public:
    ShaderVertex    _shader;
    GLuint          _texure0;
    CELL::float2    _uvOffset;
    double          _hasElasped;
    float           _dir;

    Particle        _particles[MAX_PARTICLES];
    double          _allHasElapsed;
public:
    SamplerExt()
    {  
        _dir            =   1.0f;
        _allHasElapsed  =   0;
    }
    virtual void    onInitGL()
    {
        glewInit();
        _shader.initialize();
        glEnable(GL_TEXTURE_2D);

        char    binPath[1024];
        getResourcePath(0, binPath);
        char    resPath[1024];
        sprintf(resPath, "%s../data/particle.bmp", binPath);
        _texure0 =   createTextureFromImage(resPath);

        glEnable(GL_PROGRAM_POINT_SIZE);
        glEnable(GL_POINT_SPRITE_ARB);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        for (int i = 0; i < MAX_PARTICLES; ++i)
        {
            _particles[i]._curPos = float3(0.0f, 0.0f, 0.0f);
            _particles[i]._curVel = getRandomVector() * rangeRandom(0.5f, 5.0f);

            _particles[i]._r = rand() % 255;
            _particles[i]._g = rand() % 255;
            _particles[i]._b = rand() % 255;
            _particles[i]._a = 255;
        }

    }
    void updatePointSprites(double dElpasedFrameTime)
    {
        _allHasElapsed  +=  dElpasedFrameTime;
        if (_allHasElapsed >= 5.0f)
        {
            for (int i = 0; i < MAX_PARTICLES; ++i)
                _particles[i]._curPos = float3(0.0f, 0.0f, 0.0f);

            _allHasElapsed = dElpasedFrameTime;
        }

        for (int i = 0; i < MAX_PARTICLES; ++i)
            _particles[i]._curPos += _particles[i]._curVel * (float)dElpasedFrameTime;
    }

    float3      getRandomVector(void)
    {
        float3 vVector;

        vVector.z = rangeRandom(-1.0f, 1.0f);

        float radius = (float)sqrt(1 - vVector.z * vVector.z);

        float t = rangeRandom(-PI, PI);

        vVector.x = (float)cosf(t) * radius;
        vVector.y = (float)sinf(t) * radius;

        return vVector;
    }
    virtual void    render(double fElapsed)
    {
        using namespace CELL;
       
        updatePointSprites(fElapsed);
        CELL::matrix4   model;
        model.translate(0,0,-10);
        CELL::matrix4   prj =   CELL::perspective<float>(45.0f, double(_width) / double(_height), 0.1f, 100.0f);
        CELL::matrix4   mvp =   prj * model;
        

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,_texure0);

        _shader.begin();
        glUniformMatrix4fv(_shader._mvp,1,GL_FALSE,mvp.data());
        glUniform1i(_shader._texture0,0);
        glVertexAttribPointer(_shader._pos,     3,   GL_FLOAT,          GL_FALSE,   sizeof(Particle),_particles);
        glVertexAttribPointer(_shader._color,   4,   GL_UNSIGNED_BYTE,  GL_TRUE,    sizeof(Particle),&_particles[0]._r);
        
        glDrawArrays(GL_POINTS,0,MAX_PARTICLES);
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