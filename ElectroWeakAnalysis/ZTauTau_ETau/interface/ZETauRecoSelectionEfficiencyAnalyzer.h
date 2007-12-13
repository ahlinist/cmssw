
//
// Original Author:  K.A Petridis
//         Created:  Wed Jul 31 10:08:38 BST 2007
// $Id: ZETauRecoSelectionEfficiencyAnalyzer.h,v 1.1 2007/08/08 14:45:44 kap01 Exp $
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
#include "TString.h"

class ZETauRecoSelectionEfficiencyAnalyzer: public edm::EDAnalyzer {
   public:
      explicit ZETauRecoSelectionEfficiencyAnalyzer(const edm::ParameterSet&);
      ~ZETauRecoSelectionEfficiencyAnalyzer();
  
  private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;
  typedef std::vector<std::string> vstring;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  
  void CalculateObjectEfficiencies(const edm::Event&,TLorentzVector,TLorentzVector);
  bool FillPlotsForEfficiencyAndPurityCalc(Handle<LorentzVectorCollection>&,TLorentzVector,vector<TH1D*>&,double,bool);
  void MakeEfficiencyPurityHistos();
  void InitialiseEfficiencyAndPurityPlots();
  // ----------member data ---------------------------
  int nEvt;// used to count the number of events
  VInputTag m_mcTauProducts;
  std::string m_Output; 
  bool m_do_pureff;  
  // to be used for root output tree
  TFile *thefile;
  TTree *smalltree;
  
  
  std::vector<TH1D*> hltElectronHistVec,gsfElectronHistVec,gsfEtCutElectronHistVec,
    allTauJetHistVec,cleanTauJetHistVec,isoTauJetHistVec,prongTauJetHistVec,
    hltElectronEffHistVec,gsfElectronEffHistVec,gsfEtCutElectronEffHistVec,
    allTauJetEffHistVec,cleanTauJetEffHistVec,isoTauJetEffHistVec,prongTauJetEffHistVec;

 std::vector<TString> hltElectronStringVec,gsfElectronStringVec,gsfEtCutElectronStringVec,
    allTauJetStringVec,cleanTauJetStringVec,isoTauJetStringVec,prongTauJetStringVec,
    hltElectronEffStringVec,gsfElectronEffStringVec,gsfEtCutElectronEffStringVec,
    allTauJetEffStringVec,cleanTauJetEffStringVec,isoTauJetEffStringVec,prongTauJetEffStringVec;


  TH1D* hAllElecEt;TH1D* hAllElecEta;TH1D* hAllElecPhi;
  TH1D* hAllElecEt_Fid;TH1D* hAllElecEta_Fid;TH1D* hAllElecPhi_Fid;
  TH1D* hAllTauEt;TH1D* hAllTauEta;TH1D* hAllTauPhi;

  TH1D* hltElecEt;TH1D* hltElecEta;TH1D* hltElecPhi;
  TH1D* hltMatchElecEt;TH1D* hltMatchElecEta;TH1D* hltMatchElecPhi;
  TH1D* gsfElecEt;TH1D* gsfElecEta;TH1D* gsfElecPhi;
  TH1D* gsfMatchElecEt;TH1D* gsfMatchElecEta;TH1D* gsfMatchElecPhi;
  
  TH1D* gsfEtCutElecEt;TH1D* gsfEtCutElecEta;TH1D* gsfEtCutElecPhi;
  TH1D* gsfMatchEtCutElecEt;TH1D* gsfMatchEtCutElecEta;TH1D* gsfMatchEtCutElecPhi;


  TH1D* allTauJetEt;TH1D* allTauJetEta;TH1D* allTauJetPhi;
  TH1D* allMatchTauJetEt;TH1D* allMatchTauJetEta;TH1D* allMatchTauJetPhi;
  TH1D* cleanTauJetEt;TH1D* cleanTauJetEta;TH1D* cleanTauJetPhi;
  TH1D* cleanMatchTauJetEt;TH1D* cleanMatchTauJetEta;TH1D* cleanMatchTauJetPhi;
  TH1D* isoTauJetEt;TH1D* isoTauJetEta;TH1D* isoTauJetPhi;
  TH1D* isoMatchTauJetEt;TH1D* isoMatchTauJetEta;TH1D* isoMatchTauJetPhi;
  TH1D* prongTauJetEt;TH1D* prongTauJetEta;TH1D* prongTauJetPhi;
  TH1D* prongMatchTauJetEt;TH1D* prongMatchTauJetEta;TH1D* prongMatchTauJetPhi;

  TH1D* hltElecEtEff;TH1D* hltElecEtaEff;TH1D* hltElecPhiEff;
  TH1D* hltMatchElecEtEff;TH1D* hltMatchElecEtaEff;TH1D* hltMatchElecPhiEff;
  TH1D* gsfElecEtEff;TH1D* gsfElecEtaEff;TH1D* gsfElecPhiEff;
  TH1D* gsfMatchElecEtEff;TH1D* gsfMatchElecEtaEff;TH1D* gsfMatchElecPhiEff;
  TH1D* gsfEtCutElecEtEff;TH1D* gsfEtCutElecEtaEff;TH1D* gsfEtCutElecPhiEff;
  TH1D* gsfMatchEtCutElecEtEff;TH1D* gsfMatchEtCutElecEtaEff;TH1D* gsfMatchEtCutElecPhiEff;  

  TH1D* allTauJetEtEff;TH1D* allTauJetEtaEff;TH1D* allTauJetPhiEff;
  TH1D* allMatchTauJetEtEff;TH1D* allMatchTauJetEtaEff;TH1D* allMatchTauJetPhiEff;
  TH1D* cleanTauJetEtEff;TH1D* cleanTauJetEtaEff;TH1D* cleanTauJetPhiEff;
  TH1D* cleanMatchTauJetEtEff;TH1D* cleanMatchTauJetEtaEff;TH1D* cleanMatchTauJetPhiEff;
  TH1D* isoTauJetEtEff;TH1D* isoTauJetEtaEff;TH1D* isoTauJetPhiEff;
  TH1D* isoMatchTauJetEtEff;TH1D* isoMatchTauJetEtaEff;TH1D* isoMatchTauJetPhiEff;
  TH1D* prongTauJetEtEff;TH1D* prongTauJetEtaEff;TH1D* prongTauJetPhiEff;
  TH1D* prongMatchTauJetEtEff;TH1D* prongMatchTauJetEtaEff;TH1D* prongMatchTauJetPhiEff;
	
  TH1D* hltDRMatching;TH1D* gsfDRMatching;

};
  



