
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

void compNumbers(double numEventsObserved, double numEventsBgr, double errBgrUp, double errBgrDown, 
		 double intLumiData, double errIntLumiData, 
		 double br, double accSig, double errAccSig, double effSig, double errEffExclTauIdSig,
		 double numEventsSigC1, double errEventsSigC1, double tauIdEff, double errTauIdEff, double cov_numEventsSigC1_tauIdEff)
{
//--- compute estimate for number of signal events in region Xp
  double numEvents_Xp = (numEventsObserved - numEventsBgr) - numEventsSigC1*tauIdEff;
  std::cout << " numEvents_Xp = " << numEvents_Xp << std::endl;

  double effEvtSelExclTauId = effSig/tauIdEff;

  double Xsection = (numEvents_Xp/tauIdEff + numEventsSigC1)/(intLumiData*br*accSig*effEvtSelExclTauId);

//--- compute uncertainty on number of signal events in C1p region
//   (formula 6 of: George W. Bohrnstedt, Arthur A. Goldberger,
//                  "On the exact covariance of products of random variables",
//                  Journal of the American Statistical Association, Vol. 64, No. 328 (1969), pp. 1439-1442)
  double errEventsSig_C1p2 = 
    square(numEventsSigC1*errTauIdEff) 
   + square(errEventsSigC1*tauIdEff)
   + 2*numEventsSigC1*tauIdEff*cov_numEventsSigC1_tauIdEff
   + square(errEventsSigC1*errTauIdEff) + square(cov_numEventsSigC1_tauIdEff);
  std::cout << " errEventsSig_C1p = " << TMath::Sqrt(errEventsSig_C1p2) << std::endl;

//--- compute estimate for uncertainty on number of background events contributing to region Xp
  double errBgrUp_Xp2   = square(errBgrUp)   - errEventsSig_C1p2;
  if ( errBgrUp_Xp2   < 0. ) errBgrUp_Xp2   = 0.;
  std::cout << " errBgrUp_Xp = " << TMath::Sqrt(errBgrUp_Xp2) << std::endl;
  double errBgrDown_Xp2 = square(errBgrDown) - errEventsSig_C1p2;
  if ( errBgrDown_Xp2 < 0. ) errBgrDown_Xp2 = 0.;
  std::cout << " errBgrDown_Xp = " << TMath::Sqrt(errBgrDown_Xp2) << std::endl;

//--- compute uncertainty on cross-section taking into account 
//    correction between cross-section and tau id. efficiency uncertainty measurements
  double errXsectionUp2 =
    (square(TMath::Sqrt(errBgrDown_Xp2)/tauIdEff)
   + square(numEvents_Xp*errTauIdEff/square(tauIdEff))
   - 2*(numEvents_Xp/square(tauIdEff))*cov_numEventsSigC1_tauIdEff
   + square(errEventsSigC1))/square(intLumiData*br*accSig*effEvtSelExclTauId)
   + square(Xsection)*(square(errAccSig/accSig) + square(errEffExclTauIdSig/effEvtSelExclTauId));
  double errXsectionDown2 = 
    (square(TMath::Sqrt(errBgrUp_Xp2)/tauIdEff)
   + square(numEvents_Xp*errTauIdEff/square(tauIdEff))
   - 2*(numEvents_Xp/square(tauIdEff))*cov_numEventsSigC1_tauIdEff
   + square(errEventsSigC1))/square(intLumiData*br*accSig*effEvtSelExclTauId)
   + square(Xsection)*(square(errAccSig/accSig) + square(errEffExclTauIdSig/effEvtSelExclTauId));

  std::cout << "Xsection = " << Xsection 
	    << " +/- " << Xsection*TMath::Sqrt(numEventsObserved)/(numEventsObserved - numEventsBgr) << " (stat.)"
	    << " + " << TMath::Sqrt(errXsectionUp2) 
	    << " - " << TMath::Sqrt(errXsectionDown2) << " (sys.)" 
            << " +/- " << Xsection*errIntLumiData << " (lumi.)" << std::endl;

//--- compare with uncertainty on cross-section obtained
//    in case correction between cross-section and tau id. efficiency uncertainty measurements
//    is **not** taken into account
  double sysUncertaintyExclTauIdUp2 = 
    square(errAccSig) 
   + square(errEffExclTauIdSig)
   + square(errBgrDown/(numEventsObserved - numEventsBgr));
  double sysUncertaintyExclTauIdDown2 = 
    square(errAccSig) 
   + square(errEffExclTauIdSig)
   + square(errBgrUp/(numEventsObserved - numEventsBgr));	    

  std::cout << "rel. effect of tau id. eff. uncertainty:" << std::endl; 
  std::cout << " uncorrelated: " << errTauIdEff/tauIdEff << std::endl;
  std::cout << " correlated:" 
	    << " +" << TMath::Sqrt(errXsectionUp2/square(Xsection) - sysUncertaintyExclTauIdUp2)
	    << " -" << TMath::Sqrt(errXsectionDown2/square(Xsection) - sysUncertaintyExclTauIdDown2) << std::endl;
}

