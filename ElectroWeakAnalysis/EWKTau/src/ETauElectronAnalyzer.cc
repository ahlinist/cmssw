#include "ElectroWeakAnalysis/EWKTau/interface/ETauElectronAnalyzer.h"

ETauElectronAnalyzer::ETauElectronAnalyzer(const edm::ParameterSet& cfg):
  elecs_(cfg.getParameter<VInputTag>("ElecInputTags")),
  refObjects_(cfg.getParameter<VInputTag>("RefInputTags")),
  n_elecs_max_(cfg.getParameter<unsigned int>("NelecsMax")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  doTuning_(cfg.getParameter<bool>("DoTuning")),
  doVeto_(cfg.getParameter<bool>("DoVeto")),
  eventWeight_(cfg.getParameter<double>("EventWeight"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
  hTrkIsoElec=fs->make<TH1F>("TrkIsoElec","TrkIsoElec",20,0,10);
  hEclIsoElec=fs->make<TH1F>("EclIsoElec","EclIsoElec",80,0,20);
  hHclIsoElec=fs->make<TH1F>("HclIsoElec","HclIsoElec",40,0,20);
  hEoPElec=fs->make<TH1F>("EoPElec","EoPElec",50,0,2);
  hDphiInElec=fs->make<TH1F>("DphiInElec","DphiInElec",100,-0.1,0.1);
  hDetaInElec=fs->make<TH1F>("DetaInElec","DetaInElec",100,-0.04,0.04);
  hHoEElec=fs->make<TH1F>("HoEElec","HoEElec",50,0,0.1);
  hSigEtaEtaElec=fs->make<TH1F>("SigEtaEtaElec","SigEtaEtaElec",20,0,1);

  std::vector<TString> elecHistoNames;
  elecHistoNames.push_back("PatL1Elec");
  elecHistoNames.push_back("SelKinElec");
  elecHistoNames.push_back("SelMchHLTElec");
  elecHistoNames.push_back("SelTrkIsoElec");
  elecHistoNames.push_back("SelEclIsoElec");
  elecHistoNames.push_back("SelHclIsoElec");
  elecHistoNames.push_back("SelIdElec");
  elecHistoNames.push_back("SelIpElecEt");
  m_elecEtHistoVec.reserve(elecHistoNames.size());
  m_elecEtaHistoVec.reserve(elecHistoNames.size());
  m_elecPhiHistoVec.reserve(elecHistoNames.size());
  for(size_t i=0;i<elecHistoNames.size();i++)
    {
      m_elecEtHistoVec[i]=fs->make<TH1F>(elecHistoNames[i]+"Et",
				       elecHistoNames[i]+"Et",100,0,200);
      m_elecEtaHistoVec[i]=fs->make<TH1F>(elecHistoNames[i]+"Eta",
				       elecHistoNames[i]+"Eta",60,-3,3);
      m_elecPhiHistoVec[i]=fs->make<TH1F>(elecHistoNames[i]+"Phi",
				       elecHistoNames[i]+"Phi",60,-3,3);
    }
  m_trkIsoHistoVec.reserve(10);
  m_eclIsoHistoVec.reserve(10);
  m_eclIsoHistoVec_Bar.reserve(10);
  m_eclIsoHistoVec_End.reserve(10);
  m_eclIsoHistoVecVsEta.reserve(10);
  m_hclIsoHistoVec.reserve(10);
  for(size_t i=0;i<10;i++)
    {
      TString name = "trkIsoVal";
      m_trkIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,20,0,10);
      name = "eclIsoVal";
      m_eclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,80,0,20);
      name = "eclBarIsoVal";
      m_eclIsoHistoVec_Bar[i]=fs->make<TH1F>(name+=i,name+=i,80,0,20);
      name = "eclEndIsoVal";
      m_eclIsoHistoVec_End[i]=fs->make<TH1F>(name+=i,name+=i,80,0,20);
      name = "eclIsoValVsEta";
      m_eclIsoHistoVecVsEta[i]=fs->make<TH2F>(name+=i,name+=i,80,0,30,60,-3.,3.);
      name = "hclIsoVal";
      m_hclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,40,0,20);
    }
  
  hSelHisto=fs->make<TH1F>("SelHisto","SelHisto",30,0,30);

}


