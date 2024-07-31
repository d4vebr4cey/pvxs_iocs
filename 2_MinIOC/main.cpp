#include <epicsExit.h>
#include <iocsh.h>
#include <pvxs/log.h>
#include <pvxs/nt.h>
#include <pvxs/sharedpv.h>
#include <pvxs/server.h>
#include <pvxs/iochooks.h>

using namespace std;
using namespace pvxs;

DEFINE_LOGGER(MinIOCLog, "MinIOC");

int main(int argc, char* argv[])
{
  iocsh("iocsh.txt");
  logger_config_env();
  log_info_printf(MinIOCLog, "Started %s 2\n", argv[0]);

  Value val = nt::NTScalar(TypeCode::Int32, true).create();

  server::SharedPV pv = server::SharedPV::buildMailbox();

  pv.open(val);

  string pvname = "MinIOC:Int32";

  ioc::server().addPV(pvname, pv);

  log_info_printf(MinIOCLog, "Added pv %s\n", pvname.c_str());

  int ret = iocsh(0);
  epicsExit(ret);
  return ret;
}
