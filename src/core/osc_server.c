#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "ck_ring.h"
#include "lo/lo.h"

#include "dbg.h"

#include "core/osc_server.h"
#include "core/app.h"
#include "core/control_message.h"

void send_code(AppState *app, const char *code) {
  ControlMessage *cm = cm_create(code);
  if (
    ck_ring_enqueue_spsc(&(app->osc_control_bus), app->osc_control_bus_buffer, cm) == false
  ) {
    printf("Could not send message to main thread\n");
  }
}

void error(int num, const char *msg, const char *path) {
  printf("liblo server error %d in path %s: %s\n", num, path, msg);
  fflush(stdout);
}

int quit_handler(
  const char *path, const char *types, lo_arg **argv,
  int argc, void *data, void *user_data
) {
  AppState *app = (AppState*) user_data;
  app->running = false;

  fflush(stdout);

  return 0;
}

int run_code_handler(
  const char *path, const char *types, lo_arg **argv,
  int argc, void *data, void *user_data
) {
  const char *sent = &argv[0]->s;
  long sent_length = strlen(sent) + 1;
  char *code = malloc(sent_length * sizeof(char));
  strncpy(code, sent, sent_length);
  send_code(user_data, code);
  fflush(stdout);
  return 0;
}

OSCServer osc_start_server(AppState *app) {
  printf("Starting OSC server\n");

  lo_server_thread osc_server = lo_server_thread_new("7770", error);

  lo_server_thread_add_method(osc_server, "/run/code", "s", run_code_handler, app);
  lo_server_thread_add_method(osc_server, "/quit", NULL, quit_handler, app);

  lo_server_thread_start(osc_server);

  return osc_server;
}

void osc_stop_server(OSCServer osc_server) {
  lo_server_thread_free(osc_server);
}
