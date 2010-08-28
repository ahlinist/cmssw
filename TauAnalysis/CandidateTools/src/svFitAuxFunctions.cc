#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/Vector3D.h"

#include <TMath.h>

#include <limits>

namespace SVfit_namespace {
  
  // Adapted for our vector types from TVector3 class
  reco::Candidate::Vector rotateUz(const math::RThetaPhiVector& toRotate,
				   const reco::Candidate::Vector& newUzVector)
  {
    // NB: newUzVector must be a unit vector !
    Double_t u1 = newUzVector.X();
    Double_t u2 = newUzVector.Y();
    Double_t u3 = newUzVector.Z();
    Double_t up = u1*u1 + u2*u2;
    
    Double_t fX = toRotate.X(); 
    Double_t fY = toRotate.Y(); 
    Double_t fZ = toRotate.Z();
    
    if ( up ) {
      up = TMath::Sqrt(up);
      Double_t px = fX,  py = fY,  pz = fZ;
      fX = (u1*u3*px - u2*py + u1*up*pz)/up;
      fY = (u2*u3*px + u1*py + u2*up*pz)/up;
      fZ = (u3*u3*px -    px + u3*up*pz)/up;
    } else if ( u3 < 0. ) { 
      fX = -fX; fZ = -fZ; 
    } else {}; // phi = 0, theta = pi
    return reco::Candidate::Vector(fX, fY, fZ);
  }
  
  double pVisRestFrame(double tauVisMass, double tauNuNuMass) 
  {
    return TMath::Sqrt((tauLeptonMass2 - square(tauVisMass + tauNuNuMass))*
		       (tauLeptonMass2 - square(tauVisMass - tauNuNuMass)))/(2*tauLeptonMass);
  }
  
  double gjAngleToLabFrame(double pVisRestFrame, double gjAngle, double pVisLabFrame)
  {
    // Get the compenent of the rest frame momentum perpindicular to the tau
    // boost direction. This quantity is Lorentz invariant.
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
    
    // Now use the Lorentz equation for pVis along the tau direction to solve for
    // the gamma of the tau boost.
    const double pVisRestFramePara = pVisRestFrame * TMath::Cos(gjAngle);
    const double eVisRestFrame = TMath::Sqrt(square(tauVisMass) + square(pVisRestFrame));
    
    const double gamma = (eVisRestFrame * TMath::Sqrt(square(eVisRestFrame) + square(pVisLabFramePara) 
      - square(pVisRestFramePara)) - pVisRestFramePara*pVisLabFramePara)/(square(eVisRestFrame) - square(pVisRestFramePara));
    
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
    // NB: tauDirection must be a unit vector !
    reco::Candidate::Vector tauMomentum = tauDirection*tauMomentumLabFrame;
    return reco::Candidate::LorentzVector(
      math::PtEtaPhiMLorentzVector(tauMomentum.rho(), tauMomentum.eta(), tauMomentum.phi(), tauLeptonMass));
  }

//
//-------------------------------------------------------------------------------
//

  double logGaussian(double residual, double sigma)
  {
    if ( sigma > 0. ) {
      return -0.5*TMath::Log(2*TMath::Pi()*sigma) - 0.5*square(residual/sigma);
    } else { 
      edm::LogError ("logGaussian")
	<< " Parameter sigma must not be zero !!";
      return std::numeric_limits<float>::min();
    }
  }
}
