#include "entity.h"
extern int screenInverted;
extern glm::vec2 levelSize;
float Entity::lookAtNearest(std::string targetType) {
    float nearestDist = 100000.0;
    glm::vec2 nearestPos;
    for (Entity * e : *allEntities) {
      if (e->type == targetType) {
        float x;
        x=sqrt(pow(e->pos.x-pos.x, 2) +
               pow(e->pos.z-pos.z, 2));
        if (x < nearestDist) {
          nearestDist = x;
          nearestPos = glm::vec2(e->pos.x,e->pos.z);
        }
      }
    }
    if (nearestDist != 100000.0) {
      nearestPos.x = nearestPos.x - pos.x;
      nearestPos.y = nearestPos.y - pos.z;
      rot.y = -(atan2(nearestPos.y, nearestPos.x))+1.57f;
      rot.x = -1.57f;
      return true;
    }else {
      return false;
    }
}

bool Entity::doesEntityExist(std::string targetType) {
	for (Entity * e : *allEntities) {
		if (e->type == targetType && !e->dead) {
			return true;
		}
	}
	return false;
}
void Entity::setInverted(int i) {
  screenInverted = i;
}
float Entity::getDistanceFromNearest(std::string targetType) {
    float nearestDist = 100000.0;
    for (Entity * e : *allEntities) {
      if (e->type == targetType) {
        float x;
        x=sqrt(pow(e->pos.x-pos.x, 2) +
               pow(e->pos.z-pos.z, 2));
        if (x < nearestDist) {
          nearestDist = x;
        }
      }
    }
    return nearestDist;
}
void Entity::setDamageAnimation(int anim) {
  damageAnim = anim;
}
void Entity::restartCollisionBox() {
  collider.resetBox();
  collider.addBox(glm::vec3(0.0f,0.0f,0.0f),scaleColl*glm::vec3(scale*2.0));
}
void Entity::pushInsideLevel() {
  if (pos.x > levelSize.x) {pos.x = levelSize.x;}
  if (pos.z > levelSize.y) {pos.z = levelSize.y;}

  if (pos.x < -levelSize.x) {pos.x = -levelSize.x;}
  if (pos.z < -levelSize.y) {pos.z = -levelSize.y;}
}
float Entity::getPositionFromNearestX(std::string targetType) {
    float nearestDist = 100000.0;
    glm::vec2 position;
    for (Entity * e : *allEntities) {
      if (e->type == targetType) {
        float x;
        x=sqrt(pow(e->pos.x-pos.x, 2) +
               pow(e->pos.z-pos.z, 2));
        if (x < nearestDist) {
          nearestDist = x;
          position = glm::vec2(e->pos.x,e->pos.z);
        }
      }
    }
    return position.x;
}

float Entity::getPositionFromNearestY(std::string targetType) {
    float nearestDist = 100000.0;
    glm::vec2 position;
    for (Entity * e : *allEntities) {
      if (e->type == targetType) {
        float x;
        x=sqrt(pow(e->pos.x-pos.x, 2) +
               pow(e->pos.z-pos.z, 2));
        if (x < nearestDist) {
          nearestDist = x;
          position = glm::vec2(e->pos.x,e->pos.z);
        }
      }
    }
    return position.y;
}
void Entity::Shake(float amount) {
  auto camera = get<CameraComponent>();
  camera->Shake(amount);
}
void Entity::isFirstPerson(bool firstPerson) {
	auto camera = get<CameraComponent>();
	camera->isFirstPerson = firstPerson;
}

void Entity::setFOV(float FOV) {
    auto camera = get<CameraComponent>();
    if (camera != NULL) {
      camera->setFOV(FOV);
    }
}

int Entity::getAnimFrame() {
    auto graphics = get<GraphicsComponent>();
    return graphics->frame;
}
void Entity::playDamageAnimation() {
    auto graphics = get<GraphicsComponent>();
    graphics->playAnim(damageAnim);
}
void Entity::playAnimation(int id) {
    auto graphics = get<GraphicsComponent>();
    graphics->playAnim(id);
}

