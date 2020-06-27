#pragma once

#include "CELLMath.hpp"
#include "GLSLProgram.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "ShaderBillboard.hpp"
#include "ShaderBillboardGpu.hpp"
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
    void    render(Camera& camera,float fElapsed,ShaderBillboardGpu& shader)
    {
        /// 这个将点数据传递给shader
        glVertexAttribPointer(shader._position,    3, GL_FLOAT,        GL_FALSE,   sizeof(float3),&_pos);
        glVertexAttribDivisor(shader._position,1);
        
        /// 绘制函数,完成绘制
        glDrawArraysInstanced(GL_TRIANGLES,0,6,1);
        glVertexAttribDivisor(shader._position,0);
    }
};

class   BillboardMgr
{
public:
    typedef     std::vector<Billboard>  Billboards;
public:

    ShaderBillboardGpu  _program;
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
            glUniform3f(_program._rightDir,camera._right.x,camera._right.y,camera._right.z);
            glUniform3f(_program._upDir,0,1,0);

            glUniformMatrix4fv(_program._mvp, 1, false, MVP.data());
            for ( ; itr != _billboard.end() ; ++ itr)
            {
                (*itr).render(camera,0.006f,_program);
            }
        }
        _program.end();
    }
};