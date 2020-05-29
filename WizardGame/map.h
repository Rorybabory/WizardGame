#ifndef MAP_H
#define MAP_H

#include "luaLibrary.h"

#include <LuaBridge/LuaBridge.h>
#include <vector>
#include "entitySystem.h"

class Map {
public:
  void init(std::string fileName, lua_State* L) {
    luah::loadScript(L,"./res/maps/" + fileName + ".lua");
    luabridge::LuaRef mapRef = luabridge::getGlobal(L,fileName.c_str());

    for (int i = 0; i < mapRef.length(); ++i) {
      luabridge::LuaRef item = mapRef[i+1];
      if (item["type"] == "prop") {
        std::string file = item["file"];
        float scale = item["scale"];
        float x = item["x"];
        float y = item["y"];
        bool collide = item["collides"];
        eSystem.addProp(file,glm::vec3(scale,scale,scale),glm::vec3(x,0.0f,y),collide);
      }else if (item["type"] == "entity") {
        std::string file = item["file"];
        float x = item["x"];
        float y = item["y"];
        eSystem.addEntityAtPos("res/scripts/entities.lua",file, glm::vec2(x,y),L);
      }else {
        std::cout << "ERROR: MAP: " << fileName << " HAS INVALID TYPED ITEM..." << "\n";
      }
    }
  }
  EntitySystem eSystem;
private:
};

#endif
