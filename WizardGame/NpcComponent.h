#ifndef NPCCOMPONENT_H
#define NPCCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"

#include <LuaBridge/LuaBridge.h>
#include "Component.h"

class NpcComponent : public Component {
public:
  NpcComponent(luabridge::LuaRef& componentTable) {
    auto phraseRef = componentTable["phrase"];
    if (phraseRef.isString()) {
      setPhrase(phraseRef.cast<std::string>());
    }else{
      std::cout << "Error, Phrase is not a string!" << '\n';
    }
  }
  std::string getPhrase() {
    return phrase;
  }
  void setPhrase(std::string phrase) {
    this->phrase = phrase;
  }
protected:
private:
  std::string phrase;
};
#endif
