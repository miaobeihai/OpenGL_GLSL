#pragma once

#include "CELLMath.hpp"
#include "Camera.hpp"
#include "ShaderLightV3N3.hpp"

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
    ModelBox        _modelBox;
    std::vector<CELL::matrix4>  _locals;
    GLuint          _vbo;
    GLuint          _modelVBO;
public:
    ModelMgr()
    :_modelBox(float3(1,1,1))
    {
        for (float x = 0; x < 10; x ++)
        {
            for (float z = 0; z < 10; z ++)
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
    }

    void    caluVisible(CELL::tfrustum<float> frustm)
    {
        float   len =   CELL::length(CELL::float3(2,2,2)) * 0.5f;
        for (size_t i = 0 ;i < _locals.size(); ++ i)
        {
            float3  pos =   float3(0,0,0) * _locals[i];
            if (frustm.sphereInFrustum(pos,len))
            {
                
            }
            else
            {
            int ss = 0;
            }
        }
    }

    void    render(Camera& camera)
    {
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

            /// 绘制函数,完成绘制
            glDrawArraysInstanced(GL_TRIANGLES, 0, 36, _locals.size());
            glVertexAttribDivisor(_program._local + 0,0);
            glVertexAttribDivisor(_program._local + 1,0);
            glVertexAttribDivisor(_program._local + 2,0);
            glVertexAttribDivisor(_program._local + 3,0);
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
