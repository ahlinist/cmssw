#include "ElectroWeakAnalysis/EWKTau/interface/ETauAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


ETauAnalyzer::ETauAnalyzer(const edm::ParameterSet& cfg):
  elecs_(cfg.getParameter<VInputTag>("ElecInputTags")),
  taus_(cfg.getParameter<VInputTag>("TauInputTags")),
  refObjects_(cfg.getParameter<VInputTag>("RefInputTags")),
  met_(cfg.getParameter<edm::InputTag>("MetInputTag")),  
  doTauAfterElec_(cfg.getParameter<bool>("DoTauIdAfterElecId")),
  doMetCuts_(cfg.getParameter<bool>("DoMetCuts")),
  eMetDphi_(cfg.getParameter<double>("eMetDphiCut")),
  eMetMass_(cfg.getParameter<double>("eMetMassCut")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  doZee_(cfg.getParameter<bool>("DoZee"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
  
  hPatL1ElecEt=fs->make<TH1F>("PatL1ElecEt","PatL1ElecEt",100,0.,200);
  hSelKinElecEt=fs->make<TH1F>("SelKinElecEt","SelKinElecEt",100,0.,200);
  hSelMchHLTElecEt=fs->make<TH1F>("SelMchHLTElecEt","SelMchHLTElecEt",100,0.,200);
  hSelTrkIsoElecEt=fs->make<TH1F>("SelTrkIsoElecEt","SelTrkIsoElecEt",100,0.,200);
  hSelEclIsoElecEt=fs->make<TH1F>("SelEclIsoElecEt","SelEclIsoElecEt",100,0.,200);
  hSelHclIsoElecEt=fs->make<TH1F>("SelHclIsoElecEt","SelHclIsoElecEt",100,0.,200);
  hSelIdElecEt=fs->make<TH1F>("SelIdElecEt","SelIdElecEt",100,0.,200);
  hSelIpElecEt=fs->make<TH1F>("SelIpElecEt","SelIpElecEt",100,0.,200);

  hPatL1ElecEta=fs->make<TH1F>("PatL1ElecEta","PatL1ElecEta",60,-3.,3.);
  hSelKinElecEta=fs->make<TH1F>("SelKinElecEta","SelKinElecEta",60,-3.,3.);
  hSelMchHLTElecEta=fs->make<TH1F>("SelMchHLTElecEta","SelMchHLTElecEta",60,-3.,3.);
  hSelTrkIsoElecEta=fs->make<TH1F>("SelTrkIsoElecEta","SelTrkIsoElecEta",60,-3.,3.);
  hSelEclIsoElecEta=fs->make<TH1F>("SelEclIsoElecEta","SelEclIsoElecEta",60,-3.,3.);
  hSelHclIsoElecEta=fs->make<TH1F>("SelHclIsoElecEta","SelHclIsoElecEta",60,-3.,3.);
  hSelIdElecEta=fs->make<TH1F>("SelIdElecEta","SelIdElecEta",60,-3.,3.);
  hSelIpElecEta=fs->make<TH1F>("SelIpElecEta","SelIpElecEta",60,-3.,3.);  

  hPatL1ElecPhi=fs->make<TH1F>("PatL1ElecPhi","PatL1ElecPhi",60,-3.,3.);
  hSelKinElecPhi=fs->make<TH1F>("SelKinElecPhi","SelKinElecPhi",60,-3.,3.);
  hSelMchHLTElecPhi=fs->make<TH1F>("SelMchHLTElecPhi","SelMchHLTElecPhi",60,-3.,3.);
  hSelTrkIsoElecPhi=fs->make<TH1F>("SelTrkIsoElecPhi","SelTrkIsoElecPhi",60,-3.,3.);
  hSelEclIsoElecPhi=fs->make<TH1F>("SelEclIsoElecPhi","SelEclIsoElecPhi",60,-3.,3.);
  hSelHclIsoElecPhi=fs->make<TH1F>("SelHclIsoElecPhi","SelHclIsoElecPhi",60,-3.,3.);
  hSelIdElecPhi=fs->make<TH1F>("SelIdElecPhi","SelIdElecPhi",60,-3.,3.);
  hSelIpElecPhi=fs->make<TH1F>("SelIpElecPhi","SelIpElecPhi",60,-3.,3.);

  hTrkIsoElec=fs->make<TH1F>("TrkIsoElec","TrkIsoElec",20,0,20);
  hEclIsoElec=fs->make<TH1F>("EclIsoElec","EclIsoElec",80,0,20);
  hHclIsoElec=fs->make<TH1F>("HclIsoElec","HclIsoElec",40,0,20);
  hEoPElec=fs->make<TH1F>("EoPElec","EoPElec",20,0,2);
  hDphiInElec=fs->make<TH1F>("DphiInElec","DphiInElec",100,-1,1);
  hDetaInElec=fs->make<TH1F>("DetaInElec","DetaInElec",100,-1,1);
  hHoEElec=fs->make<TH1F>("HoEElec","HoEElec",50,0,10);
  hSigEtaEtaElec=fs->make<TH1F>("SigEtaEtaElec","SigEtaEtaElec",20,0,1);

  hPatL1TauEt=fs->make<TH1F>("PatL1TauEt","PatL1TauEt",100,0.,200);
  hSelKinTauEt=fs->make<TH1F>("SelKinTauEt","SelKinTauEt",100,0.,200);
  hSelMchHLTTauEt=fs->make<TH1F>("SelMchHLTTauEt","SelMchHLTTauEt",100,0.,200);
  hSelTrkIsoTauEt=fs->make<TH1F>("SelTrkIsoTauEt","SelTrkIsoTauEt",100,0.,200);
  hSelEclIsoTauEt=fs->make<TH1F>("SelEclIsoTauEt","SelEclIsoTauEt",100,0.,200);
  hSelHclIsoTauEt=fs->make<TH1F>("SelHclIsoTauEt","SelHclIsoTauEt",100,0.,200);
  hSelTrkTauEt=fs->make<TH1F>("SelTrkTauEt","SelTrkTauEt",100,0.,200);
  hSelLdgTrkTauEt=fs->make<TH1F>("SelLdgTrkTauEt","SelLdgTrkTauEt",100,0.,200);
  hSelElecRejTauEt=fs->make<TH1F>("SelElecRejTauEt","SelElecRejTauEt",100,0.,200);
  hSelCrackTauEt=fs->make<TH1F>("SelCrackTauEt","SelCrackTauEt",100,0.,200);
  hSelProngTauEt=fs->make<TH1F>("SelProngTauEt","SelProngTauEt",100,0.,200);
  
  hPatL1TauEta=fs->make<TH1F>("PatL1TauEta","PatL1TauEta",60,-3.,3.);
  hSelKinTauEta=fs->make<TH1F>("SelKinTauEta","SelKinTauEta",60,-3.,3.);
  hSelMchHLTTauEta=fs->make<TH1F>("SelMchHLTTauEta","SelMchHLTTauEta",60,-3.,3.);
  hSelTrkIsoTauEta=fs->make<TH1F>("SelTrkIsoTauEta","SelTrkIsoTauEta",60,-3.,3.);
  hSelEclIsoTauEta=fs->make<TH1F>("SelEclIsoTauEta","SelEclIsoTauEta",60,-3.,3.);
  hSelHclIsoTauEta=fs->make<TH1F>("SelHclIsoTauEta","SelHclIsoTauEta",60,-3.,3.);
  hSelTrkTauEta=fs->make<TH1F>("SelTrkTauEta","SelTrkTauEta",60,-3.,3.);
  hSelLdgTrkTauEta=fs->make<TH1F>("SelLdgTrkTauEta","SelLdgTrkTauEta",60,-3.,3.);
  hSelElecRejTauEta=fs->make<TH1F>("SelElecRejTauEta","SelElecRejTauEta",60,-3.,3.);
  hSelCrackTauEta=fs->make<TH1F>("SelCrackTauEta","SelCrackTauEta",60,-3.,3.);
  hSelProngTauEta=fs->make<TH1F>("SelProngTauEta","SelProngTauEta",60,-3.,3.);
  
  hPatL1TauPhi=fs->make<TH1F>("PatL1TauPhi","PatL1TauPhi",60,-3.,3.);
  hSelKinTauPhi=fs->make<TH1F>("SelKinTauPhi","SelKinTauPhi",60,-3.,3.);
  hSelMchHLTTauPhi=fs->make<TH1F>("SelMchHLTTauPhi","SelMchHLTTauPhi",60,-3.,3.);
  hSelTrkIsoTauPhi=fs->make<TH1F>("SelTrkIsoTauPhi","SelTrkIsoTauPhi",60,-3.,3.);
  hSelEclIsoTauPhi=fs->make<TH1F>("SelEclIsoTauPhi","SelEclIsoTauPhi",60,-3.,3.);
  hSelHclIsoTauPhi=fs->make<TH1F>("SelHclIsoTauPhi","SelHclIsoTauPhi",60,-3.,3.);
  hSelTrkTauPhi=fs->make<TH1F>("SelTrkTauPhi","SelTrkTauPhi",60,-3.,3.);
  hSelLdgTrkTauPhi=fs->make<TH1F>("SelLdgTrkTauPhi","SelLdgTrkTauPhi",60,-3.,3.);
  hSelElecRejTauPhi=fs->make<TH1F>("SelElecRejTauPhi","SelElecRejTauPhi",60,-3.,3.);
  hSelCrackTauPhi=fs->make<TH1F>("SelCrackTauPhi","SelCrackTauPhi",60,-3.,3.);
  hSelProngTauPhi=fs->make<TH1F>("SelProngTauPhi","SelProngTauPhi",60,-3.,3.);

  hRawMet = fs->make<TH1F>("RawMet","RawMet",50,0,100);
  hType1Met = fs->make<TH1F>("Type1Met","Type1Met",50,0,100);
  hMuonMet = fs->make<TH1F>("MuonMet","MuonMet",50,0,100);
  hMuonType1Met = fs->make<TH1F>("MuonType1Met","MuonType1Met",50,0,100);
  hGenMet = fs->make<TH1F>("GenMet","GenMet",50,0,100);
  hRawMetPhi = fs->make<TH1F>("RawMetPhi","RawMetPhi",60,-3.,3.);
  hType1MetPhi = fs->make<TH1F>("Type1MetPhi","Type1MetPhi",60,-3.,3.);
  hMuonMetPhi = fs->make<TH1F>("MuonMetPhi","MuonMetPhi",60,-3.,3.);
  hMuonType1MetPhi = fs->make<TH1F>("MuonType1MetPhi","MuonType1MetPhi",
				    60,-3.,3.);
  hGenMetPhi = fs->make<TH1F>("GenMetPhi","GenMetPhi",60,-3.,3.);

  hRawMetRes = fs->make<TH1F>("RawMetRes","RawMetRes",50,-2,2);
  hType1MetRes = fs->make<TH1F>("Type1MetRes","Type1MetRes",50,-2,2);
  hMuonMetRes = fs->make<TH1F>("MuonMetRes","MuonMetRes",50,-2,2);
  hMuonType1MetRes = fs->make<TH1F>("MuonType1MetRes","MuonType1MetRes",
				    50,-2,2);
  hRawMetPhiRes = fs->make<TH1F>("RawMetPhiRes","RawMetPhiRes",50,-2,2);
  hType1MetPhiRes = fs->make<TH1F>("Type1MetPhiRes","Type1MetPhiRes",50,-2,2);
  hMuonMetPhiRes = fs->make<TH1F>("MuonMetPhiRes","MuonMetPhiRes",50,-2,2);
  hMuonType1MetPhiRes = fs->make<TH1F>("MuonType1MetPhiRes","MuonType1MetPhiRes",
				       50,-2,2);
  hM_eMet = fs->make<TH1F>("MeMet","MeMet",50,0,100);
  hDphi_eMet = fs->make<TH1F>("DphieMet","DphieMet",60,-3.2,3.2);
  hDphi_tMet = fs->make<TH1F>("DphitMet","DphitMet",60,-3.2,3.2);

  hMvis = fs->make<TH1F>("Mvis","Mvis",80,0,200);

  trkIsoHistoVec.reserve(10);eclIsoHistoVec.reserve(10);hclIsoHistoVec.reserve(10);
  for(int i=0;i<10;i++)
    {
      TString name = "trkIsoVal";
      trkIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,20,0,20);
      name = "eclIsoVal";
      eclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,80,0,20);
      name = "hclIsoVal";
      hclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,40,0,20);
    }

  hSelHisto=fs->make<TH1F>("SelHisto","SelHisto",30,0,30);

}


