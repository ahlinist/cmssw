
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

void compWtaunuXsectionNumbers()
{
//-------------------------------------------------------------------------------
// define number of observed events
//-------------------------------------------------------------------------------

  double numEventsObserved = 412;

  std::cout <<"nr events observed in data = "<<numEventsObserved<<std::endl;
//-------------------------------------------------------------------------------
// Estimate the number of QCD events as an average of ABCD and template methods
//-------------------------------------------------------------------------------

  double numEventsExpected_qcd_template = 122; //Nr of bg events predicted by the template fit
  double errEventsExpected_qcd_template =  22; //Error of bg events predicted by the template fit
  
  double numEventsExpected_qcd_ABCD       = 126; //ABCD method
  double errEventsExpected_qcd_ABCD    =  20;

  double weight_qcd_template            = 1./square(errEventsExpected_qcd_template);
  double weight_qcd_ABCD                  = 1./square(errEventsExpected_qcd_ABCD);

  double numEventsExpected_qcd = (weight_qcd_template*numEventsExpected_qcd_template + weight_qcd_ABCD*numEventsExpected_qcd_ABCD)/(weight_qcd_template + weight_qcd_ABCD);
  double errEventsExpected_qcd  =  TMath::Sqrt(1./(square(1./errEventsExpected_qcd_template) + square(1./errEventsExpected_qcd_ABCD)));

  double numEventsExpected_Wenu = 18.6;
  double errEventsExpected_Wenu = 18.6;
  double numEventsExpected_Wmunu = 3.6;
  double errEventsExpected_Wmunu = 3.6;
  double numEventsExpected_Ztautau = 27.2;
  double errEventsExpected_Ztautau = 27.2;

  double numEventsExpected_ewk = numEventsExpected_Wenu + numEventsExpected_Wmunu + numEventsExpected_Ztautau;
  double errEventsExpected_ewk = TMath::Sqrt(square(errEventsExpected_Wenu) + square(errEventsExpected_Wmunu) + square(errEventsExpected_Ztautau));
  
  std::cout << "Nr QCD = " << numEventsExpected_qcd  << " +- " << errEventsExpected_qcd << " (" << errEventsExpected_qcd/numEventsExpected_qcd*100. << "%)" << std::endl;
  std::cout << "Nr ewk = " << numEventsExpected_ewk << "+-" << errEventsExpected_ewk<<std::endl;
	
  double numEventsExpected_Bgr = numEventsExpected_qcd + numEventsExpected_ewk;
  double errEventsExpected_Bgr = TMath::Sqrt(square(errEventsExpected_qcd) + square(errEventsExpected_ewk));
  //----------------------------------------------------------------------------
  // Expected number of signal events
  //---------------------------------------------------------------------------
  std::cout << "Nsig = " << (numEventsObserved - numEventsExpected_Bgr)<< " +- " << TMath::Sqrt(numEventsObserved + square(errEventsExpected_Bgr)) << std::endl;
  
//-------------------------------------------------------------------------------
// define signal efficiency and acceptance
//-------------------------------------------------------------------------------

  double intLumiData = 17.8;
  
  double mcAcceptance_Sig = 0.0789;
  double mcEfficiency_Sig = 0.0325;
  double brToHadTau_Sig    = 0.648;
  
//-------------------------------------------------------------------------------
// define Data to Monte Carlo correction factors
//-------------------------------------------------------------------------------
/*
    double corrTriggerEff = 0.981;
  double corrMuonIdEff  = 1.001;
  double corrMuonIsoEff = 0.984;
*/
//-------------------------------------------------------------------------------
// define systematic uncertainties
//
// NOTE: all uncertainties are **relative** to central value
//
//-------------------------------------------------------------------------------

//  double errTriggerEff   = 0.0060/corrTriggerEff;

  double errTauIdEff     = 0.07;
  double errIntLumiData  = 0.04; 

  double errTauEnScale  = 0.15;
  double errJetEnScaleUp   = 0.109;
  double errJetEnScaleDown = 0.093;
  
  //  double errAccISR       = 0.0380;
  // double errAccFSR       = 0.0010; 
  //double errAccPDF       = 0.0340;
  //  double errAccGenerator = 0.0737; // difference in efficiency*acceptance between PYTHIA and POWHEG

//-------------------------------------------------------------------------------
// compute W --> tau nu cross-section "central value" and uncertainties
//-------------------------------------------------------------------------------

  double denominator = (intLumiData*brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig);

  double Xsection = (numEventsObserved - numEventsExpected_Bgr)/denominator;
  std::cout << "Xsection = " << Xsection << std::endl;
  
   std::cout << "rel. effect of Nbgr uncertainty:" << " = " << errEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr)  << std::endl;

   /*(  double errAcc2 = 
    square(errAccISR)
   + square(errAccFSR)
   + square(errAccPDF)
   + square(errAccGenerator);
  std::cout << "errAcc = " << TMath::Sqrt(errAcc2) << std::endl;
   */
   
   double errSysTauJetUp2 = square(errTauEnScale) + square(errJetEnScaleUp);	//   square(errTriggerEff)
   double errSysTauJetDown2 = square(errTauEnScale) + square(errJetEnScaleDown);    //   square(errTriggerEff)
   std::cout << "Sys. error from tau+jet energy scale  = + " << TMath::Sqrt(errSysTauJetDown2) <<" - "<<TMath::Sqrt(errSysTauJetUp2)<< std::endl;


   double sysUncertaintyTotUp2 = errSysTauJetUp2 + square(errEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr));
   double sysUncertaintyTotDown2 = errSysTauJetDown2 + square(errEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr));

   double statUncertainty = TMath::Sqrt(numEventsObserved)/denominator;
   std::cout << "statUncertainty = " << statUncertainty << std::endl;
   
   std::cout << "sysUncertainty:" <<" = + " << TMath::Sqrt(sysUncertaintyTotDown2)*Xsection <<" - "<<TMath::Sqrt(sysUncertaintyTotUp2)*Xsection<<std::endl;
   
   std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << std::endl;
   
}
