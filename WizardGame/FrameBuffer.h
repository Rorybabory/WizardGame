#ifndef FRAMEBUFFERPP_H
#define FRAMEBUFFERPP_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "mesh.h"
#include "shader.h"

class FrameBuffer
{
    public:
        FrameBuffer() : quad("./res/postRender.obj",true), ppShader("./res/ppShader") {
                glGenFramebuffers(1, &framebuffer);
                glGenTextures(1, &textureColorbuffer);
                glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

                glGenRenderbuffers(1, &rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);

                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
        }
        virtual ~FrameBuffer();
        void BindFrameBuffer();
        void RenderFrameBuffer();
    protected:
    private:
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int rbo;
        Mesh quad;
        Shader ppShader;
};

#endif // FRAMEBUFFER_H

