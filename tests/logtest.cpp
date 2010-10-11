#include <qm/log>

void test_empty_log_macro()
{
  log_info("===== empty macro testing =====");
  log_debug();
  log_info();
  log_warning();
  log_error();
  log_critical();
  log_info("===============================");
}

#define test_log_macro_with_fmt(fmt, ...) \
do {                                                  \
  log_info("====== fmt macro testing =====");         \
  log_debug(fmt, ## __VA_ARGS__);                     \
  log_info(fmt, ## __VA_ARGS__);                      \
  log_warning(fmt, ## __VA_ARGS__);                   \
  log_error(fmt, ## __VA_ARGS__);                     \
  log_critical(fmt, ## __VA_ARGS__);                  \
  log_info("===============================");        \
} while(0)                                            \

void do_log()
{
  test_empty_log_macro();
  test_log_macro_with_fmt("string only");
  test_log_macro_with_fmt("\"x = %d\" == \"x = 5\"", 5);

  const char * format = "\"fmt\" is in \"const char * format\"";
  test_log_macro_with_fmt(format);
}

void log_change_settings_locally_second()
{
  ADD_FILE_LOG("my-logtest-minor.log");
  log_debug("Inital level for my-logtest-minor.log shall be debug");

  SET_TEMP_LOG_SETTINGS(LOG_LEVEL_CRITICAL, LOG_MAX_LOCATION, LoggerSettings::EMessage);
  ADD_STDOUT_LOG(LOG_LEVEL_INFO);

  log_critical("==== temp minor output settings ====");
  do_log();
  log_critical("=== temp minor output settings done ===");
}

void log_change_settings_locally()
{
  SET_TEMP_LOG_SETTINGS_MAX_DEBUG();

  log_warning("===== temp max debug settings ======");
  do_log();
  log_change_settings_locally_second();
  log_warning("=== temp max debug settings done ===");
}

void log_without_init()
{
  log_warning("============== no init =============");

  do_log();
  log_change_settings_locally();

  log_warning("=========== no init done ===========");
}

void log_with_init()
{
  INIT_LOGGER("my-logtest");
  ADD_SYSLOG();
  ADD_STDERR_LOG();
  ADD_FILE_LOG("my-logtest.log");

  log_warning("=========== with init ===========");
  do_log();
  log_change_settings_locally();
  log_warning("========== with init done ==========");
}

int main(void)
{
  log_warning("============= start of logtest =============");

  log_with_init();
  log_without_init();

  log_warning("============= end of logtest ==============");
  return 0;
}
