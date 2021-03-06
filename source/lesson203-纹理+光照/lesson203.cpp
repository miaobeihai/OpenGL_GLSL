
#include <windows.h>
#include <tchar.h>
#include <math.h>

#include "stb_image.h"
#include "OpenGLWindow.h"
#include "CELLMath.hpp"
#include "ShaderLight.hpp"
#include "Camera.hpp"
#include "ModelBox.hpp"
#

class   SamplerExt :public OpenGLWindow
{
public:
    ShaderLight     _shader;
    Camera          _camera;
    bool            _lButtonDown;
    bool            _rButtonDown;
    CELL::int2      _mouseDown;
    CELL::float3    _mouseRot;
    ModelBox        _modelBox;
    GLuint          _texGround;
    GLuint          _texBox;
public:

    struct  Vertex
    {
        float   x,y,z;
        float   nx,ny,nz;
        float   u,v;
    };

    SamplerExt()
        :_modelBox(float3(10,10,10))
    {   
        _lButtonDown    =   false;
        _rButtonDown    =   false;
    }
    void    onInitGL()
    {
        _camera.setUp(CELL::float3(0,0,1));
        _camera.setRight(CELL::float3(1,0,0));
        _camera.setTarget(CELL::real3(0,1,0));
        _camera.setEye(CELL::real3(0,100,0));
        _camera.update();
        _shader.initialize();
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_DEPTH_TEST);
        char    binPath[1024];
        getResourcePath(0,binPath);
        char    resPath[1024];
        sprintf(resPath,"%s../data/1.jpg",binPath);
        _texGround  =   createTextureFromImage(resPath);
        sprintf(resPath, "%s../data/Terrain.bmp", binPath);
        _texBox     =   createTextureFromImage(resPath);
        
    }
    void    render()
    {
        float   gSizeX = 500;
        float   gSizeY = 500;
        float   gPos = 0;
        float   rept = 10;

        Vertex grounds[] =
        {
            { -gSizeX, gPos,-gSizeY, 0,-1,0, 0.0f, 0.0f },
            { gSizeX, gPos,-gSizeY, 0,-1,0, rept, 0.0f },
            { gSizeX, gPos, gSizeY, 0,-1,0, rept, rept },

            { -gSizeX, gPos,-gSizeY, 0,-1,0, 0.0f, 0.0f },
            { gSizeX, gPos, gSizeY, 0,-1,0, rept, rept },
            { -gSizeX, gPos, gSizeY, 0,-1,0, 0.0f, rept },
        };
        _camera.setViewSize(_width, _height);
        _camera.perspective(45, (float)_width / (float)_height, 1, 100000);
        CELL::matrix4   mvp = _camera.getProject() * _camera.getView();
        glBindTexture(GL_TEXTURE_2D,_texGround);
        _shader.begin();

        glUniform1i(_shader._texture,0);
        glUniform3f(_shader._lightDiffuse, 0.1f, 0.1f, 0.1f);

        glUniformMatrix4fv(_shader._mvp, 1, GL_FALSE, mvp.data());
        glUniform3f(_shader._lightColor, 1.0f,1.0f,1.0f);
        glUniform3f(_shader._lightDir, _camera._dir.x, _camera._dir.y, _camera._dir.z);

        glVertexAttribPointer(_shader._position,3, GL_FLOAT, GL_FALSE, sizeof(Vertex), grounds);
        glVertexAttribPointer(_shader._normal,  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), &grounds[0].nx);
        glVertexAttribPointer(_shader._uv,      2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &grounds[0].u);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glBindTexture(GL_TEXTURE_2D,_texBox);
        #if 1
        static  float   angle = 0;

        //angle += 0.1f;
        CELL::matrix4   matRot;
        CELL::matrix4   matTrans;
        matTrans.translate(0, 20, 0);
        matRot.rotateYXZ(angle, angle, angle);
        mvp *= (matTrans *matRot);
        glUniform3f(_shader._lightColor, 1, 1.0f, 1.0f);
        glUniformMatrix4fv(_shader._mvp, 1, GL_FALSE, mvp.data());
        glVertexAttribPointer(_shader._position,3,  GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), _modelBox._data);
        glVertexAttribPointer(_shader._normal,  3,  GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), &_modelBox._data[0].nx);
        glVertexAttribPointer(_shader._uv,      2,  GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), &_modelBox._data[0].u);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        #endif
        _shader.end();
    }

    virtual LRESULT events(HWND hWnd, UINT msgId, WPARAM wParam, LPARAM lParam)
    {
        switch (msgId)
        {
        case WM_SIZE:
            {
                if (::IsWindow(hWnd))
                {
                    RECT    rt;
                    GetClientRect(_hWnd, &rt);
                    int     w = rt.right - rt.left;
                    int     h = rt.bottom - rt.top;
                    glViewport(0, 0, w, h);
                    _camera.setViewSize(w,h);
                }
            }
            break;
        case WM_RBUTTONUP:
            {
                _rButtonDown    =   false;
                ReleaseCapture();
            }
            break;
        case WM_RBUTTONDOWN:
            {
                SetCapture(hWnd);
                _rButtonDown    =   true;
                _mouseDown      =   int2(LOWORD(lParam), HIWORD(lParam));
                
                ///  计算和地面的交点
                Ray     ray = _camera.createRayFromScreen(_mouseDown.x, _mouseDown.y);

                float3  pos = ray.getOrigin();
                float   tm  = abs((pos.y - 0) / ray.getDirection().y);
                _mouseRot = ray.getPoint(tm);
                _mouseRot.y = 0;
            }
            break;
        case WM_LBUTTONDOWN:
            {
                SetCapture(hWnd);
                _lButtonDown    =   true;
                _mouseDown      =   int2(LOWORD(lParam), HIWORD(lParam));
            }
            break;
        
        case WM_LBUTTONUP:
            {
                ReleaseCapture();
                _lButtonDown    =   false;
            }
            break;
        case WM_MOUSEWHEEL:
            {
                POINT   pt;
                GetCursorPos(&pt);
                ScreenToClient(_hWnd, &pt);
                int     delta   =   (short)HIWORD(wParam);

                real    persent =   delta > 0 ? 1.1f : 0.9f;
                Ray     ray     =   _camera.createRayFromScreen(pt.x, pt.y);
                float3  pos     =   ray.getOrigin();
                float   tm      =   abs((pos.y - 0) / ray.getDirection().y);
                float3  center  =   ray.getPoint(tm);
                center.y = 0;

                _camera.scaleCameraByPos(center, persent);
            }
            break;

        case WM_MOUSEMOVE:
            {
                int2    mouseCur=   int2(LOWORD(lParam), HIWORD(lParam));
                if (_lButtonDown)
                {
                    int2    pos(mouseCur.x,mouseCur.y);
                    /**
                    *   首先计算出来一个像素和当前场景的比例
                    */
                    Ray   ray0      =   _camera.createRayFromScreen(pos.x,pos.y);
                    Ray   ray1      =   _camera.createRayFromScreen(_mouseDown.x,_mouseDown.y);

                    real3  pos0     =   calcIntersectPoint(ray0);
                    real3  pos1     =   calcIntersectPoint(ray1);

                    real3  offset = pos1 - pos0;
                    
             
                    _mouseDown      =   mouseCur;

                    real3  newEye   =   _camera.getEye() + offset;
                    real3  newTgt   =   _camera.getTarget() + offset;

                    _camera.setEye(newEye);
                    _camera.setTarget(newTgt);

                    _camera.update();
                }
                else if(_rButtonDown)
                {
                    int2    curPos(mouseCur.x, mouseCur.y);
                    int2    offset = curPos - _mouseDown;
                    _mouseDown = curPos;
                    //_camera.rotateViewYByCenter(offset.x * 0.5f, _mouseRot);
                    //_camera.rotateViewXByCenter(offset.y * 0.5f, _mouseRot);
                    _camera.rotateViewY(offset.x * 0.5f);
                    _camera.rotateViewX(offset.y * 0.5f);
                    _camera.update();
                }
            }
            break;
        }
        return  __super::events(hWnd, msgId, wParam, lParam);
    }
protected:

    float3      calcIntersectPoint(CELL::Ray& ray)
    {
        float3   pos = ray.getOrigin();
        float    tm = abs((pos.y) / ray.getDirection().y);
        float3   target = ray.getPoint(tm);
        return   float3(target.x, 0, target.z);
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
     *	调用freeimage加载图片
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
