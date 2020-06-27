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
    typedef     std::vector<float4>  Billboards;
public:

    ShaderBillboardGpu  _program;
    Billboards          _billboard;
    GLuint              _texBillboard;
    GLuint              _vbo;
    float               _times;
public:
    BillboardMgr()
    {
        _times  =   0;
    }

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
                float sp = rangeRandom(PI* 0.33f,PI * 0.5f);
                _billboard.push_back(float4(x,0,z,sp));
            }
        }
        ;
        glGenBuffers(1,&_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferData(GL_ARRAY_BUFFER,_billboard.size() * sizeof(CELL::float4),&_billboard.front(),GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
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

        _times  +=  0.02f;
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        _program.begin();
        {
            glUniform1i(_program._texture,0);
            glUniform1f(_program._times,_times);
            glUniform3f(_program._rightDir,camera._right.x,camera._right.y,camera._right.z);
            glUniform3f(_program._upDir,0,1,0);

            glUniformMatrix4fv(_program._mvp, 1, false, MVP.data());
            /// 这个将点数据传递给shader
            glVertexAttribPointer(_program._position, 4, GL_FLOAT, GL_FALSE, sizeof(float4), 0);
            glVertexAttribDivisor(_program._position, 1);

            /// 绘制函数,完成绘制
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, _billboard.size());
            glVertexAttribDivisor(_program._position, 0);
          
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }
};