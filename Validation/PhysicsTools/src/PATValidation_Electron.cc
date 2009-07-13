// -*- C++ -*-
//
// Package:    PATValidation
// Class:      PATValidation
// 
/**\class PATValidation PATValidation_Electron.cc PATValidation/PATValidation/src/PATValidation_Electron.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sudhir_Malik
//         Created:  Fri Mar 13 09:52:17 CDT 2009
// $Id$
//
//


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

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"   

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "PATValidation_Electron.h"

#include "TH1.h"



using namespace reco;
using namespace std; 

int nRecoElectrons=0;
int nPatElectrons=0;


//
// constructors and destructor
//
PATValidation_Electron::PATValidation_Electron(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

recoElectron_= iConfig.getParameter<edm::InputTag>("recoElectron");
patElectron_ = iConfig.getParameter<edm::InputTag>("patElectron");

deltaR = iConfig.getParameter<double>("DeltaR");
etamin=iConfig.getParameter<double>("Etamin");
etamax=iConfig.getParameter<double>("Etamax");
phimin=iConfig.getParameter<double>("Phimin");
phimax=iConfig.getParameter<double>("Phimax");
ptmax=iConfig.getParameter<double>("Ptmax");
pmax=iConfig.getParameter<double>("Pmax");
eopmax=iConfig.getParameter<double>("Eopmax");
eopmaxsht=iConfig.getParameter<double>("Eopmaxsht");
detamin=iConfig.getParameter<double>("Detamin");
detamax=iConfig.getParameter<double>("Detamax");
dphimin=iConfig.getParameter<double>("Dphimin");
dphimax=iConfig.getParameter<double>("Dphimax");
detamatchmin=iConfig.getParameter<double>("Detamatchmin");
detamatchmax=iConfig.getParameter<double>("Detamatchmax");
dphimatchmin=iConfig.getParameter<double>("Dphimatchmin");
dphimatchmax=iConfig.getParameter<double>("Dphimatchmax");
fhitsmax=iConfig.getParameter<double>("Fhitsmax");
lhitsmax=iConfig.getParameter<double>("Lhitsmax");
nbineta=iConfig.getParameter<int>("Nbineta");
nbineta2D=iConfig.getParameter<int>("Nbineta2D");
nbinp=iConfig.getParameter<int>("Nbinp");
nbinpt=iConfig.getParameter<int>("Nbinpt");
nbinp2D=iConfig.getParameter<int>("Nbinp2D");
nbinpt2D=iConfig.getParameter<int>("Nbinpt2D");
nbinpteff=iConfig.getParameter<int>("Nbinpteff");
nbinphi=iConfig.getParameter<int>("Nbinphi");
nbinphi2D=iConfig.getParameter<int>("Nbinphi2D");
nbineop=iConfig.getParameter<int>("Nbineop");
nbineop2D=iConfig.getParameter<int>("Nbineop2D");
nbinfhits=iConfig.getParameter<int>("Nbinfhits");
nbinlhits=iConfig.getParameter<int>("Nbinlhits");
nbinxyz=iConfig.getParameter<int>("Nbinxyz");
nbindeta=iConfig.getParameter<int>("Nbindeta");
nbindphi=iConfig.getParameter<int>("Nbindphi");
nbindetamatch=iConfig.getParameter<int>("Nbindetamatch");
nbindphimatch=iConfig.getParameter<int>("Nbindphimatch");
nbindetamatch2D=iConfig.getParameter<int>("Nbindetamatch2D");
nbindphimatch2D=iConfig.getParameter<int>("Nbindphimatch2D");
outputFile_ = iConfig.getUntrackedParameter<std::string>("OutputFile");


 if (outputFile_.size() > 0)
    edm::LogInfo("OutputInfo") << " ParticleFLow Task histograms will be saved to '" << outputFile_.c_str()<< "'";
    else edm::LogInfo("OutputInfo") << " ParticleFlow Task histograms will NOT be saved";
     
}

PATValidation_Electron::~PATValidation_Electron()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//


// ------------ method called once each job just before starting event loop  ------------
void
PATValidation_Electron::beginJob(const edm::EventSetup&)
{
                   
dbe = edm::Service<DQMStore>().operator->();
if(dbe){

      string path = "Benchmarks/";

      dbe->setCurrentFolder(path.c_str());
        
      dbe->setCurrentFolder("Benchmarks/electrons/RECO");

      me["RecoElectron_pt"] = dbe->book1D("pt", "RecoElectron", 50, 0, 200);
      me["RecoElectron_eta"]  = dbe->book1D("eta", "RecoElectron", 50, -6, 6);
      me["RecoElectron_phi"]  = dbe->book1D("phi", "RecoElectron", 50, -4, 4);

   ///RECO invariant dielec mass
     me["Recodielecmass"]=dbe->book1D("DiElectronMass","all ele pairs invariant mass", 100, 0., 150. );


   // RECO electron iso
      me["RecoTrackIso"] = dbe->book1D("TrackIso","Electron Track Isolation", 100, 0, 10);         
      me["RecoCaloIso"] = dbe->book1D("CaloIso","Electron Calo Isolation", 100, 0, 10);

 
   // RECO matching object distributions

      me["Recoele_matchingObjectEta"]    = dbe->book1D("ele_matchingObjectEta", "ele_matchingObjectEta", nbineta,etamin,etamax);
      me["Recoele_matchingObjectPt"]     = dbe->book1D("ele_matchingObjectPt","ele_matchingObjectPt",  nbinpteff,5.,ptmax); 
      me["Recoele_matchingObjectPhi"]    = dbe->book1D("ele_matchingObjectPhi", "ele_matchingObjectPhi", nbinphi,phimin,phimax);
      me["Recoele_matchingObjectZ"]      = dbe->book1D("ele_matchingObjectZ", "ele_matchingObjectZ",    50, -25, 25 );

   // RECO matched electrons basic quantities and eff

      me["Recoele_charge"]         = dbe->book1D("ele_charge",         "ele charge",             5,-2.,2.);
      me["Recoele_vertexP"]        = dbe->book1D("ele_vertexP",        "ele p at vertex",       nbinp,0.,pmax);
      me["Recoele_vertexPt"]       = dbe->book1D("ele_vertexPt",       "ele p_{T} at vertex",  nbinpt,0.,ptmax);
      me["Recoele_vertexEta"]      = dbe->book1D("ele_vertexEta",      "ele #eta at vertex",    nbineta,etamin,etamax);
      me["Recoele_vertexPhi"]      = dbe->book1D("ele_vertexPhi",      "ele #phi at vertex",    nbinphi,phimin,phimax);
      me["Recoele_vertexX"]        = dbe->book1D("ele_vertexX",      "ele x at vertex",    nbinxyz,-0.1,0.1 );
      me["Recoele_vertexY"]        = dbe->book1D("ele_vertexY",      "ele y at vertex",    nbinxyz,-0.1,0.1 );
      me["Recoele_vertexZ"]        = dbe->book1D("ele_vertexZ",      "ele z at vertex",    nbinxyz,-25, 25 );
//      me["Recoele_matchingObjectPt_matched"]   = dbe->book1D("ele_matchingObjectPt_matched",       "matching SC p_{T}",  nbinpteff,5.,ptmax);
//      me["Recoele_ptEff"]                      = dbe->book1D( "h_ele_ptEff",       "fraction of reco ele matching a reco SC",  nbinpteff,5.,ptmax);
//      me["Recoele_matchingObjectEta_matched"]  = dbe->book1D("ele_matchingObjectEta_matched",      "matching SC #eta",    nbineta,etamin,etamax);
//      me["Recoele_etaEff"]                     = dbe->book1D("ele_etaEff","fraction of reco ele matching a reco SC",    nbineta,etamin,etamax);
//      me["Recoele_matchingObjectPhi_matched"]  = dbe->book1D("ele_matchingObjectPhi_matched", "matching SC phi",        nbinphi,phimin,phimax);
//      me["Recoele_phiEff"]                     = dbe->book1D( "h_ele_phiEff", "fraction of reco ele matching a reco SC",  nbinphi,phimin,phimax);
//      me["Recoele_matchingObjectZ_matched"]    = dbe->book1D( "ele_matchingObjectZ_matched",      "matching SC z",    nbinxyz,-25,25);
//      me["Recoele_zEff"]                       = dbe->book1D("ele_zEff",      "fraction of reco ele matching a reco SC",    nbinxyz,-25,25);
      

   //RECO electron matching and ID
   
      me["Recoele_EoverP_all"]  = dbe->book1D("ele_EoverP_all","all reco ele, E/p at vertex",  nbineop,0.,eopmax);
      me["Recoele_EoPout"]         = dbe->book1D("ele_EoPout",         "ele E_{seed}/P_{out}",           nbineop,0.,eopmax);
      me["Recoele_dEtaSc_propVtx"] = dbe->book1D("ele_dEtaSc_propVtx", "ele #eta_{sc} - #eta_{tr},prop from vertex",      nbindetamatch,detamatchmin,detamatchmax);
      me["Recoele_dPhiSc_propVtx"] = dbe->book1D("ele_dPhiSc_propVtx", "ele #phi_{sc} - #phi_{tr},prop from vertex",      nbindphimatch,dphimatchmin,dphimatchmax);
      me["Recoele_dPhiCl_propOut"] = dbe->book1D("ele_dPhiCl_propOut", "ele #phi_{cl} - #phi_{tr},prop from outermost",   nbindphimatch,dphimatchmin,dphimatchmax);
      me["Recoele_HoE"]            = dbe->book1D("ele_HoE", "ele H/E", 55,-0.05,0.5);
      me["Recoele_classes"]        = dbe->book1D("ele_classes", "ele electron classes",      150,0.0,150.);
      me["Recoele_eta"]            = dbe->book1D("ele_eta", "ele electron eta",  nbineta/2,0.0,etamax);
      me["Recoele_eta_golden"]     = dbe->book1D("ele_eta_golden", "ele electron eta golden",  nbineta/2,0.0,etamax);
//      me["Recoele_eta_goldenFrac"] = dbe->book1D("ele_eta_goldenFrac", "fraction of golden electrons",  nbineta/2,0.0,etamax);  
      me["Recoele_eta_shower"]     = dbe->book1D("ele_eta_show", "ele electron eta showering",  nbineta/2,0.0,etamax);
//      me["Recoele_eta_showerFrac"] = dbe->book1D("ele_eta_showerFrac", "fraction of showering electrons",  nbineta/2,0.0,etamax);
//      me["Recoele_zEff"]           = dbe->book1D("ele_zEff",   "matching SC z",    nbinxyz,-25,25);


    // RECO electron tracks

      me["Recoele_foundHits"]      = dbe->book1D("ele_foundHits",      "ele track # found hits",      nbinfhits,0.,fhitsmax);
      me["Recoele_chi2"]           = dbe->book1D("ele_chi2",           "ele track #chi^{2}/ndf",         100,0.,15.);
      me["Recoele_PinMnPout_mode"] = dbe->book1D("ele_PinMnPout_mode",      "ele track inner p - outer p, mode"   ,nbinp,0.,100.);


     
      dbe->setCurrentFolder("Benchmarks/electrons/PAT");
 
      me["PatElectron_pt"]  = dbe->book1D("pt", "PatElectron", 50, 0, 200);
      me["PatElectron_eta"]  = dbe->book1D("eta", "PatElectron", 50, -6, 6);
      me["PatElectron_phi"]  = dbe->book1D("phi", "PatElectron", 50, -4, 4);

 //PAT invariant dielec mass
     me["Patdielecmass"]=dbe->book1D("DiElectronMass","all ele pairs invariant mass", 100, 0., 150. );

 // PAT electron iso
      me["PatTrackIso"] = dbe->book1D("TrackIso","Electron Track Isolation", 100, 0, 10);
      me["PatCaloIso"] = dbe->book1D("CaloIso","Electron Calo Isolation", 100, 0, 10);

      // PAT matching object distributions 

      me["Patele_matchingObjectEta"]    = dbe->book1D("ele_matchingObjectEta", "ele_matchingObjectEta", nbineta,etamin,etamax); 
      me["Patele_matchingObjectPt"]     = dbe->book1D("ele_matchingObjectPt","ele_matchingObjectPt",  nbinpteff,5.,ptmax); 
      me["Patele_matchingObjectPhi"]    = dbe->book1D("ele_matchingObjectPhi", "ele_matchingObjectPhi", nbinphi,phimin,phimax); 
      me["Patele_matchingObjectZ"]      = dbe->book1D("ele_matchingObjectZ", "ele_matchingObjectZ",    50, -25, 25 ); 

      // PAT matched electrons basic quantities and eff
      me["Patele_charge"]         = dbe->book1D("ele_charge",         "ele charge",             5,-2.,2.);   
      me["Patele_vertexP"]        = dbe->book1D("ele_vertexP",        "ele p at vertex",       nbinp,0.,pmax);
      me["Patele_vertexPt"]       = dbe->book1D("ele_vertexPt",       "ele p_{T} at vertex",  nbinpt,0.,ptmax);
      me["Patele_vertexEta"]      = dbe->book1D("ele_vertexEta",      "ele #eta at vertex",    nbineta,etamin,etamax);
      me["Patele_vertexPhi"]      = dbe->book1D("ele_vertexPhi",      "ele #phi at vertex",    nbinphi,phimin,phimax);
      me["Patele_vertexX"]        = dbe->book1D("ele_vertexX",      "ele x at vertex",    nbinxyz,-0.1,0.1 );
      me["Patele_vertexY"]        = dbe->book1D("ele_vertexY",      "ele y at vertex",    nbinxyz,-0.1,0.1 );
      me["Patele_vertexZ"]        = dbe->book1D("ele_vertexZ",      "ele z at vertex",    nbinxyz,-25, 25 );
//      me["Patele_matchingObjectPt_matched"]   = dbe->book1D("ele_matchingObjectPt_matched",       "matching SC p_{T}",  nbinpteff,5.,ptmax);
//      me["Patele_ptEff"]                      = dbe->book1D( "h_ele_ptEff",       "fraction of reco ele matching a reco SC",  nbinpteff,5.,ptmax);
//      me["Patele_matchingObjectEta_matched"]  = dbe->book1D("ele_matchingObjectEta_matched",      "matching SC #eta",    nbineta,etamin,etamax);
//      me["Patele_etaEff"]                     = dbe->book1D("ele_etaEff","fraction of reco ele matching a reco SC",    nbineta,etamin,etamax);
//      me["Patele_matchingObjectPhi_matched"]  = dbe->book1D("ele_matchingObjectPhi_matched", "matching SC phi",        nbinphi,phimin,phimax); 
//      me["Patele_phiEff"]                     = dbe->book1D( "h_ele_phiEff", "fraction of reco ele matching a reco SC",  nbinphi,phimin,phimax);
//      me["Patele_matchingObjectZ_matched"]    = dbe->book1D( "ele_matchingObjectZ_matched",      "matching SC z",    nbinxyz,-25,25);
//      me["Patele_zEff"]                       = dbe->book1D("ele_zEff",      "fraction of reco ele matching a reco SC",    nbinxyz,-25,25);


      //PAT electron matching and ID 

      me["Patele_EoverP_all"]  = dbe->book1D("ele_EoverP_all","all reco ele, E/p at vertex",  nbineop,0.,eopmax);
      me["Patele_EoPout"]         = dbe->book1D("ele_EoPout",         "ele E_{seed}/P_{out}",           nbineop,0.,eopmax);
      me["Patele_dEtaSc_propVtx"] = dbe->book1D("ele_dEtaSc_propVtx", "ele #eta_{sc} - #eta_{tr},prop from vertex",      nbindetamatch,detamatchmin,detamatchmax);
      me["Patele_dPhiSc_propVtx"] = dbe->book1D("ele_dPhiSc_propVtx", "ele #phi_{sc} - #phi_{tr},prop from vertex",      nbindphimatch,dphimatchmin,dphimatchmax);
      me["Patele_dPhiCl_propOut"] = dbe->book1D("ele_dPhiCl_propOut", "ele #phi_{cl} - #phi_{tr},prop from outermost",   nbindphimatch,dphimatchmin,dphimatchmax);
      me["Patele_HoE"]            = dbe->book1D("ele_HoE", "ele H/E", 55,-0.05,0.5);
      me["Patele_classes"]        = dbe->book1D("ele_classes", "ele electron classes",      150,0.0,150.);
      me["Patele_eta"]            = dbe->book1D("ele_eta", "ele electron eta",  nbineta/2,0.0,etamax);
      me["Patele_eta_golden"]     = dbe->book1D("ele_eta_golden", "ele electron eta golden",  nbineta/2,0.0,etamax);
//      me["Patele_eta_goldenFrac"] = dbe->book1D("ele_eta_goldenFrac", "fraction of golden electrons",  nbineta/2,0.0,etamax);
      me["Patele_eta_shower"]     = dbe->book1D("ele_eta_show", "ele electron eta showering",  nbineta/2,0.0,etamax);
//      me["Patele_eta_showerFrac"] = dbe->book1D("ele_eta_showerFrac", "fraction of showering electrons",  nbineta/2,0.0,etamax);
//      me["Patele_zEff"]           = dbe->book1D("ele_zEff",   "matching SC z",    nbinxyz,-25,25); 

      // PAT electron tracks

      me["Patele_foundHits"]      = dbe->book1D("ele_foundHits",      "ele track # found hits",      nbinfhits,0.,fhitsmax);
      me["Patele_chi2"]           = dbe->book1D("ele_chi2",           "ele track #chi^{2}/ndf",         100,0.,15.);   
      me["Patele_PinMnPout_mode"] = dbe->book1D("ele_PinMnPout_mode",      "ele track inner p - outer p, mode"   ,nbinp,0.,100.);




  }
  return;
    
  
}


// ------------ method called to for each event  ------------
void
PATValidation_Electron::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
 //   using namespace edm;
   
   // Typedefs to use views

//====================================RECO ELECTRON=====================================
//    typedef edm::View<reco::Candidate> candidateCollection ;

     //get reco electron collection
//     edm::Handle<candidateCollection> recoele_hnd;
//     iEvent.getByLabel(recoElectron_, recoele_hnd);
//     const candidateCollection & RECOELE =*recoele_hnd;

     edm::Handle<std::vector<reco::GsfElectron> > recoele_hnd;
     iEvent.getByLabel(recoElectron_, recoele_hnd);
     std::vector<reco::GsfElectron> const  & RECOELE =*recoele_hnd;


    // Loop over Reco Electrons
     for (int i = 0; i != RECOELE.size(); ++i) {
      me["RecoElectron_pt"]->Fill(RECOELE[i].pt());
//      nRecoElectrons++;
//      cout << "RecoElectron pt " << RECOELE[i].pt() << endl;
      me["RecoElectron_eta"]->Fill(RECOELE[i].eta());
      me["RecoElectron_phi"]->Fill(RECOELE[i].phi());


      for (int j = i+1; j != RECOELE.size(); ++j) {
      math::XYZTLorentzVector p12 = (RECOELE[i]).p4()+(RECOELE[j]).p4();
      float mee2 = p12.Dot(p12);
      me["Recodielecmass"]->Fill(sqrt(mee2));
      }

   // RECO electron iso
     // me["RecoTrackIso"] ->Fill(RECOELE[i].trackIso()); 
     // me["RecoCaloIso"]  ->Fill(RECOELE[i].caloIso()); 



   // RECO matching object distributions
   
//      me["Recoele_matchingObjectEta"]    ->Fill
//      me["Recoele_matchingObjectPt"]     ->Fill
//      me["Recoele_matchingObjectPhi"]    ->Fill
//      me["Recoele_matchingObjectZ"]      ->Fill

   // RECO matched electrons basic quantities and eff
      
      me["Recoele_charge"]       -> Fill(RECOELE[i].charge() );
      me["Recoele_vertexP"]      -> Fill(RECOELE[i].p());  
      me["Recoele_vertexPt"]     -> Fill(RECOELE[i].pt());   
      me["Recoele_vertexEta"]    -> Fill(RECOELE[i].eta());
      me["Recoele_vertexPhi"]    -> Fill(RECOELE[i].phi() );
      me["Recoele_vertexX"]      -> Fill(RECOELE[i].vertex().x()); 
      me["Recoele_vertexY"]      -> Fill(RECOELE[i].vertex().y());  
      me["Recoele_vertexZ"]      -> Fill(RECOELE[i].vertex().z());
//      me["Recoele_matchingObjectPt_matched"]  -> Fill(RECOELE[i]);   
//      me["Recoele_ptEff"]                     -> Fill(RECOELE[i]);  
//      me["Recoele_matchingObjectEta_matched"] -> Fill(RECOELE[i]); 
//      me["Recoele_etaEff"]                    -> Fill(RECOELE[i]); 
//      me["Recoele_matchingObjectPhi_matched"] -> Fill(RECOELE[i]); 
//      me["Recoele_phiEff"]                    -> Fill(RECOELE[i]); 
//      me["Recoele_matchingObjectZ_matched"]   -> Fill(RECOELE[i]); 
//      me["Recoele_zEff"]                      -> Fill(RECOELE[i]); 

     
    //RECO electron matching and ID

      me["Recoele_EoverP_all"]     ->Fill(RECOELE[i].eSuperClusterOverP());
      me["Recoele_EoPout"]         ->Fill(RECOELE[i].eSeedClusterOverPout());
      me["Recoele_dEtaSc_propVtx"] ->Fill(RECOELE[i].deltaEtaSuperClusterTrackAtVtx());
      me["Recoele_dPhiSc_propVtx"] ->Fill(RECOELE[i].deltaPhiSuperClusterTrackAtVtx());
      me["Recoele_dPhiCl_propOut"] ->Fill(RECOELE[i].deltaPhiSeedClusterTrackAtCalo());
      me["Recoele_HoE"]            ->Fill(RECOELE[i].hadronicOverEm());

      //classes
	int eleClass = RECOELE[i].classification();
	me["Recoele_classes"] ->Fill(eleClass);	

	eleClass = eleClass%100; // get rid of barrel/endcap distinction
        me["Recoele_eta"] ->Fill(fabs(RECOELE[i].eta()));
        if (eleClass == 0) {
	  me["Recoele_eta_golden"] ->Fill(fabs(RECOELE[i].eta()));
	}
        if (eleClass == 30 || eleClass == 31 || eleClass == 32  || eleClass == 33 || eleClass == 34 ) {
	   me["Recoele_eta_shower"] ->Fill(RECOELE[i].eta());
	}
 
//      me["Recoele_eta_goldenFrac"] ->Fill(RECOELE[i]);
//      me["Recoele_eta_showerFrac"] ->Fill(RECOELE[i]);
//      me["Recoele_zEff"]           ->Fill(RECOELE[i]);


    // RECO electron tracks

       me["Recoele_foundHits"]      -> Fill(RECOELE[i].gsfTrack()->numberOfValidHits());
       me["Recoele_chi2"]           -> Fill(RECOELE[i].gsfTrack()->normalizedChi2());
       me["Recoele_PinMnPout_mode"] -> Fill(RECOELE[i].trackMomentumAtVtx().R() - RECOELE[i].trackMomentumOut().R()); 
     
     


//      cout << "reco electron pt is = " << RECOELE[i].pt() << endl; 
       }

//====================================PAT ELECTRON=====================================

    //get pat electron collection
     edm::Handle <std::vector<pat::Electron> >  patele_hnd;
     iEvent.getByLabel(patElectron_, patele_hnd);
     std::vector<pat::Electron> const & PATELE = *patele_hnd;
   // Loop over Pat Electrons
     for (int i = 0; i != PATELE.size(); ++i) {
	me["PatElectron_pt"]->Fill(PATELE[i].pt());
//        nPatElectrons++;
//        cout << "PatElectron pt " << PATELE[i].pt() << endl;
	me["PatElectron_eta"]->Fill(PATELE[i].eta());
	me["PatElectron_phi"]->Fill(PATELE[i].phi()); 


 for (int j = i+1; j != PATELE.size(); ++j) {
      math::XYZTLorentzVector p12 = (PATELE[i]).p4()+(PATELE[j]).p4();
      float mee2 = p12.Dot(p12);
      me["Patdielecmass"]->Fill(sqrt(mee2));
      }


  // PAT electron iso
      me["PatTrackIso"] ->Fill(PATELE[i].trackIso());
      me["PatCaloIso"]  ->Fill(PATELE[i].caloIso());



      
   // PAT matching object distributions
      
//      me["Patele_matchingObjectEta"]    ->Fill
//      me["Patele_matchingObjectPt"]     ->Fill
//      me["Patele_matchingObjectPhi"]    ->Fill
//      me["Patele_matchingObjectZ"]      ->Fill

   // PAT matched electrons basic quantities and eff

      me["Patele_charge"]       -> Fill(PATELE[i].charge() );
      me["Patele_vertexP"]      -> Fill(PATELE[i].p());
      me["Patele_vertexPt"]     -> Fill(PATELE[i].pt());
      me["Patele_vertexEta"]    -> Fill(PATELE[i].eta());
      me["Patele_vertexPhi"]    -> Fill(PATELE[i].phi() );
      me["Patele_vertexX"]      -> Fill(PATELE[i].vertex().x());
      me["Patele_vertexY"]      -> Fill(PATELE[i].vertex().y());
      me["Patele_vertexZ"]      -> Fill(PATELE[i].vertex().z());
//      me["Patele_matchingObjectPt_matched"]  -> Fill(PATELE[i]);
//      me["Patele_ptEff"]                     -> Fill(PATELE[i]);
//      me["Patele_matchingObjectEta_matched"] -> Fill(PATELE[i]);
//      me["Patele_etaEff"]                    -> Fill(PATELE[i]);
//      me["Patele_matchingObjectPhi_matched"] -> Fill(PATELE[i]);
//      me["Patele_phiEff"]                    -> Fill(PATELE[i]);
//      me["Patele_matchingObjectZ_matched"]   -> Fill(PATELE[i]);
//      me["Patele_zEff"]                      -> Fill(PATELE[i]);

  //PAT electron matching and ID

      me["Patele_EoverP_all"]     ->Fill(PATELE[i].eSuperClusterOverP());
      me["Patele_EoPout"]         ->Fill(PATELE[i].eSeedClusterOverPout());
      me["Patele_dEtaSc_propVtx"] ->Fill(PATELE[i].deltaEtaSuperClusterTrackAtVtx());
      me["Patele_dPhiSc_propVtx"] ->Fill(PATELE[i].deltaPhiSuperClusterTrackAtVtx());
      me["Patele_dPhiCl_propOut"] ->Fill(PATELE[i].deltaPhiSeedClusterTrackAtCalo());
      me["Patele_HoE"]            ->Fill(PATELE[i].hadronicOverEm());
  
      //classes
        int eleClass = PATELE[i].classification();
        me["Patele_classes"] ->Fill(eleClass);

        eleClass = eleClass%100; // get rid of barrel/endcap distinction
        me["Patele_eta"] ->Fill(fabs(PATELE[i].eta()));
        if (eleClass == 0) {
          me["Patele_eta_golden"] ->Fill(fabs(PATELE[i].eta()));
        }
        if (eleClass == 30 || eleClass == 31 || eleClass == 32  || eleClass == 33 || eleClass == 34 ) {
           me["Patele_eta_shower"] ->Fill(PATELE[i].eta());
        }

//      me["Patele_eta_goldenFrac"] ->Fill(PATELE[i]);
//      me["Patele_eta_showerFrac"] ->Fill(PATELE[i]);
//      me["Patele_zEff"]           ->Fill(PATELE[i]);




  // PAT electron tracks
      
      me["Patele_foundHits"]      -> Fill(PATELE[i].gsfTrack()->numberOfValidHits());
      me["Patele_chi2"]           -> Fill(PATELE[i].gsfTrack()->normalizedChi2());
      me["Patele_PinMnPout_mode"] -> Fill(PATELE[i].trackMomentumAtVtx().R() - PATELE[i].trackMomentumOut().R());
      
     }


//cout << "NUM OF PATELECTRONS = " << nPatElectrons << endl;
//cout << "NUM OF RECOELECTRONS = " << nRecoElectrons << endl;
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
PATValidation_Electron::endJob() {

// Store the DAQ Histograms
  if (outputFile_.size() != 0)
    dbe->save(outputFile_);



}

//define this as a plug-in
DEFINE_FWK_MODULE(PATValidation_Electron);
