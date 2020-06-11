#include <stdlib.h>

#include <libconfig.h>

#include "dbg.h"

#include "core/config.h"

#define DEFAULT_MS_SLEEP 10

TraxCfg *cfg_read(char *config_path) {
  printf("Loading config from %s\n", config_path);

  config_t *cfg = NULL;

  TraxCfg *trax_cfg = malloc(sizeof(TraxCfg));
  check_mem(trax_cfg);

  trax_cfg->cfg = malloc(sizeof(config_t));
  cfg = trax_cfg->cfg;

  config_init(cfg);

  check(config_read_file(cfg, config_path),
    "Could not read config file - %s:%d - %s",
    config_error_file(cfg), config_error_line(cfg), config_error_text(cfg));

  check(config_lookup_string(
    cfg, "startup_code_path", &trax_cfg->startup_code_path),
    "Could not read startup code filepath setting");

  double sleep_ms_interval = 0.0;
  int found = config_lookup_float(cfg, "sleep_ms_interval", &sleep_ms_interval);
  if (!found) {
    sleep_ms_interval = DEFAULT_MS_SLEEP;
  } else {
    check(sleep_ms_interval > 0.0, "Need to choose a sleep interval greater than 0");
  }
  trax_cfg->ms_sleep = sleep_ms_interval;

  return trax_cfg;
 error:
  if (cfg != NULL) config_destroy(cfg);
  if (trax_cfg != NULL) free(trax_cfg);
  return NULL;
};

void cfg_destroy(TraxCfg *tcfg) {
  check(tcfg != NULL, "Invalid config");
  if (tcfg->cfg != NULL) config_destroy(tcfg->cfg);
  free(tcfg);
  return;
error:
  log_err("Could not clean up config");
}
