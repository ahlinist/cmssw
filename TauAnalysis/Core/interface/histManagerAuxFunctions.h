#ifndef TauAnalysis_Core_histManagerAuxFunctions_h
#define TauAnalysis_Core_histManagerAuxFunctions_h

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/PatCandidates/interface/Isolation.h"

void fillLeptonIsoDepositHistograms(const pat::IsoDeposit*, MonitorElement*, MonitorElement*, MonitorElement*);

void clearIsoParam(reco::isodeposit::AbsVetos&);

#endif
