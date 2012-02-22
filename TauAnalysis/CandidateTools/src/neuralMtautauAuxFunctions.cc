#include "TauAnalysis/CandidateTools/interface/neuralMtautauAuxFunctions.h"

#include <TMath.h>

double compZetaPhi(const reco::Candidate::LorentzVector& leg1P4, const reco::Candidate::LorentzVector& leg2P4)
{
//--- compute azimutal angle of axis 'zeta',
//    the axis which bisects the angle between the visible decay products of two tau leptons
  double leg1x = TMath::Cos(leg1P4.phi());
  double leg1y = TMath::Sin(leg1P4.phi());
  double leg2x = TMath::Cos(leg2P4.phi());
  double leg2y = TMath::Sin(leg2P4.phi());
  double zetaX = leg1x + leg2x;
  double zetaY = leg1y + leg2y;
  double zetaPhi = TMath::ATan2(zetaY, zetaX);
  return zetaPhi;
}

reco::Candidate::LorentzVector compP4inZetaFrame(const reco::Candidate::LorentzVector& p4, double zetaPhi)
{
  //std::cout << "<compP4inZetaFrame>:" << std::endl;
  //std::cout << " zetaPhi = " << zetaPhi << std::endl;
  double cosZetaPhi = TMath::Cos(zetaPhi);
  double sinZetaPhi = TMath::Sin(zetaPhi);
  double px = p4.px();
  double py = p4.py();
  double pxInZetaFrame =  cosZetaPhi*px + sinZetaPhi*py;
  double pyInZetaFrame = -sinZetaPhi*px + cosZetaPhi*py;
  reco::Candidate::LorentzVector p4inZetaFrame(pxInZetaFrame, pyInZetaFrame, p4.pz(), p4.E());
  //std::cout << "p4: E = " << p4.E() << ", Pt = " << p4.pt() << "," 
  //	      << " eta = " << p4.eta() << ", phi = " << p4.phi() << ", M = " << p4.M() << std::endl;
  //std::cout << "p4inZetaFrame: E = " << p4inZetaFrame.E() << ", Pt = " << p4inZetaFrame.pt() << "," 
  //	      << " eta = " << p4inZetaFrame.eta() << ", phi = " << p4inZetaFrame.phi() << ", M = " << p4inZetaFrame.M() << std::endl;
  return p4inZetaFrame;
}

TMatrixD compCovMatrixInZetaFrame(const TMatrixD& cov, double zetaPhi)
{
  double cosZetaPhi = TMath::Cos(zetaPhi);
  double sinZetaPhi = TMath::Sin(zetaPhi);
  double cov00 = cov(0,0);
  double cov01 = cov(0,1);
  double cov10 = cov(1,0);
  double cov11 = cov(1,1);
  TMatrixD covInZetaFrame(2,2);
  covInZetaFrame(0,0) = cosZetaPhi*cosZetaPhi*cov00 + cosZetaPhi*sinZetaPhi*cov01
                       + sinZetaPhi*cosZetaPhi*cov10 + sinZetaPhi*sinZetaPhi*cov11;
  covInZetaFrame(0,1) = -sinZetaPhi*cosZetaPhi*cov00 + cosZetaPhi*cosZetaPhi*cov01
                       - sinZetaPhi*sinZetaPhi*cov10 + cosZetaPhi*sinZetaPhi*cov11;
  covInZetaFrame(1,0) = -cosZetaPhi*sinZetaPhi*cov00 - sinZetaPhi*sinZetaPhi*cov01
                       + cosZetaPhi*cosZetaPhi*cov10 + sinZetaPhi*cosZetaPhi*cov11;
  covInZetaFrame(1,1) = sinZetaPhi*sinZetaPhi*cov00 - cosZetaPhi*sinZetaPhi*cov01
                       - sinZetaPhi*cosZetaPhi*cov10 + cosZetaPhi*cosZetaPhi*cov11;
  return covInZetaFrame;
}
