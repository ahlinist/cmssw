#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoObjectQualityAnalyzer.h"

using namespace edm;
using namespace reco;
using namespace std;  

ZETauRecoObjectQualityAnalyzer::ZETauRecoObjectQualityAnalyzer(const edm::ParameterSet& iConfig):
								   m_mcTauProducts(iConfig.getParameter<VInputTag>("mcTauProducts")),
								   m_Output(iConfig.getUntrackedParameter<std::string>("Output","Comparison.root")),
								   m_signal(iConfig.getParameter<bool>("RunSignal")),
                                                                   m_zee(iConfig.getParameter<bool>("RunZee"))
{                                                             
  std::string treeString="DataTree";
  const char* treeChar=treeString.c_str();
  const char* output=m_Output.c_str();
  
  thefile = new TFile(output,"recreate");
  thefile->cd();
  smalltree= new TTree(treeChar,treeChar);  
  nEvt=0;
  Count_0=0;
  Count=0;
  InitializeHistograms(); 
  InialiseResolutionPlots();
 
  
}

ZETauRecoObjectQualityAnalyzer::~ZETauRecoObjectQualityAnalyzer()
{
 
   
}

void ZETauRecoObjectQualityAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if((nEvt%10==0 && nEvt<=100)||(nEvt%100==0 && nEvt>100)||(nEvt%1000==0 && nEvt>1000))
    std::cout << "reading event " << nEvt << std::endl;
  nEvt++;

  edm::ESHandle<MagneticField> field;
  iSetup.get<IdealMagneticFieldRecord>().get(field);
  edm::ESHandle<Propagator> propagator;
  iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", propagator);

  Handle<LorentzVectorCollection> MCinfo;
  TLorentzVector mcTauJet,mcLeptonFromTau,mcNeutrina;
  int j=0;
  for(vector<edm::InputTag>::const_iterator t = m_mcTauProducts.begin(); t != m_mcTauProducts.end(); ++ t )
    {
      try
	{
	  j++;
	  iEvent.getByLabel(*t,MCinfo);
	  LorentzVectorCollection::const_iterator imc=MCinfo->begin();
	  for(;imc!=MCinfo->end();++imc)
	    {
	      if(j==1)mcTauJet.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	      if(j==2)mcLeptonFromTau.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	      if(j==3)mcNeutrina.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	    }
	
	}catch(...){;}
    }

  Handle<IsolatedTauTagInfoCollection> tautagHandle;
  Handle<CandidateCollection> genCandidateHandle;
  
  try{
    iEvent.getByLabel("genParticleCandidates",genCandidateHandle);
    iEvent.getByLabel("coneIsolationTauJetTags",tautagHandle);
        
    Handle<CandidateCollection> electronHandle;
    iEvent.getByLabel("allElectrons",electronHandle); 
    
    double m_Rm=0.1, m_Riso=0.45, m_Rsig=0.07, m_Ptmin_lt=6;
    vector<TLorentzVector> eFromZVec;
    if(m_zee)eFromZVec=ElectronsFromZee(genCandidateHandle);
    ElectronRejectionVariables(tautagHandle,mcTauJet,eFromZVec,m_Rm,m_Riso,m_Rsig,m_Ptmin_lt,20,m_signal,m_zee,*field,*propagator);
    
  }catch(...){;}

  if(m_signal)MakeResolutionPlots(iEvent,iSetup); 
  smalltree->Fill();
}

void ZETauRecoObjectQualityAnalyzer::beginJob(const edm::EventSetup&)
{
}

void ZETauRecoObjectQualityAnalyzer::endJob() {

  SetupHistos();

  std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "analyzed " << nEvt << " events " << "::"<<std::endl;
  std::cout<<  "Events Before Eta Cracks " <<Count_0<<" events "<<" Events After Eta Cracks "<<Count<<" events "<<std::endl;
  std::cout << "writing information into file: " << thefile->GetName() << std::endl;


  thefile->Write();
  thefile->Close();
    
}

