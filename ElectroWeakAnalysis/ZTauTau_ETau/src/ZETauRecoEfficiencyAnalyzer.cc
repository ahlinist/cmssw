#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoEfficiencyAnalyzer.h"
#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoPropagateTrackToCalo.h"

using namespace edm;
using namespace reco;
using namespace std;  

ZETauRecoEfficiencyAnalyzer::ZETauRecoEfficiencyAnalyzer(const edm::ParameterSet& iConfig):
								   m_mcTauProducts(iConfig.getParameter<VInputTag>("mcTauProducts")),
								   m_zETauRecoModules(iConfig.getParameter<VInputTag>("zETauRecoModules")),
                                                                   m_zETauRecoModulesTau(iConfig.getParameter<VInputTag>("zETauRecoModulesTau")),
                                                                   m_zETauRecoModulesLep(iConfig.getParameter<VInputTag>("zETauRecoModulesLep")),
                                                                   m_MassProducer(iConfig.getUntrackedParameter<std::string>("MassProducer")),
                                                                   m_RecomoduleName(iConfig.getParameter<vstring>("ModuleNames")),
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
  Count=0;
  
  InitializeHistograms(); 
  
  
  for(int i=0;i<20;i++){
    m_module_match[i]=0;
    m_module[i]=0;
  }
    
  
}

ZETauRecoEfficiencyAnalyzer::~ZETauRecoEfficiencyAnalyzer()
{
}

void ZETauRecoEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if((nEvt%10==0 && nEvt<=100)||(nEvt%100==0 && nEvt>100)||(nEvt%1000==0 && nEvt>1000))
    std::cout << "reading event " << nEvt << std::endl;
 

  edm::ESHandle<MagneticField> field;
  try{
    iSetup.get<IdealMagneticFieldRecord>().get(field);
  }catch(...){;}
  edm::ESHandle<Propagator> propagator;
  try{
    iSetup.get<TrackingComponentsRecord>().get("SteppingHelixPropagatorAlong", propagator);
  }catch(...){;}

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

  if(true/*(mcTauJet+mcLeptonFromTau+mcNeutrina).M()<110&&
	   (mcTauJet+mcLeptonFromTau+mcNeutrina).M()>70*/){

  nEvt++;
  Handle<LorentzVectorCollection> objectVectorHandle;
  Handle<LorentzVectorCollection> objectTauVectorHandle;
  Handle<LorentzVectorCollection> objectLepVectorHandle;

  int pass=0;
  int tau_match[20],lep_match[20];
  for(int i=0;i<20;i++){tau_match[i]=0;lep_match[i]=0;}
  TLorentzVector obj;
  
  nPaths=m_zETauRecoModules.size();

  for( vector<edm::InputTag>::const_iterator s = m_zETauRecoModules.begin(); s != m_zETauRecoModules.end(); ++ s ) {
    try
      {
	iEvent.getByLabel(*s,objectVectorHandle);
	if(objectVectorHandle->size())m_module[pass]++;
	
      }catch(...){;}
    pass++;
  }
  
  pass=0;
  for( vector<edm::InputTag>::const_iterator s = m_zETauRecoModulesTau.begin(); s != m_zETauRecoModulesTau.end(); ++ s ) {
    
    try
      {
	iEvent.getByLabel(*s,objectTauVectorHandle);
	LorentzVectorCollection::const_iterator filterit=objectTauVectorHandle->begin();
	
	for(;filterit!=objectTauVectorHandle->end();++filterit)
	  {
	    obj.SetPxPyPzE(filterit->px(),filterit->py(),filterit->pz(),filterit->energy());
	    if(m_signal&&obj.DeltaR(mcTauJet)<0.5)tau_match[pass]=1;
	    else if(!m_signal)tau_match[pass]=0;
	  }	   
	
      }catch(...){;}
    pass++;
  }

  pass=0;
  for( vector<edm::InputTag>::const_iterator s = m_zETauRecoModulesLep.begin(); s != m_zETauRecoModulesLep.end(); ++ s ) { 
    try
      {
	iEvent.getByLabel(*s,objectLepVectorHandle);
	LorentzVectorCollection::const_iterator filterit=objectLepVectorHandle->begin();
	
	for(;filterit!=objectLepVectorHandle->end();++filterit)
	  {
	    obj.SetPxPyPzE(filterit->px(),filterit->py(),filterit->pz(),filterit->energy());
	    if(m_signal&&obj.DeltaR(mcLeptonFromTau)<0.5)lep_match[pass]=1;
	    else if(!m_signal)lep_match[pass]=0;
	  }	   
	
      }catch(...){;}
    pass++;
  }

  Handle< vector<float> > visMassHandle;
  Handle<CandidateCollection> fastElecHandle;
  Handle<IsolatedTauTagInfoCollection> fastTauHandle;
  Handle<CandidateCollection> caloJetHandle,caloMetHandle,genCandidateHandle;
  vector<float>::const_iterator visMassit;
  CandidateCollection::const_iterator fastElecit;
  IsolatedTauTagInfoCollection::const_iterator fastTauit;
  CandidateCollection::const_iterator caloMetit;
  LorentzVectorCollection::const_iterator nonColTauit,nonColElecit;
  TLorentzVector fastelec,fasttau,caloMet;

  try{
 
    iEvent.getByLabel("ZETauRecoMedSignalProngFilter","TauTag",fastTauHandle);
    fastTauit=fastTauHandle->begin();
    IsolatedTauTagInfo fasttau_cand;
    TLorentzVector fasttau_temp;
    for(;fastTauit!=fastTauHandle->end();++fastTauit)
      {
	fasttau_temp.SetPxPyPzE(fastTauit->jet().get()->px(),fastTauit->jet().get()->py(),fastTauit->jet().get()->pz(),
				 fastTauit->jet().get()->energy());
	if(fasttau_temp.Et()>fasttau.Et()){
	  fasttau=fasttau_temp;
	  fasttau_cand=(*fastTauit);
	}
      }
   
    iEvent.getByLabel("ZETauRecoMedCollinearityFilter","Electron",fastElecHandle);
    fastElecit=fastElecHandle->begin();
    Candidate* fastelec_cand;
    TLorentzVector fastelec_temp;
    for(;fastElecit!=fastElecHandle->end();++fastElecit)
      {
	fastelec_temp.SetPxPyPzE(fastElecit->px(),fastElecit->py(),fastElecit->pz(),fastElecit->energy());
	if(fastelec_temp.Et()>fastelec.Et()){
	  fastelec=fastelec_temp;
	  fastelec_cand=fastElecit->clone();
	}
      }
    const reco::TrackRef leadTk = fasttau_cand.leadingSignalTrack(0.1,6.);//Ldg Pt>16GeV to increase purity
    edm::RefVector< TrackCollection > sigtrks_temp = (fasttau_cand.tracksInCone((*leadTk).momentum(),0.07,1.));
    int  chg_temp=0;
    int NbTkSigCone=sigtrks_temp.size();
    for (edm::RefVector< TrackCollection >::const_iterator it = sigtrks_temp.begin(); it!=sigtrks_temp.end(); it++) 
      { chg_temp += (**it).charge(); }	

    hSSOScharge->Fill((chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge()));
    cout<<chg_temp<<"::"<<fastelec_cand->charge()<<endl;

    if(true/*(chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge())==-1*/){
      m_module[nPaths]++;
      iEvent.getByLabel("caloMETShallowCloneProducer",caloMetHandle);
      caloMetit=caloMetHandle->begin();
      caloMet.SetPxPyPzE(caloMetit->px(),caloMetit->py(),caloMetit->pz(),caloMetit->energy());
      TLorentzVector eMET=caloMet+fastelec;
      heMET->Fill(eMET.M());
      double eMETdPhi=fabs(fastelec.Phi()-caloMet.Phi());
      if(eMETdPhi>acos(-1.))eMETdPhi=2*acos(-1.)-eMETdPhi;
      heMETDphi->Fill(eMETdPhi);

      if(eMETdPhi<2.0){
	m_module[nPaths+1]++;
	iEvent.getByLabel("caloJetShallowCloneProducer",caloJetHandle);    
	int nJets=CountNumberOfJets(caloJetHandle,20,2.5,fasttau,fastelec);
	hnJets->Fill(nJets);
	
	if(eMET.M()<60){
	  m_module[nPaths+2]++;
	  
	  if(nJets<2){
	    Count++;
	    m_module[nPaths+3]++;
	    vector<double> massinfo;
	    double mz_vis=(fastelec+fasttau).M();
	    hRecoVisZmass_new->Fill(mz_vis); 
	    massinfo.reserve(5);
	    massinfo=CollinearMass(fasttau,fastelec,caloMet);
	    double dPhi=fabs(fasttau.Phi()-fastelec.Phi());
	    if(dPhi>acos(-1.))dPhi=2*acos(-1.)-dPhi;
	   
	    if(dPhi<(178.0*acos(-1.0)/(1.0*180))){
	      m_module[nPaths+4]++;

	      if(massinfo[0]>0.&&massinfo[1]>0.){
		double mz = sqrt(2.0*(massinfo[2]*massinfo[3])*(1.0-massinfo[4]));
		
		hRecoVisZmass->Fill(mz_vis);  
		hRecoCollZmass->Fill(mz);
		m_module[nPaths+5]++;

		if(mz_vis>40 && mz_vis<80){
		  m_module[nPaths+6]++;
		  if(m_signal&&(fasttau).DeltaR(mcTauJet)<0.5&&(fastelec).DeltaR(mcLeptonFromTau)<0.5)
		    m_module_match[nPaths+7]++;
		}
		if(m_signal)Zmass->Fill((mcTauJet+mcLeptonFromTau).M());
	      }
	    }
	  }
	}
      }
    }
  }catch(...){;}
  
  for(int i=0;i<20;i++)
    if(tau_match[i]&&lep_match[i])m_module_match[i]++;
  
 
  double m_Rm=0.1, m_Riso=0.45, m_Rsig=0.07, m_Ptmin_lt=6;  
  VariableIsoDiscriminator(iEvent,mcTauJet,20.,m_Rm,m_Riso,m_Rsig,m_Ptmin_lt,m_signal,*field,*propagator);
  smalltree->Fill();

  }
}


