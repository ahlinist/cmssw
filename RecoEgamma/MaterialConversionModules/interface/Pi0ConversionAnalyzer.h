#ifndef Pi0ConversionAnalyzer_h
#define Pi0ConversionAnalyzer_h

// -*- C++ -*-
//
// Package:    MaterialConversionModules
// Class:  Pi0ConversionAnalyzer
// 
/**\class Pi0ConversionAnalyzer Pi0ConversionAnalyzer.cc 
 
  Description: <one line class summary>
 
  Implementation:
      <Notes on implementation>
 */
 //
 // Original Author:  Andrew Askew
//
//
 
 // system include files
#include <memory>
#include<string>
#include "math.h"
 
// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Handle.h"
 
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/HepMCCandidate/interface/HepMCCandidate.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h" 
#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversion.h"
#include "TH1.h"
#include "TFile.h"
#include "TTree.h"
 //
 // class declaration
 //
 
class Pi0ConversionAnalyzer : public edm::EDAnalyzer {
  public:
  explicit Pi0ConversionAnalyzer(const edm::ParameterSet&);
  ~Pi0ConversionAnalyzer();
 
 
  private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
   // ----------member data ---------------------------
  float pimass(float eta_g, float phi_g, float e_g, float eta_e, float phi_e, float e_e, float eta_p, float phi_p, float e_p);
   double minElePt_;
   double REleCut_;
   TFile*  rootFile_;  
   std::string outputFile_;
   edm::InputTag electronProducer_;
   std::string basicclusterCollection_;
   std::string basicclusterProducer_;   
   std::string mcProducer_;
   edm::InputTag scProducer_;
   float GetSep(float phi1, float eta1, float phi2, float eta2);

   //   TH1F* h1_recoEleDeltaPhi_;
  
   edm::InputTag matchedStripRecHitsInputTag_;
   edm::InputTag rphiStripRecHitsInputTag_;
   edm::InputTag stereoStripRecHitsInputTag_;
   edm::InputTag pixelRecHitsInputTag_;
   std::string Pi0MaterialConversionCollection_;
   std::string Pi0MaterialConversionProducer_;
   std::string outputfile_;
   std::string src_;
   TTree *tree;
   Int_t NumberofPi0;
   Float_t Pi0eta[50000];
   Float_t Pi0phi[50000];
   Float_t Pi0pt[50000];
   Int_t NumberofEta;
   Float_t Etaeta[50000];
   Float_t Etaphi[50000];
   Float_t Etapt[50000];

   Int_t PhotonConverted;
   Int_t NumberofConversions;
   Int_t NumberofPhotons;
   Float_t ConversionMCX[50000];
   Float_t ConversionMCY[50000];
   Float_t ConversionMCZ[50000];
   Float_t ConversionMCR[50000];
   Float_t ConversionMCPhi[50000];
   Float_t Electron1Px[50000];
   Float_t Electron1Py[50000];
   Float_t Electron1Pz[50000];
   Float_t Electron1Pt[50000];
   Float_t Electron1Phi[50000];
   Float_t Electron1Eta[50000];

   Float_t Electron2Px[50000];
   Float_t Electron2Py[50000];
   Float_t Electron2Pz[50000];
   Float_t Electron2Pt[50000];
   Float_t Electron2Phi[50000];
   Float_t Electron2Eta[50000];
 
   Float_t PhotonPx1[50000];
   Float_t PhotonPy1[50000];
   Float_t PhotonPz1[50000];
   Float_t PhotonPt1[50000];
   Float_t PhotonEta1[50000];
   Float_t PhotonPhi1[50000];

   Int_t PhotonVtx1[50000];
   Float_t PhotonVtx1R[50000];

   Float_t PhotonPx2[50000];
   Float_t PhotonPy2[50000];
   Float_t PhotonPz2[50000];
   Float_t PhotonPt2[50000];
   Float_t PhotonEta2[50000];
   Float_t PhotonPhi2[50000];

   Int_t PhotonVtx2[50000];
   Float_t PhotonVtx2R[50000];


   Float_t PhoMass[50000];

