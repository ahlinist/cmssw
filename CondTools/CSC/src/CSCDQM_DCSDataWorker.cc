/*
 * =====================================================================================
 *
 *       Filename:  CSCDCS_DCSDataWorker.cc
 *
 *    Description:  CSCDCS DCS Data worker implementation
 *
 *        Version:  1.0
 *        Created:  05/04/2009 12:53:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "CondTools/CSC/interface/CSCDQM_DCSDataWorker.h"

#define SET_Q_OUTPUT_FORMAT \
      q->defineOutputType("ENDCAP", "string"); \
      q->defineOutputType("RING", "unsigned int"); \
      q->defineOutputType("PH_NUM", "unsigned int"); \

namespace cscdqm {

  bool DCSDataWorker::get(DCSData* data, const TimeType prev_iov, const TimeType prev_last_change) {

    cond::CoralTransaction& trans = dbCon->coralTransaction();
    trans.start(true);
    coral::ISchema& schema = trans.nominalSchema();
    
    updateTimes(schema, "DCS_TEMP_TIME", data, prev_iov);
    updateTimes(schema, "DCS_HV_V_TIME", data, prev_iov);
    updateTimes(schema, "DCS_LV_V_TIME", data, prev_iov);
    updateTimes(schema, "DCS_LV_I_TIME", data, prev_iov);

    if (data->last_change <= prev_last_change) {
      trans.commit();
      edm::LogWarning("cscdqm::DCSConnection") << "No changes in CSC DCS wrt previous IOV. Skipping...";
      return false;
    }

    try {
      getMode(schema, "DCS_TEMP_MODE", "int", data->temp_mode);
      coral::IQuery* q = schema.newQuery();
      SET_Q_OUTPUT_FORMAT
      q->addToTableList("DCS_TEMP_EXCEPTIONS");
      q->defineOutputType("BOARD", "string");
      q->defineOutputType("BOARD_NUM", "unsigned short");
      q->defineOutputType("MVALUE", "int");
      coral::ICursor& c = q->execute();
      while (c.next()) {
        TempMeasType meas;
        const coral::AttributeList& row = c.currentRow();
        meas.adr = getDCSAddress(row);
        meas.board   = DCSBoardUtility::getDCSBoard(row["BOARD"].data<std::string>());
        meas.boardId = row["BOARD_NUM"].data<unsigned short>();
        meas.value   = row["MVALUE"].data<int>();
        data->temp_meas.push_back(meas);
      }
      delete q;
    } catch (std::exception& e) {
      edm::LogError("DCSTempWorker") << "Standard Exception while retrieving DCS_TEMP data:" << e.what();
    }

    try {
      getMode(schema, "DCS_HV_V_MODE", "int", data->hvv_mode);
      coral::IQuery* q = schema.newQuery();
      q->addToTableList("DCS_HV_V_EXCEPTIONS");
      SET_Q_OUTPUT_FORMAT
      q->defineOutputType("POSITION", "unsigned int");
      q->defineOutputType("MVALUE", "int");
      coral::ICursor& c = q->execute();
      while (c.next()) {
        HVVMeasType meas;
        const coral::AttributeList& row = c.currentRow();
        meas.adr = getDCSAddress(row);
        meas.position = (row["POSITION"].isNull() ? 0 : row["POSITION"].data<unsigned int>());
        meas.value   = row["MVALUE"].data<int>();
        data->hvv_meas.push_back(meas);
      }
      delete q;
    } catch (std::exception& e) {
      edm::LogError("DCSDataWorker") << "Standard Exception while retrieving DCS_HV_V data:" << e.what();
    }

    try {
      getMode(schema, "DCS_LV_V_MODE", "bool", data->lvv_mode);
      coral::IQuery* q = schema.newQuery();
      q->addToTableList("DCS_LV_V_EXCEPTIONS");
      SET_Q_OUTPUT_FORMAT
      q->defineOutputType("BOARD", "string");
      q->defineOutputType("BOARD_NUM", "unsigned short");
      q->defineOutputType("NOMINAL_V", "float");
      coral::ICursor& c = q->execute();
      while (c.next()) {
        LVVMeasType meas;
        const coral::AttributeList& row = c.currentRow();
        meas.adr = getDCSAddress(row);
        meas.board   = DCSBoardUtility::getDCSBoard(row["BOARD"].data<std::string>());
        meas.boardId = row["BOARD_NUM"].data<unsigned short>();
        meas.nominal_v = row["NOMINAL_V"].data<float>();
        data->lvv_meas.push_back(meas);
      }
      delete q;
    } catch (std::exception& e) {
      edm::LogError("DCSDataWorker") << "Standard Exception while retrieving DCS_LV_V data:" << e.what();
    }

    try {
      getMode(schema, "DCS_LV_I_MODE", "float", data->lvi_mode);
      coral::IQuery* q = schema.newQuery();
      q->addToTableList("DCS_LV_I_EXCEPTIONS");
      SET_Q_OUTPUT_FORMAT
      q->defineOutputType("BOARD", "string");
      q->defineOutputType("BOARD_NUM", "unsigned short");
      q->defineOutputType("NOMINAL_V", "float");
      q->defineOutputType("VALUE", "float");
      coral::ICursor& c = q->execute();
      while (c.next()) {
        LVIMeasType meas;
        const coral::AttributeList& row = c.currentRow();
        meas.adr = getDCSAddress(row);
        meas.board   = DCSBoardUtility::getDCSBoard(row["BOARD"].data<std::string>());
        meas.boardId = row["BOARD_NUM"].data<unsigned short>();
        meas.nominal_v = row["NOMINAL_V"].data<float>();
        meas.value = row["VALUE"].data<float>();
        data->lvi_meas.push_back(meas);
      }
      delete q;
    } catch (std::exception& e) {
      edm::LogError("DCSDataWorker") << "Standard Exception while retrieving DCS_LV_I data:" << e.what();
    }

    trans.commit();

    return true;
  }

}

#undef SET_Q_OUTPUT_FORMAT
