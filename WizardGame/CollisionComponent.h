#ifndef CCOMPONENT_H
#define CCOMPONENT_H
#include <LuaBridge/LuaBridge.h>
#include <glm/glm.hpp>
#include "Component.h"
#include <vector>
class Box {
public:
  Box()
private:

  glm::vec3 pos;
  glm::vec3 scale;
};
class CollisionComponent : public Component {
public:
  CollisionComponent(luabridge::LuaRef& componentTable) {

  }
private:
  glm::vec3 scale;
  glm::vec3 pos;
};

#endif
