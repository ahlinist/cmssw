#ifndef CSCRenderPlugin_ChamberMap_H
#define CSCRenderPlugin_ChamberMap_H

/*
 * =====================================================================================
 *
 *       Filename:  CSCRenderPlugin_ChamberMap.h
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

#include <math.h>
#include <string>
#include <iostream>
#include <bitset>
#include <TH1.h>
#include <TH2.h>
#include <TBox.h>
#include <TText.h>
#include <TPRegexp.h>
#include <TStyle.h>
#include <TCanvas.h>

/**
 * @class ChamberMap
 * @brief Class that draws CSC Map diagram
 */
class ChamberMap
{
public:
  ChamberMap();
  ~ChamberMap();
  void draw(TH2*& me) const;
  void drawStats(TH2*& me) const;

private:
  void printLegendBox(unsigned int& number, const std::string title, const int color) const;
  const float Xmin_local_derived_from_ChamberID(const int side, const int station, const int ring, const int chamber) const;
  const float Xmax_local_derived_from_ChamberID(const int side, const int station, const int ring, const int chamber) const;
  const float Ymin_local_derived_from_ChamberID(const int side, const int station, const int ring, const int chamber) const;
  const float Ymax_local_derived_from_ChamberID(const int side, const int station, const int ring, const int chamber) const;
  const int N_ring(const int station) const;
  const int N_chamber(const int station, const int ring) const;
  TBox *bBlank;
};

#endif
