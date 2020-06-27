#pragma once

#include "CELLMath.hpp"
#include "GLSLProgram.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "ShaderBillboard.hpp"
/**
 *	该类主要目的是实现广告牌的绘制
 */
class   Billboard
{
public:
    /**
    *   大小,你在屏幕上看到的草大小信息,宽度和高度
    */
    float2      _size;
    /**
    *   位置,在场景中的位置信息
    */
    float3      _pos;
    

public:
    Billboard()
    {
    }
public:

    /**
     *	参数,摄像机,因为草是一个布告板,需要面向摄像机,用到摄像机的水平方向
     *  摄像机有三个轴 可以认为是xyz,但一般叫做up  ,right,lookat
     *  fElapsed,时间,绘制一帧需要的时间,为了计算动画使用,时间和速度的乘积.
     *  shader:绘制布告板用到的shader
     */
    void    render(Camera& camera,float fElapsed,ShaderBillboard& shader)
    {

        /// 绘制点数据需要的,世界坐标
        /// 纹理坐标
        /// 颜色,目前可以不用
        struct  ObjectVertex
        {
            float3      _pos;
            float2      _uv;
        };

        float3  faceDir =   camera._right;

        /// 计算布告板的四个点,根据摄像机camera._right,可以理解为水平(x轴)
        float3  lb      =   _pos - faceDir * _size.x * 0.5f;
        float3  rb      =   _pos + faceDir * _size.x * 0.5f;

        /// 计算四个点的下面两个点,上面的点就是下面的,在垂直方向增加一个高度
        float3  lt = lb + float3(0, _size.y, 0);
        float3  rt = rb + float3(0, _size.y, 0);

        /// 绘制一个草需要两个三角形,六个点
        ObjectVertex vert[6] =   
        {
            {   lb, float2(0,1)},
            {   rb, float2(1,1)},
            {   rt, float2(1,0)},

            {   lb, float2(0,1)},
            {   rt, float2(1,0)},
            {   lt, float2(0,0)},
        };
        /// 这个将点数据传递给shader
        glVertexAttribPointer(shader._position,    3, GL_FLOAT,        GL_FALSE,   sizeof(ObjectVertex),vert);
        glVertexAttribPointer(shader._uv,          2, GL_FLOAT,        GL_FALSE,   sizeof(ObjectVertex),(void*)&vert[0]._uv);
        
        /// 绘制函数,完成绘制
        glDrawArrays(GL_TRIANGLES,0,sizeof(vert) / sizeof(vert[0]) );
    }
};

class   BillboardMgr
{
public:
    typedef     std::vector<Billboard>  Billboards;
public:

    ShaderBillboard     _program;
    Billboards          _billboard;
    GLuint              _texBillboard;
public:

    /**
     *	初始化
     */
    void    init(GLuint tex)
    {
        _texBillboard   =   tex;
        /**
         *	初始化shader
         */
        _program.initialize();
       /**
        *   草地布告板数据
        */
        for (float x = 0 ; x < 100 ; x += 5)
        {
            for (float z = 0 ; z < 100 ; z += 5)
            {
                Billboard   billboard;
                float       height  =   0;
                billboard._pos      =   float3(x,height,z);
                billboard._size     =   float2(3,3);
                _billboard.push_back(billboard);
            }
        }
    }
    
    /**
     *	绘制
     */
    void    render(Camera& camera)
    {
        Billboards::iterator itr = _billboard.begin();


        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,  _texBillboard );

        CELL::matrix4   MVP     =   camera._matProj * camera._matView;

        _program.begin();
        {
            glUniform1i(_program._texture,0);
            glUniformMatrix4fv(_program._MVP, 1, false, MVP.data());
            for ( ; itr != _billboard.end() ; ++ itr)
            {
                (*itr).render(camera,0.006f,_program);
            }
        }
        _program.end();
    }
};