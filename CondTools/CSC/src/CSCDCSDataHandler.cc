/*
 * =====================================================================================
 *
 *       Filename:  CSCDCSDataHandler.cc
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

#include "CondTools/CSC/interface/CSCDCSDataHandler.h"

namespace popcon {

  void CSCDCSDataHandler::getNewObjects() {

    cscdqm::DCSData* data = new cscdqm::DCSData();
    cscdqm::TimeType prev_iov = 0, prev_last_change = 0;
    try {
      const cscdqm::DCSData prev = *lastPayload();
      prev_iov = prev.iov;
      prev_last_change = prev.last_change;
    } catch (...) { }

    cscdqm::DCSDataWorker worker(dbConnect, authPath, logLevel);
    if (worker.get(data, prev_iov, prev_last_change)) {
      //m_to_transfer.push_back(std::make_pair((cscdqm::DCSData*) data, (unsigned int) data->iov));
      m_to_transfer.push_back(std::make_pair((cscdqm::DCSData*) data, 4999999));
    } else {
      delete data;
    }

  }

}

