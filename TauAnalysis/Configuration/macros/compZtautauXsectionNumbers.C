
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

  double numEventsExpected_Zmumu      = 0.;
  double numEventsExpected_WplusJets  = 0.;
  double numEventsExpected_TTplusJets = 0.;
  double numEventsExpected_QCD        = 0.;
  //double numEventsExpected_Bgr        = 
  //  numEventsExpected_Zmumu + numEventsExpected_WplusJets + numEventsExpected_TTplusJets + numEventsExpected_QCD;
  double numEventsExpected_Bgr        = 236;

//-------------------------------------------------------------------------------
// define signal efficiency and acceptance
//-------------------------------------------------------------------------------

  double mcAcceptance_Sig = 0.0681;
  double mcEfficiency_Sig = 0.3613;

  double brToMuTau_Sig    = 0.2250;

//-------------------------------------------------------------------------------
// define integrated luminosity of analyzed dataset
//-------------------------------------------------------------------------------

  double intLumiData = 36.2;

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
  double errBgrSum       = 0.1000;
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

  std::cout << "rel. effect of errBgrSum: " << errBgrSum*numEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr) << std::endl;

  double sysUncertaintyRel2 = square(errTriggerEff) 
                             + square(errMuonIdEff)
                             + square(errMuonIsoEff)
                             + square(errMuonMomScale)
                             + square(errTauIdEff)
                             + square(errTauEnScale)
                             + square(errJetEnScale)
                             + square(errZrecoilCorr)
                             + square(errBgrSum*numEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr))
                             + square(errAccISR)
                             + square(errAccFSR)
                             + square(errAccPDF)
                             + square(errAccGenerator);
  std::cout << "sysUncertainty = " << TMath::Sqrt(sysUncertaintyRel2)*Xsection << std::endl;

  std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << std::endl;

//-------------------------------------------------------------------------------
// compute Z --> tau+ tau-/Z --> mu+ mu- cross-section ratio
//-------------------------------------------------------------------------------

  double XsectionZmumuMgt60Mlt120 = 882;
  double statUncertaintyZmumuUp   = 77;
  double statUncertaintyZmumuDown = 73;
  double sysUncertaintyZmumuUp    = 42;
  double sysUncertaintyZmumuDown  = 36;
  
  double XsectionRatioMgt60Mlt120 = 0.5834;
  double XsectionZtautauMgt60Mlt120 = Xsection*XsectionRatioMgt60Mlt120;

  double XsectionRatio = XsectionZtautauMgt60Mlt120/XsectionZmumuMgt60Mlt120;
  double statUncertaintyRatioUp   = 
    XsectionRatio*TMath::Sqrt(square(statUncertainty/Xsection) + square(statUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double statUncertaintyRatioDown = 
    XsectionRatio*TMath::Sqrt(square(statUncertainty/Xsection) + square(statUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRatioUp   = 
    XsectionRatio*TMath::Sqrt(sysUncertaintyRel2 + square(sysUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRatioDown = 
    XsectionRatio*TMath::Sqrt(sysUncertaintyRel2 + square(sysUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));

  double sysUncertaintyRatioExclTauIdEffUp =
    XsectionRatio*TMath::Sqrt((sysUncertaintyRel2 - square(errTauIdEff)) + square(sysUncertaintyZmumuDown/XsectionZmumuMgt60Mlt120));
  double sysUncertaintyRatioExclTauIdEffDown =
    XsectionRatio*TMath::Sqrt((sysUncertaintyRel2 - square(errTauIdEff)) + square(sysUncertaintyZmumuUp/XsectionZmumuMgt60Mlt120));
  
  std::cout << "Z --> tau+ tau-/Z --> mu+ mu- Xsection ratio = " << XsectionRatio 
	    << " + " << statUncertaintyRatioUp << " - " << statUncertaintyRatioDown << " (stat.)"
	    << " + " << sysUncertaintyRatioUp << " - " << sysUncertaintyRatioDown << " (sys.)" << std::endl;
  std::cout << "(+ " << sysUncertaintyRatioExclTauIdEffUp 
	    << " - " << sysUncertaintyRatioExclTauIdEffDown << " sys. excl. errTauIdEff)" << std::endl;
}
