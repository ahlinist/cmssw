#ifndef ZTauTauAnalysis_H
#define ZTauTauAnalysis_H

// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TLorentzVector.h"

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "Math/GenVector/BitReproducible.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "SimDataFormats/TrackingHit/interface/PSimHit.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"
#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "DataFormats/Common/interface/RefVector.h"
#include "TrackingTools/GeomPropagators/interface/HelixExtrapolatorToLine2Order.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/METCollection.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include "DataFormats/TauReco/interface/Tau.h"


#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <cmath>
#include "TStyle.h"

#include "HepMC/GenEvent.h"
#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"
#include "DataFormats/Common/interface/Handle.h"
#include <string>
#include <vector>
#include <set>



//
// class declaration
//

class ZTauTauAnalysis : public edm::EDAnalyzer {
public:
  explicit ZTauTauAnalysis(const edm::ParameterSet&);
  ~ZTauTauAnalysis();
  
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  
  // ----------member data ---------------------------
  int irun, ievt;
  
  
  edm::InputTag trackTags_; 
  
  string rootfile_;
  
  //  string CaloTaus_;
  string RecoMuons_;
  string PFTaus_;
  string GenMet_;
  string CaloMet_;
  
  bool skip;
  int eventCount_;
  TTree* tree_;
  TFile* file_;  
  
  TLorentzVector tauneut, tauh_ap, taul_ap , mu, neutau, neumu, jetMom;
  
  //    int nEvent;
  //    vector<float> nEventsUsed;
  //    vector<float> nEventsRiso;
  //    int nEventTaggedJets;
  
  float neumuener, neumuenertr, neutauener, neutauenertr, neutaupx;
  float neutaupy, neutaupz, neumupx, neumupy, neumupz, neutaupt, neumupt;
  float jetMomener, jetMomenertr,jetMompt, jetMometa, jetMommass, jetMommasstr, jetMompx, jetMompy, jetMompz, jetMomtheta;
  float muener, muenertr,mupt, mueta, mumass, mumasstr, mupx, mupy, mupz, mutheta;
  //float Zpt, 
float Z_eta;
  float Z_rap, Z_rapidity;
  
  //float Zener,
  //float Zenertr; 
  //Zpz, Zpx, Zpy, 
  float tauhener_ap, tauhenertr_ap, tauhpt_ap, tauheta_ap, tauhmass_ap, tauhmasstr_ap;
  float taulener_ap, taulenertr_ap, taulpt_ap, tauleta_ap, taulmass_ap, taulmasstr_ap;
  float tauneutener, tauneutenertr, tauneutpx, tauneutpy, tauneutpz, tauneutpt;
  
  float pxtaul_bp, pytaul_bp,pztaul_bp, pxtauh_bp, pytauh_bp, pztauh_bp, etaul_bp, etauh_bp; 
  //Zmass_bp, 
  //float Zmass_ap, 
  float pxtaul_ap, pytaul_ap,pztaul_ap, pxtauh_ap, pytauh_ap, pztauh_ap, etaul_ap, etauh_ap;
  //float Zmass_vis, 
  //float Zmasstr_vis,Zmasslongit_vis,  
  //float Zmasstr_all;  
  //Zmass_all, 
  
  //float ZmassTLV, 
  //float ZmasstrTLV;
  float Zeta_fin_px, Zeta_fin_py, Zeta_fin_pz, Zeta_fin_pt;
  float Zeta_fin_ener, Zeta_fin_enertr, Zeta_fin_eta, Zeta_fin_rapidity;
  // Zeta_fin_masstrTLV;
    //Zeta_fin_massTLV, 
  float ptleadingtrack, ptparticle;
  int barcodemulept;
  int barcodemujet;
  int countchpartjet;
  float charged;
  
  float energiatrasvZ;
  
  //float Zeta_fin_masstr_relat; 
    //Zeta_masstr_relat, Zmasstr_all_relat;
  //float Zvisibletransversemass_relat1, 
float Zvisibletransversemass_relat2  ;
  //float trmass1, trmass2, trmass3, trmass4, trmass5, 
  //float trmass6;
  float trvismass1; 
    //trvismass4, trvismass5, trvismass6; 
  //trvismass2, trvismass3, 
  float tauh_aptheta, taul_aptheta;
  //float leptpx, leptpy, leptpz, leptener, leptenertr;
  //float energiaZ, energiatrZ, pxZ, pyZ, pzZ, ptZ;
  
