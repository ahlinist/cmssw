
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

  //----------------------------------------------------------------------------
  // assign 40% uncertainty on the sum(ewk)
  //----------------------------------------------------------------------------
  double numEventsExpected_Wenu = 27.17;
  double errEventsExpected_Wenu = 10.8;
  double numEventsExpected_Wmunu = 3.9;
  double errEventsExpected_Wmunu = 1.44;
  double numEventsExpected_Ztautau = 29.4;
  double errEventsExpected_Ztautau = 11.8;

  double numEventsExpected_ewk = numEventsExpected_Wenu + numEventsExpected_Wmunu + numEventsExpected_Ztautau;
  double errEventsExpected_ewk = errEventsExpected_Wenu + errEventsExpected_Wmunu + errEventsExpected_Ztautau;

  //----------------------------------------------------------------------------
  // Sum Bgr = ewk + qcd
  //---------------------------------------------------------------------------
  double numEventsExpected_Bgr = numEventsExpected_qcd + numEventsExpected_ewk;
  double errEventsExpected_Bgr = TMath::Sqrt(square(errEventsExpected_qcd) + square(errEventsExpected_ewk));

  //-----------------------------------------------------------------------------
  // relative uncertainties
  //------------------------------------------------------------------------------
  double errEventsExpectedRel_ewk = errEventsExpected_ewk/numEventsExpected_ewk;
  double errEventsExpectedRel_qcd = errEventsExpected_qcd/numEventsExpected_qcd;
  double errEventsExpectedRel_Bgr = errEventsExpected_Bgr/numEventsExpected_Bgr;
  
  std::cout << "Nr QCD = " << numEventsExpected_qcd  << " +- " << errEventsExpected_qcd << " (" << errEventsExpectedRel_qcd << "%)" << std::endl;
  std::cout << "Nr ewk = " << numEventsExpected_ewk << "+-" << errEventsExpected_ewk<<"("<<errEventsExpectedRel_ewk<<"%)"<<std::endl;
  std::cout<<"Nr Bgr = "<< numEventsExpected_Bgr << "+-" << errEventsExpected_Bgr<<"( "<<errEventsExpectedRel_Bgr<<"%)"<<std::endl;
	
 //----------------------------------------------------------------------------
 // Number of signal events
 //---------------------------------------------------------------------------
  double numSignal = numEventsObserved-numEventsExpected_Bgr;
  double errSignal = errEventsExpected_Bgr;
  double errSignalRel = errSignal/numSignal;
  std::cout << "NSig = "<<numSignal<<"+-"<<errSignal<<" ("<<errSignalRel<<"%)"<<std::endl;
  
//-------------------------------------------------------------------------------
// define signal efficiency and acceptance
//-------------------------------------------------------------------------------

  double intLumiData = 17.8;
  
  double mcAcceptance_Sig = 0.0789;
  double mcEfficiency_Sig = 0.0325;
  double brToHadTau_Sig    = 0.648;
  
//-------------------------------------------------------------------------------
// define systematic uncertainties
//
// NOTE: all uncertainties are **relative** to central value
//-------------------------------------------------------------------------------

  double errTriggerEff   = 0.12;

  double errTauIdEff     = 0.07;

  double errTauEnScaleUp  = 0.151;
  double errTauEnScaleDown = 0.142;
  double errJetEnScaleUp   = 0.095;
  double errJetEnScaleDown = 0.106;

  double errIntLumiData  = 0.04;
  
  double errAcc = 0.02;

//-------------------------------------------------------------------------------
// compute W --> tau nu cross-section "central value" 
//-------------------------------------------------------------------------------

  double denominator = (intLumiData*brToHadTau_Sig*mcAcceptance_Sig*mcEfficiency_Sig);

  double Xsection = numSignal/denominator;
  std::cout << "Xsection = " << Xsection << std::endl;


  //-------------------------------------------------------------------------------
  // compute error estimation on cross section
  //-------------------------------------------------------------------------------

   
   double errSysJetpTauJetUp2 = square(errTauEnScaleUp) + square(errJetEnScaleUp);	//   square(errTriggerEff)
   double errSysJetpTauJetDown2 = square(errTauEnScaleDown) + square(errJetEnScaleDown);    //   square(errTriggerEff)
   std::cout << "Sys. error from tau+jet energy scale  = + " << TMath::Sqrt(errSysJetpTauJetUp2) <<" - "<<TMath::Sqrt(errSysJetpTauJetDown2)<< std::endl;

   double effExclTauId = 3.3e-3;

   double errEffAccUp2 =errSysJetpTauJetUp2 + square(errTriggerEff) + square(errAcc) + square(effExclTauId*errTauIdEff);
   double errEffAccDown2 = errSysJetpTauJetDown2 + square(errTriggerEff) + square(errAcc) + square(effExclTauId*errTauIdEff);

   double sysUncertaintyTotUp2 = errEffAccUp2 + square(errSignalRel);
   double sysUncertaintyTotDown2 = errEffAccDown2 + square(errSignalRel);
	 
   //   double sysUncertaintyTotUp2 = errSysTauJetUp2 + square(errEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr));
   //double sysUncertaintyTotDown2 = errSysTauJetDown2 + square(errEventsExpected_Bgr/(numEventsObserved - numEventsExpected_Bgr));

	 
   //------------------------------------------------------------------------------
   // summarize error estimates
   //----------------------------------------------------------------------------
   double statUncertainty = TMath::Sqrt(numEventsObserved)/denominator; 
   std::cout << "statUncertainty = " << statUncertainty << std::endl;
   
   std::cout << "sysUncertainty:" <<" = + " << TMath::Sqrt(sysUncertaintyTotUp2)*Xsection <<" - "<<TMath::Sqrt(sysUncertaintyTotDown2)*Xsection<<std::endl;

   std::cout << "intLumiDataUncertainty = " << errIntLumiData*Xsection << std::endl;

}
