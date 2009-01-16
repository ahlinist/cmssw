#include "ElectroWeakAnalysis/EWKTau/interface/ETauElectronAnalyzer.h"

ETauElectronAnalyzer::ETauElectronAnalyzer(const edm::ParameterSet& cfg):
  elecs_(cfg.getParameter<VInputTag>("ElecInputTags")),
  refObjects_(cfg.getParameter<edm::InputTag>("RefInputTags")),
  ebRecHits_(cfg.getParameter<edm::InputTag>("EBRecHitTag")),
  eeRecHits_(cfg.getParameter<edm::InputTag>("EERecHitTag")),
  vtxColl_(cfg.getParameter<edm::InputTag>("PVtag")),
  n_elecs_max_(cfg.getParameter<unsigned int>("NelecsMax")),
  doMatching_(cfg.getParameter<bool>("DoMatching")),
  doTuning_(cfg.getParameter<bool>("DoTuning")),
  doVeto_(cfg.getParameter<bool>("DoVeto")),
  trkIso_(cfg.getParameter<double>("TrkIsoVal")),
  eclIsoEB_(cfg.getParameter<double>("EclEBisoVal")),
  eclIsoEE_(cfg.getParameter<double>("EclEEisoVal")),
  hclIso_(cfg.getParameter<double>("HclIsoVal")),
  eId_(cfg.getParameter<std::string>("ElectronID")),
  trk_isoParameters_(cfg.getParameter<std::vector<std::string> >("TrkIsoParams")),
  ecl_isoParameters_(cfg.getParameter<std::vector<std::string> >("EclIsoParams")),
  hcl_isoParameters_(cfg.getParameter<std::vector<std::string> >("HclIsoParams")),
  eventWeight_(cfg.getParameter<double>("EventWeight"))
{
  m_count_sel=0;
  edm::Service<TFileService> fs;
  hTrkIsoElec=fs->make<TH1F>("TrkIsoElec","TrkIsoElec",60,0,20);
  hEclIsoElec=fs->make<TH1F>("EclIsoElec","EclIsoElec",160,0,40);
  hHclIsoElec=fs->make<TH1F>("HclIsoElec","HclIsoElec",80,0,40);
  
  hEoPElecBar=fs->make<TH1F>("EoPElecBar","EoPElecBar",50,0,2);
  hDphiInElecBar=fs->make<TH1F>("DphiInElecBar","DphiInElecBar",100,-0.1,0.1);
  hDetaInElecBar=fs->make<TH1F>("DetaInElecBar","DetaInElecBar",100,-0.04,0.04);
  hHoEElecBar=fs->make<TH1F>("HoEElecBar","HoEElecBar",50,0,0.1);
  hSigEtaEtaElecBar=fs->make<TH1F>("SigEtaEtaElecBar","SigEtaEtaElecBar",50,0,0.05);
  
  hEoPElecEnd=fs->make<TH1F>("EoPElecEnd","EoPElecEnd",50,0,2);
  hDphiInElecEnd=fs->make<TH1F>("DphiInElecEnd","DphiInElecEnd",100,-0.1,0.1);
  hDetaInElecEnd=fs->make<TH1F>("DetaInElecEnd","DetaInElecEnd",100,-0.04,0.04);
  hHoEElecEnd=fs->make<TH1F>("HoEElecEnd","HoEElecEnd",50,0,0.1);
  hSigEtaEtaElecEnd=fs->make<TH1F>("SigEtaEtaElecEnd","SigEtaEtaElecEnd",50,0,0.05);
  
  hIpElec=fs->make<TH1F>("IpElec","IpElec",50,-0.1,0.1);
  hElecConvDR=fs->make<TH1F>("ElecConvDR","ElecConvDR",100,0,5);
  hElecConvDRisConv=fs->make<TH1F>("ElecConvDRisConv","ElecConvDRisConv",
				   100,0,5);
  hElecConvVertPerp=fs->make<TH1F>("ElecConvVertPerp","ElecConvVertPerp",
				   360,0,120);
  hElecConvVertZ=fs->make<TH1F>("ElecConvVertZ","ElecConvVertZ",
				   100,-100,100);
  hElecConvNtracks=fs->make<TH1F>("ElecConvNtracks","ElecConvNtracks",
				   5,0,5);
  hMTracksElec=fs->make<TH1F>("MTracksElec","MTracksElec",100,0,2);
  hDRinTracksElec=fs->make<TH1F>("DRinTracksElec","DRinTracksElec",100,0,1);
  hDZinTracksElec=fs->make<TH1F>("DZinTracksElec","DZinTracksElec",100,-0.3,0.3);
  hDRinVsMTracksElec=fs->make<TH2F>("DRinVsMTracksElec","DRinVsMTracksElec",
				    100,0,1,500,0,5);
  hCotThetaTracksElec=fs->make<TH1F>("CotThetaTracksElec","CotThetaTracksElec",40,-0.08,0.08);
  hDcaTracksElec=fs->make<TH1F>("DcaTracksElec","DcaTracksElec",40,-2.5,1.5);
  hDcaVsCotThetaTrackElec=fs->make<TH2F>("DcaVsCotThetaTrackElec","DcaVsCotThetaTrackElec",
					 40,-0.08,0.08,40,-2.5,1.5);
  hnTracksPass=fs->make<TH1F>("nTracksPass","nTracksPass",10,0,10);
  hnTracksPassDca=fs->make<TH1F>("nTracksPassDca","nTracksPassDca",10,0,10);
  hTracksPt=fs->make<TH1F>("TracksPt","TracksPt",30,0,10);
  hTracksPtDca=fs->make<TH1F>("TracksPtDca","TracksPtDca",30,0,10);
  hElecPtVsTracksPt=fs->make<TH2F>("ElecPtVsTracksPt","ElecPtVsTracksPt",50,0,100,50,0,100);
  std::vector<TString> elecHistoNames;

  elecHistoNames.push_back("PatL1Elec");
  elecHistoNames.push_back("SelKinElec");
  elecHistoNames.push_back("SelMchHLTElec");
  elecHistoNames.push_back("SelTrkIsoElec");
  elecHistoNames.push_back("SelEclIsoElec");
  elecHistoNames.push_back("SelHclIsoElec");
  elecHistoNames.push_back("SelIdElec");
  elecHistoNames.push_back("SelIpElec");
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
      m_trkIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,60,0,20);
      name = "eclIsoVal";
      m_eclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,160,0,40);
      name = "eclBarIsoVal";
      m_eclIsoHistoVec_Bar[i]=fs->make<TH1F>(name+=i,name+=i,160,0,40);
      name = "eclEndIsoVal";
      m_eclIsoHistoVec_End[i]=fs->make<TH1F>(name+=i,name+=i,160,0,40);
      name = "eclIsoValVsEta";
      m_eclIsoHistoVecVsEta[i]=fs->make<TH2F>(name+=i,name+=i,160,0,40,60,-3.,3.);
      name = "hclIsoVal";
      m_hclIsoHistoVec[i]=fs->make<TH1F>(name+=i,name+=i,80,0,40);
    }
  
  hSelHisto=fs->make<TH1F>("SelHisto","SelHisto",30,0,30);

}