ETauAnalyzer::~ETauAnalyzer()
{
}


void ETauAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{  
   
  m_refInd.clear();
  m_count_sel=0;
  hSelHisto->Fill(m_count_sel+0.5);
  
  GetRefObjects(evt);

  FillElecHists(evt,elecs_[0],hPatL1ElecEt,hPatL1ElecEta,hPatL1ElecPhi);

  FillElecHists(evt,elecs_[1],hSelKinElecEt,hSelKinElecEta,hSelKinElecPhi);

  FillElecHists(evt,elecs_[2],hSelMchHLTElecEt,hSelMchHLTElecEta,hSelMchHLTElecPhi);
  
  FillElecHists(evt,elecs_[3],hSelTrkIsoElecEt,hSelTrkIsoElecEta,hSelTrkIsoElecPhi);

  FillElecHists(evt,elecs_[4],hSelEclIsoElecEt,hSelEclIsoElecEta,hSelEclIsoElecPhi);
  
  FillElecHists(evt,elecs_[5],hSelHclIsoElecEt,hSelHclIsoElecEta,hSelHclIsoElecPhi);
  
  FillElecHists(evt,elecs_[6],hSelIdElecEt,hSelIdElecEta,hSelIdElecPhi);
  
  FillElecHists(evt,elecs_[7],hSelIpElecEt,hSelIpElecEta,hSelIpElecPhi);

  FillElecIsoHistos(evt,elecs_[2]);
  

  int n_e_sel = elecs_.size();
  edm::Handle<std::vector<pat::Electron> > final_elecs;
  m_count_sel++; // Create separation between tau and elec selections
  if((doTauAfterElec_&&evt.getByLabel(elecs_[n_e_sel-1],final_elecs)&&
     final_elecs->size()) || !doTauAfterElec_){
   
    FillTauHists(evt,taus_[0],hPatL1TauEt,hPatL1TauEta,hPatL1TauPhi);
    
    FillTauHists(evt,taus_[0],hSelKinTauEt,hSelKinTauEta,hSelKinTauPhi);
    
    FillTauHists(evt,taus_[1],hSelTrkTauEt,hSelTrkTauEta,hSelTrkTauPhi);
    
    FillTauHists(evt,taus_[2],hSelLdgTrkTauEt,hSelLdgTrkTauEta,hSelLdgTrkTauPhi);
    
    FillTauHists(evt,taus_[3],hSelEclIsoTauEt,hSelEclIsoTauEta,hSelEclIsoTauPhi);
    
    FillTauHists(evt,taus_[4],hSelTrkIsoTauEt,hSelTrkIsoTauEta,hSelTrkIsoTauPhi);
    
    FillTauHists(evt,taus_[5],hSelElecRejTauEt,hSelElecRejTauEta,hSelElecRejTauPhi);
    
    //FillTauHists(evt,taus_[6],hSelCrackTauEt,hSelCrackTauEta,hSelCrackTauPhi);
    
    FillTauHists(evt,taus_[6],hSelProngTauEt,hSelProngTauEta,hSelProngTauPhi);
  }
  
  
  int n_t_sel=taus_.size();
  edm::Handle<std::vector<pat::Tau> > final_taus;
  m_count_sel++; // Create separation between tau and event cleaning selections
  if(evt.getByLabel(elecs_[n_e_sel-1],final_elecs)&&final_elecs->size()&&
     evt.getByLabel(taus_[n_t_sel-1],final_taus)&&final_taus->size()){
    
    TLorentzVector raw_met,muon_met,muon_type1_met,type1_met,gen_met;
    MakeMetLVs(evt,raw_met,muon_met,muon_type1_met,type1_met,gen_met);
    
    pat::Electron elec = final_elecs->at(0);
    pat::Tau tau = final_taus->at(0);
    
    TLorentzVector tauLV,elecLV;
    tauLV.SetPtEtaPhiE(tau.et(),tau.eta(),tau.phi(),tau.energy());
    elecLV.SetPtEtaPhiE(elec.et(),elec.eta(),elec.phi(),elec.energy());
  
    std::vector<double> metCuts = MakeMetPlots(raw_met,muon_met,muon_type1_met,
					       type1_met,gen_met,tauLV,elecLV);
    std::vector<bool> matches = MatchFinalObjects(tauLV,elecLV);
    if((doMatching_&&matches[0]&&matches[1]) || !doMatching_){
      if(doMetCuts_ && metCuts[0]<eMetDphi_ || !doMetCuts_){
	m_count_sel++;
	hSelHisto->Fill(m_count_sel+0.5);
	if(doMetCuts_ && metCuts[3]<eMetMass_ || !doMetCuts_){
	  m_count_sel++;
	  hSelHisto->Fill(m_count_sel+0.5);
	  std::cout<<tauLV.Et()<<"::"<<elecLV.Et()<<"::"<<(tauLV+elecLV).M()<<"::"<<
	    tauLV.DeltaR(elecLV)<<"::"<<tau.electronPreIDDecision()<<std::endl;
	  hMvis->Fill((tauLV+elecLV).M());
	}
      }		    
    }
  }


}


