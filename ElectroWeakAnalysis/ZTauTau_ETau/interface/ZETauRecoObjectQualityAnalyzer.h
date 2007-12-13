
//
// Original Author:  K.A Petridis
//         Created:  Wed Jul 31 10:08:38 BST 2007
// $Id: ZETauRecoObjectQualityAnalyzer.h,v 1.1 2007/08/08 14:45:44 kap01 Exp $
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
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminatorByIsolation.h"
#include "RecoTauTag/TauTagTools/interface/PFTauElementsOperators.h"
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
#include "TProfile.h"

class ZETauRecoObjectQualityAnalyzer: public edm::EDAnalyzer {
   public:
      explicit ZETauRecoObjectQualityAnalyzer(const edm::ParameterSet&);
      ~ZETauRecoObjectQualityAnalyzer();
  
  private:
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  typedef std::vector<edm::InputTag> VInputTag;
  typedef std::vector<std::string> vstring;

      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  
  void CalculateHotHcalTowerEfficiency(TLorentzVector,edm::Handle<reco::CandidateCollection>&,
				       const reco::IsolatedTauTagInfo&,std::vector<TLorentzVector>,
				       double,double,double,double,double,bool);
  void InitializeHistograms();
  void SetupHistos();
  
  std::vector<TLorentzVector> ElectronsFromZee(edm::Handle<reco::CandidateCollection>&);
  void ElectronRejectionVariables(edm::Handle<reco::IsolatedTauTagInfoCollection>&,TLorentzVector,std::vector<TLorentzVector>,
				  double,double,double,double,double,bool,bool,const MagneticField&,const Propagator&);
  void MakeResolutionPlots(const edm::Event&, const edm::EventSetup&);
  void InialiseResolutionPlots();
  
  GlobalPoint PropagateTrackToCalo(const reco::Track&,const MagneticField&,const Propagator&);

  // ----------member data ---------------------------
  int nEvt;// used to count the number of events
  int Count,Count_0;  
  VInputTag m_mcTauProducts;
 
  vstring m_RecomoduleName;
  std::string m_Output; 
  bool m_signal,m_zee;

 
  
 // to be used for root output tree
  TFile *thefile;
  TTree *smalltree;
  
  TH2D* hOutTrPhiVsEta;
  TH1D* hOutTrMaxEdR;
  TH1D* hHhot; TH1D* hEhot;
  TH1D* hHtotOverLdgPt;TH1D* hHhotOverHtot;
  TH1D* hEtotOverLdgPt;TH1D* hEhotOverEtot;
  TH1D* hHhotOverLdgPt;TH1D* hEhotOverLdgPt;
  TH2D* hHhotOLdgPtVsLdgPt;TH2D* hEhotOLdgPtVsLdgPt;
  TH2D* hHtotOLdgPtVsLdgPt;TH2D* hEtotOLdgPtVsLdgPt;
  TH2D* hHhotOLdgPtVsHhot;TH2D* hEhotOLdgPtVsEhot;
  TH2D* hHtotOLdgPtVsHtot;TH2D* hEtotOLdgPtVsEtot;
  TH2D* hEhotOLdgPtVsHhot;
  TH2D* hHtotOLdgPtVsOutTrHhotDEta;TH2D* hHtotOLdgPtVsOutTrHhotDR;
  TH1D* hOutTrHhotDEta;TH1D* hOutTrHhotDR;
  TH1D* hOutTrHhotDRPassesDEta;

  TH1D* hVisRecoZmassRes;TH1D* hVisRecoPFZmassRes;TH1D* hVisMetRecoZmassRes;TH1D* hVisMetRecoPFZmassRes;
  TH1D* hVisRecoZmass;TH1D* hVisRecoPFZmass;TH1D* hVisMetRecoZmass;TH1D* hVisMetRecoPFZmass;
  TH1D* hMEtEtRes;TH1D* hMEtPhiRes;
  TH1D* hPFTauEtRes;TH1D* hPFTauEtaRes;TH1D* hPFTauPhiRes;
  TH1D* hTauEtRes;TH1D* hTauEtaRes;TH1D* hTauPhiRes;
  TH1D* hElecEtRes;TH1D* hElecEtaRes;TH1D* hElecPhiRes;
  TH1D* hDeltaRPFTau;
  

  TProfile*  hTauEtProf;TProfile*  hTauEtaProf;
  TProfile*  hPFTauEtProf;TProfile* hPFTauEtaProf;
  TProfile*  hElecEtProf;TProfile* hElecEtaProf;
};
  



