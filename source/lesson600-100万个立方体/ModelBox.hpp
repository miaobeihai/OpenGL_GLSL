#pragma once

#include "CELLMath.hpp"
#include "Camera.hpp"
#include "ShaderLightV3N3.hpp"
class   ModelBox
{
public:
    struct V3N3
    {
        float x,y,z;
        float nx,ny,nz;
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
        memcpy(_data,verts,sizeof(verts));
    }
};

class   ModelMgr
{
public:
    ShaderLightV3N3 _program;
    ModelBox        _modelBox;
    std::vector<CELL::matrix4>  _locals;
public:
    ModelMgr()
    :_modelBox(float3(0.5,.5,0.5))
    {
        for (float x = 0; x < 1000; x += 2)
        {
            for (float z = 0; z < 100; z += 2)
            {
                CELL::matrix4   mat;
                mat.translate(float3(x, 0, z));
                _locals.push_back(mat);
            }
        }
    }

    void    render(Camera& camera)
    {
        _program.begin();
        {
            /// 这个将点数据传递给shader
            glVertexAttribPointer(_program._position,   3, GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), _modelBox._data);
            glVertexAttribPointer(_program._normal,     3, GL_FLOAT, GL_FALSE, sizeof(ModelBox::V3N3), &_modelBox._data[0].nx);
            glUniform3f(_program._lightDir, camera._dir.x, camera._dir.y, camera._dir.z);
            glUniform3f(_program._lightColor, 1, 1, 1);
            CELL::matrix4   pv = camera.getProject() * camera.getView();
            CELL::matrix4   mvp ;
        
            for (size_t i = 0 ;i < _locals.size() ; ++ i)
            {   
                mvp =  pv *  _locals[i];                
                glUniformMatrix4fv(_program._mvp, 1, false, mvp.data());
                /// 绘制函数,完成绘制
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
           
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
