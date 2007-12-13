
//
// Original Author:  K.A Petridis
//         Created:  Wed Jul 31 10:08:38 BST 2007
// $Id: ZETauRecoEfficiencyAnalyzer.h,v 1.1 2007/08/08 14:45:44 kap01 Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "FWCore/Framework/interface/ESHandle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"


#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaReco/interface/ElectronPixelSeed.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/BTauReco/interface/PFIsolatedTauTagInfo.h"
#include <DataFormats/Common/interface/RefVector.h>
#include <DataFormats/Common/interface/RefVectorIterator.h>
#include <DataFormats/Common/interface/Ref.h>
#include "DataFormats/Common/interface/RefToBase.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryParametrization/interface/GlobalTrajectoryParameters.h"
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "DataFormats/GeometrySurface/interface/Plane.h"

#include "DataFormats/Math/interface/LorentzVector.h"

// basic cpp:
#include <iostream>
#include <string>
#include <vector>

// some root includes
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include "TClonesArray.h"
#include "TVectorF.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"

class ZETauRecoTauTaggingEfficiencyCalculator: public edm::EDAnalyzer {
   public:
      explicit ZETauRecoTauTaggingEfficiencyCalculator(const edm::ParameterSet&);
      ~ZETauRecoTauTaggingEfficiencyCalculator();
  
  private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;
  typedef std::vector<std::string> vstring;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  
  
  void InitializeHistograms();
  
  
  int CountNumberOfJets(edm::Handle<reco::CandidateCollection>&,double,double,
			TLorentzVector,TLorentzVector);
  
  std::vector<double> CollinearMass(TLorentzVector,TLorentzVector,TLorentzVector);  
  // ----------member data ---------------------------
  int nEvt;// used to count the number of events
  int Count;  
  VInputTag m_mcTauProducts;
  std::string m_MassProducer;
  vstring m_RecomoduleName;
  std::string m_Output; 
  bool m_signal,m_zee;

  int m_module[20],m_module_match[20];
  int nPaths;
  
 // to be used for root output tree
  TFile *thefile;
  TTree *smalltree;
  
  TH1D* Zmass;
  TH1D* hRecoVisZmass;TH1D* hRecoVisZmass_1;TH1D* hRecoVisZmass_2;
  TH1D* hRecoCollZmass;TH1D* hRecoCollZmass_1;TH1D* hRecoCollZmass_2;
  TH1D* heMET;
  TH1D* hnJets;
  TH1D* hTauPassingEt;TH1D* hTauEt;
  TH1D* hTauPassingEta;TH1D* hTauEta;
  TH1D* hTauAllPassingEt;TH1D* hTauAllEt;TH1D* hTauEtMatching;TH1D* hTauAllEtMatching;
  TH1D* hTauEtaMatching;
  TH1D* hTauAllPassingEta;TH1D* hTauAllEta;
  TH1D* hnTauJets;TH1D* hnTauJets_1;TH1D* hnTauJets_2;
  TH1D* hDeltaRJetElec;TH1D* hDeltaRJetElec_1;
  TH1D* hPtLdgTrack;TH1D* hPtLdgTrack_1;
  TH1D* hSSOScharge;TH1D* hSSOScharge_1;
  TH1D* hJetEt;TH1D* hJetEt_1;
  TH1D* hJetEt_2;TH1D* hJetEt_3;
  TH1D* hJetEt_4;TH1D* hJetEt_5;
  TH1D* hPtLtrRel;TH1D* hPtLtrRel_1;

  TGraphAsymmErrors* hEffEt;TGraphAsymmErrors* hEffEta;
  TGraphAsymmErrors* hEffAllEt;TGraphAsymmErrors* hEffAllEta;
};
  



