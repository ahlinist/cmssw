
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>

#include <TMath.h>

double square(double x)
{
  return x*x;
}

void compZtautauXsectionNumbers()
{
//-------------------------------------------------------------------------------
// define number of observed events
//-------------------------------------------------------------------------------

  double numEventsObserved = 556;

//-------------------------------------------------------------------------------
// define number of events expected from background proceses
//-------------------------------------------------------------------------------

  double numEventsExpected_Bgr_template = 226.5;
  double errEventsExpected_Bgr_template =  33.1;
  double numEventsExpected_Bgr_fr       = 236.1;
  double errEventsExpected_Bgr_frUp     =  24.1;
  double errEventsExpected_Bgr_frDown   =  65.9;

  double weight_Bgr_template            = 1./square(errEventsExpected_Bgr_template);
  double weight_Bgr_fr                  = 2./(square(errEventsExpected_Bgr_frUp) + square(errEventsExpected_Bgr_frDown));

  double numEventsExpected_Bgr          = 
    (weight_Bgr_template*numEventsExpected_Bgr_template + weight_Bgr_fr*numEventsExpected_Bgr_fr)/
    (weight_Bgr_template + weight_Bgr_fr);
  double errEventsExpected_BgrUp        = 
    TMath::Sqrt(1./(square(1./errEventsExpected_Bgr_template) + square(1./errEventsExpected_Bgr_frUp)));
  double errEventsExpected_BgrDown      = 
    TMath::Sqrt(1./(square(1./errEventsExpected_Bgr_template) + square(1./errEventsExpected_Bgr_frDown)));
  
  std::cout << "Nbgr = " << numEventsExpected_Bgr 
	    << " + " << errEventsExpected_BgrUp << " (" << errEventsExpected_BgrUp/numEventsExpected_Bgr*100. << "%)"  
	    << " - " << errEventsExpected_BgrDown << " (" << errEventsExpected_BgrDown/numEventsExpected_Bgr*100. << "%)"
	    << std::endl;

  std::cout << "Nsig = " << (numEventsObserved - numEventsExpected_Bgr)
	    << " + " << TMath::Sqrt(numEventsObserved + square(errEventsExpected_BgrDown)) 
	    << " - " << TMath::Sqrt(numEventsObserved + square(errEventsExpected_BgrUp)) << std::endl;

//-------------------------------------------------------------------------------
// define signal efficiency and acceptance
// (average of values obtained by PYTHIA (D6T tune) and POWHEG (Z2 tune) generators)
//-------------------------------------------------------------------------------

  double mcAcceptance_Sig = 0.0681;
  double mcEfficiency_Sig = 0.3613;

  double brToMuTau_Sig    = 0.2250;

//-------------------------------------------------------------------------------
// define integrated luminosity of analyzed dataset
//-------------------------------------------------------------------------------

  double intLumiData = 36.1;

//-------------------------------------------------------------------------------
// define Data to Monte Carlo correction factors
//-------------------------------------------------------------------------------

  double corrTriggerEff = 0.981;
  double corrMuonIdEff  = 1.001;
  double corrMuonIsoEff = 0.984;

//-------------------------------------------------------------------------------
// define systematic uncertainties
//
// NOTE: all uncertainties are **relative** to central value
//
//-------------------------------------------------------------------------------

  double errTriggerEff   = 0.0060/corrTriggerEff;
  double errMuonIdEff    = 0.0010/corrMuonIdEff;
  double errMuonIsoEff   = 0.0060/corrMuonIsoEff;
  double errMuonMomScale = 0.;
  double errTauIdEff     = 0.2340;
  double errTauEnScale   = 0.0480;
  double errJetEnScale   = 0.0100;
  double errZrecoilCorr  = 0.0100;
  double errIntLumiData  = 0.1100; 
  double errAccISR       = 0.0380;
  double errAccFSR       = 0.0010; 
  double errAccPDF       = 0.0340;
  double errAccGenerator = 0.0737; // difference in efficiency*acceptance between PYTHIA and POWHEG

//-------------------------------------------------------------------------------
// compute Z --> tau+ tau- cross-section "central value" and uncertainties
//-------------------------------------------------------------------------------

  double denominator = (intLumiData*brToMuTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig);

  double Xsection = (numEventsObserved - numEventsExpected_Bgr)/denominator;
  std::cout << "Xsection = " << Xsection << std::endl;

  double statUncertainty = TMath::Sqrt(numEventsObserved)/denominator;
  std::cout << "statUncertainty = " << statUncertainty << std::endl;

  std::cout << "rel. effect of Nbgr uncertainty:" 
	    << " '+' = " << errEventsExpected_BgrDown/(numEventsObserved - numEventsExpected_Bgr) << "," 
	    << " '-' = " << errEventsExpected_BgrUp/(numEventsObserved - numEventsExpected_Bgr) << std::endl;

  double sysUncertaintyExclNbgrRel2 = 
    square(errTriggerEff) 
   + square(errMuonIdEff)
   + square(errMuonIsoEff)
   + square(errMuonMomScale)
   + square(errTauIdEff)
   + square(errTauEnScale)
   + square(errJetEnScale)
   + square(errZrecoilCorr)
   + square(errAccISR)
   + square(errAccFSR)
   + square(errAccPDF)
   + square(errAccGenerator);
  double sysUncertaintyRelUp2 = 
    sysUncertaintyExclNbgrRel2
   + square(errEventsExpected_BgrDown/(numEventsObserved - numEventsExpected_Bgr));
  double sysUncertaintyRelDown2 = 
    sysUncertaintyExclNbgrRel2
   + square(errEventsExpected_BgrUp/(numEventsObserved - numEventsExpected_Bgr));
  std::cout << "sysUncertainty:" 
	    << " '+' = " << TMath::Sqrt(sysUncertaintyRelUp2)*Xsection << ","
	    << " '-' = " << TMath::Sqrt(sysUncertaintyRelDown2)*Xsection << std::endl;

  std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << std::endl;

//-------------------------------------------------------------------------------
// compute Z --> tau+ tau-/Z --> mu+ mu- cross-section ratio
//-------------------------------------------------------------------------------

  double XsectionZmumuMgt60Mlt120 = 882;
  double statUncertaintyZmumuUp   = 77;
  double statUncertaintyZmumuDown = 73;
  double sysUncertaintyZmumuUp    = 42;
  double sysUncertaintyZmumuDown  = 36;
  
  double mcAcceptance_SigPOWHEG = 0.0720;
  double mcEfficiency_SigPOWHEG = 0.3746;

//--- Z --> mu+ mu- cross-section is measured using acceptance value obtained from POWHEG
//   --> recompute Z --> tau+ cross-section using POWHEG estimates for signal acceptance and efficiency
//   --> no need to add difference between PYTHIA and POWHEG to systematic uncertainties
  double XsectionRatioMgt60Mlt120 = 0.5834;
  double XsectionZtautauMgt60Mlt120 = Xsection*XsectionRatioMgt60Mlt120
                                     *(mcAcceptance_Sig*mcEfficiency_Sig)/(mcAcceptance_SigPOWHEG*mcEfficiency_SigPOWHEG);

  double XsectionRatio = XsectionZtautauMgt60Mlt120/XsectionZmumuMgt60Mlt120;
  double statUncertaintyRatioUp   = 
    XsectionRatio*TMath::Sqrt(square(statUncertainty/Xsection) + square(statUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double statUncertaintyRatioDown = 
    XsectionRatio*TMath::Sqrt(square(statUncertainty/Xsection) + square(statUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRelUpExclAccGenerator2   = sysUncertaintyRelUp2 - square(errAccGenerator);
  double sysUncertaintyRelDownExclAccGenerator2 = sysUncertaintyRelUp2 - square(errAccGenerator);
  double sysUncertaintyRatioUp   = 
    XsectionRatio*TMath::Sqrt(sysUncertaintyRelUpExclAccGenerator2 + square(sysUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRatioDown = 
    XsectionRatio*TMath::Sqrt(sysUncertaintyRelDownExclAccGenerator2 + square(sysUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));

  double sysUncertaintyRatioExclTauIdEffUp =
    XsectionRatio*TMath::Sqrt(sysUncertaintyRelUpExclAccGenerator2 - square(errTauIdEff) 
                             + square(sysUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRatioExclTauIdEffDown =
    XsectionRatio*TMath::Sqrt(sysUncertaintyRelDownExclAccGenerator2 - square(errTauIdEff) 
		             + square(sysUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));
  
  std::cout << "Z --> tau+ tau-/Z --> mu+ mu- Xsection ratio = " << XsectionRatio 
	    << " + " << statUncertaintyRatioUp << " - " << statUncertaintyRatioDown << " (stat.)"
	    << " + " << sysUncertaintyRatioUp << " - " << sysUncertaintyRatioDown << " (sys.)" << std::endl;
  std::cout << "(+ " << sysUncertaintyRatioExclTauIdEffUp 
	    << " - " << sysUncertaintyRatioExclTauIdEffDown << " sys. excl. errTauIdEff)" << std::endl;
}
