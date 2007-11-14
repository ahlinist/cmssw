#ifndef Analyzer_h
#define Analyzer_h
/*
Author:
  Sunil Bansal ( Panjab University Chandigarh)

*/
// system include files
  #include <memory>
#include <iostream>
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/MuIsoDeposit.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectronFwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJetfwd.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/EcalCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/JetReco/interface/CaloJetfwd.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/GenMETFwd.h"
#include "DataFormats/METReco/interface/SpecificGenMETData.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/MuonReco/interface/MuIsoDepositFwd.h"
#include "RecoMuon/MuonIsolationProducers/src/MuIsolationSumInConesProducer.h"
#include "RecoEgamma/EgammaHLTAlgos/interface/EgammaHLTEcalIsolation.h"
#include "RecoEgamma/EgammaHLTAlgos/interface/EgammaHLTHcalIsolation.h"
#include "RecoEgamma/EgammaHLTAlgos/interface/EgammaHLTTrackIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaEcalIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHcalIsolation.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/ElectronTkIsolation.h"
#include  "DataFormats/EgammaCandidates/interface/ElectronIsolationAssociation.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "FWCore/Framework/interface/ESHandle.h"
//#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
//#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
//*****Btagging file************************************
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

 // essentials !!!
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TTree.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <vector>
 using namespace edm;
 using namespace std;
 using namespace reco;
#define PI 3.141592654
#define TWOPI 6.283185308

class PtSorter {
public:
  template <class T> bool operator() ( const T& a, const T& b ) {
    return ( a.pt() > b.pt() );
  }
};
//************************************** class decleration************************//

class Analyzer : public edm::EDAnalyzer {
   public:
      explicit Analyzer(const edm::ParameterSet&);
      ~Analyzer();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------



TFile  *hOutputFile;
string fOutputFileName;
TTree * JetMet;

int  nEvt;

double dR, recphi1, recphi2, correcphi1, correcphi2, dijetmass, etadifference, deltaphi, NV, corNV, cordijetmass, coretadifference, cordeltaphi, delpx, delpy, delsumet;
  //***********MET Related********************************
  double Rec_MET, Cor_MET, Gen_MET, Rec_MET_phi, Gen_MET_phi, Cor_MET_phi, Rec_Scal_Et, Cor_Scal_Et, Cor_MuMET, Cor_MuMETx, Cor_MuMETy;

  //***********Muons Related*******************************
int MUONMAX, nMuonCand;

double Muon_pt[15], Muon_eta[15], Muon_charge[15], Muon_px[15], Muon_py[15], Muon_pz[15], Muon_p[15], Muon_phi[15], Muon_theta[15],Muonvtx[15], Muonvty[15], Muonvtz[15], MuonIP[15], MuonDz[15];

double EcalM_Tot[15], HcalM_Tot[15], HoM_Tot[15], TrkM_Tot[15];

double EcalM_Frac[15][5], HcalM_Frac[15][5], HoM_Frac[15][5], TrkM_Frac[15][5];

int TrkIsoM_N[15][5];
  //***********Electrons Related****************************
int ElectronMax, nElectronCand;

int  TrkIsoE_N[15][5][5];

double TrkIsoE_Pt[15][5][5],EcalIsoE_Pt[15][5],HcalIsoE_Pt[15][5][5];

double Electron_pt[15], Electron_eta[15], Electron_charge[15],Electron_phi[15],Electron_px[15], Electron_py[15], Electron_pz[15], Electron_p[15], Electron_theta[15], Electron_HDovrEM[15],Electronvtx[15], Electronvty[15], Electronvtz[15], ElectronIP[15], ElectronDz[15];
int Electron_ID[15];
  //******Jets Related****************************************
int JETMAX, Cor_Jet_size, Rec_Jet_size, Gen_Jet_size;

double Rec_Jet_pt[15], Rec_Jet_eta[15], Rec_Jet_phi[15], Rec_Jet_px[15], Rec_Jet_py[15], Rec_Jet_pz[15], Rec_Jet_p[15], Rec_Jet_et[15], Rec_Jet_HDfrac[15], Rec_Jet_EMfrac[15];

double Gen_Jet_pt[15], Gen_Jet_eta[15], Gen_Jet_phi[15], Gen_Jet_px[15], Gen_Jet_py[15], Gen_Jet_pz[15], Gen_Jet_p[15];

double Cor_Jet_pt[15],Cor_Jet_eta[15],Cor_Jet_phi[15],Cor_Jet_px[15],Cor_Jet_py[15],Cor_Jet_pz[15],Cor_Jet_p[15],Cor_Jet_et[15], Cor_Jet_HDfrac[15], Cor_Jet_EMfrac[15];

double btagdisc[200];
int nevents, bjets,BMAX;

edm::InputTag theECALIsoDepositLabel;
edm::InputTag theHCALIsoDepositLabel;
edm::InputTag theHOCALIsoDepositLabel;
edm::InputTag theTrackerIsoDepositLabel;
edm::InputTag muon_;
edm::InputTag elec_;
edm::InputTag ctfTrk_;
edm::InputTag eBC_;
edm::InputTag eSC_;
edm::InputTag hbhe_;
std::string electronProducer_;
std::string electronIDAssocProducer_;
};

#endif
