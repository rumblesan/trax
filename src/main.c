#include <stdlib.h>
#include <stdio.h>

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "dbg.h"

int simpletest(lua_State *L) {
  lua_pushinteger(L, 1337);
  return 1;
}

const struct luaL_Reg traxlib [] = {
  {"test", simpletest},
  {NULL, NULL}
};

int luaopen_trax(lua_State *L) {
  luaL_newlib(L, traxlib);
  return 1;
}


int run_sequencer(lua_State *L) {
  int error = luaL_loadstring(L, "run()") || lua_pcall(L, 0, 0, 0);
  if (error) {
    return luaL_error(L, "should have run sequencer");
  }
  return 0;
}

int main(void) {
  char buff[256];
  int error;

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luaL_requiref(L, "trax", luaopen_trax, 1);
  lua_pop(L, 1);

  if (luaL_dofile(L, "./assets/lua/trax.lua")) {
    printf("Could not load trax");
    lua_close(L);
    return 1;
  }

  while (fgets(buff, sizeof(buff), stdin) != NULL) {
    error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_pop(L, 1);
    }

    run_sequencer(L);

  }

  lua_close(L);
  return 0;
}