// ------------ method called once each job just before starting event loop  ------------
void ZETauRecoEfficiencyAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZETauRecoEfficiencyAnalyzer::endJob() {

  int nfilters = nPaths+7, neventspassHLTe=m_module[0];
  int markerstyle=21;
  
  recoEffwrtHLTe->SetMarkerStyle(markerstyle);
  recoEffwrtTotal->SetMarkerStyle(markerstyle);
  recoPurwrtTotal->SetMarkerStyle(markerstyle);

  for(int i=1;i<nfilters+1;i++)
    {
      double effwrtHLTe=1.*m_module[i-1]/(1.*neventspassHLTe);
      double effwrtHLTeerr=sqrt(effwrtHLTe*(1-effwrtHLTe)/(1.*neventspassHLTe));

      double effwrtTotal=1.*m_module[i-1]/(1.*nEvt);
      double effwrtTotalerr=sqrt(effwrtTotal*(1-effwrtTotal)/(1.*nEvt));
      
      double purwrtTotal=1.*m_module_match[i-1]/(1.*m_module[i-1]);
      double purwrtTotalerr=sqrt(purwrtTotal*(1-purwrtTotal)/(1.*m_module[i-1]));
      cout<<m_module_match[i-1]<<endl;
      cout<<m_module[i-1]<<endl;
      recoEffwrtHLTe->SetBinContent(i,effwrtHLTe);
      recoEffwrtHLTe->SetBinError(i,effwrtHLTeerr);
      if(i<nPaths+1)recoEffwrtHLTe->GetXaxis()->SetBinLabel(i,m_RecomoduleName[i-1].c_str());
    
      recoEffwrtTotal->SetBinContent(i,effwrtTotal);
      recoEffwrtTotal->SetBinError(i,effwrtTotalerr);
      if(i<nPaths+1)recoEffwrtTotal->GetXaxis()->SetBinLabel(i,m_RecomoduleName[i-1].c_str());

      recoPurwrtTotal->SetBinContent(i,purwrtTotal);
      recoPurwrtTotal->SetBinError(i,purwrtTotalerr);
      if(i<nPaths+1)recoPurwrtTotal->GetXaxis()->SetBinLabel(i,m_RecomoduleName[i-1].c_str());
    }

  CalculateIsolationEfficiency();

  std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "analyzed " << nEvt << " events " << "::"<<std::endl;
  std::cout << "writing information into file: " << thefile->GetName() << std::endl;

  
  thefile->Write();
  thefile->Close();
    
}

