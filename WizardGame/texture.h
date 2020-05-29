#ifndef TEXTURE_H
#define TEXTURE_H
#include <glm/glm.hpp>
#include <string>
#include <GL/glew.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <map>
#include "mesh.h"
#include "shader.h"
#include FT_FREETYPE_H
struct Character {
  GLuint      TextureID;
  glm::ivec2  Bearing;
  glm::ivec2  Size;
  GLuint      Advance;
};
class Texture{
public:
  Texture(const std::string& fileName);
  // Texture(const std::string& fontName, int fontSize);
  Texture();
  void Bind(unsigned int unit);
  void BindAsRenderTarget();
  void UnBind();
  // void RenderText(Camera cam,Shader &s,std::string text, GLfloat x, GLfloat y, GLfloat scale);
  void InitTex(const std::string& fileName);
  virtual ~Texture();
protected:
private:
  FT_Library ft;
  FT_Face face;
  GLuint m_frameBuffer;
  std::map<GLchar,Character> Characters;
  int texInit = 1;
  GLuint m_texture;
};

#endif
