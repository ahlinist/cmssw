#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"
#include "DataFormats/Candidate/interface/Candidate.h"

const double tauLeptonMass = 1.77685; // GeV
const double tauLeptonMass2 = tauLeptonMass*tauLeptonMass;

#include "TMath.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"


namespace SVfit_namespace {

inline double square(double x)
{
  return x*x;	
}

inline double energy(double mass, double momentum)
{
   return TMath::Sqrt(square(mass) + square(momentum));
}

// Adapted for our vector types from TVector3 class
reco::Candidate::Vector rotateUz(const math::RThetaPhiVector& toRotate,
     const reco::Candidate::Vector& newUzVector)
{
   // newUzVector must be normalized !
   Double_t u1 = newUzVector.X();
   Double_t u2 = newUzVector.Y();
   Double_t u3 = newUzVector.Z();
   Double_t up = u1*u1 + u2*u2;

   Double_t fX = toRotate.X(); 
   Double_t fY = toRotate.Y(); 
   Double_t fZ = toRotate.Z();

   if (up) {
      up = TMath::Sqrt(up);
      Double_t px = fX,  py = fY,  pz = fZ;
      fX = (u1*u3*px - u2*py + u1*up*pz)/up;
      fY = (u2*u3*px + u1*py + u2*up*pz)/up;
      fZ = (u3*u3*px -    px + u3*up*pz)/up;
   }
   else if (u3 < 0.) { fX = -fX; fZ = -fZ; }      // phi=0  teta=pi
   else {};
   return reco::Candidate::Vector(fX, fY, fZ);
}


double pVisRestFrame(double tauVisMass, double tauNuNuMass) 
{
   double numerator = (tauLeptonMass2 - square(tauVisMass + tauNuNuMass))*
                      (tauLeptonMass2 - square(tauVisMass - tauNuNuMass));
   return TMath::Sqrt(numerator)/(2*tauLeptonMass);
}

double gjAngleToLabFrame(double pVisRestFrame, double gjAngle, double pVisLabFrame)
{
   // Get the compenent of the rest frame momentum perpindicular to the tau
   // boost direction.  This quantity is Lorentz invariant.
   const double pVisRestFramePerp = pVisRestFrame * TMath::Sin(gjAngle);
   // Determine the corresponding opening angle in the LAB frame
   return TMath::ASin(pVisRestFramePerp/pVisLabFrame);
}

double tauMomentumLabFrame(double tauVisMass, double pVisRestFrame, double gjAngle, double pVisLabFrame)
{
   // Determine the corresponding opening angle in the LAB frame
   const double angleVisLabFrame = gjAngleToLabFrame(pVisRestFrame, gjAngle, pVisLabFrame);
   // Get the visible momentum perpindicular/parallel to the tau boost direction in the LAB
   const double pVisLabFramePara = pVisLabFrame*TMath::Cos(angleVisLabFrame);

   // Now use the Lorentz equation for VisP along the tau direction to solve for
   // the gamma of the tau boost.
   const double pVisRestFramePara = pVisRestFrame * TMath::Cos(gjAngle);
   const double eVisRestFrame = energy(tauVisMass, pVisRestFrame);

   const double gamma = (
         eVisRestFrame * TMath::Sqrt(
            square(eVisRestFrame) + 
            square(pVisLabFramePara) - 
            square(pVisRestFramePara)
            ) - pVisRestFramePara*pVisLabFramePara
         ) / (square(eVisRestFrame) - square(pVisRestFramePara));

   return TMath::Sqrt(square(gamma) - 1)*tauLeptonMass;
}

reco::Candidate::Vector tauDirection(const reco::Candidate::Vector& pVisLabFrame, double angleVisLabFrame, double phiLab)
{
   // The direction is defined using polar coordinates in a system where the visible energy
   // defines the Z axis.
   math::RThetaPhiVector tauDirectionVisibleSystem(1.0, angleVisLabFrame, phiLab);
   // Rotate into the LAB coordinate system
   return rotateUz(tauDirectionVisibleSystem, pVisLabFrame.Unit());
}

reco::Candidate::LorentzVector tauP4(const reco::Candidate::Vector& tauDirection, double tauMomentumLabFrame)
{
   // NB tauDirection must be a unit vector!
   reco::Candidate::Vector tauMomentum = tauDirection*tauMomentumLabFrame;
   return reco::Candidate::LorentzVector(
         math::PtEtaPhiMLorentzVector(tauMomentum.rho(), tauMomentum.eta(), tauMomentum.phi(), tauLeptonMass)
         );
}

//
//-------------------------------------------------------------------------------
//

reco::Candidate::Point operator+(const reco::Candidate::Point& p1, const reco::Candidate::Point& p2)
{
  return reco::Candidate::Point(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
}

}
