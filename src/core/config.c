#include <stdlib.h>

#include <libconfig.h>

#include "dbg.h"

#include "core/config.h"

TraxCfg *cfg_read(char *config_path) {
  log_info("Loading config from %s", config_path);

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
