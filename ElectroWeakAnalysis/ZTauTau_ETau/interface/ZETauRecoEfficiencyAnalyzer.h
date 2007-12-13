
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


class ZETauRecoEfficiencyAnalyzer: public edm::EDAnalyzer {
   public:
      explicit ZETauRecoEfficiencyAnalyzer(const edm::ParameterSet&);
      ~ZETauRecoEfficiencyAnalyzer();
  
  private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;
  typedef std::vector<std::string> vstring;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  void VariableIsoDiscriminator(const edm::Event&,TLorentzVector,double,double,
				double,double,double,bool,const MagneticField&,const Propagator&);
  
  void InitializeHistograms();
  void CalculateIsolationEfficiency();
  
  int CountNumberOfJets(edm::Handle<reco::CandidateCollection>&,double,double,
			TLorentzVector,TLorentzVector);
  bool MatchesElectron(edm::Handle<reco::CandidateCollection>&,TLorentzVector);
  bool MatchesMcTauJet(TLorentzVector,TLorentzVector,bool);
  double HottestHcalTower(IsolatedTauTagInfo&,const MagneticField&,const Propagator&);
  void FillVariableIsoHistos(IsolatedTauTagInfo&,double,double,double,double,bool);
  void FillVariablePFIsoHistos(PFIsolatedTauTagInfo&,double,double,double,double,bool);
  std::vector<double> CollinearMass(TLorentzVector,TLorentzVector,TLorentzVector);  
  // ----------member data ---------------------------
  int nEvt;// used to count the number of events
  int Count;  
  VInputTag m_mcTauProducts;
  VInputTag m_zETauRecoModules;
  VInputTag m_zETauRecoModulesTau;
  VInputTag m_zETauRecoModulesLep;
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
  TH1D* hRecoVisZmass;
  TH1D* hRecoVisZmass_new;
  TH1D* hRecoCollZmass;
  TH1D* heMET;
  TH1D* heMETDphi;
  TH1D* hSSOScharge;
  TH1D* hnJets;
  TH1D* hdRElecTau;
  TH1D* hHotHcalTower;
  TH1D* hHotHcalTower1;
  TH1D* hHotHcalTowerMatchedToRecoElec;
  TH1D* hHotHcalTowerMatchedToRecoElec1;	
  TH1D* hHotHcalTowerMatchedToMcTau;
  TH1D* hEtaDistribution;
  TH2D* hHotHcalVsEta;
  TH2D* hNtauVsNelec;

  TH1D* hRecoZeeMass;
  TH1D* hRecoZeeMass1;
  TH1D* hRecoZeeMass2;

  TH1D* recoEffwrtHLTe;

  TH1D* recoEffwrtTotal;
  TH1D* recoPurwrtTotal;
  
  TH1D* triggEffVsRiso;TH1D* triggEffVsRsig;TH1D* triggEffVsPtLtr;
  
  TH2D* triggEffVsRisoVsRsig;TH2D* triggEffVsRsigVsPtLtr;TH2D* triggEffVsPtLtrVsRiso;
  
  TH1D* allRiso;TH1D* allRsig;TH1D* allPtLtr;
  
  TH2D* allRisoVsRsig;TH2D* allRsigVsPtLtr;TH2D* allPtLtrVsRiso;

  TH1D* triggRiso;TH1D* triggRsig;TH1D* triggPtLtr;
  
  TH2D* triggRisoVsRsig;TH2D* triggRsigVsPtLtr;TH2D* triggPtLtrVsRiso;



  TH1D* triggPFEffVsRiso;TH1D* triggPFEffVsRsig;TH1D* triggPFEffVsPtLtr;
  
  TH2D* triggPFEffVsRisoVsRsig;TH2D* triggPFEffVsRsigVsPtLtr;TH2D* triggPFEffVsPtLtrVsRiso;
  
  TH1D* allPFRiso;TH1D* allPFRsig;TH1D* allPFPtLtr;
  
  TH2D* allPFRisoVsRsig;TH2D* allPFRsigVsPtLtr;TH2D* allPFPtLtrVsRiso;

  TH1D* triggPFRiso;TH1D* triggPFRsig;TH1D* triggPFPtLtr;
  
  TH2D* triggPFRisoVsRsig;TH2D* triggPFRsigVsPtLtr;TH2D* triggPFPtLtrVsRiso;

 
};
  



