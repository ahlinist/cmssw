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
#include <Math/VectorUtil.h>



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
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "PhysicsTools/JetRejectorTool/interface/PhysicsJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "PhysicsTools/JetRejectorTool/interface/JetRejLRObservables.h"
#include "AnalysisDataFormats/TopObjects/interface/JetRejObs.h"
//#include "PhysicsTools/JetRejectorTool/interface/JetRejObs.h"
#include "TopQuarkAnalysis/TopTools/interface/JetPartonMatching.h"

using namespace edm;
using namespace std;
using namespace reco;

//
// class decleration
//

class JetRejObsProducer : public edm::EDProducer {
   public:
      explicit JetRejObsProducer(const edm::ParameterSet&);
      ~JetRejObsProducer();
      void produce(edm::Event&, const edm::EventSetup&);
      
   
    
   private:
      // std::vector<reco::CaloJet>  CaloJetSelected;
      // std::vector<reco::JetTag>  JetTagSelected;
      edm::InputTag selgenjetsrc_;
      edm::InputTag selcalojetsrc_;
      edm::InputTag seljettagsrc_;
      double jetNumNoRad_;
      double leptonIsolation_, DeltaRcut_;
      JetRejLRObservables     *myJetRejLRObs;
      //TFile *outputRootFile;
      unsigned int switchSignalDefinition;
};