void ZETauRecoEfficiencyAnalyzer::InitializeHistograms()
{
  
  triggEffVsRiso=new TH1D("htriggEffVsRiso","Eff Vs Riso;Riso;Efficiency",10,0,0.5);
  triggEffVsRsig=new TH1D("htriggEffVsRsig","Eff Vs Rsig;Rsig;Efficiency",10,0,0.2);
  triggEffVsPtLtr=new TH1D("htriggEffVsPtLtr","Eff Vs P_{T} Ldg Track;P_{T} Ldg Track;Efficiency",10,0,20);
  
  triggEffVsRisoVsRsig=new TH2D("htriggEffVsRisoVsRsig","Eff Vs Riso Vs Rsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  triggEffVsRsigVsPtLtr=new TH2D("htriggEffVsRsigVsPtLtr","Eff Vs Rsig Vs PtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  triggEffVsPtLtrVsRiso=new TH2D("htriggEffVsPtLtrVsRiso","Eff Vs P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);
  
  allRiso=new TH1D("hallRiso"," Riso;Riso",10,0,0.5);
  allRsig=new TH1D("hallRsig"," Rsig;Rsig",10,0,0.2);
  allPtLtr=new TH1D("hallPtLtr","P_{T} Ldg Track;P_{T} Ldg Track",10,0,20);
  
  allRisoVsRsig=new TH2D("hallRisoVsRsig","Riso Vs Rsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  allRsigVsPtLtr=new TH2D("hallRsigVsPtLtr","Rsig Vs PtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  allPtLtrVsRiso=new TH2D("hallPtLtrVsRiso","P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);


  triggRiso=new TH1D("htriggRiso","Riso;Riso;",10,0,0.5);
  triggRsig=new TH1D("htriggRsig","Rsig;Rsig;",10,0,0.2);
  triggPtLtr=new TH1D("htriggPtLtr","P_{T} Ldg Track;P_{T} Ldg Track;",10,0,20);
  
  triggRisoVsRsig=new TH2D("htriggRisoVsRsig","RisoVsRsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  triggRsigVsPtLtr=new TH2D("htriggRsigVsPtLtr","RsigVsPtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  triggPtLtrVsRiso=new TH2D("htriggPtLtrVsRiso"," P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);

  triggEffVsRiso->Sumw2();
  triggEffVsRsig->Sumw2();
  triggEffVsPtLtr->Sumw2();
  triggEffVsRisoVsRsig->Sumw2();
  triggEffVsRsigVsPtLtr->Sumw2();
  triggEffVsPtLtrVsRiso->Sumw2();
  
  triggPFEffVsRiso=new TH1D("htriggPFEffVsRiso","Eff Vs Riso;Riso;Efficiency",10,0,0.5);
  triggPFEffVsRsig=new TH1D("htriggPFEffVsRsig","Eff Vs Rsig;Rsig;Efficiency",10,0,0.2);
  triggPFEffVsPtLtr=new TH1D("htriggPFEffVsPtLtr","Eff Vs P_{T} Ldg Track;P_{T} Ldg Track;Efficiency",10,0,20);
  
  triggPFEffVsRisoVsRsig=new TH2D("htriggPFEffVsRisoVsRsig","Eff Vs Riso Vs Rsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  triggPFEffVsRsigVsPtLtr=new TH2D("htriggPFEffVsRsigVsPtLtr","Eff Vs Rsig Vs PtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  triggPFEffVsPtLtrVsRiso=new TH2D("htriggPFEffVsPtLtrVsRiso","Eff Vs P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);
  
  allPFRiso=new TH1D("hallPFRiso"," Riso;Riso",10,0,0.5);
  allPFRsig=new TH1D("hallPFRsig"," Rsig;Rsig",10,0,0.2);
  allPFPtLtr=new TH1D("hallPFPtLtr","P_{T} Ldg Track;P_{T} Ldg Track",10,0,20);
  
  allPFRisoVsRsig=new TH2D("hallPFRisoVsRsig","Riso Vs Rsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  allPFRsigVsPtLtr=new TH2D("hallPFRsigVsPtLtr","Rsig Vs PtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  allPFPtLtrVsRiso=new TH2D("hallPFPtLtrVsRiso","P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);


  triggPFRiso=new TH1D("htriggPFRiso","Riso;Riso;",10,0,0.5);
  triggPFRsig=new TH1D("htriggPFRsig","Rsig;Rsig;",10,0,0.2);
  triggPFPtLtr=new TH1D("htriggPFPtLtr","P_{T} Ldg Track;P_{T} Ldg Track;",10,0,20);
  
  triggPFRisoVsRsig=new TH2D("htriggPFRisoVsRsig","RisoVsRsig ;Riso;Rsig",10,0,0.5,10,0,0.2);
  triggPFRsigVsPtLtr=new TH2D("htriggPFRsigVsPtLtr","RsigVsPtLtr;Rsig; PtLtr",10,0,0.2,10,0,20);
  triggPFPtLtrVsRiso=new TH2D("htriggPFPtLtrVsRiso"," P_{T} Ldg Track Vs Riso;P_{T} Ldg Track;Riso",10,0,20,10,0,0.5);

  triggPFEffVsRiso->Sumw2();
  triggPFEffVsRsig->Sumw2();
  triggPFEffVsPtLtr->Sumw2();
  triggPFEffVsRisoVsRsig->Sumw2();
  triggPFEffVsRsigVsPtLtr->Sumw2();
  triggPFEffVsPtLtrVsRiso->Sumw2();

  Zmass=new TH1D("mcZmass","mcZmass;mZ [GeV/c^{2}];Entries",30,0,90);
  hRecoVisZmass=new TH1D("RecoVisZmass","RecoVisZmass;mZ [GeV/c^{2}];Entries",30,0,120);
  hRecoVisZmass_new=new TH1D("RecoVisZmass_new","RecoVisZmass_new;mZ [GeV/c^{2}];Entries",30,0,120);
  hRecoCollZmass=new TH1D("RecoCollZmass","RecoCollZmass;mZ [GeV/c^{2}];Entries",50,0,200);
  recoEffwrtHLTe=new TH1D("recoEffwrtHLTe","Reco Efficiency wrt HLTe",20,0,20);
  recoEffwrtTotal=new TH1D("recoEffwrtTotal","Reco Efficiency wrt Total",20,0,20);
  recoPurwrtTotal=new TH1D("purwrtTotal","Reco Purity wrt Total",20,0,20);
  hSSOScharge=new TH1D("SSOScharge","SSOScharge",2,-1,1);
  heMETDphi=new TH1D("eMETDphi","eMETDphi",20,0,4);
  heMET=new TH1D("eMET","e+MET Invariant Mass",100,0,100);
  hnJets=new TH1D("nJets","Jet Multiplicity",10,0,10);
  
  hRecoZeeMass=new TH1D("RecoZeeMass","RecoZeeMass",50,40,140);
  hRecoZeeMass1=new TH1D("RecoZeeMass1","RecoZeeMass1",50,40,140);
  hRecoZeeMass2=new TH1D("RecoZeeMass2","RecoZeeMass2",50,40,140);

}


void ZETauRecoEfficiencyAnalyzer::VariableIsoDiscriminator(const edm::Event& iEvent,TLorentzVector tauJetMc,double etcut,double Rm,double Riso,double Rsig, 
							   double PtLtr,bool runOnSignal,const MagneticField& field, const Propagator& propagator)
{
  Handle<IsolatedTauTagInfoCollection> tautagHandle;
  Handle<PFIsolatedTauTagInfoCollection> pftautagHandle;
  Handle<CandidateCollection> electronHandle;
  
  iEvent.getByLabel("allElectrons",electronHandle);
  iEvent.getByLabel("coneIsolationTauJetTags",tautagHandle);
  try{
    iEvent.getByLabel("pfConeIsolation",pftautagHandle);
  }catch(...){;}

  IsolatedTauTagInfoCollection::const_iterator tauit = tautagHandle->begin();
  PFIsolatedTauTagInfoCollection::const_iterator pftauit = pftautagHandle->begin();   
  CandidateCollection::const_iterator elecit;
  
  for(;tauit!=tautagHandle->end();++tauit)
    {
      IsolatedTauTagInfo tautag=(*tauit);
      TLorentzVector tau(tauit->jet().get()->px(),tauit->jet().get()->py(),
			 tauit->jet().get()->pz(),tauit->jet().get()->energy());
      
      
      bool matches_elec=MatchesElectron(electronHandle,tau);
      bool matches_mc=MatchesMcTauJet(tauJetMc,tau,runOnSignal);
      double HCalTowerEtMax=HottestHcalTower(tautag,field,propagator);
      
      if(tautag.jet().get()->et()>etcut&&!matches_elec&&HCalTowerEtMax>.25)
	FillVariableIsoHistos(tautag,Rm,Riso,Rsig,PtLtr,matches_mc);
      
    }
  /*
   for(;pftauit!=pftautagHandle->end();++pftauit)
    {
      PFIsolatedTauTagInfo pftautag=(*pftauit);
      TLorentzVector tau(pftauit->pfjetRef()->px(),pftauit->pfjetRef()->py(),
			 pftauit->pfjetRef()->pz(),pftauit->pfjetRef()->energy());
      bool matches_elec=MatchesElectron(electronHandle,tau);
      bool matches_mc=MatchesMcTauJet(tauJetMc,tau,runOnSignal);
      if(pftautag.pfjetRef()->et()>etcut&&!matches_elec)
	FillVariablePFIsoHistos(pftautag,Rm,Riso,Rsig,PtLtr,matches_mc);
    }
  */
}


void ZETauRecoEfficiencyAnalyzer::FillVariableIsoHistos(IsolatedTauTagInfo& tautag,double Rm,
							double Riso,double Rsig,double PtLtr,bool matches_mc)
{
  for(int i=0;i<20;++i)
    {
      double riso=i*0.025;
      double rsig=i*0.01;
      double ptltr=i*1;
      
      allRiso->Fill(riso);
      allRsig->Fill(rsig);
      allPtLtr->Fill(ptltr);
      if(matches_mc){
	if(tautag.discriminator(Rm,Rsig,riso,PtLtr,1.,0,0.2))triggRiso->Fill(riso);
	if(tautag.discriminator(Rm,rsig,Riso,PtLtr,1.,0,0.2))triggRsig->Fill(rsig);
	if(tautag.discriminator(Rm,Rsig,Riso,ptltr,1.,0,0.2))triggPtLtr->Fill(ptltr);
      } 
      
      for(int j=0;j<20;++j)
	{
	  double riso1=j*0.025;
	  double rsig1=j*0.01;
	  double ptltr1=j*1; 
	  
	  allRisoVsRsig->Fill(riso,rsig1);
	  allRsigVsPtLtr->Fill(rsig,ptltr1);
	  allPtLtrVsRiso->Fill(ptltr,riso1); 
	  if(matches_mc){   
	    if(tautag.discriminator(Rm,rsig1,riso,PtLtr,1.,0,0.2))triggRisoVsRsig->Fill(riso,rsig1);
	    if(tautag.discriminator(Rm,rsig,Riso,ptltr1,1.,0,0.2))triggRsigVsPtLtr->Fill(rsig,ptltr1);
	    if(tautag.discriminator(Rm,Rsig,riso1,ptltr,1.,0,0.2))triggPtLtrVsRiso->Fill(ptltr,riso1);
	  }
	}
    }
}

void ZETauRecoEfficiencyAnalyzer::FillVariablePFIsoHistos(PFIsolatedTauTagInfo& pftautag,double Rm,
							  double Riso,double Rsig,double PtLtr,bool matches_mc)
{
  for(int i=0;i<20;++i)
    {
      double riso=i*0.025;
      double rsig=i*0.01;
      double ptltr=i*1;
      
      allPFRiso->Fill(riso);
      allPFRsig->Fill(rsig);
      allPFPtLtr->Fill(ptltr);
      if(matches_mc){
	
	if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,Rsig,riso,true,PtLtr,1.,0))triggPFRiso->Fill(riso);
	if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,rsig,Riso,true,PtLtr,1.,0))triggPFRsig->Fill(rsig);
	if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,Rsig,Riso,true,ptltr,1.,0))triggPFPtLtr->Fill(ptltr);
	      
      } 
      
      for(int j=0;j<20;++j)
	{
	  double riso1=j*0.025;
	  double rsig1=j*0.01;
	  double ptltr1=j*1; 
	  
	  allPFRisoVsRsig->Fill(riso,rsig1);
	  allPFRsigVsPtLtr->Fill(rsig,ptltr1);
	  allPFPtLtrVsRiso->Fill(ptltr,riso1); 
	  if(matches_mc){   
	    if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,rsig1,riso,true,PtLtr,1.,0))triggPFRisoVsRsig->Fill(riso,rsig1);
	    if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,rsig,Riso,true,ptltr1,1.,0))triggPFRsigVsPtLtr->Fill(rsig,ptltr1);
	    if(pftautag.discriminatorByIsolPFChargedHadrCandsN(Rm,Rsig,riso1,true,ptltr,1.,0))triggPFPtLtrVsRiso->Fill(ptltr,riso1);
	  }
	}
    }
}


