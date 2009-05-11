/*
 * =====================================================================================
 *
 *       Filename:  CSCDQM_DCSConnection.cc
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/16/2009 04:12:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "CondTools/CSC/interface/CSCDQM_DCSConnection.h"

namespace cscdqm {

  DCSConnection::DCSConnection(const std::string& connectionString, const std::string& authPath, const cond::MessageLevel level) {
    dbSession = new cond::DBSession;
    dbSession->configuration().setMessageLevel(level);
    dbSession->configuration().setAuthenticationMethod(cond::XML);
    dbSession->configuration().setAuthenticationPath(authPath);
    dbCon = new cond::Connection(connectionString, -1);
    dbSession->open();
    dbCon->connect(dbSession);
  }

  DCSConnection::~DCSConnection() {
    if (dbCon) {
      dbCon->disconnect();
      delete dbCon;
    }
    if (dbSession) {
      delete dbSession;
    }
  }

}
