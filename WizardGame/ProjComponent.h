#ifndef PCOMPONENT_H
#define PCOMPONENT_H
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include "animatedObject.h"
#include <LuaBridge/LuaBridge.h>


#include "Component.h"
#include "particleSystem.h"

class ProjectileComponent : public Component {
public:
  ProjectileComponent(luabridge::LuaRef& componentTable) {
    auto modelRef = componentTable["model"];
    auto colorRRef = componentTable["colorR"];
    auto colorGRef = componentTable["colorG"];
    auto colorBRef = componentTable["colorB"];
    auto speedRef = componentTable["speed"];
    auto rangeRef = componentTable["range"];
    auto heightRef = componentTable["height"];
    auto delayRef = componentTable["delay"];
    if (modelRef.isString() ||
     colorRRef.isNumber() ||
      colorGRef.isNumber() ||
       colorBRef.isNumber() ||
        speedRef.isNumber() ||
        rangeRef.isNumber() ||
        delayRef.isNumber() ||
        heightRef.isNumber()) {
      setColor(glm::vec4(colorRRef.cast<float>(),colorGRef.cast<float>(),colorBRef.cast<float>(),1.0f));
      setSpeed(speedRef.cast<float>());
      setModel(modelRef.cast<std::string>());
      setRange(rangeRef.cast<float>());
      height = heightRef.cast<float>();
      delay = delayRef.cast<int>();
    }else{
      std::cout << "ERROR: PROJECTILE DATA IS INVALID TYPE..." << '\n';
    }
  }
  void setModel(std::string model) {
    this->model = model;
  }
  void Draw(Camera camera) {
    if (!objects.empty()) {
      for (int i = 0; i < objects.size(); i++) {
        if (objects[i]->destroy == false) {
          objects[i]->Draw(camera);
          projectileEmitters[i]->Draw(camera);
        }
      }
    }
  }
  glm::vec3 RadiansToFront(glm::vec3 radians) {
    glm::vec3 front;
    front.x = cos(radians.y) * cos(radians.z);
    front.y = sin(radians.y);
    front.z = cos(radians.y) * sin(radians.z);

    return glm::normalize(front);
  }
  glm::vec3 FrontToRadians(glm::vec3 front) {
    glm::vec3 radians;
    radians.y = sin(front.x);
    radians.x = cos(front.x);
    radians.z = cos(front.x);
    return radians;
  }
  // glm::vec3 makeRadians() {
  //
  // }
  void Update() {
    for (int i = 0; i < objects.size(); i++) {
      projectileEmitters[i]->Update();
      // std::cout << objects[i]->counter << " ";
      // std::cout << glm::degrees(FrontToRadians(objects[i]->lastDir).y) << '\n';
      if (objects[i]->hasInit) {
        glm::vec3 dir = glm::vec3(objects[i]->projB.body->GetLinearVelocity().x,objects[i]->projB.body->GetLinearVelocity().y,objects[i]->projB.body->GetLinearVelocity().z);
        if (dir.x-objects[i]->lastDir.x != 0 &&
            dir.y-objects[i]->lastDir.y != 0 &&
            dir.z-objects[i]->lastDir.z != 0) {
              // std::cout << "CHANGE DIRECTION" << '\n';
              objects[i]->timesBounced++;
        }

        objects[i]->projB.body->SetToAwake();
        objects[i]->setPos(glm::vec3(objects[i]->projB.getPos().x,objects[i]->projB.getPos().y,objects[i]->projB.getPos().z));
        projectileEmitters[i]->setPos(objects[i]->getPos());
        if (objects[i]->destroy == false) {
          objects[i]->delay++;
          if (objects[i]->delay > range) {
            //projectileEmitters[i]->Emit(1);
            objects[i]->delay = 0;
          }
        }else {
            projectileEmitters[i]->deleteParticles();
        }

      }
      // objects[i]->moveBy(objects[i]->forward*speed);
      objects[i]->counter++;
      if (objects[i]->counter >= range) {
        // objects.erase(objects.begin()+i);
        objects[i]->destroy = true;
      }
      if (objects[i]->destroy == true) {
        objects[i]->counter2++;
        if (objects[i]->counter2>=2) {
          objects[i]->boxDestroy = true;
        }
      }
      // if (objects[i]->destroy == true) {
      //   delete objects[i];
      //   objects.erase(objects.begin()+i);
      // }
    }
    // std::cout << "" << '\n';
  }
  void collUpdate() {
    for (int i = 0; i < objects.size(); i++) {
      if (objects[i]->hasInit) {
        objects[i]->lastDir = glm::vec3(objects[i]->projB.body->GetLinearVelocity().x,objects[i]->projB.body->GetLinearVelocity().y,objects[i]->projB.body->GetLinearVelocity().z);
        objects[i]->projB.body->SetTransform(q3Vec3(objects[i]->getPos().x,objects[i]->getPos().y,objects[i]->getPos().z),q3Vec3(0.0f,0.0f,0.0f),0.0f);
        // std::cout << "PROJ POS: " << objects[i]->getPos().y << std::endl;
      }
    }
  }
  bool checkCollisionWithBox(glm::vec2 pos, glm::vec2 scale) {
    // for (o : objects) {
    //   glm::vec2 newPos = glm::vec2(pos.x-(scale.x/2.0f),pos.y-(scale.y/2.0f));
    //   if (newPos.x < o->getPos().x + 1 &&
    //       newPos.x + scale.x > o->getPos().x &&
    //       newPos.y < o->getPos().y + 1 &&
    //       newPos.y + scale.y > o->getPos().y) {
    //         o->destroy = true;
    //         return true;
    //       }
    // }
    return false;
  }
  bool Fire(glm::vec2 startPos,glm::vec3 forward) {
    delayCount++;
    if (delayCount>delay) {
      delayCount = 0;
      Object * o = new Object(model,color,"./res/basicShader",glm::vec3(startPos.x+(forward.x*7.0f),height+(forward.y*2.0f),startPos.y+(forward.z*7.0f)),false);
      o->forward = forward;
      objects.push_back(o);
      o->setScale(glm::vec3(2.0f,2.0f,2.0f));
      Emitter * e = new Emitter(0.02f, 20, glm::vec4(color.r,color.g,color.b,0.5f));
      projectileEmitters.push_back(e);
      return true;
    }
    return false;
  }
  void setSpeedMultiplier(double SpeedMultiplier) {
    this->SpeedMultiplier = SpeedMultiplier;
  }
  void setRange(float range) {
    this->range = range;
  }
  void setColor(glm::vec4 color) {
    this->color = color;
  }
  glm::vec4 getColor() {
    return color;
  }
  void setScale(float scale) {
    object.setScale(glm::vec3(scale,scale,scale));
  }
  void setSpeed(float speed) {
    this->speed = speed;
  }
Object object;
std::vector<Object*> objects;
std::vector<Emitter*> projectileEmitters;
double SpeedMultiplier;
float speed;
float height;
int delay;
int delayCount;
bool emitDelay = false;
protected:
private:
  int data = 0;
  std::string owner;
  glm::vec4 color = glm::vec4(0.0f,0.7f,1.0f,1.0f);
  std::string folder;
  float range;
  std::string model;
};
#endif
