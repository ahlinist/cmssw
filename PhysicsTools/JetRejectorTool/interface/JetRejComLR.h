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
#include "TopQuarkAnalysis/TopTools/interface/LRHelpFunctions.h"

using namespace edm;
using namespace std;
using namespace reco;

//
// class decleration
//

class JetRejComLR : public edm::EDProducer {
   public:
      explicit JetRejComLR(const edm::ParameterSet&);
      ~JetRejComLR();
      void produce(edm::Event&, const edm::EventSetup&);
      
   
    
   private:
      // std::vector<reco::CaloJet>  CaloJetSelected;
      // std::vector<reco::JetTag>  JetTagSelected;
      edm::InputTag obssrc_;

      double  DeltaRcut_;
      vector< int > JetSelObs_;
      string lrJetCombFile_;
      /*     int   nrJetCombLRtotBins_;
      double JetCombLRtotMin_;
      double JetCombLRtotMax_ ;
      string JetCombLRtotFitFunction_;
      */
      int     nrJetCombLRtotBins;
      double JetCombLRtotMin;
      double JetCombLRtotMax;
      char*  JetCombLRtotFitFunction ;
      
      TString  JetCombOutFileName ;
      TString  JetCombPSfile ;
      vector<int> obsNrs;   
      LRHelpFunctions *myLRhelper; 

      //TFile *outputRootFile;
};
