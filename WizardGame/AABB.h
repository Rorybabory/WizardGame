#ifndef AABB_H
#define AABB_H
#include <vector>
#include <glm/glm.hpp>

class AABB {
public:
  AABB(std::vector<glm::vec3> vertices) {
    glm::vec3 tempMin;
    glm::vec3 tempMax;
    for (v : vertices) {
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
  }
  void setMinMax(glm::vec3 min,glm::vec3 max) {
    this->min = min;
    this->max = max;
  }
  glm::vec3 getMin() {
    return min;
  }
  glm::vec3 getMax() {
    return max;
  }
  void setPos(glm::vec3 pos) {
    this->pos = pos;
  }
  void setRot(glm::vec3 rot) {
    this->rot = rot;
  }
  void setScale(glm::vec3 scale) {
    this->scale = scale;
  }
  glm::vec3 getPos() {
    return pos;
  }
  glm::vec3 getRot() {
    return rot;
  }
  glm::vec3 getScale() {
    return scale;
  }
private:
  glm::vec3 rot;
  glm::vec3 pos;
  glm::vec3 scale;

  glm::vec3 min;
  glm::vec3 max;
};

#endif
