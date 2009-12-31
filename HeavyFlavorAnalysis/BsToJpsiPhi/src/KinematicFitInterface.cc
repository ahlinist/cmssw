#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/KinematicFitInterface.h"
#include <TMath.h>

KinematicFitInterface::KinematicFitInterface(){


}


bool KinematicFitInterface::doFit(vector<reco::TransientTrack> t_tracks, const double muonMass, const double mass1, const double  mass2){
  
  reco::TransientTrack track_MuP = t_tracks[0];
  reco::TransientTrack track_MuM = t_tracks[1];
  reco::TransientTrack track_Kp = t_tracks[2];
  reco::TransientTrack track_Km = t_tracks[3];
  
  //Creating a KinematicParticleFactory
  KinematicParticleFactoryFromTransientTrack pFactory;
  
  //The mass of a muon and the insignificant mass sigma to avoid singularities in the covariance matrix.
  float muon_sigma = 0.0000000001;
  float kaon_sigma = 0.000016;
  //  float pi_sigma = 0.000016;	      
  
  //initial chi2 and ndf before kinematic fits. The chi2 of the reconstruction is not considered
  float chi = 0.;
  float ndf = 0.;	    
  
  vector<RefCountedKinematicParticle> allParticlesMu;
  allParticlesMu.push_back(pFactory.particle (track_MuP, muonMass, chi, ndf, muon_sigma));
  allParticlesMu.push_back(pFactory.particle (track_MuM, muonMass, chi, ndf, muon_sigma));
  
  
  vector<RefCountedKinematicParticle> allParticlesTrk;
  allParticlesTrk.push_back(pFactory.particle (track_Kp, mass1, chi, ndf, kaon_sigma));
  allParticlesTrk.push_back(pFactory.particle (track_Km, mass2, chi, ndf, kaon_sigma));
  
  KinematicParticleVertexFitter Fitter;
  RefCountedKinematicTree JpsiTree = Fitter.fit(allParticlesMu);
  
  // if the fit fails, do not consider this as candidate
  if(JpsiTree->isEmpty()) return 0;
  
  KinematicParticleFitter constFitter;
  
  double nominalJpsiMass =  3.096916;
  float jpsiMsigma = 0.00004;
  KinematicConstraint * jpsi_const = new MassKinematicConstraint( nominalJpsiMass, jpsiMsigma);
  
  JpsiTree = constFitter.fit(jpsi_const,JpsiTree);
  
  // if the fit fails, do not consider this as candidate
  if(JpsiTree->isEmpty()) return 0;
  
  JpsiTree->movePointerToTheTop();
  RefCountedKinematicParticle Jpsi_branch = JpsiTree->currentParticle();
  allParticlesTrk.push_back(Jpsi_branch);
  
  myTree_Bs = Fitter.fit(allParticlesTrk);
  
  // if the fit fails, do not consider this as candidate
  if(myTree_Bs->isEmpty()) return 0;
  
  
  myTree_Bs->movePointerToTheTop();
 
  bs = myTree_Bs->currentParticle();
  bVertex = myTree_Bs->currentDecayVertex();
  
  if (!bVertex->vertexIsValid()) return 0;
  // vertex is valid
  
  

  vtxprob_Bs = TMath::Prob(bs->chiSquared(), (int)bs->degreesOfFreedom());
  
  
  return 1;
}