void ZETauRecoObjectQualityAnalyzer::InitializeHistograms()
{
  hOutTrPhiVsEta=new TH2D("OutTrPhiVsEta","Outer Ldg Track Phi Vs Eta;Phi;Eta",4000,0,400,3000,0,3);
  hOutTrMaxEdR=new TH1D("OutTrMaxEdR","DR Outer Ldg Track - Hot ECal Tower",50,0,2);

  hHhot=new TH1D("Hhot","Max E_{T} Hcal Tower",60,0,60);
  hEhot=new TH1D("Ehot","Max E_{T} Ecal Tower",60,0,60);

  hHhot->Sumw2();
  hEhot->Sumw2();
  
  

  hHhotOverHtot=new TH1D("HhotOverHtot","Hhot Over Htot",50,0,1.5);
  hEhotOverEtot=new TH1D("EhotOverEtot","Ehot Over Etot",50,0,1.5);

  hHtotOverLdgPt=new TH1D("HtotOverLdgPt","Htot Over LdgPt;#frac{Hcal_{tot}}{P_{T}^{LdgTr}}",60,0,6);
  hEtotOverLdgPt=new TH1D("EtotOverLdgPt","Etot Over LdgPt;#frac{Ecal_{tot}}{P_{T}^{LdgTr}}",60,0,16);

  hHtotOverLdgPt->Sumw2();
  hEtotOverLdgPt->Sumw2();
  
  hHhotOverLdgPt=new TH1D("HhotOverLdgPt","Hhot Over LdgPt;#frac{Hcal_{hot}}{P_{T}^{LdgTr}}",60,0,6);
  hEhotOverLdgPt=new TH1D("EhotOverLdgPt","Ehot Over LdgPt;#frac{Ecal_{hot}}{P_{T}^{LdgTr}}",60,0,10);

  hHhotOLdgPtVsLdgPt=new TH2D("HhotOverLdgPtVsLdgPt","Hhot Over LdgPt Vs LdgPt;#frac{Hcal_{hot}}{P_{T}^{LdgTr}};P_{T}^{Ltr} [GeV]",60,0,6,20,0,20);
  hEhotOLdgPtVsLdgPt=new TH2D("EhotOverLdgPtVsLdgPt","Ehot Over LdgPt Vs LdgPt;#frac{Ecal_{hot}}{P_{T}^{LdgTr}};P_{T}^{Ltr} [GeV]",60,0,10,20,0,20);
	
  hHhotOLdgPtVsHhot=new TH2D("HhotOLdgPtVsHhot","Hhot Over LdgPt Vs Hhot;#frac{Hcal_{hot}}{P_{T}^{LdgTr}};Hcal_{hot} [GeV]",60,0,6,60,0,60);
  hEhotOLdgPtVsEhot=new TH2D("EhotOLdgPtVsEhot","Ehot Over LdgPt Vs Ehot;#frac{Ecal_{hot}}{P_{T}^{LdgTr}};Ecal_{hot} [GeV]",60,0,10,60,0,60);
	
  hHtotOLdgPtVsLdgPt=new TH2D("HtotOLdgPtVsLdgPt","Htot Over LdgPt Vs LdgPt;#frac{Hcal_{tot}}{P_{T}^{LdgTr}};P_{T}^{Ltr} [GeV]",60,0,6,20,0,20);
  hEtotOLdgPtVsLdgPt=new TH2D("EtotOLdgPtVsLdgPt","Etot Over LdgPt Vs LdgPt;#frac{Ecal_{tot}}{P_{T}^{LdgTr}};P_{T}^{Ltr} [GeV]",60,0,16,20,0,20);
	
  hHtotOLdgPtVsHtot=new TH2D("HtotOLdgPtVsHtot","Htot Over LdgPt Vs Htot;#frac{Hcal_{tot}}{P_{T}^{LdgTr}};Hcal_{tot} [GeV]",60,0,16,80,0,80);
  hEtotOLdgPtVsEtot=new TH2D("EtotOLdgPtVsEtot","Etot Over LdgPt Vs Etot;#frac{Ecal_{tot}}{P_{T}^{LdgTr}};Ecal_{tot} [GeV]",60,0,16,80,0,80);

  hEhotOLdgPtVsHhot=new TH2D("EhotOLdgPtVsHhot","Ehot Over LdgPt Vs Hhot;#frac{Ecal_{hot}}{P_{T}^{LdgTr}};Hcal_{hot} [GeV]",10,0,5,20,0,10);
  
  hHtotOLdgPtVsOutTrHhotDEta=new TH2D("HtotOLdgPtVsOutTrHhotDEta","HtotOLdgPt Vs ;#frac{Hcal_{tot}}{P_{T}^{LdgTr}};#Delta#eta LdgTr at Ecal - Hhot",60,0,6,60,0,0.6);

  hHtotOLdgPtVsOutTrHhotDR=new TH2D("HtotOLdgPtVsOutTrHhotDR","HtotOLdgPt Vs ;#frac{Hcal_{tot}}{P_{T}^{LdgTr}};#DeltaR LdgTr at Ecal - Hhot",60,0,6,60,0,0.6);
 
  hOutTrHhotDR=new TH1D("OutTrHhotDR","OutTr-Hhot #DeltaR",60,0,.6);
  hOutTrHhotDEta=new TH1D("OutTrHhotDEta","OutTr-Hhot #Delta#eta",60,0,.6);
  hOutTrHhotDRPassesDEta=new TH1D("OutTrHhotDRPassesDEta","OutTrHhotDRPassesDEta",60,0,.6);
}


