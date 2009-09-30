// $Id: EcalCondDBInterface.cc,v 1.17 2009/09/01 10:08:32 dellaric Exp $

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <stdexcept>
#include "OnlineDB/Oracle/interface/Oracle.h"

#include "OnlineDB/EcalCondDB/interface/IDBObject.h"
#include "OnlineDB/ESCondDB/interface/ESCondDBInterface.h"
#include "OnlineDB/ESCondDB/interface/ESMonRunIOV.h"
#include "OnlineDB/EcalCondDB/interface/EcalDBConnection.h"

using namespace std;
using namespace oracle::occi;


ESMonRunIOV ESCondDBInterface::fetchESMonRunIOV(RunTag* runtag, std::string montag, run_t run, subrun_t subrun)
  throw(runtime_error)
{
  RunIOV runiov = fetchRunIOV(runtag, run);
  ESMonRunIOV moniov;
  moniov.setConnection(env, conn);
  moniov.setByRun(montag, &runiov, subrun);
  return moniov;
}