void Entity::spawnEntity(std::string name, float x, float y) {
    spawnedEntityData.push_back(SpawnData(name, glm::vec2(x,y)));
}

void Entity::killProgram() {
    exit(0);
}

// bool Entity::GetCollisionWithPoints() {
//     bool projColl = projColliding;
//     if (projColl) {
//       std::cout << "COLLISION SUCCESS..." << '\n';
//     }
//     return projColl;
// }

void Entity::kill() {
    dead = true;
    killed = true;
    collider.body->RemoveAllBoxes();
    auto projc = get<ProjectileComponent>();
    if (projc != NULL) {
      for (Object * o : projc->objects) {
        delete o;
      }
      projc->objects.clear();
    }
}

float Entity::getPlayerDistance() {
    float x;
    x=sqrt(pow(playerPos.x-pos.x, 2) +
           pow(playerPos.z-pos.z, 2));
    return x;
}

void Entity::write(const std::string& str) {
    std::cout << str << '\n';
}

void Entity::setScale(float scale) {
    this->scale = scale;
}

float Entity::random(float min, float max) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float range = max-min;
    return (random*range) + min;
}

void Entity::lookAtPlayer() {
    glm::vec2 targetPos = glm::vec2(playerPos.x,playerPos.z);
    targetPos.x = targetPos.x - pos.x;
    targetPos.y = targetPos.y - pos.z;
    float targetAngle = RADS_TO_DEGS(-(atan2(targetPos.y, targetPos.x))+1.57f);
    float rotDegs = RADS_TO_DEGS(rot.y);
    rotDegs = targetAngle;
    rot.y = DEGS_TO_RADS(rotDegs);
    rot.x = -1.57f;
    rot.z = -0.0f;
    // std::cout << "target is " << targetAngle << " abs is " << std::abs(rotDegs-targetAngle) << " current rotation: " << rotDegs << std::endl;
}

void Entity::lookAt(float x, float y) {
    glm::vec2 targetPos = glm::vec2(x,y);
    targetPos.x = targetPos.x - pos.x;
    targetPos.y = targetPos.y - pos.z;
    float targetAngle = -(atan2(targetPos.y, targetPos.x))+1.57f;
    if (std::abs(rot.y-targetAngle) > 0.05) {
      if (targetAngle>rot.y) {
        rot.y += 0.02;
      }else {
        rot.y -= 0.02;
      }
    }
    rot.x = -1.57f;
}

void Entity::Strafe(float amount) {
    pos += glm::cross(glm::vec3(0.0f,1.0f,0.0f), getForward()) * amount * speedModifier;
}

void Entity::moveForward(float amount) {
    // pos.x = pos.x+(sin(rot.y*180/3.14)*amount);
    // pos.z = pos.z+(cos(rot.y*180/3.14)*amount);
    auto cameraC = get<CameraComponent>();
    if (cameraC != NULL) {
      cameraC->getCamera().MoveForward(amount);
    }else {
      pos.x += getForward().x*amount * speedModifier;
      pos.z += getForward().z*amount * speedModifier;
    }

}

void Entity::moveBackward(float amount) {
    pos.x = pos.x-(cos(rot.y*180/3.14)*-amount * speedModifier);
    pos.z = pos.z-(sin(rot.y*180/3.14)*-amount * speedModifier);
}

void Entity::setAnimation(int id) {
    auto graphics = get<GraphicsComponent>();
    graphics->setAnimID(id);
    graphics->object.lastAnimation = id;
}

int Entity::getAnimation() {
    auto graphics = get<GraphicsComponent>();
    return graphics->getAnimID();
}

void Entity::setPos(float x, float y, float z) {
    if (type == "player") {
      auto cameraC = get<CameraComponent>();
      cameraC->setPos(glm::vec3(x,y,z));
    }else {
      pos = glm::vec3(x,y,z);
    }
}

float Entity::getX() {
    return pos.x;
}

float Entity::getY() {
    return pos.y;
}

float Entity::getZ() {
    return pos.z;
}