ETauElectronAnalyzer::~ETauElectronAnalyzer()
{
}


void ETauElectronAnalyzer::analyze(const edm::Event& evt, const edm::EventSetup& setup)
{  
  m_count_sel=0;
  hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
  m_count_sel++; // Create separation between all evts and elec selections

  //Get the matching objs
  if(doMatching_)evt.getByLabel(refObjects_[0],m_eRefs);
  
  //Fill Elec Kin Histos
  for(size_t i=0;i<elecs_.size();i++)
    FillElecHists(evt,elecs_[i],m_elecEtHistoVec[i],
		  m_elecEtaHistoVec[i],m_elecPhiHistoVec[i]);
  
  //Fill Elec Iso/Id Histos
  if(doTuning_)FillElecIsoHistos(evt,elecs_[2]);
  
  //Look at Elec veto efficiency
  if(doVeto_){
    int n_e_sel = elecs_.size();
    edm::Handle<std::vector<pat::Electron> > final_coll;
    if(evt.getByLabel(elecs_[n_e_sel-1], final_coll))
      if(final_coll->size()&&final_coll->size()<=n_elecs_max_){
	m_count_sel++;
	hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
      }
  } 
  
}


void ETauElectronAnalyzer::beginJob(const edm::EventSetup&)
{
}

void ETauElectronAnalyzer::endJob()
{
}

void ETauElectronAnalyzer::FillElecHists(const edm::Event& evt,edm::InputTag tag,
				 TH1F* het,TH1F* heta,TH1F* hphi)
{
  edm::Handle<std::vector<pat::Electron> > elecs;
  bool matched = false;
  if(evt.getByLabel(tag, elecs)){
    for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();
	 elec!=elecs->end(); ++elec){    
      bool elec_matched=false;
      LorentzVector eLV(elec->px(),elec->py(),elec->pz(),elec->energy());
      if(doMatching_){
	for(size_t i=0;i<m_eRefs->size();i++)
	  if(ROOT::Math::VectorUtil::DeltaR(eLV,m_eRefs->at(i))<0.1)
	    elec_matched = true;  
      }
      else elec_matched=true;      
      
      if(elec_matched == true){
	het->Fill(elec->et(),eventWeight_);
	heta->Fill(elec->eta(),eventWeight_);
	hphi->Fill(elec->phi(),eventWeight_);
	matched = true;
      }
    }
  }
  m_count_sel++;
  if(matched==true)hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
}

void ETauElectronAnalyzer::FillElecIsoHistos(const edm::Event& evt,edm::InputTag tag)
{
  edm::Handle<std::vector<pat::Electron> > elecs;
  if(evt.getByLabel(tag, elecs)){
    for( std::vector<pat::Electron>::const_iterator elec=elecs->begin();
	 elec!=elecs->end(); ++elec){
      LorentzVector eLV(elec->px(),elec->py(),elec->pz(),elec->energy());
      bool match = false;
      if(doMatching_){
	for(size_t i=0;i<m_eRefs->size();i++)
	  if(ROOT::Math::VectorUtil::DeltaR(m_eRefs->at(i),eLV)<0.1)match=true;
      }
      else match = true;
      if(match){
	hTrkIsoElec->Fill(elec->trackIso(),eventWeight_);
	TuneIsoDeposit((*elec),"trk");
	if(elec->trackIso()<0.0001){
	  hEclIsoElec->Fill(elec->ecalIso(),eventWeight_);
	  TuneIsoDeposit((*elec),"ecl");
	  if(elec->ecalIso()<3.8){
	    hHclIsoElec->Fill(elec->hcalIso(),eventWeight_);
	    TuneIsoDeposit((*elec),"hcl");
	    if(elec->hcalIso()<3.5){
	      hEoPElec->Fill(elec->eSuperClusterOverP(),eventWeight_);
	      hDphiInElec->Fill(elec->deltaPhiSuperClusterTrackAtVtx(),eventWeight_);
	      hDetaInElec->Fill(elec->deltaEtaSuperClusterTrackAtVtx(),eventWeight_);
	      hHoEElec->Fill(elec->hadronicOverEm(),eventWeight_);
	      //hSigEtaEta->Fill(,eventWeight_); // Dont know how to access it yet
	    }
	  }
	}
      }
    }
  }
}