void ETauAnalyzer::beginJob(const edm::EventSetup&)
{
}

void ETauAnalyzer::endJob()
{
}


void ETauAnalyzer::GetRefObjects(const edm::Event& evt)
{
  evt.getByLabel(refObjects_[0],m_eRefs);
  evt.getByLabel(refObjects_[1],m_tRefs);
}

void ETauAnalyzer::FillElecHists(const edm::Event& evt,edm::InputTag tag,
				 TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Electron> > elecs;
  bool matched = false;
  if(evt.getByLabel(tag, elecs)){
    for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();
	 elec!=elecs->end(); ++elec){
      
      if(doMatching_){
	LorentzVector eLV;
	for(size_t i=0;i<m_eRefs->size();i++)
	  {
	    eLV.SetPxPyPzE(elec->px(),elec->py(),elec->pz(),elec->energy());
	    if(ROOT::Math::VectorUtil::DeltaR(eLV,m_eRefs->at(i))<0.1){
	      het->Fill(elec->et());
	      heta->Fill(elec->eta());
	      hphi->Fill(elec->phi());
	      if(m_count_sel==elecs_.size()-1){m_refInd.push_back(i);}
	      matched = true;
	    }
	  }
      } 
      else {
	het->Fill(elec->et());
	heta->Fill(elec->eta());
	hphi->Fill(elec->phi());
	matched = true;
      }
    }
  }
  m_count_sel++;
  if(matched==true)hSelHisto->Fill(m_count_sel+0.5);
}