void Entity::setColor(float r, float g, float b, float a) {
    auto graphics = get<GraphicsComponent>();
    auto sgraphics = get<StaticGraphicsComponent>();
    if (graphics != NULL) {
      graphics->setColor(glm::vec4(r,g,b,a));
    }else if (sgraphics != NULL) {
      sgraphics->setColor(glm::vec4(r,g,b,a));
    }else {
      std::cout << "COULD NOT SET COLOR OF ENTITY OF TYPE: " << type << std::endl;
    }
}
bool Entity::isPlayer() {
  return (type == "player");
}
float Entity::getKeyDirectionX() {
    SDL_PumpEvents();
    keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    float direction = 0.0f;
    if (keys[SDL_SCANCODE_A]){
      direction -= 1.0f;
    }
    if (keys[SDL_SCANCODE_D]){
      direction += 1.0f;
    }
    return direction;
}

float Entity::getKeyDirectionY() {
    SDL_PumpEvents();
    keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    float direction = 0.0f;
    if (keys[SDL_SCANCODE_S]){
      direction -= 1.0f;
    }
    if (keys[SDL_SCANCODE_W]){
      direction += 1.0f;
    }
    return direction;
}

std::string Entity::getKeyPressed() {
    return keypressed;
}

void Entity::TopDownUpdate() {
    setPos(0.0f,250.0f,0.0f);
    keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    if (allEntities != NULL) {

    }
    if (keys[SDL_SCANCODE_LSHIFT]){
      keypressed = "LSHIFT";
    }
    if (keys[SDL_SCANCODE_ESCAPE]){
      keypressed = "ESCAPE";
    }
    if (keys[SDL_SCANCODE_LCTRL]) {
      keypressed = "LCTRL";
    }
    if (keys[SDL_SCANCODE_TAB]) {
      keypressed = "TAB";
    }
    if (keys[SDL_SCANCODE_SPACE]) {
      keypressed = "SPACE";
    }
    SDL_PollEvent( &e );
}

void Entity::FPSControllerUpdate(float speed) {
    auto cameraC = get<CameraComponent>();
//    pos = cameraC->getCamera().m_position;
    cameraC->lastX = cameraC->x;
    cameraC->lastY = cameraC->y;
    SDL_PumpEvents();
    keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    glm::vec3 startPos;
    startPos = cameraC->getCamera().m_position;
    SDL_PollEvent( &e );
    SDL_GetRelativeMouseState( &cameraC->x, &cameraC->y );
    cameraC->getCamera().RotateY(((float)-cameraC->x/300.0f));
    cameraC->getCamera().Pitch(((float)cameraC->y/300.0f));
    std::string lastKey = keypressed;
//    playerVel = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 startPlayerVel = glm::vec3(0.0f,0.0f,0.0f);
    if (keys[SDL_SCANCODE_S]){
      pos += cameraC->getCamera().m_forward*-speed;
      moved = true;
    }
    if (keys[SDL_SCANCODE_W]){
      pos += cameraC->getCamera().m_forward*speed;
      moved = true;
    }
    if (keys[SDL_SCANCODE_A]){
      pos += glm::cross(cameraC->getCamera().m_up, cameraC->getCamera().m_forward) * speed;
      moved = true;
    }
    if (keys[SDL_SCANCODE_D]){
      pos += glm::cross(cameraC->getCamera().m_up, cameraC->getCamera().m_forward) * -speed;
      moved = true;
    }
    if (startPlayerVel != glm::vec3(0.0f,0.0f,0.0f)) {
        playerVel = startPlayerVel;
    }

    if (startPos == pos) {
      collider.resetVelocity();
    }

//    pos = cameraC->getCamera().m_position;
    cameraC->getCamera().m_position.y = 10.0f;
}
void Entity::UpdateKeyPresses() {
  SDL_PollEvent( &e );
  keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
  std::string tempKeyPressed = "";
  if (keys[SDL_SCANCODE_ESCAPE]){
    tempKeyPressed = "ESCAPE";
  }
  if (keys[SDL_SCANCODE_LCTRL]) {
    tempKeyPressed = "LCTRL";
  }
  if (keys[SDL_SCANCODE_TAB]) {
    tempKeyPressed = "TAB";
  }
  if (keys[SDL_SCANCODE_SPACE]) {
    tempKeyPressed = "SPACE";
  }
  if (keys[SDL_SCANCODE_LSHIFT]){
    tempKeyPressed = "LSHIFT";
  }
  if (tempKeyPressed != keypressed) {
    keyCount++;
    if (keyCount > 3) {
      keyCount = 0;
      keypressed = tempKeyPressed;
    }
  }
}
int Entity::getHP() {
    return hp;
}

