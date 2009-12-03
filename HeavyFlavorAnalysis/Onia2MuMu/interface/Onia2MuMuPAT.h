#ifndef HeavyFlavorAnalysis_Onia2MuMu_Onia2MuMuPAT_h
#define HeavyFlavorAnalysis_Onia2MuMu_Onia2MuMuPAT_h


// system include files
#include <memory>

// FW include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/Utils/interface/PtComparator.h"

// DataFormat includes
#include <DataFormats/MuonReco/interface/Muon.h>
#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>
//
// class decleration
//

class Onia2MuMuPAT : public edm::EDProducer {
 public:
  explicit Onia2MuMuPAT(const edm::ParameterSet&);
  ~Onia2MuMuPAT();

 private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  bool selectionMuons(const reco::Muon& muon,int selectionType) const;
  bool isAbHadron(int pdgID);
  std::pair<int, float> findJpsiMCInfo(reco::GenParticleRef genJpsi);

  // ----------member data ---------------------------
 private:
  edm::InputTag muons_;
  edm::InputTag thebeamspot_;
  edm::InputTag thePVs_;
  int selectionType1_;
  int selectionType2_;
  bool addCommonVertex_;
  bool addMCTruth_;
  GreaterByPt<pat::CompositeCandidate> pTComparator_;

};

//
// constants, enums and typedefs
//


//
// static data member definitions
//

//
// constructors and destructor
//

#endif
