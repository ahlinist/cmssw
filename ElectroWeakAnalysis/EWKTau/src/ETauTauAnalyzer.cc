#include "ElectroWeakAnalysis/EWKTau/interface/ETauTauAnalyzer.h"

ETauTauAnalyzer::ETauTauAnalyzer(const edm::ParameterSet& cfg):
  taus_(cfg.getParameter<VInputTag>("TauInputTags")),
  elec_(cfg.getParameter<edm::InputTag>("FinalElecTag")),
  refObjects_(cfg.getParameter<VInputTag>("RefInputTags")),
  doTauAfterElec_(cfg.getParameter<bool>("DoTauIdAfterElecId")), 
  doTuning_(cfg.getParameter<bool>("DoTuning")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  eventWeight_(cfg.getParameter<double>("EventWeight"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
 
  hTauBremFracEoP=fs->make<TH1F>("TauBremFracEoP","TauBremFracEoP",80,0,4);
  hTauEmFracVsEoP_ePreId=fs->make<TH2F>("TauEmFracVsEoP_ePreId","TauEmFracVsEoP_ePreId",50,0,1,80,0,4);
  hTauEmFracVsEoP_AePreId=fs->make<TH2F>("TauEmFracVsEoP_AePreId","TauEmFracVsEoP_AePreId",80,0,4,80,0,4);
  hTauH3x3oPVsEoP_ePreId=fs->make<TH2F>("TauH3x3oPVsEoP_ePreId","TauH3x3oPVsEoP_ePreId",80,0,4,80,0,4);
  hTauH3x3oPVsEoP_AePreId=fs->make<TH2F>("TauH3x3oPVsEoP_AePreId","TauH3x3oPVsEoP_AePreId",80,0,4,80,0,4);
  hTauElecPreId=fs->make<TH1F>("TauElecPreId","TauElecPreId",2,0,2);
  hTauH3x3oP=fs->make<TH1F>("TauH3x3oP","TauH3x3oP",80,0,4);
  hTauEmFrac=fs->make<TH1F>("TauEmFrac","TauEmFrac",50,0,1); 
  hTauHoP=fs->make<TH1F>("TauHoP","TauHoP",80,0,4); 
  hTauHmaxoP=fs->make<TH1F>("TauHmaxoP","TauHmaxoP",80,0,4);

  std::vector<TString> tauHistoNames;
  tauHistoNames.push_back("SelKinTau");
  tauHistoNames.push_back("SelTrkTau");
  tauHistoNames.push_back("SelLdgTrkTau");
  tauHistoNames.push_back("SelTrkIsoTau");
  tauHistoNames.push_back("SelEclIsoTau");
  tauHistoNames.push_back("SelElecRejTau");
  tauHistoNames.push_back("SelProngTau");
  m_tauEtHistoVec.reserve(tauHistoNames.size());
  m_tauEtaHistoVec.reserve(tauHistoNames.size());
  m_tauPhiHistoVec.reserve(tauHistoNames.size());
  for(size_t i=0;i<tauHistoNames.size();i++)
    {
      m_tauEtHistoVec[i]=fs->make<TH1F>(tauHistoNames[i]+"Et",
				       tauHistoNames[i]+"Et",100,0,200);
      m_tauEtaHistoVec[i]=fs->make<TH1F>(tauHistoNames[i]+"Eta",
				       tauHistoNames[i]+"Eta",60,-3,3);
      m_tauPhiHistoVec[i]=fs->make<TH1F>(tauHistoNames[i]+"Phi",
				       tauHistoNames[i]+"Phi",60,-3,3);
    }
  
  hSelHisto=fs->make<TH1F>("SelHisto","SelHisto",30,0,30);

}


ETauTauAnalyzer::~ETauTauAnalyzer()
{
}

void ETauTauAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{  
  m_count_sel=0;
  hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
  m_count_sel++; // Create separation between all evts and tau selections
  
  //Get the matching objs
  if(doMatching_)evt.getByLabel(refObjects_[0],m_tRefs);
  
  // Get Final Electron Collection
  edm::Handle<std::vector<pat::Electron> > final_elecs;
  //Fill Selections after Elec pass Or Not
  if(doTauAfterElec_&&evt.getByLabel(elec_,final_elecs)&&final_elecs->size()
     ||!doTauAfterElec_){
    //Fill Elec Kin Histos
    for(size_t i=0;i<taus_.size();i++)
      FillTauHists(evt,taus_[i],m_tauEtHistoVec[i],
		   m_tauEtaHistoVec[i],m_tauPhiHistoVec[i]);
  
    MakeElecRejPlots(evt,taus_[4]);
  
  }
  
}


void ETauTauAnalyzer::beginJob(const edm::EventSetup&)
{
}

void ETauTauAnalyzer::endJob()
{
}

void ETauTauAnalyzer::FillTauHists(const edm::Event& evt,edm::InputTag tag,
				TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Tau> > taus;
  bool matched = false;
  if(evt.getByLabel(tag, taus)){
    for( std::vector<pat::Tau>::const_iterator tau=taus->begin();
	 tau!=taus->end(); ++tau){
      bool tau_matched = false;
      if(doMatching_){
	LorentzVector tLV(tau->px(),tau->py(),tau->pz(),tau->energy());
	for(size_t i=0;i<m_tRefs->size();i++)
	  if(tau->isPFTau()&&ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.1||
	     tau->isPFTau()&&ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.3)
	    tau_matched = true;
      }
      else tau_matched = true;
  
      if(tau_matched ==true){
	het->Fill(tau->et(),eventWeight_);
	heta->Fill(tau->eta(),eventWeight_);
	hphi->Fill(tau->phi(),eventWeight_);
	matched = true;
      }
    }
  }
  m_count_sel++;
  if(matched == true)hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
}


void ETauTauAnalyzer::MakeElecRejPlots(const edm::Event& evt,edm::InputTag ttag)
{
  edm::Handle<std::vector<pat::Tau> > taus;
  if(evt.getByLabel(ttag, taus)){
    for( std::vector<pat::Tau>::const_iterator tau=taus->begin();
	 tau!=taus->end(); ++tau)
      {
	bool matched=false;
	LorentzVector tLV(tau->px(),tau->py(),tau->pz(),tau->energy());
	if(doMatching_){
	  for(size_t i=0;i<m_tRefs->size();i++)
	    if(tau->isPFTau()&&ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.1||
	       tau->isCaloTau()&&ROOT::Math::VectorUtil::DeltaR(tLV,m_tRefs->at(i))<0.3)
	      matched = true;
	}
	else matched = true;
	
	if(matched&&tau->isPFTau()){
	  bool ePreId = tau->electronPreIDDecision();
	  double H3x3oP = tau->hcal3x3OverPLead();
	  double HoP = tau->hcalTotOverPLead();
	  double EoP = tau->ecalStripSumEOverPLead();
	  double HmaxoP = tau->hcalMaxOverPLead();
	  double emFrac = tau->emFraction();
	  hTauElecPreId->Fill(ePreId,eventWeight_);
	  hTauBremFracEoP->Fill(EoP,eventWeight_);
	  if(ePreId){
	    hTauEmFracVsEoP_ePreId->Fill(emFrac,EoP,eventWeight_);
	    hTauH3x3oPVsEoP_ePreId->Fill(H3x3oP,EoP,eventWeight_);
	  }
	  if(!ePreId){
	    hTauEmFracVsEoP_AePreId->Fill(emFrac,EoP,eventWeight_);
	    hTauH3x3oPVsEoP_AePreId->Fill(H3x3oP,EoP,eventWeight_);
	  }
	  hTauH3x3oP->Fill(H3x3oP,eventWeight_);
	  hTauEmFrac->Fill(emFrac,eventWeight_);
	  hTauHoP->Fill(HoP,eventWeight_);
	  hTauHmaxoP->Fill(HmaxoP,eventWeight_);
	}
	if(matched&&tau->isCaloTau()){
	  double ptLdgTrk = tau->leadTrack()->pt();
	  double H3x3oP = tau->leadTrackHCAL3x3hitsEtSum()/ptLdgTrk;
	  double HoP = tau->leadTrackHCAL3x3hitsEtSum();// No Such Method Yet
	  double HmaxoP = tau->maximumHCALhitEt()/ptLdgTrk;
	  double emFrac = tau->isolationECALhitsEtSum()/tau->leadTrackHCAL3x3hitsEtSum();// No Such Method Yet
	  hTauH3x3oP->Fill(H3x3oP,eventWeight_);
	  hTauEmFrac->Fill(emFrac,eventWeight_);
	  hTauHoP->Fill(HoP,eventWeight_);
	  hTauHmaxoP->Fill(HmaxoP,eventWeight_);
	}
      }
  }
}


