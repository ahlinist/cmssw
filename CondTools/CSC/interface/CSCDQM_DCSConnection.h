/*
 * =====================================================================================
 *
 *       Filename:  CSCDQM_DCSConnection.h
 *
 *    Description:  CSCDQM DCS Database Connection Object
 *
 *        Version:  1.0
 *        Created:  04/16/2009 04:08:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifndef CSCDQM_DCSConnection_h
#define CSCDQM_DCSConnection_h

#include <iostream>
#include <string>

#include <FWCore/MessageLogger/interface/MessageLogger.h>

#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondCore/DBCommon/interface/Connection.h"

#include "RelationalAccess/ISession.h"
#include "RelationalAccess/ITable.h"
#include "RelationalAccess/IQuery.h"
#include "RelationalAccess/ISchema.h"
#include "RelationalAccess/ICursor.h"
#include "RelationalAccess/IRelationalDomain.h"
#include "RelationalAccess/RelationalServiceException.h"

#include "CoralKernel/Context.h"
#include "CoralBase/MessageStream.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/Attribute.h"

#include "CondFormats/CSCObjects/interface/CSCDQM_DCSBase.h"

namespace cscdqm {

  /**
   * @brief  Update Time values in the object provided
   * @param  schema Database connection schema object
   * @param  table_name Table name to query
   * @param  t Object to update
   * @param  prev_iov Previous payload IOV
   */
  template <typename T>
  void updateTimes(coral::ISchema& schema, const std::string table_name, T* t, const TimeType prev_iov) {
    coral::IQuery* q = 0;
    try {
      q = schema.newQuery();
      q->addToTableList(table_name);
      q->defineOutputType("CHANGE_UTIME", "long long");
      coral::ICursor& c = q->execute();
      while (c.next()) {
        const coral::AttributeList& row = c.currentRow();
        const coral::Attribute& cell = row[0];
        TimeType tm = cell.data<TimeType>();
        if (tm > t->last_change) {
          t->last_change = tm;
        }
        if (t->iov == 0 && tm > prev_iov) {
          t->iov = tm;
        }
      }
    } catch (std::exception& e) {
      edm::LogError("cscdqm::DCSConnection") << "Standard Exception while retrieving " << table_name << " data:" << e.what();
    }
    if (q) delete q;
  }

  /**
   * @brief  Get mode value from the table
   * @param  schema Database connection schema object
   * @param  table_name Table name to query
   * @param  type_name name of the type to retrieve
   * @param  v value to write mode to
   */
  template <typename T>
  void getMode(coral::ISchema& schema, const std::string table_name, const std::string type_name, T& v) {
    coral::IQuery* q = 0;
    try {
      q = schema.newQuery();
      q->addToTableList(table_name);
      q->defineOutputType("MODE_VALUE", type_name);
      coral::ICursor& c = q->execute();
      if (c.next()) {
        const coral::AttributeList& row = c.currentRow();
        const coral::Attribute& cell = row[0];
        v = cell.data<T>();
      }
    } catch (std::exception& e) {
      edm::LogError("cscdqm::DCSConnection") << "Standard Exception while retrieving " << table_name << " data:" << e.what();
    }
    if (q) delete q;
  }

  /**
   * @class DCSConnection
   * @brief Base class that provides connection to the database routines and other utilities
   */
  class DCSConnection {

    public:

      DCSConnection(const std::string& connectionString, const std::string& authPath, const cond::MessageLevel level = cond::Info);
      virtual ~DCSConnection();

    protected:

      DCSAddressType getDCSAddress(const coral::AttributeList& row) const {
        return getDCSAddress(row["ENDCAP"].data<std::string>(), row["RING"].data<unsigned int>(), row["PH_NUM"].data<unsigned int>());
      }

      cond::DBSession    *dbSession;
      cond::Connection   *dbCon;

    private:

      DCSAddressType getDCSAddress(const std::string endcap, const unsigned int ring, const unsigned int ph_num) const {
        DCSAddressType adr;
        adr.iendcap = 0;
        if (endcap.compare("P") == 0) { adr.iendcap = 1; } 
        else if (endcap.compare("P") == 0) { adr.iendcap = 2; }
        adr.iring = adr.istation = 0;
        if (ring > 9 && ring < 100) {
          adr.iring  = ring % 10;
          adr.istation = (ring - adr.iring) / 10;
        }
        adr.ichamber = ph_num;
        return adr;
      }

  };

}

#endif

