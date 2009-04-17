// -*- C++ -*-
//
// Package:    HydQuarkoniaAnalyzer
// Class:      HydQuarkoniaAnalyzer
// 
/**\class HydQuarkoniaAnalyzer HydQuarkoniaAnalyzer.cc UserCode/DMoonAna/src/HydQuarkoniaAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dong Ho Moon
//         Created:  Mon Oct  6 14:12:25 CEST 2008
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


#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include <vector>
#include <iostream>


#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"


#include "FWCore/ParameterSet/interface/InputTag.h"
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"


//
// class decleration
//

#include <TH1.h>
#include <TFile.h>
#include <TString.h>
#include <stdio.h>
#include <TH2F.h>

// ************* NTuple ***************************************************
#include <TNtuple.h>
// ************************************************************************

#include <TLorentzVector.h>


class HydQuarkoniaAnalyzer : public edm::EDAnalyzer {
public:
  explicit HydQuarkoniaAnalyzer(const edm::ParameterSet&);
  ~HydQuarkoniaAnalyzer();
  
  
private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;


  std::string fOutputFile_;  
  
  TNtuple *nGenMuon, *nGenJpsi, *nGenUpsilon, *nGenZ0;
  TNtuple *nGenPriJpsi, *nGenPriUpsi, *nGenPriZ0;
  TNtuple *nGenSecJpsi, *nGenSecUpsi, *nGenSecZ0; 
  TNtuple *nGenSec9900Jpsi, *nGenJpsi9900Muon, *nGenJpsi9900MM, *nGenJpsi9900Mom, *nGenJpsi9900Dau;
  TNtuple *nGenJpsiMuon, *nGenUpsiMuon, *nGenZ0Muon;
  TNtuple *nGenJpsiMM, *nGenUpsiMM, *nGenZ0MM;
  TNtuple *nGenJpsiToMM, *nGenUpsiToMM, *nGenZ0ToMM;
  TNtuple *nGenMuonMom, *nGenJpsiMom, *nGenUpsiMom, *nGenZ0Mom; 
  TNtuple *nGenJpsiDau, *nGenUpsiDau, *nGenZ0Dau; //, *nParInfo; TNtuple *nGenJpsiToMM, *nGenUpsiToMM, *nGenZ0ToMM;

  TNtuple *nGenPriJpsiDau, *nGenPriJpsiMuon, *nGenPriJpsiMM;
  TNtuple *nGenPriUpsiDau, *nGenPriUpsiMuon, *nGenPriUpsiMM;
  TNtuple *nGenSecJpsiDau, *nGenSecJpsiMuon, *nGenSecJpsiMM;
  TNtuple *nGenSecUpsiDau, *nGenSecUpsiMuon, *nGenSecUpsiMM;
  
  TFile *hOutputFile;
  
  
  // ----------member data ---------------------------
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HydQuarkoniaAnalyzer::HydQuarkoniaAnalyzer(const edm::ParameterSet& iConfig):
fOutputFile_(iConfig.getUntrackedParameter<std::string>("hOutputFile"))
{
   //now do what ever initialization is needed

}


HydQuarkoniaAnalyzer::~HydQuarkoniaAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// ------------ method called once each job just before starting event loop  ------------
void 
HydQuarkoniaAnalyzer::beginJob(const edm::EventSetup&)
{
  hOutputFile   = new TFile( fOutputFile_.c_str(), "RECREATE" ) ;
  //nParInfo = new TNtuple("nParInfo","Gen Particle info","id:p_pt:p_eta:p_phi:p_mass");
  nGenMuon = new TNtuple("nGenMuon","Gen Muon","Mu_pt:Mu_eta:Mu_phi:Mu_mass");
  nGenJpsi = new TNtuple("nGenJpsi","Gen Jpsi","Jp_pt:Jp_eta:Jp_phi:Jp_mass:Jp_st");
  nGenPriJpsi = new TNtuple("nGenPriJpsi","Gen pri Jpsi","Jp_pt:Jp_eta:Jp_phi:Jp_mass:Jp_st");
  nGenSecJpsi = new TNtuple("nGenSecJpsi","Gen sec Jpsi","Jp_pt:Jp_eta:Jp_phi:Jp_mass:m_id:m_st:Jp_st");
  nGenJpsiMuon = new TNtuple("nGenJpsiMuon","Gen JpsiMu","JpMu_pt:JpMu_eta:JpMu_phi:JpMu_mass");
  nGenUpsilon = new TNtuple("nGenUpsilon","Gen Upsi","Up_pt:Up_eta:Up_phi:Up_mass:Up_st");
  nGenPriUpsi = new TNtuple("nGenPriUpsi","Gen pri Upsi","Up_pt:Up_eta:Up_phi:Up_mass:Up_st");
  nGenSecUpsi = new TNtuple("nGenSecUpsi","Gen sec Upsi","Up_pt:Up_eta:Up_phi:Up_mass:m_id:m_st:Up_st");
  nGenUpsiMuon = new TNtuple("nGenUpsiMuon","Gen UpsiMu","UpMu_pt:UpMu_eta:UpMu_phi:UpMu_mass");
  nGenZ0 = new TNtuple("nGenZ0","Gen Z0","Z0_pt:Z0_eta:Z0_phi:Z0_mass:Z0_st");
  nGenPriZ0 = new TNtuple("nGenPriZ0","Gen pri Z0","Z0_pt:Z0_eta:Z0_phi:Z0_mass:Z0_st");
  nGenSecZ0 = new TNtuple("nGenSecZ0","Gen sec Z0","Z0_pt:Z0_eta:Z0_phi:Z0_mass:m_id:m_st:Z0_st");
  nGenZ0Muon = new TNtuple("nGenZ0Muon","Gen Z0 Mu","ZMu_pt:ZMu_eta:ZMu_phi:ZMu_mass");
 
  nGenJpsiMM = new TNtuple("nGenJpsiMM","Gen Jpsi to mumu","Jp_pt:Jp_eta:Jp_phi:Jp_mass");
//  nGenJpsi9900MM = new TNtuple("nGenJpsi9900MM","Gen Jpsi to mumu","Jp_pt:Jp_eta:Jp_phi:Jp_mass");
  nGenJpsiToMM = new TNtuple("nGenJpsiToMM","Gen Jpsi to mumu","Jp_id:Jp_pt:Jp_eta:Jp_phi:Jp_mass");
  nGenUpsiMM = new TNtuple("nGenUpsiMM","Gen Upsilon to mumu","Up_pt:Up_eta:Up_phi:Up_mass");
  nGenUpsiToMM = new TNtuple("nGenUpsiToMM","Gen Upsilon to mumu","Up_id:Up_pt:Up_eta:Up_phi:Up_mass");
  nGenZ0ToMM = new TNtuple("nGenZ0ToMM","Gen Z_0 to mumu","z_id:z_pt:z_eta:z_phi:z_mass");
  nGenZ0MM = new TNtuple("nGenZ0MM","Gen Z_0 to mumu","z_pt:z_eta:z_phi:z_mass");
  nGenMuonMom = new TNtuple("nGenMuonMom","muon's mother","m_id:m_pt:m_eta:m_phi:m_mass");
  nGenJpsiMom = new TNtuple("nGenJpsiMom","Jpsi's mother","m_id:m_pt:m_eta:m_phi:m_mass:m_st");
//  nGenJpsi9900Mom = new TNtuple("nGenJpsi9900Mom","Jpsi's mother","m_id:m_pt:m_eta:m_phi:m_mass");
  nGenUpsiMom = new TNtuple("nGenUpsiMom","Upsi's mother","m_id:m_pt:m_eta:m_phi:m_mass:m_st");
  nGenZ0Mom = new TNtuple("nGenZ0Mom","Z0's mother","m_id:m_pt:m_eta:m_phi:m_mass:m_st");
  nGenJpsiDau = new TNtuple("nGenJpsiDau","Jpsi's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
//  nGenJpsi9900Dau = new TNtuple("nGenJpsi9900Dau","Jpsi's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenUpsiDau = new TNtuple("nGenUpsiDau","Upsilon's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenZ0Dau = new TNtuple("nGenZ0Dau","Z0's daughter","d_id:d_pt:d_eta:d_phi:d_mass");

  nGenPriJpsiDau = new TNtuple("nGenPriJpsiDau","Pri Jpsi's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenPriUpsiDau = new TNtuple("nGenPriUpsiDau","Pri Upsilon's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenPriJpsiMuon = new TNtuple("nGenPriJpsiMuon","Gen pri JpsiMu","JpMu_pt:JpMu_eta:JpMu_phi:JpMu_mass");
  nGenPriUpsiMuon = new TNtuple("nGenPriUpsiMuon","Gen pri UpsiMu","UpMu_pt:UpMu_eta:UpMu_phi:UpMu_mass");
  nGenPriJpsiMM = new TNtuple("nGenPriJpsiMM","Gen pri Jpsi to mumu","Jp_pt:Jp_eta:Jp_phi:Jp_mass");
  nGenPriUpsiMM = new TNtuple("nGenPriUpsiMM","Gen pri Upsilon to mumu","Up_pt:Up_eta:Up_phi:Up_mass");
  
  nGenSecJpsiDau = new TNtuple("nGenSecJpsiDau","Sec Jpsi's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenSecUpsiDau = new TNtuple("nGenSecUpsiDau","Sec Upsilon's daughter","d_id:d_pt:d_eta:d_phi:d_mass");
  nGenSecJpsiMuon = new TNtuple("nGenSecJpsiMuon","Gen sec JpsiMu","JpMu_pt:JpMu_eta:JpMu_phi:JpMu_mass");
  nGenSecUpsiMuon = new TNtuple("nGenSecUpsiMuon","Gen sec UpsiMu","UpMu_pt:UpMu_eta:UpMu_phi:UpMu_mass");
  nGenSecJpsiMM = new TNtuple("nGenSecJpsiMM","Gen sec Jpsi to mumu","Jp_pt:Jp_eta:Jp_phi:Jp_mass");
  nGenSecUpsiMM = new TNtuple("nGenSecUpsiMM","Gen sec Upsilon to mumu","Up_pt:Up_eta:Up_phi:Up_mass");
}

//
// member functions
//

// ------------ method called to for each event  ------------
void
HydQuarkoniaAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;
   using namespace std;

#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif

   Handle<GenParticleCollection> genParticles;
   iEvent.getByLabel("genParticles", genParticles);
   for(size_t i = 0; i < genParticles->size(); ++ i) {
     
    const Candidate & p = (*genParticles)[i];
    int id = p.pdgId();
        
    if (abs(id) == 13 ) {
    	nGenMuon->Fill(p.pt(), p.eta(), p.phi(), p.mass());
	std::cout<<"I am muon : "<<id<<std::endl;
	int k=p.numberOfMothers();
	for(int l=0; l<k; l++){
		const Candidate * m = p.mother(l);
		int m_id = 0;
		m_id=m->pdgId();
 		std::cout<<"I am the mother of muon : "<<m_id<<std::endl;
		nGenMuonMom->Fill(m_id,m->pt(),m->eta(),m->phi(),m->mass());
		if(m_id == 443){
			nGenJpsiToMM->Fill(m_id,m->pt(),m->eta(),m->phi(),m->mass());
			std::cout<<"the mother of muon is jpsi : "<<m_id<<std::endl;
		}else if(m_id == 553){
			nGenUpsiToMM->Fill(m_id,m->pt(),m->eta(),m->phi(),m->mass());
			std::cout<<"the mother of muon is upsilon : "<<m_id<<std::endl;
		}else if(m_id == 23){
			nGenZ0ToMM->Fill(m_id,m->pt(),m->eta(),m->phi(),m->mass());
			std::cout<<"the mother of muon is Z_0 : "<<m_id<<std::endl;
		}
	}
    }
    if (abs(id) == 443){
	nGenJpsi->Fill(p.pt(), p.eta(), p.phi(), p.mass(),p.status());
	std::cout<<"I am jpsi : "<<id<<std::endl;
	int k=p.numberOfMothers();
	if(k == 0) {
	std::cout<<"This is primary jpsi, its status : "<<p.status()<<std::endl;
	nGenPriJpsi->Fill(p.pt(), p.eta(), p.phi(), p.mass(),p.status());
		int n=p.numberOfDaughters();
        	for(int j=0; j<n ; j++){
                  	const Candidate * d = p.daughter(j);
                  	int dj_id = 0;
                  	dj_id = d->pdgId();
                  	//std::cout<<"daughter particle is "<<dj_id<<std::endl;
                  	nGenPriJpsiDau->Fill(dj_id,d->pt(),d->eta(),d->phi(),d->mass());
                  	if(dj_id == 13){
                        	nGenPriJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                        	nGenPriJpsiMM->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                  	}
                  	if(dj_id == -13){
                        	nGenPriJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                  	}
        	}

	}
	if(k == 1) {
	std::cout<<"This is secondary jpsi"<<std::endl;
		const Candidate * m = p.mother(0);
		std::cout<<"I am the mother of jpsi : "<<m->pdgId()<<", jpsi mom status : "<<m->status()<<", jpsi status : "<<p.status()<<std::endl;	
    		nGenJpsiMom->Fill(m->pdgId(),m->pt(), m->eta(), m->phi(), m->mass(), m->status());
		nGenSecJpsi->Fill(p.pt(), p.eta(), p.phi(), p.mass(), m->pdgId(), m->status(), p.status());
		int n=p.numberOfDaughters();
                for(int j=0; j<n ; j++){
                        const Candidate * d = p.daughter(j);
                        int dj_id = 0;
                        dj_id = d->pdgId();
                        //std::cout<<"daughter particle is "<<dj_id<<std::endl;
                        nGenSecJpsiDau->Fill(dj_id,d->pt(),d->eta(),d->phi(),d->mass());
                        if(dj_id == 13){
                                nGenSecJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                                nGenSecJpsiMM->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                        }
                        if(dj_id == -13){
                                nGenSecJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                        }
                }

	}
	int n=p.numberOfDaughters();
    	for(int j=0; j<n ; j++){
    		  const Candidate * d = p.daughter(j);
		  int dj_id = 0;
		  dj_id = d->pdgId();
		  //std::cout<<"daughter particle is "<<dj_id<<std::endl;
		  nGenJpsiDau->Fill(dj_id,d->pt(),d->eta(),d->phi(),d->mass());
    		  if(dj_id == 13){
    			nGenJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
    			nGenJpsiMM->Fill(p.pt(),p.eta(),p.phi(),p.mass());
    		  }
		  if(dj_id == -13){
		  	nGenJpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());		 
		  }
	}
    }
    if (abs(id) == 553){
	std::cout<<"I am upsilon : "<<id<<std::endl;
    	nGenUpsilon->Fill(p.pt(), p.eta(), p.phi(), p.mass(), p.status());
	int k=p.numberOfMothers();
        if(k == 0) {
        std::cout<<"This is primary upsilon, its status : "<<p.status()<<std::endl;
        nGenPriUpsi->Fill(p.pt(), p.eta(), p.phi(), p.mass(), p.status());
        } 
	if(k == 1) {
	std::cout<<"This is secondary upsilon"<<std::endl;
	const Candidate * m = p.mother(0);
	std::cout<<"I am the mother of upsilon : "<<m->pdgId()<<", mother status : "<<m->status()<<", upsilon status : "<<p.status()<<std::endl;
	nGenSecUpsi->Fill(p.pt(), p.eta(), p.phi(), p.mass(), m->pdgId(), m->status(), p.status());
	nGenUpsiMom->Fill(m->pdgId(),m->pt(), m->eta(), m->phi(), m->mass(), m->status());
    	}
	int n=p.numberOfDaughters();
        for(int j=0; j<n ; j++){
                  const Candidate * d = p.daughter(j);
                  int dj_id = 0;
                  dj_id = d->pdgId();
                  //std::cout<<"daughter particle is "<<dj_id<<std::endl;
                  nGenUpsiDau->Fill(dj_id,d->pt(),d->eta(),d->phi(),d->mass());
                  if(dj_id == 13){
                        nGenUpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                        nGenUpsiMM->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                  }
                  if(dj_id == -13){
                        nGenUpsiMuon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                  }
        }
    }
    
    if (abs(id) == 23){
	std::cout<<"I am Z_0 : "<<id<<std::endl;
    	nGenZ0->Fill(p.pt(), p.eta(), p.phi(), p.mass(), p.status());
	int k=p.numberOfMothers();
        if(k == 0) {
        std::cout<<"This is primary Z_0, its status : "<<p.status()<<std::endl;
        nGenPriZ0->Fill(p.pt(), p.eta(), p.phi(), p.mass(), p.status());
        } 
	if(k == 1) {
	std::cout<<"This is secondary Z_0"<<std::endl;
        const Candidate * m = p.mother(0);
        std::cout<<"I am the mother of Z_0 : "<<m->pdgId()<<", Z_0 mom status : "<<m->status()<<", Z_0 status : "<<p.status()<<std::endl;
        nGenSecZ0->Fill(p.pt(), p.eta(), p.phi(), p.mass(), m->pdgId(), m->status(), p.status());
        nGenZ0Mom->Fill(m->pdgId(),m->pt(), m->eta(), m->phi(), m->mass());
	}
	int n=p.numberOfDaughters();
        for(int j=0; j<n ; j++){
                  const Candidate * d = p.daughter(j);
                  int dj_id = 0;
                  dj_id = d->pdgId();
                  //std::cout<<"daughter particle is "<<dj_id<<std::endl;
                  nGenZ0Dau->Fill(dj_id,d->pt(),d->eta(),d->phi(),d->mass());
                  if(dj_id == 13){
                        nGenZ0Muon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                        nGenZ0MM->Fill(p.pt(),p.eta(),p.phi(),p.mass());
                  }
                  if(dj_id == -13){
                        nGenZ0Muon->Fill(d->pt(),d->eta(),d->phi(),d->mass());
                  }
        }
    }
}
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HydQuarkoniaAnalyzer::endJob() {

  hOutputFile->SetCompressionLevel(2);
  hOutputFile->cd();

  hOutputFile->Write();
  hOutputFile->Close();

}

//define this as a plug-in
DEFINE_FWK_MODULE(HydQuarkoniaAnalyzer);
