#pragma once

#include "GLSLProgram.hpp"

class   ShaderTessllation :public GLSLProgram
{
public:
    uniform _mvp;
    uniform _texture;
public:
    virtual void    initialize()
    {
        const char* vs =
        {
            "#version 400 \n\
            layout (location = 0) in vec3 in_Vertex;\n\
            layout (location = 1) in vec2 in_uvCoord1;\n\
            out vec2 in_uvCoord;\n\
            void main()\n\
            {\n\
                in_uvCoord      =   in_uvCoord1;\n\
                gl_Position     =   vec4(in_Vertex, 1);\n\
            }\n"
        };
        const char* ps =
        {
            "#version 400 \n\
            uniform sampler2D  _texture; \n\
            in vec2 out_uvCoord;\n\
            void main() \n\
            {\n\
                gl_FragColor = texture2D(_texture,out_uvCoord); \n\
            }\n"
        };

        const char * tcs =
        {
            "#version 400 core  \n\
            layout (vertices = 4) out;  \n\
            in  vec2 in_uvCoord[];\n\
            out vec2 out_uvCoord[];\n\
            void main(void) \n\
            {   \n\
                gl_TessLevelInner[0] = 4.0; \n\
                gl_TessLevelInner[1] = 4.0; \n\
                gl_TessLevelOuter[0] = 3.0; \n\
                gl_TessLevelOuter[1] = 3.0; \n\
                gl_TessLevelOuter[2] = 3.0; \n\
                gl_TessLevelOuter[3] = 3.0; \n\
                gl_out[gl_InvocationID].gl_Position =   gl_in[gl_InvocationID].gl_Position; \n\
                out_uvCoord[gl_InvocationID]        =   in_uvCoord[gl_InvocationID]; \n\
            }"
        };

        const char * tes =
        {
            "#version 400 core  \n\
            layout ( quads, equal_spacing, ccw) in;\n\
            uniform mat4  mvp;\n\
            in  vec2 in_uvCoord[];\n\
            out vec2 out_uvCoord;\n\
            vec4 interpolate(in vec4 v0, in vec4 v1, in vec4 v2, in vec4 v3)\n\
            {\n\
                vec4 a = mix(v0, v1, gl_TessCoord.x);\n\
                vec4 b = mix(v3, v2, gl_TessCoord.x);\n\
                return mix(a, b, gl_TessCoord.y);\n\
            }\n\
            vec2 interpolateUV(in vec2 v0, in vec2 v1, in vec2 v2, in vec2 v3)\n\
            {\n\
                vec2 a = mix(v0, v1, gl_TessCoord.x);\n\
                vec2 b = mix(v3, v2, gl_TessCoord.x);\n\
                return mix(a, b, gl_TessCoord.y);\n\
            }\n\
            void main(void)\n\
            {   \n\
                vec4    pos =   interpolate(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, gl_in[3].gl_Position);\n\
                out_uvCoord =   interpolateUV(in_uvCoord[0],in_uvCoord[1], in_uvCoord[2], in_uvCoord[3]);\n\
                gl_Position = mvp * pos;\
            }"
        };

        createProgram(vs, ps, tcs, tes);

        _mvp    =   glGetUniformLocation(_program, "mvp");
        _texture=   glGetUniformLocation(_program, "_texture");
    }

};