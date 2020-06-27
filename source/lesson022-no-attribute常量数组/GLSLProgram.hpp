#pragma once

#include <assert.h>
#include "glew/glew.h"

class   GLSLProgram
{
public:
    typedef GLint   attribute;
    typedef GLint   uniform;
public:
    GLint           _vHandle;
    GLint           _pHandle;
    GLint           _program;
public:
    GLSLProgram()
    {
        _vHandle    =   -1;
        _pHandle    =   -1;
        _program    =   -1;
    }
    virtual ~GLSLProgram()
    {
    }

    virtual void    initialize()
    {
        const char* vs = "";
        const char* ps = "";
        createProgram(vs, ps);
    }
    
    /**
    *   ʹ��shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
    }
    /**
    *   ����
    */
    virtual void    end()
    {
        glUseProgram(0);
    }
protected:
    /**
     *	���������Դ�������shader
     */
    bool    createProgram(const char* vs, const char* ps)
    {
        bool    result = false;
        do
        {
            //! ����shader
            _vHandle = glCreateShader(GL_VERTEX_SHADER);
            _pHandle = glCreateShader(GL_FRAGMENT_SHADER);

            //! ָ��shader��Դ����
            glShaderSource(_vHandle, 1, (const GLchar**)&vs, 0);
            glShaderSource(_pHandle, 1, (const GLchar**)&ps, 0);

            //! ����shader
            glCompileShader(_vHandle);
            GLint   status = 0;
            char    compileLog[1024] = { 0 };
            glGetShaderiv(_vHandle, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetShaderInfoLog(_vHandle, sizeof(compileLog), 0, compileLog);
                assert(false && compileLog);
                break;
            }

            glCompileShader(_pHandle);

            glGetShaderiv(_pHandle, GL_COMPILE_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetShaderInfoLog(_pHandle, sizeof(compileLog), 0, compileLog);
                assert(false && compileLog);
                break;
            }
            //! 4��������
            _program = glCreateProgram();
            glAttachShader(_program, _vHandle);
            glAttachShader(_program, _pHandle);

            //! 5���ӳ���
            glLinkProgram(_program);

            glGetProgramiv(_program, GL_LINK_STATUS, &status);
            if (status == GL_FALSE)
            {
                glGetProgramInfoLog(_program, sizeof(compileLog), 0, compileLog);
                break;
            }
            result = true;

        } while (false);

        if (!result)
        {
            if (_vHandle != -1)
            {
                glDeleteShader(_vHandle);
            }
            if (_pHandle != -1)
            {
                glDeleteShader(_pHandle);
            }
            if (_program != -1)
            {
                glDeleteProgram(_program);
            }
            _vHandle = -1;
            _pHandle = -1;
            _program = -1;
        }
        return  result;
    }
};

/// gl_Vertex,gl_Normal,
class   ShaderVertex :public GLSLProgram
{
public:
    uniform _mvp;
public:
    virtual void    initialize()
    {
        const char* vs  =   "#version 130\n\
                            uniform mat4    _mvp;\n\
                            const vec3      _pos[4]=    vec3[](\n\
                                                        vec3(10,10,0),\n\
                                                        vec3(410,10,0),\n\
                                                        vec3(10,410,0),\n\
                                                        vec3(410,410,0));\n\
                            void main()\n\
                            {\n\
                                gl_Position =   _mvp * vec4(_pos[gl_VertexID],1.0);\n\
                            }";
        const char* ps  =   "void main()\n\
                            {\n\
                                gl_FragColor = vec4(1,0,0,1);\n\
                            }";
        createProgram(vs, ps);

        _mvp        =   glGetUniformLocation(_program,"_mvp");
    }

    /**
    *   ʹ��shader
    */
    virtual void    begin()
    {
        glUseProgram(_program);
    }
    /**
    *   ����
    */
    virtual void    end()
    {
        glUseProgram(0);
    }


};