void ZETauRecoObjectQualityAnalyzer::SetupHistos()
{
  double scale=1./Count_0;

  hOutTrMaxEdR->Scale(scale);
  hHhot->Scale(scale);
  hEhot->Scale(scale);
  hHhotOverHtot->Scale(scale);
  hEhotOverEtot->Scale(scale);
  hHhotOverLdgPt->Scale(scale);
  hHtotOverLdgPt->Scale(scale);
  hEhotOverLdgPt->Scale(scale);
  hEtotOverLdgPt->Scale(scale);
  hEhotOLdgPtVsHhot->Scale(scale);  
  hOutTrHhotDR->Scale(scale);
  hOutTrHhotDEta->Scale(scale);
  hOutTrHhotDRPassesDEta->Scale(scale);
  /*
  hOutTrMaxEdR->Scale(1/(1.*(hOutTrMaxEdR->GetEntries())));
  hHhot->Scale(1/(1.*(hHhot->GetEntries())));
  hEhot->Scale(1/(1.*(hEhot->GetEntries())));
  hHhotOverHtot->Scale(1/(1.*(hHhotOverHtot->GetEntries())));
  hEhotOverEtot->Scale(1/(1.*(hEhotOverEtot->GetEntries())));
  hHhotOverLdgPt->Scale(1/(1.*(hHhotOverLdgPt->GetEntries())));
  hHtotOverLdgPt->Scale(1/(1.*(hHtotOverLdgPt->GetEntries())));
  hEhotOverLdgPt->Scale(1/(1.*(hEhotOverLdgPt->GetEntries())));
  hEtotOverLdgPt->Scale(1/(1.*(hEtotOverLdgPt->GetEntries())));
  hEhotOLdgPtVsHhot->Scale(1/(1.*hEhotOLdgPtVsHhot->GetEntries()));  
  */

  //hHtotOLdgPtVsOutTrHhotDEta->Scale(1./(hHtotOLdgPtVsOutTrHhotDEta->GetEntries()));
  //hHtotOLdgPtVsOutTrHhotDR->Scale(1./(hHtotOLdgPtVsOutTrHhotDR->GetEntries()));
  

  if(m_signal){
    hOutTrMaxEdR->SetLineStyle(2);
    hOutTrMaxEdR->SetLineWidth(3);
    hHhot->SetLineStyle(2);
    hHhot->SetLineWidth(3);
    hEhot->SetLineStyle(2);
    hEhot->SetLineWidth(3);
    hHhotOverHtot->SetLineStyle(2);
    hHhotOverHtot->SetLineWidth(3);
    hEhotOverEtot->SetLineStyle(2);
    hEhotOverEtot->SetLineWidth(3);
    hHhotOverLdgPt->SetLineStyle(2);
    hHhotOverLdgPt->SetLineWidth(3);
    hHtotOverLdgPt->SetLineStyle(2);
    hHtotOverLdgPt->SetLineWidth(3);
    hEhotOverLdgPt->SetLineStyle(2);
    hEhotOverLdgPt->SetLineWidth(3);
    hEtotOverLdgPt->SetLineStyle(2);
    hEtotOverLdgPt->SetLineWidth(3);
    hOutTrHhotDR->SetLineStyle(2);
    hOutTrHhotDR->SetLineWidth(3);
    hOutTrHhotDEta->SetLineStyle(2);
    hOutTrHhotDEta->SetLineWidth(3);
    hOutTrHhotDRPassesDEta->SetLineStyle(2);
    hOutTrHhotDRPassesDEta->SetLineWidth(3);
  }
}

vector<TLorentzVector> ZETauRecoObjectQualityAnalyzer::ElectronsFromZee(Handle<CandidateCollection>& genCandidateHandle)
{
  vector<TLorentzVector> eFromZ;
  for(size_t mcount=0;mcount!=genCandidateHandle->size();++mcount){
    const Candidate& p = (*genCandidateHandle)[mcount];
    if(p.pdgId()==23&&p.status()==3){
      unsigned int n=p.numberOfDaughters();
      for(size_t mccountd=0;mccountd<n;++mccountd){
	const Candidate* z=p.daughter(mccountd);
	if(fabs(z->pdgId())==11){
	  unsigned int nn=z->numberOfDaughters();
	  for(size_t mccountdd=0;mccountdd<nn;++mccountdd){
	    const Candidate* zz=z->daughter(mccountdd);
	    if(fabs(zz->pdgId())==11&&zz->status()==1){
	      TLorentzVector elec(zz->px(),zz->py(),zz->pz(),zz->energy());
	      eFromZ.push_back(elec);
	    }
	  }
	}
      }
    }
  }
  return eFromZ;
}


