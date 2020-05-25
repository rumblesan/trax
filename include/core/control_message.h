#pragma once

typedef struct ControlMessage {
  const char *code;
} ControlMessage;

ControlMessage *cm_create(const char *code);

void cm_destroy(ControlMessage *cm);
