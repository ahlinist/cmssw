// $Id: ESCondDBInterface.cc,v 1.1 2009/09/30 09:52:51 gcheveni Exp $

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


int ESCondDBInterface::query_side(const int num)  throw(std::runtime_error)
{
  int side=0;

  try {
    stmt->setSQL("select sum(b.logic_id) as Side from CMS_ES_COND.RUN_CONFIG_DAT A ,CMS_ES_COND.RUN_DAT B ,CMS_ES_COND.RUN_IOV C where A.config_tag like :1 and B.num_events >0 and  c.run_num=:2   and c.iov_id=b.iov_id  and c.iov_id=a.iov_id group by c.iov_id having count(B.Logic_id)=:3");
    stmt->setString(1,"PEDESTAL-%G-TTC");
    stmt->setInt(2,num);
    stmt->setInt(3,20);
    ResultSet* rset = stmt->executeQuery();
    rset->next();
    side=rset->getInt(1);
  } catch (SQLException &e) {
    throw(runtime_error("ESCondDBInterface::query_side()  "+e.getMessage()));
  }
 return  side ;
}


string ESCondDBInterface::query_location(const int num)  throw(std::runtime_error)
{
  string loc="";

  try {
    stmt->setSQL("select b.location  from CMS_ES_COND.RUN_TAG A inner join CMS_ES_COND.location_def B on b.def_id=a.location_id inner join CMS_ES_COND.RUN_IOV C on c.tag_id=a.tag_id where c.run_num=:1");
    stmt->setInt(1,num);
    ResultSet* rset = stmt->executeQuery();
    rset->next();
    loc=rset->getString(1);
  } catch (SQLException &e) {
    throw(runtime_error("ESCondDBInterface::query_location()  "+e.getMessage()));
  }
 return  loc ;
}
