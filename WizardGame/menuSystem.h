#ifndef MSYSTEM_H
#define MSYSTEM_H
#include <vector>
#include <string>
#include "object.h"
#include "text.h"
#include "luaLibrary.h"

class Button {
public:
  Button(std::string text, std::string file, glm::vec4 color1,glm::vec4 color2, int size, glm::vec2 location) : textObject(size,file) {
    this->color1 = color1;
    this->color2 = color2;
    this->text = text;
    this->location = location;
  }
  void Draw(int width, int height) {
    if (hovered == true) {
      textObject.Draw(text,location,color1,width,height);
      std::cout << "PRINTED HOVERED" << '\n';
    }else {
      textObject.Draw(text,location,color2,width,height);
    }
  }
  void setLoc(glm::vec2 location) {
    this->location = location;
  }
  void changeLocBy(glm::vec2 change) {
    location.x+=change.x;
    location.y+=change.y;
  }
  void setColor1(glm::vec4 color1) {
    this->color1=color1;
  }
  void setColor2(glm::vec4 color2) {
    this->color2=color2;
  }
  void setHovered(bool hovered) {
    this->hovered = hovered;
  }
  bool hovered = false;
  std::string text;

private:
  glm::vec4 color1;
  glm::vec4 color2;
  Text textObject;
  glm::vec2 location;
};
class Label {
public:
  Label(std::string text,std::string file, int size, glm::vec2 loc, glm::vec4 color) : textObject(size, file) {
    this->text = text;
    this->loc = loc;
    this->color = color;
  }
  void Draw(int width, int height) {
    textObject.Draw(text,loc,color,width,height);
  }
  Text textObject;
  glm::vec4 color;
  glm::vec2 loc;
  std::string text;

private:
};
class Menu {
public:
  Menu(std::string fileName) {
    luah::loadScript(L,"./res/menus/" + fileName + ".lua");
    std::cout << "MENU LOADED" << "\n";
    luabridge::LuaRef menuRef = luabridge::getGlobal(L,fileName.c_str());

    for (int i = 0; i < menuRef.length(); ++i) {
      std::cout << "MENU CREATED..." << '\n';
      luabridge::LuaRef item = menuRef[i+1];
      luabridge::LuaRef posRef = item["pos"];
      glm::vec2 pos = glm::vec2(posRef["x"],posRef["y"]);
      std::string textShown = item["text"];
      std::string font = item["font"];
      luabridge::LuaRef ColorRef = item["color"];
      glm::vec4 color = glm::vec4(ColorRef["r"],ColorRef["g"],ColorRef["b"],ColorRef["a"]);
      int size = item["size"];
      if (item["type"] == "label") {
        labels.push_back(new Label(textShown,font,size,pos,color));
      }else if (item["type"] == "button") {
        luabridge::LuaRef Color2Ref = item["color2"];
        glm::vec4 color2 = glm::vec4(ColorRef["r"],ColorRef["g"],ColorRef["b"],ColorRef["a"]);
        buttons.push_back(new Button(textShown,font,color,color2,size,pos));
      }else {
        std::cout << "ERROR: MENU: " << fileName << " HAS ITEM WITH INVALID TYPE..." << "\n";
      }
    }
  }
  void moveDown() {
    if (buttonSelected<buttons.size()-1) {
      buttonSelected--;
    }
  }
  void moveUp() {
    if (buttonSelected>0) {
      buttonSelected++;
    }
  }
  void Draw(int width, int height) {
    for (Button * b : buttons) {
      b->hovered = false;
    }
    // if (buttonSelected < buttons.size()) {
    //   buttons[buttonSelected]->hovered = true;
    // }
    for (Button * b : buttons) {
      b->Draw(width,height);
    }
    for (Label * l : labels) {
      l->Draw(width,height);
    }
  }
  std::vector<Button*> buttons;
  std::vector<Label*> labels;
  lua_State* L = luaL_newstate();

private:
  int buttonSelected = 0;
};
class MenuManager {
public:
private:
  std::vector<Menu> menus;
};

#endif
