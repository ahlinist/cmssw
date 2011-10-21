
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

void compWtaunuXsectionNumbersMinus()
{
  //-------------------------------------------------------------------------------
  // define number of observed events
  //-------------------------------------------------------------------------------

  double numEventsObserved = 349;

  std::cout << "Nr Events observed in Data = " << numEventsObserved << std::endl;

  //-------------------------------------------------------------------------------
  // Estimate the number of QCD events as an average of ABCD and template methods
  //-------------------------------------------------------------------------------

  double numEventsExpected_qcd_template = 62.9; //Nr of bg events predicted by the template fit
  double errEventsExpected_qcd_template =  19; //Error of bg events predicted by the template fit
  // CV: subtract statistical component of uncertainty on QCD background estimate
  //     returned by RooFit/template fit
  errEventsExpected_qcd_template = TMath::Sqrt(square(errEventsExpected_qcd_template) - numEventsExpected_qcd_template);
  cout<<"err events expected QCD template = "<<errEventsExpected_qcd_template<<endl;  
  double numEventsExpected_qcd_ABCD     = 100.2; //ABCD method
  double errEventsExpected_qcd_ABCD     =  7.5;

  double weight_qcd_template            = 1./square(errEventsExpected_qcd_template);
  double weight_qcd_ABCD                = 1./square(errEventsExpected_qcd_ABCD);

  double numEventsExpected_qcd          = 
    (weight_qcd_template*numEventsExpected_qcd_template + weight_qcd_ABCD*numEventsExpected_qcd_ABCD)/
    (weight_qcd_template + weight_qcd_ABCD);
  double errEventsExpected_qcd          =  
    TMath::Sqrt(1./(square(1./errEventsExpected_qcd_template) + square(1./errEventsExpected_qcd_ABCD)));

  //----------------------------------------------------------------------------
  // assign 40% uncertainty on EWK backgrounds
  //----------------------------------------------------------------------------

  double numEventsExpected_Wenu    = 20.6; //update to NNLO
  double errEventsExpected_Wenu    = 0.2*numEventsExpected_Wenu;
  double numEventsExpected_Wmunu   =  1.9; //update to NNLO
  double errEventsExpected_Wmunu   = TMath::Sqrt( square(0.75*0.2 + 0.2*0.8) + square(0.12))*numEventsExpected_Wmunu; //0.75 uncertainty for muons faking taus and 0.2 uncertainty for jets faking taus
  double numEventsExpected_Ztautau = 29.6; //update to average of D6T and Z2T
  double errEventsExpected_Ztautau = 0.3*numEventsExpected_Ztautau; // tau + jet energy scale uncertainty + trigger uncertainty
  double numEventsExpected_WtaunuPlus = 2.35;
  double errEventsExpected_WtaunuPlusUp = 2*numEventsExpected_WtaunuPlus;
  double errEventsExpected_WtaunuPlusDown = numEventsExpected_WtaunuPlus;

  double numEventsExpected_ewk     = numEventsExpected_Wenu + numEventsExpected_Wmunu + numEventsExpected_Ztautau +numEventsExpected_WtaunuPlus;
  double errEventsExpected_ewkDown     = TMath::Sqrt(square(errEventsExpected_Wenu) + square(errEventsExpected_Wmunu) + square(errEventsExpected_Ztautau) + square(errEventsExpected_WtaunuPlusDown));
  double errEventsExpected_ewkUp     = TMath::Sqrt(square(errEventsExpected_Wenu) + square(errEventsExpected_Wmunu) + square(errEventsExpected_Ztautau) + square(errEventsExpected_WtaunuPlusUp));
  
  //----------------------------------------------------------------------------
  // Sum Bgr = ewk + qcd
  //---------------------------------------------------------------------------

  double numEventsExpected_Bgr     = numEventsExpected_qcd + numEventsExpected_ewk;
  double errEventsExpected_BgrUp     = TMath::Sqrt(square(errEventsExpected_qcd) + square(errEventsExpected_ewkUp));
  double errEventsExpected_BgrDown     = TMath::Sqrt(square(errEventsExpected_qcd) + square(errEventsExpected_ewkDown));
  
  //-----------------------------------------------------------------------------
  // relative uncertainties
  //------------------------------------------------------------------------------

  double errEventsExpectedRel_ewkUp = errEventsExpected_ewkUp/numEventsExpected_ewk;
  double errEventsExpectedRel_ewkDown = errEventsExpected_ewkDown/numEventsExpected_ewk;
  double errEventsExpectedRel_qcd = errEventsExpected_qcd/numEventsExpected_qcd;
  double errEventsExpectedRel_BgrUp = errEventsExpected_BgrUp/numEventsExpected_Bgr;
  double errEventsExpectedRel_BgrDown = errEventsExpected_BgrDown/numEventsExpected_Bgr;
  
	std::cout << "Nr QCD = " << numEventsExpected_qcd << " +/- " <<errEventsExpected_qcd <<" ("<< errEventsExpectedRel_qcd << "%)" << std::endl;

  std::cout << "Nr EWK = " << numEventsExpected_ewk
			<< " +/- "<<errEventsExpected_Wmunu<<"(Wmunu) +/- "<<errEventsExpected_Wenu<<"(Wenu) +- "<<errEventsExpected_Ztautau <<"(Ztautau) = "
			<<numEventsExpected_ewk<<" + "<<errEventsExpected_ewkUp << " - " <<errEventsExpected_ewkDown<<" ( +" << errEventsExpectedRel_ewkUp<<", - "<<errEventsExpectedRel_ewkDown <<"%)" << std::endl;
  
  std::cout << "Nr Bgr = "<< numEventsExpected_Bgr 
			<< " + " << errEventsExpected_BgrUp<<" - " << errEventsExpected_BgrDown << " ( +" << errEventsExpectedRel_BgrUp <<", -"<< errEventsExpectedRel_BgrDown << "%)" << std::endl;
	
 //----------------------------------------------------------------------------
 // Number of signal events
 //---------------------------------------------------------------------------

  double numSignal = numEventsObserved-numEventsExpected_Bgr;
  double errSignalUp = errEventsExpected_BgrUp;
  double errSignalDown = errEventsExpected_BgrDown;
  double errSignalRelUp = errSignalUp/numSignal;
  double errSignalRelDown = errSignalDown/numSignal;
  
  std::cout << "Nr Sig = " << numSignal 
			<< " + " << errSignalUp << " - " << errSignalDown << " ( +" << errSignalRelUp << "%)" << std::endl;
  
  //-------------------------------------------------------------------------------
  // define signal efficiency and acceptance
  //-------------------------------------------------------------------------------

  double intLumiData       = 32.35; // pb^-1
  
  double mcAcceptance_Sig  = 0.0820;
  double mcEfficiency_Sig  = 0.0331;
  double brToHadTau_Sig    = 0.648;
  
  //-------------------------------------------------------------------------------
  // define systematic uncertainties
  //
  // NOTE: all uncertainties are **relative** to central value
  //-------------------------------------------------------------------------------

  double errTriggerEff     = 0.15;

  double errTauIdEff       = 0.07;

  double errTauEnScaleUp   = 0.170;
  double errTauEnScaleDown = 0.167;
  double errJetEnScaleUp   = 0.113;
  double errJetEnScaleDown = 0.093;

  double errIntLumiData    = 0.04;
  
  double errAcc            = 0.02;

  //-------------------------------------------------------------------------------
  // compute W --> tau nu cross-section "central value" 
  //-------------------------------------------------------------------------------

  // CV: convert luminosity from pb^-1 to nb^-1 
  //    (--> cross-section from pb to nb)
  double denominator = (intLumiData*1.e+3*brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig);
  cout<<"Signal efficiency tot = "<<brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig<<endl;
 
  double Xsection = numSignal/denominator;
  std::cout << "measured Xsection = " << Xsection << " nb" << std::endl;

  double expXsection = 4.286; // nb; computed with FEWZ @ NNLO using MSTW 2008 PDF
                              // ( cf. https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections )
  std::cout << "Theory prediction = " << expXsection << " nb" << std::endl;
  
  //-------------------------------------------------------------------------------
  // compute error estimation on cross section
  //-------------------------------------------------------------------------------
   
   double errSysJetpTauJetUp2    = square(errTauEnScaleUp) + square(errJetEnScaleDown);	
   double errSysJetpTauJetDown2  = square(errTauEnScaleDown) + square(errJetEnScaleUp);
   std::cout << "Sys. error from tau+jet energy scale = " 
	     << " + " << TMath::Sqrt(errSysJetpTauJetUp2) << " - " << TMath::Sqrt(errSysJetpTauJetDown2) << std::endl;

   double errEffAccUp2           = errSysJetpTauJetUp2   + square(errTriggerEff) + square(errAcc) + square(errTauIdEff);
   double errEffAccDown2         = errSysJetpTauJetDown2 + square(errTriggerEff) + square(errAcc) + square(errTauIdEff);

   double sysUncertaintyTotUp2   = errEffAccUp2   + square(errSignalRelUp);
   double sysUncertaintyTotDown2 = errEffAccDown2 + square(errSignalRelDown);
	 
   //------------------------------------------------------------------------------
   // summarize error estimates
   //----------------------------------------------------------------------------

   double statUncertainty = TMath::Sqrt(numEventsObserved)/denominator; 
   std::cout << "statUncertainty = " 
	     << statUncertainty << " nb" << std::endl;

   std::cout << "sysUncertainty = " 
	     << " + " << TMath::Sqrt(sysUncertaintyTotUp2)*Xsection 
	     << " - " << TMath::Sqrt(sysUncertaintyTotDown2)*Xsection << " nb" << std::endl;

   std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << " nb" << std::endl;

   std::cout << "Data/Theory = " << Xsection/expXsection 
	     << " + " << TMath::Sqrt(0.//square(statUncertainty) 
				    + (sysUncertaintyTotUp2   + square(errIntLumiData))*square(Xsection))/expXsection
	     << " - " << TMath::Sqrt(0.//square(statUncertainty) 
				    + (sysUncertaintyTotDown2 + square(errIntLumiData))*square(Xsection))/expXsection 
	     << std::endl;
}
