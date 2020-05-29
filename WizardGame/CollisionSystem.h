#ifndef CSYSTEM_H
#define CSYSTEM_H
#include <qu3e/q3.h>
#include "messaging.h"
#include "camera.h"
#include "object.h"
class Box {
public:
  Box(glm::vec3 scale, q3Scene * scene) {
    q3BodyDef bodyDef;

    bodyDef.bodyType = eDynamicBody;
    // bodyDef.gravityScale = 0.0f;
    body = scene->CreateBody(bodyDef);
    q3BoxDef boxDef;
    q3Transform localSpace;
    q3Identity(localSpace);
    boxDef.Set(localSpace,q3Vec3(scale.x,scale.y,scale.z));
    body->AddBox(boxDef);
  }

  void init(glm::vec3 scale,glm::vec3 pos, q3Scene * scene,q3BodyType type) {

    q3BodyDef bodyDef;
    bodyDef.axis = q3Vec3(0.01f,0.01f,0.01f);
    bodyDef.bodyType = type;
    // bodyDef.gravityScale = 0.0f;
    body = scene->CreateBody(bodyDef);
    body->SetTransform(q3Vec3(pos.x,pos.y,pos.z));
    q3BoxDef boxDef;
    q3Transform localSpace;
    q3Identity(localSpace);
    boxDef.Set(localSpace,q3Vec3(scale.x,scale.y,scale.z));
    body->AddBox(boxDef);
  }
  void initBody(q3Scene * scene,q3BodyType type, glm::vec3 position) {
    q3BodyDef bodyDef;
    bodyDef.axis = q3Vec3(0.01f,0.01f,0.01f);
    bodyDef.position = q3Vec3(position.x,position.y,position.z);
    bodyDef.bodyType = type;
    // bodyDef.gravityScale = 0.0f;
    body = scene->CreateBody(bodyDef);
  }
  void addBox(glm::vec3 pos, glm::vec3 scale) {
    q3BoxDef boxDef;
    q3Transform localSpace;
    q3Identity(localSpace);
    localSpace.position = q3Vec3(pos.x,pos.y,pos.z);
    // body->SetTransform(q3Vec3(pos.x,pos.y,pos.z));
    boxDef.Set(localSpace,q3Vec3(scale.x,scale.y,scale.z));
    this->sc = scale;
    this->boxPos = pos;
    body->AddBox(boxDef);
  }
  void resetBox() {
    body->RemoveAllBoxes();
  }
  void resetVelocity() {
    body->SetLinearVelocity(q3Vec3(0.0f,0.0f,0.0f));
  }
  q3Vec3 getVelocity() {
    return body->GetLinearVelocity();
  }
  void setVelocity(glm::vec3 vel) {
    body->ApplyLinearImpulse(q3Vec3(vel.x,vel.y,vel.z));
  }
  Box() {}
  void setPos(glm::vec3 pos) {
    body->SetTransform(q3Vec3(pos.x,pos.y,pos.z));
  }
  glm::vec3 getPos() {
    glm::vec3 pos = glm::vec3(body->GetTransform().position.x,body->GetTransform().position.y,body->GetTransform().position.z);
    return pos;
  }
  void renderBox(Camera camera) {
    // o->setPos(glm::vec3(getPos().x,getPos().y,getPos().z));
    // o->setScale(sc);
    // o->Draw(camera);
  }
  std::string name;
  int id;
  q3Body* body;
  glm::vec3 sc;
  glm::vec3 boxPos;
private:

};

class CollisionSystem {
public:
  CollisionSystem() : scene(1.0/60.0){

  }
  void addBox(glm::vec3 scale,std::string name,int id) {
    Box * b = new Box(scale,&scene);
    b->name = name;
    b->id = id;
    colliders.push_back(b);
  }
  void addBoxChar(glm::vec3 scale,std::string name,int id) {
    Box * b = new Box(scale,&scene);
    b->name = name;
    b->id = id;
    characters.push_back(b);
  }
  void Update() {
    std::vector<glm::vec3> positions;
    positions = messager.getMessage().Vec3Vector;

    std::vector<glm::vec2> charPos;
    charPos = messager.getMessage().Vec2Vector;

    for (int i = 0; i < positions.size(); i++) {
      colliders[i]->body->SetTransform(q3Vec3(positions[i].x,positions[i].y,positions[i].z));
    }
    for (int i = 0; i < charPos.size(); i++) {
      colliders[i]->body->SetTransform(q3Vec3(charPos[i].x,0.0f,charPos[i].y));
    }
    scene.Step();

    std::vector<glm::vec3> newPos;
    for (Box* c : colliders) {
      newPos.push_back(glm::vec3(c->body->GetTransform().position.x,c->body->GetTransform().position.y,c->body->GetTransform().position.z));
    }
    std::vector<glm::vec2> newPos2;
    for (Box* c : characters) {
      newPos2.push_back(glm::vec2(c->body->GetTransform().position.x,c->body->GetTransform().position.z));
    }
    messager.sendMessage(Message("EntitySystem",newPos2));

  }
private:
  q3Scene scene;
  std::vector<Box*> colliders;
  std::vector<Box*> characters;
  Messager messager;
};

#endif
