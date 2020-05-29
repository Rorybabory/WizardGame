#ifndef ESYSTEM_H
#define ESYSTEM_H
#include "entity.h"
#include "luaLibrary.h"


#include "camera.h"
#include "messaging.h"
#include "CollisionSystem.h"
#include <vector>
#include <string>
#include <math.h>
#include <qu3e/q3.h>
#include <stdio.h>
#include <cmath>
#include "prop.h"
#include "ProjComponent.h"
#include "imgui_sdl.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

extern double FPS;

extern int globalVariable;
extern void addTextBox(std::string text, glm::vec2 pos, glm::vec3 color, int scale);
extern void clearText();
extern void drawText();

class EntitySystem {
public:
   void drawConsoleWindow() {
        ImGui::Begin("Sample window");
        ImGui::SetWindowSize(ImVec2(300, 300));
        ImGui::Text("This is a console command window");
        ImGui::Text(commandLog.c_str());
        ImGui::InputText("", command, 255);
        ImGui::End();
    }
  int roundToNearest(int value, int nearest) {
    return value + ((nearest-(value % nearest)) % nearest);
  }
  void addEntity(std::string script, std::string type, lua_State* L) {
    luaL_openlibs(L);
    luah::loadScript(L,script);
    luabridge::LuaRef entityRef = luabridge::getGlobal(L,"entities");
    for (int i = 0; i < entityRef.length(); ++i) {
      std::string name = entityRef[i+1].cast<std::string>();
      luah::loadScript(L,"res/scripts/" + name + ".lua");
    }
    Entity * e = loadEntity(L,type);
    e->setUpCollider(&scene,e->scaleColl);
    e->LPointer = L;
    e->OnStart(L);
    entities.push_back(e);
  }
  void addProp(std::string folder,glm::vec3 scale,glm::vec3 pos1, bool collide) {
    Prop * p = new Prop(folder,scale,pos1,&scene, collide);
    p->b.setPos(pos1);
    props.push_back(p);
  }
  void addEntityAtPos(std::string script, std::string type, glm::vec2 pos, lua_State* L) {
    luaL_openlibs(L);
    luah::loadScript(L,script);
    luabridge::LuaRef entityRef = luabridge::getGlobal(L,"entities");
    for (int i = 0; i < entityRef.length(); ++i) {
      std::string name = entityRef[i+1].cast<std::string>();
    }
    luah::loadScript(L,"res/scripts/" + type + ".lua");

    Entity * e = loadEntity(L,type);

    if (type != "player") {
      e->setPos(pos.x,0.0f,pos.y);
    }else {
      e->setPos(pos.x,0.0f,pos.y);
      e->setCamPos(glm::vec3(pos.x,-15.0f,pos.y));
    }
    e->setUpCollider(&scene,e->scaleColl);
    e->OnStart(L);
    entities.push_back(e);
    e->startingPos = pos;
    e->pos = glm::vec3(pos.x, 0.0f, pos.y);
    this->L = L;
  }
  void UpdateDistances() {
    for (Entity * mainEnt : entities) {
      float smallestDist = 10000.0f;
      for (Entity * targetEnt : entities) {
          float dist = sqrt(pow(mainEnt->pos.x-targetEnt->pos.x,2)+
                            pow(mainEnt->pos.z-targetEnt->pos.z,2)*1.0f);
          if (dist < smallestDist) {
            smallestDist = dist;
            mainEnt->nearestEntityDist = smallestDist;
            if (targetEnt->type == "player") {
              mainEnt->nearestEntityPos = playerPos;
            }else {
              mainEnt->nearestEntityPos = targetEnt->pos;
            }
            mainEnt->nearestEntityType = targetEnt->type;
          }
      }
    }
  }
  Entity* getEntityByName(std::string name) {
    for (Entity * entity : entities) {
      if (strcmp(entity->getType().c_str(),name.c_str())) {
        return entity;
      }
    }
  }
  void restartCollision() {
    scene = q3Scene(1.0/60.0);
    for (Entity * e : entities) {
      e->setUpCollider(&scene, glm::vec3(e->scale,10,e->scale));
    }
    std::cout << "Scene Restarting..." << std::endl;

    for (Prop * prop : props) {
      prop->setUpCollider(&scene);
    }
  }
  EntitySystem ()  : scene(1.0/60.0) {
    // Box b;
    text = new std::string("MEME");
    // b.init(glm::vec3(0.01f,0.01f,0.01f),glm::vec3(0.0f,0.0f,0.0f),&scene,eDynamicBody);
    addTextBox("DANK MEMES",glm::vec2(0.0f,1.0f), glm::vec3(0.0f,1.0f,0.0f), 30);
    floor.init(glm::vec3(1000.0f,1.0,1000.0f),glm::vec3(0.0f,0.0f,0.0f),&scene, eStaticBody);
    // luaL_openlibs(L);
    // luah::loadScript(L,script);
    // luabridge::LuaRef entityRef = luabridge::getGlobal(L,"entities");
    // for (int i = 0; i < entityRef.length(); ++i) {
    //   animatedObject * ao = new animatedObject();
    //   std::string name = entityRef[i+1].cast<std::string>();
    //   luah::loadScript(L,"res/scripts/" + name + ".lua");
    //   Entity * e = loadEntity(L,name);
    //   e->setUpCollider(&scene,glm::vec3(5.0f,500.0f,5.0f));
    //   e->OnStart(L);
    //   startEnt.push_back(e);
    // }
  }
  Entity* getEntityByID(int id) {
    return entities[id];
  }
  void checkSpawnedEntities(lua_State* L) {
    for (Entity * e : entities) {
      for (SpawnData sD : e->spawnedEntityData) {
        addEntityAtPos("res/scripts/entities.lua",sD.name,sD.pos,L);
      }
      if (e->spawnedEntityData.size()>0) {
        e->spawnedEntityData.clear();
      }
    }
  }
  void checkCollision() {
    for (Entity * e : entities) {
      bool c = false;
      for (int i = 0; i<messager.getMessage().Vec2Vector.size(); i++) {
        if (sqrt(pow(messager.getMessage().Vec2Vector[i].x-e->pos.x,2)+
                 pow(messager.getMessage().Vec2Vector[i].y-e->pos.z,2))<2.0f) {
              if (!messager.getMessage().StringVector.empty()) {
                if (messager.getMessage().StringVector[i] != e->type) {
                  c = true;
                }
              }
        }
      }
      e->projColliding = c;
    }
  }
  void checkForCollisionError() {
    for (Entity * e : entities) {
      if (e->type == "player" && e->hasCollisionError == true) {
        // restartCollision();
        e->hasCollisionError = false;
        std::cout << "COLLISION ERROR FOUND. POSITION IS NULL" << std::endl;
      }
    }
  }
  void DumpToFile() {
    FILE * pFile;
    char buffer [100];
#pragma warning(disable:4996)
    pFile = fopen("res/scripts/dumps/dump1.txt","w+");
    if (pFile == NULL) {
      std::cout << "error opening file..." << '\n';
    }
    for (Entity * e : entities) {
      if (e->type == "player") {
        e->collider.body->Dump(pFile,1);
      }
    }
  }
  void Update(lua_State* L) {
    checkCollision();
    checkSpawnedEntities(L);
    // UpdateDistances();
    setHeroTarget();
    checkForCollisionError();
    checkForConsole();
  //  clearText();
  //  addTextBox(*text,glm::vec2(-0.9f,0.9f), glm::vec3(1.0f,1.0f,0.0f), 30);
    if (FPS < 35.0) {
      std::cout << "FPS: " << FPS << std::endl;
    }

    if (printDelta==true) {
      printDelta=false;
    }else {
      printDelta = true;
    }
    if (printDelta == true) {
      if ((1000/FPS)/16.6666<2.0) {
        speedMultiplier = (1000/FPS)/20;
      }
    }
    // speedMultiplier = ((-((FPS/60)-1.0))*2.0)+1.0;
    // std::cout << "Global int: " << globalVariable << std::endl;
    // if (dumped==false) {
    //   DumpToFile();
    //   dumped = true;
    // }

    for (int i = 0; i<entities.size(); i++) {
      // if (entities[i]->type == "player") {
      //   std::cout << entities[i]->collider.body->GetTransform( ).position.z << "\n";
      // }
      entities[i]->allEntities = &entities;
      entities[i]->scenePointer = &scene;
      entities[i]->speedModifier = speedMultiplier;
      auto projc = entities[i]->get<ProjectileComponent>();
      auto sgraphicsc = entities[i]->get<StaticGraphicsComponent>();
      // if (sgraphicsc != NULL && !sgraphicsc->canMove && entities[i]->type == "player") {
      //   entities[i]->lastPosStatic = entities[i]->pos;
      // }
      if (projc != NULL) {
        projc->collUpdate();
        projc->setSpeedMultiplier(speedMultiplier);
      }
      if (entities[i]->type == "player") {
        if (entities[i]->killed == true) {
          DumpToFile();
          exit(0);
        }
      }

      if (entities[i]->type != "player" || entities[i]->type != "AIHero") {
        entities[i]->collider.setPos(glm::vec3(entities[i]->pos.x,0.0f,entities[i]->pos.z));
      }else {

        entities[i]->collider.setPos(glm::vec3(playerPos.x,0.0f,playerPos.z));
      }
      // if (entities[i]->killed == true) {
      //   // scene.RemoveBody(entities[i]->collider.body);
      //   entities.erase(entities.begin()+i);
      //   delete entities[i];
      // }
        entities[i]->lastPos = entities[i]->pos;
    }
    scene.Step();

    for (Entity * e : entities) {
      if (e->modText == true) {
        text = e->textPointer;
        e->modText = false;
      }
      // if (e->moved == false && e->type == "player") {
      //   e->pos.x = e->startingPos.x;
      //   e->pos.z = e->startingPos.y;
      //   e->collider.setPos(glm::vec3(e->startingPos.x, 0.0, e->startingPos.y));
      // }
      if (e->type != "player") {
        e->pos = glm::vec3(e->collider.getPos().x,0.0f,e->collider.getPos().z);
      }else {
        e->pos = glm::vec3(e->collider.getPos().x,0.0f,e->collider.getPos().z);

        //e->setCamPos(glm::vec3(e->collider.getPos().x,2.0f,e->collider.getPos().z));
        //e->collider.setPos(e->getCamPos());
      }
      collisions++;
      if (collisions == 2) {
        collisions = 0;
      }
      e->collider.resetVelocity();

      e->collider.body->SetToAwake();
      auto sgraphicsc = e->get<StaticGraphicsComponent>();
      if (sgraphicsc != NULL && !sgraphicsc->canMove) {
        e->pos = e->lastPosStatic;
      }


      auto projc = e->get<ProjectileComponent>();
      if (e->type == "AIHero") {
        if ( e->collider.getVelocity().x != 0.0f
          && e->collider.getVelocity().z != 0.0f
          && e->collider.getVelocity().y != 0.0f) {
          std::cout << "ERROR....." << std::endl;
        }
      }
      if (projc != NULL) {
        for (int i = 0; i < projc->objects.size(); i++) {
          if (projc->objects[i]->boxDestroy == true) {

            scene.RemoveBody(projc->objects[i]->projB.body);
            delete projc->objects[i];
            projc->objects.erase(projc->objects.begin()+i);
          }
        }
        if (frozen == false) {
            projc->Update();
        }
      }
      if (e->type == "player") {
        if (e->globalFrozen == true) {
          frozen = true;
        }else {
          if (frozen == true) {
            for (Entity * e2 : entities) {
              auto projc2 = e2->get<ProjectileComponent>();
              if (projc2 != NULL) {
                projc2->delayCount = 0;
              }
            }
          }
          frozen = false;
        }
        if (e->pos.x < -1000.0f || e->pos.x > 1000.0f ||
            e->pos.z < -1000.0f || e->pos.z > 1000.0f) {
          // e->setCamPos(glm::vec3(0.0f,10.0f,0.0f));
          auto cameraC = e->get<CameraComponent>();
          if (cameraC->isFirstPerson == false) {
            e->setCamPos(glm::vec3(0.0f,500.0f,0.0f));
          }else {
            e->pos = glm::vec3(0.0f,10.0f,0.0f);
          }
          std::cout << "TELEPORTED PLAYER ENTITY" << std::endl;
        }
      }
      if (projc != NULL) {
        if (!projc->objects.empty()) {
          for (Object * o : projc->objects) {

            if (o->hasInit == false) {
              o->projB.init(glm::vec3(2.0f,2.0f,2.0f),glm::vec3(0.0f,0.0f,0.0f),&scene,eDynamicBody);
              // o->projB.setPos(glm::vec3(o->projB.getPos().x-o->forward.x*5.0f,o->projB.getPos().y,o->projB.getPos().z-o->forward.z*5.0f));

              o->projB.body->SetLinearVelocity(q3Vec3(o->forward.x*projc->speed*projc->SpeedMultiplier*100.0f,o->forward.y*projc->speed*projc->SpeedMultiplier*100.0f,o->forward.z*projc->speed*projc->SpeedMultiplier*100.0f));
              o->hasInit = true;
            }else {
              // std::cout << "Forward Firing:" << o->projB.body->GetLinearVelocity().y << std::endl;
            }
          }
          // for (o : projc->objects) {
          //   o->projB.init(glm::vec3(1.0f,1.0f,1.0f),glm::vec3(0.0f,0.0f,0.0f),&scene,eDynamicBody);
          // }
          // for (p : props) {
          //   bool coll = projc->checkCollisionWithBox(glm::vec2(p->pos.x,p->pos.z),glm::vec2(p->b.sc.x,p->b.sc.z));
          //   if (coll) {
          //     count++;
          //     std::cout << "BULLET COLLISION " << count << '\n';
          //   }
          // }
        }
      }
      // if (!messager.getMessage().Vec2Vector.empty()) {
      //   e->UpdateProj(messager.getMessage().Vec2Vector);
      // }
      e->entities = &entities;
      e->setPlayerPos(playerPos);
      e->isPaused = isConsoleOpen;

      if (frozen == false && isConsoleOpen == false) {
        e->Update(L,&scene);
      }else {
        if (e->type == "player") {
          e->Update(L,&scene);
        }
      }
      if (e->firing == true && e->canFire == true) {
        if (e->type == "player") {
          e->canFire = false;
          messager.sendMessage(Message("ProjectileManager", glm::vec2(e->getCamPos().x-(e->getPlayerRot().x*3.5f),e->getCamPos().z-(e->getPlayerRot().z*3.5f)),e->getPlayerRot(),e->fire_type));
        }else {
          messager.sendMessage(Message("ProjectileManager", glm::vec2(e->pos.x-(e->getForward().x*3.5f),e->pos.z-(e->getForward().z*3.5f)),e->getForward(),e->fire_type));
        }
        e->firing = false;
      }else if (e->type == "player") {
        messager.clearBus();
      }
       bool coll = false;
       for (Entity * eTarget : entities) {
         auto projc = eTarget->get<ProjectileComponent>();

         if (projc != NULL) {
            projc->Draw(getMainCam());
           for (Object * o : projc->objects) {
            // o->Draw(getMainCam());
            if (eTarget->type != e->type) {
              // float dist = sqrt(pow(e->pos.x-o->getPos().x,2)+
              //                    pow(e->pos.z-o->getPos().z,2)*1.0f);
              float dist3d = sqrt(pow(e->pos.x - o->getPos().x, 2) +
                                  pow(e->pos.y - o->getPos().y, 2) +
                                  pow(e->pos.z - o->getPos().z, 2) * 1.0);
              // std::cout << "PROJECTILE Y: " << o->getPos().y << " MIN Y: " << e->projMin.y << "\n";
              if (e->getCollisionWithPoint(o->getPos()) == true && e->type != eTarget->type && !e->dead) {
                if (e->canBeHit && frozen == false && o->destroy == false) {
                  coll = true;
                  o->destroy = true;
                  eTarget->Hit(L, o->timesBounced,e);
                }
              }
            }

          }
        }
      }
      e->projColliding = coll;
    } // end of loop
  }
  Entity* getEntity(std::string type) {
      for (Entity* e : entities) {
          if (e->type == type) {
              return e;
          }
      }
  }
  void runPrintCommand(std::vector<std::string> tokens) {
      std::string tempString;
      for (int i = 1; i < tokens.size(); i++) {
          tempString += tokens[i];
          tempString += " ";
      }
      tempString += "\n";
      commandLog += tempString;

  }
  void spawnEntityCommand(std::vector<std::string> tokens) {
      addEntity("res/scripts/entities.lua", tokens[1], L);
      commandLog += "added entity ";
      commandLog += tokens[1];
      commandLog += "\n";
  }
  void setSpeedCommand(std::vector<std::string> tokens) {
      for (Entity * e : entities) {
          if (e->type == "player") {
              e->mainSpeed = ::atof(tokens[1].c_str());
          }
      }
  }
  void runCommand() {
      std::cout << "run command\n";
      std::string cmd = std::string(command);
      std::vector <std::string> tokens;
      stringstream check1(cmd);
      string intermediate;
      while (std::getline(check1, intermediate, ' '))
      {
          tokens.push_back(intermediate);
      }
      
      if (tokens[0] == "print") {
          runPrintCommand(tokens);
      }
      else if (tokens[0] == "spawnEntity") {
          spawnEntityCommand(tokens);
      }
      else if (tokens[0] == "setSpeed") {
          setSpeedCommand(tokens);
      }
  }
  void checkForConsole() {
      Uint8* keys;
      keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
      SDL_Event e;
      ImGuiIO& io = ImGui::GetIO(); (void)io;
      io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//      SDL_PollEvent(&e);
      for (int i = 0; i <= 255; i++) {
          io.KeysDown[i] = false;
      }
      while (SDL_PollEvent(&e))
      {
          switch (e.type)
          {
          case SDL_TEXTINPUT:
              io.AddInputCharacter(e.text.text[0]);
              break;
          case SDL_KEYUP:
              io.KeysDown[e.key.keysym.scancode] = false;
              io.KeyCtrl = (e.key.keysym.mod & KMOD_CTRL) != 0;
              io.KeyShift = (e.key.keysym.mod & KMOD_SHIFT) != 0;
              break;
              // etc...
          case SDL_KEYDOWN:
              int key = e.key.keysym.sym & ~SDLK_SCANCODE_MASK;
              bool isKeyDown = (e.type == SDL_KEYDOWN);

              std::cout << key << "\n";

              if (key == 8) {
                  std::cout << "backspace\n";
                  io.KeysDown[io.KeyMap[ImGuiKey_Backspace]] = true;
              }
              if (key == 13) {
                  runCommand();
              }
          }
              
         
      }

      if (keys[SDL_SCANCODE_BACKSLASH]) {
          if (isConsoleOpen == true && canKey == true) {
              isConsoleOpen = false;
              canKey = false;
          }
          else if (isConsoleOpen == false && canKey == true) {
              isConsoleOpen = true;
              canKey = false;
          }
      } else {
          canKey = true;
      }
      
      

  }
  void Draw(double deltaTime) {
    if (isConsoleOpen == false) {
        for (Prop* p : props) {
            p->Draw(getMainCam());
            p->Update();
        }
        SDL_SetRelativeMouseMode(SDL_TRUE);

    }
    else {
        SDL_SetRelativeMouseMode(SDL_FALSE);

    }
    
    for (Entity * e : entities) {
      e->Draw(getMainCam(), frozen);
    }
    if (isConsoleOpen == true) {
        drawConsoleWindow();
    }
    
  }
  void setHeroTarget() {
    glm::vec3 target;
    for (Entity * e : entities) {
      e->isPaused = isConsoleOpen;
      auto camc = e->get<CameraComponent>();
      if (camc != NULL) {
        if (camc->isFirstPerson == true) {
          target = camc->camera.m_position;
          playerPos = target;
          break;
        }else {
          for (Entity * e2 : entities) {
            if (e2->type == "AIHero") {
              target = e2->pos;
              playerPos = target;
              break;
            }
          }
          break;
        }
      }
    }
    for (Entity * e : entities) {
      e->playerPos = target;
    }
  }
  Camera getMainCam() {
    for (Entity * e : entities) {
      auto camc = e->get<CameraComponent>();
      if (camc) {
        // playerPos = camc->getCamera().m_position;
        return camc->getCamera();
      }
    }
    Camera badCam;
    std::cout << "No entity with a camera" << '\n';
    return badCam;
  }
  Messager messager;
  glm::vec3 playerPos;
  bool frozen = false;
  bool printDelta = false;
  bool firstCollisionUpdate = true;
  bool isConsoleOpen = false;
  char command[255] = "Input Command Here";
  std::string commandLog = "\n";
private:
  std::string * text;
  bool dumped = false;
  std::vector<Entity*> startEnt;
  q3Scene scene;
  std::vector<Prop*> props;
  int count;
  std::vector<Entity*> entities;
  int collisions = 0;
  double speedMultiplier = 1.0;
  double FPSToUse = 60;
  std::vector<double> FPS_List;
  Box floor;
  bool canKey = true;
  lua_State* L;
};
#endif
