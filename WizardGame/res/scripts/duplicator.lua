duplicator = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/crawler"
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.5,
    colorG = 1.0,
    colorB = 1.0,
    speed = 2.0,
    range = 100
  }
}
function duplicator_Hit(e,hits)
  if (hits > 0) then
    e:setColor(1.0,1.0,0.0,1.0)
  else
    e:setColor(0.9,0.1,0.9,1.0)
  end
  e:write(hits+1)
  e:spawnEntity("test2",e:getX(),e:getY());
  e:setHP(0)
end
function duplicator_Update(e)
  e:moveForward(0.25)
  e:setColor(1.0,0.0,0.0,1.0)
  if (e:getHP() <= 0) then
    e:kill()
  end
  e:lookAtPlayer()
end
function duplicator_Start(e)
  e:setScale(0.75)
  e:setAnimation(0)
  e:setHP(20)
  e:addCounter(0)
end
