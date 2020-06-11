#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include <lua/lua.h>

#include "ck_ring.h"

#include "dbg.h"

#include "core/app.h"

AppState *app_state_create(lua_State *L) {

  AppState *as = calloc(1, sizeof(AppState));
  check_mem(as);

  as->running = true;

  check_mem(L);
  as->lua = L;

  uint16_t control_bus_size = 1024;

  ck_ring_init(&as->osc_control_bus, control_bus_size);
  as->osc_control_bus_buffer = calloc(1, sizeof(ck_ring_buffer_t) * control_bus_size);
  check_mem(as->osc_control_bus_buffer);

  as->p_time = 0.0;

  return as;
error:
  return NULL;
}

double app_set_time(AppState *as) {
  struct timeval tv;
  gettimeofday(&tv, NULL);

  double ms = ((double)tv.tv_sec * 1000.0) + ((double)tv.tv_usec / 1000.0);
  as->p_time = ms;
  return ms;
}

void app_state_destroy(AppState *as) {
  check(as != NULL, "Invalid App State");

  // FIXME cleanup everything properlly
  free(as);
  return;
error:
  log_err("Could not clean up App State");
}
