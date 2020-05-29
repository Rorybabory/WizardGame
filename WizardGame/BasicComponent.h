#ifndef BASICCOMPONENT_H
#define BASICCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"

#include <LuaBridge/LuaBridge.h>
#include "Component.h"
#include "camera.h"
class BasicComponent : public Component {
public:
  BasicComponent(luabridge::LuaRef& componentTable) {
    auto onStartRef = componentTable["onStart"];
    if (onStartRef.isFunction()) {

    }else{
      std::cout << "Error, onStart is not a Function!" << '\n';
    }
  }
protected:
private:

};
#endif
