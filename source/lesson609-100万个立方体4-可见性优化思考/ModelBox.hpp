#pragma once

#include "CELLMath.hpp"
#include "Camera.hpp"
#include "ShaderLightV3N3.hpp"
#include "ShaderCULL.hpp"
#include "CELLTimestamp.hpp"

class   ModelBox
{
public:
    struct V3N3
    {
        float x, y, z;
        float nx, ny, nz;
    };
public:
    V3N3    _data[36];
public:
    ModelBox(const CELL::float3& halfSize)
    {
        V3N3  verts[] =
        {
            {-halfSize.x,-halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },
            { halfSize.x, halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },
            { halfSize.x,-halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },

            {-halfSize.x,-halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },
            {-halfSize.x, halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },
            { halfSize.x, halfSize.y, halfSize.z,   0.0f, 0.0f, -1.0f },

            {-halfSize.x,-halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f},
            { halfSize.x, halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f},
            { halfSize.x,-halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f},

            {-halfSize.x,-halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f},
            {-halfSize.x, halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f},
            { halfSize.x, halfSize.y, -halfSize.z,   0.0f, 0.0f, +1.0f },

            {-halfSize.x, halfSize.y,-halfSize.z,   0.0f, -1.0f, 0.0f},
            { halfSize.x, halfSize.y, halfSize.z,   0.0f, -1.0f, 0.0f},
            {-halfSize.x, halfSize.y, halfSize.z,   0.0f, -1.0f, 0.0f},

            {-halfSize.x, halfSize.y,-halfSize.z,   0.0f, -1.0f, 0.0f},
            { halfSize.x, halfSize.y,-halfSize.z,   0.0f, -1.0f, 0.0f},
            { halfSize.x, halfSize.y, halfSize.z,   0.0f, -1.0f, 0.0f},

            {-halfSize.x, -halfSize.y,-halfSize.z,   0.0f, +1.0f, 0.0f},
            { halfSize.x, -halfSize.y, halfSize.z,   0.0f, +1.0f, 0.0f},
            {-halfSize.x, -halfSize.y, halfSize.z,   0.0f, +1.0f, 0.0f},

            {-halfSize.x, -halfSize.y,-halfSize.z,   0.0f, +1.0f, 0.0f},
            { halfSize.x, -halfSize.y,-halfSize.z,   0.0f, +1.0f, 0.0f},
            { halfSize.x, -halfSize.y, halfSize.z,   0.0f, +1.0f, 0.0f},

            { halfSize.x,-halfSize.y,-halfSize.z,   -1.0f, 0.0f, 0.0f},
            { halfSize.x, halfSize.y, halfSize.z,   -1.0f, 0.0f, 0.0f},
            { halfSize.x, halfSize.y,-halfSize.z,   -1.0f, 0.0f, 0.0f},

            { halfSize.x,-halfSize.y,-halfSize.z,   -1.0f, 0.0f, 0.0f},
            { halfSize.x,-halfSize.y, halfSize.z,   -1.0f, 0.0f, 0.0f},
            { halfSize.x, halfSize.y, halfSize.z,   -1.0f, 0.0f, 0.0f},


            { -halfSize.x,-halfSize.y,-halfSize.z,   +1.0f, 0.0f, 0.0f},
            { -halfSize.x, halfSize.y, halfSize.z,   +1.0f, 0.0f, 0.0f},
            { -halfSize.x, halfSize.y,-halfSize.z,   +1.0f, 0.0f, 0.0f},

            { -halfSize.x,-halfSize.y,-halfSize.z,   +1.0f, 0.0f, 0.0f},
            { -halfSize.x,-halfSize.y, halfSize.z,   +1.0f, 0.0f, 0.0f},
            { -halfSize.x, halfSize.y, halfSize.z,   +1.0f, 0.0f, 0.0f},
        };
        memcpy(_data, verts, sizeof(verts));
    }
};

