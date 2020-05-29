#pragma once
#include <string>
#include <vector>
#include <iostream>
extern "C" {
#include <lua5.2/lua.h>
#include <lua5.2/lauxlib.h>
#include <lua5.2/lualib.h>
}

struct lua_State;
namespace luah {
  void lua_gettostack(lua_State* L, const std::string& variableName);
  bool loadScript(lua_State* L, const std::string& filename);
  void loadGetKeysFunction(lua_State* L);
  std::vector<std::string> getTableKeys(lua_State* L, const std::string& name);
}
