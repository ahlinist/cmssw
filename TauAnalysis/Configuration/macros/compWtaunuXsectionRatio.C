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

void compWtaunuXsectionRatio()
{
  double numSignalPlus = 281.5;
  double numSignalMinus = 200.3;
  
  double SignalRatio = numSignalPlus/numSignalMinus;

  double ErrQCDPlus = 6.56;
  double ErrQCDMinus = 6.88;
  double ErrQCDRatio = TMath::Sqrt( square(ErrQCDPlus/numSignalMinus) + square(numSignalPlus/numSignalMinus**2*ErrQCDMinus) );

  double ErrWmunuRatio = 3e-4;
  double ErrWenuRatioUp = 7e-4;
  double ErrWenuRatioDown = 8e-4;
  double ErrZtautauRatioUp = 2.13e-2;
  double ErrZtautauRatioDown = 2.32e-2;
  double ErrChargeUp = 7.40e-3;
  double ErrChargeDown = 1.32e-3;

  double ErrSignalRatioUp = TMath::Sqrt(ErrWmunuRatio**2 + ErrWenuRatioUp**2 + ErrZtautauRatioUp**2 + ErrChargeUp**2 + ErrQCDRatio**2);
  double ErrSignalRatioDown = TMath::Sqrt(ErrWmunuRatio**2 + ErrWenuRatioDown**2 + ErrZtautauRatioDown**2 + ErrChargeDown**2 + ErrQCDRatio**2);
  std::cout<<"Signal ratio = "<<SignalRatio<<" + "<<ErrSignalRatioUp << "-" <<ErrSignalRatioDown<<std::endl;
	
  double AccPlus = 0.0773;
  double AccMinus = 0.0820;

  double AccRatio = AccMinus/AccPlus;
  double ErrAcc = 0.02;
  
  double EffPlus = 0.333;
  double EffMinus = 0.331;

  double EffRatio = EffMinus/EffPlus;
  
  double ErrTauUp = 1.386e-2;
  double ErrTauDown = -3.477e-2;
  double ErrJetUp = -1.990e-2;
  double ErrJetDown = -1.177e-2;
  
  double Err_EffRatioUp = ErrTauUp*EffRatio;
  double Err_EffRatioDown = TMath::Sqrt(ErrTauDown**2 + ErrJetUp**2 + ErrJetDown**2)*EffRatio;
  std::cout<<"Eff ratio = "<<EffRatio<<" + "<<Err_EffRatioUp<<" - "<<Err_EffRatioDown<<endl;

  double RatioPlusMinus = AccRatio*EffRatio*SignalRatio;
  double ErrPlusMinusUp = TMath::Sqrt( square(Err_EffRatioUp*AccRatio*SignalRatio) + square(ErrSignalRatioUp*AccRatio*EffRatio) + square(ErrAcc*EffRatio*SignalRatio));
  double ErrPlusMinusDown = TMath::Sqrt( square(Err_EffRatioDown*AccRatio*SignalRatio) + square(ErrSignalRatioDown*AccRatio*EffRatio) + square(ErrAcc*EffRatio*SignalRatio));
  
  
  std::cout<< "Ratio = "<<RatioPlusMinus<<" + "<<ErrPlusMinusUp<<" - "<<ErrPlusMinusDown<<endl;

  
}