void Entity::setHP(int hp) {
    this->hp = hp;
}

void Entity::addCounter(int start) {
    counters.push_back(start);
}

bool Entity::updateCounter(int counter,int end) {
    counters[counter]++;
    if (counters[counter]>=end) {
      counters[counter]=0;
      return true;
    }else {
      return false;
    }
}

int Entity::getCount() {
    return count;
}

void Entity::setCount(int count) {
    this->count=count;
}

bool Entity::Fire() {
    if (!dead) {
      auto projC = get<ProjectileComponent>();
      if (type != "player") {
        if (projC->Fire(glm::vec2(pos.x,pos.z),getForward())) {
          return true;
        }else {
          return false;
        }
      }else {
        if (projC->Fire(glm::vec2(pos.x,pos.z),getCamForward())) {
          return true;
        }else {
          return false;
        }
      }
    }
}

void Entity::setGlobalFrozen(bool f) {
    globalFrozen = f;
}

Entity* Entity::getNearestEntWithName(std::string entityName) {
    Entity * nearest = nullptr;
    float dist = 10000.0f;
    for (Entity * e : *allEntities) {
      if (e->type == entityName) {
        if (dist > getDistanceBetweenTwoPoints(glm::vec2(pos.x,pos.z), glm::vec2(e->pos.x,e->pos.z))) {
          nearest = e;
          dist = getDistanceBetweenTwoPoints(glm::vec2(pos.x,pos.z), glm::vec2(e->pos.x,e->pos.z));
        }
      }
    }
    if (nearest == nullptr) {
        return NULL;
    }
    else {
        return nearest;
    }
}

bool Entity::getGlobalFrozen() {
    return globalFrozen;
}

void Entity::setFrozen(bool f) {
    frozen = f;
}

bool Entity::getFrozen() {
    return frozen;
}

float Entity::getDistanceBetweenTwoPoints(glm::vec2 pos1, glm::vec2 pos2) {
  float x_diff = pos1.x - pos2.x;
  float y_diff = pos1.y - pos2.y;
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}

void Entity::setAnimationTag(std::string tag, int animation) {
  auto gfxc = get<GraphicsComponent>();
  if (gfxc != NULL) {
    gfxc->setAnimationWithTag(tag,animation);
  }
}

void Entity::playAnimationTag(std::string tag) {
  auto gfxc = get<GraphicsComponent>();
  if (gfxc != NULL) {
    gfxc->playAnimTag(tag);
  }
}
bool Entity::isAnimationPlaying(std::string tag) {
  auto gfxc = get<GraphicsComponent>();
  if (gfxc != NULL) {
    return gfxc->isAnimationPlaying(tag);
  }
}

float Entity::getLastX() {return lastPos.x;}
float Entity::getLastY() {return lastPos.y;}
float Entity::getLastZ() {return lastPos.z;}

float Entity::getDistanceBetweenTwoPointsAPI(float x, float y, float x2, float y2) {
  float x_diff = x - x2;
  float y_diff = y - y2;
  return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}
void Entity::setCollisionBox(float width, float height, float length) {
    Min.x = -width;
    Min.y = -height;
    Min.z = -length;
    Max.x = width;
    Max.y = height;
    Max.z = length;
    scaleColl = glm::vec3(width,length,height);
}
void Entity::Delay(int milliseconds) {
    SDL_Delay(milliseconds);
}
void Entity::setUIText(std::string text) {
    textPointer = new std::string(text);
    modText = true;
}