void ETauAnalyzer::FillTauHists(const edm::Event& evt,edm::InputTag tag,
				TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Tau> > taus;
  bool matched = false;
  if(evt.getByLabel(tag, taus)){
    for( std::vector<pat::Tau>::const_iterator tau=taus->begin();
	 tau!=taus->end(); ++tau){
      if(doMatching_){
	LorentzVector tLV;
	tLV.SetPxPyPzE(tau->px(),tau->py(),tau->pz(),tau->energy());
	std::vector<int> refInd;
	for(size_t i=0;i<m_tRefs->size();i++)
	  {
	    if(ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.1){
	      refInd.push_back(i);
	      matched = true;
	      if(doZee_){
		for(size_t j=0;j<m_refInd.size();j++)
		  {
		    if((m_tRefs->at(m_refInd[j])+tLV).M()>70.&&
		       (m_tRefs->at(m_refInd[j])+tLV).M()<110.){
		      het->Fill(tau->et());
		      heta->Fill(tau->eta());
		      hphi->Fill(tau->phi());
		    }
		  }
	      }
	      if(!doZee_){
		het->Fill(tau->et());
		heta->Fill(tau->eta());
		hphi->Fill(tau->phi());
	      }
	    }
	  }
	
      }//if(doMatching_...
      else {
	het->Fill(tau->et());
	heta->Fill(tau->eta());
	hphi->Fill(tau->phi());
	matched = true;
      }
    }// for( std::vector ...
  }// if(getByLabel...
  m_count_sel++;
  if(matched == true)hSelHisto->Fill(m_count_sel+0.5);
}