ETauElectronAnalyzer::~ETauElectronAnalyzer()
{
}


void ETauElectronAnalyzer::analyze(const edm::Event& evt, 
				   const edm::EventSetup& setup)
{  
 
  m_count_sel=0;
  hSelHisto->Fill(m_count_sel+0.5,eventWeight_);
  m_count_sel++; // Create separation between all evts and elec selections

  //Get the matching objs
  if(doMatching_)evt.getByLabel(refObjects_,m_eRefs);
  
  //Fill Elec Kin Histos
  for(size_t i=0;i<elecs_.size();i++)
    FillElecHists(evt,elecs_[i],m_elecEtHistoVec[i],
		  m_elecEtaHistoVec[i],m_elecPhiHistoVec[i]);
  
  //Fill Elec Iso/Id Histos
  if(doTuning_)FillElecIsoHistos(evt,setup,elecs_[2]);
  
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

void ETauElectronAnalyzer::FillElecIsoHistos(const edm::Event& evt,
					     const edm::EventSetup& setup,
					     edm::InputTag tag)
{
  
  // Standard primary vertex collection
  edm::Handle<reco::VertexCollection> thePVs;
  evt.getByLabel(vtxColl_, thePVs);
  const reco::VertexCollection vertCollection=*(thePVs.product());
  reco::Vertex thePV;
  thePV=*(vertCollection.begin());

  EcalClusterLazyTools lazyTools(evt,setup,ebRecHits_,eeRecHits_);
  
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
	if(elec->trackIso()<trkIso_){
	  hEclIsoElec->Fill(elec->ecalIso(),eventWeight_);
	  TuneIsoDeposit((*elec),"ecl");
	  if((fabs(elec->superCluster()->eta())<1.479&&elec->ecalIso()<eclIsoEB_)||
	     (fabs(elec->superCluster()->eta())>1.479&&elec->ecalIso()<eclIsoEE_)){
	    hHclIsoElec->Fill(elec->hcalIso(),eventWeight_);
	    TuneIsoDeposit((*elec),"hcl");
	    if(elec->hcalIso()<hclIso_){
	      
	      std::vector<float> vCov = 
		lazyTools.covariances(*(elec->superCluster()->seed()));
	      double sigmaee = sqrt(vCov[0]);
	      
	      if(fabs(elec->superCluster()->eta())<1.479){
		hDphiInElecBar->Fill(elec->deltaPhiSuperClusterTrackAtVtx(),
				     eventWeight_);
		hDetaInElecBar->Fill(elec->deltaEtaSuperClusterTrackAtVtx(),
				     eventWeight_);
		hHoEElecBar->Fill(elec->hadronicOverEm(),eventWeight_);
		hSigEtaEtaElecBar->Fill(sigmaee,eventWeight_);
	      }
	      
	      if(fabs(elec->superCluster()->eta())>1.479){
		hDphiInElecEnd->Fill(elec->deltaPhiSuperClusterTrackAtVtx(),
				     eventWeight_);
		hDetaInElecEnd->Fill(elec->deltaEtaSuperClusterTrackAtVtx(),
				     eventWeight_);
		hHoEElecEnd->Fill(elec->hadronicOverEm(),eventWeight_);
		hSigEtaEtaElecEnd->Fill(sigmaee-0.02*(elec->p4().Eta()-2.3),eventWeight_);//Correct for sigee dependance on eta
	      }
	      
	      if(elec->electronID("robust")>0){
		
		if(fabs(elec->superCluster()->eta())<1.479)
		  hEoPElecBar->Fill(elec->eSuperClusterOverP(),eventWeight_);
		if(fabs(elec->superCluster()->eta())>1.479)
		  hEoPElecEnd->Fill(elec->eSuperClusterOverP(),eventWeight_);
		
		hIpElec->Fill(elec->gsfTrack()->dxy(thePV.position()),eventWeight_);
		if((fabs(elec->superCluster()->eta())<1.479&&elec->eSuperClusterOverP()>0.95&&elec->eSuperClusterOverP()<1.05)||
		   (fabs(elec->superCluster()->eta())>1.479&&elec->eSuperClusterOverP()>0.95&&elec->eSuperClusterOverP()<1.12))
		  SearchForConversions(evt,setup,(*elec));
	      }
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
    veto_1=trk_isoParameters_[0];//"0.02";
    veto_2=trk_isoParameters_[1];//"Threshold(1.0)";
    veto_3="0.0";
    veto_4="0.0";
    veto_5="0.0";
    veto_6="0.0";
  }
  else if(isoType=="ecl"){
    isoDep=elec.ecalIsoDeposit();
    veto_1=ecl_isoParameters_[0];//"EcalBarrel:0.040";
    veto_2=ecl_isoParameters_[1];//"EcalBarrel:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)";
    veto_3=ecl_isoParameters_[2];//"EcalEndcaps:0.070";
    veto_4=ecl_isoParameters_[3];//"EcalEndcaps:RectangularEtaPhiVeto(-0.02,0.02,-0.5,0.5)";
    veto_5=ecl_isoParameters_[4];//"EcalBarrel:ThresholdFromTransverse(0.08)"; //For Future Pat
    veto_6=ecl_isoParameters_[5];//"EcalEndcaps:ThresholdFromTransverse(0.3)"; //For Future Pat
  }
  else if(isoType=="hcl"){
    isoDep=elec.hcalIsoDeposit();
    veto_1=hcl_isoParameters_[0];//"0.0";
    veto_2=hcl_isoParameters_[1];//"0.0";
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

void ETauElectronAnalyzer::SearchForConversions(const edm::Event& evt,
						const edm::EventSetup& setup,
						const pat::Electron& elec)
{
  edm::Handle<reco::ConversionCollection> convertedPhotonHandle; 
  if(evt.getByLabel("conversions",convertedPhotonHandle)){
    const reco::ConversionCollection phoCollection = *(convertedPhotonHandle.product());
    reco::ConversionCollection::const_iterator  iPho = phoCollection.begin();
    double dRmin=100000.;
    float conVertX=0.,conVertY,conVertZ=0.,conVertPerp=0.;
    int conNtracks=0;
    for(;iPho != phoCollection.end(); iPho++) {
      if((*iPho).isConverted()){
   
	float phiVert=0,etaVert=0,ptVert=0;
	
	phiVert=(*iPho).caloCluster()[0]->phi();
	etaVert=(*iPho).caloCluster()[0]->eta();
	ptVert=(*iPho).caloCluster()[0]->energy();

	float deltaPhi=fabs(phiVert-elec.gsfTrack()->phi());
	if(deltaPhi>acos(-1.))deltaPhi=2*acos(-1.)-deltaPhi;
	float deltaEta=fabs(etaVert-elec.gsfTrack()->eta());
	float deltaR = sqrt(deltaEta*deltaEta+deltaPhi*deltaPhi);
	hElecConvDR->Fill(deltaR,eventWeight_);
	if(deltaR<dRmin&&ptVert>5){
	  dRmin=deltaR;
	  conVertX=(*iPho).conversionVertex().x();
	  conVertY=(*iPho).conversionVertex().y();
	  conVertZ=(*iPho).conversionVertex().z();
	  conVertPerp = sqrt(conVertX*conVertX+conVertY*conVertY);
	  conNtracks = (*iPho).nTracks();
	}
      }
    }
    
    if(dRmin!=100000.){
      hElecConvDRisConv->Fill(dRmin,eventWeight_);
      hElecConvVertPerp->Fill(conVertPerp,eventWeight_);
      hElecConvVertZ->Fill(conVertZ,eventWeight_);
      hElecConvNtracks->Fill(conNtracks,eventWeight_);
    }
    
  }else{std::cout<<"WRONG CONVERSION COLLECTION"<<std::endl;}
  
  //get the tracks
  edm::Handle<reco::TrackCollection> tracks;
  evt.getByLabel("generalTracks",tracks);
  const reco::TrackCollection* trackCollection = tracks.product();
  

  edm::ESHandle<MagneticField> magFieldHandle;
  setup.get<IdealMagneticFieldRecord>().get( magFieldHandle );
  const MagneticField* magField = magFieldHandle.product();

  //ROOT::Math::VectorUtil::DeltaR(m_eRefs->at(i),eLV)
  int nTracksPass=0,nTracksPassDca=0;double fastPt=-1.;
  for(size_t t=0;t<trackCollection->size();t++)
    {
      double dRin=ROOT::Math::VectorUtil::DeltaR(trackCollection->at(t).innerPosition(),
					       elec.gsfTrack()->innerPosition());
      double dZin= trackCollection->at(t).innerPosition().z()-
	elec.gsfTrack()->innerPosition().z();
      TLorentzVector trackP,elecP;
      trackP.SetPtEtaPhiM(trackCollection->at(t).pt(),
			  trackCollection->at(t).eta(),
			  trackCollection->at(t).phi(),
			  0.);
      elecP.SetPtEtaPhiM(elec.pt(),
			 elec.eta(),
			 elec.phi(),
			 0.);
      double mTracksElec = (trackP+elecP).M();
      double dca=GetDcaBetweenTracks(evt,setup,magField,trackCollection->at(t),*(elec.gsfTrack()));
      double cotTheta=1./tan(elec.gsfTrack()->innerMomentum().Theta())-1./tan(trackCollection->at(t).innerMomentum().Theta());

      if(elecP.DeltaR(trackP)<0.05&&trackP.Pt()>fastPt)
	fastPt=trackP.Pt();
	
      if(trackP.Pt()<10){	
	hMTracksElec->Fill(mTracksElec);
	hDRinTracksElec->Fill(dRin,eventWeight_);
	hDZinTracksElec->Fill(dZin,eventWeight_);
	hDRinVsMTracksElec->Fill(dRin,mTracksElec,eventWeight_);
	hDcaTracksElec->Fill(dca,eventWeight_);
	hCotThetaTracksElec->Fill(dca,eventWeight_);
	hDcaVsCotThetaTrackElec->Fill(cotTheta,dca,eventWeight_);
	if(mTracksElec<0.75&&dRin<0.1){
	  hTracksPt->Fill(trackP.Pt(),eventWeight_);
	  nTracksPass++;
	}
	if(dca<0.2&&dca>-0.1&&fabs(cotTheta)<0.02){
	  hTracksPtDca->Fill(trackP.Pt(),eventWeight_);
	  nTracksPassDca++;
	}
      
      }
    }
  hnTracksPass->Fill(nTracksPass,eventWeight_);
  hnTracksPassDca->Fill(nTracksPassDca,eventWeight_);
  if(fastPt>-1.)
    hElecPtVsTracksPt->Fill(elec.pt(),fastPt,eventWeight_);
}
 
double ETauElectronAnalyzer::GetDcaBetweenTracks(const edm::Event& evt, 
						 const edm::EventSetup& setup,
						 const MagneticField* magField,
						 const reco::Track& trk,
						 const reco::GsfTrack& gsf)
{
  
  double gx=gsf.innerPosition().x(),gy=gsf.innerPosition().y(),gz=gsf.innerPosition().z();
  double rad_gsf = gsf.innerMomentum().Rho()/(0.3*magField->inTesla(GlobalPoint(gx,gy,gz)).z())*100.;
  
  double tx=gsf.innerPosition().x(),ty=trk.innerPosition().y(),tz=trk.innerPosition().z();
  double rad_trk = trk.innerMomentum().Rho()/(0.3*magField->inTesla(GlobalPoint(tx,ty,tz)).z())*100.;
  
  double x0_g = gx+rad_gsf*sin(gsf.innerMomentum().phi())*gsf.charge();
  double y0_g = gy-rad_gsf*cos(gsf.innerMomentum().phi())*gsf.charge();

  double x0_t = tx+rad_trk*sin(trk.innerMomentum().phi())*trk.charge();
  double y0_t = ty-rad_trk*cos(trk.innerMomentum().phi())*trk.charge();
  
  double dca = sqrt((x0_t-x0_g)*(x0_t-x0_g)+(y0_t-y0_g)*(y0_t-y0_g))-rad_gsf-rad_trk;

  return dca;
}
