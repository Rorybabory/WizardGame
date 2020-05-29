#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(suppress : 4996)
#include <iostream>
#include <map>
#include <typeindex>
#include <string>
#include <glm/glm.hpp>
#include <math.h>
#include <cmath>

#include <stdlib.h>
#include "animatedObject.h"
#include "luaLibrary.h"

#include <LuaBridge/LuaBridge.h>
#include <memory>
#include "GraphicsComponent.h"
#include "StaticGraphicsComponent.h"
#include "NpcComponent.h"
#include "CameraComponent.h"
#include "Component.h"
#include <qu3e/q3.h>
#include "CollisionSystem.h"
#include "ProjComponent.h"
#include <map>
#ifndef ENTITY_H
#define ENTITY_H
static float getDistance(float x, float y, float x2,float y2) {
  float result = sqrt(pow(x-x2, 2) +
                  pow(y-y2, 2));
  return result;
}
class SpawnData {
public:
  SpawnData(std::string name, glm::vec2 pos) {
    this->pos = pos;
    this->name = name;
  }
  glm::vec2 pos;
  std::string name;
private:
};
class Entity {
public:
  float sign (glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
  }
  bool PointInTriangle (glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) { //Thanks stack overflow!
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = sign(pt, v1, v2);
    d2 = sign(pt, v2, v3);
    d3 = sign(pt, v3, v1);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
  }