void compZtautauXsectionNumbers_wCorrelation()
{
//-------------------------------------------------------------------------------
// define numbers common to TaNC loose and HPS loose
//-------------------------------------------------------------------------------

  double intLumiData       = 36.1;
  double errIntLumiData    =  0.1100; // relative uncertainty 
  
  double mcAcceptance_Sig  =  0.0681;
  double errAcceptance_Sig =  0.0896*mcAcceptance_Sig;

  double brToMuTau_Sig     =  0.2250;

//-------------------------------------------------------------------------------
// define numbers specific to TaNC loose
//-------------------------------------------------------------------------------

  double numEventsObserved_TaNCloose                = 556;
  double numEventsBgr_TaNCloose                     = 229.5;
  double errBgrUp_TaNCloose                         =  19.5;
  double errBgrDown_TaNCloose                       =  29.6;

  double mcEfficiency_Sig_TaNCloose                 =   0.9650*0.3613;
  double errEfficiencyEvtSelExclTauId_Sig_TaNCloose =   0.0508*mcEfficiency_Sig_TaNCloose;

  double numEventsSigC1_TaNCloose                   = 280.39;
  double errEventsSigC1_TaNCloose                   =  66.16;
  double tauIdEff_TaNCloose                         =   0.437;  
  double errTauIdEff_TaNCloose                      =   0.101;
  double cov_numEventsSigC1_tauIdEff_TaNCloose      =  -6.92;

//-------------------------------------------------------------------------------
// define numbers specific to HPS loose
//-------------------------------------------------------------------------------

  double numEventsObserved_HPSloose                 = 518;
  double numEventsBgr_HPSloose                      = 245.0;
  double errBgr_HPSloose                            =  22.0;
  
  double mcEfficiency_Sig_HPSloose                  =   0.997*0.0221/mcAcceptance_Sig;
  double errEfficiencyEvtSelExclTauId_Sig_HPSloose  =   0.064*mcEfficiency_Sig_HPSloose;

  double numEventsSigC1_HPSloose                    = 279.74;
  double errEventsSigC1_HPSloose                    =  64.27;
  double tauIdEff_HPSloose                          =   0.428;  
  double errTauIdEff_HPSloose                       =   0.099;
  double cov_numEventsSigC1_tauIdEff_HPSloose       =  -6.13;

//-------------------------------------------------------------------------------
// compute cross-sections for TaNC loose and HPS loose
//-------------------------------------------------------------------------------

  std::cout << "TaNC loose:" << std::endl;
  compNumbers(numEventsObserved_TaNCloose, numEventsBgr_TaNCloose, errBgrUp_TaNCloose, errBgrDown_TaNCloose,
	      intLumiData, errIntLumiData, 
	      brToMuTau_Sig, mcAcceptance_Sig, errAcceptance_Sig, mcEfficiency_Sig_TaNCloose, errEfficiencyEvtSelExclTauId_Sig_TaNCloose,
	      numEventsSigC1_TaNCloose, errEventsSigC1_TaNCloose, 
	      tauIdEff_TaNCloose, errTauIdEff_TaNCloose, cov_numEventsSigC1_tauIdEff_TaNCloose);
  std::cout << std::endl;

  std::cout << "HPS loose:" << std::endl;
  compNumbers(numEventsObserved_HPSloose, numEventsBgr_HPSloose, errBgr_HPSloose, errBgr_HPSloose,
	      intLumiData, errIntLumiData, 
	      brToMuTau_Sig, mcAcceptance_Sig, errAcceptance_Sig, mcEfficiency_Sig_HPSloose, errEfficiencyEvtSelExclTauId_Sig_HPSloose,
	      numEventsSigC1_HPSloose, errEventsSigC1_HPSloose, 
	      tauIdEff_HPSloose, errTauIdEff_HPSloose, cov_numEventsSigC1_tauIdEff_HPSloose);
  std::cout << std::endl;
}
