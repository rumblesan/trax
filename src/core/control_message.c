#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "dbg.h"

#include "core/control_message.h"

ControlMessage *cm_create(const char *code) {
  ControlMessage *cm = malloc(sizeof(ControlMessage));
  check_mem(cm);
  cm->code = code;

  return cm;
error:
  return NULL;
}

void cm_destroy(ControlMessage *cm) {
  check(cm->code != NULL, "Invalid Control Message Code");
  free((void *)cm->code);
  check(cm != NULL, "Invalid Control Message");
  free(cm);
  return;
error:
  log_err("Could not clean up Control Message");
}