  float RADS_TO_DEGS(float radians) {
    return radians * 180/3.1415926535;
  }
  float DEGS_TO_RADS(float degrees) {
    return degrees * 3.1415926535/180;
  }
  int FindTurnSide(int current, int target)
  {
    int diff = target - current;
    if(diff < 0) {
      diff += 360;
    }
    if(diff > 180) {
          return -1;
    } else {
          return 1;
    }
  }
  // virtual Entity* clone() = 0;
  ~Entity() {
    for (auto& c : components) {
      delete c.second;
    }
  }
  Entity(lua_State* L) : UpdateFunction(L), OnStartFunction(L), CollisionObject("./res/cube.obj",glm::vec4(0.0f,1.0f,0.0f,1.0f),"./res/basicShader",true),
                                                                CollisionObject2("./res/cube.obj",glm::vec4(1.0f,0.0f,0.0f,1.0f),"./res/basicShader",true) {
  }
  void CompileLuaFunctions(lua_State* L) {
    std::string UpdateString = type + "_Update";
    UpdateFunction = luabridge::getGlobal(L,UpdateString.c_str());
    std::string OnStartString = type + "_Start";
    OnStartFunction = luabridge::getGlobal(L,OnStartString.c_str());
  }
  void Hit(lua_State* L, int hits, Entity * ent) {
    if (!dead) {
      // std::cout << "YOU HIT A " << type << " AT " << pos.x << " " << pos.z << " with " << hits << " hits" << '\n';
      std::string HitString = type + "_Hit";
      try {
        auto hit = luabridge::getGlobal(L,HitString.c_str());
        hit(this,ent,hits);
      }catch(luabridge::LuaException const& e) {
        std::cout << "MISSING HIT FUNCTION..." << '\n';
      }
    }
  }
  void Draw(Camera cam, bool f) {
    auto gfxc = get<GraphicsComponent>();
    if (gfxc != NULL && !dead) {
      if (f == false && frozen == false && isPaused == false) {
        gfxc->Update(speedModifier);
      }
      if (isPaused != true) {
          gfxc->Draw(cam);

      }
      CollisionObject.Draw(cam);
      CollisionObject2.Draw(cam);
    }
    auto sgfxc = get<StaticGraphicsComponent>();
    if (sgfxc != NULL && !dead) {
      sgfxc->Update();
      sgfxc->Draw(cam);
    }
  }
  bool getCollisionWithPoint(glm::vec3 point) {
    return  (point.x >= projMin.x-3.0f && point.x <= projMax.x+3.0f) &&
            (point.y >= projMin.y && point.y <= projMax.y) &&
            (point.z >= projMin.z-3.0f && point.z <= projMax.z+3.0f);
  }
  void Update(lua_State* L, q3Scene * scene) {
    auto graphics = get<GraphicsComponent>();
    auto cameraC = get<CameraComponent>();
    auto sgraphics = get<StaticGraphicsComponent>();
    textPointer = new std::string("DANK");
    if (cameraC != NULL) {
      cameraC->camera.m_position = pos;
      cameraC->UpdateCamera();
    }
    if (hp < 1) {
      kill();
    }
    if (!readyToFire) {
      fireCount++;
      if (fireCount == 30) {
        readyToFire = true;
        fireCount=0;
      }
    }
    if (sgraphics != NULL) {
        if (pos.y < 0.0f) {pos.y = 0.0f;}
        if (sgraphics->color.x > 0.0) {sgraphics->color.x-=0.03;}
        if (sgraphics->color.y > 0.0) {sgraphics->color.y-=0.03;}
        if (sgraphics->color.z > 0.0) {sgraphics->color.z-=0.03;}
        if (sgraphics->color.x < 0.0) {sgraphics->color.x = 0.0;}
        if (sgraphics->color.y < 0.0) {sgraphics->color.y = 0.0;}
        if (sgraphics->color.z < 0.0) {sgraphics->color.z = 0.0;}
    }
    if (graphics != NULL) {
      if (graphics->color.x > 0.0) {graphics->color.x-=0.03;}
      if (graphics->color.y > 0.0) {graphics->color.y-=0.03;}
      if (graphics->color.z > 0.0) {graphics->color.z-=0.03;}
      if (graphics->color.x < 0.0) {graphics->color.x = 0.0;}
      if (graphics->color.y < 0.0) {graphics->color.y = 0.0;}
      if (graphics->color.z < 0.0) {graphics->color.z = 0.0;}
      projMin = glm::vec3(graphics->object.transform.GetModelColl() * glm::vec4(Min, 1.0));
      projMax = glm::vec3(graphics->object.transform.GetModelColl() * glm::vec4(Max, 1.0));
      if (projMin.x > 0.0f) {projMin.x = -projMin.x;}
      if (projMin.y > 0.0f) {projMin.y = -projMin.y;}
      if (projMin.z > 0.0f) {projMin.z = -projMin.z;}

      if (projMax.x < 0.0f) {projMax.x = -projMax.x;}
      if (projMax.y < 0.0f) {projMax.y = -projMax.y;}
      if (projMax.z < 0.0f) {projMax.z = -projMax.z;}
      projMin+=pos;
      projMax+=pos;
      glm::vec3 tempMin = projMin;
      // if (projMin.x > projMax.x) {projMin.x = projMax.x; projMax.x = tempMin.x;}
      // if (projMin.y > projMax.y) {projMin.y = projMax.y; projMax.y = tempMin.y;}
      // if (projMin.z > projMax.z) {projMin.z = projMax.z; projMax.z = tempMin.z;}

      projMin.y += projMax.y;
      projMax.y += projMax.y;

      if (type != "player") {
        graphics->setPos(pos);
        graphics->setRot(rot);
        graphics->setScale(scale);
        // std::cout << "scale X: " << scale.x << "scale Y: " << scale.y << "scale Z: " << scale.z << std::endl;
      }else {
        graphics->setPos(glm::vec3(pos.x,pos.y-15,pos.z));
      }
      min = graphics->object.min;
      max = graphics->object.max;
    }else {
      projMin = (Min+pos);
      projMax = (Max+pos);

      projMin.y += scaleColl.y;
      projMax.y += scaleColl.y;
    }
    // if (type == "player") {
    //   std::cout << "max.y: " << Max.y << " min.y: " << Min.y << " coll Y: " << scaleColl.y << "\n";
    // }
    if (sgraphics != NULL) {
      pos = glm::vec3(startingPos.x, 0.0f, startingPos.y);
      sgraphics->object.setPos(glm::vec3(startingPos.x,0.0f,startingPos.y));
      sgraphics->object.setRot(glm::vec3(sgraphics->object.getRot().x,rot.y,sgraphics->object.getRot().z));
    }
    std:string UpdateString = type + "_Update";
    try {
      if (!dead && !frozen) {
        // UpdateFunction = luabridge::getGlobal(L,UpdateString.c_str());
        UpdateFunction(this);
      }else {
        pos = glm::vec3(999.0f,999.0f,999.0f);
      }

    }catch(luabridge::LuaException const& e) {
      std::cout << "UPDATE FAILED" << '\n';
    }
    vel = pos-lastPos;
    if (type != "player") {
      CollisionObject.setPos(glm::vec3(projMin.x,projMin.y,projMin.z));
      CollisionObject2.setPos(projMax);
      CollisionObject.setScale(glm::vec3(0.3,0.3,0.3));
      CollisionObject2.setScale(glm::vec3(0.3,0.3,0.3));
      CollisionObject.Update();
      CollisionObject2.Update();
    }
    // if (type == "player") {
    //   auto playerCamC = get<CameraComponent>();
    //   glm::vec3 playerPos = playerCamC->camera.m_position;
    //   // std::cout << "X: " << playerCamC->camera.m_position.x << " Y: " << playerCamC->camera.m_position.z << std::endl;
    //   if (playerPos.x != playerPos.x ||
    //       playerPos.y != playerPos.y ||
    //       playerPos.z != playerPos.z) {
    //     // DumpToFile();
    //     std::cout << "PLAYER BOX RECREATED" << std::endl;
    //   }
    // }
  }

