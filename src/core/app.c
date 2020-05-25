#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include <lua/lua.h>

#include "ck_ring.h"

#include "dbg.h"

#include "core/app.h"

bool create_ring_buffer(ck_ring_buffer_t **buffer, ck_ring_t **bus, uint16_t size) {
  *bus = malloc(sizeof(ck_ring_t));
  check_mem(*bus);
  *buffer = malloc(sizeof(ck_ring_buffer_t) * size);
  check_mem(*buffer);
  ck_ring_init(*bus, size);
  return true;
error:
  return false;
}

AppState *app_state_create(lua_State *L) {

  AppState *as = malloc(sizeof(AppState));
  check_mem(as);

  as->running = true;

  check_mem(L);
  as->lua = L;

  uint16_t control_bus_size = 1024;

  check(
    create_ring_buffer(&as->osc_control_bus_buffer, &as->osc_control_bus, control_bus_size),
    "Could not create control bus ring"
  );

  return as;
error:
  return NULL;
}

void app_state_destroy(AppState *as) {
  check(as != NULL, "Invalid App State");

  // FIXME cleanup everything properlly
  free(as);
  return;
error:
  log_err("Could not clean up App State");
}
