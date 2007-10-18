// -*- C++ -*-
//
// Package:    MaterialConversionModules
// Class:      Pi0ConversionAnalyzer
// 
/**\class Pi0ConversionAnalyzer Pi0ConversionAnalyzer.cc 
 
Description: <one line class summary>
Analyzer based off of the example in EgammaElectronProducers
Implementation:
<Notes on implementation>
*/
//
// Original Author:  Andrew Askew
//         Created:  Wed Apr 4 15:00:35 CDT 2007
// $Id: Pi0ConversionAnalyzer.cc,v 1.15 2007/03/14 09:13:55 rahatlou Exp $
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
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0ConversionAnalyzer.h"
#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialConversion.h"
//#include "RecoEgamma/MaterialConversionModules/interface/Pi0MaterialProducer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
 
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2D.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h" 
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "TrackingTools/RoadSearchHitAccess/interface/DetHitAccess.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

 
#include "TH1.h"
#include "TFile.h"

//
// constructors and destructor
//
Pi0ConversionAnalyzer::Pi0ConversionAnalyzer(const edm::ParameterSet& iConfig){
  matchedStripRecHitsInputTag_ = iConfig.getParameter<edm::InputTag>("matchedStripRecHits");
  rphiStripRecHitsInputTag_    = iConfig.getParameter<edm::InputTag>("rphiStripRecHits");
  stereoStripRecHitsInputTag_  = iConfig.getParameter<edm::InputTag>("stereoStripRecHits");
  pixelRecHitsInputTag_  = iConfig.getParameter<edm::InputTag>("pixelRecHits");  
  basicclusterCollection_ = iConfig.getParameter<std::string>("basicclusterCollection");
  basicclusterProducer_ = iConfig.getParameter<std::string>("basicclusterProducer");
  //now do what ever initialization is needed
  outputFile_ = iConfig.getParameter<std::string>("outputfile");
  rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE"); // open output file to store histograms
  Pi0MaterialConversionCollection_= iConfig.getParameter<std::string>("Pi0MaterialConversionCollection");
  Pi0MaterialConversionProducer_=iConfig.getParameter<std::string>("Pi0MaterialConversionProducer");
  mcProducer_ = iConfig.getParameter<std::string>("mcProducer");
  tree = new TTree("tree","Vertex Comparison Tree");
  src_ = iConfig.getParameter<std::string>("src"); 

  tree->Branch("NumberofPi0",&NumberofPi0,"NumberofPi0/I");
  tree->Branch("Pi0eta",Pi0eta,"Pi0eta[NumberofPi0]/F");
  tree->Branch("Pi0phi",Pi0phi,"Pi0phi[NumberofPi0]/F");
  tree->Branch("Pi0pt",Pi0pt,"Pi0pt[NumberofPi0]/F");
  tree->Branch("NumberofEta",&NumberofEta,"NumberofEta/I");
  tree->Branch("Etaeta",Etaeta,"Etaeta[NumberofEta]/F");
   tree->Branch("Etaphi",Etaphi,"Etaphi[NumberofEta]/F");
  tree->Branch("Etapt",Etapt,"Etapt[NumberofEta]/F");
   
  tree->Branch("PhotonConverted",&PhotonConverted,"PhotonConverted/I");
  tree->Branch("NumberofConversions",&NumberofConversions,"NumberofConversions/I");
  tree->Branch("ConversionMCX",ConversionMCX,"ConversionMCX[NumberofConversions]/F");
  tree->Branch("ConversionMCY",ConversionMCY,"ConversionMCY[NumberofConversions]/F");
  tree->Branch("ConversionMCPhi",ConversionMCPhi,"ConversionMCPhi[NumberofConversions]/F");
  tree->Branch("ConversionMCZ",ConversionMCZ,"ConversionMCZ[NumberofConversions]/F");
  tree->Branch("ConversionMCR",ConversionMCR,"ConversionMCR[NumberofConversions]/F");
  tree->Branch("Electron1Px", Electron1Px, "Electron1Px[NumberofConversions]/F");
  tree->Branch("Electron1Py", Electron1Py, "Electron1Py[NumberofConversions]/F");
  tree->Branch("Electron1Pz", Electron1Pz, "Electron1Pz[NumberofConversions]/F");
  tree->Branch("Electron1Pt", Electron1Pt, "Electron1Pt[NumberofConversions]/F");
  tree->Branch("Electron1Eta", Electron1Eta, "Electron1Eta[NumberofConversions]/F");
  tree->Branch("Electron1Phi", Electron1Phi, "Electron1Phi[NumberofConversions]/F");
  tree->Branch("Electron2Px", Electron2Px, "Electron2Px[NumberofConversions]/F");
  tree->Branch("Electron2Py", Electron2Py, "Electron2Py[NumberofConversions]/F");
  tree->Branch("Electron2Pz", Electron2Pz, "Electron2Pz[NumberofConversions]/F");
  tree->Branch("Electron2Pt", Electron2Pt, "Electron2Pt[NumberofConversions]/F");
  tree->Branch("Electron2Eta", Electron2Eta, "Electron2Eta[NumberofConversions]/F");
  tree->Branch("Electron2Phi", Electron2Phi, "Electron2Phi[NumberofConversions]/F");

  tree->Branch("NumberofPhotons", &NumberofPhotons,"NumberofPhotons/I");
  tree->Branch("PhotonPx1", PhotonPx1, "PhotonPx1[NumberofPhotons]/F");
  tree->Branch("PhotonPy1", PhotonPy1, "PhotonPy1[NumberofPhotons]/F");
  tree->Branch("PhotonPz1", PhotonPz1, "PhotonPz1[NumberofPhotons]/F");
  tree->Branch("PhotonPt1", PhotonPt1, "PhotonPt1[NumberofPhotons]/F");
  tree->Branch("PhotonEta1", PhotonEta1, "PhotonEta1[NumberofPhotons]/F");
  tree->Branch("PhotonPhi1", PhotonPhi1, "PhotonPhi1[NumberofPhotons]/F");

  tree->Branch("PhotonVtx1", PhotonVtx1, "PhotonVtx1[NumberofPhotons]/I");
  tree->Branch("PhotonVtx1R", PhotonVtx1R, "PhotonVtx1R[NumberofPhotons]/F");
 
  tree->Branch("PhotonPx2", PhotonPx2, "PhotonPx2[NumberofPhotons]/F");
  tree->Branch("PhotonPy2", PhotonPy2, "PhotonPy2[NumberofPhotons]/F");
  tree->Branch("PhotonPz2", PhotonPz2, "PhotonPz2[NumberofPhotons]/F");
  tree->Branch("PhotonPt2", PhotonPt2, "PhotonPt2[NumberofPhotons]/F");
  tree->Branch("PhotonEta2", PhotonEta2, "PhotonEta2[NumberofPhotons]/F");
  tree->Branch("PhotonPhi2", PhotonPhi2, "PhotonPhi2[NumberofPhotons]/F");
  tree->Branch("PhotonVtx2", PhotonVtx2, "PhotonVtx2[NumberofPhotons]/I");

  tree->Branch("PhotonVtx2R", PhotonVtx2R, "PhotonVtx2R[NumberofPhotons]/F");
  tree->Branch("PhoMass",PhoMass,"PhoMass[NumberofPhotons]/F");

  tree->Branch("Candidate",&Candidate,"Candidate/I");
  tree->Branch("NumberofCandidates",&NumberofCandidates,"NumberofCandidates/I");
  tree->Branch("CandidateREst",CandidateREst,"CandidateREst[NumberofCandidates]/F");
  tree->Branch("CandidatePhiEst",CandidatePhiEst,"CandidatePhiEst[NumberofCandidates]/F");
  tree->Branch("CandidateVertex",CandidateVertex,"CandidateVertex[NumberofCandidates]/I");
  tree->Branch("CandidateVertexX",CandidateVertexX,"CandidateVertexX[NumberofCandidates]/F");
  tree->Branch("CandidateVertexY",CandidateVertexY,"CandidateVertexY[NumberofCandidates]/F");
  tree->Branch("CandidateVertexZ",CandidateVertexZ,"CandidateVertexZ[NumberofCandidates]/F");
  tree->Branch("CandidateVertexR",CandidateVertexR,"CandidateVertexR[NumberofCandidates]/F");
  tree->Branch("CandidateVertexChi2",CandidateVertexChi2,"CandidateVertexChi2[NumberofCandidates]/F");
  tree->Branch("CandidateStub1Char",CandidateStub1Char,"CandidateStub1Char[NumberofCandidates]/I");
  tree->Branch("CandidateStub2Char",CandidateStub2Char,"CandidateStub2Char[NumberofCandidates]/I");
  tree->Branch("CandidateVertexPhi",CandidateVertexPhi,"CandidateVertexPhi[NumberofCandidates]/F");
  tree->Branch("CandidateMass",CandidateMass,"CandidateMass[NumberofCandidates]/F");
  tree->Branch("CandidateIso",CandidateIso,"CandidateIso[NumberofCandidates]/F");
  tree->Branch("CandidateHCalE", CandidateHCalE, "CandidateHCalE[NumberofCandidates]/F");

  tree->Branch("CandidateEleTrackA",CandidateEleTrackA,"CandidateEleTrackA[NumberofCandidates]/I");
  tree->Branch("CandidateEleTrackpTA",CandidateEleTrackpTA,"CandidateEleTrackpTA[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackpTFA",CandidateEleTrackpTFA,"CandidateEleTrackpTFA[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackPhiFA",CandidateEleTrackPhiFA,"CandidateEleTrackPhiFA[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackEtaFA",CandidateEleTrackEtaFA,"CandidateEleTrackEtaFA[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackSmallRA",CandidateEleTrackSmallRA,"CandidateEleTrackSmallRA[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackChargeA",CandidateEleTrackChargeA,"CandidateEleTrackChargeA[NumberofCandidates]/I");
  tree->Branch("TrkAHits",&TrkAHits,"TrkAHits/I");
  tree->Branch("TrkAHitsX",TrkAHitsX,"TrkAHitsX[TrkAHits]/F");
  tree->Branch("TrkAHitsY",TrkAHitsY,"TrkAHitsY[TrkAHits]/F");
  tree->Branch("TrkAHitsZ",TrkAHitsZ,"TrkAHitsZ[TrkAHits]/F");

  tree->Branch("CandidateEleEnergyA",CandidateEleEnergyA,"CandidateEleEnergyA[NumberofCandidates]/F");
  tree->Branch("CandidateEleETA",CandidateEleETA,"CandidateEleETA[NumberofCandidates]/F");
  tree->Branch("CandidateElePhiA",CandidateElePhiA,"CandidateElePhiA[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaA",CandidateEleEtaA,"CandidateEleEtaA[NumberofCandidates]/F");

  tree->Branch("CandidateEleE1maxA", CandidateEleE1maxA,"CandidateEleE1maxA[NumberofCandidates]/F");
  tree->Branch("CandidateEleE12ndA", CandidateEleE12ndA,"CandidateEleE12ndA[NumberofCandidates]/F");
  tree->Branch("CandidateEle2x2A", CandidateEle2x2A,"CandidateEle2x2A[NumberofCandidates]/F");
  tree->Branch("CandidateEle3x3A", CandidateEle3x3A,"CandidateEle3x3A[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaEtaA",CandidateEleEtaEtaA,"CandidateEleEtaEtaA[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaPhiA",CandidateEleEtaPhiA,"CandidateEleEtaPhiA[NumberofCandidates]/F");
  tree->Branch("CandidateElePhiPhiA",CandidateElePhiPhiA,"CandidateElePhiPhiA[NumberofCandidates]/F");

  tree->Branch("CandidateEleHitsA",CandidateEleHitsA,"CandidateEleHitsA[NumberofCandidates]/I");


  tree->Branch("CandidateEleTrackB",CandidateEleTrackB,"CandidateEleTrackB[NumberofCandidates]/I");
  tree->Branch("CandidateEleTrackpTB",CandidateEleTrackpTB,"CandidateEleTrackpTB[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackpTFB",CandidateEleTrackpTFB,"CandidateEleTrackpTFB[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackPhiFB",CandidateEleTrackPhiFB,"CandidateEleTrackPhiFB[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackEtaFB",CandidateEleTrackEtaFB,"CandidateEleTrackEtaFB[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackSmallRB",CandidateEleTrackSmallRB,"CandidateEleTrackSmallRB[NumberofCandidates]/F");
  tree->Branch("CandidateEleTrackChargeB",CandidateEleTrackChargeB,"CandidateEleTrackChargeB[NumberofCandidates]/I");

  tree->Branch("TrkBHits",&TrkBHits,"TrkBHits/I");
  tree->Branch("TrkBHitsX",TrkBHitsX,"TrkBHitsX[TrkBHits]/F");
  tree->Branch("TrkBHitsY",TrkBHitsY,"TrkBHitsY[TrkBHits]/F");
  tree->Branch("TrkBHitsZ",TrkBHitsZ,"TrkBHitsZ[TrkBHits]/F");

  tree->Branch("CandidateEleEnergyB",CandidateEleEnergyB,"CandidateEleEnergyB[NumberofCandidates]/F");
  tree->Branch("CandidateEleETB",CandidateEleETB,"CandidateEleETB[NumberofCandidates]/F");
  tree->Branch("CandidateElePhiB",CandidateElePhiB,"CandidateElePhiB[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaB",CandidateEleEtaB,"CandidateEleEtaB[NumberofCandidates]/F");
  tree->Branch("CandidateEleHitsB",CandidateEleHitsB,"CandidateEleHitsB[NumberofCandidates]/I");

  tree->Branch("CandidateEleE1maxB", CandidateEleE1maxB,"CandidateEleE1maxB[NumberofCandidates]/F");
  tree->Branch("CandidateEleE12ndB", CandidateEleE12ndB,"CandidateEleE12ndB[NumberofCandidates]/F");
  tree->Branch("CandidateEle2x2B", CandidateEle2x2B,"CandidateEle2x2B[NumberofCandidates]/F");
  tree->Branch("CandidateEle3x3B", CandidateEle3x3B,"CandidateEle3x3B[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaEtaB",CandidateEleEtaEtaB,"CandidateEleEtaEtaB[NumberofCandidates]/F");
  tree->Branch("CandidateEleEtaPhiB",CandidateEleEtaPhiB,"CandidateEleEtaPhiB[NumberofCandidates]/F");
  tree->Branch("CandidateElePhiPhiB",CandidateElePhiPhiB,"CandidateElePhiPhiB[NumberofCandidates]/F");

  tree->Branch("CandidatePhoEnergy",CandidatePhoEnergy,"CandidatePhoEnergy[NumberofCandidates]/F");
  tree->Branch("CandidatePhoET",CandidatePhoET,"CandidatePhoET[NumberofCandidates]/F");
  tree->Branch("CandidatePhoPhi",CandidatePhoPhi,"CandidatePhoPhi[NumberofCandidates]/F");
  tree->Branch("CandidatePhoEta",CandidatePhoEta,"CandidatePhoEta[NumberofCandidates]/F");

  tree->Branch("CandidatePhoE1max", CandidatePhoE1max,"CandidatePhoE1max[NumberofCandidates]/F");
  tree->Branch("CandidatePhoE12nd", CandidatePhoE12nd,"CandidatePhoE12nd[NumberofCandidates]/F");
  tree->Branch("CandidatePho2x2", CandidatePho2x2,"CandidatePho2x2[NumberofCandidates]/F");
  tree->Branch("CandidatePho3x3", CandidatePho3x3,"CandidatePho3x3[NumberofCandidates]/F");
  tree->Branch("CandidatePhoEtaEta",CandidatePhoEtaEta,"CandidatePhoEtaEta[NumberofCandidates]/F");
  tree->Branch("CandidatePhoEtaPhi",CandidatePhoEtaPhi,"CandidatePhoEtaPhi[NumberofCandidates]/F");
  tree->Branch("CandidatePhoPhiPhi",CandidatePhoPhiPhi,"CandidatePhoPhiPhi[NumberofCandidates]/F");

  tree->Branch("GamAssign",GamAssign,"GammaAssign[NumberofCandidates]/I");
  tree->Branch("EAAssign",EAAssign,"EAAssign[NumberofCandidates]/I");
  tree->Branch("EBAssign",EBAssign,"EBAssign[NumberofCandidates]/I");

  tree->Branch("TotalHits",&TotalHits,"TotalHits/I");
  tree->Branch("AllHitsX",AllHitsX,"AllHitsX[TotalHits]/F");
  tree->Branch("AllHitsY",AllHitsY,"AllHitsY[TotalHits]/F");
  tree->Branch("AllHitsZ",AllHitsZ,"AllHitsZ[TotalHits]/F");

  tree->Branch("Stub1Hits",&Stub1Hits,"Stub1Hits/I");
  tree->Branch("S1HitsX",S1HitsX,"S1HitsX[Stub1Hits]/F");
  tree->Branch("S1HitsY",S1HitsY,"S1HitsY[Stub1Hits]/F");
  tree->Branch("S1HitsZ",S1HitsZ,"S1HitsZ[Stub1Hits]/F");

  tree->Branch("Stub2Hits",&Stub2Hits,"Stub2Hits/I");
  tree->Branch("S2HitsX",S2HitsX,"S2HitsX[Stub2Hits]/F");
  tree->Branch("S2HitsY",S2HitsY,"S2HitsY[Stub2Hits]/F");
  tree->Branch("S2HitsZ",S2HitsZ,"S2HitsZ[Stub2Hits]/F");

  tree->Branch("NBasicClusters", &NBasicClusters, "NBasicClusters/I");
  tree->Branch("BasicClusterE", BasicClusterE, "BasicClusterE[NBasicClusters]/F");
  tree->Branch("BasicClusterEt", BasicClusterEt, "BasicClusterEt[NBasicClusters]/F");
  tree->Branch("BasicClusterEta", BasicClusterEta, "BasicClusterEta[NBasicClusters]/F");
  tree->Branch("BasicClusterPhi", BasicClusterPhi, "BasicClusterPhi[NBasicClusters]/F");
  

  tree->SetAutoSave(1000000);
}

 
 
