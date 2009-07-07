#ifndef CSCRenderPlugin_SummaryMap_H
#define CSCRenderPlugin_SummaryMap_H

/*
 * =====================================================================================
 *
 *       Filename:  CSCRenderPlugin_SummaryMap.h
 *
 *    Description:  CSC Histogram Rendering Plugin
 *
 *        Version:  1.0
 *        Created:  05/06/2008 03:50:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), Valdas.Rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#ifdef DQMLOCAL
#include "DQM/CSCMonitorModule/interface/CSCDQM_Detector.h"
#else
#include "CSCDQM_Detector.h"
#endif 

class TH2;
class TH2F;
class TBox;

/**
 * @class SummaryMap
 * @brief Class that draws CSC Summary Map diagram
 */
class SummaryMap
{
public:
  SummaryMap();
  ~SummaryMap();
  void drawDetector(TH2* me);
  void drawStation(TH2* me, const int station);

private:
  cscdqm::Detector detector;
  TH2F *h1;
  TBox *bBlank;
  TBox *bEmptyMinus;
  TBox *bEmptyPlus;
};

#endif