  //float adrpx, adrpy, adrpz, adrener, adrenertr;
  
  float CaloRecoP_TTau, CaloRecoP_TLeadTrk, CaloRecoInvMassTau, CaloRecoNumTrks, CaloRecoEmEnergyFraction, CaloRecoMaxHadronEnergy, CaloRecoNumSignalTrks,CaloRecoNumIsolationTrks, CaloRecoSumpTIsolationTracks, CaloRecoSumE_TIsolationGammaCandidates, CalosignalgammacandidatesSize;
  
  float CaloRecoEnergyTau, CaloRecoEtaTau, CaloRecoTrEnergyTau, CaloRecoTrMassTau;
  
  float RecoP_TMuon, RecoP_TMuonFrompxpy, RecoEnergyMuon, RecoTrEnergyMuon, RecoInvMassMuon, RecoEtaMuon, RecoTrMassMuon, RecoP_XMuon, RecoP_YMuon, RecoP_ZMuon;
  
  float CaloRecoVisMassZ, CaloRecoInvMassZ, CaloRecoP_TZ;
  float CaloRecoP_XTau, CaloRecoP_YTau, CaloRecoP_ZTau;
  
  float PFRecoP_TTau, PFRecoInvMassTau, PFRecoEmEnergyFraction, PFRecoMaxHadronEnergy,PFRecoNumChargHadrCand, PFRecoNumNeutHadrCand, PFRecoNumGammaCand, PFRecoSignChargHadr, PFRecoNumIsolChargHadr, PFRecoIsolGammaCand, PFRecoSumpTIsolationTracks, PFRecoSumE_TIsolationGammaCandidates, PFsignalgammacandidatesSize,PFRecoP_TLeadTrk,PFRecoSignalGammaCandSize;
  
  float PFRecoEnergyTau, PFRecoEtaTau, PFRecoTrEnergyTau, PFRecoTrMassTau;
  
  float PFRecoVisMassZ, PFRecoInvMassZ_A, PFRecoInvMassZ_B,  PFRecoInvMassZ_C, PFRecoP_TZ, PFRecoEnergyZ, PFRecoTrEnergyZ, PFRecoTrVisEnergyZ,PFRecoVisEnergyZ, PFRecoP_XZ, PFRecoP_YZ, PFRecoP_ZZ, PFRecoP_Z, PFRecoTrMassZ_A, PFRecoTrMassZ_B,PFRecoTrMassZ_C, PFRecoTrVisMassZ,PFRecoTrVisMassZ_rel ;
 
  float  PFRecoVisP_XZ,PFRecoVisP_YZ, PFRecoVisP_ZZ, PFRecoVisP_Z;
  float PFRecoP_XTau, PFRecoP_YTau, PFRecoP_ZTau;
  
  float GenMETMissTrEner, GenMETSumMissTrEner, GenMETSignificance, GenMET_E_long;
  float CaloMETMissTrEner, CaloMETSumMissTrEner, CaloMETSignificance, CaloMET_E_long, CaloMET_x, CaloMET_y, CaloMET_time;
  
  //float ResPFRecoVisMassZ,ResPFRecoEnergyZ; 
  //float ResPFRecoP_TZ,  ResPFRecoP_XZ, ResPFRecoP_YZ, ResPFRecoP_ZZ;
  
  float Zmasstr_all_A; 
    //Zmasstr_all_B;
  // Zeta_fin_masstr_relat_A;
    //Zeta_fin_masstr_relat_B;
  float GenEnergyZ, GenTrEnergyZ,GenVisTrEnergyZ,GenP_XZ, GenP_YZ, GenP_ZZ, GenP_TZ, GenInvMassZ, GenTrMassZ_rel, GenTrMassZ_A, GenVisMassZ;
    //GenVisTrMassZ_B, 
float GenVisTrMassZ_A,GenVisTrMassZ_rel,energiatrasvvisZ, GenVisEnergyZ;   
  //GenTrMassZ_B, 

 

edm::ESHandle<MagneticField> theMF;
  GlobalVector magField; 
  
};

#endif