class   ModelMgr
{
public:
    ShaderLightV3N3 _program;
    ShaderCULL      _shaderFeedback;
    ModelBox        _modelBox;
    std::vector<CELL::matrix4>  _locals;
    GLuint          _vbo;
    GLuint          _modelVBO;
    GLuint          _feedbackId;
    GLuint          _sphereVBOIn;
    GLuint          _sphereVBOOut;

    std::vector<float>  _feedData;
public:
    ModelMgr()
    :_modelBox(float3(0.2f,0.2f,0.2f))
    {
        for (float x = 0; x < 80; x += 1)
        {
            for (float z = 0; z < 80; z += 1)
            {
                CELL::matrix4   mat;
                mat.translate(float3(x, 0, z));
                _locals.push_back(mat);
            }
        }
    }

    void    initialize()
    {
        glGenBuffers(1, &_modelVBO);
        glBindBuffer(GL_ARRAY_BUFFER, _modelVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_modelBox._data), _modelBox._data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1,&_vbo);
        glBindBuffer(GL_ARRAY_BUFFER,_vbo);
        glBufferData(GL_ARRAY_BUFFER,_locals.size() * sizeof(CELL::matrix4),&_locals.front(),GL_STATIC_DRAW);
  
        glBindBuffer(GL_ARRAY_BUFFER,0);

