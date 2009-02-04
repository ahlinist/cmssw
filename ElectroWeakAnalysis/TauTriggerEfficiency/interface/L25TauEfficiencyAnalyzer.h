// Class:      L25TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25TauEfficiencyAnalyzer.h,v 1.12 2008/12/12 15:42:55 gfball Exp $
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
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TFile.h"
#include "TTree.h"

typedef math::XYZTLorentzVector LorentzVector;

class L25TauEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L25TauEfficiencyAnalyzer();
      explicit L25TauEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L25TauEfficiencyAnalyzer();
void Setup(const edm::ParameterSet&,TTree*);
virtual void fill(const edm::Event&, const reco::PFTau&);   
virtual void fill(const edm::Event&, const reco::CaloTau&);
virtual void fill(const edm::Event&, const LorentzVector&);

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      edm::InputTag   l25tauSource;
      edm::InputTag l25JetSource;
      edm::InputTag l25PtCutSource;
      edm::InputTag l25IsoSource;
      float l25Et;
      float l25Phi;
      float l25Eta;
      float l25Pt;
      int   l25Depth;
      double l25MatchingCone;
};
