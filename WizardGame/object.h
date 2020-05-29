#ifndef OBJECT_H
#define OBJECT_H
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "obj_loader.h"
#include "transform.h"
#include "CollisionSystem.h"
using namespace std;
class Box;

class Object {
public:
  Object(string file, glm::vec4 Color, string shaderFile, bool hasUVs);
  Object(string file, glm::vec4 Color, string shaderFile,glm::vec3 pos, bool hasUVs);
  void initObject(string file, glm::vec4 Color, string shaderFile, bool hasUVs);
  Object() {}
  glm::vec3 getSize();
  void setPos(glm::vec3 position);
  glm::vec3 getCollPos();
  glm::vec3 getPos() {return transform.getPos();}
  glm::vec3 getRot() {return transform.getRot();}
  void setColor(glm::vec4 color) {this->color = color;}
  bool getDestroy() {return destroy;}
  void setScale(glm::vec3 scale) {transform.setScale(scale);}
  void setRot(glm::vec3 rot) {transform.setRot(rot);}
  void setModel(std::string fileName);
  void setDestroy(bool destroy) {this->destroy = destroy;}
  void moveBy(glm::vec3 vel);
  bool CheckCollision(glm::vec3 point);
  void UpdateMinMax();
  glm::vec3 getMeshPos();
  virtual ~Object();
  void Draw(Camera camera);
  void Update();
  Box projB;
  Transform transform;
  Texture texture;
  Mesh mesh;
  Shader shader;
  glm::vec3 min;
  glm::vec3 max;
  glm::vec4 color;
  float counter = 0;
  float counter2 = 0;
  glm::vec3 forward;
  bool collision = false;
  bool destroy = false;
  bool boxDestroy = false;
  bool hasInit = false;
  int delay = 0;
  int timesBounced = 0;
  glm::vec3 lastDir;
protected:
private:

 };
#endif