        float   radius     =   CELL::length(CELL::float3(2, 2, 2)) * 0.5f;
        std::vector<CELL::float4>   arSphere;
        arSphere.resize(_locals.size());
        for (size_t i = 0 ;i < arSphere.size() ; ++ i)
        {
            float3  pos = float3(0, 0, 0) * _locals[i];
            arSphere[i].x   =   pos.x;
            arSphere[i].y   =   pos.y;
            arSphere[i].z   =   pos.z;
            arSphere[i].w   =   radius;
        }
        glGenBuffers(1, &_sphereVBOIn);
        glBindBuffer(GL_ARRAY_BUFFER, _sphereVBOIn);
        glBufferData(GL_ARRAY_BUFFER, _locals.size() * sizeof(CELL::float4), &arSphere.front(), GL_STATIC_DRAW);
  
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &_sphereVBOOut);
        glBindBuffer(GL_ARRAY_BUFFER, _sphereVBOOut);
        glBufferData(GL_ARRAY_BUFFER, _locals.size() * sizeof(float), 0, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

        glGenTransformFeedbacks(1,&_feedbackId);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK,_feedbackId);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _sphereVBOOut);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        _shaderFeedback.initialize();
        _feedData.resize(_locals.size());

        
    }

    void    beginFeedback(CELL::tfrustum<float> frust)
    {
    
        CELL::CELLTimestamp   tm;
//         float   len = CELL::length(CELL::float3(2, 2, 2)) * 0.5f;
//         int     nCnt = 0;
//         for (size_t i = 0; i < _locals.size(); ++i)
//         {
//             float3  pos = float3(0, 0, 0) * _locals[i];
//             if (frust.sphereInFrustum(pos, len))
//             {
//                 ++nCnt;
//             }
//         }

        
        
        float4  planes[6];
        frust.convertTo(planes);
        
        /// 不进行 ps 渲染

        glEnable(GL_RASTERIZER_DISCARD);
        _shaderFeedback.begin();
        {
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, _feedbackId);
            glBeginTransformFeedback(GL_POINTS);
            /// frustum
            glUniform4fv(_shaderFeedback._FRUSTUM_LEFT,     1, &planes[Frustum::FRUSTUM_LEFT][0]);
            glUniform4fv(_shaderFeedback._FRUSTUM_RIGHT,    1, &planes[Frustum::FRUSTUM_RIGHT][0]);
            glUniform4fv(_shaderFeedback._FRUSTUM_TOP,      1, &planes[Frustum::FRUSTUM_TOP][0]);
            glUniform4fv(_shaderFeedback._FRUSTUM_BOTTOM,   1, &planes[Frustum::FRUSTUM_BOTTOM][0]);
            glUniform4fv(_shaderFeedback._FRUSTUM_FAR,      1, &planes[Frustum::FRUSTUM_FAR][0]);
            glUniform4fv(_shaderFeedback._FRUSTUM_NEAR,     1, &planes[Frustum::FRUSTUM_NEAR][0]);
            
            glBindBuffer(GL_ARRAY_BUFFER,_sphereVBOIn);
            glVertexAttribPointer(_shaderFeedback._sphere,  4, GL_FLOAT, GL_FALSE, sizeof(CELL::float4),  0);
           
            glDrawArrays(GL_POINTS,0,_locals.size());

            glEndTransformFeedback();
            glFlush();
            
            glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, _feedData.size() * sizeof(float), &_feedData.front());
            glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

            char    szBuf[128];
            sprintf(szBuf, "%d - %lf\n", 0, tm.getElapsedTimeInMicroSec());
            SetWindowTextA(GetActiveWindow(), szBuf);

        }
        _shaderFeedback.end();
        
        glDisable(GL_RASTERIZER_DISCARD);


    }

    void    render(Camera& camera,CELL::tfrustum<float> frustm)
    {
        beginFeedback(frustm);
        _program.begin();
        {
            CELL::matrix4   mvp     =   camera.getProject() * camera.getView();
            char*           local   =   0;
            glUniformMatrix4fv(_program._mvp, 1, false, mvp.data());

            glUniform3f(_program._lightDir, camera._dir.x,camera._dir.y,camera._dir.z);
            glUniform3f(_program._lightColor, 1,1,1);
            
            /// 这个将点数据传递给shader
            glBindBuffer(GL_ARRAY_BUFFER,_modelVBO);
            glVertexAttribPointer(_program._position,   3, GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), 0);
            glVertexAttribPointer(_program._normal,     3, GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), (void*)12);
            
            glBindBuffer(GL_ARRAY_BUFFER,_vbo);
            glVertexAttribPointer(_program._local + 0,  4, GL_FLOAT, GL_FALSE, sizeof(CELL::matrix4),   local);
            glVertexAttribPointer(_program._local + 1,  4, GL_FLOAT, GL_FALSE, sizeof(CELL::matrix4),   local + 16);
            glVertexAttribPointer(_program._local + 2,  4, GL_FLOAT, GL_FALSE, sizeof(CELL::matrix4),   local + 32);
            glVertexAttribPointer(_program._local + 3,  4, GL_FLOAT, GL_FALSE, sizeof(CELL::matrix4),   local + 48);

            
            glVertexAttribDivisor(_program._local + 0,1);
            glVertexAttribDivisor(_program._local + 1,1);
            glVertexAttribDivisor(_program._local + 2,1);
            glVertexAttribDivisor(_program._local + 3,1);

            typedef  struct {
                uint  count;
                uint  instanceCount;
                uint  first;
                uint  baseInstance;
            } CMD;

            std::vector<CMD>    arCmd;
            arCmd.reserve(1024);
            int     nDraw   =   0;
            int     nCnt    =   0;
            for (size_t i = 0; i < _feedData.size(); ++i)
            {
                if (_feedData[i] > 0)
                {
                    ++nCnt;
                    ++nDraw;
                }
                else
                {
                    if (nCnt > 0)
                    {
                        CMD cmd = { 36,nCnt,0,i - nCnt };
                        arCmd.push_back(cmd);
                        nCnt = 0;
                    }
                }
            }
            if (nCnt > 0)
            {
                CMD cmd = { 36,nCnt,0,_locals.size() - nCnt };
                arCmd.push_back(cmd);
                //glDrawArraysInstancedBaseInstance(GL_TRIANGLES, 0, 36,nCnt,_locals.size() - nCnt);
            }
            if (!arCmd.empty())
            {
                glMultiDrawArraysIndirect(GL_TRIANGLES,&arCmd.front(),arCmd.size(),sizeof(CMD));
            }
            
            

            /// 绘制函数,完成绘制
            //glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _locals.size());

            glVertexAttribDivisor(_program._local + 0,0);
            glVertexAttribDivisor(_program._local + 1,0);
            glVertexAttribDivisor(_program._local + 2,0);
            glVertexAttribDivisor(_program._local + 3,0);
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
