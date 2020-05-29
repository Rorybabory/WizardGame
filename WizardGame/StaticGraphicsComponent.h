#ifndef SGCOMPONENT_H
#define SGCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"
#include "object.h"

#include <LuaBridge/LuaBridge.h>
#include "Component.h"

class StaticGraphicsComponent : public Component {
public:
  StaticGraphicsComponent(luabridge::LuaRef& componentTable) {
    auto folderRef = componentTable["folder"];
    auto color = componentTable["color"];
    auto size = componentTable["size"];
    auto colorR = color["r"];
    auto colorG = color["g"];
    auto colorB = color["b"];
    auto colorA = color["a"];
    auto CanMove = componentTable["canMove"];
    if (folderRef.isString() && colorR.isNumber() && colorG.isNumber() && colorB.isNumber() && colorA.isNumber()
         && size.isNumber()) {
      setColor(glm::vec4(colorR.cast<float>(),colorG.cast<float>(),colorB.cast<float>(),colorA.cast<float>()));
      setFolder(folderRef.cast<std::string>());
      setScale(size.cast<float>());
      canMove = CanMove.cast<bool>();
    }else{
      std::cout << "Error with static graphics component..." << '\n';
    }
  }
  void setFolder(const std::string& folder) {
    this->folder = folder;
    object.initObject(this->folder,color, "./res/basicShader",false);
  }
  std::string getFolder() const {
    return folder;
  }
  void setAnimID(int animID) {
    this->animID = animID;
  }
  int getAnimID() {
    return animID;
  }
  void Draw(Camera camera) {
    object.Draw(camera);
  }
  void Update() {
    object.setColor(color);
    object.Update();
  }

  void setColor(glm::vec4 color) {
    this->color = color;
    object.color = color;
  }
  glm::vec4 getColor() {
    return color;
  }
  void setPos(glm::vec3 pos) {
    object.setPos(pos);
  }
  glm::vec3 getPos() {
    return object.getPos();
  }
  void setRot(glm::vec3 rot) {
    object.setRot(rot);
  }
  glm::vec3 getRot() {
    return object.getRot();
  }
  void setScale(float scale) {
    object.setScale(glm::vec3(scale,scale,scale));
  }
  Object object;
  bool canMove;
  glm::vec4 color = glm::vec4(0.0f,0.7f,1.0f,1.0f);

protected:
private:
  int slowCount = 0;
  int animID = 0;
  std::string folder;
};
#endif
