#include "ElectroWeakAnalysis/EWKTau/interface/ETauMassAnalyzer.h"

ETauMassAnalyzer::ETauMassAnalyzer(const edm::ParameterSet& cfg):
  tau_(cfg.getParameter<edm::InputTag>("TauTag")),
  elec_(cfg.getParameter<edm::InputTag>("ElecTag")),
  met_(cfg.getParameter<edm::InputTag>("MetTag")),
  refObjects_(cfg.getParameter<VInputTag>("RefInputTags")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  eventWeight_(cfg.getParameter<double>("EventWeight"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
  
  hETauMassVis=fs->make<TH1F>("ETauMassVis","ETauMassVis",100,0,200);
  hETauMassRaw=fs->make<TH1F>("ETauMassRaw","ETauMassRaw",100,0,200);
  hETauMassMu=fs->make<TH1F>("ETauMassMu","ETauMassMu",100,0,200);
  hETauMassT1=fs->make<TH1F>("ETauMassT1","ETauMassT1",100,0,200);
  hETauMassT1Mu=fs->make<TH1F>("ETauMassT1Mu","ETauMassT1Mu",100,0,200);
  
  hSelHisto=fs->make<TH1F>("SelHisto","SelHisto",30,0,30);
  
}


ETauMassAnalyzer::~ETauMassAnalyzer()
{
}

void ETauMassAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{  
  m_count_sel=0;
  
  edm::Handle<LorentzVectorCollection> mc_e;
  try{evt.getByLabel(refObjects_[0],mc_e);}catch(...){;}
 
  edm::Handle<LorentzVectorCollection> mc_t;
  try{evt.getByLabel(refObjects_[1],mc_t);}catch(...){;}
  
  edm::Handle<LorentzVectorCollection> mc_nu;
  try{evt.getByLabel(refObjects_[2],mc_nu);}catch(...){;}
  
  edm::Handle<std::vector<pat::Electron> > elecs;
  edm::Handle<std::vector<pat::Tau> > taus;
  edm::Handle<std::vector<pat::MET> > met;
  if(evt.getByLabel(elec_,elecs)&&
     evt.getByLabel(tau_,taus)&&
     evt.getByLabel(met_,met)&&
     elecs->size()==1&&
     taus->size()&&
     met->size()){
    LorentzVector fast_elec(
			    elecs->at(0).px(),
			    elecs->at(0).py(),
			    elecs->at(0).pz(),
			    elecs->at(0).energy()
			    );
    LorentzVector fast_tau(
			   taus->at(0).px(),
			   taus->at(0).py(),
			   taus->at(0).pz(),
			   taus->at(0).energy()
			   );
    
    pat::MET metObj = met->at(0);
    TLorentzVector raw;
    raw.SetPtEtaPhiM(metObj.uncorrectedPt(),0,
		     metObj.uncorrectedPhi(),0);
    TLorentzVector mu;
    mu.SetPtEtaPhiM(metObj.uncorrectedPt(pat::MET::uncorrJES),0,
		    metObj.uncorrectedPhi(pat::MET::uncorrJES),0);
    TLorentzVector t1;
    t1.SetPtEtaPhiM(metObj.uncorrectedPt(pat::MET::uncorrMUON),0,
		    metObj.uncorrectedPhi(pat::MET::uncorrMUON),0);
    TLorentzVector t1mu;
    t1mu.SetPtEtaPhiM(metObj.pt(),0,
		      metObj.phi(),0);
    
    LorentzVector metRaw(raw.Px(),raw.Py(),raw.Pz(),raw.E());
    LorentzVector metMu(mu.Px(),mu.Py(),mu.Pz(),mu.E());
    LorentzVector metT1(t1.Px(),t1.Py(),t1.Pz(),t1.E());
    LorentzVector metT1Mu(t1mu.Px(),t1mu.Py(),t1mu.Pz(),t1mu.E());
    
    double mVis = (fast_tau+fast_elec).M();
    double mRaw = (fast_tau+fast_elec+metRaw).M();
    double mMu = (fast_tau+fast_elec+metMu).M();
    double mT1 = (fast_tau+fast_elec+metT1).M();
    double mT1Mu = (fast_tau+fast_elec+metT1Mu).M();
    
    hETauMassVis->Fill(mVis,eventWeight_);
    hETauMassRaw->Fill(mRaw,eventWeight_);
    hETauMassT1->Fill(mT1,eventWeight_);
    hETauMassMu->Fill(mMu,eventWeight_);
    hETauMassT1Mu->Fill(mT1Mu,eventWeight_);
  }
}

void ETauMassAnalyzer::beginJob(const edm::EventSetup&)
{
}

void ETauMassAnalyzer::endJob()
{
}


