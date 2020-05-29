blackKnight = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/models/blackKnight"
  },
  {
    componentName = "ProjComponent",
    model = "res/models/projectiles/fireball.obj",
    colorR = 0.5,
    colorG = 1.0,
    colorB = 1.0,
    speed = 1.0,
    range = 100,
    height = 8.0,
    delay = 96
  }
}
function blackKnight_Hit(e,hits)
  e:setHP(e:getHP()-1)
  e:setColor(1.0,1.0,1.0,1.0)
end
function blackKnight_Update(e)
  e:moveForward(0.25)
  e:setColor(0.0,0.0,0.0,1.0)
  if (e:getHP() <= 0) then
    e:kill()
  end
  e:lookAtPlayer()
end
function blackKnight_Start(e)
  e:setScale(1.0)
  e:setAnimation(0)
  e:setHP(20)
end
