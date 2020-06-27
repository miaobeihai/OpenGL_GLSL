#pragma once

#include "CELLMath.hpp"
#include "GLSLProgram.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "ShaderBillboard.hpp"
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
    void    render(Camera& camera,float fElapsed,ShaderBillboard& shader)
    {

        /// ���Ƶ�������Ҫ��,��������
        /// ��������
        /// ��ɫ,Ŀǰ���Բ���
        struct  ObjectVertex
        {
            float3      _pos;
            float2      _uv;
        };

        float3  faceDir =   camera._right;

        /// ���㲼�����ĸ���,���������camera._right,�������Ϊˮƽ(x��)
        float3  lb      =   _pos - faceDir * _size.x * 0.5f;
        float3  rb      =   _pos + faceDir * _size.x * 0.5f;

        /// �����ĸ��������������,����ĵ���������,�ڴ�ֱ��������һ���߶�
        float3  lt = lb + float3(0, _size.y, 0);
        float3  rt = rb + float3(0, _size.y, 0);

        /// ����һ������Ҫ����������,������
        ObjectVertex vert[6] =   
        {
            {   lb, float2(0,1)},
            {   rb, float2(1,1)},
            {   rt, float2(1,0)},

            {   lb, float2(0,1)},
            {   rt, float2(1,0)},
            {   lt, float2(0,0)},
        };
        /// ����������ݴ��ݸ�shader
        glVertexAttribPointer(shader._position,    3, GL_FLOAT,        GL_FALSE,   sizeof(ObjectVertex),vert);
        glVertexAttribPointer(shader._uv,          2, GL_FLOAT,        GL_FALSE,   sizeof(ObjectVertex),(void*)&vert[0]._uv);
        
        /// ���ƺ���,��ɻ���
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
            glUniformMatrix4fv(_program._MVP, 1, false, MVP.data());
            for ( ; itr != _billboard.end() ; ++ itr)
            {
                (*itr).render(camera,0.006f,_program);
            }
        }
        _program.end();
    }
};