#ifndef L1Scales_L1JetEtScaleRcd_h
#define L1Scales_L1JetEtScaleRcd_h
// -*- C++ -*-
//
// Package:     L1Scales
// Class  :     L1JetEtScaleRcd
// 
/**\class L1JetEtScaleRcd L1JetEtScaleRcd.h L1Trigger/L1Scales/interface/L1JetEtScaleRcd.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      
// Created:     Wed Oct  4 16:49:43 CEST 2006
// $Id$
//

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"

class L1JetEtScale;

class L1JetEtScaleRcd : public edm::eventsetup::EventSetupRecordImplementation<L1JetEtScale> {};

#endif
