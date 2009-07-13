// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Muon.cc PATValidation/PATValidation/src/PATValidation_Muon.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>

//
// Original Author:  Sudhir_Malik
//         Created:  Fri Mar 13 09:52:17 CDT 2009
// $Id$
//
//
*/

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//my includes
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "PATValidation_Muon.h"

#include "TH1.h"
#include "TMath.h"


//using namespace edm;
using namespace reco;
using namespace std; 

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATValidation_Muon::PATValidation_Muon(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoMuon_    = iConfig.getParameter<edm::InputTag>("recoMuon");
patMuon_     = iConfig.getParameter<edm::InputTag>("patMuon");
outputFile_  = iConfig.getUntrackedParameter<std::string>("OutputFile");

etaBin       = iConfig.getParameter<int>("etaBin");
etaMin       = iConfig.getParameter<double>("etaMin");
etaMax       = iConfig.getParameter<double>("etaMax");

thetaBin     = iConfig.getParameter<int>("thetaBin");
thetaMin     = iConfig.getParameter<double>("thetaMin");
thetaMax     = iConfig.getParameter<double>("thetaMax");

phiBin       = iConfig.getParameter<int>("phiBin");
phiMin       = iConfig.getParameter<double>("phiMin");
phiMax       = iConfig.getParameter<double>("phiMax");

chi2Bin      = iConfig.getParameter<int>("chi2Bin");
chi2Min      = iConfig.getParameter<double>("chi2Min");
chi2Max      = iConfig.getParameter<double>("chi2Max");

pBin         = iConfig.getParameter<int>("pBin");
pMin         = iConfig.getParameter<double>("pMin");
pMax         = iConfig.getParameter<double>("pMax");

ptBin        = iConfig.getParameter<int>("ptBin");
ptMin        = iConfig.getParameter<double>("ptMin");
ptMax        = iConfig.getParameter<double>("ptMax");

pResBin      = iConfig.getParameter<int>("pResBin");
pResMin      = iConfig.getParameter<double>("pResMin");
pResMax      = iConfig.getParameter<double>("pResMax");

rhBin        = iConfig.getParameter<int>("rhBin");
rhMin        = iConfig.getParameter<double>("rhMin");
rhMax        = iConfig.getParameter<double>("rhMax");

//set histograms dimensuons
 // Set histogram dimensions
//////////


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAT/RECO histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " PAT/RECO histograms will NOT be saved";
     
}

PATValidation_Muon::~PATValidation_Muon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void PATValidation_Muon::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

