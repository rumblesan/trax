#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <lua/lua.h>

#include "ck_ring.h"
#include "portmidi.h"

typedef struct AppState {

  volatile bool running;

  lua_State *lua;

  ck_ring_buffer_t *osc_control_bus_buffer;
  ck_ring_t *osc_control_bus;

} AppState;

AppState *app_state_create(lua_State *L);

void app_state_destroy(AppState *as);
