#pragma once

#include "CELLMath.hpp"
#include "Camera.hpp"
#include "ShaderCubeMap.hpp"
class   ModelBoxNM
{
public:
    struct V3N3UV2
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    struct NORMALMAP
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
        float tx, ty, tz;
        float bx, by, bz;
    };
public:
    V3N3UV2 _data[36];
public:
    ModelBoxNM(const CELL::float3& halfSize)
    {
        V3N3UV2  verts[] =
        {
            { 0.5f, -0.5f,  0.5f ,  1.0, 0.0, 0.0, 0.0f, 0.0f  },
            { 0.5f, -0.5f, -0.5f ,  1.0, 0.0, 0.0, 1.0f, 0.0f  },
            { 0.5f,  0.5f, -0.5f ,  1.0, 0.0, 0.0, 1.0f, 1.0f  },

            { 0.5f,  0.5f, -0.5f ,  1.0, 0.0, 0.0, 1.0f, 1.0f  },
            { 0.5f,  0.5f,  0.5f ,  1.0, 0.0, 0.0, 0.0f, 1.0f  },
            { 0.5f, -0.5f,  0.5f ,  1.0, 0.0, 0.0, 0.0f, 0.0f  },

            {-0.5f, -0.5f, -0.5f ,  -1.0, 0.0,0.0, 0.0f, 0.0f  },
            {-0.5f, -0.5f,  0.5f ,  -1.0, 0.0,0.0, 1.0f, 0.0f  },
            {-0.5f,  0.5f,  0.5f ,  -1.0, 0.0,0.0, 1.0f, 1.0f  },

            {-0.5f,  0.5f,  0.5f ,  -1.0, 0.0,0.0, 1.0f, 1.0f  },
            {-0.5f,  0.5f, -0.5f ,  -1.0, 0.0,0.0, 0.0f, 1.0f  },
            {-0.5f, -0.5f, -0.5f ,  -1.0, 0.0,0.0, 0.0f, 0.0f  },

            {-0.5f,  0.5f,  0.5f ,  -0.0, 1.0,0.0, 1.0f, 0.0f  },
            { 0.5f,  0.5f,  0.5f ,  -0.0, 1.0,0.0, 0.0f, 0.0f  },
            { 0.5f,  0.5f, -0.5f ,  -0.0, 1.0,0.0, 0.0f, 1.0f  },

            { 0.5f,  0.5f, -0.5f ,  0.0, 1.0,0.0, 0.0f, 1.0f   },
            {-0.5f,  0.5f, -0.5f ,  0.0, 1.0,0.0, 1.0f, 1.0f   },
            {-0.5f,  0.5f,  0.5f ,  0.0, 1.0,0.0, 1.0f, 0.0f   },

            {-0.5f, -0.5f, -0.5f ,  0.0, -1.0,0.0, 1.0f, 0.0f  },
            { 0.5f, -0.5f, -0.5f ,  0.0, -1.0,0.0, 0.0f, 0.0f  },
            { 0.5f, -0.5f,  0.5f ,  0.0, -1.0,0.0, 0.0f, 1.0f  },

            { 0.5f, -0.5f,  0.5f ,  0.0, -1.0,0.0, 0.0f, 1.0f  },
            {-0.5f, -0.5f,  0.5f ,  0.0, -1.0,0.0, 1.0f, 1.0f  },
            {-0.5f, -0.5f, -0.5f ,  0.0, -1.0,0.0, 1.0f, 0.0f  },

            {-0.5f, -0.5f,  0.5f,   0.0, 0.0, 1.0,  0.0f, 0.0f },
            { 0.5f, -0.5f,  0.5f,   0.0, 0.0, 1.0,  1.0f, 0.0f },
            { 0.5f,  0.5f,  0.5f,   0.0, 0.0, 1.0,  1.0f, 1.0f },

            { 0.5f,  0.5f,  0.5f,   0.0, 0.0, 1.0,  1.0f, 1.0f },
            {-0.5f,  0.5f,  0.5f,   0.0, 0.0, 1.0,  0.0f, 1.0f },
            {-0.5f, -0.5f,  0.5f,   0.0, 0.0, 1.0,  0.0f, 0.0f },

            { 0.5f, -0.5f, -0.5f,   0.0, 0.0,-1.0,  0.0f, 0.0f },
            {-0.5f, -0.5f, -0.5f,   0.0, 0.0,-1.0,  1.0f, 0.0f },
            {-0.5f,  0.5f, -0.5f,   0.0, 0.0,-1.0,  1.0f, 1.0f },

            {-0.5f,  0.5f, -0.5f,   0.0, 0.0,-1.0,  1.0f, 1.0  },
            { 0.5f,  0.5f, -0.5f,   0.0, 0.0,-1.0,  0.0f, 1.0  },
            { 0.5f, -0.5f, -0.5f,   0.0, 0.0,-1.0,  0.0f, 0.0  },
        };
        for (int i = 0; i < 36; ++i)
        {
            verts[i].x *= halfSize.x;
            verts[i].y *= halfSize.y;
            verts[i].z *= halfSize.z;
        }
        memcpy(_data,verts,sizeof(verts));
    }
};

class   ModelMgrNM
{
public:
    ShaderCubeMap   _program;
    ModelBoxNM      _modelBox;
public:
    ModelMgrNM()
        :_modelBox(float3(30,30,30))
    {
    }

    void    render(Camera& camera)
    {
        static  float   angle   =   0;
        angle   +=  0.1f;
        _program.begin();
        {
            CELL::matrix4   matTrans;
            matTrans.translate(0,40,0);
            CELL::matrix4   matRot;
            matRot.rotateYXZ(angle,angle,angle);
            CELL::matrix4   matModel    =   matTrans;// * matRot;
            CELL::matrix4   matProj     =   camera.getProject();
            CELL::matrix4   matView     =   camera.getView();
            CELL::matrix4   mvp         =   matProj * matView * matModel;
            glUniformMatrix4fv(_program._mvp,       1,  false, mvp.data());
            glUniformMatrix4fv(_program._model,     1,  false, matModel.data());

            glUniform1i(_program._texture,0);
            
            glUniform3f(_program._cameraPos, camera._eye.x,camera._eye.y,camera._eye.z);
            
            /// 这个将点数据传递给shader
            glVertexAttribPointer(_program._position,   3, GL_FLOAT, GL_FALSE, sizeof(ModelBoxNM::V3N3UV2), _modelBox._data);
            glVertexAttribPointer(_program._normal,     3, GL_FLOAT, GL_FALSE, sizeof(ModelBoxNM::V3N3UV2), &_modelBox._data[0].nx);
                 
            glDrawArrays(GL_TRIANGLES,0,36);
        }
        _program.end();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};