void ETauAnalyzer::FillElecIsoHistos(const edm::Event& evt,edm::InputTag tag)
{
  edm::Handle<std::vector<pat::Electron> > elecs;
  if(evt.getByLabel(tag, elecs)){
    for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();
	 elec!=elecs->end(); ++elec){
      hTrkIsoElec->Fill(elec->trackIso());
      TuneIsoDeposit((*elec),"trk");
      if(elec->trackIso()==0){
	hEclIsoElec->Fill(elec->ecalIso());
	TuneIsoDeposit((*elec),"ecl");
	if(elec->ecalIso()<1.){
	  hHclIsoElec->Fill(elec->hcalIso());
	  TuneIsoDeposit((*elec),"hcl");
	  if(elec->hcalIso()<1.){
	    hEoPElec->Fill(elec->eSuperClusterOverP());
	    hDphiInElec->Fill(elec->deltaPhiSuperClusterTrackAtVtx());
	    hDetaInElec->Fill(elec->deltaEtaSuperClusterTrackAtVtx());
	    hHoEElec->Fill(elec->hadronicOverEm());
	    //hSigEtaEta->Fill(); // Dont know how to access it yet
	  }
	}
      }
    }
  }
}

void ETauAnalyzer::TuneIsoDeposit(const pat::Electron& elec,std::string isoType)
{
  const reco::IsoDeposit* isoDep = 0;
  std::string veto_1 = "",veto_2 = "";
  reco::isodeposit::AbsVetos vetos;

  if(isoType=="trk"){
    isoDep=elec.trackerIsoDeposit();
    veto_1="0.02";
    veto_2="Threshold(1.0)";
  }
  else if(isoType=="ecl"){
    isoDep=elec.ecalIsoDeposit();
    veto_1="0.0";
    veto_2="0.0";
  }
  else if(isoType=="hcl"){
    isoDep=elec.hcalIsoDeposit();
    veto_1="0.0";
    veto_2="0.0";
  }
  else {
    throw cms::Exception("Error")<<"Please Enter isoType = ecl,trk,hcl";
  }
    
  vetos.push_back(IsoDepositVetoFactory::make(veto_1.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_2.c_str()));
  for(size_t i=0;i<vetos.size();i++)
    vetos[i]->centerOn(isoDep->eta(),isoDep->phi());
  
  for(int i=1;i<10;i++)
    {
      double isoVal=0.;
      if(isoType=="trk"){
	isoVal=isoDep->countWithin(i*0.1,vetos,false);
	trkIsoHistoVec[i-1]->Fill(isoVal);
      }
      else if(isoType=="ecl"){
	isoVal=isoDep->depositWithin(i*0.1,vetos,true);
	eclIsoHistoVec[i-1]->Fill(isoVal);
      }
      else if(isoType=="hcl"){
	isoVal=isoDep->depositWithin(i*0.1,vetos,false);
	hclIsoHistoVec[i-1]->Fill(isoVal);
      }
    }
}