  void DumpToFile() {
    #pragma warning(push)
    #pragma warning(disable: 4996) //4996 for _CRT_SECURE_NO_WARNINGS equivalent

    FILE * pFile;
    char buffer [100];
    pFile = fopen("res/scripts/dumps/playerDump.txt","w+");
    if (pFile == NULL) {
      std::cout << "error opening file..." << '\n';
    }
    if (type == "player") {
      collider.body->Dump(pFile,1);
    }
    #pragma warning(pop)
  }
  void OnStart(lua_State* L) {
    try {
      // auto OnStart = luabridge::getGlobal(L,OnStartString.c_str());

      OnStartFunction(this);
      if (type == "player") {
        std::cout << "Player Started" << std::endl;
      }
    }catch(luabridge::LuaException const& e) {
      std::cout << "ON START FAILED" << '\n';
    }
  }
  void setUpCollider(q3Scene * scene, glm::vec3 scale) {
    if (type != "player") {
      collider.init(scale,glm::vec3(0.0f,0.0f,0.0f),scene,eDynamicBody);
    }else {
      collider.init(scale,glm::vec3(0.0f,0.0f,0.0f),scene,eDynamicBody);
    }
  }
  //ADDS FUNTIONS FOR API
  void addFunctions(lua_State* L) {
    using namespace luabridge;
    getGlobalNamespace(L)
      .beginNamespace("util")
      .addFunction("getDistance",&getDistance)
      .endNamespace()
      .beginClass<Entity>("Entity")
      // .addFunction("Damage",&Entity::Damage)
      .addFunction("setAnimation",&Entity::setAnimation)
      .addFunction("playAnimation",&Entity::playAnimation)
      .addFunction("getAnimation",&Entity::getAnimation)
      .addFunction("setPos",&Entity::setPos)
      .addFunction("getX",&Entity::getX)
      .addFunction("getY",&Entity::getY)
      .addFunction("getZ",&Entity::getZ)
      .addFunction("setColor",&Entity::setColor)
      .addFunction("lookAt",&Entity::lookAt)
      .addFunction("lookAtPlayer",&Entity::lookAtPlayer)
      .addFunction("moveForward",&Entity::moveForward)
      .addFunction("moveBackward",&Entity::moveBackward)
      .addFunction("Strafe",&Entity::Strafe)
      .addFunction("write",&Entity::write)
      .addFunction("random",&Entity::random)
      .addFunction("setScale",&Entity::setScale)
      .addFunction("getPlayerDistance",&Entity::getPlayerDistance)
      .addFunction("getKeyDirectionX",&Entity::getKeyDirectionX)
      .addFunction("getKeyDirectionY",&Entity::getKeyDirectionY)
      .addFunction("FPSControllerUpdate",&Entity::FPSControllerUpdate)
      .addFunction("kill",&Entity::kill)
      .addFunction("getHP",&Entity::getHP)
      .addFunction("setHP",&Entity::setHP)
      .addFunction("addCounter",&Entity::addCounter)
      .addFunction("updateCounter",&Entity::updateCounter)
      .addFunction("getCount",&Entity::getCount)
      .addFunction("setCount",&Entity::setCount)
      .addFunction("killProgram",&Entity::killProgram)
      .addFunction("spawnEntity",&Entity::spawnEntity)
      .addFunction("Fire", &Entity::Fire)
      .addFunction("getFloat", &Entity::getFloat)
      .addFunction("setFloat", &Entity::setFloat)
      .addFunction("getBool", &Entity::getBool)
      .addFunction("setBool", &Entity::setBool)
      .addFunction("getString", &Entity::getString)
      .addFunction("setString", &Entity::setString)
      .addFunction("getAnimFrame", &Entity::getAnimFrame)
      .addFunction("getKeyPressed", &Entity::getKeyPressed)
      .addFunction("setCanBeHit", &Entity::setCanBeHit)
      .addFunction("setFOV", &Entity::setFOV)
      .addFunction("TopDownUpdate", &Entity::TopDownUpdate)
      .addFunction("isFirstPerson", &Entity::isFirstPerson)
      .addFunction("lookAtNearest", &Entity::lookAtNearest)
      .addFunction("getDistanceFromNearest", &Entity::getDistanceFromNearest)
      .addFunction("getPositionFromNearestX", &Entity::getPositionFromNearestX)
      .addFunction("getPositionFromNearestY", &Entity::getPositionFromNearestY)
      .addFunction("doesEntityExist", &Entity::doesEntityExist)
      .addFunction("setGlobalFrozen", &Entity::setGlobalFrozen)
      .addFunction("getGlobalFrozen", &Entity::getGlobalFrozen)
      .addFunction("setFrozen", &Entity::setFrozen)
      .addFunction("getFrozen", &Entity::getFrozen)
      .addFunction("getNearestEntWithName", &Entity::getNearestEntWithName)
      .addFunction("getDistanceBetweenTwoPointsAPI", &Entity::getDistanceBetweenTwoPointsAPI)
      .addFunction("Shake", &Entity::Shake)
      .addFunction("isPlayer", &Entity::isPlayer)
      .addFunction("setInverted", &Entity::setInverted)
      .addFunction("pushInsideLevel", &Entity::pushInsideLevel)
      .addFunction("restartCollisionBox", &Entity::restartCollisionBox)
      .addFunction("playDamageAnimation", &Entity::playDamageAnimation)
      .addFunction("setDamageAnimation", &Entity::setDamageAnimation)
      .addFunction("UpdateKeyPresses", &Entity::UpdateKeyPresses)
      .addFunction("setAnimationTag", &Entity::setAnimationTag)
      .addFunction("playAnimationTag", &Entity::playAnimationTag)
      .addFunction("isAnimationPlaying", &Entity::isAnimationPlaying)
      .addFunction("getLastX", &Entity::getLastX)
      .addFunction("getLastY", &Entity::getLastY)
      .addFunction("getLastZ", &Entity::getLastZ)
      .addFunction("setCollisionBox", &Entity::setCollisionBox)
      .addFunction("Delay", &Entity::Delay)
      .addFunction("setUIText", &Entity::setUIText)
      .addFunction("getPaused", &Entity::getPaused)
      .addFunction("getDefaultSpeed", &Entity::getDefaultSpeed)

      .endClass();
  }
  void setCanBeHit(bool val) {
    this->canBeHit = val;
    if (val == false) {
      std::cout << "SHIELD: " << countSHIELD << std::endl;
      countSHIELD++;
    }
  }
  void setFloat(std::string name, float value) {
    FloatsVars[name] = value;
  }
  float getFloat(std::string name) {
    return FloatsVars[name];
  }