   Int_t Candidate;
   Int_t NumberofCandidates;
   Float_t CandidateREst[100];
   Float_t CandidatePhiEst[100];
   Int_t CandidateVertex[100];
   Float_t CandidateVertexX[100];
   Float_t CandidateVertexY[100];
   Float_t CandidateVertexZ[100];
   Float_t CandidateVertexR[100];
   Float_t CandidateVertexChi2[100];
   Float_t CandidateVertexPhi[100];
   Float_t CandidateMass[100];
   Int_t CandidateStub1Char[100];
   Int_t CandidateStub2Char[100];
   Float_t CandidateIso[100];
   Float_t CandidateHCalE[100];

   Int_t CandidateEleTrackA[100];
   Float_t CandidateEleTrackpTA[100];
   Float_t CandidateEleTrackpTFA[100];
   Float_t CandidateEleTrackPhiFA[100];
   Float_t CandidateEleTrackEtaFA[100];
   Float_t CandidateEleTrackSmallRA[100];
   Float_t CandidateEleEnergyA[100];
   Float_t CandidateEleETA[100];
   Float_t CandidateElePhiA[100];
   Float_t CandidateEleEtaA[100];
   Float_t CandidateEleE1maxA[100];
   Float_t CandidateEleE12ndA[100];
   Float_t CandidateEle2x2A[100];
   Float_t CandidateEle3x3A[100];
   Float_t CandidateEleEtaEtaA[100];
   Float_t CandidateEleEtaPhiA[100];
   Float_t CandidateElePhiPhiA[100];
   Int_t CandidateEleHitsA[100];
   Int_t CandidateEleTrackChargeA[100];

   Int_t CandidateEleTrackB[100];
   Float_t CandidateEleTrackpTB[100];
   Float_t CandidateEleTrackpTFB[100];
   Float_t CandidateEleTrackPhiFB[100];
   Float_t CandidateEleTrackEtaFB[100];
   Float_t CandidateEleTrackSmallRB[100];
   Float_t CandidateEleEnergyB[100];
   Float_t CandidateEleETB[100];
   Float_t CandidateElePhiB[100];
   Float_t CandidateEleEtaB[100];

   Float_t CandidateEleE1maxB[100];
   Float_t CandidateEleE12ndB[100];
   Float_t CandidateEle2x2B[100];
   Float_t CandidateEle3x3B[100];
   Float_t CandidateEleEtaEtaB[100];
   Float_t CandidateEleEtaPhiB[100];
   Float_t CandidateElePhiPhiB[100];

   Int_t CandidateEleHitsB[100];
   Int_t CandidateEleTrackChargeB[100];

   Float_t CandidatePhoEnergy[100];
   Float_t CandidatePhoET[100];
   Float_t CandidatePhoPhi[100];
   Float_t CandidatePhoEta[100];
   Float_t CandidatePhoE1max[100];
   Float_t CandidatePhoE12nd[100];
   Float_t CandidatePho2x2[100];
   Float_t CandidatePho3x3[100];
   Float_t CandidatePhoEtaEta[100];
   Float_t CandidatePhoEtaPhi[100];
   Float_t CandidatePhoPhiPhi[100];

   Int_t GamAssign[100];
   Int_t EAAssign[100];
   Int_t EBAssign[100];

   Int_t TotalHits;
   Float_t AllHitsX[10000];
   Float_t AllHitsY[10000];
   Float_t AllHitsZ[10000];

   Int_t Stub1Hits;
   Float_t S1HitsX[1000];
   Float_t S1HitsY[1000];
   Float_t S1HitsZ[1000];

   Int_t Stub2Hits;
   Float_t S2HitsX[1000];
   Float_t S2HitsY[1000];
   Float_t S2HitsZ[1000];
   
   Int_t TrkAHits;
   Float_t TrkAHitsX[100];
   Float_t TrkAHitsY[100];
   Float_t TrkAHitsZ[100];
  
   Int_t TrkBHits;
   Float_t TrkBHitsX[100];
   Float_t TrkBHitsY[100];
   Float_t TrkBHitsZ[100];

   Int_t NBasicClusters;
   Float_t BasicClusterE[100];
   Float_t BasicClusterEt[100];
   Float_t BasicClusterEta[100];
   Float_t BasicClusterPhi[100];

 };
 
#endif

