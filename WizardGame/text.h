#ifndef TEXT_H
#define TEXT_H
#include "object.h"
#include "mesh.h"
#include "shader.h"
#include "transform.h"
#include <string>
#include <glfreetype/TextRenderer.hpp>
class Text {
public:
  Text(int size, std::string file) {
    this->file = file;
    this->textSize = size;
    Font.init(file.c_str(), size);
  }
  int CalculateLocX(float val, float size) {
    float size2 = size-(size/10);
    return (int) ((((val/1.0f)*size)/2)+(size/2));
  }
  int CalculateLocY(float val, float size) {
    float size2 = size-(size/10);
    return (int) ((((val/1.0f)*size)/2)+(size/2))-(val*textSize);
  }
  void Draw(std::string text, glm::vec2 loc, glm::vec4 color, int width, int height) {
    glPushMatrix();
    glLoadIdentity();
    glColor4f(color.x,color.y,color.z,color.w);
    glfreetype::print(Font,CalculateLocX(loc.x,(float) width),CalculateLocY(loc.y,(float) height),text);
    // std::cout << "x=" << CalculateLocX(x,(float) width) << " y=" << CalculateLocY(y,(float) height) << std::endl;
    glPopMatrix();
  }
private:
  int textSize;
  int width;
  int height;
  std::string file;
  glfreetype::font_data Font;
};
#endif
