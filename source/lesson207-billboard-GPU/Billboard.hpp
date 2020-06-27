#pragma once

#include "CELLMath.hpp"
#include "GLSLProgram.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "ShaderBillboard.hpp"
#include "ShaderBillboardGpu.hpp"
/**
 *	������ҪĿ����ʵ�ֹ���ƵĻ���
 */
class   Billboard
{
public:
    /**
    *   ��С,������Ļ�Ͽ����Ĳݴ�С��Ϣ,��Ⱥ͸߶�
    */
    float2      _size;
    /**
    *   λ��,�ڳ����е�λ����Ϣ
    */
    float3      _pos;
    

public:
    Billboard()
    {
    }
public:

    /**
     *	����,�����,��Ϊ����һ�������,��Ҫ���������,�õ��������ˮƽ����
     *  ������������� ������Ϊ��xyz,��һ�����up  ,right,lookat
     *  fElapsed,ʱ��,����һ֡��Ҫ��ʱ��,Ϊ�˼��㶯��ʹ��,ʱ����ٶȵĳ˻�.
     *  shader:���Ʋ�����õ���shader
     */
    void    render(Camera& camera,float fElapsed,ShaderBillboardGpu& shader)
    {
        /// ����������ݴ��ݸ�shader
        glVertexAttribPointer(shader._position,    3, GL_FLOAT,        GL_FALSE,   sizeof(float3),&_pos);
        glVertexAttribDivisor(shader._position,1);
        
        /// ���ƺ���,��ɻ���
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
     *	��ʼ��
     */
    void    init(GLuint tex)
    {
        _texBillboard   =   tex;
        /**
         *	��ʼ��shader
         */
        _program.initialize();
       /**
        *   �ݵز��������
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
     *	����
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