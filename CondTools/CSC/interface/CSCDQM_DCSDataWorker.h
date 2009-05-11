/*
 * =====================================================================================
 *
 *       Filename:  CSCDQM_DCSDataWorker.h
 *
 *    Description:  CSCDQM DCS HV V worker object
 *
 *        Version:  1.0
 *        Created:  05/04/2009 12:25:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifndef CSCDQM_DCSDataWORKER_H
#define CSCDQM_DCSDataWORKER_H

#include <exception>
#include "CondTools/CSC/interface/CSCDQM_DCSConnection.h"
#include "CondFormats/CSCObjects/interface/CSCDQM_DCSData.h"

namespace cscdqm {

  class DCSDataWorker : public DCSConnection {

    public:

      DCSDataWorker(const std::string& connectionString, const std::string& authPath, const cond::MessageLevel level = cond::Info) :
        DCSConnection(connectionString, authPath, level) { }

      bool get(DCSData* hvv, const TimeType prev_iov, const TimeType prev_last_change);

  };

}

#endif

