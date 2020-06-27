#pragma once

#include "GLSLProgram.hpp"

class   ShaderBillboardGpu :public GLSLProgram
{
public:
    attribute   _position;
    uniform     _mvp;
    uniform     _rightDir;
    uniform     _upDir;
    uniform     _texture;
    uniform     _times;
public:
    ShaderBillboardGpu()
    {
    }
    ~ShaderBillboardGpu()
    {
    }

    /// 初始化函数
    virtual void    initialize()
    {
        const char * vs =
        {
            "#version 330\n\
            uniform    vec3    _rightDir; \n\
            uniform     vec3    _upDir; \n\
            uniform     mat4    _mvp; \n\
            uniform     float   _times;\n\
            in          vec4    _pos; \n\
            out         vec2    _texcoord;\n\
            void main(void )\n\
            {\n\
            const vec2 uvData[6] = vec2[6]( \n\
                vec2(0.0, 0.0),\n\
                vec2(0.0, 1.0),\n\
                vec2(1.0, 1.0),\n\
                vec2(0.0, 0.0),\n\
                vec2(1.0, 1.0),\n\
                vec2(1.0, 0.0));\n\
                _texcoord       =   uvData[gl_VertexID];\n\
                float xxx       =   _texcoord.x;\n\
                float yyy       =   _texcoord.y;\n\
                float xOffset   =   sin(_pos.x + _times * _pos.w);\n\
                float yOffset   =   cos(_pos.y + _times * _pos.w);\n\
                vec3 newPs      =   _pos.xyz + ((xxx  - 0.5)* 5.0)* _rightDir + (yyy * 5.0) * _upDir;\n\
                     newPs      +=  vec3(xOffset,0,yOffset) * yyy;\n\
                vec4 res        =   vec4(newPs.x,newPs.y,newPs.z,1);\n\
                gl_Position     =   _mvp*res;\n\
            }\n"
        };
        char * ps =
        {
            "uniform sampler2D _texture; \n\
            in      vec2       _texcoord;\n\
            void main(void)\n\
            {\n\
                vec4   color   =   texture2D(_texture,vec2(_texcoord.x,1-_texcoord.y)); \n\
               if(color.a < 0.2) discard;\n\
               gl_FragColor   =   color;\n\
            }\n"
        };

        createProgram(vs, ps);
        _position   =   glGetAttribLocation(_program, "_pos");
        _mvp        =   glGetUniformLocation(_program, "_mvp");
        _texture    =   glGetUniformLocation(_program, "_texture");
        _rightDir   =   glGetUniformLocation(_program, "_rightDir");
        _upDir      =   glGetUniformLocation(_program, "_upDir");
        _times      =   glGetUniformLocation(_program, "_times");
    }

    /**
    *   使用程序
    */
    virtual void    begin()
    {
        glUseProgram(_program);
        glEnableVertexAttribArray(_position);
    }
    /**
    *   使用完成
    */
    virtual void    end()
    {
        glDisableVertexAttribArray(_position);
        glUseProgram(0);
    }
};
