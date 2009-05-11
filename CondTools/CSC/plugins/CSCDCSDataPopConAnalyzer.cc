/*
 * =====================================================================================
 *
 *       Filename:  CSCDCSDataPopConAnalyzer.cc
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/16/2009 05:13:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Valdas Rapsevicius (VR), valdas.rapsevicius@cern.ch
 *        Company:  CERN, CH
 *
 * =====================================================================================
 */

#include "CondCore/PopCon/interface/PopConAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CondTools/CSC/interface/CSCDCSDataHandler.h"

typedef popcon::PopConAnalyzer<popcon::CSCDCSDataHandler> CSCDCSDataPopConAnalyzer;
DEFINE_FWK_MODULE(CSCDCSDataPopConAnalyzer);

