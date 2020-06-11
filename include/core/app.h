#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <lua/lua.h>

#include "ck_ring.h"

typedef struct {

  double p_time;

  volatile bool running;

  lua_State *lua;

  ck_ring_t osc_control_bus;
  ck_ring_buffer_t *osc_control_bus_buffer;

} AppState;

AppState *app_state_create(lua_State *L);

double app_set_time(AppState *as);

void app_state_destroy(AppState *as);
