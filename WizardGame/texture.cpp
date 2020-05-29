#include "texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include FT_FREETYPE_H
Texture::Texture(const std::string& fileName) {
  int width, height, numComponents;
  m_frameBuffer = 0;
  unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

  if (imageData == NULL) {
    std::cerr << "Texture loading failed for texture " << fileName << std::endl;
  }
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D,m_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,imageData);

  stbi_image_free(imageData);

  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);

}

Texture::Texture() {}
void Texture::UnBind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::InitTex(const std::string& fileName) {
    m_frameBuffer = 0;
  int width, height, numComponents;
  unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

  if (imageData == NULL) {
    std::cerr << "Texture loading failed for texture " << fileName << std::endl;
  }
  glGenTextures(texInit, &m_texture);
  glBindTexture(GL_TEXTURE_2D,m_texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,imageData);

  stbi_image_free(imageData);

}
void Texture::Bind(unsigned int unit) {
  glActiveTexture(GL_TEXTURE0+unit);
  glBindTexture(GL_TEXTURE_2D,m_texture);
}
void Texture::BindAsRenderTarget() {

}
Texture::~Texture() {
    if (m_frameBuffer) {glDeleteFramebuffers(1, &m_frameBuffer);}
    glDeleteTextures(1, &m_texture);
}
