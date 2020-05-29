#ifndef NPC_H
#define NPC_H
#include "luaLibrary.h"


#include <cstdlib>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "object.h"
#include "camera.h"
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "obj_loader.h"
#include "transform.h"
#include "animatedObject.h"
static int l_write2(lua_State* L) {
  const char* str = lua_tostring(L,1);
  std::cout << str << '\n';
  return 0;
}

static int l_atan2(lua_State* L) {
  float y = lua_tonumber(L, 1);
  float x = lua_tonumber(L, 2);
  lua_pushnumber(L, atan2(y,x));
  return 0;
}

class NPC {
public:
  NPC(const std::string& scriptFile) : aObject("./res/char", glm::vec4(1.0f,0.0f,0.0f,1.0f)) {
    //script.initFunc("atan2",l_atan2);
  }
  void Draw(Camera camera) {
  }

  void Update(double deltaTime, glm::vec2 playerPos) {
  }
  animatedObject aObject;
  glm::vec4 color;
  int animID = 1;
protected:
private:
};

#endif
