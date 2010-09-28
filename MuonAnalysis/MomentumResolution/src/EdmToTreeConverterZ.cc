#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include <CLHEP/Random/RandGauss.h>
#include "utils.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include "HLTrigger/Muon/interface/HLTMuonIsoFilter.h"
#include "HLTrigger/Muon/interface/HLTMuonL1Filter.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"

#include <CLHEP/Random/RandGauss.h>
//#include "master.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h" 
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TVectorD.h"
#include "TGraph.h"

#include <map>
#include <vector>
//#include "ZMuMuPtAnalyzer.h"






class TFile;
class TH1D;
class TH2D;


//struct t_data{double pt1,pt2,px_pt1,px_pt2,py_pt1,py_pt2,pz_pt1,pz_pt2,p_pt1,p_pt2,eta1,phi1,eta2,phi2,mass,r1,r2,rp1,rp2;};

using namespace std;
using namespace edm;
using namespace reco;


class EdmToTreeConverterZ : public edm::EDAnalyzer {
public:
  EdmToTreeConverterZ(const ParameterSet& pset);
  virtual ~EdmToTreeConverterZ();
  virtual void beginJob();//const EventSetup& eventSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
private:
  TTree * treeZ; TTree * treeU;
  t_data data;
  TH1D * hZMass;
  TH1D * hUMass;
  bool doups;
  int nbins_hmassZ;
  int inibin_hmassZ;
  int endbin_hmassZ;
  int nbins_hmassU;
  int inibin_hmassU;
  int endbin_hmassU;
};

#include <CLHEP/Random/RandGauss.h>

/// Constructor
EdmToTreeConverterZ::EdmToTreeConverterZ(const ParameterSet& pset) :
  doups(pset.getUntrackedParameter<bool>("Doups")),
  nbins_hmassZ(pset.getUntrackedParameter<int>("Nbins_histomassZ",40)),
  inibin_hmassZ(pset.getUntrackedParameter<int>("Inibin_histomassZ",70)),
  endbin_hmassZ(pset.getUntrackedParameter<int>("Endbin_histomassZ",110)),
  nbins_hmassU(pset.getUntrackedParameter<int>("Nbins_histomassU",10)),
  inibin_hmassU(pset.getUntrackedParameter<int>("Inibin_histomassU",6)),
  endbin_hmassU(pset.getUntrackedParameter<int>("Endbin_histomassU",8))
{ 
}

/// Destructor
EdmToTreeConverterZ::~EdmToTreeConverterZ(){
}

void EdmToTreeConverterZ::beginJob(){//const EventSetup& eventSetup){
  edm::Service<TFileService> fs;

  hZMass = fs->make<TH1D>("HistoMassZ","HistoMassZ",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);

  if(doups){
    hUMass = fs->make<TH1D>("HistoMassU","HistoMassU",nbins_hmassU,inibin_hmassU,endbin_hmassU);
  }
  treeZ = new TTree("treeZ","data");
  treeU = new TTree("treeU","data");
  treeZ->Branch("reco",&data.pt1,"pt1/D:pt2:pt1_gen:pt2_gen:px_pt1:px_pt2:py_pt1:py_pt2:pz_pt1:pz_pt2:p_pt1:p_pt2:eta1:phi1:eta2:phi2:mass:r1:r2:rp1:rp2");
  treeU->Branch("reco",&data.pt1,"pt1/D:pt2:pt1_gen:pt2_gen:px_pt1:px_pt2:py_pt1:py_pt2:pz_pt1:pz_pt2:p_pt1:p_pt2:eta1:phi1:eta2:phi2:mass:r1:r2:rp1:rp2");
}

void EdmToTreeConverterZ::endJob(){//const EventSetup& eventSetup){
}

void EdmToTreeConverterZ::analyze(const Event & event, const EventSetup& eventSetup){
  
  edm::Handle< vector<reco::CompositeCandidate> > CandHandle;
  if (!event.getByLabel(edm::InputTag("zmmCands"),CandHandle)) return;
  
  if(CandHandle->size()==0) return;
  edm::Handle< vector<reco::GenParticle> > GenHandle;
  event.getByLabel(edm::InputTag("zmmPrunedGenParticles"),GenHandle);

  for (unsigned int i=0; i<CandHandle->size(); ++i) {
    double mass = (*CandHandle)[i].mass();
    if(event.isRealData() == 1){
      if(mass > 60){
	hZMass->Fill((*CandHandle)[i].mass());
      }else{
	hUMass->Fill((*CandHandle)[i].mass());
      }
    }	
    else{
      const Candidate * cand1,* cand2;
      if((*CandHandle)[i].daughter(0)->charge()==-1) {cand1 = (*CandHandle)[i].daughter(0); cand2 = (*CandHandle)[i].daughter(1);}
      else {cand1 = (*CandHandle)[i].daughter(1); cand2 = (*CandHandle)[i].daughter(0);}
      data.pt1 = cand1->pt();      data.pt2 = cand2->pt();
      data.px_pt1= cand1->px()/cand1->pt(); data.px_pt2 = cand2->px()/cand2->pt();
      data.py_pt1= cand1->py()/cand1->pt(); data.py_pt2 = cand2->py()/cand2->pt();
      data.pz_pt1= cand1->pz()/cand1->pt(); data.pz_pt2 = cand2->pz()/cand2->pt();
      data.p_pt1= cand1->p()/cand1->pt(); data.p_pt2 = cand2->p()/cand2->pt();
      data.eta1 = cand1->eta();       data.eta2 = cand2->eta(); 
      data.phi1 = cand1->phi();       data.phi2 = cand2->phi(); 
      data.mass = (*CandHandle)[i].mass();
      data.r1 = CLHEP::RandGauss::shoot(0,1);  data.r2 = CLHEP::RandGauss::shoot(0,1);  data.rp1 = CLHEP::RandGauss::shoot(0,1);  data.rp2 = CLHEP::RandGauss::shoot(0,1);
      if(mass > 60) treeZ->Fill(); else treeU->Fill();
    }
  }
  if(event.isRealData()!=1){
    for (unsigned int i=0; i<GenHandle->size(); ++i) {
      const reco::GenParticle & p = (*GenHandle)[ i ];
      short GEN_daughters = p.numberOfDaughters();
      int GEN_particleID = p.pdgId();
      // if the particle is a W+ or W- and has at least 1 daughter
      if (GEN_particleID==23 && GEN_daughters > 0){
	for (short j=0; j<GEN_daughters; ++j){
	  const reco::Candidate *dau =p.daughter(j);
	  double dauId = dau->pdgId();
	  // if the daugther is a muon+ or muon-
	  if(dauId==13) data.pt1_gen = dau->pt();
	  
	  if(dauId==-13) data.pt2_gen = dau->pt();
	}
      }
    }
  }
}

DEFINE_FWK_MODULE(EdmToTreeConverterZ);
