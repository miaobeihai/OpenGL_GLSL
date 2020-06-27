#pragma once

#include "CELLMath.hpp"
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
            { +halfSize.x,   -halfSize.y, +halfSize.z,  0.0f, +1.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, +halfSize.z,  0.0f, +1.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z,  0.0f, +1.0f,  0.0f },

            { -halfSize.x,   -halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },
            { +halfSize.x,   -halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },
            { +halfSize.x,   +halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },

            { +halfSize.x,   +halfSize.y, -halfSize.z, -1.0f,  0.0f, -0.0f },
            { +halfSize.x,   +halfSize.y, +halfSize.z, -1.0f,  0.0f, -0.0f },
            { +halfSize.x,   -halfSize.y, +halfSize.z, -1.0f,  0.0f, -0.0f },

            { -halfSize.x,   +halfSize.y, +halfSize.z,  0.0f, -1.0f,  0.0f },
            { +halfSize.x,   +halfSize.y, +halfSize.z,  0.0f, -1.0f,  0.0f },
            { +halfSize.x,   +halfSize.y, -halfSize.z,  0.0f, -1.0f,  0.0f },

            { -halfSize.x,   +halfSize.y, +halfSize.z, +1.0f,  0.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z, +1.0f,  0.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, +halfSize.z, +1.0f,  0.0f,  0.0f },

            { -halfSize.x,   +halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },
            { -halfSize.x,   -halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },
            { +halfSize.x,   +halfSize.y, +halfSize.z,  0.0f,  0.0f, -1.0f },

            { +halfSize.x,   -halfSize.y, -halfSize.z,  0.0f, +1.0f,  0.0f },
            { +halfSize.x,   -halfSize.y, +halfSize.z,  0.0f, +1.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z,  0.0f, +1.0f,  0.0f },

            { +halfSize.x,   -halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },
            { +halfSize.x,   +halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },

            { +halfSize.x,   -halfSize.y, -halfSize.z, -1.0f,  0.0f,  0.0f },
            { +halfSize.x,   +halfSize.y, -halfSize.z, -1.0f,  0.0f,  0.0f },
            { +halfSize.x,   -halfSize.y, +halfSize.z, -1.0f,  0.0f,  0.0f },

            { -halfSize.x,   +halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },
            { +halfSize.x,   +halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z,  0.0f,  0.0f, +1.0f },

            { -halfSize.x,   +halfSize.y, -halfSize.z, +1.0f,  0.0f,  0.0f },
            { -halfSize.x,   -halfSize.y, -halfSize.z, +1.0f,  0.0f,  0.0f },
            { -halfSize.x,   +halfSize.y, +halfSize.z, +1.0f,  0.0f,  0.0f },

            { -halfSize.x,   +halfSize.y, -halfSize.z,  0.0f, -1.0f,  0.0f },
            { -halfSize.x,   +halfSize.y, +halfSize.z,  0.0f, -1.0f,  0.0f },
            { +halfSize.x,   +halfSize.y, -halfSize.z,  0.0f, -1.0f,  0.0f },
        };
        memcpy(_data,verts,sizeof(verts));
    }
};
