#include "ElectroWeakAnalysis/EWKTau/interface/MuTauAnalyzer.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


MuTauAnalyzer::MuTauAnalyzer(const edm::ParameterSet& cfg):
  muons_(cfg.getParameter<VInputTag>("MuInputTags")),
  taus_(cfg.getParameter<VInputTag>("TauInputTags")),
  refObjects_(cfg.getParameter<VInputTag>("RefInputTags")),
  met_(cfg.getParameter<edm::InputTag>("MetInputTag")),  
  doTauAfterMu_(cfg.getParameter<bool>("DoTauIdAfterMuId")),
  doMetCuts_(cfg.getParameter<bool>("DoMetCuts")),
  muMetDphi_(cfg.getParameter<double>("muMetDphiCut")),
  muMetMass_(cfg.getParameter<double>("muMetMassCut")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  doZmumu_(cfg.getParameter<bool>("DoZmumu"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
  
  hPatL1MuEt=fs->make<TH1F>("PatL1MuEt","PatL1MuEt",100,0.,200);
  hSelKinMuEt=fs->make<TH1F>("SelKinMuEt","SelKinMuEt",100,0.,200);
  hSelMchHLTMuEt=fs->make<TH1F>("SelMchHLTMuEt","SelMchHLTMuEt",100,0.,200);
  hSelTrkIsoMuEt=fs->make<TH1F>("SelTrkIsoMuEt","SelTrkIsoMuEt",100,0.,200);
  hSelEclIsoMuEt=fs->make<TH1F>("SelEclIsoMuEt","SelEclIsoMuEt",100,0.,200);
  hSelHclIsoMuEt=fs->make<TH1F>("SelHclIsoMuEt","SelHclIsoMuEt",100,0.,200);
  hSelIdMuEt=fs->make<TH1F>("SelIdMuEt","SelIdMuEt",100,0.,200);
  hSelIpMuEt=fs->make<TH1F>("SelIpMuEt","SelIpMuEt",100,0.,200);

  hPatL1MuEta=fs->make<TH1F>("PatL1MuEta","PatL1MuEta",60,-3.,3.);
  hSelKinMuEta=fs->make<TH1F>("SelKinMuEta","SelKinMuEta",60,-3.,3.);
  hSelMchHLTMuEta=fs->make<TH1F>("SelMchHLTMuEta","SelMchHLTMuEta",60,-3.,3.);
  hSelTrkIsoMuEta=fs->make<TH1F>("SelTrkIsoMuEta","SelTrkIsoMuEta",60,-3.,3.);
  hSelEclIsoMuEta=fs->make<TH1F>("SelEclIsoMuEta","SelEclIsoMuEta",60,-3.,3.);
  hSelHclIsoMuEta=fs->make<TH1F>("SelHclIsoMuEta","SelHclIsoMuEta",60,-3.,3.);
  hSelIdMuEta=fs->make<TH1F>("SelIdMuEta","SelIdMuEta",60,3.,-3.);
  hSelIpMuEta=fs->make<TH1F>("SelIpMuEta","SelIpMuEta",60,3.,-3.);  

  hPatL1MuPhi=fs->make<TH1F>("PatL1MuPhi","PatL1MuPhi",60,-3.,3.);
  hSelKinMuPhi=fs->make<TH1F>("SelKinMuPhi","SelKinMuPhi",60,-3.,3.);
  hSelMchHLTMuPhi=fs->make<TH1F>("SelMchHLTMuPhi","SelMchHLTMuPhi",60,-3.,3.);
  hSelTrkIsoMuPhi=fs->make<TH1F>("SelTrkIsoMuPhi","SelTrkIsoMuPhi",60,-3.,3.);
  hSelEclIsoMuPhi=fs->make<TH1F>("SelEclIsoMuPhi","SelEclIsoMuPhi",60,-3.,3.);
  hSelHclIsoMuPhi=fs->make<TH1F>("SelHclIsoMuPhi","SelHclIsoMuPhi",60,-3.,3.);
  hSelIdMuPhi=fs->make<TH1F>("SelIdMuPhi","SelIdMuPhi",60,-3.,3.);
  hSelIpMuPhi=fs->make<TH1F>("SelIpMuPhi","SelIpMuPhi",60,-3.,3.);

  hTrkIsoMu=fs->make<TH1F>("TrkIsoMu","TrkIsoMu",20,0,20);
  hEclIsoMu=fs->make<TH1F>("EclIsoMu","EclIsoMu",80,0,20);
  hHclIsoMu=fs->make<TH1F>("HclIsoMu","HclIsoMu",40,0,20);
  hEoPMu=fs->make<TH1F>("EoPMu","EoPMu",20,0,2);
  hDphiInMu=fs->make<TH1F>("DphiInMu","DphiInMu",100,-1,1);
  hDetaInMu=fs->make<TH1F>("DetaInMu","DetaInMu",100,-1,1);
  hHoEMu=fs->make<TH1F>("HoEMu","HoEMu",50,0,10);
  hSigEtaEtaMu=fs->make<TH1F>("SigEtaEtaMu","SigEtaEtaMu",20,0,1);

  hPatL1TauEt=fs->make<TH1F>("PatL1TauEt","PatL1TauEt",100,0.,200);
  hSelKinTauEt=fs->make<TH1F>("SelKinTauEt","SelKinTauEt",100,0.,200);
  hSelMchHLTTauEt=fs->make<TH1F>("SelMchHLTTauEt","SelMchHLTTauEt",100,0.,200);
  hSelTrkIsoTauEt=fs->make<TH1F>("SelTrkIsoTauEt","SelTrkIsoTauEt",100,0.,200);
  hSelEclIsoTauEt=fs->make<TH1F>("SelEclIsoTauEt","SelEclIsoTauEt",100,0.,200);
  hSelHclIsoTauEt=fs->make<TH1F>("SelHclIsoTauEt","SelHclIsoTauEt",100,0.,200);
  hSelTrkTauEt=fs->make<TH1F>("SelTrkTauEt","SelTrkTauEt",100,0.,200);
  hSelLdgTrkTauEt=fs->make<TH1F>("SelLdgTrkTauEt","SelLdgTrkTauEt",100,0.,200);
  //hSelElecRejTauEt=fs->make<TH1F>("SelElecRejTauEt","SelElecRejTauEt",100,0.,200);
  //hSelCrackTauEt=fs->make<TH1F>("SelCrackTauEt","SelCrackTauEt",100,0.,200);
  hSelProngTauEt=fs->make<TH1F>("SelProngTauEt","SelProngTauEt",100,0.,200);
  
  hPatL1TauEta=fs->make<TH1F>("PatL1TauEta","PatL1TauEta",60,-3.,3.);
  hSelKinTauEta=fs->make<TH1F>("SelKinTauEta","SelKinTauEta",60,-3.,3.);
  hSelMchHLTTauEta=fs->make<TH1F>("SelMchHLTTauEta","SelMchHLTTauEta",60,-3.,3.);
  hSelTrkIsoTauEta=fs->make<TH1F>("SelTrkIsoTauEta","SelTrkIsoTauEta",60,-3.,3.);
  hSelEclIsoTauEta=fs->make<TH1F>("SelEclIsoTauEta","SelEclIsoTauEta",60,-3.,3.);
  hSelHclIsoTauEta=fs->make<TH1F>("SelHclIsoTauEta","SelHclIsoTauEta",60,-3.,3.);
  hSelTrkTauEta=fs->make<TH1F>("SelTrkTauEta","SelTrkTauEta",60,-3.,3.);
  hSelLdgTrkTauEta=fs->make<TH1F>("SelLdgTrkTauEta","SelLdgTrkTauEta",60,-3.,3.);
  //hSelElecRejTauEta=fs->make<TH1F>("SelElecRejTauEta","SelElecRejTauEta",60,-3.,3.);
  //hSelCrackTauEta=fs->make<TH1F>("SelCrackTauEta","SelCrackTauEta",60,-3.,3.);
  hSelProngTauEta=fs->make<TH1F>("SelProngTauEta","SelProngTauEta",60,-3.,3.);
  
  hPatL1TauPhi=fs->make<TH1F>("PatL1TauPhi","PatL1TauPhi",60,-3.,3.);
  hSelKinTauPhi=fs->make<TH1F>("SelKinTauPhi","SelKinTauPhi",60,-3.,3.);
  hSelMchHLTTauPhi=fs->make<TH1F>("SelMchHLTTauPhi","SelMchHLTTauPhi",60,-3.,3.);
  hSelTrkIsoTauPhi=fs->make<TH1F>("SelTrkIsoTauPhi","SelTrkIsoTauPhi",60,-3.,3.);
  hSelEclIsoTauPhi=fs->make<TH1F>("SelEclIsoTauPhi","SelEclIsoTauPhi",60,-3.,3.);
  hSelHclIsoTauPhi=fs->make<TH1F>("SelHclIsoTauPhi","SelHclIsoTauPhi",60,-3.,3.);
  hSelTrkTauPhi=fs->make<TH1F>("SelTrkTauPhi","SelTrkTauPhi",60,-3.,3.);
  hSelLdgTrkTauPhi=fs->make<TH1F>("SelLdgTrkTauPhi","SelLdgTrkTauPhi",60,-3.,3.);
  //hSelElecRejTauPhi=fs->make<TH1F>("SelElecRejTauPhi","SelElecRejTauPhi",60,-3.,3.);
  //hSelCrackTauPhi=fs->make<TH1F>("SelCrackTauPhi","SelCrackTauPhi",60,-3.,3.);
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
  hM_muMet = fs->make<TH1F>("MmuMet","MmuMet",50,0,100);
  hDphi_muMet = fs->make<TH1F>("DphimuMet","DphimuMet",60,-3.2,3.2);
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


MuTauAnalyzer::~MuTauAnalyzer()
{
}


void MuTauAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{  
  std::cout<<"inizia analyze"<<std::endl; 
  m_refInd.clear();
  m_count_sel=0;
  hSelHisto->Fill(m_count_sel+0.5);
  
   GetRefObjects(evt);

  FillMuHists(evt,muons_[0],hPatL1MuEt,hPatL1MuEta,hPatL1MuPhi);

  FillMuHists(evt,muons_[1],hSelKinMuEt,hSelKinMuEta,hSelKinMuPhi);

  FillMuHists(evt,muons_[2],hSelMchHLTMuEt,hSelMchHLTMuEta,hSelMchHLTMuPhi);
  
  FillMuHists(evt,muons_[3],hSelTrkIsoMuEt,hSelTrkIsoMuEta,hSelTrkIsoMuPhi);

  FillMuHists(evt,muons_[4],hSelEclIsoMuEt,hSelEclIsoMuEta,hSelEclIsoMuPhi);
  
  FillMuHists(evt,muons_[5],hSelHclIsoMuEt,hSelHclIsoMuEta,hSelHclIsoMuPhi);
  
  FillMuHists(evt,muons_[6],hSelIdMuEt,hSelIdMuEta,hSelIdMuPhi);
  
  FillMuHists(evt,muons_[7],hSelIpMuEt,hSelIpMuEta,hSelIpMuPhi);

  FillMuIsoHistos(evt,muons_[2]);
  

   int n_mu_sel = muons_.size();
  edm::Handle<std::vector<pat::Muon> > final_muons;
   m_count_sel++; // Create separation between tau and elec selections
  if((doTauAfterMu_&&evt.getByLabel(muons_[n_mu_sel-1],final_muons)&&
     final_muons->size()) || !doTauAfterMu_){
   
    FillTauHists(evt,taus_[0],hPatL1TauEt,hPatL1TauEta,hPatL1TauPhi);
    
    FillTauHists(evt,taus_[0],hSelKinTauEt,hSelKinTauEta,hSelKinTauPhi);
    
    FillTauHists(evt,taus_[1],hSelTrkTauEt,hSelTrkTauEta,hSelTrkTauPhi);
    
    FillTauHists(evt,taus_[2],hSelLdgTrkTauEt,hSelLdgTrkTauEta,hSelLdgTrkTauPhi);
    
    FillTauHists(evt,taus_[3],hSelEclIsoTauEt,hSelEclIsoTauEta,hSelEclIsoTauPhi);
    
    FillTauHists(evt,taus_[4],hSelTrkIsoTauEt,hSelTrkIsoTauEta,hSelTrkIsoTauPhi);
    
    //FillTauHists(evt,taus_[5],hSelElecRejTauEt,hSelElecRejTauEta,hSelElecRejTauPhi);
    
    //FillTauHists(evt,taus_[6],hSelCrackTauEt,hSelCrackTauEta,hSelCrackTauPhi);
    
    FillTauHists(evt,taus_[6],hSelProngTauEt,hSelProngTauEta,hSelProngTauPhi);
  }
  
  
  int n_t_sel=taus_.size();
  edm::Handle<std::vector<pat::Tau> > final_taus;
  m_count_sel++; // Create separation between tau and event cleaning selections
  if(evt.getByLabel(muons_[n_mu_sel-1],final_muons)&&final_muons->size()&&
     evt.getByLabel(taus_[n_t_sel-1],final_taus)&&final_taus->size()){
    
     TLorentzVector raw_met,muon_met,muon_type1_met,type1_met,gen_met;
     MakeMetLVs(evt,raw_met,muon_met,muon_type1_met,type1_met,gen_met);
    
    pat::Muon muon = final_muons->at(0);
    pat::Tau tau = final_taus->at(0);
    
   TLorentzVector tauLV,muonLV;
     tauLV.SetPtEtaPhiE(tau.et(),tau.eta(),tau.phi(),tau.energy());
     muonLV.SetPtEtaPhiE(muon.et(),muon.eta(),muon.phi(),muon.energy());
  
    std::vector<double> metCuts = MakeMetPlots(raw_met,muon_met,muon_type1_met,
					       type1_met,gen_met,tauLV,muonLV);
    std::vector<bool> matches = MatchFinalObjects(tauLV,muonLV);
    if((doMatching_&&matches[0]&&matches[1]) || !doMatching_){
      if(doMetCuts_ && metCuts[0]<muMetDphi_ || !doMetCuts_){
	m_count_sel++;
	hSelHisto->Fill(m_count_sel+0.5);
	if(doMetCuts_ && metCuts[3]<muMetMass_ || !doMetCuts_){
	  m_count_sel++;
	  hSelHisto->Fill(m_count_sel+0.5);
// 	  //std::cout<<tauLV.Et()<<"::"<<muonLV.Et()<<"::"<<(tauLV+muonLV).M()<<"::"<<tauLV.DeltaR(muonLV)<<"::"<<tau.muonPreIDDecision()<<std::endl;
            std::cout<<tauLV.Et()<<"::"<<muonLV.Et()<<"::"<<(tauLV+muonLV).M()<<"::"<<tauLV.DeltaR(muonLV)<<"::"<<std::endl;

	  hMvis->Fill((tauLV+muonLV).M());
	}
      }		    
    }
 }


}


void MuTauAnalyzer::beginJob(const edm::EventSetup&)
{
  std::cout<<"inizia mio analizer"<<std::endl;
}

void MuTauAnalyzer::endJob()
{
}


void MuTauAnalyzer::GetRefObjects(const edm::Event& evt)
{
  evt.getByLabel(refObjects_[0],m_mRefs);
  evt.getByLabel(refObjects_[1],m_tRefs);
}

void MuTauAnalyzer::FillMuHists(const edm::Event& evt,edm::InputTag tag,
				 TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Muon> > muons;
  bool matched = false; //false
  if(evt.getByLabel(tag, muons)){
    for( std::vector<pat::Muon>::const_iterator muon=muons->begin();
	 muon!=muons->end(); ++muon){
      
      if(doMatching_){

	LorentzVector mLV;
	for(size_t i=0;i<m_mRefs->size();i++)
	  {
	    mLV.SetPxPyPzE(muon->px(),muon->py(),muon->pz(),muon->energy());
	    if(ROOT::Math::VectorUtil::DeltaR(mLV,m_mRefs->at(i))<0.1){
	      het->Fill(muon->et());
	      heta->Fill(muon->eta());
	      hphi->Fill(muon->phi());
	      if(m_count_sel==muons_.size()-1){m_refInd.push_back(i);}
	      matched = true;
	      std::cout<<"in Fill"<<std::endl;
	    }
	  }
      } 
      else {
	het->Fill(muon->et());
	heta->Fill(muon->eta());
	hphi->Fill(muon->phi());
	matched = true;
      }
    }
  }
  m_count_sel++;
  if(matched==true)hSelHisto->Fill(m_count_sel+0.5);
}

void MuTauAnalyzer::FillTauHists(const edm::Event& evt,edm::InputTag tag,
				TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Tau> > taus;
  bool matched = false;
  if(evt.getByLabel(tag, taus)){
    for( std::vector<pat::Tau>::const_iterator tau=taus->begin();
	 tau!=taus->end(); ++tau){
      if(doMatching_){
	LorentzVector tLV;
	for(size_t i=0;i<m_tRefs->size();i++)
	  {
	    tLV.SetPxPyPzE(tau->px(),tau->py(),tau->pz(),tau->energy());
	    if(ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.1){
	      if(doZmumu_&&taus->size()>=m_refInd.size()){
		for(size_t j=0;j<m_refInd.size();j++)
		  {
		    if((m_tRefs->at(m_refInd[j])+tLV).M()>70.&&
		       (m_tRefs->at(m_refInd[j])+tLV).M()<110.){
		      het->Fill(tau->et());
		      heta->Fill(tau->eta());
		      hphi->Fill(tau->phi());
		    }
		  }
	      }// if(doZmumu...
	      else {
		het->Fill(tau->et());
		heta->Fill(tau->eta());
		hphi->Fill(tau->phi());
	      }
	      matched = true;
	    }// if(DR<0.1...
	  }// for(Refs...
      }// if(doMatching...
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

void MuTauAnalyzer::FillMuIsoHistos(const edm::Event& evt,edm::InputTag tag)
{
  edm::Handle<std::vector<pat::Muon> > muons;
  if(evt.getByLabel(tag, muons)){
    for( std::vector<pat::Muon>::const_iterator muon=muons->begin();
	 muon!=muons->end(); ++muon){
      hTrkIsoMu->Fill(muon->trackIso());
      TuneIsoDeposit((*muon),"trk");
      if(muon->trackIso()==0){
	hEclIsoMu->Fill(muon->ecalIso());
	TuneIsoDeposit((*muon),"ecl");
	if(muon->ecalIso()<1.){
	  hHclIsoMu->Fill(muon->hcalIso());
	  TuneIsoDeposit((*muon),"hcl");
	  if(muon->hcalIso()<1.){
	    //hEoPMu->Fill(muon->eSuperClusterOverP());
	    //hDphiInMu->Fill(muon->deltaPhiSuperClusterTrackAtVtx());
	    //hDetaInMu->Fill(muon->deltaEtaSuperClusterTrackAtVtx());
	    //hHoEMu->Fill(muon->hadronicOverEm());
	    //hSigEtaEta->Fill(); // Dont know how to access it yet
	  }
	}
      }
    }
  }
}

void MuTauAnalyzer::TuneIsoDeposit(const pat::Muon& muon,std::string isoType)
{
  const reco::IsoDeposit* isoDep = 0;
  std::string veto_1 = "",veto_2 = "";
  reco::isodeposit::AbsVetos vetos;

  if(isoType=="trk"){
    isoDep=muon.trackerIsoDeposit();
    veto_1="0.02";
    veto_2="Threshold(1.0)";
  }
  else if(isoType=="ecl"){
    isoDep=muon.ecalIsoDeposit();
    veto_1="0.0";
    veto_2="0.0";
  }
  else if(isoType=="hcl"){
    isoDep=muon.hcalIsoDeposit();
    veto_1="0.0";
    veto_2="0.0";
  }
  else {
    throw cms::Exception("Error")<<"Please Enter isoType = ecl,trk,hcl";
  }
    
  vetos.push_back(IsoDepositVetoFactory::make(veto_1.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_2.c_str()));
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

std::vector<bool> MuTauAnalyzer::MatchFinalObjects(TLorentzVector& tauLV,TLorentzVector& muonLV)
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
  LorentzVector mLV;
  mLV.SetPxPyPzE(muonLV.Px(),muonLV.Py(),muonLV.Pz(),muonLV.E());
  bool matchMu=false;
  for(size_t i=0;i<m_mRefs->size();i++)
    {
      if(ROOT::Math::VectorUtil::DeltaR(mLV,m_mRefs->at(i))<0.1){
	matchMu=true;
      }
    }
  
  std::vector<bool> matchVec;
  matchVec.push_back(matchMu);matchVec.push_back(matchTau);

  return matchVec;
}

void MuTauAnalyzer::MakeMetLVs(const edm::Event& evt,TLorentzVector& raw_met,
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

std::vector<double> MuTauAnalyzer::MakeMetPlots(TLorentzVector& raw_met,
				TLorentzVector& muon_met,
				TLorentzVector& muon_type1_met,
				TLorentzVector& gen_met,
				TLorentzVector& type1_met,
				TLorentzVector& tauLV,
				TLorentzVector& muonLV)
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

  double mMetDphi = fabs(muonLV.Phi()-muon_met.Phi());
  if(mMetDphi>acos(-1.))mMetDphi = 2*acos(-1.)-mMetDphi;
  double tMetDphi = fabs(tauLV.Phi()-muon_met.Phi());
  if(tMetDphi>acos(-1.))tMetDphi = 2*acos(-1.)-tMetDphi;
  
  double mMetMass = (muonLV+muon_met).M();
  
  hM_muMet->Fill(mMetMass);
  hDphi_muMet->Fill(mMetDphi);
  hDphi_tMet->Fill(tMetDphi);

  std::vector<double> metCuts;
  metCuts.push_back(mMetDphi);
  metCuts.push_back(tMetDphi);
  metCuts.push_back(mMetMass);

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
