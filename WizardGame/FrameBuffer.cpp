#include "FrameBuffer.h"


void FrameBuffer::BindFrameBuffer() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
}
void FrameBuffer::RenderFrameBuffer() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ppShader.Bind();
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    quad.Draw();
}
FrameBuffer::~FrameBuffer()
{
}

