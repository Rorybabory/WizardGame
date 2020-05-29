#include "object.h"
#include "obj_loader.h"
#include <fstream>
#include <string>
Object::Object(string file, glm::vec4 Color, string shaderFile, bool hasUVs) {

  IndexedModel model = OBJModel(file, hasUVs).ToIndexedModel();
  mesh.InitMesh(model);
  shader.InitShader(shaderFile);
  color = Color;
  lastDir = glm::vec3(0.0f,0.0f,0.0f);
}
Object::Object(string file, glm::vec4 Color, string shaderFile,glm::vec3 pos, bool hasUVs) {
  IndexedModel model = OBJModel(file, hasUVs).ToIndexedModel();
  mesh.InitMesh(model);
  shader.InitShader(shaderFile);
  color = Color;
  setPos(pos);
  lastDir = glm::vec3(0.0f,0.0f,0.0f);

}
void Object::moveBy(glm::vec3 vel) {
  setPos(getPos()+vel);
}
void Object::initObject(string file, glm::vec4 Color, string shaderFile, bool hasUVs) {
  IndexedModel model = OBJModel(file, hasUVs).ToIndexedModel();
  mesh.InitMesh(model);
  shader.InitShader(shaderFile);
  color = Color;
  lastDir = glm::vec3(0.0f,0.0f,0.0f);
}
glm::vec3 Object::getSize() {
  UpdateMinMax();
  if (min.x != min.x) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  if (min.y != min.y) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  if (min.z != min.z) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  glm::vec3 s;
  // s = max-min;
  s.x = max.x-min.x;
  s.y = max.y-min.y;
  s.z = max.z-min.z;
  std::cout << "min: " << min.x << " " << min.y << " " << min.z << " | max: "  << max.x << " " << max.y << " " << max.z << '\n';

  return s;
}
glm::vec3 Object::getCollPos() {
  UpdateMinMax();
  if (min.x != -min.x) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  if (min.y != -min.y) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  if (min.z != -min.z) {
    UpdateMinMax();
    std::cout << "Find object size restarted." << std::endl;
  }
  glm::vec3 p = glm::vec3((max.x+min.x)/2.0f,(max.y+min.y)/2.0f,(max.z+min.z)/2.0f);
  return p;
}
void Object::UpdateMinMax() {
  glm::vec3 tempMin = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 tempMax = glm::vec3(0.0f,0.0f,0.0f);

for(glm::vec3 v : mesh.modelSave.positions) {
  if (v.x < tempMin.x) {
    tempMin.x = v.x;
  }
  if (v.y < tempMin.y) {
    tempMin.y = v.y;
  }
  if (v.z < tempMin.z) {
    tempMin.z = v.z;
  }

  if (v.x > tempMax.x) {
    tempMax.x = v.x;
  }
  if (v.y > tempMax.y) {
    tempMax.y = v.y;
  }
  if (v.z > tempMax.z) {
    tempMax.z = v.z;
  }
}
min = tempMin*transform.getScale();
max = tempMax*transform.getScale();

}
glm::vec3 Object::getMeshPos() {
  glm::vec3 sum = glm::vec3(0.0f,0.0f,0.0f);
  for(glm::vec3 v : mesh.modelSave.positions) {
    sum.x+=v.x;
    sum.y+=v.y;
    sum.z+=v.z;
  }
  sum.x /= mesh.modelSave.positions.size();
  sum.y /= mesh.modelSave.positions.size();
  sum.z /= mesh.modelSave.positions.size();
  return sum;
}
void Object::setPos(glm::vec3 position) {
  transform.setPos(position);
}
bool Object::CheckCollision(glm::vec3 point) {

  return (point.x >= min.x && point.x <= max.x) &&
          (point.z >= min.z && point.z <= max.z);
}

Object::~Object() {

}
void Object::Update() {
  //transform.getRot().y += 0.01f;
  max = mesh.max+transform.getPos();
  min = mesh.min+transform.getPos();
}
void Object::Draw(Camera camera) {
  shader.Bind(color);
  shader.Update(transform, camera);
  mesh.Draw();
  shader.UnBind();
}
