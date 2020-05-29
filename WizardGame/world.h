#ifndef WORLD_H
#define WORLD_H
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#define GLM_ENABLE_EXPERIMENTAL

#include "camera.h"
#include "object.h"
#include "animatedMesh.h"
#include "animatedObject.h"
// #include "collada.h"
#include "NPC.h"
#include "entity.h"
#include "entitySystem.h"
#include "projectile.h"
#include "prop.h"
#include "CollisionSystem.h"
#include <qu3e/q3.h>
#include "map.h"
#include "text.h"
#include "menuSystem.h"
// #include "ui.h"
using namespace std;
class World {
public:
  World() : menu("TestMenu"){
    shader.InitShader("./res/aShader");
    textCamera.InitCam(glm::vec3(0.0f,0.0f,-2.0f), 70.0f, 800.0f/600.0f,0.01f, 1000.0f);
    transform.m_scale = glm::vec3(0.3f,0.3f,0.3f);
    transform.m_rot = glm::vec3(-1.57f,0.0f,0.0f);
    transform.m_pos = glm::vec3(0.0f,-2.0f,0.0f);
    // transform.setScale(glm::vec3(1.0f,1.0f,1.0f));
    luaL_openlibs(L);
    testMap.init("newMap", L);
  };
  virtual ~World();
  void Update();
  void Draw();
  double deltaTime;
  Mesh aMesh;
  Shader shader;
  Transform transform;
  Map testMap;
  // Script script;
  bool countData;
  const double maxFPS = 60.0;
  const double maxPeriod = 1.0/maxFPS;
  double lastTime = 0.0;
  double time;
  lua_State* L = luaL_newstate();
  // Entity* e;
  ProjectileManager pManager;
  // EntitySystem eSystem;
  CollisionSystem cSystem;
  Camera textCamera;
  Menu menu;
  bool printDelta = false;
protected:
private:
  SDL_Renderer * renderer;
 };
#endif