  void setString(std::string name, std::string value) {
    StringVars[name] = value;
  }
  std::string getString(std::string name) {
    return StringVars[name];
  }

  void setBool(std::string name, bool value) {
    BoolVars[name] = value;
  }
  bool getBool(std::string name) {
    return BoolVars[name];
  }

  void UpdateProj(std::vector<glm::vec2> proj) {
    this->proj = proj;
  }


  void addComponent(std::type_index type, Component* c) {
    components[type] = c;
  }
  void setCamPos(glm::vec3 p) {
    auto cameraC = get<CameraComponent>();
    cameraC->setPos(p);
  }
  glm::vec3 getCamPos() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_position;
  }
  glm::vec3 getCamForward() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_forward;
  }
  glm::vec3 getForward() {
    static const glm::vec3 UP(0.0f, 1.0f, 0.0f);

    glm::mat4 rotation = glm::rotate(rot.y, UP);

    glm::vec3 forward = glm::vec3(glm::normalize(rotation * glm::vec4(glm::vec3(0,0,1), 0.0)));
    return forward;
  }
  void DrawProj(Camera camera) {
    auto projC = get<ProjectileComponent>();
    projC->Draw(camera);
    std::cout << "Drew Projectiles" << std::endl;
  }
  //API FUNCTIONS
  // void Damage() {
  //   Hit()
  // }
  float getDefaultSpeed() { return mainSpeed; }

  void setUIText(std::string text);
  void Delay(int milliseconds);
  void setCollisionBox(float width, float height, float length);
  float getDistanceBetweenTwoPointsAPI(float x, float y, float x2, float y2);
  float getLastX();
  float getLastY();
  float getLastZ();
  void UpdateKeyPresses();
  void setDamageAnimation(int anim);
  void playDamageAnimation();
  void restartCollisionBox();
  void Shake(float amount);
  void setInverted(int i);
  float getDistanceBetweenTwoPoints(glm::vec2 pos1, glm::vec2 pos2);
  void setGlobalFrozen(bool f);
  Entity* getNearestEntWithName(std::string entityName);
  bool getGlobalFrozen();
  void setFrozen(bool f);
  bool getFrozen();
  float lookAtNearest(std::string targetType);
  bool doesEntityExist(std::string targetType);
  float getDistanceFromNearest(std::string targetType);
  float getPositionFromNearestX(std::string targetType);
  float getPositionFromNearestY(std::string targetType);
  void isFirstPerson(bool firstPerson);
  void setFOV(float FOV);
  int getAnimFrame();
  void playAnimation(int id);
  void spawnEntity(std::string name, float x, float y);
  void killProgram();
  void kill();
  float getPlayerDistance();
  void write(const std::string& str);
  void setScale(float scale);
  float random(float min, float max);
  void lookAtPlayer();
  void lookAt(float x, float y);
  void Strafe(float amount);
  void moveForward(float amount);
  void moveBackward(float amount);
  void setAnimation(int id);
  int getAnimation();
  void setPos(float x, float y, float z);
  float getX();
  float getY();
  float getZ();
  void setColor(float r, float g, float b, float a);
  float getKeyDirectionX();
  float getKeyDirectionY();
  std::string getKeyPressed();
  void TopDownUpdate();
  void FPSControllerUpdate(float speed);
  int getHP();
  void setHP(int hp);
  void addCounter(int start);
  bool updateCounter(int counter,int end);
  int getCount();
  void setCount(int count);
  bool Fire();
  bool isPlayer();
  void pushInsideLevel();
  void setAnimationTag(std::string tag, int animation);
  void playAnimationTag(std::string tag);
  bool isAnimationPlaying(std::string tag);
  void Damage(int numHits);
  bool getPaused() { return isPaused; }
  //END API FUNCTIONS

  template <typename T>
  T* get() {
    auto it = components.find(std::type_index(typeid(T)));
    if (it != components.end()) {
      return dynamic_cast<T*>(it->second);
    }
    return nullptr;
  }
  void setType(const std::string& type) {
    this->type = type;
  }
  std::string getType() const {
    return type;
  }

  glm::vec3 getPlayerRot() {
    auto cameraC = get<CameraComponent>();
    return cameraC->getCamera().m_forward;
  }
  void setPlayerPos(glm::vec3 playerPos) {
    this->playerPos = playerPos;
  }
  int collisionUpdateCounter = 0;
  float nearestEntityDist;
  std::string nearestEntityType;
  glm::vec3 nearestEntityPos;
  std::string keypressed;
  glm::vec3 playerPos;
  bool readyToFire = true;
  int fireCount = 0;
  std::vector<Entity*> * entities;
  bool dead = false;
  bool killed = false;
  bool firing = false;
  bool canFire = true;
  std::string type;
  glm::vec3 min;
  glm::vec3 max;
  bool projColliding = false;
  glm::vec3 vel;
  glm::vec3 lastPos;
  glm::vec3 lastPosStatic;
  glm::vec3 pos;
  glm::vec3 rot;
  std::string fire_type = "fireball";
  int hp;
  int count = 0;
  std::vector<int> counters;
  std::vector<std::string> spawnedEntityNames;
  std::vector<glm::vec2> spawnedEntityPosition;
  std::vector<SpawnData> spawnedEntityData;
  Box collider;
  std::map<std::string,float> FloatsVars;
  std::map<std::string,bool> BoolVars;
  std::map<std::string,std::string> StringVars;
  bool canBeHit = true;
  int countSHIELD = 0;
  std::vector<Entity*> * allEntities;
  glm::vec3 testVel;
  float scale;
  bool hasCollisionError = false;
  bool globalFrozen = false;
  bool frozen = false;
  bool firstColl = false;
  double speedModifier = 1.0;
  luabridge::LuaRef UpdateFunction;
  luabridge::LuaRef OnStartFunction;
  q3Scene * scenePointer;
  glm::vec3 lastCollSize = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projMin = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projMax = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projPos = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 projScale = glm::vec3(0.0f,0.0f,0.0f);
  Object CollisionObject;
  Object CollisionObject2;
  float largestScale = 0.0f;
  glm::vec3 projPosDiff = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec2 startingPos = glm::vec2(0.0f,0.0f);
  glm::vec3 Min = glm::vec3(-5.0f,-5.0f,-5.0f);
  glm::vec3 Max = glm::vec3(5.0f, 5.0f, 5.0f);
  bool moved = false;
  glm::vec3 scaleColl = glm::vec3(5.0f,5.0f,5.0f);
  int damageAnim = 0;
  int keyCount = 0;
  lua_State* LPointer;
  glm::vec3 playerVel;
  std::string * textPointer;
  bool modText = false;
  bool isPaused;
  float mainSpeed = 0.5f;
