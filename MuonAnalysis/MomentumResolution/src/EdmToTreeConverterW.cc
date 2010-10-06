///////////////////////////////////////////////////////////////////////////////
//
//   EdmToTreeConverterW, to create an ultrasimple Tree to analyze ZMuMus
//
///////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "TH1D.h"
#include "TTree.h"

#include "utilsW.h"

namespace reco {class GenParticle;}

class EdmToTreeConverterW : public edm::EDAnalyzer {
public:
  EdmToTreeConverterW (const edm::ParameterSet &);
  virtual ~EdmToTreeConverterW();
  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  void getGenData(edm::Handle<std::vector<reco::GenParticle> > genParticles,t_data& data);
private:
  int nbins_h, inibin_h, endbin_h;
  bool mode;int charge;int eta_bin;
  TH1D * hPt;
  TTree * tree;  
  t_data data;
};

#include <CLHEP/Random/RandGauss.h>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

using namespace std;
using namespace edm;
using namespace reco;

EdmToTreeConverterW::EdmToTreeConverterW( const ParameterSet & pset ) :
  nbins_h(pset.getUntrackedParameter<int>("Nbins_histo",40)),
  inibin_h(pset.getUntrackedParameter<int>("Inibin_histo",20)),
  endbin_h(pset.getUntrackedParameter<int>("Endbin_histo",90)),
  charge(pset.getUntrackedParameter<int>("Charge",0)),
  eta_bin(pset.getUntrackedParameter<int>("EtaBin",0))
{
}

/// Destructor
EdmToTreeConverterW::~EdmToTreeConverterW(){
}

void EdmToTreeConverterW::beginJob() {
  edm::Service<TFileService> fs;

  // Book histograms and canvas
  hPt=fs->make<TH1D>("HistoPt","HistoPt",nbins_h,inibin_h,endbin_h);
  tree = new TTree("tree","data");
  tree->Branch("reco",&data.mu_pt,"mu_pt/D:mu_pt_gen:nu_pt:mu_pz:mu_eta:mu_phi:mu_charge:nu_phi:WmassT:r1:rp1");  
}


void EdmToTreeConverterW::analyze (const Event & event, const EventSetup &) {
  // Get collections
  edm::Handle< vector<reco::CompositeCandidate> > CandHandle;
  if(!event.getByLabel(edm::InputTag("wmnCands"),CandHandle)) return; if(CandHandle->size()==0) return;
  edm::Handle< vector<reco::GenParticle>  > GenHandle;
  if(event.isRealData() != 1) event.getByLabel(edm::InputTag("wmnPrunedGenParticles"),GenHandle) ;

  for(unsigned int i=0;i<CandHandle->size();++i){
    const Candidate * mu = 0;
    const CompositeCandidate  WMuNu = (*CandHandle)[i];
    const Candidate * met = 0;
    if( (*CandHandle)[i].daughter(0)->charge() != 0) {mu = (*CandHandle)[i].daughter(0);met=(*CandHandle)[i].daughter(1);}
    else {(*CandHandle)[i].daughter(1);(*CandHandle)[i].daughter(0);}
    
    if(charge!=0 && charge*mu->charge() < 0) return;
    if(eta_bin == 1 && fabs(mu->eta()) > 0.9) return;
    if(eta_bin == 2 && fabs(mu->eta()) < 0.9) return;
    // in MC case, we get the generated parameters from Ws
    if(event.isRealData() == 1){
      hPt->Fill(mu->pt());
    }else{
      getGenData(GenHandle,data);
      //with collision data, we get the reco parameters from Ws
      // Get all interesting parameters for resolution studies
      data.mu_pt = mu->pt();
      data.mu_eta = mu->eta();
      data.WmassT = WMuNu.mt();
      data.mu_charge = WMuNu.charge();
      data.nu_pt= met->pt(); data.mu_pz = mu->pz(); data.mu_phi = mu->phi(); data.nu_phi = met->phi(); 
      data.r = CLHEP::RandGauss::shoot(0,1);  data.rp = CLHEP::RandGauss::shoot(0,1);   
      tree->Fill();
    }
    // random numbers necessary for the resolution method
  }
}

void EdmToTreeConverterW::endJob() {

}

void EdmToTreeConverterW::getGenData(Handle<reco::GenParticleCollection> genParticles,t_data& data){
  //for all the particles in the event (only in MC case)
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const reco::GenParticle & p = (*genParticles)[ i ];
    short GEN_daughters = p.numberOfDaughters();
    int GEN_particleID = p.pdgId();
    // if the particle is a W+ or W- and has at least 1 daughter
    if ((GEN_particleID==24 || GEN_particleID==-24) && GEN_daughters > 0){
      // iterate over all daughters
      for (short j=0; j<GEN_daughters; ++j){
        const reco::Candidate *dau =p.daughter(j);
        double dauId = dau->pdgId();
	// if the daugther is a muon+ or muon-
        if(dauId==13||dauId==-13){
	  // get the muon related parameters
	  data.mu_pt_gen = dau->pt();
	} 
      }
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(EdmToTreeConverterW);
