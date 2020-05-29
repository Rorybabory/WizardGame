zombie = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/basicZombie"
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 0.3,
      y = 0.6,
      z = 0.3
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
function zombie_Hit(e,e2,hits)
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

function zombie_Update(e)
    e:moveForward(0.25)
    e:lookAtPlayer()
end
function zombie_Start(e)
    e:setHP(5)
    e:lookAtPlayer()
    e:setScale(1.0)
    e:setFloat("raiseCount", 0)
    e:setCollisionBox(3.0,3.0,7.5)
    e:restartCollisionBox()
    e:setAnimationTag("default",0)
    e:setAnimationTag("damage",0)
end