void ETauElectronAnalyzer::TuneIsoDeposit(const pat::Electron& elec,
					  std::string isoType)
{
  const reco::IsoDeposit* isoDep = 0;
  std::string veto_1 = "",veto_2 = "",veto_3="",veto_4="",veto_5="",veto_6="";
  reco::isodeposit::AbsVetos vetos;

  if(isoType=="trk"){
    isoDep=elec.trackerIsoDeposit();
    veto_1="0.02";
    veto_2="Threshold(1.0)";
    veto_3="0.0";
    veto_4="0.0";
    veto_5="0.0";
    veto_6="0.0";
  }
  else if(isoType=="ecl"){
    isoDep=elec.ecalIsoDeposit();
    veto_1="EcalBarrel:0.040";
    veto_2="EcalBarrel:RectangularEtaPhiVeto(-0.01,0.01,-0.5,0.5)";
    veto_3="EcalEndcaps:0.070";
    veto_4="EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)";
    veto_5="EcalBarrel:ThresholdFromTransverse(0.08)"; //For Future Pat
    veto_6="EcalEndcaps:ThresholdFromTransverse(0.3)"; //For Future Pat
  }
  else if(isoType=="hcl"){
    isoDep=elec.hcalIsoDeposit();
    veto_1="0.0";
    veto_2="0.0";
    veto_3="0.0";
    veto_4="0.0";
    veto_5="0.0";
    veto_6="0.0";
  }
  else {
    throw cms::Exception("Error")<<"Please Enter isoType = ecl,trk,hcl";
  }
    
  vetos.push_back(IsoDepositVetoFactory::make(veto_1.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_2.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_3.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_4.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_5.c_str()));
  vetos.push_back(IsoDepositVetoFactory::make(veto_6.c_str()));
  
  for(size_t i=0;i<vetos.size();i++)
    vetos[i]->centerOn(isoDep->eta(),isoDep->phi());
  
  for(int i=1;i<10;i++)
    {
      double isoVal=0.;
      if(isoType=="trk"){
	isoVal=isoDep->depositWithin(i*0.1,vetos,false);
	m_trkIsoHistoVec[i-1]->Fill(isoVal,eventWeight_);
      }
      else if(isoType=="ecl"){
	isoVal=isoDep->depositWithin(i*0.1,vetos,false);
	m_eclIsoHistoVec[i-1]->Fill(isoVal,eventWeight_);
	m_eclIsoHistoVecVsEta[i-1]->Fill(isoVal,elec.eta(),eventWeight_);
	if(fabs(elec.superCluster()->eta())<1.479)
	  m_eclIsoHistoVec_Bar[i-1]->Fill(isoVal,eventWeight_);
	if(fabs(elec.superCluster()->eta())>1.479)
	  m_eclIsoHistoVec_End[i-1]->Fill(isoVal,eventWeight_);
	
      }
      else if(isoType=="hcl"){
	isoVal=isoDep->depositWithin(i*0.1,vetos,false);
	m_hclIsoHistoVec[i-1]->Fill(isoVal,eventWeight_);
      }
    }
}