Pi0ConversionAnalyzer::~Pi0ConversionAnalyzer()
{
  
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  delete rootFile_;
  
  
}
//
// member functions
//
 
// ------------ method called to for each event  ------------
void Pi0ConversionAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
    
  // Get conversion objects
  Handle<Pi0MaterialConversionCollection> pPi0ConvObj;
  iEvent.getByLabel(Pi0MaterialConversionProducer_, Pi0MaterialConversionCollection_, pPi0ConvObj);
  const Pi0MaterialConversionCollection* Pi0Conversions = pPi0ConvObj.product();
  
  //GET GENERATOR EVENT - BEGIN
  Handle<SimTrackContainer> SimTk;
  iEvent.getByLabel(mcProducer_.c_str(), SimTk); 
  std::vector<SimTrack> simmer = *SimTk;

  Handle<SimVertexContainer> SimVx;
  iEvent.getByLabel(mcProducer_.c_str(), SimVx); 
  std::vector<SimVertex> verter = *SimVx;

//   Handle<HepMCProduct> h_mcp;
//   iEvent.getByType( h_mcp );

  Handle< HepMCProduct > hepProd ;
  iEvent.getByLabel( src_.c_str(), hepProd ) ;
  const HepMC::GenEvent * mc = hepProd->GetEvent();

  if( mc == 0 ) 
    std::cout << "HepMC has null pointer to GenEvent" << std::endl;
  //GET GENERATOR Particles - END
 
  PhotonConverted =0;
  NumberofConversions=0;
  Candidate=0;
  NumberofCandidates=0;
  NumberofPi0=0;
  NumberofEta=0;
  NumberofPhotons=0;
  TrkAHits=0;
  TrkBHits=0;

  for(int j=0;j<50000;j++){
    Electron1Px[j]=0;
    Electron1Py[j]=0;
    Electron1Pz[j]=0;
    Electron1Pt[j]=0;
    Electron1Phi[j]=0;
    Electron1Eta[j]=0;
    
    Electron2Px[j]=0;
    Electron2Py[j]=0;
    Electron2Pz[j]=0;
    Electron2Pt[j]=0;
    Electron2Phi[j]=0;
    Electron2Eta[j]=0;
    
    PhotonPx1[j]=0;
    PhotonPy1[j]=0;
    PhotonPz1[j]=0;
    PhotonPt1[j]=0;
    PhotonEta1[j]=0;
    PhotonPhi1[j]=0;
    PhotonVtx1[j]=0;
    PhotonVtx1R[j]=0;
    PhotonVtx2[j]=0;
    PhotonVtx2R[j]=0;
    PhotonPx2[j]=0;
    PhotonPy2[j]=0;
    PhotonPz2[j]=0;
    PhotonPt2[j]=0;
    PhotonEta2[j]=0;
    PhotonPhi2[j]=0;
    PhoMass[j]=0;
 
    Pi0eta[j]=0;
    Pi0phi[j]=0;
    Pi0pt[j]=0;
    Etaeta[j]=0;
    Etaphi[j]=0;
    Etapt[j]=0;

  }
  for(int j=0;j<100;j++){

    ConversionMCX[j]=0;
    ConversionMCY[j]=0;
    ConversionMCZ[j]=0;
    ConversionMCR[j]=0;
    ConversionMCPhi[j]=0;
    CandidateREst[j]=0;
    CandidatePhiEst[j]=0;
    CandidateVertex[j]=0;
    CandidateVertexX[j]=0;
    CandidateVertexY[j]=0;
    CandidateVertexZ[j]=0;
    CandidateVertexR[j]=0;
    CandidateVertexChi2[j]=-999;
    CandidateVertexPhi[j]=0;
    CandidateStub1Char[j]=0;
    CandidateStub2Char[j]=0;

    CandidateEleTrackA[j]=0;
    CandidateEleTrackpTA[j]=0;
    CandidateEleTrackpTFA[j]=0;
    CandidateEleTrackPhiFA[j]=0;
    CandidateEleTrackEtaFA[j]=0;
    CandidateEleTrackSmallRA[j]=0;
    CandidateEleEnergyA[j]=0;
    CandidateEleETA[j]=0;
    CandidateElePhiA[j]=0;
    CandidateEleEtaA[j]=0;

    CandidateEleE1maxA[j]=0;
    CandidateEleE12ndA[j]=0;
    CandidateEle2x2A[j]=0;
    CandidateEle3x3A[j]=0;
    CandidateEleEtaEtaA[j]=0;
    CandidateEleEtaPhiA[j]=0;
    CandidateElePhiPhiA[j]=0;
    CandidateEleE1maxB[j]=0;
    CandidateEleE12ndB[j]=0;
    CandidateEle2x2B[j]=0;
    CandidateEle3x3B[j]=0;
    CandidateEleEtaEtaB[j]=0;
    CandidateEleEtaPhiB[j]=0;
    CandidateElePhiPhiB[j]=0;
    CandidatePhoE1max[j]=0;
    CandidatePhoE12nd[j]=0;
    CandidatePho2x2[j]=0;
    CandidatePho3x3[j]=0;
    CandidatePhoEtaEta[j]=0;
    CandidatePhoEtaPhi[j]=0;
    CandidatePhoPhiPhi[j]=0;
    CandidateIso[j]=0;
    CandidateHCalE[j]=0;
    CandidateEleHitsA[j]=0;
    CandidateEleTrackChargeA[j]=0;


    CandidateEleTrackB[j]=0;
    CandidateEleTrackpTB[j]=0;
    CandidateEleTrackpTFB[j]=0;
    CandidateEleTrackPhiFB[j]=0;
    CandidateEleTrackEtaFB[j]=0;
    CandidateEleTrackSmallRB[j]=0;
    CandidateEleEnergyB[j]=0;
    CandidateEleETB[j]=0;
    CandidateElePhiB[j]=0;
    CandidateEleEtaB[j]=0;
    CandidateEleHitsB[j]=0;
    CandidateEleTrackChargeB[j]=0;
    CandidatePhoEnergy[j]=0;
    CandidatePhoET[j]=0;
    CandidatePhoPhi[j]=0;
    CandidatePhoEta[j]=0;
    GamAssign[j]=0;
    EAAssign[j]=0;
    EBAssign[j]=0;

    TrkAHitsX[j]=0;
    TrkAHitsY[j]=0;
    TrkAHitsZ[j]=0;    

    TrkBHitsX[j]=0;
    TrkBHitsY[j]=0;
    TrkBHitsZ[j]=0;
    
  }
  
  TotalHits=0;
  for (int i=0;i<10000;i++){
    AllHitsX[i]=0;
    AllHitsY[i]=0;
    AllHitsZ[i]=0;
  }
  
  Stub1Hits=0;
  Stub2Hits=0;
  for (int i=0;i<1000;++i){
    S1HitsX[i]=0;
    S1HitsY[i]=0;
    S1HitsZ[i]=0;

    S2HitsX[i]=0;
    S2HitsY[i]=0;
    S2HitsZ[i]=0;
  }

  //loop over MC pi0 and find our single guy.

  for( HepMC::GenEvent::particle_const_iterator p = mc->particles_begin(); 
       p != mc->particles_end(); ++ p ) {
    int id = abs( (*p)->pdg_id() );
    if (id == 111){
      //pi0
      if (NumberofPi0<50000){
	const CLHEP::HepLorentzVector pmom = (*p)->momentum();
	Pi0eta[NumberofPi0] = pmom.eta();
	Pi0phi[NumberofPi0] = pmom.phi();
	Pi0pt[NumberofPi0] = sqrt(pmom.px()*pmom.px() + pmom.py()*pmom.py());
	if (fabs(Pi0eta[NumberofPi0])<2 && Pi0pt[NumberofPi0]>3)
	  NumberofPi0++;
      }
    }
    if (id == 221){
      //eta
      if (NumberofEta<50000){
	const CLHEP::HepLorentzVector pmom = (*p)->momentum();
	Etaeta[NumberofEta] = pmom.eta();
	Etaphi[NumberofEta] = pmom.phi();
	Etapt[NumberofEta] = sqrt(pmom.px()*pmom.px() + pmom.py()*pmom.py());
	if (fabs(Etaeta[NumberofEta])<2 && Etapt[NumberofEta]>3)
	NumberofEta++;
      }	
    }
  }

  


  std::vector<SimTrack> pi0s;
  std::vector<int> positrons;
  std::vector<int> electrons;
  std::vector<int> photons;

  std::vector<int> electronVert;
  std::vector<int> positronVert;
  std::vector<int> photonVert; 
  std::vector<int> photonVertAssc;
  std::vector<SimVertex> conversionVertex;
  std::vector<SimVertex> gammagammaVertex;
  
  for (int i=0;i<int(simmer.size());++i){
    SimTrack p = simmer[i];
    if (p.type() == 111){
      pi0s.push_back(p);
    }
    if (p.type() == 11){
      electrons.push_back(i);
      if (p.vertIndex() < int(verter.size())){
	int ider = p.vertIndex();
	electronVert.push_back(ider);
      }
      else electronVert.push_back(-1);
    }
    if (p.type() == -11){
      positrons.push_back(i);
      if (p.vertIndex() < int(verter.size())){
	int ider = p.vertIndex();
	positronVert.push_back(ider);
      }
      else positronVert.push_back(-1);
    }
    if (p.type() == 22){
      photons.push_back(i);
      int ider = p.vertIndex();
      if (p.vertIndex() < int(verter.size())){
	photonVert.push_back(ider);
      }
    }
  }// This is a loop over identifying all electrons, photons and pi0

  //Identify all photon-photon vertices.  These should be the decay of eta/pi0
  std::vector<Int_t> InitialPhoton1;
  std::vector<Int_t> InitialPhoton2;
  if (photonVert.size() == photons.size()){
    for (int j=0;j<int(photonVert.size());++j){
      for (int k=j+1;k<int(photonVert.size());++k){
	if (photonVert[j] == photonVert[k]
	    && photonVert[j] !=-1){
	  if (NumberofPhotons<50000){
	    Int_t photon1 = photons[j];
	    Int_t photon2 = photons[k];
	    
	    Int_t in = photonVert[j];
	    SimVertex vertexa = verter[in];
	    
	    PhotonPx1[NumberofPhotons]=simmer[photon1].momentum().px();
	    PhotonPy1[NumberofPhotons]=simmer[photon1].momentum().py();
	    PhotonPz1[NumberofPhotons]=simmer[photon1].momentum().pz();
	    Float_t PhotonE = simmer[photon1].momentum().e();
	    PhotonPt1[NumberofPhotons]=sqrt(PhotonPx1[NumberofPhotons]*PhotonPx1[NumberofPhotons] + PhotonPy1[NumberofPhotons]*PhotonPy1[NumberofPhotons]);
	    PhotonEta1[NumberofPhotons]=simmer[photon1].momentum().eta();
	    PhotonPhi1[NumberofPhotons]=simmer[photon1].momentum().phi();
	    //Look to see if photon1 is the parent of a vertex.
	    Int_t vertI1 = -1;
	    for (int ik=0;ik<int(verter.size());++ik){
	      //Looking to match up parent index
	      Int_t parIdx = verter[ik].parentIndex();
	      if (parIdx == photon1) vertI1 = ik;
	    }
	    if (vertI1 !=-1){
	      PhotonVtx1[NumberofPhotons]=1;
	      PhotonVtx1R[NumberofPhotons]= verter[vertI1].position().perp();
	    }
	    else{
	      PhotonVtx1[NumberofPhotons]=0;
	      PhotonVtx1R[NumberofPhotons]= -999;
	    }

	    PhotonPx2[NumberofPhotons]=simmer[photon2].momentum().px();
	    PhotonPy2[NumberofPhotons]=simmer[photon2].momentum().py();
	    PhotonPz2[NumberofPhotons]=simmer[photon2].momentum().pz();
	    Float_t PhotonE2 = simmer[photon2].momentum().e();
	    PhotonPt2[NumberofPhotons]=sqrt(PhotonPx2[NumberofPhotons]*PhotonPx2[NumberofPhotons] + PhotonPy2[NumberofPhotons]*PhotonPy2[NumberofPhotons]);
	    PhotonEta2[NumberofPhotons]=simmer[photon2].momentum().eta();
	    PhotonPhi2[NumberofPhotons]=simmer[photon2].momentum().phi();
	    //Look to see if photon2 is the parent of a vertex.
	    Int_t vertI2 = -1;
	    for (int ik=0;ik<int(verter.size());++ik){
	      //Looking to match up parent index
	      Int_t parIdx = verter[ik].parentIndex();
	      if (parIdx == photon2) vertI2 = ik;
	    }
	    if (vertI2 !=-1){
	      PhotonVtx2[NumberofPhotons]=1;
	      PhotonVtx2R[NumberofPhotons]= verter[vertI2].position().perp();
	    }
	    else{
	      PhotonVtx2[NumberofPhotons]=0;
	      PhotonVtx2R[NumberofPhotons]= -999;
	    }



	    PhoMass[NumberofPhotons] = sqrt( pow((PhotonE + PhotonE2),2) - (
									    pow((PhotonPx1[NumberofPhotons]+PhotonPx2[NumberofPhotons]),2)
									    +pow((PhotonPy1[NumberofPhotons]+PhotonPy2[NumberofPhotons]),2)
									    +pow((PhotonPz1[NumberofPhotons]+PhotonPz2[NumberofPhotons]),2)));
	    NumberofPhotons++;
	  }//check on array bound
	}//test if same vertex.
      }//loop k
    }//loop over photon vert.
  }//Done identifying photon-photon vertex.

  //Identify all electron-positron vertices.  These should be the conversions.    
  for (int j=0;j<int(positronVert.size());++j){
    if (positronVert[j]!=-1){
      Int_t vertexIdx = positronVert[j];
      for (int k=0;k<int(electronVert.size());++k){
	if (vertexIdx == electronVert[k]
	    && electronVert[k]!=-1){
	  //conversion vertex
	  if (NumberofConversions<50000){
	    SimVertex verta = verter[vertexIdx];
	    Int_t ElectronConvert1 = positrons[j];
	    Int_t ElectronConvert2 = electrons[k];
	    ConversionMCX[NumberofConversions]=verta.position().x();
	    ConversionMCY[NumberofConversions]=verta.position().y();
	    ConversionMCZ[NumberofConversions]=verta.position().z();
	    ConversionMCR[NumberofConversions]=verta.position().perp();
	    ConversionMCPhi[NumberofConversions]=verta.position().phi();
	    Electron1Px[NumberofConversions]=simmer[ElectronConvert1].momentum().px();
	    Electron1Py[NumberofConversions]=simmer[ElectronConvert1].momentum().py();
	    Electron1Pz[NumberofConversions]=simmer[ElectronConvert1].momentum().pz();
	    Electron1Pt[NumberofConversions]=sqrt(Electron1Px[NumberofConversions]*Electron1Px[NumberofConversions] + Electron1Py[NumberofConversions]*Electron1Py[NumberofConversions]);
	    Electron1Phi[NumberofConversions]=simmer[ElectronConvert1].momentum().phi();
	    Electron1Eta[NumberofConversions]=simmer[ElectronConvert1].momentum().eta();
	    Electron2Px[NumberofConversions]=simmer[ElectronConvert2].momentum().px();
	    Electron2Py[NumberofConversions]=simmer[ElectronConvert2].momentum().py();
	    Electron2Pz[NumberofConversions]=simmer[ElectronConvert2].momentum().pz();
	    Electron2Pt[NumberofConversions]=sqrt(Electron2Px[NumberofConversions]*Electron2Px[NumberofConversions] + Electron2Py[NumberofConversions]*Electron2Py[NumberofConversions]);
	    Electron2Phi[NumberofConversions]=simmer[ElectronConvert2].momentum().phi();
	    Electron2Eta[NumberofConversions]=simmer[ElectronConvert2].momentum().eta();
	    NumberofConversions++;
	  }
	}
      }//Loop over electrons 
    }//Check on vertex
  }//loop over positrons.
  

  if (Pi0Conversions->size()>0){
    Candidate=1;
    for(int j=0;j<int(Pi0Conversions->size()) && j<100 ;j++){
      Pi0MaterialConversion bl = (*Pi0Conversions)[j];
      CandidateREst[j]= bl.getREst();
      CandidatePhiEst[j]=bl.getPhiEst();
      CandidateMass[j]=bl.getMass();
      CandidateStub1Char[j]=bl.getStub1Char();
      CandidateStub2Char[j]=bl.getStub2Char();
      CandidateIso[j] = bl.getIso();
      CandidateHCalE[j] = bl.getHCALEnergy();
      if (bl.getVtxChi2()>0){
	CandidateVertex[NumberofCandidates]=1;
	CandidateVertexX[NumberofCandidates]=bl.getVtxXPos();
	CandidateVertexY[NumberofCandidates]=bl.getVtxYpos();
	CandidateVertexZ[NumberofCandidates]=bl.getVtxZpos();
	CandidateVertexR[NumberofCandidates]=bl.getVtxRpos();
	CandidateVertexPhi[NumberofCandidates]=bl.getVtxPhipos(); 
	CandidateVertexChi2[NumberofCandidates]=bl.getVtxChi2();
	
      }//has reconstructed vertex
      if (bl.getTrk1Chi2()>0){
	CandidateEleTrackA[NumberofCandidates]=1;
	CandidateEleTrackpTA[NumberofCandidates]=bl.getTrk1PTIS();
	CandidateEleTrackpTFA[NumberofCandidates]=bl.getTrk1PTFS();
	CandidateEleTrackPhiFA[NumberofCandidates]=bl.getTrk1PhiFS();
	if (CandidateEleTrackPhiFA[NumberofCandidates] <0)  CandidateEleTrackPhiFA[NumberofCandidates]+=TMath::Pi()*2.;
	CandidateEleTrackEtaFA[NumberofCandidates]=bl.getTrk1EtaFS();
	CandidateEleTrackSmallRA[NumberofCandidates]=bl.getTrk1SmallRHit();
	CandidateEleHitsA[NumberofCandidates]=bl.getTrk1NHIT();
	CandidateEleTrackChargeA[NumberofCandidates]=bl.getTrk1Charge();
	
	std::vector<GlobalPoint> tta = bl.getTrack1Hits();
	
	for (int i=0;i<int(tta.size()) && TrkAHits<100;++i){
	  TrkAHitsX[TrkAHits] = tta[i].x();
	  TrkAHitsY[TrkAHits] = tta[i].y();
	  TrkAHitsZ[TrkAHits] = tta[i].z();
	  TrkAHits++;
	}
      }
      
      CandidateEleEnergyA[NumberofCandidates]=bl.getE1E();
      float tempeta1 = bl.getE1eta();
      float temptheta1 = 2. * atan(exp(-1.*tempeta1));
      CandidateEleETA[NumberofCandidates]=sin(temptheta1)*bl.getE1E();
      CandidateElePhiA[NumberofCandidates]=bl.getE1phi();
      CandidateEleEtaA[NumberofCandidates]=bl.getE1eta();
      
      CandidatePhoEnergy[NumberofCandidates]=bl.getGamE();
      float tempGeta1 = bl.getGameta();
      float tempGtheta1 = 2. * atan(exp(-1.*tempGeta1));
      CandidatePhoET[NumberofCandidates]=sin(tempGtheta1)*bl.getGamE();;
      CandidatePhoPhi[NumberofCandidates]=bl.getGamphi();
      CandidatePhoEta[NumberofCandidates]=bl.getGameta();
      
      CandidateEleE1maxA[NumberofCandidates]=bl.getE1Emax();
      CandidateEleE12ndA[NumberofCandidates]=bl.getE1E2nd();
      CandidateEle2x2A[NumberofCandidates]=bl.getE1e2x2();
      CandidateEle3x3A[NumberofCandidates]=bl.getE1e3x3();
      CandidateEleEtaEtaA[NumberofCandidates]=bl.getE1etaeta();
      CandidateEleEtaPhiA[NumberofCandidates]=bl.getE1etaphi();
      CandidateElePhiPhiA[NumberofCandidates]=bl.getE1phiphi();
      CandidateEleE1maxB[NumberofCandidates]=bl.getE2Emax();
      CandidateEleE12ndB[NumberofCandidates]=bl.getE2E2nd();
      CandidateEle2x2B[NumberofCandidates]=bl.getE2e2x2();
      CandidateEle3x3B[NumberofCandidates]=bl.getE2e3x3();
      CandidateEleEtaEtaB[NumberofCandidates]=bl.getE2etaeta();
      CandidateEleEtaPhiB[NumberofCandidates]=bl.getE2etaphi();
      CandidateElePhiPhiB[NumberofCandidates]=bl.getE2phiphi();
      CandidatePhoE1max[NumberofCandidates]=bl.getGamEmax();
      CandidatePhoE12nd[NumberofCandidates]=bl.getGamE2nd();
      CandidatePho2x2[NumberofCandidates]=bl.getGame2x2();
      CandidatePho3x3[NumberofCandidates]=bl.getGame3x3();
      CandidatePhoEtaEta[NumberofCandidates]=bl.getGametaeta();
      CandidatePhoEtaPhi[NumberofCandidates]=bl.getGametaphi();
      CandidatePhoPhiPhi[NumberofCandidates]=bl.getGamphiphi();

      
      if (bl.getTrk2Chi2()>0){
	CandidateEleTrackB[NumberofCandidates]=1;
	CandidateEleTrackpTB[NumberofCandidates]=bl.getTrk2PTIS();
	CandidateEleTrackpTFB[NumberofCandidates]=bl.getTrk2PTFS();
	CandidateEleTrackPhiFB[NumberofCandidates]=bl.getTrk2PhiFS();
	if (CandidateEleTrackPhiFB[NumberofCandidates] <0)  CandidateEleTrackPhiFB[NumberofCandidates]+=TMath::Pi()*2.;
	CandidateEleTrackEtaFB[NumberofCandidates]=bl.getTrk2EtaFS();
	CandidateEleTrackSmallRB[NumberofCandidates]=bl.getTrk2SmallRHit();
	CandidateEleHitsB[NumberofCandidates]=bl.getTrk2NHIT();
	CandidateEleTrackChargeB[NumberofCandidates]=bl.getTrk2Charge();
	
	std::vector<GlobalPoint> ttb = bl.getTrack2Hits();
	
	for (int i=0;i<int(ttb.size()) && TrkBHits<100;++i){
	  TrkBHitsX[TrkBHits] = ttb[i].x();
	  TrkBHitsY[TrkBHits] = ttb[i].y();
	  TrkBHitsZ[TrkBHits] = ttb[i].z();
	  TrkBHits++;
	}
	
      }
      CandidateEleEnergyB[NumberofCandidates]=bl.getE2E();
      float tempeta2 = bl.getE2eta();
      float temptheta2 = 2. * atan(exp(-1.*tempeta2));
      CandidateEleETB[NumberofCandidates]=sin(temptheta2)*bl.getE2E();
      CandidateElePhiB[NumberofCandidates]=bl.getE2phi();
      CandidateEleEtaB[NumberofCandidates]=bl.getE2eta();
      
//       //Check your assignment:
// 	//photon
// 	Float_t dRGamGam=GetSep(PhotonPhi, PhotonEta, CandidatePhoPhi[NumberofCandidates], CandidatePhoEta[NumberofCandidates]);
// 	Float_t dRGamEA=GetSep(PhotonPhi, PhotonEta, CandidateElePhiA[NumberofCandidates], CandidateEleEtaA[NumberofCandidates]);
// 	Float_t dRGamEB=GetSep(PhotonPhi, PhotonEta, CandidateElePhiB[NumberofCandidates], CandidateEleEtaB[NumberofCandidates]);
// 	if (dRGamGam < dRGamEA 
// 	    && dRGamGam < dRGamEB)
// 	  GamAssign[NumberofCandidates]=1;
// 	//E1
// 	Float_t dRE1Gam=GetSep(Electron1Phi, Electron1Eta, CandidatePhoPhi[NumberofCandidates], CandidatePhoEta[NumberofCandidates]);
// 	Float_t dRE1EA=GetSep(Electron1Phi, Electron1Eta, CandidateElePhiA[NumberofCandidates], CandidateEleEtaA[NumberofCandidates]);
// 	Float_t dRE1EB=GetSep(Electron1Phi, Electron1Eta, CandidateElePhiB[NumberofCandidates], CandidateEleEtaB[NumberofCandidates]);
// 	if (dRE1EA < dRE1EB 
// 	    && dRE1EA < dRE1Gam)
// 	  EAAssign[NumberofCandidates]=1;
	
// 	//E2
// 	Float_t dRE2Gam=GetSep(Electron2Phi, Electron2Eta, CandidatePhoPhi[NumberofCandidates], CandidatePhoEta[NumberofCandidates]);
// 	Float_t dRE2EA=GetSep(Electron2Phi, Electron2Eta, CandidateElePhiA[NumberofCandidates], CandidateEleEtaA[NumberofCandidates]);
// 	Float_t dRE2EB=GetSep(Electron2Phi, Electron2Eta, CandidateElePhiB[NumberofCandidates], CandidateEleEtaB[NumberofCandidates]);
// 	if (dRE2EB < dRE2EA 
// 	    && dRE2EB < dRE2Gam)
// 	  EBAssign[NumberofCandidates]=1;
      std::vector<GlobalPoint> stb1 = bl.getStub1Hits();
      std::vector<GlobalPoint> stb2 = bl.getStub2Hits();
      
      for (int i=0;i<int(stb1.size()) && Stub1Hits<1000;++i){
	S1HitsX[Stub1Hits]=stb1[i].x();
	S1HitsY[Stub1Hits]=stb1[i].y();
	S1HitsZ[Stub1Hits]=stb1[i].z();
	Stub1Hits++;
      }
      for (int i=0;i<int(stb2.size()) && Stub2Hits<1000;++i){
	S2HitsX[Stub2Hits]=stb2[i].x();
	S2HitsY[Stub2Hits]=stb2[i].y();
	S2HitsZ[Stub2Hits]=stb2[i].z();
	Stub2Hits++;
      }
      
      
      NumberofCandidates++;
    }//loop over conversioncandidates
    
    // get Inputs
    edm::Handle<SiStripMatchedRecHit2DCollection> pmatchedRecHits;
    iEvent.getByLabel(matchedStripRecHitsInputTag_ ,pmatchedRecHits);
    edm::Handle<SiStripRecHit2DCollection> prphiRecHits;
    iEvent.getByLabel(rphiStripRecHitsInputTag_ ,prphiRecHits);
    edm::Handle<SiStripRecHit2DCollection> pstereoRecHits;
    iEvent.getByLabel(stereoStripRecHitsInputTag_ ,pstereoRecHits);
    // retrieve InputTag for pixel rechits
    
    const SiStripMatchedRecHit2DCollection* matchedRecHits = pmatchedRecHits.product();
    const SiStripRecHit2DCollection *rphiRecHits = prphiRecHits.product();
    const SiStripRecHit2DCollection *stereoRecHits = pstereoRecHits.product();
    
    // special treatment for getting pixel collection
    // if collection exists in file, use collection from file
    // if collection does not exist in file, create empty collection
    const SiPixelRecHitCollection *pixelRecHitCollection = 0;
    try {
      edm::Handle<SiPixelRecHitCollection> pixelRecHits;
      iEvent.getByLabel(pixelRecHitsInputTag_, pixelRecHits);
      pixelRecHitCollection = pixelRecHits.product();
    }
    catch (edm::Exception const& x) {
      if ( x.categoryCode() == edm::errors::ProductNotFound ) {
	if ( x.history().size() == 1 ) {
	  static const SiPixelRecHitCollection s_empty;
	  pixelRecHitCollection = &s_empty;
	  
	}
      }
    }
    //Select hit collections
    DetHitAccess recHitVectorClass;
    const std::vector<DetId> availableIDs = rphiRecHits->ids();
    const std::vector<DetId> availableID2 = stereoRecHits->ids();
    const std::vector<DetId> availableID3 = matchedRecHits->ids();
    const std::vector<DetId> availableID4 = pixelRecHitCollection->ids();
    
    recHitVectorClass.setCollections(rphiRecHits,stereoRecHits,matchedRecHits,pixelRecHitCollection);
    recHitVectorClass.setMode(DetHitAccess::standard);
    //	    recHitVectorClass.use_rphiRecHits(true);
    //	    recHitVectorClass.use_stereoRecHits(true);
    
    //This is where I will put ALL the hits.
    
    std::vector <TrackingRecHit*> FullTracker;
    for (int kj=0;kj < int(availableIDs.size());++kj){
      std::vector <TrackingRecHit*> FullTracker1 = recHitVectorClass.getHitVector(&availableIDs[kj]);
      for (int ji=0;ji<int(FullTracker1.size());ji++){
	FullTracker.push_back((FullTracker1)[ji]);
      }
    }
    
    for (int kj=0;kj < int(availableID2.size());++kj){
      std::vector <TrackingRecHit*> FullTracker2 = recHitVectorClass.getHitVector(&availableID2[kj]);
      for (int ji=0;ji<int(FullTracker2.size());ji++){
	FullTracker.push_back((FullTracker2)[ji]);
      }
    }
    
    for (int kj=0;kj < int(availableID3.size());++kj){
      std::vector <TrackingRecHit*> FullTracker3 = recHitVectorClass.getHitVector(&availableID3[kj]);
      for (int ji=0;ji<int(FullTracker3.size());ji++){
	FullTracker.push_back((FullTracker3)[ji]);
      }
    }
    
    for (int kj=0;kj < int(availableID4.size());++kj){
      std::vector <TrackingRecHit*> FullTracker4 = recHitVectorClass.getHitVector(&availableID4[kj]);
      for (int ji=0;ji<int(FullTracker4.size());ji++){
	FullTracker.push_back((FullTracker4)[ji]);
      }
    }
    
    
    edm::ESHandle<TrackerGeometry> tracker;
    iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
    const TrackerGeometry& geometry = *tracker;
    
    for (int i=0;i<int(FullTracker.size()) && TotalHits<10000;i++){
      TrackingRecHit *htr = (FullTracker)[i];
      
      GlobalPoint position = geometry.idToDet( 
					      htr->geographicalId()
					      )->surface().toGlobal(
								    htr->localPosition());
      AllHitsX[TotalHits]=position.x();
      AllHitsY[TotalHits]=position.y();
      AllHitsZ[TotalHits]=position.z();
      TotalHits++;
    }
    

  }//size of conversioncandidates 
  //Dump in all island clusters:
    //Get IslandBasicClusters to start.
  edm::Handle<reco::BasicClusterCollection> pBarrelBasicClusters;
  try {
    iEvent.getByLabel(basicclusterProducer_, basicclusterCollection_, pBarrelBasicClusters);
  } catch ( cms::Exception& ex ) {
    std::cout << "Error! can't get collection with label " << basicclusterCollection_.c_str()<<std::endl; ;
  }
  
  reco::BasicClusterCollection BarrelBasicClusters = *pBarrelBasicClusters;
  //Got the IslandBasicClusters
  NBasicClusters=0;

  for (int loop1=0;loop1<int(BarrelBasicClusters.size())&& loop1<100;loop1++){
    reco::BasicCluster clus1 = BarrelBasicClusters[loop1];
    BasicClusterEta[loop1]= clus1.eta();
    float eta1 = clus1.eta();
    float energy1 = clus1.energy();
    BasicClusterE[loop1] = clus1.energy();
    float theta1 = 2*atan(exp(-1.*eta1));
    BasicClusterEt[loop1] = energy1 * sin(theta1);
    BasicClusterPhi[loop1]= clus1.phi();
    NBasicClusters++;
  }


  tree->Fill();
  
}//Analyze

 
 
 // ------------ method called once each job just before starting event loop  ------------
