// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Photon.cc PATValidation/PATValidation/src/PATValidation_Photon.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sudhir_Malik
//         Created:  Fri Mar 13 09:52:17 CDT 2009
// $Id: PATValidation_Photon.cc,v 1.5 2009/07/17 18:31:37 malik Exp $
//
//


// system include files
//#include <memory>

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

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "DataFormats/EgammaCandidates/interface/Conversion.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DQMOffline/EGamma/interface/PhotonAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TH1.h"
#include "TH2.h"
#include "PATValidation_Photon.h"



using namespace reco;
using namespace std; 

int nRecoPhotons=0;
int nPatPhotons=0;
//
// class decleration
//

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PATValidation_Photon::PATValidation_Photon(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoPhoton_  = iConfig.getParameter<edm::InputTag>("recoPhoton");
patPhoton_   = iConfig.getParameter<edm::InputTag>("patPhoton");
outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");

nPhoMin = iConfig.getParameter<double>("nPhoMin");
nPhoMax = iConfig.getParameter<double>("nPhoMax");
nPhoBin = iConfig.getParameter<int>("nPhoBin");
eBin    = iConfig.getParameter<int>("eBin");
eMax    = iConfig.getParameter<double>("eMax"); 
eMin    = iConfig.getParameter<double>("eMin"); 
etaBin    = iConfig.getParameter<int>("etaBin");
etaMax    = iConfig.getParameter<double>("etaMax");
etaMin    = iConfig.getParameter<double>("etaMin");
phiBin    =iConfig.getParameter<int>("phiBin");
phiMax    =iConfig.getParameter<double>("phiMax");
phiMin    =iConfig.getParameter<double>("phiMin");
eopBin    =iConfig.getParameter<double>("eopBin"); 
eopMax    =iConfig.getParameter<double>("eopMax"); 
eopMin    =iConfig.getParameter<double>("eopMin"); 

 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " PAT/RECO histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Photon::~PATValidation_Photon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Photon::beginJob(const edm::EventSetup&)
{


dbe = edm::Service<DQMStore>().operator->();
if(dbe){


      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());

      ///RECO histograms

      dbe->setCurrentFolder("Benchmarks/photons/RECO");
	 me["RecoPhoton_pt"]  = dbe->book1D("pt", "RecoPhoton", 50, 0, 200);
         me["RecoPhoton_eta"]  = dbe->book1D("eta", "RecoPhoton", 50, -6, 6);
         me["RecoPhoton_phi"]  = dbe->book1D("phi", "RecoPhoton", 50, -4, 4);	
         
    
         me["RecoTrackIso"] =  dbe->book1D("PhotonTrackIso","Photon Track Isolation"    , 100, 0, 100.);
         me["RecoCaloIso"] =  dbe->book1D("PhotonCaloIso" ,"Photon Calo Isolation"     , 100, 0, 100.);
 
         histname="nPho";
         me_nPho_[0][0]         = dbe->book1D(histname+"All","Number Of Isolated Photon candidates per events: All Ecal" ,nPhoBin,nPhoMin,nPhoMax);
         me_nPho_[0][1] 	= dbe->book1D(histname+"Barrel","Number Of Isolated Photon candidates per events: Ecal Barrel" ,nPhoBin,nPhoMin,nPhoMax); 	   
         me_nPho_[0][2]         = dbe->book1D(histname+"Endcap","Number Of Isolated Photon candidates per events: Ecal Endcap" ,nPhoBin,nPhoMin,nPhoMax);

         histname = "scE"; 
         me_scE_[0][0]         = dbe->book1D(histname+"All","Isolated SC Energy: All Ecal",eBin,eMin,eMax); 
         me_scE_[0][1]         = dbe->book1D(histname+"Barrel","Isolated SC Energy: Barrel",eBin,eMin,eMax);
         me_scE_[0][2]         = dbe->book1D(histname+"Endcap","Isolated SC Energy: Endcap",eBin,eMin,eMax);

         histname = "scEt";
         me_scEt_[0][0]        =dbe->book1D(histname+"All","Isolated SC Et: All Ecal",eBin,eMin,eMax);
	 me_scEt_[0][1]        =dbe->book1D(histname+"Barrel","Isolated SC Et: Barrel",eBin,eMin,eMax);
         me_scEt_[0][2]        =dbe->book1D(histname+"Endcap","Isolated SC Et: Endcap",eBin,eMin,eMax);

         histname = "r9";
         me_r9_[0][0]        =dbe->book1D(histname+"All","Isolated r9: All Ecal",eBin,eMin,eMax);
         me_r9_[0][1]        =dbe->book1D(histname+"Barrel","Isolated r9: Barrel",eBin,eMin,eMax);
         me_r9_[0][2]        =dbe->book1D(histname+"Endcap","Isolated r9: Endcap",eBin,eMin,eMax);

         me_scEta_[0]           =dbe->book1D("scEta","Isolated SC Eta",etaBin,etaMin,etaMax);  
         me_scPhi_[0]           =dbe->book1D("scPhi","Isolated SC Phi",phiBin,phiMin,phiMax); 

         histname = "phoE";
         me_phoE_[0][0]        =dbe->book1D(histname+"All","Isolated Photon Energy: All Ecal",eBin,eMin,eMax);
         me_phoE_[0][1]        =dbe->book1D(histname+"Barrel","Isolated Photon Energy: Barrel",eBin,eMin,eMax);
         me_phoE_[0][2]        =dbe->book1D(histname+"Endcap","Isolated Photon Energy: Endcap",eBin,eMin,eMax);

         histname = "phoEt"; 
         me_phoEt_[0][0]        =dbe->book1D(histname+"All","Isolated Photon Transverse Energy: All Ecal",eBin,eMin,eMax);
         me_phoEt_[0][1]        =dbe->book1D(histname+"Barrel","Isolated Photon Transverse Energy: Barrel",eBin,eMin,eMax);
         me_phoEt_[0][2]        =dbe->book1D(histname+"Endcap","Isolated Photon Transverse Energy: Endcap",eBin,eMin,eMax); 

         me_phoEta_[0]           =dbe->book1D("phoEta","Isolated Photon Eta",etaBin,etaMin,etaMax);
         me_phoPhi_[0]           =dbe->book1D("phoPhi","Isolated Photon Phi",phiBin,phiMin,phiMax);

         histname = "nConv";
         me_nConv_[0][0]        =dbe->book1D(histname+"All","Number Of Conversions per isolated candidates per events: All Ecal",eBin,eMin,eMax);
         me_nConv_[0][1]        =dbe->book1D(histname+"Barrel","Number Of Conversions per isolated candidates per events: Barrel",eBin,eMin,eMax);
         me_nConv_[0][2]        =dbe->book1D(histname+"Endcap","Number Of Conversions per isolated candidates per events",eBin,eMin,eMax);

         me_convEta_[0]              =dbe->book1D("convEta","Isolated converted Photon Eta",etaBin,etaMin,etaMax);
         me_convPhi_[0]              =dbe->book1D("convPhi","Isolated converted Photon Phi",phiBin,phiMin,phiMax); 

         
         histname="EoverPTracks";
         me_EoverPTracks_[0][0] =dbe->book1D(histname+"All","Isolated photons conversion E/p: all Ecal",eopBin,eopMin,eopMax);
         me_EoverPTracks_[0][1] =dbe->book1D(histname+"Barrel","Isolated photons conversion E/p: Barrel Ecal",eopBin,eopMin,eopMax); 
         me_EoverPTracks_[0][2] =dbe->book1D(histname+"Endcap","Isolated photons conversion E/p: Endcap Ecal",eopBin,eopMin,eopMax);
         
         me_tkChi2_[0]           =dbe->book1D("tkChi2","Isolated Photons:Tracks from conversions: #chi^{2} of tracks",100, 0, 20.);
        
         histname="DPhiTracksAtVtx";
         me_DPhiTracksAtVtx_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_DPhiTracksAtVtx_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DPhiTracksAtVtx_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);
 
         histname="DCotTracksx";
         me_DCotTracks_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: All Ecal",100, 0, 1);
         me_DCotTracks_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DCotTracks_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Endcap Ecal",100, 0, 1);


         histname="InvMass";
         me_invMass_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: Pair invariant mass: All Ecal",100, 0, 1.5);
         me_invMass_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: Pair invariant mass: Barrel Ecal",100, 0, 1.5);
         me_invMass_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: Pair invariant mass: Endcap Ecal",100, 0, 1.5);

         histname="DPhiTracksAtEcal";
         me_DPhiTracksAtEcal_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_DPhiTracksAtEcal_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DPhiTracksAtEcal_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);

         histname="DEtaTracksAtEcal";
         me_DEtaTracksAtEcal_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: All Ecal",100, 0, 1);
         me_DEtaTracksAtEcal_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DEtaTracksAtEcal_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Endcap Ecal",100, 0, 1); 
 
         me_zPVFromTracks_[0]       =dbe->book1D("zPVFromTracks","Isolated Photons: PV z from conversion tracks"     , 100, -25.,  25.);

