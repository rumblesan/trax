#pragma once

#include <stdbool.h>

#include <libconfig.h>

typedef struct {

  config_t *cfg;

  const char *startup_code_path;

  double ms_sleep;

} TraxCfg;

TraxCfg *cfg_read(char *config_path);

void cfg_destroy(TraxCfg *cfg);