protected:
private:
  Uint8* keys;
  std::map<std::type_index, Component*> components;
  // LuaEntityHandle handle;
  float lastDist = 0.0f;
  bool countData = false;
  SDL_Event e;
  std::vector<glm::vec2> proj;
};
template <typename T>
void addComponent(Entity* e, luabridge::LuaRef& componentTable) {
  e->addComponent(std::type_index(typeid(T)), new T(componentTable));
}
static Entity* loadEntity(lua_State* L, const std::string& type) {
  auto e = new Entity(L);
  e->setType(type);
  e->addFunctions(L);
  luabridge::LuaRef entityRef = luabridge::getGlobal(L,type.c_str());
  for (int i = 0; i < entityRef.length(); ++i) {
    std::string componentName = entityRef[i+1]["componentName"];
    if (componentName == "GraphicsComponent") {
      luabridge::LuaRef gcTable = entityRef[i+1];

      addComponent<GraphicsComponent>(e,gcTable);

    }if (componentName == "StaticGraphicsComponent") {
      luabridge::LuaRef sgcTable = entityRef[i+1];
      addComponent<StaticGraphicsComponent>(e,sgcTable);
    }else if (componentName == "NpcComponent") {
      luabridge::LuaRef npcTable = entityRef[i+1];
      addComponent<NpcComponent>(e,npcTable);
    }else if (componentName == "CameraComponent") {
      luabridge::LuaRef camTable = entityRef[i+1];
      addComponent<CameraComponent>(e,camTable);
    }else if (componentName == "BasicComponent") {
      luabridge::LuaRef basicTable = entityRef[i+1];
      // e->onStartFunc = basicTable["onStart"];
    }else if (componentName == "ProjComponent") {
      luabridge::LuaRef camTable = entityRef[i+1];
      addComponent<ProjectileComponent>(e,camTable);
    }else if (componentName == "CollisionComponent") {
      luabridge::LuaRef collisionTable = entityRef[i+1];
      luabridge::LuaRef scaleTable = collisionTable["scale"];
      luabridge::LuaRef x = scaleTable["x"];
      luabridge::LuaRef z = scaleTable["y"];
      luabridge::LuaRef y = scaleTable["z"];
      e->Min.x = -x.cast<float>();
      e->Min.y = -y.cast<float>();
      e->Min.z = -z.cast<float>();
      e->Max.x = x.cast<float>();
      e->Max.y = y.cast<float>();
      e->Max.z = z.cast<float>();
      e->scaleColl = glm::vec3(x.cast<float>(),z.cast<float>(),y.cast<float>());
    }
    // std::cout << "Added " << componentName << " to " << type << '\n';
  }
  e->CompileLuaFunctions(L);
  return e;
}

#endif
