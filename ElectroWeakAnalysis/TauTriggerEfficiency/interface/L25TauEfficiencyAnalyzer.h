// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.h,v 1.7 2008/10/22 11:28:17 smaruyam Exp $
#include <memory>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TFile.h"
#include "TTree.h"
class L25TauEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L25TauEfficiencyAnalyzer();
      explicit L25TauEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L25TauEfficiencyAnalyzer();
      void Setup(const edm::ParameterSet&,TTree*);
      void fill(const edm::Event&,const reco::PFTau&);   
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      edm::InputTag   tauSource;
      edm::InputTag l25JetSource;
      edm::InputTag l25PtCutSource;
      edm::InputTag l25IsoSource;
      edm::InputTag outputFileName;
      std::string rootFile_;
      TFile* l25file;
      TTree* l25tree;
      float tauPt;
      float tauEt;
      float tauEta;
      float tauPhi;
      float tauTjDR;
      float tauTrkSig;
      float l25Et;
      float l25Phi;
      float l25Eta;
      float l25Pt;
      float l25TjDR;
      float l25TrkQPx;
      int   l25Depth;
      int   leadDepth1;
      int   leadDepth2;
      float minDR;
      float bareEt;
      double matchingCone;
};