/////////// NON ISO stuff
         histname="nPhoNoIso";
         me_nPho_[1][0]         = dbe->book1D(histname+"All","Number Of Non Isolated Photon candidates per events: All Ecal" ,nPhoBin,nPhoMin,nPhoMax);
         me_nPho_[1][1]         = dbe->book1D(histname+"Barrel","Number Of Non Isolated Photon candidates per events: Ecal Barrel" ,nPhoBin,nPhoMin,nPhoMax);
         me_nPho_[1][2]         = dbe->book1D(histname+"Endcap","Number Of Non Isolated Photon candidates per events: Ecal Endcap" ,nPhoBin,nPhoMin,nPhoMax);
         
         histname = "scENoIso";
         me_scE_[1][0]         = dbe->book1D(histname+"All","Non Isolated SC Energy: All Ecal",eBin,eMin,eMax);
         me_scE_[1][1]         = dbe->book1D(histname+"Barrel","Non Isolated SC Energy: Barrel",eBin,eMin,eMax);
         me_scE_[1][2]         = dbe->book1D(histname+"Endcap","Non Isolated SC Energy: Endcap",eBin,eMin,eMax);
         
         histname = "scEtNoIso";
         me_scEt_[1][0]        =dbe->book1D(histname+"All","Non Isolated SC Et: All Ecal",eBin,eMin,eMax);
         me_scEt_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated SC Et: Barrel",eBin,eMin,eMax);
         me_scEt_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated SC Et: Endcap",eBin,eMin,eMax);


         histname = "r9NoIso";
         me_r9_[1][0]        =dbe->book1D(histname+"All","Non Isolated r9: All Ecal",eBin,eMin,eMax);
         me_r9_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated r9: Barrel",eBin,eMin,eMax);
         me_r9_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated r9: Endcap",eBin,eMin,eMax);
         
         me_scEta_[1]           =dbe->book1D("scEtaNoIso","Isolated SC Eta",etaBin,etaMin,etaMax);
         me_scPhi_[1]           =dbe->book1D("scPhiNoIso","Isolated SC Phi",phiBin,phiMin,phiMax);

         histname = "phoENoIso";
         me_phoE_[1][0]        =dbe->book1D(histname+"All","Non Isolated Photon Energy: All Ecal",eBin,eMin,eMax);
         me_phoE_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated Photon Energy: Barrel",eBin,eMin,eMax);
         me_phoE_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated Photon Energy: Endcap",eBin,eMin,eMax);
         
         histname = "phoEtNoIso";
         me_phoEt_[1][0]        =dbe->book1D(histname+"All","Non Isolated Photon Transverse Energy: All Ecal",eBin,eMin,eMax);
         me_phoEt_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated Photon Transverse Energy: Barrel",eBin,eMin,eMax);
         me_phoEt_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated Photon Transverse Energy: Endcap",eBin,eMin,eMax);
  
         me_phoEta_[1]           =dbe->book1D("phoEtaNoIso","Isolated Photon Eta",etaBin,etaMin,etaMax);
         me_phoPhi_[1]           =dbe->book1D("phoPhiNoIso","Isolated Photon Phi",phiBin,phiMin,phiMax);


	 histname="nConvNoIso";
         me_nConv_[1][0]        =dbe->book1D(histname+"All","Number Of Conversions per Non isolated candidates per events: All Ecal",eBin,eMin,eMax);
         me_nConv_[1][1]        =dbe->book1D(histname+"Barrel","Number Of Conversions per Non isolated candidates per events: Barrel",eBin,eMin,eMax);
         me_nConv_[1][2]        =dbe->book1D(histname+"Endcap","Number Of Conversions per Non isolated candidates per events",eBin,eMin,eMax);
         
         me_convEta_[1]              =dbe->book1D("convEtaNoIso","Non Isolated converted Photon Eta",etaBin,etaMin,etaMax);
         me_convPhi_[1]              =dbe->book1D("convPhiNoIso","Non Isolated converted Photon Phi",phiBin,phiMin,phiMax);
   
       	 histname = "EoverPTracksNoIso";
         me_EoverPTracks_[1][0] =dbe->book1D(histname+"All","Non Isolated photons conversion E/p: all Ecal",eopBin,eopMin,eopMax);
         me_EoverPTracks_[1][1] =dbe->book1D(histname+"Barrel","Non Isolated photons conversion E/p: Barrel Ecal",eopBin,eopMin,eopMax);
         me_EoverPTracks_[1][2] =dbe->book1D(histname+"Endcap","Non Isolated photons conversion E/p: Endcap Ecal",eopBin,eopMin,eopMax);

         me_tkChi2_[1]           =dbe->book1D("tkChi2NoIso","Isolated Photons:Tracks from conversions: #chi^{2} of tracks",100, 0, 20.);

	 histname = "DPhiTracksAtVtxNoIso";
         me_DPhiTracksAtVtx_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_DPhiTracksAtVtx_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DPhiTracksAtVtx_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);

         histname="DCotTracksNoIso";
         me_DCotTracks_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: All Ecal",100, 0, 1);
         me_DCotTracks_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DCotTracks_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Endcap Ecal",100, 0, 1);

	 histname="InvMassNoIso";
         me_invMass_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: Pair invariant mass: All Ecal",100, 0, 1.5);
         me_invMass_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: Pair invariant mass: Barrel Ecal",100, 0, 1.5);
         me_invMass_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: Pair invariant mass: Endcap Ecal",100, 0, 1.5);

         histname="DPhiTracksAtEcalNoIso";
         me_DPhiTracksAtEcal_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_DPhiTracksAtEcal_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DPhiTracksAtEcal_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);
         
         histname="DEtaTracksAtEcalNoIso";
         me_DEtaTracksAtEcal_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: All Ecal",100, 0, 1);
         me_DEtaTracksAtEcal_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_DEtaTracksAtEcal_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Endcap Ecal",100, 0, 1);

         me_zPVFromTracks_[1]       =dbe->book1D("zPVFromTracksNoIso","Non Isolated Photons: PV z from conversion tracks"     , 100, -25.,  25.);
                
     

	 ////////PAT histograms 

       dbe->setCurrentFolder("Benchmarks/photons/PAT");
         me["PatPhoton_pt"]  = dbe->book1D("pt", "PatPhoton", 50, 0, 200);
         me["PatPhoton_eta"]  = dbe->book1D("eta", "PatPhoton", 50, -6, 6);
         me["PatPhoton_phi"]  = dbe->book1D("phi", "PatPhoton", 50, -4, 4);
              
	    
         me["PatTrackIso"] =  dbe->book1D("PhotonTrackIso","Photon Track Isolation"    , 100, 0, 100.);
         me["PatCaloIso"] =  dbe->book1D("PhotonCaloIso" ,"Photon Calo Isolation"     , 100, 0, 100.);
 
         histname="nPho";
         me_pat_nPho_[0][0]         = dbe->book1D(histname+"All","Number Of Isolated Photon candidates per events: All Ecal" ,nPhoBin,nPhoMin,nPhoMax);
         me_pat_nPho_[0][1] 	= dbe->book1D(histname+"Barrel","Number Of Isolated Photon candidates per events: Ecal Barrel" ,nPhoBin,nPhoMin,nPhoMax); 	   
         me_pat_nPho_[0][2]         = dbe->book1D(histname+"Endcap","Number Of Isolated Photon candidates per events: Ecal Endcap" ,nPhoBin,nPhoMin,nPhoMax);

         histname = "scE"; 
         me_pat_scE_[0][0]         = dbe->book1D(histname+"All","Isolated SC Energy: All Ecal",eBin,eMin,eMax); 
         me_pat_scE_[0][1]         = dbe->book1D(histname+"Barrel","Isolated SC Energy: Barrel",eBin,eMin,eMax);
         me_pat_scE_[0][2]         = dbe->book1D(histname+"Endcap","Isolated SC Energy: Endcap",eBin,eMin,eMax);

         histname = "scEt";
         me_pat_scEt_[0][0]        =dbe->book1D(histname+"All","Isolated SC Et: All Ecal",eBin,eMin,eMax);
	 me_pat_scEt_[0][1]        =dbe->book1D(histname+"Barrel","Isolated SC Et: Barrel",eBin,eMin,eMax);
         me_pat_scEt_[0][2]        =dbe->book1D(histname+"Endcap","Isolated SC Et: Endcap",eBin,eMin,eMax);

         histname = "r9";
         me_pat_r9_[0][0]        =dbe->book1D(histname+"All","Isolated r9: All Ecal",eBin,eMin,eMax);
         me_pat_r9_[0][1]        =dbe->book1D(histname+"Barrel","Isolated r9: Barrel",eBin,eMin,eMax);
         me_pat_r9_[0][2]        =dbe->book1D(histname+"Endcap","Isolated r9: Endcap",eBin,eMin,eMax);

         me_pat_scEta_[0]           =dbe->book1D("scEta","Isolated SC Eta",etaBin,etaMin,etaMax);  
         me_pat_scPhi_[0]           =dbe->book1D("scPhi","Isolated SC Phi",phiBin,phiMin,phiMax); 

         histname = "phoE";
         me_pat_phoE_[0][0]        =dbe->book1D(histname+"All","Isolated Photon Energy: All Ecal",eBin,eMin,eMax);
         me_pat_phoE_[0][1]        =dbe->book1D(histname+"Barrel","Isolated Photon Energy: Barrel",eBin,eMin,eMax);
         me_pat_phoE_[0][2]        =dbe->book1D(histname+"Endcap","Isolated Photon Energy: Endcap",eBin,eMin,eMax);

         histname = "phoEt"; 
         me_pat_phoEt_[0][0]        =dbe->book1D(histname+"All","Isolated Photon Transverse Energy: All Ecal",eBin,eMin,eMax);
         me_pat_phoEt_[0][1]        =dbe->book1D(histname+"Barrel","Isolated Photon Transverse Energy: Barrel",eBin,eMin,eMax);
         me_pat_phoEt_[0][2]        =dbe->book1D(histname+"Endcap","Isolated Photon Transverse Energy: Endcap",eBin,eMin,eMax); 

         me_pat_phoEta_[0]           =dbe->book1D("phoEta","Isolated Photon Eta",etaBin,etaMin,etaMax);
         me_pat_phoPhi_[0]           =dbe->book1D("phoPhi","Isolated Photon Phi",phiBin,phiMin,phiMax);

         histname = "nConv";
         me_pat_nConv_[0][0]        =dbe->book1D(histname+"All","Number Of Conversions per isolated candidates per events: All Ecal",eBin,eMin,eMax);
         me_pat_nConv_[0][1]        =dbe->book1D(histname+"Barrel","Number Of Conversions per isolated candidates per events: Barrel",eBin,eMin,eMax);
         me_pat_nConv_[0][2]        =dbe->book1D(histname+"Endcap","Number Of Conversions per isolated candidates per events",eBin,eMin,eMax);

         me_pat_convEta_[0]              =dbe->book1D("convEta","Isolated converted Photon Eta",etaBin,etaMin,etaMax);
         me_pat_convPhi_[0]              =dbe->book1D("convPhi","Isolated converted Photon Phi",phiBin,phiMin,phiMax); 

         
         histname="EoverPTracks";
         me_pat_EoverPTracks_[0][0] =dbe->book1D(histname+"All","Isolated photons conversion E/p: all Ecal",eopBin,eopMin,eopMax);
         me_pat_EoverPTracks_[0][1] =dbe->book1D(histname+"Barrel","Isolated photons conversion E/p: Barrel Ecal",eopBin,eopMin,eopMax); 
         me_pat_EoverPTracks_[0][2] =dbe->book1D(histname+"Endcap","Isolated photons conversion E/p: Endcap Ecal",eopBin,eopMin,eopMax);
         
         me_pat_tkChi2_[0]           =dbe->book1D("tkChi2","Isolated Photons:Tracks from conversions: #chi^{2} of tracks",100, 0, 20.);
        
         histname="DPhiTracksAtVtx";
         me_pat_DPhiTracksAtVtx_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DPhiTracksAtVtx_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DPhiTracksAtVtx_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);
 
         histname="DCotTracksx";
         me_pat_DCotTracks_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DCotTracks_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DCotTracks_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Endcap Ecal",100, 0, 1);


         histname="InvMass";
         me_pat_invMass_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: Pair invariant mass: All Ecal",100, 0, 1.5);
         me_pat_invMass_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: Pair invariant mass: Barrel Ecal",100, 0, 1.5);
         me_pat_invMass_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: Pair invariant mass: Endcap Ecal",100, 0, 1.5);

         histname="DPhiTracksAtEcal";
         me_pat_DPhiTracksAtEcal_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DPhiTracksAtEcal_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DPhiTracksAtEcal_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);

         histname="DEtaTracksAtEcal";
         me_pat_DEtaTracksAtEcal_[0][0]=dbe->book1D(histname+"All","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DEtaTracksAtEcal_[0][1]=dbe->book1D(histname+"Barrel","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DEtaTracksAtEcal_[0][2]=dbe->book1D(histname+"Endcap","Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Endcap Ecal",100, 0, 1); 
 
         me_pat_zPVFromTracks_[0]       =dbe->book1D("zPVFromTracks","Isolated Photons: PV z from conversion tracks"     , 100, -25.,  25.);

/////////// NON ISO stuff
         histname="nPhoNoIso";
         me_pat_nPho_[1][0]         = dbe->book1D(histname+"All","Number Of Non Isolated Photon candidates per events: All Ecal" ,nPhoBin,nPhoMin,nPhoMax);
         me_pat_nPho_[1][1]         = dbe->book1D(histname+"Barrel","Number Of Non Isolated Photon candidates per events: Ecal Barrel" ,nPhoBin,nPhoMin,nPhoMax);
         me_pat_nPho_[1][2]         = dbe->book1D(histname+"Endcap","Number Of Non Isolated Photon candidates per events: Ecal Endcap" ,nPhoBin,nPhoMin,nPhoMax);
         
         histname = "scENoIso";
         me_pat_scE_[1][0]         = dbe->book1D(histname+"All","Non Isolated SC Energy: All Ecal",eBin,eMin,eMax);
         me_pat_scE_[1][1]         = dbe->book1D(histname+"Barrel","Non Isolated SC Energy: Barrel",eBin,eMin,eMax);
         me_pat_scE_[1][2]         = dbe->book1D(histname+"Endcap","Non Isolated SC Energy: Endcap",eBin,eMin,eMax);
         
         histname = "scEtNoIso";
         me_pat_scEt_[1][0]        =dbe->book1D(histname+"All","Non Isolated SC Et: All Ecal",eBin,eMin,eMax);
         me_pat_scEt_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated SC Et: Barrel",eBin,eMin,eMax);
         me_pat_scEt_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated SC Et: Endcap",eBin,eMin,eMax);


         histname = "r9NoIso";
         me_pat_r9_[1][0]        =dbe->book1D(histname+"All","Non Isolated r9: All Ecal",eBin,eMin,eMax);
         me_pat_r9_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated r9: Barrel",eBin,eMin,eMax);
         me_pat_r9_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated r9: Endcap",eBin,eMin,eMax);
         
         me_pat_scEta_[1]           =dbe->book1D("scEtaNoIso","Isolated SC Eta",etaBin,etaMin,etaMax);
         me_pat_scPhi_[1]           =dbe->book1D("scPhiNoIso","Isolated SC Phi",phiBin,phiMin,phiMax);

         histname = "phoENoIso";
         me_pat_phoE_[1][0]        =dbe->book1D(histname+"All","Non Isolated Photon Energy: All Ecal",eBin,eMin,eMax);
         me_pat_phoE_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated Photon Energy: Barrel",eBin,eMin,eMax);
         me_pat_phoE_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated Photon Energy: Endcap",eBin,eMin,eMax);
         
         histname = "phoEtNoIso";
         me_pat_phoEt_[1][0]        =dbe->book1D(histname+"All","Non Isolated Photon Transverse Energy: All Ecal",eBin,eMin,eMax);
         me_pat_phoEt_[1][1]        =dbe->book1D(histname+"Barrel","Non Isolated Photon Transverse Energy: Barrel",eBin,eMin,eMax);
         me_pat_phoEt_[1][2]        =dbe->book1D(histname+"Endcap","Non Isolated Photon Transverse Energy: Endcap",eBin,eMin,eMax);
  
         me_pat_phoEta_[1]           =dbe->book1D("phoEtaNoIso","Isolated Photon Eta",etaBin,etaMin,etaMax);
         me_pat_phoPhi_[1]           =dbe->book1D("phoPhiNoIso","Isolated Photon Phi",phiBin,phiMin,phiMax);


	 histname="nConvNoIso";
         me_pat_nConv_[1][0]        =dbe->book1D(histname+"All","Number Of Conversions per Non isolated candidates per events: All Ecal",eBin,eMin,eMax);
         me_pat_nConv_[1][1]        =dbe->book1D(histname+"Barrel","Number Of Conversions per Non isolated candidates per events: Barrel",eBin,eMin,eMax);
         me_pat_nConv_[1][2]        =dbe->book1D(histname+"Endcap","Number Of Conversions per Non isolated candidates per events",eBin,eMin,eMax);
         
         me_pat_convEta_[1]              =dbe->book1D("convEtaNoIso","Non Isolated converted Photon Eta",etaBin,etaMin,etaMax);
         me_pat_convPhi_[1]              =dbe->book1D("convPhiNoIso","Non Isolated converted Photon Phi",phiBin,phiMin,phiMax);
   
       	 histname = "EoverPTracksNoIso";
         me_pat_EoverPTracks_[1][0] =dbe->book1D(histname+"All","Non Isolated photons conversion E/p: all Ecal",eopBin,eopMin,eopMax);
         me_pat_EoverPTracks_[1][1] =dbe->book1D(histname+"Barrel","Non Isolated photons conversion E/p: Barrel Ecal",eopBin,eopMin,eopMax);
         me_pat_EoverPTracks_[1][2] =dbe->book1D(histname+"Endcap","Non Isolated photons conversion E/p: Endcap Ecal",eopBin,eopMin,eopMax);

         me_pat_tkChi2_[1]           =dbe->book1D("tkChi2NoIso","Isolated Photons:Tracks from conversions: #chi^{2} of tracks",100, 0, 20.);

	 histname = "DPhiTracksAtVtxNoIso";
         me_pat_DPhiTracksAtVtx_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DPhiTracksAtVtx_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DPhiTracksAtVtx_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);

         histname="DCotTracksNoIso";
         me_pat_DCotTracks_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DCotTracks_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DCotTracks_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta cotg(#Theta) Tracks at vertex: Endcap Ecal",100, 0, 1);

	 histname="InvMassNoIso";
         me_pat_invMass_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: Pair invariant mass: All Ecal",100, 0, 1.5);
         me_pat_invMass_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: Pair invariant mass: Barrel Ecal",100, 0, 1.5);
         me_pat_invMass_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: Pair invariant mass: Endcap Ecal",100, 0, 1.5);

         histname="DPhiTracksAtEcalNoIso";
         me_pat_DPhiTracksAtEcal_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DPhiTracksAtEcal_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DPhiTracksAtEcal_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#phi Tracks at vertex: Endcap Ecal",100, 0, 1);
         
         histname="DEtaTracksAtEcalNoIso";
         me_pat_DEtaTracksAtEcal_[1][0]=dbe->book1D(histname+"All","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: All Ecal",100, 0, 1);
         me_pat_DEtaTracksAtEcal_[1][1]=dbe->book1D(histname+"Barrel","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Barrel Ecal",100, 0, 1);
         me_pat_DEtaTracksAtEcal_[1][2]=dbe->book1D(histname+"Endcap","Non Isolated Photons:Tracks from conversions: #delta#eta Tracks at vertex: Endcap Ecal",100, 0, 1);

         me_pat_zPVFromTracks_[1]       =dbe->book1D("zPVFromTracksNoIso","Non Isolated Photons: PV z from conversion tracks"     , 100, -25.,  25.);
                
     
   


  } //end of dbe
  return;
    
  
}  //end of begin job


