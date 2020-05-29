AIHero = {
  {
    componentName = "GraphicsComponent",
    folder = "./res/zombie"
  },
  {
    componentName = "CollisionComponent",
    scale = 5.0
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
    delay = 5
  }
}
function AIHero_Hit(e,hits)
end
function AIHero_Update(e)
  if (e:doesEntityExist("test2") == true) then
    e:lookAtNearest("test2")
    if (e:getDistanceFromNearest("test2") < 100) then
      e:moveBackward(1.0)
    else
      e:moveForward(1.0)
      e:write(e:getDistanceFromNearest("test2"))
    end
  end
  
  if (e:getFloat("Fire")>2) then
    e:setFloat("Fire",0)
    -- e:Fire()
  end
  e:setFloat("Fire",e:getFloat("Fire")+1)
end
function AIHero_Start(e)
  e:setScale(0.8)
  e:setAnimation(0)
  e:setColor(1.0,0.0,0.0,1.0)
  e:setFloat("Fire", 0);
end
