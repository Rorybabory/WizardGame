#ifndef SCRIPT_H
#define SCRIPT_H
#include "luaLibrary.h"

class Script {
public:
  void print_error(lua_State* state) {
    const char* message = lua_tostring(state, -1);
    puts(message);
    lua_pop(state,1);
  }
  void execute(const char* filename){
    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    int result;
    if (result != LUA_OK) {
      print_error(state);
      return;
    }
    result = lua_pcall(state,0,LUA_MULTRET,0);
    if (result != LUA_OK) {
      print_error(state);
      return;
    }
  }
  Script() {
    puts("Usage: runlua file(s)");
    puts("Loads and executes Lua programs.");
    execute("./res/hello.lua");

  }
protected:
private:
};

#endif
