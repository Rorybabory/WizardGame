spiderEgg = {
  {
    componentName = "StaticGraphicsComponent",
    folder = "./res/models/spiderEgg.obj",
    color = {
      r = 0.0,
      g = 0.0,
      b = 0.0,
      a = 1.0
    },
    size = 5.0,
    canMove = false
  },
  {
    componentName = "CollisionComponent",
    scale = {
      x = 4.0,
      y = 4.0,
      z = 4.0
    }
  },
}


function spiderEgg_Update(e)
  if (e:getHP() <= 0) then
    e:kill()
  end
  if (e:getFloat("SpawnCount") == 100) then
    e:spawnEntity("smallSpider", e:getX(), e:getZ())
    e:setFloat("SpawnCount",0)
  end
  e:setFloat("SpawnCount", e:getFloat("SpawnCount")+1)
end
function spiderEgg_Start(e)
  e:setFloat("SpawnCount", 0)
  e:setHP(20)
end