// ------------ method called to for each event  ------------
void
PATValidation_Photon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views
//====================================RECO PHOTON=====================================
//    typedef edm::View<reco::Candidate> candidateCollection ;
      typedef edm::View<reco::Photon> photonCollection;
   //get reco photon collection

//     edm::Handle<candidateCollection> recopho_hnd;

      edm::Handle<photonCollection> recopho_hnd;

     iEvent.getByLabel(recoPhoton_, recopho_hnd); 
//     const candidateCollection & RECOPHO =*recopho_hnd;
       const photonCollection & RECOPHO =*recopho_hnd; 

    // Loop over Reco Taus 
     for (int i = 0; i != RECOPHO.size(); ++i) { 
        me["RecoPhoton_pt"]->Fill(RECOPHO[i].pt());
        nRecoPhotons++;
	me["RecoPhoton_eta"]->Fill(RECOPHO[i].eta());
	me["RecoPhoton_phi"]->Fill(RECOPHO[i].phi());  
//        cout << "RECO photon pt is = " << RECOPHO[i].pt() << endl; 
//       me["RecoTrackIso"]->Fill(RECOPHO[i].trackIso());
//       me["RecoCaloIso"] ->Fill(RECOPHO[i].caloIso());

        const Float_t mElec= 0.000511;

	std::vector<int> nPho(2);
        std::vector<int> nPhoBarrel(2);
	std::vector<int> nPhoEndcap(2);
	for ( unsigned int ii=0; ii<nPho.size(); ii++ ) nPho[ii]=0;
        for ( unsigned int ii=0; ii<nPhoBarrel.size(); ii++ ) nPhoBarrel[ii]=0;
        for ( unsigned int ii=0; ii<nPhoEndcap.size(); ii++ ) nPhoEndcap[ii]=0;

       bool  phoIsInBarrel=false;
       bool  phoIsInEndcap=false;

        ///////////////////a little bit corrections
        float etaPho=RECOPHO[i].eta();
        if ( fabs(etaPho) <  1.479 ) {
         phoIsInBarrel=true;
        } else {
         phoIsInEndcap=true;
        }

//      float phiClu=RECOPHO[i].superCluster()->phi();
        float etaClu=RECOPHO[i].superCluster()->eta();

        bool  scIsInBarrel=false;
        bool  scIsInEndcap=false;
        if ( fabs(etaClu) <  1.479 ) 
        scIsInBarrel=true;
        else
        scIsInEndcap=true;

//What is the criteria for isolation// Keep it for future
//        bool isIsolated=false;

       int type=0;
//What is the criteria for isolation// Keep it for future  
     //if ( !isIsolated ) type=1;

       nPho[type]++; 
       if (phoIsInBarrel) nPhoBarrel[type]++;
       if (phoIsInEndcap) nPhoEndcap[type]++;

       /// e3x3 needs to be calculated
    float e3x3= 0; // EcalClusterTools::e3x3(  *(   (RECOPHO[i]).superCluster()->seed()  ), &ecalRecHitCollection, &(*topology));
    float r9 =e3x3/( (RECOPHO[i]).superCluster()->rawEnergy()+ (RECOPHO[i]).superCluster()->preshowerEnergy());

    me_scEta_[type]->Fill(RECOPHO[i].superCluster()->eta());
    me_scPhi_[type]->Fill(RECOPHO[i].superCluster()->phi());


///////////////////////////////////////////////////////////////two dimensional histogram
//    me_scEtaPhi_[type]->Fill(RECOPHO[i].superCluster()->eta(),RECOPHO[i].superCluster()->phi() );
    me_scE_[type][0]->Fill(RECOPHO[i].superCluster()->energy());
    me_scEt_[type][0]->Fill(RECOPHO[i].superCluster()->energy()/cosh(RECOPHO[i].superCluster()->eta()));
    me_r9_[type][0]->Fill(r9);
    me_phoEta_[type]->Fill(RECOPHO[i].eta());
    me_phoPhi_[type]->Fill(RECOPHO[i].phi());
    me_phoE_[type][0]->Fill(RECOPHO[i].energy());
    me_phoEt_[type][0]->Fill(RECOPHO[i].energy()/ cosh(RECOPHO[i].eta()));
    me_nConv_[type][0]->Fill(float(RECOPHO[i].conversions().size()));
    if ( scIsInBarrel ) {
      me_scE_[type][1]->Fill(RECOPHO[i].superCluster()->energy());
      me_scEt_[type][1]->Fill(RECOPHO[i].superCluster()->energy()/cosh(RECOPHO[i].superCluster()->eta()));
      me_r9_[type][1]->Fill(r9);
     }

    if ( scIsInEndcap ) {
      me_scE_[type][2]->Fill(RECOPHO[i].superCluster()->energy());
      me_scEt_[type][2]->Fill(RECOPHO[i].superCluster()->energy()/cosh(RECOPHO[i].superCluster()->eta()));
      me_r9_[type][2]->Fill(r9);
     }

    if ( phoIsInBarrel ) {
      me_phoE_[type][1]->Fill(RECOPHO[i].energy());
      me_phoEt_[type][1]->Fill(RECOPHO[i].energy()/cosh(RECOPHO[i].eta()));
      me_nConv_[type][1]->Fill(float(RECOPHO[i].conversions().size()));
     }
    
    if ( phoIsInEndcap ) {
      me_phoE_[type][2]->Fill(RECOPHO[i].energy());
     me_phoEt_[type][2]->Fill(RECOPHO[i].energy()/ cosh( (RECOPHO[i]).eta()));
      me_nConv_[type][2]->Fill(float(RECOPHO[i].conversions().size()));
     }
////////////////// plot quantitied related to conversions
     reco::ConversionRefVector conversions = RECOPHO[i].conversions();

     for (unsigned int iConv=0; iConv<conversions.size(); iConv++) {

 // two dimensional histogram
 //      me_r9VsNofTracks_[type][0]->Fill( r9, conversions[iConv]->nTracks() ) ; 
  
 //      if ( phoIsInBarrel ) me_r9VsNofTracks_[type][1]->Fill( r9,  conversions[iConv]->nTracks() ) ; 
  
 //      if ( phoIsInEndcap ) me_r9VsNofTracks_[type][2]->Fill( r9,  conversions[iConv]->nTracks() ) ; 
 
       if ( conversions[iConv]->nTracks() <2 ) continue; 
  
       me_convEta_[type]->Fill( conversions[iConv]-> caloCluster()[0]->eta() );
       me_convPhi_[type]->Fill( conversions[iConv]-> caloCluster()[0]->phi() );
       me_EoverPTracks_[type][0] ->Fill( conversions[iConv]->EoverP()) ;
       if ( phoIsInBarrel ) me_EoverPTracks_[type][1] ->Fill( conversions[iConv]->EoverP());
       if ( phoIsInEndcap ) me_EoverPTracks_[type][2] ->Fill( conversions[iConv]->EoverP());
 /* /// two dimensional histogram      
       if ( conversions[iConv]->conversionVertex().isValid()) 
         me_convVtxRvsZ_[type] ->Fill ( fabs (conversions[iConv]->conversionVertex().position().z() ),  sqrt(conversions[iConv]->conversionVertex().position().perp2())  ) ;
*/         
  
        me_zPVFromTracks_[type]->Fill ( conversions[iConv]->zOfPrimaryVertexFromTracks());
        std::vector<reco::TrackRef> tracks = conversions[iConv]->tracks();

         
        float px=0;
        float py=0;
        float pz=0;
        float e=0;
        for (unsigned int l=0; l<tracks.size(); l++) {
 //         two dimensional histo
 //         p_tk_nHitsVsEta_[type]->fill(  conversions[iConv]->caloCluster()[0]->eta(),   float(tracks[l]->recHitsSize() ) );
          me_tkChi2_[type] ->Fill (tracks[l]->normalizedChi2()); 
          px+= tracks[l]->innerMomentum().x();
          py+= tracks[l]->innerMomentum().y();
          pz+= tracks[l]->innerMomentum().z();
          e +=  sqrt (  tracks[l]->innerMomentum().x()*tracks[l]->innerMomentum().x() +
                        tracks[l]->innerMomentum().y()*tracks[l]->innerMomentum().y() +
                        tracks[l]->innerMomentum().z()*tracks[l]->innerMomentum().z() +
                        +  mElec*mElec ) ;
        }
        float totP = sqrt(px*px +py*py + pz*pz);
        float invM=  (e + totP) * (e-totP) ;
  
        if ( invM> 0.) {
          invM= sqrt( invM);
        } else {
          invM=-1;
        }
        me_invMass_[type][0] ->Fill(invM);
        if (phoIsInBarrel) me_invMass_[type][1] ->Fill(invM);
        if (phoIsInEndcap) me_invMass_[type][2] ->Fill(invM);
 
        float  dPhiTracksAtVtx = -99;
        
        float phiTk1= tracks[0]->innerMomentum().phi();
        float phiTk2= tracks[1]->innerMomentum().phi();
        dPhiTracksAtVtx = phiTk1-phiTk2;
        dPhiTracksAtVtx = phiNormalization( dPhiTracksAtVtx );
        me_DPhiTracksAtVtx_[type][0]->Fill( dPhiTracksAtVtx);
        if ( phoIsInBarrel ) me_DPhiTracksAtVtx_[type][1]->Fill( dPhiTracksAtVtx);
        if ( phoIsInEndcap ) me_DPhiTracksAtVtx_[type][2]->Fill( dPhiTracksAtVtx);
        me_DCotTracks_[type][0] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
        if ( phoIsInBarrel ) me_DCotTracks_[type][1] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
        if ( phoIsInEndcap ) me_DCotTracks_[type][2] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
                
        float  dPhiTracksAtEcal=-99;
        float  dEtaTracksAtEcal=-99;
        if (conversions[iConv]-> bcMatchingWithTracks()[0].isNonnull() && conversions[iConv]->bcMatchingWithTracks()[1].isNonnull() ) {
 //       cout << "SUDHIRMALIK1" << endl;  
          float recoPhi1 = conversions[iConv]->ecalImpactPosition()[0].phi();
          float recoPhi2 = conversions[iConv]->ecalImpactPosition()[1].phi();
          float recoEta1 = conversions[iConv]->ecalImpactPosition()[0].eta();
          float recoEta2 = conversions[iConv]->ecalImpactPosition()[1].eta();
          float bcPhi1 = conversions[iConv]->bcMatchingWithTracks()[0]->phi();
          float bcPhi2 = conversions[iConv]->bcMatchingWithTracks()[1]->phi();
 //         float bcEta1 = conversions[iConv]->bcMatchingWithTracks()[0]->eta();
 //         float bcEta2 = conversions[iConv]->bcMatchingWithTracks()[1]->eta();
          recoPhi1 = phiNormalization(recoPhi1);
          recoPhi2 = phiNormalization(recoPhi2);
          bcPhi1 = phiNormalization(bcPhi1);
          bcPhi2 = phiNormalization(bcPhi2);
          dPhiTracksAtEcal = recoPhi1 -recoPhi2;
          dPhiTracksAtEcal = phiNormalization( dPhiTracksAtEcal );
          dEtaTracksAtEcal = recoEta1 -recoEta2;
          
          me_DPhiTracksAtEcal_[type][0]->Fill(dPhiTracksAtEcal);
          me_DEtaTracksAtEcal_[type][0]->Fill( dEtaTracksAtEcal);
                  
       //cout << "SUDHIRMALIK2" << endl;

          if ( phoIsInBarrel ) {
            me_DPhiTracksAtEcal_[type][1]->Fill( dPhiTracksAtEcal);
            me_DEtaTracksAtEcal_[type][1]->Fill( dEtaTracksAtEcal);
          }
         //          cout << "SUDHIRMALIK3" << endl;

          if ( phoIsInEndcap ) {
            me_DPhiTracksAtEcal_[type][2]->Fill( dPhiTracksAtEcal);
            me_DEtaTracksAtEcal_[type][2]->Fill( dEtaTracksAtEcal);
          //         cout << "SUDHIRMALIK4" << endl;

          }
      }
 
    } // loop over conversions
 
 
 //  }/// End loop over Reco  particles
   me_nPho_[0][0]-> Fill(float(nPho[0]));
   me_nPho_[0][1]-> Fill(float(nPhoBarrel[0]));
   me_nPho_[0][2]-> Fill(float(nPhoEndcap[0]));
   me_nPho_[1][0]-> Fill(float(nPho[1]));
   me_nPho_[1][1]-> Fill(float(nPhoBarrel[1]));
   me_nPho_[1][2]-> Fill(float(nPhoEndcap[1]));
 

     }
