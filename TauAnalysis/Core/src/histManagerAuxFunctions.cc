#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

void fillLeptonIsoDepositHistograms(const pat::IsoDeposit* isoDeposit, 
				    MonitorElement* isoDepositValProfile,
				    MonitorElement* isoDepositEtaDistProfile, 
				    MonitorElement* isoDepositPhiDistProfile)
{
  if ( isoDeposit ) {
    for ( pat::IsoDeposit::const_iterator it = isoDeposit->begin();
	  it != isoDeposit->end(); ++it ) {
      isoDepositValProfile->Fill(it->value());
      isoDepositEtaDistProfile->Fill(TMath::Abs(it->eta() - isoDeposit->eta()));
      isoDepositEtaDistProfile->Fill(TMath::Abs(it->phi() - isoDeposit->phi()));
    }
  }
}
