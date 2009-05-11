/*
 * =====================================================================================
 *
 *       Filename:  CSCDCSDataHandler.h
 *
 *    Description:  CSCDCSData Cond Object PopCon application
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

#ifndef CSCDCSDataHANDLER_H
#define CSCDCSDataHANDLER_H

#include "CondCore/PopCon/interface/PopConSourceHandler.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "CondTools/CSC/interface/CSCDQM_DCSDataWorker.h"

namespace popcon {

  class CSCDCSDataHandler : public popcon::PopConSourceHandler<cscdqm::DCSData> {

    public:

      CSCDCSDataHandler(const edm::ParameterSet& pset) {
        name = pset.getUntrackedParameter<std::string>("name", "CSCDCSDataHandler");
        dbConnect = pset.getUntrackedParameter<std::string>("db_connect", "");
        authPath = pset.getUntrackedParameter<std::string>("db_auth", "");
        logLevel = (cond::MessageLevel) pset.getUntrackedParameter<unsigned int>("log_level", cond::Info);
      }

      ~CSCDCSDataHandler() {
      }

      std::string id() const { return name; }
      void getNewObjects();

    private:

      std::string name;
      cond::MessageLevel logLevel;
      std::string dbConnect;
      std::string authPath;

    };

}

#endif

