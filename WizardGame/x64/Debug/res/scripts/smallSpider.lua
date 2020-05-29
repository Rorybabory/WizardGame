smallSpider = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/spider"
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 0.5,
      y = 0.5,
      z = 0.5
    }
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.028,
    colorG = 0.027,
    colorB = 0.004,
    speed = 1.0,
    range = 100,
    height = 8.0,
    delay = 96
  }
}
smallSpider_distance = 0.0
function smallSpider_Hit(e,e2,hits)
  -- e2:setColor(0.0,0.0,0.0,1.0)
  e2:setHP(e2:getHP()-1)
  -- if (e:getAnimation() == 2) then
  --   e:playAnimation(0)
  -- end
  if (e2:isPlayer() == true) then
    e2:Shake(5.0)
  end
  e2:write(e2:getHP())
end

function smallSpider_Update(e)
  e:moveForward(0.5)
  e:lookAtPlayer()
end
function smallSpider_Start(e)
  e:setHP(1)
  e:lookAtPlayer()
  e:setScale(1.0)
  e:write("RAN START")
  e:setColor(0.0,0.0,0.0,0.0)
  e:setCollisionBox(5.0,2.0,2.0)
  e:restartCollisionBox()
  e:setDamageAnimation(1)
  e:setAnimationTag("default",0)
  e:setAnimationTag("damaged",0)
  e:setAnimationTag("attack",0)
  e:setBool("isCounting", false)
end
