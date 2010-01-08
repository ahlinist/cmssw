// -*- C++ -*-
//
// Package:    PATBu2JpsiKstar
// Class:      PATBu2JpsiKstar
// 
/**\class PATBu2JpsiKstar PATBu2JpsiKstar.cc HeavyFlavorAnalysis/Examples/src/PATBu2JpsiKstar.cc

 Description: <one line class summary>
Make rootTuple for b->s mu mu reconstruction

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Keith Ulmer
//         Created:  Mon Apr 21 09:53:19 MDT 2008
// $Id: PATBu2JpsiKstar.h,v 1.3 2009/12/18 22:19:46 kaulmer Exp $
//
//

#ifndef _PATBu2JpsiKstar_h
#define _PATBu2JpsiKstar_h

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackFromFTSFactory.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/V0Candidate/interface/V0Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "RecoVertex/V0Producer/interface/V0Producer.h"

#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "TFile.h"
#include "TTree.h"


//
// class decleration
//

class PATBu2JpsiKstar : public edm::EDAnalyzer {
public:
  explicit PATBu2JpsiKstar(const edm::ParameterSet&);
  ~PATBu2JpsiKstar();
  void fillPsi(const reco::Candidate& genpsi);
  void fillV0(const reco::Candidate& genv0);
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  void printout(const RefCountedKinematicVertex& myVertex) const;
  void printout(const RefCountedKinematicParticle& myParticle) const;
  void printout(const RefCountedKinematicTree& myTree) const;
  
  // ----------member data ---------------------------
  std::string hepMC;
  edm::InputTag hlTriggerResults_;
  std::string blist;
  std::string v0Producer;
  std::string v0Type;
  std::string muonType;
  std::string vtxSample;
  TTree*      tree_;

  unsigned int        l1_mu3, l1_2mu3, l1_muOpen, l1_mu0;
  unsigned int        hlt_mu3, hlt_mu5, hlt_mu7, hlt_mu9, hlt_2mu0, hlt_2mu3, hlt_2mu3JPsi, hltBJPsiMuMu;
  unsigned int        nB;
  float               priVtxX, priVtxY, priVtxZ, priVtxXE, priVtxYE, priVtxZE, priVtxCL;
  vector<float>       *bMass, *bVtxCL, *bVtxC2, *bPx, *bPy, *bPz;
  vector<double>      *bPxE, *bPyE, *bPzE;
  vector<float>       *bctauMPVBS;
  vector<float>       *bDecayVtxX, *bDecayVtxY, *bDecayVtxZ;
  vector<double>       *bDecayVtxXE, *bDecayVtxYE, *bDecayVtxZE;
  vector<float>       *bResMass, *bResVtxCL, *bResPx, *bResPy, *bResPz;
  vector<float>       *bResDecayVtxX, *bResDecayVtxY, *bResDecayVtxZ;
  vector<float>       *bResDecayVtxXE, *bResDecayVtxYE, *bResDecayVtxZE;
  vector<float>       *VMass, *VVtxCL, *VPx, *VPy, *VPz;
  vector<float>       *VDecayVtxX, *VDecayVtxY, *VDecayVtxZ;
  vector<float>       *VDecayVtxXE, *VDecayVtxYE, *VDecayVtxZE;
  vector<float>       *JMass, *JVtxCL, *JVtxC2, *JPx, *JPy, *JPz;
  vector<float>       *JDecayVtxX, *JDecayVtxY, *JDecayVtxZ;
  vector<float>       *JDecayVtxXE, *JDecayVtxYE, *JDecayVtxZE;
  vector<float>       *mumPx, *mumPy, *mumPz, *mumD0, *mumD0E;
  vector<float>       *mupPx, *mupPy, *mupPz, *mupD0, *mupD0E;
  vector<float>       *VTrkpMass, *VTrkpPx, *VTrkpPy, *VTrkpPz, *VTrkpD0, *VTrkpD0E;
  vector<float>       *VTrkmMass, *VTrkmPx, *VTrkmPy, *VTrkmPz, *VTrkmD0, *VTrkmD0E;
  vector<float>       *bResTrkPx, *bResTrkPy, *bResTrkPz, *bResTrkD0, *bResTrkD0E;
  vector<int>         *bResTrkChg;
  vector<float>       *bResTrkC2, *bResTrkNormC2, *bResTrkPsiD02D, *bResTrkPsiD03D, *bResTrkPsiD02DE, *bResTrkPsiD03DE;
  vector<int>         *bResTrkNHits, *bResTrkNValidHits, *bResTrkPixelLayers, *bResTrkQual;
  int                 genKsPsi, genKstarpPsi, genLambdaPsi, feedup, feeddown;

  vector<float>       *JVVtxCL, *JVVtxC2, *JpiVtxCL, *JpiVtxC2;

  float       truePriVtxX, truePriVtxY, truePriVtxZ;
  float       trueBPx, trueBPy, trueBPz;
  float       trueBDecayVtxX, trueBDecayVtxY, trueBDecayVtxZ;
  float       trueBResPx, trueBResPy, trueBResPz;
  float       trueBResDecayVtxX, trueBResDecayVtxY, trueBResDecayVtxZ;
  float       trueVPx, trueVPy, trueVPz;
  float       trueVDecayVtxX, trueVDecayVtxY, trueVDecayVtxZ;
  float       trueJPx, trueJPy, trueJPz;
  float       trueJDecayVtxX, trueJDecayVtxY, trueJDecayVtxZ;
  float       trueMumPx, trueMumPy, trueMumPz, trueMumD0;
  float       trueMupPx, trueMupPy, trueMupPz, trueMupD0;
  float       trueVTrkpPx, trueVTrkpPy, trueVTrkpPz, trueVTrkpD0;
  float       trueVTrkmPx, trueVTrkmPy, trueVTrkmPz, trueVTrkmD0;
  float       trueBResTrkPx, trueBResTrkPy, trueBResTrkPz, trueBResTrkD0;
  int         trueBResTrkChg;
  int         prompt;
  vector<int> truthMatch, truthKstp, truthKs, truthPsi;


};

#endif
