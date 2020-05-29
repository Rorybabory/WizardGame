#ifndef PROP_H
#define PROP_H
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <glfreetype/TextRenderer.hpp>
#include "object.h"
#include "CollisionSystem.h"

#include "luaLibrary.h" 
class Prop {
public:
  Prop(std::string folder,glm::vec3 scale,glm::vec3 pos,q3Scene * scene, bool collision)  : collisionObject("./res/cube.obj",glm::vec4(1.0,0.0,0.0,1.0),"./res/basicShader",false) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luah::loadScript(L,"./res/props/" + folder + "/data.lua");
    b.initBody(scene,eStaticBody,glm::vec3(0.0f,0.0f,0.0f));


    luabridge::LuaRef propRef = luabridge::getGlobal(L,folder.c_str());
    for (int i = 0; i < propRef.length(); ++i) {
      luabridge::LuaRef prop = propRef[i+1];
      std::string name = prop["name"];

      float r = prop["r"];
      float g = prop["g"];
      float b2 = prop["b"];
      // std::cout << name << '\n';

      std::string path = "./res/props/"+folder+"/"+name+".obj";
      Object * object = new Object(path,glm::vec4(r,g,b2,1.0f),"./res/basicShader",false);
      object->setPos(pos);
      object->setScale(scale);
      objects.push_back(object);
      // glm::vec3 s;
      // Box * box = new Box();
      glm::vec3 tempSize = object->getSize();
      if (tempSize.z == tempSize.z) {
        std::cout << "SCALE ISN'T NULL" << std::endl;
        if (tempSize.x>sc.x) {
          sc.x = tempSize.x;
        }
        if (tempSize.y>sc.y) {
          sc.y = tempSize.y;
        }
        if (tempSize.z>sc.z) {
          sc.z = tempSize.z;
        }

      }
      // if (collision == true) {
      //   b.addBox(pos,object->getSize());
      // }
      // box->init(object->getSize(),pos,scene,eStaticBody);
      // boxes.push_back(box);
      // std::cout << "created prop" << '\n';
      // std::cout << name << " " << tempSize.x << " " << tempSize.y << " " << tempSize.z << std::endl;
    }
    this->collision = collision;
    if (collision == true) {
      if (sc.x < 10.0) {sc.x = 10.0;}
      if (sc.y < 10.0) {sc.y = 10.0;}
      if (sc.z < 10.0) {sc.z = 10.0;}
      sc.x += 3.0;
      sc.y += 3.0;
      sc.z += 3.0;
      sc.y *= 2.0f;
      b.addBox(pos,sc);
      if (sc.z != sc.z) {
        std::cout << "SCALE IS NULL" << std::endl;
      }
      // std::cout << folder << " " << sc.x << " " << sc.y << " " << sc.z << std::endl;
    }
    // texture.InitTex("./res/props/"+folder+"/texture.png");
    // std::cout << folder << ": " <<
    // b.setPos(pos);
  }
  void setUpCollider(q3Scene * scene) {
    if (collision == true) {
      // b.initBody(scene,eStaticBody,glm::vec3(0.0f,0.0f,0.0f));
      // b.addBox(pos,sc);
      b.init(sc,pos,scene,eStaticBody);
    }
  }


  Prop(std::string folder) {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luah::loadScript(L,"./res/props/" + folder + "/data.lua");
    luabridge::LuaRef propRef = luabridge::getGlobal(L,folder.c_str());
    for (int i = 0; i < propRef.length(); ++i) {
      luabridge::LuaRef prop = propRef[i+1];
      std::string name = prop["name"];

      float r = prop["r"];
      float g = prop["g"];
      float b = prop["b"];

      std::string path = "./res/props/"+folder+"/"+name+".obj";
      Object * object = new Object(path,glm::vec4(r,g,b,1.0f),"./res/basicShader",false);
      objects.push_back(object);
      glm::vec3 s;

      // b->addBox(glm::vec3(0.0f,0.0f,0.0f));
    }
  }
  void setPos(glm::vec3 pos) {
    for (Object * o : objects) {
      o->setPos(pos);
    }
    b.setPos(pos);
    this->pos = pos;
  }
  void setScale(float scale) {
    for (Object * o : objects) {
      o->setScale(glm::vec3(scale,scale,scale));
    }
  }
  void Draw(Camera camera) {
//    texture.Bind(0);
    for (Object * o : objects) {
      o->Draw(camera);
    }
    // texture.UnBind();
    // collisionObject.setScale(sc);
    // collisionObject.setPos(pos);
    // collisionObject.Draw(camera);
  }

  void Update() {
    for (Object * o : objects) {
      o->Update();
      // o->UpdateMinMax();
    }
    b.body->SetToAwake();
    // b.setPos(pos);
    b.body->SetTransform(q3Vec3(0.0f,0.0f,0.0f));
    // if (pos.x < -1000.0f || pos.x > 1000.0f ||
    //     pos.y < -1000.0f || pos.y > 1000.0f ||
    //     pos.z < -1000.0f || pos.z > 1000.0f) {
    //       b.body->SetTransform(q3Vec3(0.0f,0.0f,0.0f),q3Vec3(0.01f,0.01f,0.01f), 0.01f);
    // }
  }
  bool isColliding(glm::vec3 point) {
    bool collision = false;
    for (Object * o : objects) {
      if (o->CheckCollision(point)) {
        collision = true;
      }
    }
    return collision;
  }
  void setID(int id) {
    this->id = id;
  }
  Texture texture;
  glm::vec3 pos;
  Box b;
  std::vector<Box*> boxes;
  glm::vec3 offset;
private:
  bool collision = false;
  std::vector<Object*> objects;
  int id;
  std::string name;
  glm::vec3 sc = glm::vec3(0.0f,0.0f,0.0f);
  Object collisionObject;
};

// class PropSystem {
// public:
//   void addProp(std::string folder, glm::vec3 pos, float scale, int id) {
//     Prop * p = new Prop(folder);
//     p->setPos(pos);
//     p->setScale(scale);
//     p->setID(id);
//     props.push_back(p);
//   }
//
// private:
//   std::vector<Prop*> props;
//   Messager messager;
// };
#endif