bool ZETauRecoEfficiencyAnalyzer::MatchesElectron(Handle<CandidateCollection>& electronHandle,TLorentzVector tau)
{
  bool matches_elec=false;
  CandidateCollection::const_iterator elecit=electronHandle->begin();
  for(;elecit!=electronHandle->end();++elecit)
    {
      double dphi=fabs(tau.Phi()-elecit->phi());
      if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
      double deta=fabs(tau.Eta()-elecit->eta());
      double dR=sqrt(dphi*dphi+deta*deta);
      if(elecit->et()>15&&dR<0.5)matches_elec=true;
    }
  return matches_elec;
}

bool ZETauRecoEfficiencyAnalyzer::MatchesMcTauJet(TLorentzVector tauJetMc,TLorentzVector tau,bool runOnSignal)
{
  bool matches_mc=false;
  if(runOnSignal)
    if(tauJetMc.DeltaR(tau)<0.5)matches_mc=true;
  if(!runOnSignal)matches_mc=true;
  return matches_mc;
}

double ZETauRecoEfficiencyAnalyzer::HottestHcalTower(IsolatedTauTagInfo& tautag,const MagneticField& field, const Propagator& propagator)
{
  
  /*
  const reco::TrackRef leadTk = tautag.leadingSignalTrack(0.1,1.);
  if(!leadTk)return HCalTotOverLeadTkPt;
  
  const reco::CaloJet* cj = dynamic_cast<reco::CaloJet*>(const_cast<reco::Jet*>((tautag.jet().get())));
  const vector<CaloTowerRef> TauJetTowers = cj->getConstituents();
  double HCalTowerEtMax = -10.;
  double HCalTowerEtTotal=0.;
  for(unsigned int iTower = 0; iTower < TauJetTowers.size(); iTower++) {
    CaloTowerRef t = TauJetTowers[iTower];
    if( t->hadEt() >= HCalTowerEtMax )HCalTowerEtMax = t->hadEt();
    HCalTowerEtTotal+=t->hadEt();
  }
  double leadTkPt=leadTk->pt();
  HCalTotOverLeadTkPt= HCalTowerEtTotal/leadTkPt;
  //return HCalTowerEtMax;
  */

  double HCalTotOverLeadTkPt=-1.;
  const reco::TrackRef leadTk = tautag.leadingSignalTrack(0.1,1.);
  if(!leadTk)return HCalTotOverLeadTkPt ;
  const reco::CaloJet* cj = dynamic_cast<reco::CaloJet*>(const_cast<reco::Jet*>((tautag.jet().get())));
  const vector<CaloTowerRef> TauJetTowers = cj->getConstituents();
  double HCalTowerEtMax = -10.;double HCalTowerEtMaxEta = 0.;
  double HCal3x3Et=0.;
  
  GlobalPoint trackAtEcal=PropagateTrackToCalo(*leadTk,field,propagator);
  double trackAtEcalEta=trackAtEcal.eta();
  double trackAtEcalPhi=trackAtEcal.phi();
  double absTrackAtEcalEta=fabs(trackAtEcal.eta());
  
  for(unsigned int iTower = 0; iTower < TauJetTowers.size(); iTower++) {
    CaloTowerRef t = TauJetTowers[iTower];
    double deta=fabs(trackAtEcalEta-t->eta());double dphi=fabs(trackAtEcalPhi-t->phi());
    if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
    if(sqrt(dphi*dphi+deta*deta)<0.184){
      HCal3x3Et+=t->hadEt();
      if( t->hadEt() >= HCalTowerEtMax ){
	HCalTowerEtMax = t->hadEt();
	HCalTowerEtMaxEta = t->eta();
      }
    }
  }
  
  double leadTkPt;
  
  leadTkPt=leadTk->pt();
  HCalTotOverLeadTkPt=HCal3x3Et/leadTkPt;
  if ( absTrackAtEcalEta < 0.018 || (absTrackAtEcalEta>0.423 && absTrackAtEcalEta<0.461) || 
       (absTrackAtEcalEta>0.770 && absTrackAtEcalEta<0.806) || 
       (absTrackAtEcalEta>1.127 && absTrackAtEcalEta<1.163) || 
       (absTrackAtEcalEta>1.460 && absTrackAtEcalEta<1.558) )HCalTotOverLeadTkPt=-1;

  
  
    return HCalTotOverLeadTkPt;
}


