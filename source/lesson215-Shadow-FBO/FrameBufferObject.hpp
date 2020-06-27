#pragma once

#include "glew/glew.h"
class   FrameBufferObject
{
public:
    unsigned    _width;
    unsigned    _height;
    unsigned    _FBOID;
    unsigned    _texDepth;
public:
    FrameBufferObject()
    {}

    bool    setupOnlyDepth(int w, int h)
    {
        _width = w;
        _height = h;
        glGenFramebuffers(1, &_FBOID);
        glBindFramebuffer(GL_FRAMEBUFFER, _FBOID);
        
        glActiveTexture(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &_texDepth);
        glBindTexture(GL_TEXTURE_2D, _texDepth);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
       
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return  true;
    }

    void    begin()
    {
        int i = glGetError();
        glBindFramebuffer(GL_FRAMEBUFFER, _FBOID);  
        glDrawBuffers(0, NULL); 
        glReadBuffer(GL_NONE);
        
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, _texDepth, 0);
        glClear(GL_DEPTH_BUFFER_BIT);
        i = glGetError();
        i = 0;

    }

    void    end()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /**
    *   Ïú»Ù
    */
    void    destroy()
    {
        glDeleteFramebuffers(1, &_FBOID);
        glDeleteTextures(1,&_texDepth);
        _texDepth  =   0;
        _FBOID  =   0;
    }

};
