#ifndef PROJ_H
#define PROJ_H
#include "messaging.h"
#include "luaLibrary.h"

#include <LuaBridge/LuaBridge.h>
#include <string>
#include <glm/glm.hpp>
#include <vector>
#include <math.h>

class Projectile {
public:
  Projectile(float speed,glm::vec3 color,std::string model,float range, std::string owner) :
  object(model,glm::vec4(color.r,color.g,color.b,1.0),"./res/aShader",false) {
    this->speed = speed;
    this->color = color;
    this->model = model;
    this->range = range;
    this->owner = owner;
  }
  void Fire(glm::vec2 startPos, glm::vec3 direction) {
    this->direction = direction;
    object.setRot(glm::vec3(object.getRot().x,direction.z,object.getRot().z));
    object.setPos(glm::vec3(startPos.x,3.5f,startPos.y));
  }

  void Move() {
    object.setPos(object.getPos() += direction*speed);
  }
  void Update() {
    Move();
    counter++;
  }
  void Draw(Camera camera) {
    // std::cout << camera.m_position.x << " | " << camera.m_position.y << " | " << camera.m_position.z << '\n';
    object.Draw(camera);
  }
  float range;
  float counter = 0.0f;
  Object object;
  std::string owner;

private:
std::string type;
std::string model;
glm::vec3 color;
float speed;
glm::vec3 direction;
float rotation;
bool inUse;


};
class ProjectileManager {
public:
  ProjectileManager() {
    luaL_openlibs(L);
    luah::loadScript(L,"res/scripts/projectiles.lua");
  }
  void Update() {
    // std::cout << messager.getMessage().text << '\n';
    positions.clear();
    targets.clear();
    for (Projectile * p : projectiles) {
      targets.push_back(p->owner);
      positions.push_back(glm::vec2(p->object.getPos().x,p->object.getPos().z));
    }
    if (messager.getMessage().Vector2D != glm::vec2(0.0f,0.0f)) {
      addProjectile(messager.getMessage().text);
      positions.push_back(messager.getMessage().Vector2D);
      projectiles.back()->Fire(messager.getMessage().Vector2D,messager.getMessage().Vector3D);
      messager.clearBus();
    }
    messager.sendMessage(Message("EntitySystem",positions,targets));
    for (int i = 0; i<projectiles.size(); i++) {
      projectiles[i]->Update();
      positions[i] = glm::vec2(projectiles[i]->object.getPos().x,projectiles[i]->object.getPos().z);
      if (projectiles[i]->counter > projectiles[i]->range) {
        projectiles.erase(projectiles.begin()+i);
        positions.erase(positions.begin()+i);
      }
    }
  }
  void Draw(Camera camera) {
    for (Projectile * p : projectiles) {
      p->Draw(camera);
    }
  }
  void addProjectile(std::string type) {
    luabridge::LuaRef entityRef = luabridge::getGlobal(L,type.c_str());
    float speed = entityRef["speed"];
    glm::vec3 color = glm::vec3(entityRef["r"],entityRef["g"],entityRef["b"]);
    std::string model = entityRef["model"];
    std::string owner = entityRef["owner"];
    float range = entityRef["range"];
    Projectile * p = new Projectile(speed,color,model,range,owner);
    projectiles.push_back(p);
  }
  Messager messager;

private:
  int counter = 0;
  lua_State* L = luaL_newstate();
  std::vector<Projectile*> projectiles;
  std::vector<glm::vec2> positions;
  std::vector<string> targets;
};
#endif
