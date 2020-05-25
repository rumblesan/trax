#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#include "dbg.h"

#include "core/app.h"
#include "core/osc_server.h"
#include "core/control_message.h"
#include "core/config.h"

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

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

int main(int argc, char *argv[]) {
  int exitValue = EXIT_SUCCESS;

  int error;

  TraxCfg *cfg = NULL;
  OSCServer osc_server = NULL;
  AppState *app = NULL;


  if (argc < 2) {
    cfg = cfg_read("./config.cfg");
  } else {
    char *config_path = argv[1];
    cfg = cfg_read(config_path);
  }
  check(cfg != NULL, "Could not read config file");

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  luaL_requiref(L, "trax", luaopen_trax, 1);
  lua_pop(L, 1);

  if (luaL_dofile(L, cfg->startup_code_path)) {
    printf("Could not initialisation lua code");
    lua_close(L);
    return 1;
  }

  app = app_state_create(L);
  osc_server = osc_start_server(app);

  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = cfg->ms_sleep * NANO_SECOND_MULTIPLIER;

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

    nanosleep(&tim, &tim2);
  }

  lua_close(L);

  goto cleanup;

error:
  printf("Exiting Trax with failure\n");
  exitValue = EXIT_FAILURE;

cleanup:
  if (osc_server != NULL) osc_stop_server(osc_server);
  if (app != NULL) app_state_destroy(app);
  if (cfg != NULL) cfg_destroy(cfg);

  return exitValue;
}
