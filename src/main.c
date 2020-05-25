#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "dbg.h"

#include "core/app.h"
#include "core/osc_server.h"
#include "core/control_message.h"

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

  AppState *app = app_state_create(L);
  osc_start_server(app);

  while (app->running) {
    ControlMessage *new_control_message = NULL;

    while (
      ck_ring_dequeue_spsc(
        app->osc_control_bus,
        app->osc_control_bus_buffer,
        &new_control_message
      ) == true
    ) {
      printf("Running code %s\n", new_control_message->code);
      error = luaL_loadstring(L, new_control_message->code) || lua_pcall(L, 0, 0, 0);
      if (error) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
      }
      cm_destroy(new_control_message);
    }
    run_sequencer(L);

    sleep(5);
  }

  lua_close(L);
  return 0;
}