//=============================PAT PHOTON====================================

    //get pat photons collection
     edm::Handle <std::vector<pat::Photon> >  patpho_hnd;
     iEvent.getByLabel(patPhoton_, patpho_hnd);
     std::vector<pat::Photon> const & PATPHO = *patpho_hnd;
   // Loop over Pat Photons
     for (int j = 0; j != PATPHO.size(); ++j) {

      //////////////////////////////////////////////////////////////////////////////

      me["PatPhoton_pt"]->Fill(PATPHO[j].pt());
        nPatPhotons++;
	me["PatPhoton_eta"]->Fill(PATPHO[j].eta());
	me["PatPhoton_phi"]->Fill(PATPHO[j].phi());  
//        cout << "PAT photon pt is = " << PATPHO[j].pt() << endl; 
//       me["PatTrackIso"]->Fill(PATPHO[j].trackIso());
//       me["PatCaloIso"] ->Fill(PATPHO[j].caloIso());

        const Float_t mElec= 0.000511;

	std::vector<int> npatPho(2);
        std::vector<int> npatPhoBarrel(2);
	std::vector<int> npatPhoEndcap(2);
	for ( unsigned int jj=0; jj<npatPho.size(); jj++ ) npatPho[jj]=0;
        for ( unsigned int jj=0; jj<npatPhoBarrel.size(); jj++ ) npatPhoBarrel[jj]=0;
        for ( unsigned int jj=0; jj<npatPhoEndcap.size(); jj++ ) npatPhoEndcap[jj]=0;

       bool  patphoIsInBarrel=false;
       bool  patphoIsInEndcap=false;

        ///////////////////a little bit corrections
        float etapatPho=PATPHO[j].eta();
        if ( fabs(etapatPho) <  1.479 ) {
         patphoIsInBarrel=true;
        } else {
         patphoIsInEndcap=true;
        }

//      float patphiClu=PATPHO[j].superCluster()->phi();
        float patetaClu=PATPHO[j].superCluster()->eta();

        bool  patscIsInBarrel=false;
        bool  patscIsInEndcap=false;
        if ( fabs(patetaClu) <  1.479 ) 
        patscIsInBarrel=true;
        else
        patscIsInEndcap=true;

        bool patisIsolated=false;

       int type=0;
//What is the criteria for isolation// Keep it for future
//       if ( !patisIsolated ) type=1;

       npatPho[type]++; 
       if (patphoIsInBarrel) npatPhoBarrel[type]++;
       if (patphoIsInEndcap) npatPhoEndcap[type]++;

       /// e3x3 needs to be calculated
    float e3x3= 0; // EcalClusterTools::e3x3(  *(   (PATPHO[i]).superCluster()->seed()  ), &ecalRecHitCollection, &(*topology));
    float r9 =e3x3/( (PATPHO[j]).superCluster()->rawEnergy()+ (PATPHO[j]).superCluster()->preshowerEnergy());

    me_pat_scEta_[type]->Fill(PATPHO[j].superCluster()->eta());
    me_pat_scPhi_[type]->Fill(PATPHO[j].superCluster()->phi());

///////////////////////////////////////////////////////////////two dimensional histogram
//    me_scEtaPhi_[type]->Fill(PATPHO[j].superCluster()->eta(),PATPHO[j].superCluster()->phi() );

    me_pat_scE_[type][0]->Fill(PATPHO[j].superCluster()->energy());
    me_pat_scEt_[type][0]->Fill(PATPHO[j].superCluster()->energy()/cosh(PATPHO[j].superCluster()->eta()));
    me_pat_r9_[type][0]->Fill(r9);
    me_pat_phoEta_[type]->Fill(PATPHO[j].eta());
    me_pat_phoPhi_[type]->Fill(PATPHO[j].phi());
    me_pat_phoE_[type][0]->Fill(PATPHO[j].energy());
    me_pat_phoEt_[type][0]->Fill(PATPHO[j].energy()/ cosh(PATPHO[j].eta()));

    me_pat_nConv_[type][0]->Fill(float(PATPHO[j].conversions().size()));
    if ( patscIsInBarrel ) {
      me_pat_scE_[type][1]->Fill(PATPHO[j].superCluster()->energy());
      me_pat_scEt_[type][1]->Fill(PATPHO[j].superCluster()->energy()/cosh(PATPHO[j].superCluster()->eta()));
      me_pat_r9_[type][1]->Fill(r9);
     }

    if ( patscIsInEndcap ) {
      me_pat_scE_[type][2]->Fill(PATPHO[j].superCluster()->energy());
      me_pat_scEt_[type][2]->Fill(PATPHO[j].superCluster()->energy()/cosh(PATPHO[j].superCluster()->eta()));
      me_pat_r9_[type][2]->Fill(r9);
     }

    if ( patphoIsInBarrel ) {
      me_pat_phoE_[type][1]->Fill(PATPHO[j].energy());
      me_pat_phoEt_[type][1]->Fill(PATPHO[j].energy()/cosh(PATPHO[j].eta()));
      me_pat_nConv_[type][1]->Fill(float(PATPHO[j].conversions().size()));
     }
    
    if ( patphoIsInEndcap ) {
      me_pat_phoE_[type][2]->Fill(PATPHO[j].energy());
     me_pat_phoEt_[type][2]->Fill(PATPHO[j].energy()/ cosh( (PATPHO[j]).eta()));
      me_pat_nConv_[type][2]->Fill(float(PATPHO[j].conversions().size()));
     }

////////////////// plot quantitied related to conversions
     reco::ConversionRefVector conversions = PATPHO[j].conversions();

     for (unsigned int iConv=0; iConv<conversions.size(); iConv++) {

 // two dimensional histogram
 //      me_pat_r9VsNofTracks_[type][0]->Fill( r9, conversions[iConv]->nTracks() ) ; 
  
 //      if ( phoIsInBarrel ) me_pat_r9VsNofTracks_[type][1]->Fill( r9,  conversions[iConv]->nTracks() ) ; 
  
 //      if ( phoIsInEndcap ) me_pat_r9VsNofTracks_[type][2]->Fill( r9,  conversions[iConv]->nTracks() ) ; 
 
       if ( conversions[iConv]->nTracks() <2 ) continue; 
  
  
       me_pat_convEta_[type]->Fill( conversions[iConv]-> caloCluster()[0]->eta() );
       me_pat_convPhi_[type]->Fill( conversions[iConv]-> caloCluster()[0]->phi() );
       me_pat_EoverPTracks_[type][0] ->Fill( conversions[iConv]->EoverP()) ;
       if ( patphoIsInBarrel ) me_pat_EoverPTracks_[type][1] ->Fill( conversions[iConv]->EoverP());
       if ( patphoIsInEndcap ) me_pat_EoverPTracks_[type][2] ->Fill( conversions[iConv]->EoverP());
 /* /// two dimensional histogram      
       if ( conversions[iConv]->conversionVertex().isValid()) 
         me_pat_convVtxRvsZ_[type] ->Fill ( fabs (conversions[iConv]->conversionVertex().position().z() ),  sqrt(conversions[iConv]->conversionVertex().position().perp2())  ) ;
*/         
  
        me_pat_zPVFromTracks_[type]->Fill ( conversions[iConv]->zOfPrimaryVertexFromTracks());
        std::vector<reco::TrackRef> tracks = conversions[iConv]->tracks();

         
        float px=0;
        float py=0;
        float pz=0;
        float e=0;
        for (unsigned int k=0; k<tracks.size(); k++) {
 //         two dimensional histo
 //         p_tk_nHitsVsEta_[type]->fill(  conversions[iConv]->caloCluster()[0]->eta(),   float(tracks[k]->recHitsSize() ) );
          me_pat_tkChi2_[type] ->Fill (tracks[k]->normalizedChi2()); 
          px+= tracks[k]->innerMomentum().x();
          py+= tracks[k]->innerMomentum().y();
          pz+= tracks[k]->innerMomentum().z();
          e +=  sqrt (  tracks[k]->innerMomentum().x()*tracks[k]->innerMomentum().x() +
                        tracks[k]->innerMomentum().y()*tracks[k]->innerMomentum().y() +
                        tracks[k]->innerMomentum().z()*tracks[k]->innerMomentum().z() +
                        +  mElec*mElec ) ;
        }
        float totP = sqrt(px*px +py*py + pz*pz);
        float invM=  (e + totP) * (e-totP) ;
  
        if ( invM> 0.) {
          invM= sqrt( invM);
        } else {
          invM=-1;
        }
        me_pat_invMass_[type][0] ->Fill(invM);
        if (patphoIsInBarrel) me_pat_invMass_[type][1] ->Fill(invM);
        if (patphoIsInEndcap) me_pat_invMass_[type][2] ->Fill(invM);
 
 
        float  patdPhiTracksAtVtx = -99;
        
        float patphiTk1= tracks[0]->innerMomentum().phi();
        float patphiTk2= tracks[1]->innerMomentum().phi();
        patdPhiTracksAtVtx = patphiTk1-patphiTk2;
        patdPhiTracksAtVtx = phiNormalization( patdPhiTracksAtVtx );
        me_pat_DPhiTracksAtVtx_[type][0]->Fill( patdPhiTracksAtVtx);
        if ( patphoIsInBarrel ) me_pat_DPhiTracksAtVtx_[type][1]->Fill( patdPhiTracksAtVtx);
        if ( patphoIsInEndcap ) me_pat_DPhiTracksAtVtx_[type][2]->Fill( patdPhiTracksAtVtx);
        me_pat_DCotTracks_[type][0] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
        if ( patphoIsInBarrel ) me_pat_DCotTracks_[type][1] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
        if ( patphoIsInEndcap ) me_pat_DCotTracks_[type][2] ->Fill ( conversions[iConv]->pairCotThetaSeparation());
                
        float  patdPhiTracksAtEcal=-99;
        float  patdEtaTracksAtEcal=-99;
        if (conversions[iConv]-> bcMatchingWithTracks()[0].isNonnull() && conversions[iConv]->bcMatchingWithTracks()[1].isNonnull() ) {
//        cout << "SUDHIRMALIK1" << endl;  
          
          float patPhi1 = conversions[iConv]->ecalImpactPosition()[0].phi();
          float patPhi2 = conversions[iConv]->ecalImpactPosition()[1].phi();
          float patEta1 = conversions[iConv]->ecalImpactPosition()[0].eta();
          float patEta2 = conversions[iConv]->ecalImpactPosition()[1].eta();
          float patbcPhi1 = conversions[iConv]->bcMatchingWithTracks()[0]->phi();
          float patbcPhi2 = conversions[iConv]->bcMatchingWithTracks()[1]->phi();
 //         float patbcEta1 = conversions[iConv]->bcMatchingWithTracks()[0]->eta();
 //         float patbcEta2 = conversions[iConv]->bcMatchingWithTracks()[1]->eta();
          patPhi1 = phiNormalization(patPhi1);
          patPhi2 = phiNormalization(patPhi2);
          patbcPhi1 = phiNormalization(patbcPhi1);
          patbcPhi2 = phiNormalization(patbcPhi2);
          patdPhiTracksAtEcal = patPhi1 -patPhi2;
          patdPhiTracksAtEcal = phiNormalization( patdPhiTracksAtEcal );
          patdEtaTracksAtEcal = patEta1 -patEta2;
          
          me_pat_DPhiTracksAtEcal_[type][0]->Fill(patdPhiTracksAtEcal);
          me_pat_DEtaTracksAtEcal_[type][0]->Fill(patdEtaTracksAtEcal);
          //        cout << "SUDHIRMALIK2" << endl;

          if ( patphoIsInBarrel ) {
            me_pat_DPhiTracksAtEcal_[type][1]->Fill( patdPhiTracksAtEcal);
            me_pat_DEtaTracksAtEcal_[type][1]->Fill( patdEtaTracksAtEcal);
          }
          //         cout << "SUDHIRMALIK3" << endl;

          if ( patphoIsInEndcap ) {
            me_pat_DPhiTracksAtEcal_[type][2]->Fill( patdPhiTracksAtEcal);
            me_pat_DEtaTracksAtEcal_[type][2]->Fill( patdEtaTracksAtEcal);
            //       cout << "SUDHIRMALIK4" << endl;

          }
      }
 
    } // loop over conversions
 
 
 //  }/// End loop over Reco  particles
 
   me_pat_nPho_[0][0]-> Fill(float(npatPho[0]));
   me_pat_nPho_[0][1]-> Fill(float(npatPhoBarrel[0]));
   me_pat_nPho_[0][2]-> Fill(float(npatPhoEndcap[0]));
   me_pat_nPho_[1][0]-> Fill(float(npatPho[1]));
   me_pat_nPho_[1][1]-> Fill(float(npatPhoBarrel[1]));
   me_pat_nPho_[1][2]-> Fill(float(npatPhoEndcap[1]));

      ///////////////////////////////////////////////////////////////////////////////




     }

//cout << "NUM OF PATPHOTONS = " << nPatPhotons << endl;
//cout << "NUM OF RECOPHOTONS = " << nRecoPhotons << endl;

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
PATValidation_Photon::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);


}

float PATValidation_Photon::phiNormalization(float & phi){
//---Definitions
 const float PI    = 3.1415927;
 const float TWOPI = 2.0*PI;


 if(phi >  PI) {phi = phi - TWOPI;}
 if(phi < -PI) {phi = phi + TWOPI;}

 return phi;

}



//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Photon);
