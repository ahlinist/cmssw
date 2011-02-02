// Class:      L25and3TauEfficiencyAnalyzer
// Original Author:  Eduardo Luiggi, modified by Sho Maruyama
//         Created:  Fri Apr  4 16:37:44 CDT 2008
// $Id: L25and3TauEfficiencyAnalyzer.h,v 1.6 2011/01/17 23:31:08 eluiggi Exp $
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
//#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauTagInfo.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TFile.h"
#include "TTree.h"

typedef math::XYZTLorentzVector LorentzVector;

class L25and3TauEfficiencyAnalyzer : public edm::EDAnalyzer {
   public:
      explicit L25and3TauEfficiencyAnalyzer();
      explicit L25and3TauEfficiencyAnalyzer(const edm::ParameterSet&);
      ~L25and3TauEfficiencyAnalyzer();
void Setup(const edm::ParameterSet&,TTree*);
virtual void fill(const edm::Event&, const LorentzVector&);
virtual void fill(const edm::Event&, const reco::Candidate&);
virtual void fill(const edm::Event&, const reco::Particle&);

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      edm::InputTag l25JetSource;
      edm::InputTag l25PtCutSource;
      edm::InputTag l3IsoSource;
      float l25Et;
      float l25Phi;
      float l25Eta;
      float l25Pt;
      char l25Depth;
      char leadDepth1;
      char leadDepth2;
      float minDR;
      float bareEt;
      float l25InvPt;
      float l25IsoPtSum;
      int l25NTrksIso;
      float l25IsoTrkChi2;
      float l25IsoTrkPt;
      float l25TrkIsoPtSum;
      float l25EcalIsoEtSum;
      unsigned int l25IsoTrkNHits;
     
      double l25MatchingCone;
      bool HLTPFTau;

      float l25PtLdgJetDRLoose;
      float l25EtaLdgLoose;
      float l25PhiLdgLoose;
      float l25PtLdgLoose;
      
      bool l25DefDisc_Trk5_IsoPtMin1_Ntrk0;
      bool l25DefDisc_Trk5_IsoPtMin1_Ntrk1;
      bool l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0;
      bool l25DefDisc_Trk5_IsoPtMin1_5_Ntrk1;
      bool l25DefDisc_Trk5_IsoPtMin2_Ntrk0;

};