void ZETauRecoEfficiencyAnalyzer::CalculateIsolationEfficiency()
{

  triggEffVsRiso->Divide(triggRiso,allRiso,1,1,"b");
  triggEffVsRsig->Divide(triggRsig,allRsig,1,1,"b");
  triggEffVsPtLtr->Divide(triggPtLtr,allPtLtr,1,1,"b");
  
  triggEffVsRisoVsRsig->Divide(triggRisoVsRsig,allRisoVsRsig,1,1,"b");
  triggEffVsRsigVsPtLtr->Divide(triggRsigVsPtLtr,allRsigVsPtLtr,1,1,"b");
  triggEffVsPtLtrVsRiso->Divide(triggPtLtrVsRiso,allPtLtrVsRiso,1,1,"b");

  triggPFEffVsRiso->Divide(triggPFRiso,allRiso,1,1,"b");
  triggPFEffVsRsig->Divide(triggPFRsig,allRsig,1,1,"b");
  triggPFEffVsPtLtr->Divide(triggPFPtLtr,allPtLtr,1,1,"b");
  
  triggPFEffVsRisoVsRsig->Divide(triggPFRisoVsRsig,allPFRisoVsRsig,1,1,"b");
  triggPFEffVsRsigVsPtLtr->Divide(triggPFRsigVsPtLtr,allPFRsigVsPtLtr,1,1,"b");
  triggPFEffVsPtLtrVsRiso->Divide(triggPFPtLtrVsRiso,allPFPtLtrVsRiso,1,1,"b");
}



