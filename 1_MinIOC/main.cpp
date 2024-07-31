#include <epicsExit.h>
#include <iocsh.h>
#include <pvxs/log.h>

using namespace std;
using namespace pvxs;

DEFINE_LOGGER(MinIOCLog, "MinIOC");

int main(int argc, char* argv[])
{
  iocsh("iocsh.txt");

  logger_config_env();
  log_info_printf(MinIOCLog, "Started %s 1\n", argv[0]);

  int ret = iocsh(0);

  epicsExit(ret);
  return ret;
}
