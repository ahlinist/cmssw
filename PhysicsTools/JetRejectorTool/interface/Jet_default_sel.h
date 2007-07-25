// Original Author:  Ilaria Villella
//         Created:  Thu 12 Apr 2007

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include <vector>
#include <fstream>

//root-classes
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF1.h"
#include "TKey.h"
#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TLegend.h"

//further neccessary classes
#include <DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h>
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/JetRejectorTool/interface/PhysicsJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"


using namespace edm;
using namespace std;
using namespace reco;

//
// class decleration
//

class Jet_default_sel : public edm::EDProducer {
   public:
      explicit Jet_default_sel(const edm::ParameterSet&);
      ~Jet_default_sel();
      void produce(edm::Event&, const edm::EventSetup&);
      
      
// std::vector<reco::CaloJet>  CaloJetSelected;	
   private:
      
      // std::vector<reco::JetTag>  JetTagSelected;
      edm::InputTag genjetsrc_;
      edm::InputTag calojetsrc_;
      edm::InputTag jettagsrc_;
      double jetEnergycut_, jetNConstMin_;
      //TFile *outputRootFile;
};