void ZETauRecoObjectQualityAnalyzer::ElectronRejectionVariables(Handle<IsolatedTauTagInfoCollection>& tautagHandle,TLorentzVector tauJetMc,vector<TLorentzVector> eFromZ,
							     double Rm,double Riso,double Rsig, double PtLtr,double etcut,bool runOnSignal,bool runOnZee,
							     const MagneticField& field,const Propagator& propagator)      
{
 
  IsolatedTauTagInfoCollection::const_iterator tauit = tautagHandle->begin();  
  for(;tauit!=tautagHandle->end();++tauit)
    {
      TLorentzVector tau(tauit->jet().get()->px(),tauit->jet().get()->py(),
			 tauit->jet().get()->pz(),tauit->jet().get()->energy());
      
      bool mc_match=false;
      if(runOnZee&&!runOnSignal){
	for(unsigned int i=0;i<eFromZ.size();++i)
	  {
	    if(eFromZ[i].DeltaR(tau)<0.3)mc_match=true;
	  }
      }

      else if(!runOnZee&&runOnSignal){
	double dphi = fabs(tauit->jet().get()->phi()-tauJetMc.Phi());
	if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
	double deta=fabs(tauit->jet().get()->eta()-tauJetMc.Eta());
	double dR=sqrt(dphi*dphi+deta*deta);
	if(dR<0.3)mc_match=true;
      }
      else mc_match=true;
      

      if(mc_match&&tauit->jet().get()->et()>etcut/*&&
	 tauit->discriminator(Rm,Riso,Rsig,PtLtr,1.,0,0.2)*/){
	
	const reco::TrackRef leadTk = tauit->leadingSignalTrack(Rm,PtLtr);

	if(!leadTk)continue;
	double ldgPt = leadTk->pt();
	GlobalPoint trackAtEcal=PropagateTrackToCalo(*leadTk,field,propagator);
	double ldgPtOuterPhi=trackAtEcal.phi().value();
	double ldgPtOuterPhiDeg=trackAtEcal.phi().degrees();
	if(ldgPtOuterPhiDeg<0.)ldgPtOuterPhiDeg=360-fabs(ldgPtOuterPhiDeg);
	double ldgPtOuterEta=trackAtEcal.eta();

	Count_0++;
	double eta_gaps[]={0.435,0.783,1.131,1.479};
	
	const reco::CaloJet* cj = dynamic_cast<reco::CaloJet*>(const_cast<reco::Jet*>((tauit->jet().get())));
	const vector<CaloTowerRef> TauJetTowers = cj->getConstituents();
	double HCalTowerEtMax = -10.,ECalTowerEtMax = -10.;
	double HCalTowerEtMaxEta=0.,HCalTowerEtMaxPhi=0.;
	double ECalTowerEtMaxEta=0.,ECalTowerEtMaxPhi=0.;
	double HCalTotalEt=0.,ECalTotalEt=0.;
	double HCal3x3Et=0;
	for(unsigned int iTower = 0; iTower < TauJetTowers.size(); iTower++) {
	  CaloTowerRef t = TauJetTowers[iTower];
	  HCalTotalEt+=t->hadEt();
	  ECalTotalEt+=t->emEt();
	  double deta=fabs(ldgPtOuterEta-t->eta());double dphi=fabs(ldgPtOuterPhi-t->phi());
	  if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
	  if(sqrt(dphi*dphi+deta*deta)<0.184){
	    HCal3x3Et+=t->hadEt();
	    if( t->hadEt() >= HCalTowerEtMax ){
	      HCalTowerEtMax = t->hadEt();
	      HCalTowerEtMaxEta = t->eta();
	      HCalTowerEtMaxPhi=t->phi();
	    }
	  }
	  /*
	  if( t->hadEt() >= HCalTowerEtMax ){
	    HCalTowerEtMaxEta=t->eta();HCalTowerEtMax = t->hadEt();
	    HCalTowerEtMaxPhi=t->phi();
	    
	  }
	  */
	  if(t->emEt() >= ECalTowerEtMax){
	    ECalTowerEtMax=t->emEt();ECalTowerEtMaxEta=t->eta();
	    ECalTowerEtMaxPhi=t->phi();
	  }
	}

	double OutTrMaxEdPhi=fabs(ldgPtOuterPhi-ECalTowerEtMaxPhi);
	if(OutTrMaxEdPhi>acos(-1.))OutTrMaxEdPhi=2*acos(-1.)-OutTrMaxEdPhi;
	double OutTrMaxEdEta=fabs(ldgPtOuterEta-ECalTowerEtMaxEta);
	double OutTrMaxEdR=sqrt(OutTrMaxEdPhi*OutTrMaxEdPhi+OutTrMaxEdEta*OutTrMaxEdEta);

	double HtotOverLdgPt=HCalTotalEt/ldgPt;
	double EtotOverLdgPt=ECalTotalEt/ldgPt;
	double HhotOverLdgPt=HCal3x3Et/ldgPt;//HCalTowerEtMax/ldgPt;
	double EhotOverLdgPt=ECalTowerEtMax/ldgPt;

	double OutTrHhotDEta = fabs(HCalTowerEtMaxEta-ldgPtOuterEta);
	double OutTrHhotDPhi = fabs(HCalTowerEtMaxPhi-ldgPtOuterPhi);
	if(OutTrHhotDPhi>acos(-1.))OutTrHhotDPhi=2*acos(-1.)-OutTrHhotDPhi;
	double OutTrHhotdR = sqrt(OutTrHhotDEta*OutTrHhotDEta+OutTrHhotDPhi*OutTrHhotDPhi);

	if(HCalTowerEtMax>4.)hOutTrPhiVsEta->Fill(fabs(ldgPtOuterPhiDeg),fabs(ldgPtOuterEta));
	hOutTrMaxEdR->Fill(OutTrMaxEdR);
	
	hHhot->Fill(HCalTowerEtMax);
	hEhot->Fill(ECalTowerEtMax);
	
	if(HCalTotalEt>0.)hHhotOverHtot->Fill(HCalTowerEtMax/HCalTotalEt);
	if(ECalTotalEt>0.)hEhotOverEtot->Fill(ECalTowerEtMax/ECalTotalEt);
	
	hHtotOverLdgPt->Fill(HtotOverLdgPt);
	hEtotOverLdgPt->Fill(EtotOverLdgPt);
	
	hHhotOverLdgPt->Fill(HhotOverLdgPt);
	hEhotOverLdgPt->Fill(EhotOverLdgPt);
	
	hHhotOLdgPtVsLdgPt->Fill(HhotOverLdgPt,ldgPt);
	hEhotOLdgPtVsLdgPt->Fill(EhotOverLdgPt,ldgPt);
	
	hHhotOLdgPtVsHhot->Fill(HhotOverLdgPt,HCalTowerEtMax);
	hEhotOLdgPtVsEhot->Fill(EhotOverLdgPt,ECalTowerEtMax);      
	
	hHtotOLdgPtVsLdgPt->Fill(HtotOverLdgPt,ldgPt);
	hEtotOLdgPtVsLdgPt->Fill(EtotOverLdgPt,ldgPt);
	
	hHtotOLdgPtVsHtot->Fill(HtotOverLdgPt,HCalTotalEt);
	hEtotOLdgPtVsEtot->Fill(EtotOverLdgPt,ECalTotalEt);      
	
	hEhotOLdgPtVsHhot->Fill(EhotOverLdgPt,HCalTowerEtMax);

	hHtotOLdgPtVsOutTrHhotDEta->Fill(HtotOverLdgPt,OutTrHhotDEta);
	hHtotOLdgPtVsOutTrHhotDR->Fill(HtotOverLdgPt,OutTrHhotdR);
      
	if(HhotOverLdgPt>0.1/*HtotOverLdgPt>0.1*/){
	  hOutTrHhotDR->Fill(OutTrHhotdR);
	  hOutTrHhotDEta->Fill(OutTrHhotDEta);
	  if(OutTrHhotDEta<0.1)
	    hOutTrHhotDRPassesDEta->Fill(OutTrHhotdR);
	}
	
	double absTrackAtEcalEta=fabs(trackAtEcal.eta());
	if ( absTrackAtEcalEta < 0.018 || (absTrackAtEcalEta>0.423 && absTrackAtEcalEta<0.461) || 
	     (absTrackAtEcalEta>0.770 && absTrackAtEcalEta<0.806) || 
	     (absTrackAtEcalEta>1.127 && absTrackAtEcalEta<1.163) || 
	     (absTrackAtEcalEta>1.460 && absTrackAtEcalEta<1.558) ||
	      HhotOverLdgPt<0.1||OutTrHhotDEta>0.1 )continue;	
	
	Count++;

      }
      
      
      
    }
  
}
  