//reco muons histgrams
      dbe->setCurrentFolder("Benchmarks/muons/RECO");
      me["RecoMuon_pt"] = dbe->book1D("pt", "RecoMuon", 50, 0, 200);
      me["RecoMuon_eta"]  = dbe->book1D("eta", "RecoMuon", 50, -6, 6);  
      me["RecoMuon_phi"]  = dbe->book1D("phi", "RecoMuon", 50, -4, 4);

   int binFactor = 4;
   // monitoring of eta parameter
  std::string histname = "GlbMuon_";
  muReco = dbe->book1D("muReco", "muon reconstructed tracks", 6, 1, 7); 
  etaGlbTrack.push_back(dbe->book1D(histname+"Glb_eta", "#eta_{GLB}", etaBin, etaMin, etaMax));
  etaGlbTrack.push_back(dbe->book1D(histname+"Tk_eta", "#eta_{TKfromGLB}", etaBin, etaMin, etaMax));
  etaGlbTrack.push_back(dbe->book1D(histname+"Sta_eta", "#eta_{STAfromGLB}", etaBin, etaMin, etaMax));
  etaResolution.push_back(dbe->book1D("Res_TkGlb_eta", "#eta_{TKfromGLB} - #eta_{GLB}", etaBin*binFactor, etaMin/3000, etaMax/3000));
  etaResolution.push_back(dbe->book1D("Res_GlbSta_eta", "#eta_{GLB} - #eta_{STAfromGLB}", etaBin*binFactor, etaMin/100, etaMax/100));
  etaResolution.push_back(dbe->book1D("Res_TkSta_eta", "#eta_{TKfromGLB} - #eta_{STAfromGLB}", etaBin*binFactor, etaMin/100, etaMax/100));
  etaResolution.push_back(dbe->book2D("ResVsEta_TkGlb_eta", "(#eta_{TKfromGLB} - #eta_{GLB}) vs #eta_{GLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/3000, etaMax/3000));
  etaResolution.push_back(dbe->book2D("ResVsEta_GlbSta_eta", "(#eta_{GLB} - #eta_{STAfromGLB}) vs #eta_{GLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/100, etaMax/100));
  etaResolution.push_back(dbe->book2D("ResVsEta_TkSta_eta", "(#eta_{TKfromGLB} - #eta_{STAfromGLB}) vs #eta_{TKfromGLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/100, etaMax/100));
  etaTrack = dbe->book1D("TkMuon_eta", "#eta_{TK}", etaBin, etaMin, etaMax);
  etaStaTrack = dbe->book1D("StaMuon_eta", "#eta_{STA}", etaBin, etaMin, etaMax);
  etaEfficiency.push_back(dbe->book1D("StaEta", "#eta_{STAfromGLB}", etaBin, etaMin, etaMax));
  etaEfficiency.push_back(dbe->book1D("StaEta_ifCombinedAlso", "#eta_{STAfromGLB} if isGlb=true", etaBin, etaMin, etaMax));

     
// monitoring of theta parameter
  thetaGlbTrack.push_back(dbe->book1D(histname+"Glb_theta", "#theta_{GLB}", thetaBin, thetaMin, thetaMax));
  thetaGlbTrack.push_back(dbe->book1D(histname+"Tk_theta", "#theta_{TKfromGLB}", thetaBin, thetaMin, thetaMax));
  thetaGlbTrack.push_back(dbe->book1D(histname+"Sta_theta", "#theta_{STAfromGLB}", thetaBin, thetaMin, thetaMax));
  thetaResolution.push_back(dbe->book1D("Res_TkGlb_theta", "#theta_{TKfromGLB} - #theta_{GLB}", thetaBin*binFactor, -(thetaMax/3000), thetaMax/3000));
  thetaResolution.push_back(dbe->book1D("Res_GlbSta_theta", "#theta_{GLB} - #theta_{STAfromGLB}", thetaBin*binFactor,-(thetaMax/100), thetaMax/100));
  thetaResolution.push_back(dbe->book1D("Res_TkSta_theta", "#theta_{TKfromGLB} - #theta_{STAfromGLB}", thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  thetaResolution.push_back(dbe->book2D("ResVsTheta_TkGlb_theta", "(#theta_{TKfromGLB} - #theta_{GLB}) vs #theta_{GLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/3000), thetaMax/3000));
  thetaResolution.push_back(dbe->book2D("ResVsTheta_GlbSta_theta", "(#theta_{GLB} - #theta_{STAfromGLB}) vs #theta_{GLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  thetaResolution.push_back(dbe->book2D("ResVsTheta_TkSta_theta", "(#theta_{TKfromGLB} - #theta_{STAfromGLB}) vs #theta_{TKfromGLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  thetaTrack = dbe->book1D("TkMuon_theta", "#theta_{TK}", thetaBin, thetaMin, thetaMax);
  thetaStaTrack = dbe->book1D("StaMuon_theta", "#theta_{STA}", thetaBin, thetaMin, thetaMax);
    
// monitoring of phi paramater
  phiGlbTrack.push_back(dbe->book1D(histname+"Glb_phi", "#phi_{GLB}", phiBin, phiMin, phiMax));
  phiGlbTrack.push_back(dbe->book1D(histname+"Tk_phi", "#phi_{TKfromGLB}", phiBin, phiMin, phiMax));
  phiGlbTrack.push_back(dbe->book1D(histname+"Sta_phi", "#phi_{STAfromGLB}", phiBin, phiMin, phiMax));
  phiResolution.push_back(dbe->book1D("Res_TkGlb_phi", "#phi_{TKfromGLB} - #phi_{GLB}", phiBin*binFactor, phiMin/3000, phiMax/3000));
  phiResolution.push_back(dbe->book1D("Res_GlbSta_phi", "#phi_{GLB} - #phi_{STAfromGLB}", phiBin*binFactor, phiMin/100, phiMax/100));
  phiResolution.push_back(dbe->book1D("Res_TkSta_phi", "#phi_{TKfromGLB} - #phi_{STAfromGLB}", phiBin*binFactor, phiMin/100, phiMax/100));
  phiResolution.push_back(dbe->book2D("ResVsPhi_TkGlb_phi", "(#phi_{TKfromGLB} - #phi_{GLB}) vs #phi_GLB", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/3000, phiMax/3000));
  phiResolution.push_back(dbe->book2D("ResVsPhi_GlbSta_phi", "(#phi_{GLB} - #phi_{STAfromGLB}) vs #phi_{GLB}", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/100, phiMax/100));
  phiResolution.push_back(dbe->book2D("ResVsPhi_TkSta_phi", "(#phi_{TKfromGLB} - #phi_{STAfromGLB}) vs #phi_{TKfromGLB}", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/100, phiMax/100));
  phiTrack = dbe->book1D("TkMuon_phi", "#phi_{TK}", phiBin, phiMin, phiMax);
  phiStaTrack = dbe->book1D("StaMuon_phi", "#phi_{STA}", phiBin, phiMin, phiMax);
  phiEfficiency.push_back(dbe->book1D("StaPhi", "#phi_{STAfromGLB}", phiBin, phiMin, phiMax));
  phiEfficiency.push_back(dbe->book1D("StaPhi_ifCombinedAlso", "#phi_{STAfromGLB} if the isGlb=true", phiBin, phiMin, phiMax));

// monitoring of the chi2 parameter
  chi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Glb_chi2OverDf", "#chi_{2}OverDF_{GLB}", chi2Bin, chi2Min, chi2Max));
  chi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Tk_chi2OverDf", "#chi_{2}OverDF_{TKfromGLB}", chi2Bin, chi2Min, chi2Max));
  chi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Sta_chi2OverDf", "#chi_{2}OverDF_{STAfromGLB}", chi2Bin, chi2Min, chi2Max));
  chi2OvDFTrack = dbe->book1D("TkMuon_chi2OverDf", "#chi_{2}OverDF_{TK}", chi2Bin, chi2Min, chi2Max);
  chi2OvDFStaTrack = dbe->book1D("StaMuon_chi2OverDf", "#chi_{2}OverDF_{STA}", chi2Bin, chi2Min, chi2Max);
  
  // monitoring of the momentum
  pGlbTrack.push_back(dbe->book1D(histname+"Glb_p", "p_{GLB}", pBin, pMin, pMax));
  pGlbTrack.push_back(dbe->book1D(histname+"Tk_p", "p_{TKfromGLB}", pBin, pMin, pMax));
  pGlbTrack.push_back(dbe->book1D(histname+"Sta_p", "p_{STAfromGLB}", pBin, pMin, pMax));
  pTrack = dbe->book1D("TkMuon_p", "p_{TK}", pBin, pMin, pMax);
  pStaTrack = dbe->book1D("StaMuon_p", "p_{STA}", pBin, pMin, pMax);
  
// monitoring of the transverse momentum
  ptGlbTrack.push_back(dbe->book1D(histname+"Glb_pt", "pt_{GLB}", ptBin, ptMin, ptMax));
  ptGlbTrack.push_back(dbe->book1D(histname+"Tk_pt", "pt_{TKfromGLB}", ptBin, ptMin, ptMax));
  ptGlbTrack.push_back(dbe->book1D(histname+"Sta_pt", "pt_{STAfromGLB}", ptBin, ptMin, ptMax));
  ptTrack = dbe->book1D("TkMuon_pt", "pt_{TK}", ptBin, ptMin, ptMax);
  ptStaTrack = dbe->book1D("StaMuon_pt", "pt_{STA}", ptBin, ptMin, pMax);
  
  // monitoring of the muon charge
  qGlbTrack.push_back(dbe->book1D(histname+"Glb_q", "q_{GLB}", 5, -2.5, 2.5));
  qGlbTrack.push_back(dbe->book1D(histname+"Tk_q", "q_{TKfromGLB}", 5, -2.5, 2.5));
  qGlbTrack.push_back(dbe->book1D(histname+"Sta_q", "q_{STAformGLB}", 5, -2.5, 2.5));
  qGlbTrack.push_back(dbe->book1D(histname+"qComparison", "comparison between q_{GLB} and q_{TKfromGLB}, q_{STAfromGLB}", 8, 0.5, 8.5));
  qTrack = dbe->book1D("TkMuon_q", "q_{TK}", 5, -2.5, 2.5);
  qStaTrack = dbe->book1D("StaMuon_q", "q_{STA}", 5, -2.5, 2.5);
  
  
// monitoring of the momentum resolution
  qOverpResolution.push_back(dbe->book1D("Res_TkGlb_qOverp", "(q/p)_{TKfromGLB} - (q/p)_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  qOverpResolution.push_back(dbe->book1D("Res_GlbSta_qOverp", "(q/p)_{GLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  qOverpResolution.push_back(dbe->book1D("Res_TkSta_qOverp", "(q/p)_{TKfromGLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  oneOverpResolution.push_back(dbe->book1D("Res_TkGlb_oneOverp", "(1/p)_{TKfromGLB} - (1/p)_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  oneOverpResolution.push_back(dbe->book1D("Res_GlbSta_oneOverp", "(1/p)_{GLB} - (1/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  oneOverpResolution.push_back(dbe->book1D("Res_TkSta_oneOverp", "(q/p)_{TKfromGLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  qOverptResolution.push_back(dbe->book1D("Res_TkGlb_qOverpt", "(q/p_{t})_{TKfromGLB} - (q/p_{t})_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  qOverptResolution.push_back(dbe->book1D("Res_GlbSta_qOverpt", "(q/p_{t})_{GLB} - (q/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  qOverptResolution.push_back(dbe->book1D("Res_TkSta_qOverpt", "(q/p_{t})_{TKfromGLB} - (q/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book1D("Res_TkGlb_oneOverpt", "(1/p_{t})_{TKfromGLB} - (1/p_{t})_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  oneOverptResolution.push_back(dbe->book1D("Res_GlbSta_oneOverpt", "(1/p_{t})_{GLB} - (1/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book1D("Res_TkSta_oneOverpt", "(1/p_{t})_{TKfromGLB} - (1/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsEta_TkGlb_oneOverpt", "(#eta_{TKfromGLB} - #eta_{GLB}) vs (1/p_{t})_{GLB}", etaBin, etaMin, etaMax, pResBin*binFactor*2, pResMin/10, pResMax/10));
  oneOverptResolution.push_back(dbe->book2D("ResVsEta_GlbSta_oneOverpt", "(#eta_{GLB} - #eta_{STAfromGLB} vs (1/p_{t})_{GLB}", etaBin, etaMin, etaMax, pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsEta_TkSta_oneOverpt", "(#eta_{TKfromGLB} - #eta_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", etaBin, etaMin, etaMax, pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsPhi_TkGlb_oneOverpt", "(#phi_{TKfromGLB} - #phi_{GLB}) vs (1/p_{t})_{GLB}", phiBin, phiMin, phiMax, pResBin*binFactor*2, pResMin/10, pResMax/10));
  oneOverptResolution.push_back(dbe->book2D("ResVsPhi_GlbSta_oneOverpt", "(#phi_{GLB} - #phi_{STAfromGLB} vs (1/p_{t})_{GLB}", phiBin, phiMin, phiMax, pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsPhi_TkSta_oneOverpt", "(#phi_{TKfromGLB} - #phi_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", phiBin, phiMin, phiMax, pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsPt_TkGlb_oneOverpt", "((1/p_{t})_{TKfromGLB} - (1/p_{t})_{GLB}) vs (1/p_{t})_{GLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor*2, pResMin/10, pResMax/10));
  oneOverptResolution.push_back(dbe->book2D("ResVsPt_GlbSta_oneOverpt", "((1/p_{t})_{GLB} - (1/p_{t})_{STAfromGLB} vs (1/p_{t})_{GLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor, pResMin, pResMax));
  oneOverptResolution.push_back(dbe->book2D("ResVsPt_TkSta_oneOverpt", "((1/p_{t})_{TKfromGLB} - (1/p_{t})_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor, pResMin, pResMax));
  
  
// monitoring of the recHits provenance
  rhAnalysis.push_back(dbe->book1D("StaRh_Frac_inGlb", "recHits_{STAinGLB} / recHits_{GLB}", rhBin, rhMin, rhMax));
  rhAnalysis.push_back(dbe->book1D("TkRh_Frac_inGlb", "recHits_{TKinGLB} / recHits_{GLB}", rhBin, rhMin, rhMax));  
  rhAnalysis.push_back(dbe->book1D("StaRh_inGlb_Div_RhAssoSta", "recHits_{STAinGLB} / recHits_{STAfromGLB}", rhBin, rhMin, rhMax));
  rhAnalysis.push_back(dbe->book1D("TkRh_inGlb_Div_RhAssoTk", "recHits_{TKinGLB} / recHits_{TKfromGLB}", rhBin, rhMin, rhMax));
  rhAnalysis.push_back(dbe->book1D("GlbRh_Div_RhAssoStaTk", "recHits_{GLB} / (recHits_{TKfromGLB}+recHits_{STAfromGLB})", rhBin, rhMin, rhMax));
  rhAnalysis.push_back(dbe->book1D("invalidRh_Frac_inTk", "Invalid recHits / rechits_{GLB}", rhBin, rhMin, rhMax));
   

 
      //pat muon histograms
 
dbe->setCurrentFolder("Benchmarks/muons/PAT");

	me["PatMuon_pt"]  = dbe->book1D("pt", "PatMuon", 50, 0, 200);
       	me["PatMuon_eta"]  = dbe->book1D("eta", "PatMuon", 50, -6, 6);
        me["PatMuon_phi"]  = dbe->book1D("phi", "PatMuon", 50, -4, 4);
  	patmuReco = dbe->book1D("patmuReco", "muon reconstructed tracks", 6, 1, 7);  
	patetaGlbTrack.push_back(dbe->book1D(histname+"Glb_eta", "#eta_{GLB}", etaBin, etaMin, etaMax));
	patetaGlbTrack.push_back(dbe->book1D(histname+"Tk_eta", "#eta_{TKfromGLB}", etaBin, etaMin, etaMax));
	patetaGlbTrack.push_back(dbe->book1D(histname+"Sta_eta", "#eta_{STAfromGLB}", etaBin, etaMin, etaMax));
  	patetaResolution.push_back(dbe->book1D("Res_TkGlb_eta", "#eta_{TKfromGLB} - #eta_{GLB}", etaBin*binFactor, etaMin/3000, etaMax/3000));
  	patetaResolution.push_back(dbe->book1D("Res_GlbSta_eta", "#eta_{GLB} - #eta_{STAfromGLB}", etaBin*binFactor, etaMin/100, etaMax/100));
  	patetaResolution.push_back(dbe->book1D("Res_TkSta_eta", "#eta_{TKfromGLB} - #eta_{STAfromGLB}", etaBin*binFactor, etaMin/100, etaMax/100));
  	patetaResolution.push_back(dbe->book2D("ResVsEta_TkGlb_eta", "(#eta_{TKfromGLB} - #eta_{GLB}) vs #eta_{GLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/3000, etaMax/3000));
  	patetaResolution.push_back(dbe->book2D("ResVsEta_GlbSta_eta", "(#eta_{GLB} - #eta_{STAfromGLB}) vs #eta_{GLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/100, etaMax/100));
  	patetaResolution.push_back(dbe->book2D("ResVsEta_TkSta_eta", "(#eta_{TKfromGLB} - #eta_{STAfromGLB}) vs #eta_{TKfromGLB}", etaBin, etaMin, etaMax, etaBin*binFactor, etaMin/100, etaMax/100));  
  	patetaTrack = dbe->book1D("TkMuon_eta", "#eta_{TK}", etaBin, etaMin, etaMax);
  	patetaStaTrack = dbe->book1D("StaMuon_eta", "#eta_{STA}", etaBin, etaMin, etaMax);
  	patetaEfficiency.push_back(dbe->book1D("StaEta", "#eta_{STAfromGLB}", etaBin, etaMin, etaMax));
  	patetaEfficiency.push_back(dbe->book1D("StaEta_ifCombinedAlso", "#eta_{STAfromGLB} if isGlb=true", etaBin, etaMin, etaMax));
 
// monitoring of theta parameter
  patthetaGlbTrack.push_back(dbe->book1D(histname+"Glb_theta", "#theta_{GLB}", thetaBin, thetaMin, thetaMax));
  patthetaGlbTrack.push_back(dbe->book1D(histname+"Tk_theta", "#theta_{TKfromGLB}", thetaBin, thetaMin, thetaMax));
  patthetaGlbTrack.push_back(dbe->book1D(histname+"Sta_theta", "#theta_{STAfromGLB}", thetaBin, thetaMin, thetaMax));
  patthetaResolution.push_back(dbe->book1D("Res_TkGlb_theta", "#theta_{TKfromGLB} - #theta_{GLB}", thetaBin*binFactor, -(thetaMax/3000), thetaMax/3000));
  patthetaResolution.push_back(dbe->book1D("Res_GlbSta_theta", "#theta_{GLB} - #theta_{STAfromGLB}", thetaBin*binFactor,-(thetaMax/100), thetaMax/100));
  patthetaResolution.push_back(dbe->book1D("Res_TkSta_theta", "#theta_{TKfromGLB} - #theta_{STAfromGLB}", thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  patthetaResolution.push_back(dbe->book2D("ResVsTheta_TkGlb_theta", "(#theta_{TKfromGLB} - #theta_{GLB}) vs #theta_{GLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/3000), thetaMax/3000));
  patthetaResolution.push_back(dbe->book2D("ResVsTheta_GlbSta_theta", "(#theta_{GLB} - #theta_{STAfromGLB}) vs #theta_{GLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  patthetaResolution.push_back(dbe->book2D("ResVsTheta_TkSta_theta", "(#theta_{TKfromGLB} - #theta_{STAfromGLB}) vs #theta_{TKfromGLB}", thetaBin, thetaMin, thetaMax, thetaBin*binFactor, -(thetaMax/100), thetaMax/100));
  patthetaTrack = dbe->book1D("TkMuon_theta", "#theta_{TK}", thetaBin, thetaMin, thetaMax);
  patthetaStaTrack = dbe->book1D("StaMuon_theta", "#theta_{STA}", thetaBin, thetaMin, thetaMax);
    
// monitoring of phi paramater
  patphiGlbTrack.push_back(dbe->book1D(histname+"Glb_phi", "#phi_{GLB}", phiBin, phiMin, phiMax));
  patphiGlbTrack.push_back(dbe->book1D(histname+"Tk_phi", "#phi_{TKfromGLB}", phiBin, phiMin, phiMax));
  patphiGlbTrack.push_back(dbe->book1D(histname+"Sta_phi", "#phi_{STAfromGLB}", phiBin, phiMin, phiMax));
  patphiResolution.push_back(dbe->book1D("Res_TkGlb_phi", "#phi_{TKfromGLB} - #phi_{GLB}", phiBin*binFactor, phiMin/3000, phiMax/3000));
  patphiResolution.push_back(dbe->book1D("Res_GlbSta_phi", "#phi_{GLB} - #phi_{STAfromGLB}", phiBin*binFactor, phiMin/100, phiMax/100));
  patphiResolution.push_back(dbe->book1D("Res_TkSta_phi", "#phi_{TKfromGLB} - #phi_{STAfromGLB}", phiBin*binFactor, phiMin/100, phiMax/100));
  patphiResolution.push_back(dbe->book2D("ResVsPhi_TkGlb_phi", "(#phi_{TKfromGLB} - #phi_{GLB}) vs #phi_GLB", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/3000, phiMax/3000));
  patphiResolution.push_back(dbe->book2D("ResVsPhi_GlbSta_phi", "(#phi_{GLB} - #phi_{STAfromGLB}) vs #phi_{GLB}", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/100, phiMax/100));
  patphiResolution.push_back(dbe->book2D("ResVsPhi_TkSta_phi", "(#phi_{TKfromGLB} - #phi_{STAfromGLB}) vs #phi_{TKfromGLB}", phiBin, phiMin, phiMax, phiBin*binFactor, phiMin/100, phiMax/100));
  patphiTrack = dbe->book1D("TkMuon_phi", "#phi_{TK}", phiBin, phiMin, phiMax);
  patphiStaTrack = dbe->book1D("StaMuon_phi", "#phi_{STA}", phiBin, phiMin, phiMax);
  patphiEfficiency.push_back(dbe->book1D("StaPhi", "#phi_{STAfromGLB}", phiBin, phiMin, phiMax));
  patphiEfficiency.push_back(dbe->book1D("StaPhi_ifCombinedAlso", "#phi_{STAfromGLB} if the isGlb=true", phiBin, phiMin, phiMax));

// monitoring of the chi2 parameter
  patchi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Glb_chi2OverDf", "#chi_{2}OverDF_{GLB}", chi2Bin, chi2Min, chi2Max));
  patchi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Tk_chi2OverDf", "#chi_{2}OverDF_{TKfromGLB}", phiBin, chi2Min, chi2Max));
  patchi2OvDFGlbTrack.push_back(dbe->book1D(histname+"Sta_chi2OverDf", "#chi_{2}OverDF_{STAfromGLB}", chi2Bin, chi2Min, chi2Max));
  patchi2OvDFTrack = dbe->book1D("TkMuon_chi2OverDf", "#chi_{2}OverDF_{TK}", chi2Bin, chi2Min, chi2Max);
  patchi2OvDFStaTrack = dbe->book1D("StaMuon_chi2OverDf", "#chi_{2}OverDF_{STA}", chi2Bin, chi2Min, chi2Max);
  
  // monitoring of the momentum
  patpGlbTrack.push_back(dbe->book1D(histname+"Glb_p", "p_{GLB}", pBin, pMin, pMax));
  patpGlbTrack.push_back(dbe->book1D(histname+"Tk_p", "p_{TKfromGLB}", pBin, pMin, pMax));
  patpGlbTrack.push_back(dbe->book1D(histname+"Sta_p", "p_{STAfromGLB}", pBin, pMin, pMax));
  patpTrack = dbe->book1D("TkMuon_p", "p_{TK}", pBin, pMin, pMax);
  patpStaTrack = dbe->book1D("StaMuon_p", "p_{STA}", pBin, pMin, pMax);
  
// monitoring of the transverse momentum
  patptGlbTrack.push_back(dbe->book1D(histname+"Glb_pt", "pt_{GLB}", ptBin, ptMin, ptMax));
  patptGlbTrack.push_back(dbe->book1D(histname+"Tk_pt", "pt_{TKfromGLB}", ptBin, ptMin, ptMax));
  patptGlbTrack.push_back(dbe->book1D(histname+"Sta_pt", "pt_{STAfromGLB}", ptBin, ptMin, ptMax));
  patptTrack = dbe->book1D("TkMuon_pt", "pt_{TK}", ptBin, ptMin, ptMax);
  patptStaTrack = dbe->book1D("StaMuon_pt", "pt_{STA}", ptBin, ptMin, pMax);
  
  // monitoring of the muon charge
  patqGlbTrack.push_back(dbe->book1D(histname+"Glb_q", "q_{GLB}", 5, -2.5, 2.5));
  patqGlbTrack.push_back(dbe->book1D(histname+"Tk_q", "q_{TKfromGLB}", 5, -2.5, 2.5));
  patqGlbTrack.push_back(dbe->book1D(histname+"Sta_q", "q_{STAformGLB}", 5, -2.5, 2.5));
  patqGlbTrack.push_back(dbe->book1D(histname+"qComparison", "comparison between q_{GLB} and q_{TKfromGLB}, q_{STAfromGLB}", 8, 0.5, 8.5));
  patqTrack = dbe->book1D("TkMuon_q", "q_{TK}", 5, -2.5, 2.5);
  patqStaTrack = dbe->book1D("StaMuon_q", "q_{STA}", 5, -2.5, 2.5);
  
  
// monitoring of the momentum resolution
  patqOverpResolution.push_back(dbe->book1D("Res_TkGlb_qOverp", "(q/p)_{TKfromGLB} - (q/p)_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  patqOverpResolution.push_back(dbe->book1D("Res_GlbSta_qOverp", "(q/p)_{GLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patqOverpResolution.push_back(dbe->book1D("Res_TkSta_qOverp", "(q/p)_{TKfromGLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patoneOverpResolution.push_back(dbe->book1D("Res_TkGlb_oneOverp", "(1/p)_{TKfromGLB} - (1/p)_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  patoneOverpResolution.push_back(dbe->book1D("Res_GlbSta_oneOverp", "(1/p)_{GLB} - (1/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patoneOverpResolution.push_back(dbe->book1D("Res_TkSta_oneOverp", "(q/p)_{TKfromGLB} - (q/p)_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patqOverptResolution.push_back(dbe->book1D("Res_TkGlb_qOverpt", "(q/p_{t})_{TKfromGLB} - (q/p_{t})_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  patqOverptResolution.push_back(dbe->book1D("Res_GlbSta_qOverpt", "(q/p_{t})_{GLB} - (q/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patqOverptResolution.push_back(dbe->book1D("Res_TkSta_qOverpt", "(q/p_{t})_{TKfromGLB} - (q/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book1D("Res_TkGlb_oneOverpt", "(1/p_{t})_{TKfromGLB} - (1/p_{t})_{GLB}", pResBin*binFactor*2, pResMin/10, pResMax/10));
  patoneOverptResolution.push_back(dbe->book1D("Res_GlbSta_oneOverpt", "(1/p_{t})_{GLB} - (1/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book1D("Res_TkSta_oneOverpt", "(1/p_{t})_{TKfromGLB} - (1/p_{t})_{STAfromGLB}", pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsEta_TkGlb_oneOverpt", "(#eta_{TKfromGLB} - #eta_{GLB}) vs (1/p_{t})_{GLB}", etaBin, etaMin, etaMax, pResBin*binFactor*2, pResMin/10, pResMax/10));
  patoneOverptResolution.push_back(dbe->book2D("ResVsEta_GlbSta_oneOverpt", "(#eta_{GLB} - #eta_{STAfromGLB} vs (1/p_{t})_{GLB}", etaBin, etaMin, etaMax, pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsEta_TkSta_oneOverpt", "(#eta_{TKfromGLB} - #eta_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", etaBin, etaMin, etaMax, pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPhi_TkGlb_oneOverpt", "(#phi_{TKfromGLB} - #phi_{GLB}) vs (1/p_{t})_{GLB}", phiBin, phiMin, phiMax, pResBin*binFactor*2, pResMin/10, pResMax/10));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPhi_GlbSta_oneOverpt", "(#phi_{GLB} - #phi_{STAfromGLB} vs (1/p_{t})_{GLB}", phiBin, phiMin, phiMax, pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPhi_TkSta_oneOverpt", "(#phi_{TKfromGLB} - #phi_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", phiBin, phiMin, phiMax, pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPt_TkGlb_oneOverpt", "((1/p_{t})_{TKfromGLB} - (1/p_{t})_{GLB}) vs (1/p_{t})_{GLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor*2, pResMin/10, pResMax/10));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPt_GlbSta_oneOverpt", "((1/p_{t})_{GLB} - (1/p_{t})_{STAfromGLB} vs (1/p_{t})_{GLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor, pResMin, pResMax));
  patoneOverptResolution.push_back(dbe->book2D("ResVsPt_TkSta_oneOverpt", "((1/p_{t})_{TKfromGLB} - (1/p_{t})_{STAfromGLB}) vs (1/p_{t})_{TKfromGLB}", ptBin/5, ptMin, ptMax/100, pResBin*binFactor, pResMin, pResMax));
  
  
// monitoring of the recHits provenance
  patrhAnalysis.push_back(dbe->book1D("StaRh_Frac_inGlb", "recHits_{STAinGLB} / recHits_{GLB}", rhBin, rhMin, rhMax));
  patrhAnalysis.push_back(dbe->book1D("TkRh_Frac_inGlb", "recHits_{TKinGLB} / recHits_{GLB}", rhBin, rhMin, rhMax));  
  patrhAnalysis.push_back(dbe->book1D("StaRh_inGlb_Div_RhAssoSta", "recHits_{STAinGLB} / recHits_{STAfromGLB}", rhBin, rhMin, rhMax));
  patrhAnalysis.push_back(dbe->book1D("TkRh_inGlb_Div_RhAssoTk", "recHits_{TKinGLB} / recHits_{TKfromGLB}", rhBin, rhMin, rhMax));
  patrhAnalysis.push_back(dbe->book1D("GlbRh_Div_RhAssoStaTk", "recHits_{GLB} / (recHits_{TKfromGLB}+recHits_{STAfromGLB})", rhBin, rhMin, rhMax));
  patrhAnalysis.push_back(dbe->book1D("invalidRh_Frac_inTk", "Invalid recHits / rechits_{GLB}", rhBin, rhMin, rhMax));

     
cout << "MALIK1" << endl;
  }
  return;
    
  
}

// ------------ method called to for each event  ------------
void
PATValidation_Muon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   
   // Typedefs to use views

//====================================RECO MUON=====================================
//    typedef edm::View<reco::Candidate> candidateCollection ;
     typedef edm::View<reco::Muon> RecoMuonCollection ;
      
     //get reco muon collection
     edm::Handle<RecoMuonCollection> recomuon_hnd;
     iEvent.getByLabel(recoMuon_, recomuon_hnd);
     const RecoMuonCollection & RECOMUON =*recomuon_hnd;
    // Loop over Reco Muons
     for (int i = 0; i != RECOMUON.size(); ++i) {
      me["RecoMuon_pt"]->Fill(RECOMUON[i].pt());
      me["RecoMuon_eta"]->Fill(RECOMUON[i].eta());
      me["RecoMuon_phi"]->Fill(RECOMUON[i].phi()); 

    // Muon reco fill
    if(RECOMUON[i].isGlobalMuon()) {

    if(RECOMUON[i].isTrackerMuon() && RECOMUON[i].isStandAloneMuon())
      muReco->Fill(1);
    if(!(RECOMUON[i].isTrackerMuon()) && RECOMUON[i].isStandAloneMuon())
      muReco->Fill(2);
//    if(!RECOMUON[i].isStandAloneMuon())
    // get the track combinig the information from both the Tracker and the Spectrometer
     reco::TrackRef recoCombinedGlbTrack = RECOMUON[i].combinedMuon();
    // get the track using only the tracker data
      reco::TrackRef recoTkGlbTrack = RECOMUON[i].track();
    // get the track using only the mu spectrometer data
     reco::TrackRef recoStaGlbTrack = RECOMUON[i].standAloneMuon();

     etaGlbTrack[0]->Fill(recoCombinedGlbTrack->eta());
     etaGlbTrack[1]->Fill(recoTkGlbTrack->eta());
     etaGlbTrack[2]->Fill(recoStaGlbTrack->eta());
     etaResolution[0]->Fill(recoTkGlbTrack->eta()-recoCombinedGlbTrack->eta());
     etaResolution[1]->Fill(-recoStaGlbTrack->eta()+recoCombinedGlbTrack->eta());
     etaResolution[2]->Fill(recoTkGlbTrack->eta()-recoStaGlbTrack->eta());
     etaResolution[3]->Fill(recoCombinedGlbTrack->eta(), recoTkGlbTrack->eta()-recoCombinedGlbTrack->eta());
     etaResolution[4]->Fill(recoCombinedGlbTrack->eta(), -recoStaGlbTrack->eta()+recoCombinedGlbTrack->eta());
     etaResolution[5]->Fill(recoCombinedGlbTrack->eta(), recoTkGlbTrack->eta()-recoStaGlbTrack->eta());

     thetaGlbTrack[0]->Fill(recoCombinedGlbTrack->theta());
     thetaGlbTrack[1]->Fill(recoTkGlbTrack->theta());
     thetaGlbTrack[2]->Fill(recoStaGlbTrack->theta());
     thetaResolution[0]->Fill(recoTkGlbTrack->theta()-recoCombinedGlbTrack->theta());
     thetaResolution[1]->Fill(-recoStaGlbTrack->theta()+recoCombinedGlbTrack->theta());
     thetaResolution[2]->Fill(recoTkGlbTrack->theta()-recoStaGlbTrack->theta());
     thetaResolution[3]->Fill(recoCombinedGlbTrack->theta(), recoTkGlbTrack->theta()-recoCombinedGlbTrack->theta());
     thetaResolution[4]->Fill(recoCombinedGlbTrack->theta(), -recoStaGlbTrack->theta()+recoCombinedGlbTrack->theta());
     thetaResolution[5]->Fill(recoCombinedGlbTrack->theta(), recoTkGlbTrack->theta()-recoStaGlbTrack->theta());
     
     phiGlbTrack[0]->Fill(recoCombinedGlbTrack->phi());
     phiGlbTrack[1]->Fill(recoTkGlbTrack->phi());
     phiGlbTrack[2]->Fill(recoStaGlbTrack->phi());
     phiResolution[0]->Fill(recoTkGlbTrack->phi()-recoCombinedGlbTrack->phi());
     phiResolution[1]->Fill(-recoStaGlbTrack->phi()+recoCombinedGlbTrack->phi());
     phiResolution[2]->Fill(recoTkGlbTrack->phi()-recoStaGlbTrack->phi());
     phiResolution[3]->Fill(recoCombinedGlbTrack->phi(), recoTkGlbTrack->phi()-recoCombinedGlbTrack->phi());
     phiResolution[4]->Fill(recoCombinedGlbTrack->phi(), -recoStaGlbTrack->phi()+recoCombinedGlbTrack->phi());
     phiResolution[5]->Fill(recoCombinedGlbTrack->phi(), recoTkGlbTrack->phi()-recoStaGlbTrack->phi());
    
     chi2OvDFGlbTrack[0]->Fill(recoCombinedGlbTrack->normalizedChi2());
     chi2OvDFGlbTrack[1]->Fill(recoTkGlbTrack->normalizedChi2());
     chi2OvDFGlbTrack[2]->Fill(recoStaGlbTrack->normalizedChi2());

     pGlbTrack[0]->Fill(recoCombinedGlbTrack->p());
     pGlbTrack[1]->Fill(recoTkGlbTrack->p());
     pGlbTrack[2]->Fill(recoStaGlbTrack->p());
 
     ptGlbTrack[0]->Fill(recoCombinedGlbTrack->pt());
     ptGlbTrack[1]->Fill(recoTkGlbTrack->pt());
     ptGlbTrack[2]->Fill(recoStaGlbTrack->pt());

     qGlbTrack[0]->Fill(recoCombinedGlbTrack->charge());
     qGlbTrack[1]->Fill(recoTkGlbTrack->charge());
     qGlbTrack[2]->Fill(recoStaGlbTrack->charge());
     if(recoCombinedGlbTrack->charge()==recoStaGlbTrack->charge()) qGlbTrack[3]->Fill(1);
     else qGlbTrack[3]->Fill(2);
     if(recoCombinedGlbTrack->charge()==recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(3);
     else qGlbTrack[3]->Fill(4);
     if(recoStaGlbTrack->charge()==recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(5);
     else qGlbTrack[3]->Fill(6);
     if(recoCombinedGlbTrack->charge()!=recoStaGlbTrack->charge() && recoCombinedGlbTrack->charge()!=recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(7);
     if(recoCombinedGlbTrack->charge()==recoStaGlbTrack->charge() && recoCombinedGlbTrack->charge()==recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(8);
    
     qOverpResolution[0]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->p())-(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->p()));
     qOverpResolution[1]->Fill(-(recoStaGlbTrack->charge()/recoStaGlbTrack->p())+(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->p()));
     qOverpResolution[2]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->p())-(recoStaGlbTrack->charge()/recoStaGlbTrack->p()));
     oneOverpResolution[0]->Fill((1/recoTkGlbTrack->p())-(1/recoCombinedGlbTrack->p()));
     oneOverpResolution[1]->Fill(-(1/recoStaGlbTrack->p())+(1/recoCombinedGlbTrack->p()));
     oneOverpResolution[2]->Fill((1/recoTkGlbTrack->p())-(1/recoStaGlbTrack->p()));
     qOverptResolution[0]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->pt())-(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->pt()));
     qOverptResolution[1]->Fill(-(recoStaGlbTrack->charge()/recoStaGlbTrack->pt())+(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->pt()));
     qOverptResolution[2]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->pt())-(recoStaGlbTrack->charge()/recoStaGlbTrack->pt()));
     oneOverptResolution[0]->Fill((1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[1]->Fill(-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[2]->Fill((1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     oneOverptResolution[3]->Fill(recoCombinedGlbTrack->eta(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[4]->Fill(recoCombinedGlbTrack->eta(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[5]->Fill(recoCombinedGlbTrack->eta(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     oneOverptResolution[6]->Fill(recoCombinedGlbTrack->phi(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[7]->Fill(recoCombinedGlbTrack->phi(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[8]->Fill(recoCombinedGlbTrack->phi(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     oneOverptResolution[9]->Fill(recoCombinedGlbTrack->pt(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[10]->Fill(recoCombinedGlbTrack->pt(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     oneOverptResolution[11]->Fill(recoCombinedGlbTrack->pt(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));

         // valid hits Glb track
    double rhGlb = recoCombinedGlbTrack->found();
    // valid hits Glb track from Tracker
    double rhGlb_StaProvenance=0;
    // valid hits Glb track from Sta system
    double rhGlb_TkProvenance=0;
    for (trackingRecHit_iterator recHit = recoCombinedGlbTrack->recHitsBegin();
	 recHit!=recoCombinedGlbTrack->recHitsEnd(); ++recHit){
      if((*recHit)->isValid()){
	DetId id = (*recHit)->geographicalId();
	if (id.det() == DetId::Muon)
	  rhGlb_StaProvenance++;
	if (id.det() == DetId::Tracker)
	  rhGlb_TkProvenance++;
      }
    }
    // valid hits Sta track associated to Glb track
    double rhStaGlb = recoStaGlbTrack->recHitsSize();
    // valid hits Traker track associated to Glb track
    double rhTkGlb = recoTkGlbTrack->found();
    // invalid hits Traker track associated to Glb track
    double rhTkGlb_notValid = recoTkGlbTrack->lost();
   
    // fill the histos
    rhAnalysis[0]->Fill(rhGlb_StaProvenance/rhGlb);
    rhAnalysis[1]->Fill(rhGlb_TkProvenance/rhGlb);
    rhAnalysis[2]->Fill(rhGlb_StaProvenance/rhStaGlb);
    rhAnalysis[3]->Fill(rhGlb_TkProvenance/rhTkGlb);
    rhAnalysis[4]->Fill(rhGlb/(rhStaGlb+rhTkGlb));
    rhAnalysis[5]->Fill(rhTkGlb_notValid/rhGlb);

if(RECOMUON[i].isTrackerMuon() && !(RECOMUON[i].isGlobalMuon())) {
     if(RECOMUON[i].isStandAloneMuon())
      muReco->Fill(3);
    if(!(RECOMUON[i].isStandAloneMuon()))
      muReco->Fill(4);

    // get the track using only the tracker data
    reco::TrackRef recoTrack = RECOMUON[i].track();

    etaTrack->Fill(recoTrack->eta());
    thetaTrack->Fill(recoTrack->theta());
    phiTrack->Fill(recoTrack->phi());
    chi2OvDFTrack->Fill(recoTrack->normalizedChi2());
    pTrack->Fill(recoTrack->p());
    ptTrack->Fill(recoTrack->pt());
    qTrack->Fill(recoTrack->charge());
    
  }

  if(RECOMUON[i].isStandAloneMuon() && !(RECOMUON[i].isGlobalMuon())) {
    if(!(RECOMUON[i].isTrackerMuon()))
      muReco->Fill(5);
     
    // get the track using only the mu spectrometer data
    reco::TrackRef recoStaTrack = RECOMUON[i].standAloneMuon();

    etaStaTrack->Fill(recoStaTrack->eta());
    thetaStaTrack->Fill(recoStaTrack->theta());
    phiStaTrack->Fill(recoStaTrack->phi());
    chi2OvDFStaTrack->Fill(recoStaTrack->normalizedChi2());
    pStaTrack->Fill(recoStaTrack->p());
    ptStaTrack->Fill(recoStaTrack->pt());
    qStaTrack->Fill(recoStaTrack->charge());

  }
    
  if(RECOMUON[i].isCaloMuon() && !(RECOMUON[i].isGlobalMuon()) && !(RECOMUON[i].isTrackerMuon()) && !(RECOMUON[i].isStandAloneMuon()))
    muReco->Fill(6);
  
  //efficiency plots
  
  // get the track using only the mu spectrometer data
 //  reco::TrackRef recoStaGlbTrack = RECOMUON[i].standAloneMuon();
  
  if(RECOMUON[i].isStandAloneMuon()){
    etaEfficiency[0]->Fill(recoStaGlbTrack->eta());
    phiEfficiency[0]->Fill(recoStaGlbTrack->phi());
  }
  if(RECOMUON[i].isStandAloneMuon() && RECOMUON[i].isGlobalMuon()){
    etaEfficiency[1]->Fill(recoStaGlbTrack->eta());
    phiEfficiency[1]->Fill(recoStaGlbTrack->phi());
  }


    } 
//      cout << "reco muon pt is = " << RECOMUON[i].pt() << endl; 
       }


//=============================PAT MUON====================================


   //get pat muon collection
     edm::Handle <std::vector<pat::Muon> >  patmuon_hnd;
     iEvent.getByLabel(patMuon_, patmuon_hnd);
     std::vector<pat::Muon> const & PATMUON = *patmuon_hnd;
    // Loop over Pat Muons
     for (int j = 0; j != PATMUON.size(); ++j) {
     me["PatMuon_pt"]->Fill(PATMUON[j].pt());
     me["PatMuon_eta"]->Fill(PATMUON[j].eta());
     me["PatMuon_phi"]->Fill(PATMUON[j].phi());   
       // Muon reco fill
    if(PATMUON[j].isGlobalMuon()) {
     
    if(PATMUON[j].isTrackerMuon() && PATMUON[j].isStandAloneMuon())
      patmuReco->Fill(1);
    if(!(PATMUON[j].isTrackerMuon()) && PATMUON[j].isStandAloneMuon())
     patmuReco->Fill(2);
//    if(!PATMUON[j].isStandAloneMuon())
    // get the track combinig the information from both the Tracker and the Spectrometer
    reco::TrackRef recoCombinedGlbTrack = PATMUON[j].combinedMuon();
    // get the track using only the tracker data
    reco::TrackRef recoTkGlbTrack = PATMUON[j].track();
    // get the track using only the mu spectrometer data
     reco::TrackRef recoStaGlbTrack = PATMUON[j].standAloneMuon();

       patetaGlbTrack[0]->Fill(recoCombinedGlbTrack->eta());
       patetaGlbTrack[1]->Fill(recoTkGlbTrack->eta()); 
       patetaGlbTrack[2]->Fill(recoStaGlbTrack->eta());


     patetaResolution[0]->Fill(recoTkGlbTrack->eta()-recoCombinedGlbTrack->eta());
     patetaResolution[1]->Fill(-recoStaGlbTrack->eta()+recoCombinedGlbTrack->eta());
     patetaResolution[2]->Fill(recoTkGlbTrack->eta()-recoStaGlbTrack->eta());
     patetaResolution[3]->Fill(recoCombinedGlbTrack->eta(), recoTkGlbTrack->eta()-recoCombinedGlbTrack->eta());
     patetaResolution[4]->Fill(recoCombinedGlbTrack->eta(), -recoStaGlbTrack->eta()+recoCombinedGlbTrack->eta());
     patetaResolution[5]->Fill(recoCombinedGlbTrack->eta(), recoTkGlbTrack->eta()-recoStaGlbTrack->eta());

     patthetaGlbTrack[0]->Fill(recoCombinedGlbTrack->theta());
     patthetaGlbTrack[1]->Fill(recoTkGlbTrack->theta());
     patthetaGlbTrack[2]->Fill(recoStaGlbTrack->theta());
     patthetaResolution[0]->Fill(recoTkGlbTrack->theta()-recoCombinedGlbTrack->theta());
     patthetaResolution[1]->Fill(-recoStaGlbTrack->theta()+recoCombinedGlbTrack->theta());
     patthetaResolution[2]->Fill(recoTkGlbTrack->theta()-recoStaGlbTrack->theta());
     patthetaResolution[3]->Fill(recoCombinedGlbTrack->theta(), recoTkGlbTrack->theta()-recoCombinedGlbTrack->theta());
     patthetaResolution[4]->Fill(recoCombinedGlbTrack->theta(), -recoStaGlbTrack->theta()+recoCombinedGlbTrack->theta());
     patthetaResolution[5]->Fill(recoCombinedGlbTrack->theta(), recoTkGlbTrack->theta()-recoStaGlbTrack->theta());
     
     patphiGlbTrack[0]->Fill(recoCombinedGlbTrack->phi());
     patphiGlbTrack[1]->Fill(recoTkGlbTrack->phi());
     patphiGlbTrack[2]->Fill(recoStaGlbTrack->phi());
     patphiResolution[0]->Fill(recoTkGlbTrack->phi()-recoCombinedGlbTrack->phi());
     patphiResolution[1]->Fill(-recoStaGlbTrack->phi()+recoCombinedGlbTrack->phi());
     patphiResolution[2]->Fill(recoTkGlbTrack->phi()-recoStaGlbTrack->phi());
     patphiResolution[3]->Fill(recoCombinedGlbTrack->phi(), recoTkGlbTrack->phi()-recoCombinedGlbTrack->phi());
     patphiResolution[4]->Fill(recoCombinedGlbTrack->phi(), -recoStaGlbTrack->phi()+recoCombinedGlbTrack->phi());
     patphiResolution[5]->Fill(recoCombinedGlbTrack->phi(), recoTkGlbTrack->phi()-recoStaGlbTrack->phi());
    
     patchi2OvDFGlbTrack[0]->Fill(recoCombinedGlbTrack->normalizedChi2());
     patchi2OvDFGlbTrack[1]->Fill(recoTkGlbTrack->normalizedChi2());
     patchi2OvDFGlbTrack[2]->Fill(recoStaGlbTrack->normalizedChi2());

     patpGlbTrack[0]->Fill(recoCombinedGlbTrack->p());
     patpGlbTrack[1]->Fill(recoTkGlbTrack->p());
     patpGlbTrack[2]->Fill(recoStaGlbTrack->p());
 
     patptGlbTrack[0]->Fill(recoCombinedGlbTrack->pt());
     patptGlbTrack[1]->Fill(recoTkGlbTrack->pt());
     patptGlbTrack[2]->Fill(recoStaGlbTrack->pt());

     patqGlbTrack[0]->Fill(recoCombinedGlbTrack->charge());
     patqGlbTrack[1]->Fill(recoTkGlbTrack->charge());
     patqGlbTrack[2]->Fill(recoStaGlbTrack->charge());
     if(recoCombinedGlbTrack->charge()==recoStaGlbTrack->charge()) qGlbTrack[3]->Fill(1);
     else patqGlbTrack[3]->Fill(2);
     if(recoCombinedGlbTrack->charge()==recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(3);
     else patqGlbTrack[3]->Fill(4);
     if(recoStaGlbTrack->charge()==recoTkGlbTrack->charge()) qGlbTrack[3]->Fill(5);
     else patqGlbTrack[3]->Fill(6);
     if(recoCombinedGlbTrack->charge()!=recoStaGlbTrack->charge() && recoCombinedGlbTrack->charge()!=recoTkGlbTrack->charge()) patqGlbTrack[3]->Fill(7);
     if(recoCombinedGlbTrack->charge()==recoStaGlbTrack->charge() && recoCombinedGlbTrack->charge()==recoTkGlbTrack->charge()) patqGlbTrack[3]->Fill(8);
    
     patqOverpResolution[0]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->p())-(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->p()));
     patqOverpResolution[1]->Fill(-(recoStaGlbTrack->charge()/recoStaGlbTrack->p())+(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->p()));
     patqOverpResolution[2]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->p())-(recoStaGlbTrack->charge()/recoStaGlbTrack->p()));
     patoneOverpResolution[0]->Fill((1/recoTkGlbTrack->p())-(1/recoCombinedGlbTrack->p()));
     patoneOverpResolution[1]->Fill(-(1/recoStaGlbTrack->p())+(1/recoCombinedGlbTrack->p()));
     patoneOverpResolution[2]->Fill((1/recoTkGlbTrack->p())-(1/recoStaGlbTrack->p()));
     patqOverptResolution[0]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->pt())-(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->pt()));
     patqOverptResolution[1]->Fill(-(recoStaGlbTrack->charge()/recoStaGlbTrack->pt())+(recoCombinedGlbTrack->charge()/recoCombinedGlbTrack->pt()));
     patqOverptResolution[2]->Fill((recoTkGlbTrack->charge()/recoTkGlbTrack->pt())-(recoStaGlbTrack->charge()/recoStaGlbTrack->pt()));
     patoneOverptResolution[0]->Fill((1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[1]->Fill(-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[2]->Fill((1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     patoneOverptResolution[3]->Fill(recoCombinedGlbTrack->eta(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[4]->Fill(recoCombinedGlbTrack->eta(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[5]->Fill(recoCombinedGlbTrack->eta(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     patoneOverptResolution[6]->Fill(recoCombinedGlbTrack->phi(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[7]->Fill(recoCombinedGlbTrack->phi(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[8]->Fill(recoCombinedGlbTrack->phi(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));
     patoneOverptResolution[9]->Fill(recoCombinedGlbTrack->pt(),(1/recoTkGlbTrack->pt())-(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[10]->Fill(recoCombinedGlbTrack->pt(),-(1/recoStaGlbTrack->pt())+(1/recoCombinedGlbTrack->pt()));
     patoneOverptResolution[11]->Fill(recoCombinedGlbTrack->pt(),(1/recoTkGlbTrack->pt())-(1/recoStaGlbTrack->pt()));

         // valid hits Glb track
    double rhGlb = recoCombinedGlbTrack->found();
    // valid hits Glb track from Tracker
    double rhGlb_StaProvenance=0;
    // valid hits Glb track from Sta system
    double rhGlb_TkProvenance=0;
    for (trackingRecHit_iterator recHit = recoCombinedGlbTrack->recHitsBegin();
	 recHit!=recoCombinedGlbTrack->recHitsEnd(); ++recHit){
      if((*recHit)->isValid()){
	DetId id = (*recHit)->geographicalId();
	if (id.det() == DetId::Muon)
	  rhGlb_StaProvenance++;
	if (id.det() == DetId::Tracker)
	  rhGlb_TkProvenance++;
      }
    }
    // valid hits Sta track associated to Glb track
    double rhStaGlb = recoStaGlbTrack->recHitsSize();
    // valid hits Traker track associated to Glb track
    double rhTkGlb = recoTkGlbTrack->found();
    // invalid hits Traker track associated to Glb track
    double rhTkGlb_notValid = recoTkGlbTrack->lost();
   
    // fill the histos
    patrhAnalysis[0]->Fill(rhGlb_StaProvenance/rhGlb);
    patrhAnalysis[1]->Fill(rhGlb_TkProvenance/rhGlb);
    patrhAnalysis[2]->Fill(rhGlb_StaProvenance/rhStaGlb);
    patrhAnalysis[3]->Fill(rhGlb_TkProvenance/rhTkGlb);
    patrhAnalysis[4]->Fill(rhGlb/(rhStaGlb+rhTkGlb));
    patrhAnalysis[5]->Fill(rhTkGlb_notValid/rhGlb);

if(PATMUON[j].isTrackerMuon() && !(PATMUON[j].isGlobalMuon())) {
     if(PATMUON[j].isStandAloneMuon())
      patmuReco->Fill(3);
    if(!(PATMUON[j].isStandAloneMuon()))
      patmuReco->Fill(4);

    // get the track using only the tracker data
    reco::TrackRef recoTrack = PATMUON[j].track();

    patetaTrack->Fill(recoTrack->eta());
    patthetaTrack->Fill(recoTrack->theta());
    patphiTrack->Fill(recoTrack->phi());
    patchi2OvDFTrack->Fill(recoTrack->normalizedChi2());
    patpTrack->Fill(recoTrack->p());
    patptTrack->Fill(recoTrack->pt());
    patqTrack->Fill(recoTrack->charge());
    
  }

  if(PATMUON[j].isStandAloneMuon() && !(PATMUON[j].isGlobalMuon())) {
    if(!(PATMUON[j].isTrackerMuon()))
      patmuReco->Fill(5);
     
    // get the track using only the mu spectrometer data
    reco::TrackRef recoStaTrack = RECOMUON[j].standAloneMuon();

    patetaStaTrack->Fill(recoStaTrack->eta());
    patthetaStaTrack->Fill(recoStaTrack->theta());
    patphiStaTrack->Fill(recoStaTrack->phi());
    patchi2OvDFStaTrack->Fill(recoStaTrack->normalizedChi2());
    patpStaTrack->Fill(recoStaTrack->p());
    patptStaTrack->Fill(recoStaTrack->pt());
    patqStaTrack->Fill(recoStaTrack->charge());

  }
    
  if(PATMUON[j].isCaloMuon() && !(PATMUON[j].isGlobalMuon()) && !(PATMUON[j].isTrackerMuon()) && !(PATMUON[j].isStandAloneMuon()))
    patmuReco->Fill(6);
  
  //efficiency plots
  
  // get the track using only the mu spectrometer data
 //  reco::TrackRef recoStaGlbTrack = RECOMUON[j].standAloneMuon();
  
  if(PATMUON[j].isStandAloneMuon()){
    patetaEfficiency[0]->Fill(recoStaGlbTrack->eta());
    patphiEfficiency[0]->Fill(recoStaGlbTrack->phi());
  }
  if(PATMUON[j].isStandAloneMuon() && PATMUON[j].isGlobalMuon()){
    patetaEfficiency[1]->Fill(recoStaGlbTrack->eta());
    patphiEfficiency[1]->Fill(recoStaGlbTrack->phi());
  }






    }




   }




#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}

// ------------ method called once each job just after ending the event loop  ------------
void 

PATValidation_Muon::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Muon);
