#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"


#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEtFwd.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEt.h"
#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateTMEtFwd.h"
#include <TMath.h>

using namespace edm;
using namespace std;



int main(int argc, char* argv[]) 
{
  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();
  


   std::vector<std::string> fileNames;
   std::ifstream fin("Datatmp.txt");
   string word;
   while (fin>>word) {fileNames.push_back(word);}
   fin.close();



  //fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatMCMinBiasMuonSkim_D/patTupleMuSkim_100_1.root");

  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_12_0.root");
  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_11_0.root");
//fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_13_0.root");
//fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_14_0.root");

   //  fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_B/patTupleMuSkim_10_1.root");
  //fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_11_1.root");
  //fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_12_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_13_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_14_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_15_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_16_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_17_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_18_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_19_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_1_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_20_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_21_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_22_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_23_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_24_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_25_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_26_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_27_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_28_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_29_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_2_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_30_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_31_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_32_0.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_3_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_4_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_5_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_6_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_7_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_8_1.root");
// fileNames.push_back("rfio:/castor/cern.ch/user/l/lusito/Data356/PatDataMuonSkimJSON_A/patTupleMuSkim_9_1.root");







 
  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService("DataNew.root");
  
  TFileDirectory theDir = fs.mkdir("analyzeBasicPat");
  
  TH1F* vtxN_ = theDir.make<TH1F>("vtxN","vtxN", 10, -0.5, 9.5);
  TH1F* vtxZ_ = theDir.make<TH1F>("vtxZ", "vtxZ", 100, -15, 15);
  TH1F* vtxNDOF_ = theDir.make<TH1F>("vtxNDOF", "vtxNDOF", 100, 0, 250);
  TH1F* vtxNTrk_ = theDir.make<TH1F>("vtxNTrk", "vtxNTrk", 100, 0, 200);
  TH1F* vtxPChi2_ = theDir.make<TH1F>("vtxPChi2", "vtxPChi2", 50, 0, 1);
  
  TH1F* muonPt_  = theDir.make<TH1F>("muonPt", "pt",    50,  0.,30.);
  TH1F* muonEta_ = theDir.make<TH1F>("muonEta","eta",   100, -3.,  3.);
  TH1F* muonPhi_ = theDir.make<TH1F>("muonPhi","phi",   100, -5.,  5.);
  TH1F* muonN_= theDir.make<TH1F>("muonN","N",   6, -0.5,  5.5);  
  TH1F* muonCharge_ = theDir.make<TH1F>("muonCharge", "muonCharge", 5, -2.5, 2.5);
  TH1F* muonTracker_  = theDir.make<TH1F>("muonTracker","muonTracker", 2, -0.5, 1.5);
  TH1F* muonGlobal_  = theDir.make<TH1F>("muonGlobal","muonGlobal", 2, -0.5, 1.5);
  
  TH1F* muonEtaTracker_ = theDir.make<TH1F>("muonEtaTracker","etaTracker",   100, -3.,  3.);
  TH1F* muonEtaGlobal_ = theDir.make<TH1F>("muonEtaGlobal","etaGlobal",   100, -3.,  3.);
  TH1F* muonPhiTracker_ = theDir.make<TH1F>("muonPhiTracker","phiTracker",   100, -5.,  5.);
  TH1F* muonPhiGlobal_ = theDir.make<TH1F>("muonPhiGlobal","phiGlobal",   100, -5.,  5.);
  TH1F* muonPtTracker_  = theDir.make<TH1F>("muonPtTracker", "ptTracker",    50,  0.,30.);
  TH1F* muonPtGlobal_  = theDir.make<TH1F>("muonPtGlobal", "ptGlobal",    50,  0.,30.);
  
  
  TH1F* muonCaloCompatibility_ = theDir.make<TH1F>("muonCaloCompatibility", "CaloCompatibility", 50, 0, 1.2);
  TH1F* muonSegmentCompatibility_ = theDir.make<TH1F>("muonSegCompatibility", "SegCompatibility", 50, 0, 1.2);
  TH1F* muonIso05emEt_ = theDir.make<TH1F>("muonIso05emEt", "Iso05emEt", 50, 0, 10);
  TH1F* muonIso05hadEt_ = theDir.make<TH1F>("muonIso05hadEt", "Iso05hadEt", 50, 0, 5);
  TH1F* muonIso05sumPt_ = theDir.make<TH1F>("muonIso05sumPt", "Iso05sumPt", 50, 0, 6);
  TH1F* muonTrackIPxy_ = theDir.make<TH1F>("muonTrackIPxy", "TrackIPxy",100, -0.100, 0.100);
  TH1F* muonNChambers_ = theDir.make<TH1F>("muonNChambers", "NChambers",25, -0.5, 24.5);
  
  TH1F* tauN_ = theDir.make<TH1F>("tauN","tauN",   50, -0.5,  49.5);
  TH1F* tauPt_  = theDir.make<TH1F>("tauPt", "pt",    50,  0.,30.);
  TH1F* tauEta_ = theDir.make<TH1F>("tauEta","eta",   100, -3.,  3.);
  TH1F* tauPhi_ = theDir.make<TH1F>("tauPhi","phi",   100, -5.,  5.);
  TH1F* tauCharge_ = theDir.make<TH1F>("tauCharge","tauCharge", 11, -5.5, 5.5);
  TH1F* tauProngs_ = theDir.make<TH1F>("tauProngs","tauProngs", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt_ = theDir.make<TH1F>("tauLdgTrkPt","tauLdgTrkPt", 50, 0, 30);
  TH1F* tauDAEle_ = theDir.make<TH1F>("tauDAEle","tauDAEle", 2, -0.5, 1.5);
  TH1F* tauDAMu_ = theDir.make<TH1F>("tauDAMu","tauDAMu", 2, -0.5, 1.5);
  TH1F* tauDEcalIso_ = theDir.make<TH1F>("tauDEcalIso","tauDEcalIso", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso_ = theDir.make<TH1F>("tauDLdgPionIso","tauDLdgPionIso", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt_ = theDir.make<TH1F>("tauDLdgPionPt","tauDLdgPionPt", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind_ = theDir.make<TH1F>("tauDLdgTrkFind","tauDLdgTrkFind", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt_ = theDir.make<TH1F>("tauDLdgTrkPt","tauDLdgTrkPt", 2, -0.5, 1.5);
  TH1F* tauDTaNC_ = theDir.make<TH1F>("tauDTaNC","tauDTaNC", 2, -0.5, 1.5);
  TH1F* tauDTrkIso_ = theDir.make<TH1F>("tauDTrkIso","tauDTrkIso", 2, -0.5, 1.5);
 
 
  TH1F* tauEta010_ = theDir.make<TH1F>("tauEta010","eta010",   100, -3.,  3.);
  TH1F* tauPhi010_ = theDir.make<TH1F>("tauPhi010","phi010",   100, -5.,  5.);
  TH1F* tauCharge010_ = theDir.make<TH1F>("tauCharge010","tauCharge010", 11, -5.5, 5.5);
  TH1F* tauProngs010_ = theDir.make<TH1F>("tauProngs010","tauProngs010", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt010_ = theDir.make<TH1F>("tauLdgTrkPt010","tauLdgTrkPt010", 50, 0, 30);
  TH1F* tauDAEle010_ = theDir.make<TH1F>("tauDAEle010","tauDAEle010", 2, -0.5, 1.5);
  TH1F* tauDAMu010_ = theDir.make<TH1F>("tauDAMu010","tauDAMu010", 2, -0.5, 1.5);
  TH1F* tauDEcalIso010_ = theDir.make<TH1F>("tauDEcalIso010","tauDEcalIso010", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso010_ = theDir.make<TH1F>("tauDLdgPionIso010","tauDLdgPionIso010", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt010_ = theDir.make<TH1F>("tauDLdgPionPt010","tauDLdgPionPt010", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind010_ = theDir.make<TH1F>("tauDLdgTrkFind010","tauDLdgTrkFind010", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt010_ = theDir.make<TH1F>("tauDLdgTrkPt010","tauDLdgTrkPt010", 2, -0.5, 1.5);
  TH1F* tauDTaNC010_ = theDir.make<TH1F>("tauDTaNC010","tauDTaNC010", 2, -0.5, 1.5);
  TH1F* tauDTrkIso010_ = theDir.make<TH1F>("tauDTrkIso010","tauDTrkIso010", 2, -0.5, 1.5);
  TH1F* tauN010_ = theDir.make<TH1F>("tauN010","tauN010",   30, -0.5,  29.5);

  TH1F* tauEta1015_ = theDir.make<TH1F>("tauEta1015","eta1015",   100, -3.,  3.);
  TH1F* tauPhi1015_ = theDir.make<TH1F>("tauPhi1015","phi1015",   100, -5.,  5.);
  TH1F* tauCharge1015_ = theDir.make<TH1F>("tauCharge1015","tauCharge1015", 11, -5.5, 5.5);
  TH1F* tauProngs1015_ = theDir.make<TH1F>("tauProngs1015","tauProngs1015", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt1015_ = theDir.make<TH1F>("tauLdgTrkPt1015","tauLdgTrkPt1015", 50, 0, 30);
  TH1F* tauDAEle1015_ = theDir.make<TH1F>("tauDAEle1015","tauDAEle1015", 2, -0.5, 1.5);
  TH1F* tauDAMu1015_ = theDir.make<TH1F>("tauDAMu1015","tauDAMu1015", 2, -0.5, 1.5);
  TH1F* tauDEcalIso1015_ = theDir.make<TH1F>("tauDEcalIso1015","tauDEcalIso1015", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso1015_ = theDir.make<TH1F>("tauDLdgPionIso1015","tauDLdgPionIso1015", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt1015_ = theDir.make<TH1F>("tauDLdgPionPt1015","tauDLdgPionPt1015", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind1015_ = theDir.make<TH1F>("tauDLdgTrkFind1015","tauDLdgTrkFind1015", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt1015_ = theDir.make<TH1F>("tauDLdgTrkPt1015","tauDLdgTrkPt1015", 2, -0.5, 1.5);
  TH1F* tauDTaNC1015_ = theDir.make<TH1F>("tauDTaNC1015","tauDTaNC1015", 2, -0.5, 1.5);
  TH1F* tauDTrkIso1015_ = theDir.make<TH1F>("tauDTrkIso1015","tauDTrkIso1015", 2, -0.5, 1.5);
 TH1F* tauN1015_ = theDir.make<TH1F>("tauN1015","tauN1015",   15, -0.5,  14.5);

  TH1F* tauEta1520_ = theDir.make<TH1F>("tauEta1520","eta1520",   100, -3.,  3.);
  TH1F* tauPhi1520_ = theDir.make<TH1F>("tauPhi1520","phi1520",   100, -5.,  5.);
  TH1F* tauCharge1520_ = theDir.make<TH1F>("tauCharge1520","tauCharge1520", 11, -5.5, 5.5);
  TH1F* tauProngs1520_ = theDir.make<TH1F>("tauProngs1520","tauProngs1520", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt1520_ = theDir.make<TH1F>("tauLdgTrkPt1520","tauLdgTrkPt1520", 50, 0, 30);
  TH1F* tauDAEle1520_ = theDir.make<TH1F>("tauDAEle1520","tauDAEle1520", 2, -0.5, 1.5);
  TH1F* tauDAMu1520_ = theDir.make<TH1F>("tauDAMu1520","tauDAMu1520", 2, -0.5, 1.5);
  TH1F* tauDEcalIso1520_ = theDir.make<TH1F>("tauDEcalIso1520","tauDEcalIso1520", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso1520_ = theDir.make<TH1F>("tauDLdgPionIso1520","tauDLdgPionIso1520", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt1520_ = theDir.make<TH1F>("tauDLdgPionPt1520","tauDLdgPionPt1520", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind1520_ = theDir.make<TH1F>("tauDLdgTrkFind1520","tauDLdgTrkFind1520", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt1520_ = theDir.make<TH1F>("tauDLdgTrkPt1520","tauDLdgTrkPt1520", 2, -0.5, 1.5);
  TH1F* tauDTaNC1520_ = theDir.make<TH1F>("tauDTaNC1520","tauDTaNC1520", 2, -0.5, 1.5);
  TH1F* tauDTrkIso1520_ = theDir.make<TH1F>("tauDTrkIso1520","tauDTrkIso1520", 2, -0.5, 1.5);
 TH1F* tauN1520_ = theDir.make<TH1F>("tauN1520","tauN1520",   10, -0.5,  9.5);

  TH1F* tauEta2025_ = theDir.make<TH1F>("tauEta2025","eta2025",   100, -3.,  3.);
  TH1F* tauPhi2025_ = theDir.make<TH1F>("tauPhi2025","phi2025",   100, -5.,  5.);
  TH1F* tauCharge2025_ = theDir.make<TH1F>("tauCharge2025","tauCharge2025", 11, -5.5, 5.5);
  TH1F* tauProngs2025_ = theDir.make<TH1F>("tauProngs2025","tauProngs2025", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt2025_ = theDir.make<TH1F>("tauLdgTrkPt2025","tauLdgTrkPt2025", 50, 0, 30);
  TH1F* tauDAEle2025_ = theDir.make<TH1F>("tauDAEle2025","tauDAEle2025", 2, -0.5, 1.5);
  TH1F* tauDAMu2025_ = theDir.make<TH1F>("tauDAMu2025","tauDAMu2025", 2, -0.5, 1.5);
  TH1F* tauDEcalIso2025_ = theDir.make<TH1F>("tauDEcalIso2025","tauDEcalIso2025", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso2025_ = theDir.make<TH1F>("tauDLdgPionIso2025","tauDLdgPionIso2025", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt2025_ = theDir.make<TH1F>("tauDLdgPionPt2025","tauDLdgPionPt2025", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind2025_ = theDir.make<TH1F>("tauDLdgTrkFind2025","tauDLdgTrkFind2025", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt2025_ = theDir.make<TH1F>("tauDLdgTrkPt2025","tauDLdgTrkPt2025", 2, -0.5, 1.5);
  TH1F* tauDTaNC2025_ = theDir.make<TH1F>("tauDTaNC2025","tauDTaNC2025", 2, -0.5, 1.5);
  TH1F* tauDTrkIso2025_ = theDir.make<TH1F>("tauDTrkIso2025","tauDTrkIso2025", 2, -0.5, 1.5);
 TH1F* tauN2025_ = theDir.make<TH1F>("tauN2025","tauN2025",   10, -0.5,  9.5);

  TH1F* tauEta2530_ = theDir.make<TH1F>("tauEta2530","eta2530",   100, -3.,  3.);
  TH1F* tauPhi2530_ = theDir.make<TH1F>("tauPhi2530","phi2530",   100, -5.,  5.);
  TH1F* tauCharge2530_ = theDir.make<TH1F>("tauCharge2530","tauCharge2530", 11, -5.5, 5.5);
  TH1F* tauProngs2530_ = theDir.make<TH1F>("tauProngs2530","tauProngs2530", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt2530_ = theDir.make<TH1F>("tauLdgTrkPt2530","tauLdgTrkPt2530", 50, 0, 30);
  TH1F* tauDAEle2530_ = theDir.make<TH1F>("tauDAEle2530","tauDAEle2530", 2, -0.5, 1.5);
  TH1F* tauDAMu2530_ = theDir.make<TH1F>("tauDAMu2530","tauDAMu2530", 2, -0.5, 1.5);
  TH1F* tauDEcalIso2530_ = theDir.make<TH1F>("tauDEcalIso2530","tauDEcalIso2530", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso2530_ = theDir.make<TH1F>("tauDLdgPionIso2530","tauDLdgPionIso2530", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt2530_ = theDir.make<TH1F>("tauDLdgPionPt2530","tauDLdgPionPt2530", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind2530_ = theDir.make<TH1F>("tauDLdgTrkFind2530","tauDLdgTrkFind2530", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt2530_ = theDir.make<TH1F>("tauDLdgTrkPt2530","tauDLdgTrkPt2530", 2, -0.5, 1.5);
  TH1F* tauDTaNC2530_ = theDir.make<TH1F>("tauDTaNC2530","tauDTaNC2530", 2, -0.5, 1.5);
  TH1F* tauDTrkIso2530_ = theDir.make<TH1F>("tauDTrkIso2530","tauDTrkIso2530", 2, -0.5, 1.5);
 TH1F* tauN2530_ = theDir.make<TH1F>("tauN2530","tauN2530",   10, -0.5,  9.5);

  TH1F* tauEta30ON_ = theDir.make<TH1F>("tauEta30ON","eta30ON",   100, -3.,  3.);
  TH1F* tauPhi30ON_ = theDir.make<TH1F>("tauPhi30ON","phi30ON",   100, -5.,  5.);
  TH1F* tauCharge30ON_ = theDir.make<TH1F>("tauCharge30ON","tauCharge30ON", 11, -5.5, 5.5);
  TH1F* tauProngs30ON_ = theDir.make<TH1F>("tauProngs30ON","tauProngs30ON", 11, -5.5, 5.5);
  TH1F* tauLdgTrkPt30ON_ = theDir.make<TH1F>("tauLdgTrkPt30ON","tauLdgTrkPt30ON", 50, 0, 30);
  TH1F* tauDAEle30ON_ = theDir.make<TH1F>("tauDAEle30ON","tauDAEle30ON", 2, -0.5, 1.5);
  TH1F* tauDAMu30ON_ = theDir.make<TH1F>("tauDAMu30ON","tauDAMu30ON", 2, -0.5, 1.5);
  TH1F* tauDEcalIso30ON_ = theDir.make<TH1F>("tauDEcalIso30ON","tauDEcalIso30ON", 2, -0.5, 1.5);
  TH1F* tauDLdgPionIso30ON_ = theDir.make<TH1F>("tauDLdgPionIso30ON","tauDLdgPionIso30ON", 2, -0.5, 1.5);
  TH1F* tauDLdgPionPt30ON_ = theDir.make<TH1F>("tauDLdgPionPt30ON","tauDLdgPionPt30ON", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkFind30ON_ = theDir.make<TH1F>("tauDLdgTrkFind30ON","tauDLdgTrkFind30ON", 2, -0.5, 1.5);
  TH1F* tauDLdgTrkPt30ON_ = theDir.make<TH1F>("tauDLdgTrkPt30ON","tauDLdgTrkPt30ON", 2, -0.5, 1.5);
  TH1F* tauDTaNC30ON_ = theDir.make<TH1F>("tauDTaNC30ON","tauDTaNC30ON", 2, -0.5, 1.5);
  TH1F* tauDTrkIso30ON_ = theDir.make<TH1F>("tauDTrkIso30ON","tauDTrkIso30ON", 2, -0.5, 1.5);
 TH1F* tauN30ON_ = theDir.make<TH1F>("tauN30ON","tauN30ON",   10, -0.5,  9.5);
  
  TH1F* metEt_  = theDir.make<TH1F>("metEt", "Et",    100,  0.,100.);
  TH1F* metPt_  = theDir.make<TH1F>("metPt", "pt",    50,  0.,30.);
  TH1F* metEta_ = theDir.make<TH1F>("metEta","eta",   100, -3.,  3.);
  TH1F* metPhi_ = theDir.make<TH1F>("metPhi","phi",   100, -5.,  5.);
  TH1F* PFmetEt_  = theDir.make<TH1F>("PFmetEt", "PFEt",    100,  0.,100.);
  TH1F* PFmetPhi_ = theDir.make<TH1F>("PFmetPhi","PFphi",   100, -5.,  5.);
  
  TH1F* jetN_ = theDir.make<TH1F>("jetN","jetN",   50, -0.5,  49.5);
  TH1F* jetNTrk_ = theDir.make<TH1F>("jetNTrk","jetNTrk",   70, 0,  70);
  TH1F* jetPt_  = theDir.make<TH1F>("jetPt", "pt",    30,  0.,30.);
  TH1F* jetEta_ = theDir.make<TH1F>("jetEta","eta",   100, -3.,  3.);
  TH1F* jetPhi_ = theDir.make<TH1F>("jetPhi","phi",   100, -5.,  5.); 
  

  TH1F* jetEta010_ = theDir.make<TH1F>("jetEta010","eta010",   100, -3.,  3.);
  TH1F* jetEta1015_ = theDir.make<TH1F>("jetEta1015","eta1015",   100, -3.,  3.);
  TH1F* jetEta1520_ = theDir.make<TH1F>("jetEta1520","eta1520",   100, -3.,  3.);
  TH1F* jetEta2025_ = theDir.make<TH1F>("jetEta2025","eta2025",   100, -3.,  3.);
  TH1F* jetEta2530_ = theDir.make<TH1F>("jetEta2530","eta2530",   100, -3.,  3.);
  TH1F* jetEta30ON_ = theDir.make<TH1F>("jetEta30ON","eta30ON",   100, -3.,  3.);
  
  TH1F* jetNTrk010_ = theDir.make<TH1F>("jetNTrk010","jetNTrk010",   70, 0,  70);
  TH1F* jetNTrk1015_ = theDir.make<TH1F>("jetNTrk1015","jetNTrk1015",   70, 0,  70);
  TH1F* jetNTrk1520_ = theDir.make<TH1F>("jetNTrk1520","jetNTrk1520",   70, 0,  70);
  TH1F* jetNTrk2025_ = theDir.make<TH1F>("jetNTrk2025","jetNTrk2025",   70, 0,  70);
  TH1F* jetNTrk2530_ = theDir.make<TH1F>("jetNTrk2530","jetNTrk2530",   70, 0, 70);
  TH1F* jetNTrk30ON_ = theDir.make<TH1F>("jetNTrk30ON","jetNTrk30ON",   70, 0,  70);
  
  TH1F* jetPhi010_ = theDir.make<TH1F>("jetPhi010","phi010",   100, -5.,  5.); 
  TH1F* jetPhi1015_ = theDir.make<TH1F>("jetPhi1015","phi1015",   100, -5.,  5.); 
  TH1F* jetPhi1520_ = theDir.make<TH1F>("jetPhi1520","phi1520",   100, -5.,  5.); 
  TH1F* jetPhi2025_ = theDir.make<TH1F>("jetPhi2025","phi2025",   100, -5.,  5.); 
  TH1F* jetPhi2530_ = theDir.make<TH1F>("jetPhi2530","phi2530",   100, -5.,  5.); 
  TH1F* jetPhi30ON_ = theDir.make<TH1F>("jetPhi30ON","phi30ON",   100, -5.,  5.); 

TH1F* jetN010_ = theDir.make<TH1F>("jetN010","jetN010",   30, -0.5,  29.5);
TH1F* jetN1015_ = theDir.make<TH1F>("jetN1015","jetN1015",   15, -0.5,  14.5);
TH1F* jetN1520_ = theDir.make<TH1F>("jetN1520","jetN1520",   10, -0.5,  9.5);
TH1F* jetN2025_ = theDir.make<TH1F>("jetN2025","jetN2025",   10, -0.5,  9.5);
TH1F* jetN2530_ = theDir.make<TH1F>("jetN2530","jetN2530",   10, -0.5,  9.5);
TH1F* jetN30ON_ = theDir.make<TH1F>("jetN30ON","jetN30ON",   10, -0.5,  9.5);

  TH2F* jetEtaVsPt_ = theDir.make<TH2F>("jetEtaVsPt","EtaVsPt", 50,  0.,30.,100, -3.,  3.);  
  TH2F* jetPhiVsPt_ = theDir.make<TH2F>("jetPhiVsPt","PhiVsPt", 50,  0.,30.,100, -3.,  3.);  
  
  TH1F* pfjetN_ = theDir.make<TH1F>("pfjetN","pfjetN",   50, -0.5,  49.5);
  TH1F* pfjetNTrk_ = theDir.make<TH1F>("pfjetNTrk","pfjetNTrk",   70, 0,  70);
  TH1F* pfjetPt_  = theDir.make<TH1F>("pfjetPt", "pt",    50,  0.,30.);
  TH1F* pfjetEta_ = theDir.make<TH1F>("pfjetEta","eta",   100, -3.,  3.);
  TH1F* pfjetPhi_ = theDir.make<TH1F>("pfjetPhi","phi",   100, -5.,  5.); 
  
  TH1F* pfjetEta010_ = theDir.make<TH1F>("pfjetEta010","pfeta010",   100, -3.,  3.);
  TH1F* pfjetEta1015_ = theDir.make<TH1F>("pfjetEta1015","pfeta1015",   100, -3.,  3.);
  TH1F* pfjetEta1520_ = theDir.make<TH1F>("pfjetEta1520","pfeta1520",   100, -3.,  3.);
  TH1F* pfjetEta2025_ = theDir.make<TH1F>("pfjetEta2025","pfeta2025",   100, -3.,  3.);
  TH1F* pfjetEta2530_ = theDir.make<TH1F>("pfjetEta2530","pfeta2530",   100, -3.,  3.);
  TH1F* pfjetEta30ON_ = theDir.make<TH1F>("pfjetEta30ON","pfeta30ON",   100, -3.,  3.);
 
  TH1F* pfjetPhi010_ = theDir.make<TH1F>("pfjetPhi010","pfphi010",   100, -5.,  5.); 
  TH1F* pfjetPhi1015_ = theDir.make<TH1F>("pfjetPhi1015","pfphi1015",   100, -5.,  5.); 
  TH1F* pfjetPhi1520_ = theDir.make<TH1F>("pfjetPhi1520","pfphi1520",   100, -5.,  5.); 
  TH1F* pfjetPhi2025_ = theDir.make<TH1F>("pfjetPhi2025","pfphi2025",   100, -5.,  5.); 
  TH1F* pfjetPhi2530_ = theDir.make<TH1F>("pfjetPhi2530","pfphi2530",   100, -5.,  5.); 
  TH1F* pfjetPhi30ON_ = theDir.make<TH1F>("pfjetPhi30ON","pfphi30ON",   100, -5.,  5.); 

  TH1F* pfjetNTrk010_ = theDir.make<TH1F>("pfjetNTrk010","pfjetNTrk010",   70, 0,  70);
  TH1F* pfjetNTrk1015_ = theDir.make<TH1F>("pfjetNTrk1015","pfjetNTrk1015",   70, 0,  70);
  TH1F* pfjetNTrk1520_ = theDir.make<TH1F>("pfjetNTrk1520","pfjetNTrk1520",   70, 0,  70);
  TH1F* pfjetNTrk2025_ = theDir.make<TH1F>("pfjetNTrk2025","pfjetNTrk2025",   70, 0,  70);
  TH1F* pfjetNTrk2530_ = theDir.make<TH1F>("pfjetNTrk2530","pfjetNTrk2530",   70, 0,  70);
  TH1F* pfjetNTrk30ON_ = theDir.make<TH1F>("pfjetNTrk30ON","pfjetNTrk30ON",   70, 0,  70);
  
  TH2F* pfjetEtaVsPt_ = theDir.make<TH2F>("pfjetEtaVsPt","EtaVsPt", 50,  0.,30.,100, -3.,  3.);  
  TH2F* pfjetPhiVsPt_ = theDir.make<TH2F>("pfjetPhiVsPt","PhiVsPt", 50,  0.,30.,100, -3.,  3.);  

TH1F* pfjetN010_ = theDir.make<TH1F>("pfjetN010","pfjetN010",   30, -0.5,  29.5);
TH1F* pfjetN1015_ = theDir.make<TH1F>("pfjetN1015","pfjetN1015",   15, -0.5,  14.5);
TH1F* pfjetN1520_ = theDir.make<TH1F>("pfjetN1520","pfjetN1520",   10, -0.5,  9.5);
TH1F* pfjetN2025_ = theDir.make<TH1F>("pfjetN2025","pfjetN2025",   10, -0.5,  9.5);
TH1F* pfjetN2530_ = theDir.make<TH1F>("pfjetN2530","pfjetN2530",   10, -0.5,  9.5);
TH1F* pfjetN30ON_ = theDir.make<TH1F>("pfjetN30ON","pfjetN30ON",   10, -0.5,  9.5);

  TH1F* pairDPhi_ = theDir.make<TH1F>("pairDPhi","pairDPhi",100, -5.,  5.);
  TH1F* pairVisMass_ = theDir.make<TH1F>("pairVisMass","pairVisMass", 100, 0, 100);
  TH1F* pairDR_ = theDir.make<TH1F>("pairDR","pairDR", 50, 0, 6);
  TH1F* pairCharge_ = theDir.make<TH1F>("pairCharge","pairCharge",5, -2.5, 2.5);
  TH1F* muMETMT_ = theDir.make<TH1F>("muMETMT","muMETMT", 80, 0, 80);
  TH1F* PzetaVar_ = theDir.make<TH1F>("PzetaVar","PzetaVar", 100, -100, 100);
  TH1F* pairPt_  = theDir.make<TH1F>("diTauPt", "pt",    50,  0.,30.);
  TH1F* pairEta_ = theDir.make<TH1F>("diTauEta","eta",   100, -3.,  3.);
  TH1F* pairPhi_ = theDir.make<TH1F>("diTauPhi","phi",   100, -5.,  5.); 

  TH1F* event_ = theDir.make<TH1F>("evt","evt",   5000, 5000,  10000.); 
  TH1F* run_ = theDir.make<TH1F>("run","run",   1000, 133000.,  134000.); 



 
  //h2->ProjectionX("px", 0, 9); // where firstYbin = 0 and lastYbin = 9
  //px->Draw();
// TH1D *PtProj5_ = theDir.make<TH1D>("PtProj5","PtProj5",   100, -20.,  20.);
// TH1D *PtProj10_ = theDir.make<TH1D>("PtProj10","PtProj10",   100, -20.,  20.);
// TH1D *PtProj15_ = theDir.make<TH1D>("PtProj15","PtProj15",   100, -3.,  3.);
// TH1D *PtProj20_ = theDir.make<TH1D>("PtProj20","PtProj20",   100, -3.,  3.);
// TH1D *PtProj25_ = theDir.make<TH1D>("PtProj25","PtProj25",   100, -3.,  3.);
// TH1D *PtProj30_ = theDir.make<TH1D>("PtProj30","PtProj30",   100, -3.,  3.);

//TProfile* jetEtaVsPtProfX_ = theDir.make<TProfile>("jetEtaVsPtProfX", "jetEtaVsPtProfX",50,  0.,30., -3.,  3.); 
//TH1D *jetEtaVsPtProjY_py= theDir.make<TH1D>("jetEtaVsPtProjY_py", "jetEtaVsPtProjY_py",100,  -3.,  3.);



  // open input file (can be located on castor)
  //TFile* inFile = TFile::Open( "rfio:/castor/cern.ch/user/l/lusito/MCMerged1.root" );
  //TFile* inFile = TFile::Open( "rfio:/castor/cern.ch/user/l/lusito/DataMerged2.root" );



  // ----------------------------------------------------------------------
  // Second Part: 
  //
  //  * loop the events in the input file 
  //  * receive the collections of interest via fwlite::Handle
  //  * fill the histograms
  //  * after the loop close the input file
  // ----------------------------------------------------------------------

  // loop the events
  unsigned int iEvent=0;
  //fwlite::Event ev(inFile);
  fwlite::ChainEvent ev(fileNames);
  //fwlite::ChainEvent ev();
  //for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
  for(ev.toBegin(); !ev.atEnd(); ++ev){
    
    std::cout << "  processing event: " << iEvent << std::endl;
    
    edm::EventBase & event = ev;
    
    
    unsigned int EVENT  = ev.id().event();
    std::cout<<"EVENT = " << event.id().event() <<std::endl;
    unsigned int  RUN    = event.id().run();
    std::cout<<"RUN = " << ev.id().run() <<std::endl;
    ++iEvent;

    event_ -> Fill (EVENT);
    run_ -> Fill (RUN);
    
    //edm::EventBase const & event = ev;
    //fwlite::Event  event = ev;
    // break loop after end of file is reached 
    // or after 1000 events have been processed
    //if( iEvent==500) break;
    
    edm::Handle<std::vector<pat::Muon> > muons;
    edm::InputTag muonLabel("patMuons");
    event.getByLabel(muonLabel, muons);
    
    edm::Handle<std::vector<pat::Tau> > taus;
    edm::InputTag tauLabel("patTaus");
    event.getByLabel(tauLabel, taus);
    
    edm::Handle<std::vector<PATMuTauPair> > diTaus;
    edm::InputTag diTauLabel("allMuTauPairs");
    event.getByLabel(diTauLabel, diTaus);
    
    edm::Handle<std::vector<pat::MET> > pfmet;
    edm::InputTag pfmetLabel("patMETsPF");
    event.getByLabel(pfmetLabel, pfmet);
    
    edm::Handle<std::vector<pat::MET> > met;
    edm::InputTag metLabel("patMETs");
    event.getByLabel(metLabel, met);
    
    edm::Handle<std::vector<pat::Jet> > jets;
    edm::InputTag jetLabel("patJets");
    event.getByLabel(jetLabel, jets);
    
    edm::Handle<std::vector<reco::Vertex> > vertices;
    edm::InputTag vertexLabel("offlinePrimaryVertices");
    event.getByLabel(vertexLabel, vertices);

    edm::Handle<std::vector<pat::Jet> > pfjets;
    edm::InputTag pfjetLabel("selectedPatJetsAK5PF");
    event.getByLabel(pfjetLabel, pfjets);


    
    vtxN_ ->Fill ( vertices->size() );
    if ( vertices->size() >= 1 ) {
      const reco::Vertex& thePrimaryEventVertex = (*vertices->begin());
      vtxZ_ -> Fill( thePrimaryEventVertex.z() );
      vtxNDOF_ -> Fill( thePrimaryEventVertex.ndof() );
      vtxNTrk_ -> Fill( thePrimaryEventVertex.tracksSize() );
      vtxPChi2_ -> Fill( TMath::Prob(thePrimaryEventVertex.chi2(), TMath::Nint(thePrimaryEventVertex.ndof())) );
    }



    muonN_->Fill (muons->size());
    // loop muon collection and fill histograms
    for(unsigned i=0; i<muons->size(); ++i){
      muonPt_ ->Fill( (*muons)[i].pt()  );
      muonEta_->Fill( (*muons)[i].eta() );
      muonPhi_->Fill( (*muons)[i].phi() );
      muonCharge_->Fill( (*muons)[i].charge() );
      muonTracker_->Fill( (*muons)[i].isTrackerMuon() );
      muonGlobal_->Fill( (*muons)[i].isGlobalMuon() );
      
      if((*muons)[i].isTrackerMuon()){
	muonEtaTracker_->Fill( (*muons)[i].eta() ); 
	muonPhiTracker_->Fill( (*muons)[i].phi() );
	muonPtTracker_ ->Fill( (*muons)[i].pt()  );
      }
      
      if((*muons)[i].isGlobalMuon()){
	muonEtaGlobal_->Fill( (*muons)[i].eta() ); 
	muonPhiGlobal_->Fill( (*muons)[i].phi() );
	muonPtGlobal_ ->Fill( (*muons)[i].pt()  );
      }
      

      muonNChambers_->Fill( (*muons)[i].numberOfChambers()  );
      muonCaloCompatibility_->Fill( (*muons)[i].caloCompatibility()  );
      double segmentCompatibility = muon::segmentCompatibility((*muons)[i]);
      muonSegmentCompatibility_->Fill( segmentCompatibility  );
      
      if ( (vertices->size() >= 1)){
	const reco::Vertex& thePrimaryEventVertex = (*vertices->begin());
	if((*muons)[i].track().isNonnull()){
	  muonTrackIPxy_->Fill( ((*muons)[i].track())->dxy(thePrimaryEventVertex.position())  );
	}
      }
      
      muonIso05emEt_->Fill( (*muons)[i].isolationR05().emEt  ); 
      muonIso05hadEt_->Fill( (*muons)[i].isolationR05().hadEt  );
      muonIso05sumPt_->Fill( (*muons)[i].isolationR05().sumPt  );
    }
   
    float taun010_ = 0;
float taun1015_ = 0;
float taun1520_ = 0;
float taun2025_ = 0;
float taun2530_ = 0;
float taun30ON_ = 0;


    tauN_ ->Fill (taus->size());
    for(unsigned i=0; i<taus->size(); ++i){
      tauPt_ ->Fill( (*taus)[i].pt()  );
      tauEta_->Fill( (*taus)[i].eta() );
      tauPhi_->Fill( (*taus)[i].phi() ); 
      tauCharge_->Fill((*taus)[i].charge()  );
      tauProngs_ ->Fill( (*taus)[i].signalTracks().size()  );
      if((*taus)[i].leadTrack().isNonnull()){
	tauLdgTrkPt_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
      }
      tauDAEle_ ->Fill( (*taus)[i].tauID("againstElectron")  );
      tauDAMu_ ->Fill( (*taus)[i].tauID("againstMuon")  );
      tauDEcalIso_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
      tauDLdgPionIso_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
      tauDLdgPionPt_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
      tauDLdgTrkFind_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
      tauDLdgTrkPt_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
      tauDTaNC_ ->Fill( (*taus)[i].tauID("byTaNC")  );
      tauDTrkIso_ ->Fill( (*taus)[i].tauID("trackIsolation")  );
      

      if (((*taus)[i].pt()<=10)){
	if (fabs((*taus)[i].eta())<=2.4) {
++taun010_;
// std::cout << "tau con pt 0-10 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
 //std::cout << " taun010_ = "<< taun010_ << std::endl;
	}
	tauEta010_->Fill( (*taus)[i].eta() );
	tauPhi010_->Fill( (*taus)[i].phi() ); 
	tauCharge010_->Fill((*taus)[i].charge()  );
	tauProngs010_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt010_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle010_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu010_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso010_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso010_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt010_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind010_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt010_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC010_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso010_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }


      if (((*taus)[i].pt()>10) && ((*taus)[i].pt()<=15)){
	if (abs((*taus)[i].eta())<=2.4) {
++taun1015_;
 //std::cout << "tau con pt 10-15 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
// std::cout << " taun1015_ = "<< taun1015_ << std::endl;
	}
	tauEta1015_->Fill( (*taus)[i].eta() );
	tauPhi1015_->Fill( (*taus)[i].phi() ); 
	tauCharge1015_->Fill((*taus)[i].charge()  );
	tauProngs1015_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt1015_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle1015_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu1015_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso1015_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso1015_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt1015_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind1015_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt1015_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC1015_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso1015_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }



      if (((*taus)[i].pt()<=20) && ((*taus)[i].pt()>15)){
	if (abs((*taus)[i].eta())<=2.4){
++taun1520_;
// std::cout << "tau con pt 15-20 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
// std::cout << " taun1520_ = "<< taun1520_ << std::endl;
	}
	tauEta1520_->Fill( (*taus)[i].eta() );
	tauPhi1520_->Fill( (*taus)[i].phi() ); 
	tauCharge1520_->Fill((*taus)[i].charge()  );
	tauProngs1520_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt1520_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle1520_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu1520_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso1520_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso1520_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt1520_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind1520_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt1520_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC1520_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso1520_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }




      if (((*taus)[i].pt()>20)&&((*taus)[i].pt()<=25)){
	if (abs((*taus)[i].eta())<=2.4){
++taun2025_;
// std::cout << "tau con pt 20-25 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
// std::cout << " taun2025_ = "<< taun2025_ << std::endl;
	}
	tauEta2025_->Fill( (*taus)[i].eta() );
	tauPhi2025_->Fill( (*taus)[i].phi() ); 
	tauCharge2025_->Fill((*taus)[i].charge()  );
	tauProngs2025_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt2025_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle2025_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu2025_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso2025_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso2025_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt2025_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind2025_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt2025_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC2025_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso2025_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }

      if (((*taus)[i].pt()>25)&&((*taus)[i].pt()<=30)){
	if (abs((*taus)[i].eta())<=2.4) {
++taun2530_;
// std::cout << "tau con pt 25-30 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
// std::cout << " taun2530_ = "<< taun2530_ << std::endl;
	}
	tauEta2530_->Fill( (*taus)[i].eta() );
	tauPhi2530_->Fill( (*taus)[i].phi() ); 
	tauCharge2530_->Fill((*taus)[i].charge()  );
	tauProngs2530_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt2530_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle2530_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu2530_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso2530_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso2530_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt2530_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind2530_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt2530_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC2530_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso2530_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }


      if (((*taus)[i].pt()>30)){
	if (abs((*taus)[i].eta())<=2.4) {
++taun30ON_;
// std::cout << "tau con pt > 30 ed eta < 2.4: "<< (*taus)[i].pt() << " " << (*taus)[i].eta() << std::endl;
// std::cout << " taun30ON_ = "<< taun30ON_ << std::endl;
	}
	tauEta30ON_->Fill( (*taus)[i].eta() );
	tauPhi30ON_->Fill( (*taus)[i].phi() ); 
	tauCharge30ON_->Fill((*taus)[i].charge()  );
	tauProngs30ON_ ->Fill( (*taus)[i].signalTracks().size()  );
	if((*taus)[i].leadTrack().isNonnull()){
	  tauLdgTrkPt30ON_ ->Fill( ((*taus)[i].leadTrack())->pt()  );
	}
	tauDAEle30ON_ ->Fill( (*taus)[i].tauID("againstElectron")  );
	tauDAMu30ON_ ->Fill( (*taus)[i].tauID("againstMuon")  );
	tauDEcalIso30ON_ ->Fill( (*taus)[i].tauID("ecalIsolation")  );
	tauDLdgPionIso30ON_ ->Fill( (*taus)[i].tauID("byIsolationUsingLeadingPion")  );
	tauDLdgPionPt30ON_ ->Fill( (*taus)[i].tauID("leadingPionPtCut")  );
	tauDLdgTrkFind30ON_ ->Fill( (*taus)[i].tauID("leadingTrackFinding")  );
	tauDLdgTrkPt30ON_ ->Fill( (*taus)[i].tauID("leadingTrackPtCut")  );
	tauDTaNC30ON_ ->Fill( (*taus)[i].tauID("byTaNC")  );
	tauDTrkIso30ON_ ->Fill( (*taus)[i].tauID("trackIsolation")  );   
      }


    }
    std::cout<< "Summary: " << taun010_ <<" " << taun1015_ <<" " <<taun1520_<<" " << taun2025_<<" " <<taun2530_<<" " << taun30ON_ << std::endl;
    tauN010_ ->Fill( taun010_ );
tauN1015_ ->Fill( taun1015_ );
tauN1520_ ->Fill( taun1520_ );
 tauN2025_ ->Fill( taun2025_ );
 tauN2530_ ->Fill( taun2530_ );
  tauN30ON_ ->Fill( taun30ON_ );
 
    for(unsigned i=0; i<met->size(); ++i){
      metPt_ ->Fill( (*met)[i].pt()  );
      metEt_ ->Fill( (*met)[i].et()  );
      metEta_->Fill( (*met)[i].eta() );
      metPhi_->Fill( (*met)[i].phi() );
      
    }
    
    for(unsigned i=0; i<pfmet->size(); ++i){
      //pfmetPt_ ->Fill( (*pfmet)[i].pt()  );
      PFmetEt_ ->Fill( (*pfmet)[i].et()  );
      //pfmetEta_->Fill( (*pfmet)[i].eta() );
      PFmetPhi_->Fill( (*pfmet)[i].phi() ); 
    }




 float jetn010_ = 0;
float jetn1015_ = 0;
float jetn1520_ = 0;
float jetn2025_ = 0;
float jetn2530_ = 0;
float jetn30ON_ = 0;


    jetN_ ->Fill(jets->size());
    for(unsigned i=0; i<jets->size(); ++i){
      jetPhiVsPt_ ->Fill( (*jets)[i].pt(), (*jets)[i].phi() );
      jetEtaVsPt_ ->Fill( (*jets)[i].pt(), (*jets)[i].eta() );
      jetEta_->Fill( (*jets)[i].eta() );
      jetNTrk_->Fill( (*jets)[i].associatedTracks().size());
      jetPhi_->Fill( (*jets)[i].phi() );
      jetPt_->Fill( (*jets)[i].pt() );

      if((*jets)[i].pt()<=10) {
	if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn010_;}
	jetEta010_->Fill( (*jets)[i].eta() );
	jetNTrk010_->Fill( (*jets)[i].associatedTracks().size());
	jetPhi010_->Fill( (*jets)[i].phi() );
      }

      if(((*jets)[i].pt()>10) && ((*jets)[i].pt()<=15) ) {
if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn1015_;}
	jetEta1015_->Fill( (*jets)[i].eta() );
	jetNTrk1015_->Fill( (*jets)[i].associatedTracks().size()); 
	jetPhi1015_->Fill( (*jets)[i].phi() );
      }

      if(((*jets)[i].pt()>15) && ((*jets)[i].pt()<=20) ) {
if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn1520_;}
	jetEta1520_->Fill( (*jets)[i].eta() );
	jetNTrk1520_->Fill( (*jets)[i].associatedTracks().size()); 
	jetPhi1520_->Fill( (*jets)[i].phi() );
      }

      if(((*jets)[i].pt()>20) && ((*jets)[i].pt()<=25) ) {
if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn2025_;}
	jetEta2025_->Fill( (*jets)[i].eta() );
	jetNTrk2025_->Fill( (*jets)[i].associatedTracks().size());
	jetPhi2025_->Fill( (*jets)[i].phi() );
      }
      
      if(((*jets)[i].pt()>25) && ((*jets)[i].pt()<=30) ) {
if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn2530_;}
	jetEta2530_->Fill( (*jets)[i].eta() );
	jetNTrk2530_->Fill( (*jets)[i].associatedTracks().size());
	jetPhi2530_->Fill( (*jets)[i].phi() );
      }
      
      if(((*jets)[i].pt()>30)  ) {
if (abs((*jets)[i].eta())<=2.4) {
	  ++jetn30ON_;}
	jetEta30ON_->Fill( (*jets)[i].eta() );
	jetNTrk30ON_->Fill( (*jets)[i].associatedTracks().size()); 
	jetPhi30ON_->Fill( (*jets)[i].phi() );
      }

    }
    
 std::cout<< "Summary JET: " << jetn010_ <<" " << jetn1015_ <<" " <<jetn1520_<<" " << jetn2025_<<" " <<jetn2530_<<" " << jetn30ON_ << std::endl;
    jetN010_ ->Fill( jetn010_ );
jetN1015_ ->Fill( jetn1015_ );
jetN1520_ ->Fill( jetn1520_ );
 jetN2025_ ->Fill( jetn2025_ );
 jetN2530_ ->Fill( jetn2530_ );
  jetN30ON_ ->Fill( jetn30ON_ );   





float pfjetn010_ = 0;
float pfjetn1015_ = 0;
float pfjetn1520_ = 0;
float pfjetn2025_ = 0;
float pfjetn2530_ = 0;
float pfjetn30ON_ = 0;

    pfjetN_ ->Fill(pfjets->size());
    for(unsigned i=0; i<pfjets->size(); ++i){
      pfjetEtaVsPt_->Fill( (*pfjets)[i].pt(), (*pfjets)[i].eta() );
      pfjetPhiVsPt_->Fill( (*pfjets)[i].pt(), (*pfjets)[i].phi() );
      pfjetPt_ ->Fill( (*pfjets)[i].pt()  );
      pfjetEta_->Fill( (*pfjets)[i].eta() );
      pfjetPhi_->Fill( (*pfjets)[i].phi() );
      pfjetNTrk_->Fill( (*pfjets)[i].associatedTracks().size());
      
      if((*pfjets)[i].pt()<=10) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn010_;}
	pfjetEta010_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk010_->Fill( (*pfjets)[i].associatedTracks().size());
	pfjetPhi010_->Fill( (*pfjets)[i].phi() );
      }

      if(((*pfjets)[i].pt()>10) && ((*pfjets)[i].pt()<=15) ) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn1015_;}
	pfjetEta1015_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk1015_->Fill( (*pfjets)[i].associatedTracks().size()); 
	pfjetPhi1015_->Fill( (*pfjets)[i].phi() );
      }

      if(((*pfjets)[i].pt()>15) && ((*pfjets)[i].pt()<=20) ) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn1520_;}
	pfjetEta1520_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk1520_->Fill( (*pfjets)[i].associatedTracks().size()); 
	pfjetPhi1520_->Fill( (*pfjets)[i].phi() );
      }

      if(((*pfjets)[i].pt()>20) && ((*pfjets)[i].pt()<=25) ) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn2025_;}
	pfjetEta2025_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk2025_->Fill( (*pfjets)[i].associatedTracks().size());
	pfjetPhi2025_->Fill( (*pfjets)[i].phi() );
      }
      
      if(((*pfjets)[i].pt()>25) && ((*pfjets)[i].pt()<=30) ) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn2530_;}
	pfjetEta2530_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk2530_->Fill( (*pfjets)[i].associatedTracks().size());
	pfjetPhi2530_->Fill( (*pfjets)[i].phi() );
      }
      
      if(((*pfjets)[i].pt()>30)  ) {
if (abs((*pfjets)[i].eta())<=2.4) {
	  ++pfjetn30ON_;}
	pfjetEta30ON_->Fill( (*pfjets)[i].eta() );
	pfjetNTrk30ON_->Fill( (*pfjets)[i].associatedTracks().size()); 
	pfjetPhi30ON_->Fill( (*pfjets)[i].phi() );
      }
    }
    
   std::cout<< "Summary PFJET: " << pfjetn010_ <<" " << pfjetn1015_ <<" " <<pfjetn1520_<<" " << pfjetn2025_<<" " <<pfjetn2530_<<" " << pfjetn30ON_ << std::endl;
    pfjetN010_ ->Fill( pfjetn010_ );
pfjetN1015_ ->Fill( pfjetn1015_ );
pfjetN1520_ ->Fill( pfjetn1520_ );
 pfjetN2025_ ->Fill( pfjetn2025_ );
 pfjetN2530_ ->Fill( pfjetn2530_ );
  pfjetN30ON_ ->Fill( pfjetn30ON_ ); 
    
    
    for(unsigned i=0; i<diTaus->size(); ++i){
      
      pairDPhi_ ->Fill( (*diTaus)[i].dPhi12() );
      pairVisMass_ ->Fill( (*diTaus)[i].p4Vis().mass() );
      pairDR_ ->Fill( (*diTaus)[i].dR12() );
      pairCharge_ ->Fill( (*diTaus)[i].charge() );
      muMETMT_ ->Fill( (*diTaus)[i].mt1MET() );
      PzetaVar_ ->Fill( (*diTaus)[i].pZeta()-1.5*(*diTaus)[i].pZetaVis() ); 
      pairPt_ ->Fill( (*diTaus)[i].pt() );
      pairEta_ ->Fill( (*diTaus)[i].eta() );
      pairPhi_ ->Fill( (*diTaus)[i].phi() );     
      
    }
    
  } 

 
  // close input file
  //inFile->Close();
  //ev.atEnd().Close();
  // ----------------------------------------------------------------------
  // Third Part: 
  //
  //  * never forget to free the memory of objects you created
  // ----------------------------------------------------------------------

  // in this example there is nothing to do 
  
  // that's it!
  return 0;
}




// // ////////////*****  DATA
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_100_0.root");
// //   // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_100_0.root");
// //   // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_100_0.root");
// //   //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_141_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_142_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_143_0.root");
// //   //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_100_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_100_0.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_101_0.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_103_0.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_104_0.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_105_0.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_106_3.root");
// //  fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_107_0.root");
// //  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_108_1.root");
// //  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_109_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_10_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_110_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_111_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_112_0.root");






// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_113_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_114_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_115_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_116_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_118_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_119_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_11_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_120_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_121_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_122_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_123_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_124_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_125_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_126_0.root");


// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_127_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_128_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_129_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_12_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_130_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_131_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_132_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_133_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_134_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_135_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_136_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_137_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_138_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_139_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_13_2.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_140_0.root");

// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_144_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_145_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_146_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_147_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_148_0.root");
// //  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_149_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_14_1.root");
// //  //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_150_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_151_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_152_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_153_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_154_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_155_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_156_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_157_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_158_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_159_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_160_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_161_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_162_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_163_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_164_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_165_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_166_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_167_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_168_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_169_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_16_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_170_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_171_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_172_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_173_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_174_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_175_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_176_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_177_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_178_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_179_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_17_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_180_2.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_181_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_182_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_183_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_184_2.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_185_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_186_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_187_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_188_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_18_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_1_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_23_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_24_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_25_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_26_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_27_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_28_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_29_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_2_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_30_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_31_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_32_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_34_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_35_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_36_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_37_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_38_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_39_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_3_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_40_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_41_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_42_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_43_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_44_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_46_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_47_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_48_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_49_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_4_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_50_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_51_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_52_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_53_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_54_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_55_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_56_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_57_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_58_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_59_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_5_2.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_60_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_61_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_62_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_63_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_64_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_65_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_66_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_67_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_68_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_69_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_6_1.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_70_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_71_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_72_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_73_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_74_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_75_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_76_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_77_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_78_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_79_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_7_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_80_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_81_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_82_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_83_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_84_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_85_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_87_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_88_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_89_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_8_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_91_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_92_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_93_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_94_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_95_0.root");
// // //fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_96_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_97_1.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_98_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_99_0.root");
// // fileNames.push_back("file:/tmp/lusito/Data/patTupleMuSkim_9_0.root");