std::vector<bool> ETauAnalyzer::MatchFinalObjects(TLorentzVector& tauLV,TLorentzVector& elecLV)
{
  LorentzVector tLV;
  tLV.SetPxPyPzE(tauLV.Px(),tauLV.Py(),tauLV.Pz(),tauLV.E());
  bool matchTau=false;
  for(size_t i=0;i<m_tRefs->size();i++)
    {
      if(ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.1){
	matchTau = true;
	/*if(doZee_){
	  if((m_tRefs->at(m_refInd[i])+tLV).M()>70.&&
	  (m_tRefs->at(m_refInd[i])+tLV).M()<110.)matchTau=true;
	  }
	  else matchTau=true;*/
      }
    }
  LorentzVector eLV;
  eLV.SetPxPyPzE(elecLV.Px(),elecLV.Py(),elecLV.Pz(),elecLV.E());
  bool matchElec=false;
  for(size_t i=0;i<m_eRefs->size();i++)
    {
      if(ROOT::Math::VectorUtil::DeltaR(eLV,m_eRefs->at(i))<0.1){
	matchElec=true;
      }
    }
  
  std::vector<bool> matchVec;
  matchVec.push_back(matchElec);matchVec.push_back(matchTau);

  return matchVec;
}

void ETauAnalyzer::MakeMetLVs(const edm::Event& evt,TLorentzVector& raw_met,
			      TLorentzVector& muon_met,TLorentzVector& muon_type1_met,
			      TLorentzVector& type1_met, TLorentzVector& gen_met)
{
  edm::Handle<std::vector<pat::MET> > mets;
  evt.getByLabel(met_,mets);
  pat::MET met = mets->at(0);
  double raw_met_pt = met.uncorrectedPt(), 
    raw_met_phi = met.uncorrectedPhi();
  double muon_met_pt = met.uncorrectedPt(pat::MET::uncorrJES), 
    muon_met_phi = met.uncorrectedPhi(pat::MET::uncorrJES);
  double type1_met_pt = met.uncorrectedPt(pat::MET::uncorrMUON), 
    type1_met_phi = met.uncorrectedPhi(pat::MET::uncorrMUON);
  double muon_type1_met_pt = met.pt(),
    muon_type1_met_phi = met.phi();
  double gen_met_pt = met.genMET()->pt(),
    gen_met_phi = met.genMET()->phi();
    
  raw_met.SetPtEtaPhiE(raw_met_pt,0,raw_met_phi,raw_met_pt);
  muon_met.SetPtEtaPhiE(muon_met_pt,0,muon_met_phi,muon_met_pt);
  type1_met.SetPtEtaPhiE(type1_met_pt,0,type1_met_phi,type1_met_pt);
  muon_type1_met.SetPtEtaPhiE(muon_type1_met_pt,0,muon_type1_met_phi,muon_type1_met_pt);
  gen_met.SetPtEtaPhiE( gen_met_pt,0,gen_met_phi,gen_met_pt);
}

