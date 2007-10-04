
#include "EgammaAnalysis/EgammaEfficiencyAlgos/interface/EmObjectAlgo.h"

// framework includes
#include "FWCore/MessageLogger/interface/MessageLogger.h"

double EmObjectAlgo::ecalEta(double &EtaParticle, double &Zvertex, double &RhoVertex)
{

  const double R_ECAL = 136.5;
  const double etaBarrelEndcap = 1.479;
  const double Z_Endcap = 328.0;
  const double pi = 3.14159265;

  if (EtaParticle!= 0.)
    {
      double Theta = 0.0  ;
      double ZEcal = (R_ECAL-RhoVertex)*sinh(EtaParticle)+Zvertex;

      if(ZEcal != 0.0) Theta = atan(R_ECAL/ZEcal);
      if(Theta<0.0) Theta = Theta+pi;

      double ETA = - log(tan(0.5*Theta));

      if( fabs(ETA) > etaBarrelEndcap )
   {
     double Zend = Z_Endcap ;
     if(EtaParticle<0.0 )  Zend = -Zend ;
     double Zlen = Zend - Zvertex ;
     double RR = Zlen/sinh(EtaParticle);
     Theta = atan((RR+RhoVertex)/Zend);
     if(Theta<0.0) Theta = Theta+pi;
     ETA = - log(tan(0.5*Theta));
   }
      return ETA;
    }
  else
    {
      edm::LogWarning("") 
        << "[EcalPositionFromTrack::etaTransformation] Warning: Eta equals to zero, not correcting";
      return EtaParticle;
    }
}

double EmObjectAlgo::ecalPhi(double &PtParticle, double &EtaParticle, double &PhiParticle, int ChargeParticle, double &Rstart)
{
  //Magnetic field
  const double RBARM = 1.357 ;  // was 1.31 , updated on 16122003
  const double ZENDM = 3.186 ;  // was 3.15 , updated on 16122003
  double Rbend = RBARM-(Rstart/100.); //Assumed Rstart in cm
  double Bend  = 0.3 * 4. * Rbend/ 2.0 ;
  const double pi = 3.14159265;
  const double etaBarrelEndcap = 1.479;

  //---PHI correction
  double PHI = 0.0 ;
  if( fabs(EtaParticle) <=  etaBarrelEndcap)
    {
      if (fabs(Bend/PtParticle)<=1.)
	{
	  PHI = PhiParticle - asin(Bend/PtParticle)*ChargeParticle;
	  if(PHI >  pi) {PHI = PHI - 2*pi;}
	  if(PHI < -pi) {PHI = PHI + 2*pi;}
	}
      else
	{
	  edm::LogWarning("") << "[EcalPositionFromTrack::phiTransformation] Warning:Too low Pt, giving up ";
	  return PhiParticle;
	}
    }
  
  if( fabs(EtaParticle) >  etaBarrelEndcap )
    {
      double Rhit = 0.0 ;
      Rhit = ZENDM / sinh(fabs(EtaParticle));
      if (fabs(((Rhit-(Rstart/100.))/Rbend)*Bend/PtParticle)<=1.)
	{
	  PHI = PhiParticle - asin(((Rhit-(Rstart/100.))/Rbend)*Bend/PtParticle)*ChargeParticle;
	  if(PHI >  pi) {PHI = PHI - 2*pi;}
	  if(PHI < -pi) {PHI = PHI + 2*pi;}
	}
      else
	{
	  edm::LogWarning("") <<"[EcalPositionFromTrack::phiTransformation] Warning:Too low Pt, giving up ";
	  return PhiParticle;
	}
      
    }
  
  //---Return the result
  return PHI;
}

