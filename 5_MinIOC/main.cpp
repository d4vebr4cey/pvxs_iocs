#include <epicsExit.h>
#include <epicsTime.h>
#include <iocsh.h>
#include <pvxs/log.h>
#include <pvxs/nt.h>
#include <pvxs/sharedpv.h>
#include <pvxs/server.h>
#include <pvxs/iochooks.h>

using namespace std;
using namespace pvxs;

DEFINE_LOGGER(MinIOCLog, "MinIOC");

static bool set_val_time(Value& val)
{
  epicsTimeStamp now = { 0 };
  if (epicsTimeGetCurrent(&now) == epicsTimeOK)
  {
    val["timeStamp"]["secondsPastEpoch"] = now.secPastEpoch + POSIX_TIME_AT_EPICS_EPOCH;
    val["timeStamp"]["nanoseconds"] = now.nsec;
  }
  return now.secPastEpoch;
}

int main(int argc, char* argv[])
{
  iocsh("iocsh.txt");
  logger_config_env();
  log_info_printf(MinIOCLog, "Started %s 5\n", argv[0]);

  Value val = nt::NTScalar(TypeCode::Int32, true).create();
  set_val_time(val);
  val["value"] = 1;
  val["display"]["description"] = "Test 32 bit signed integer";
  log_info_printf(MinIOCLog, "Initial value %i\n", val["value"].as<int32_t>());

  server::SharedPV pv = server::SharedPV::buildMailbox();
  pv.open(val);

  pv.onPut([](server::SharedPV& pv, unique_ptr<server::ExecOp>&& op, Value&& val)
    {
      log_info_printf(MinIOCLog, "Put %s %i\n", op->name().c_str(), val["value"].as<int32_t>());
      set_val_time(val);
      pv.post(val);
      op->reply();
    }
  );

  string pvname = "MinIOC:Int32";
  ioc::server().addPV(pvname, pv);
  log_info_printf(MinIOCLog, "Added pv %s\n", pvname.c_str());
  int ret = iocsh(0);
  epicsExit(ret);
  return ret;
}