int ZETauRecoEfficiencyAnalyzer::CountNumberOfJets(Handle<CandidateCollection>& caloJetHandle,double etMin, double etaRange,
						    TLorentzVector fasttau, TLorentzVector fastelec)
{
  CandidateCollection::const_iterator caloJetit;  
  caloJetit=caloJetHandle->begin();
  int nJets=0;
  for(;caloJetit!=caloJetHandle->end();++caloJetit)
    {
      TLorentzVector caloJet(caloJetit->px(),caloJetit->py(),caloJetit->pz(),caloJetit->energy());
      if(caloJet.Et()>etMin && fabs(caloJet.Eta())<etaRange &&
	 caloJet.DeltaR(fastelec)>0.5&& caloJet.DeltaR(fasttau)>0.5)
	nJets++;
    }
  return nJets;
}

vector<double> ZETauRecoEfficiencyAnalyzer::CollinearMass(TLorentzVector fasttau,TLorentzVector fastelec,TLorentzVector caloMet)
{
  vector<double> massinfo;
  double x1=sin(fasttau.Theta())*cos(fasttau.Phi());
  double x2=sin(fastelec.Theta())*cos(fastelec.Phi());
  double y1=sin(fasttau.Theta())*sin(fasttau.Phi());
  double y2=sin(fastelec.Theta())*sin(fastelec.Phi());

  double E2=(caloMet.Y()*x1-caloMet.X()*y1)/(x1*y2-x2*y1);
  double E1=(caloMet.X()-E2*x2)/x1;   

  double CorrectedTau1E=fasttau.E()+E1;
  double CorrectedTau2E=fastelec.E()+E2;

  double cosje=(fasttau.E()*x1*fastelec.E()*x2+fasttau.E()*y1*fastelec.E()*y2+fasttau.E()*cos(fasttau.Theta())*fastelec.E()*cos(fastelec.Theta()))/(fasttau.E()*fastelec.E());

  massinfo.push_back(E2);massinfo.push_back(E1);massinfo.push_back(CorrectedTau1E);
  massinfo.push_back(CorrectedTau2E);massinfo.push_back(cosje);
  
  return massinfo;
}
