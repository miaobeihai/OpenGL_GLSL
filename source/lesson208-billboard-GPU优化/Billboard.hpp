#pragma once

#include "CELLMath.hpp"
#include "GLSLProgram.hpp"
#include "Camera.hpp"
#include "stb_image.h"
#include "ShaderBillboard.hpp"
#include "ShaderBillboardGpu.hpp"


class   BillboardMgr
{
public:
    typedef     std::vector<float3>  Billboards;
public:

    ShaderBillboardGpu  _program;
    Billboards          _billboard;
    GLuint              _texBillboard;
    GLuint              _vbo;
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
                _billboard.push_back(float3(x,0,z));
            }
        }
        ;
        glGenBuffers(1,&_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferData(GL_ARRAY_BUFFER,_billboard.size() * sizeof(CELL::float3),&_billboard.front(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
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

        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        _program.begin();
        {
            glUniform1i(_program._texture,0);
            glUniform3f(_program._rightDir,camera._right.x,camera._right.y,camera._right.z);
            glUniform3f(_program._upDir,0,1,0);

            glUniformMatrix4fv(_program._mvp, 1, false, MVP.data());
            /// ����������ݴ��ݸ�shader
            glVertexAttribPointer(_program._position, 3, GL_FLOAT, GL_FALSE, sizeof(float3), 0);
            glVertexAttribDivisor(_program._position, 1);

            /// ���ƺ���,��ɻ���
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _billboard.size());
            glVertexAttribDivisor(_program._position, 0);
          
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
};