void ZETauRecoObjectQualityAnalyzer::MakeResolutionPlots(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  Handle<LorentzVectorCollection> MCinfo;
  TLorentzVector mcTauJet,mcLeptonFromTau,mcNetrina;
  int j=0;
  for(vector<edm::InputTag>::const_iterator t = m_mcTauProducts.begin(); t != m_mcTauProducts.end(); ++ t )
    {
      try
	{
	  j++;
	  iEvent.getByLabel(*t,MCinfo);
	  LorentzVectorCollection::const_iterator imc=MCinfo->begin();
	  for(;imc!=MCinfo->end();++imc)
	    {
	      if(j==1)mcTauJet.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	      if(j==2)mcLeptonFromTau.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	      if(j==3)mcNetrina.SetPxPyPzE((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
	    }
	
	}catch(...){;}
    }
  
  Handle<IsolatedTauTagInfoCollection> tautagHandle;
  iEvent.getByLabel("coneIsolationTauJetTags",tautagHandle);
  IsolatedTauTagInfoCollection::const_iterator tauit = tautagHandle->begin();
  
  TLorentzVector tau,pftau,elec,caloMet,obj_temp,
    visRecoZmass,visRecoPFZmass,visMetRecoZmass,visMetRecoPFZmass,
    visMcZmass,visMetMcZmass;
  int count=0;
  for(;tauit!=tautagHandle->end();++tauit)
    {
      obj_temp.SetPxPyPzE(tauit->jet().get()->px(),tauit->jet().get()->py(),
	       tauit->jet().get()->pz(),tauit->jet().get()->energy());
      if(obj_temp.DeltaR(mcTauJet)<0.3){
	tau=obj_temp;
	count++;
      }
    }	  
  if(count>1)cout<<"LALALALALALALA::"<<count<<endl;
  
  Handle<PFTauTagInfoCollection> pftauHandle;
  iEvent.getByLabel("pfRecoTauTagInfoProducer",pftauHandle);
  PFTauTagInfoCollection::const_iterator pftauit = pftauHandle->begin();
  
  double deltaR=1000;
  for(;pftauit!=pftauHandle->end();++pftauit)
    {
      obj_temp.SetPxPyPzE(pftauit->pfjetRef()->px(),pftauit->pfjetRef()->py(),
			  pftauit->pfjetRef()->pz(),pftauit->pfjetRef()->energy());
      if(obj_temp.DeltaR(mcTauJet)<0.1&&obj_temp.DeltaR(mcTauJet)<deltaR){
	deltaR=obj_temp.DeltaR(mcTauJet);
	pftau=obj_temp;
      }
    }
  
  if(deltaR<1000)hDeltaRPFTau->Fill(deltaR);
  
  Handle<CandidateCollection> electronHandle;
  iEvent.getByLabel("allElectrons",electronHandle);
  CandidateCollection::const_iterator elecit=electronHandle->begin();

  for(;elecit!=electronHandle->end();++elecit)
    {
      obj_temp.SetPxPyPzE(elecit->px(),elecit->py(),
			  elecit->pz(),elecit->energy());
      if(obj_temp.DeltaR(mcLeptonFromTau)<0.1)elec=obj_temp;
    }

  Handle<CandidateCollection> caloMetHandle;
  try{
    iEvent.getByLabel("caloMETShallowCloneProducer",caloMetHandle);
    CandidateCollection::const_iterator caloMetit=caloMetHandle->begin();
    caloMet.SetPxPyPzE(caloMetit->px(),caloMetit->py(),caloMetit->pz(),caloMetit->energy());
  }catch(...){;}

  visRecoZmass=elec+tau;
  visRecoPFZmass=elec+pftau;
  visMetRecoPFZmass=caloMet+elec+pftau;
  visMcZmass=mcTauJet+mcLeptonFromTau;
  
  if(mcLeptonFromTau.Et()>16&&elec.Et()>0.&&tau.Et()>0.&&pftau.Et()>0.){
    if(mcTauJet.Et()>20.){
      hVisRecoZmassRes->Fill((visRecoZmass.M()-visMcZmass.M())/visMcZmass.M());
      hVisRecoPFZmassRes->Fill((visRecoPFZmass.M()-visMcZmass.M())/visMcZmass.M());

      hVisRecoZmass->Fill(visRecoZmass.M());
      hVisRecoPFZmass->Fill(visRecoPFZmass.M());
    }
  
    if(mcTauJet.Et()>25&&fabs(mcTauJet.Eta())<1.3){
      hPFTauEtRes->Fill((pftau.Et())/mcTauJet.Et());
      hPFTauEtaRes->Fill((pftau.Eta()-mcTauJet.Eta())/mcTauJet.Eta());
      hPFTauPhiRes->Fill((pftau.Phi()-mcTauJet.Phi())/mcTauJet.Phi());
      hPFTauEtProf->Fill(mcTauJet.Et(),(pftau.Et()-mcTauJet.Et())/mcTauJet.Et());
      hPFTauEtaProf->Fill(mcTauJet.Eta(),(pftau.Eta()-mcTauJet.Eta())/mcTauJet.Eta());      


      hTauEtRes->Fill((tau.Et()-mcTauJet.Et())/mcTauJet.Et());
      hTauEtaRes->Fill((tau.Eta()-mcTauJet.Eta())/mcTauJet.Eta());
      hTauPhiRes->Fill((tau.Phi()-mcTauJet.Phi())/mcTauJet.Phi());
      hTauEtProf->Fill(mcTauJet.Et(),(tau.Et()-mcTauJet.Et())/mcTauJet.Et());
      hTauEtaProf->Fill(mcTauJet.Eta(),(tau.Eta()-mcTauJet.Eta())/mcTauJet.Eta());
    }
    
    if(mcLeptonFromTau.Et()>16){
      hElecEtRes->Fill((elec.Et()-mcLeptonFromTau.Et())/mcLeptonFromTau.Et());
      hElecEtaRes->Fill((elec.Eta()-mcLeptonFromTau.Eta())/mcLeptonFromTau.Eta());
      hElecPhiRes->Fill((elec.Phi()-mcLeptonFromTau.Phi())/mcLeptonFromTau.Phi());
      hElecEtProf->Fill(mcLeptonFromTau.Et(),(elec.Et()-mcLeptonFromTau.Et())/mcLeptonFromTau.Et());
      hElecEtaProf->Fill(mcLeptonFromTau.Eta(),(elec.Eta()-mcLeptonFromTau.Eta())/mcLeptonFromTau.Eta());
    }
  }
  
  if(caloMet.Et()&&mcTauJet.Et()>20&&mcNetrina.Et()>10){
    visMetRecoZmass=caloMet+elec+tau;
    visMetMcZmass=mcTauJet+mcLeptonFromTau+mcNetrina;
    hVisMetRecoZmassRes->Fill((visMetRecoZmass.M()-visMetMcZmass.M())/visMetMcZmass.M());
    hVisMetRecoPFZmassRes->Fill((visMetRecoPFZmass.M()-visMetMcZmass.M())/visMetMcZmass.M());
    hMEtEtRes->Fill((caloMet.Et()-mcNetrina.Pt())/mcNetrina.Pt());
    hMEtPhiRes->Fill((caloMet.Phi()-mcNetrina.Phi())/mcNetrina.Phi());

    hVisMetRecoZmass->Fill(visMetRecoZmass.M());
    hVisMetRecoPFZmass->Fill(visMetRecoPFZmass.M());
  }
  

}


void ZETauRecoObjectQualityAnalyzer::InialiseResolutionPlots()
{
  hVisRecoZmassRes=new TH1D("VisRecoZmassRes","VisRecoZmassRes;Visible Z mass Resolution",200,-2,2);
  hVisRecoPFZmassRes=new TH1D("VisRecoPFZmassRes","VisRecoPFZmassRes;Visible PF Z mass Resolution",200,-2,2);
  hVisMetRecoZmassRes=new TH1D("VisMetRecoZmassRes","VisMetRecoZmassRes;Visible+MEt Z mass Resolution",200,-2,2);
  hVisMetRecoPFZmassRes=new TH1D("VisMetRecoPFZmassRes","VisMetRecoPFZmassRes;Visible+MEt PF Z mass Resolution",200,-2,2);
  
  hVisRecoZmass=new TH1D("VisRecoZmass","VisRecoZmassVis;Vis Z mass [GeV/c^{2}]",100,0,200);
  hVisRecoPFZmass=new TH1D("VisRecoPFZmass","VisRecoPFZmass;Vis Z mass with PF #tau jet [GeV/c^{2}]",100,0,200);
  hVisMetRecoZmass=new TH1D("VisMetRecoZmass","VisMetRecoZmass;Vis+MEt Z mass",100,0,200);
  hVisMetRecoPFZmass=new TH1D("VisMetRecoPFZmass","VisMetRecoPFZmass;Vis+MEt Z mass with PF #tau jet [GeV/c^{2}]",100,0,200);

  
  hMEtEtRes=new TH1D("MEtEtRes","MEtEtRes;ME_{T} E_{T} Resolution",400,-2,2);
  hMEtPhiRes=new TH1D("MEtPhiRes","MEtPhiRes;ME_{T} #phi Resolution",400,-2,2);
  
  hPFTauEtRes=new TH1D("PFTauEtRes","PFTauEtRes;PF Tau Jet Et Resolution",400,-2,2);
  hPFTauEtaRes=new TH1D("PFTauEtaRes","PFTauEtaRes;PF Tau Jet #eta Resolution",400,-2,2);
  hPFTauPhiRes=new TH1D("PFTauPhiRes","PFTauPhiRes;PF Tau Jet #phi Resolution",400,-2,2);

  hTauEtRes=new TH1D("TauEtRes","TauEtRes;Tau Jet Et Resolution",400,-2,2);
  hTauEtaRes=new TH1D("TauEtaRes","TauEtaRes;Tau Jet #eta Resolution",400,-2,2);
  hTauPhiRes=new TH1D("TauPhiRes","TauPhiRes;Tau Jet #phi Resolution",400,-2,2);
  
  hElecEtRes=new TH1D("ElecEtRes","ElecEtRes;Elec Et Resolution",400,-2,2);
  hElecEtaRes=new TH1D("ElecEtaRes","ElecEtaRes;Elec #eta Resolution",400,-2,2);
  hElecPhiRes=new TH1D("ElecPhiRes","ElecPhiRes;Elec #phi Resolution",400,-2,2);

  hTauEtProf=new TProfile("TauEtProf","TauEtProf;Mc #tau jet E_{T} [GeV];Calo #tau jet E_{T} Resolution",40,0,80,-2,2);
  hTauEtaProf=new TProfile("TauEtaProf","TauEtaProf;Mc #tau jet #eta;Calo #tau jet #eta Resolution",60,-3.,3.,-2,2);
  
  hPFTauEtProf=new TProfile("PFTauEtProf","PFTauEtProf;Mc #tau jet E_{T} [GeV];PF #tau jet E_{T} Resolution",40,0,80,-2,2);
  hPFTauEtaProf=new TProfile("PFTauEtaProf","PFTauEtaProf;Mc #tau jet #eta;PF #tau jet #eta Resolution",60,-3.,3.,-2,2);

  hElecEtProf=new TProfile("ElecEtProf","PFTauEtProf;Mc e E_{T} [GeV];Gsf e E_{T} Resolution",40,0,80,-2,2);
  hElecEtaProf=new TProfile("ElecEtaProf","PFTauEtaProf;Mc e jet #eta;Gsf e #eta Resolution",60,-3.,3.,-2,2);
  
  hDeltaRPFTau=new TH1D("hDeltaRPFTau","hDeltaRPFTau",500,0,1);

}

//Taken from RecoJets/JetAssociationAlgorithms/src/JetTracksAssociationDRCalo.cc - Fedor Ratnikov

GlobalPoint ZETauRecoObjectQualityAnalyzer::PropagateTrackToCalo (const reco::Track& fTrack,const MagneticField& fField,const Propagator& fPropagator)
{

  const double rBarrel = 143.;
  const double zEndcap = 320.;
  const double zVF = 1100.;
  const double rEndcapMin = zEndcap * tan ( 2*atan (exp (-3.)));
  const double rVFMin = zEndcap * tan ( 2*atan (exp (-5.191)));

  GlobalPoint trackPosition (fTrack.vx(), fTrack.vy(), fTrack.vz()); // reference point
  GlobalVector trackMomentum (fTrack.px(), fTrack.py(), fTrack.pz()); // reference momentum
  try {
    if (fTrack.extra().product ()) { // use outer point information, if available
      trackPosition =  GlobalPoint (fTrack.outerX(), fTrack.outerY(), fTrack.outerZ());
      trackMomentum = GlobalVector (fTrack.outerPx(), fTrack.outerPy(), fTrack.outerPz());
    }
  }
  catch (...) {}
 
  GlobalTrajectoryParameters trackParams(trackPosition, trackMomentum, fTrack.charge(), &fField);
  FreeTrajectoryState trackState (trackParams);
  
  // first propagate to barrel
  TrajectoryStateOnSurface 
    propagatedInfo = fPropagator.propagate (trackState, 
					    *Cylinder::build (Surface::PositionType (0,0,0),
							      Surface::RotationType(),
							      rBarrel)
					    );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.z()) < zEndcap) {
      // 	std::cout << "propagateTrackToCalo-> propagated to barrel:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  
  // failed with barrel, try endcap
  double zTarget = trackMomentum.z() > 0 ? zEndcap : -zEndcap;
  propagatedInfo = fPropagator.propagate (trackState, 
					  *Plane::build( Surface::PositionType(0, 0, zTarget),
							 Surface::RotationType())
					  );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.perp()) > rEndcapMin) {
      // 	std::cout << "propagateTrackToCalo-> propagated to endcap:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  // failed with endcap, try VF
  zTarget = trackMomentum.z() > 0 ? zVF : -zVF;
  propagatedInfo = fPropagator.propagate (trackState, 
					  *Plane::build( Surface::PositionType(0, 0, zTarget),
							 Surface::RotationType())
					  );
  if (propagatedInfo.isValid()) {
    GlobalPoint result (propagatedInfo.globalPosition ());
    if (fabs (result.perp()) > rVFMin) {
      // 	std::cout << "propagateTrackToCalo-> propagated to VF:"
      // 		  << " x/y/z/r: " << result.x() << '/' << result.y() << '/' << result.z() << '/' << result.perp()
      // 		  << std::endl;
      return result;
    }
  }
  // no luck
  //     std::cout << "propagateTrackToCalo-> failed to propagate track to calorimeter" << std::endl;
  return GlobalPoint (0, 0, 0);
}


/*
    iEvent.getByLabel("ZRecoMedTauTagCollinearityFilter","TauTag",nonColTauHandle);
    iEvent.getByLabel("ZRecoMedTauTagCollinearityFilter","Electron",nonColElecHandle);
    nonColTauit=nonColTauHandle->begin();
    nonColElecit=nonColElecHandle->begin();
    TLorentzVector rectau(nonColTauit->px(),nonColTauit->py(),nonColTauit->pz(),nonColTauit->energy());
    TLorentzVector recelec(nonColElecit->px(),nonColElecit->py(),nonColElecit->pz(),nonColElecit->energy());
    
    for(unsigned int i=0;i<eFromZVec.size();++i)
      {
	hdRElecTau->Fill(eFromZVec[i].DeltaR(fasttau));
	if(eFromZVec[i].DeltaR(fasttau)<0.5){
	  //if(nonColElecHandle->size()==1&&nonColTauHandle->size()==1&&recelec.DeltaR(elec)<0.2)
	  hNtauVsNelec->Fill(nonColTauHandle->size(),nonColElecHandle->size());
	}
      }
    */