void Pi0ConversionAnalyzer::beginJob(const edm::EventSetup&) {
 
 // go to *OUR* rootfile and book histograms
  rootFile_->cd();
  
 
}
 
 // ------------ method called once each job just after ending the event loop  ------------
void Pi0ConversionAnalyzer::endJob() {
 

  //   h1_islandEBBC_e3x3_Over_e5x5_->Write();
  rootFile_->cd();
  tree->Write();
  rootFile_->Close();
 
}
float Pi0ConversionAnalyzer::pimass(float eta_g, float phi_g, float e_g, float eta_e, float phi_e, float e_e, float eta_p, float phi_p, float e_p)
{


  float dphi0 = phi_e - phi_p;
  if(dphi0 > 3.1416) dphi0 -= 6.2832;
  if(dphi0 <-3.1416) dphi0 += 6.2832;
  //  float ttd1 = phi_p - phi_g;
  //  float ttd2 = phi_e - phi_g;
  
  float theta_g = 2*atan(exp(-eta_g));
  float theta_e = 2*atan(exp(-eta_e));
  float theta_p = 2*atan(exp(-eta_p));
  float et_e = e_e*sin(theta_e);
  float et_p = e_p*sin(theta_p);
  
  float r1 = et_p/1.2;
  float r2 = et_e/1.2;
  float dphi1 = -dphi0 * r2 / (r1+r2);
  float phi_gam = phi_p - dphi1; 
  float eta_gam = 0.5*(eta_p + eta_e);
  float energy_gam = e_e + e_p;
  
  float d_1 = (phi_gam - phi_g)*sin(theta_g);
  if(d_1 > 3.1416) d_1 -= 6.2832;
  if(d_1 <-3.1416) d_1 += 6.2832;
  float d_2 = (eta_gam - eta_g)*sin(theta_g);
  
  float mass = energy_gam*e_g*(d_1*d_1+d_2*d_2);
  mass = mass > 0 ? sqrt(mass) : mass;
  
  return mass;
}

float Pi0ConversionAnalyzer::GetSep(float phi1, float eta1, float phi2, float eta2)
{
  float deta = fabs(eta1-eta2);
  float dphi1 = fabs(phi1-phi2);
  float dphi=0;
  if (dphi1 > TMath::Pi()){
    dphi = TMath::Pi()*2. - dphi1;
  }
  else dphi = dphi1;
  
  float dr = sqrt(deta*deta + dphi*dphi);
  return dr;

}
