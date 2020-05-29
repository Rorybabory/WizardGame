spider = {
  {
    componentName = "StaticGraphicsComponent",
    folder = "./res/models/spider.obj",
    color = {
      r = 0.0,
      g = 0.0,
      b = 0.0,
      a = 1.0
    },
    size = 1.0,
    canMove = true
  },
  {
    componentName = "CollisionComponent",
    scale = 10.0
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.2,
    colorG = 0.5,
    colorB = 0.0,
    speed = 1.0,
    range = 100,
    height = 8.0,
    delay = 96
  }
}
function spider_Hit(e,e2,hits)
  e:kill()
end
function spider_Update(e)
  e:moveForward(0.2)
  e:lookAtPlayer()
end
function spider_Start(e)
  e:setHP(2)
end