std::vector<double> ETauAnalyzer::MakeMetPlots(TLorentzVector& raw_met,
				TLorentzVector& muon_met,
				TLorentzVector& muon_type1_met,
				TLorentzVector& gen_met,
				TLorentzVector& type1_met,
				TLorentzVector& tauLV,
				TLorentzVector& elecLV)
{
  hRawMet->Fill(raw_met.Pt());hRawMetPhi->Fill(raw_met.Phi());
  hMuonMet->Fill(muon_met.Pt());hMuonMetPhi->Fill(muon_met.Phi());
  hMuonType1Met->Fill(muon_type1_met.Pt());hMuonType1MetPhi->Fill(muon_type1_met.Phi());
  hType1Met->Fill(type1_met.Pt());hType1MetPhi->Fill(type1_met.Phi());
  hGenMet->Fill(gen_met.Pt());hGenMetPhi->Fill(gen_met.Phi());
  
  hRawMetRes->Fill((raw_met.Pt()-gen_met.Pt())/gen_met.Pt());
  hMuonMetRes->Fill((muon_met.Pt()-gen_met.Pt())/gen_met.Pt());
  hType1MetRes->Fill((type1_met.Pt()-gen_met.Pt())/gen_met.Pt());
  hMuonType1MetRes->Fill((muon_type1_met.Pt()-gen_met.Pt())/gen_met.Pt());
  
  hRawMetPhiRes->Fill((raw_met.Phi()-gen_met.Phi())/gen_met.Phi());
  hMuonMetPhiRes->Fill((muon_met.Phi()-gen_met.Phi())/gen_met.Phi());
  hType1MetPhiRes->Fill((type1_met.Phi()-gen_met.Phi())/gen_met.Phi());
  hType1MetPhiRes->Fill((muon_type1_met.Phi()-gen_met.Phi())/gen_met.Phi());

  double eMetDphi = fabs(elecLV.Phi()-muon_met.Phi());
  if(eMetDphi>acos(-1.))eMetDphi = 2*acos(-1.)-eMetDphi;
  double tMetDphi = fabs(tauLV.Phi()-muon_met.Phi());
  if(tMetDphi>acos(-1.))tMetDphi = 2*acos(-1.)-tMetDphi;
  
  double eMetMass = (elecLV+muon_met).M();
  
  hM_eMet->Fill(eMetMass);
  hDphi_eMet->Fill(eMetDphi);
  hDphi_tMet->Fill(tMetDphi);

  std::vector<double> metCuts;
  metCuts.push_back(eMetDphi);
  metCuts.push_back(tMetDphi);
  metCuts.push_back(eMetMass);

  return metCuts;
}



/*
edm::Handle<edm::HepMCProduct> mcs;
evt.getByLabel("source", mcs);
HepMC::GenEvent*  myGenEvent = new  HepMC::GenEvent(*(mcs->GetEvent()));
HepMC::GenEvent::particle_iterator p = myGenEvent->particles_begin();

for (;p != myGenEvent->particles_end(); ++p ) {
  
  if((*p)->pdg_id()==23)std::cout<<"LALALALAL::::"<<(*p)->momentum().m()<<std::endl;
  
}
*/
