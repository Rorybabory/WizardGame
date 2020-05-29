cd CPP/OPENGL
g++ -std=c++1z main.cpp display.cpp mesh.cpp obj_loader.cpp object.cpp player.cpp shader.cpp stb_image.c texture.cpp world.cpp enemy.cpp SkeletalAnimation/SkeletalModel.cpp AssimpConverter.cpp animatedMesh.cpp luaLibrary.cpp TextRenderer.cpp -lGL -lGLU -lGLEW -lSDL2 -lassimp -llua5.2 -lpthread -g -lxml2 -lqu3e -lglfreetype_lib -L lib/ -lstdc++fs `freetype-config --libs`
./a.out
