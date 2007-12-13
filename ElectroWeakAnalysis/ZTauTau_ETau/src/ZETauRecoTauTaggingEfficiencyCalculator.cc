#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTaggingEfficiencyCalculator.h"

using namespace edm;
using namespace reco;
using namespace std;  

ZETauRecoTauTaggingEfficiencyCalculator::ZETauRecoTauTaggingEfficiencyCalculator(const edm::ParameterSet& iConfig):
								   m_mcTauProducts(iConfig.getParameter<VInputTag>("mcTauProducts")),
								   m_MassProducer(iConfig.getUntrackedParameter<std::string>("MassProducer")),
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

ZETauRecoTauTaggingEfficiencyCalculator::~ZETauRecoTauTaggingEfficiencyCalculator()
{
}

void ZETauRecoTauTaggingEfficiencyCalculator::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if((nEvt%10==0 && nEvt<=100)||(nEvt%100==0 && nEvt>100)||(nEvt%1000==0 && nEvt>1000))
    std::cout << "reading event " << nEvt << std::endl;
  nEvt++;

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
  
  Zmass->Fill((mcTauJet+mcLeptonFromTau+mcNeutrina).M());		  
  
  Handle<CandidateCollection> fastElecHandle;
  Handle<IsolatedTauTagInfoCollection> tauHandle;
  Handle<CandidateCollection> caloJetHandle,caloMetHandle;
  
  CandidateCollection::const_iterator fastElecit;
  vector<IsolatedTauTagInfo>::const_iterator tauit;
  CandidateCollection::const_iterator caloMetit;
  TLorentzVector fastelec,fastelec_temp,fasttau,fasttau_1,fasttau_2,caloMet;

  try{
      
    iEvent.getByLabel("ZETauRecoMedCollinearityFilter","Electron",fastElecHandle);
    fastElecit=fastElecHandle->begin();
    Candidate* fastelec_cand;
    for(;fastElecit!=fastElecHandle->end();++fastElecit)
      {
	fastelec_temp.SetPxPyPzE(fastElecit->px(),fastElecit->py(),fastElecit->pz(),fastElecit->energy());
	if(fastelec_temp.Et()>fastelec.Et()){
	  fastelec=fastelec_temp;
	  fastelec_cand=fastElecit->clone();
	}
      }
    iEvent.getByLabel("ZETauRecoMedCollinearityFilter","TauTag",tauHandle);
    tauit=tauHandle->begin();
    
    iEvent.getByLabel("caloMETShallowCloneProducer",caloMetHandle);
    caloMetit=caloMetHandle->begin();
    caloMet.SetPxPyPzE(caloMetit->px(),caloMetit->py(),caloMetit->pz(),caloMetit->energy());
    TLorentzVector eMET=caloMet+fastelec;
    heMET->Fill(eMET.M());
    
    iEvent.getByLabel("caloJetShallowCloneProducer",caloJetHandle);   

    double dRJetElec=10000.;
    double dRTauElec;
    int nTauJets=0,nTauJets_1=0,nTauJets_2=0;

    for(;tauit!=tauHandle->end();++tauit)
      {
	TLorentzVector tau(tauit->jet().get()->px(),tauit->jet().get()->py(),tauit->jet().get()->pz(),tauit->jet().get()->energy());
	if(m_signal&&(tau).DeltaR(mcTauJet)>0.3)hJetEt->Fill(tau.Et());
	if(m_signal&&(tau).DeltaR(mcTauJet)<0.3){
	  hJetEt_1->Fill(tau.Et());
	  m_module_match[0]++;
	  const reco::TrackRef theLeadTk = tauit->leadingSignalTrack(0.1,10.);
	  if( !theLeadTk ) continue;
	  if(tau.Et()>fasttau.Et())
	    fasttau=tau;
	  if(tauit->discriminator(0.1,0.07,0.45,10.,1.,0,0.2))
	    {
	      int NTkSigCone = (tauit->tracksInCone((*theLeadTk).momentum(),0.07,1.)).size();
	      if(NTkSigCone==1||NTkSigCone==3)
		{
		  hTauAllPassingEt->Fill(tau.Et());
		  if(tau.Et()<40)hTauAllPassingEta->Fill(fabs(tau.Eta()));
		}
	    }
	}
	
      }
    
    tauit=tauHandle->begin();
    for(;tauit!=tauHandle->end();++tauit)
      {
	TLorentzVector tau(tauit->jet().get()->px(),tauit->jet().get()->py(),tauit->jet().get()->pz(),tauit->jet().get()->energy());
	int nJets=CountNumberOfJets(caloJetHandle,20,2.5,tau,fastelec);
	nTauJets++;
	hnJets->Fill(nJets);

	if(eMET.M()<60){
	  m_module[0]++;
	  
	  if(nJets<100){
	    m_module[1]++;
	    vector<double> massinfo;
	    massinfo.reserve(5);
	    massinfo=CollinearMass(tau,fastelec,caloMet);
	    double dPhi=fabs(tau.Phi()-fastelec.Phi());
	    if(dPhi>acos(-1.))dPhi=2*acos(-1.)-dPhi;
	    if(dPhi<(178.0*acos(-1.0)/(1.0*180))){
	      m_module[2]++;
	      if(massinfo[0]>0.&&massinfo[1]>0.){
		double mz = sqrt(2.0*(massinfo[2]*massinfo[3])*(1.0-massinfo[4]));
		double visMass=(tau+fastelec).M();
		
		if(m_signal&&(tau).DeltaR(mcTauJet)>0.3){
		  hRecoVisZmass_1->Fill((tau+fastelec).M());
		  hRecoCollZmass_1->Fill(mz);
		  
		}
		if(m_signal&&(tau).DeltaR(mcTauJet)<0.3){
		  hRecoVisZmass_2->Fill((tau+fastelec).M());
		  hRecoCollZmass_2->Fill(mz);
		}
		
		hRecoVisZmass->Fill((visMass));  
		hRecoCollZmass->Fill(mz);
		m_module[3]++;
		
		if((visMass)>40 && (visMass)<80){
		  
		  m_module[4]++;
		  const reco::TrackRef lTk = tauit->leadingSignalTrack(0.1,1.);	 
		  if(m_signal&&(tau).DeltaR(mcTauJet)>0.3){
		    hDeltaRJetElec->Fill(fabs(tau.Eta()-fastelec.Eta()));
		    hPtLdgTrack->Fill(lTk->pt());
		    hJetEt_2->Fill(tau.Et());
		    hPtLtrRel->Fill(tau.EtaPhiVector().operator*=(fastelec.EtaPhiVector()));
		  }
		  if(m_signal&&(tau).DeltaR(mcTauJet)<0.3){
		    hDeltaRJetElec_1->Fill(fabs(tau.Eta()-fastelec.Eta()));
		    hPtLdgTrack_1->Fill(lTk->pt());
		    hJetEt_3->Fill(tau.Et());
		    hPtLtrRel_1->Fill(tau.EtaPhiVector().operator*=(fastelec.EtaPhiVector()));
		  }
		  if(!m_signal){
		    hDeltaRJetElec->Fill(fabs(tau.Eta()-fastelec.Eta()));		  
		    hPtLdgTrack->Fill(lTk->pt());
		    hJetEt_2->Fill(tau.Et());
		    hPtLtrRel->Fill(tau.EtaPhiVector().operator*=(fastelec.EtaPhiVector()));
		  }
		  if(fabs(tau.Eta()-fastelec.Eta())>1.)continue;

		  const reco::TrackRef leadTk = tauit->leadingSignalTrack(0.1,10.);//Ldg Pt>16GeV to increase purity
		  if( !leadTk ) continue;
		  

		  edm::RefVector< TrackCollection > sigtrks_temp = (tauit->tracksInCone((*leadTk).momentum(),0.07,1.));
		  int  chg_temp=0;
		  int NbTkSigCone=sigtrks_temp.size();
		  for (edm::RefVector< TrackCollection >::const_iterator it = sigtrks_temp.begin(); it!=sigtrks_temp.end(); it++) 
		    { chg_temp += (**it).charge(); }	
		  
		  if(m_signal&&(tau).DeltaR(mcTauJet)>0.3)
		    hSSOScharge->Fill((chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge()));
		  if(m_signal&&(tau).DeltaR(mcTauJet)<0.3)
		    hSSOScharge_1->Fill((chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge()));
		  if(!m_signal)
		    hSSOScharge->Fill((chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge()));

		  if((chg_temp*fastelec_cand->charge()*1.)/fabs(chg_temp*fastelec_cand->charge())==1)continue;

		  if(tau.Et()>fasttau_1.Et())
		    fasttau_1=tau;
		  
		  nTauJets_1++;
		  if(m_signal&&(tau).DeltaR(mcTauJet)<0.3){
		    m_module_match[1]++;
		    hJetEt_5->Fill(tau.Et());
		  }
		  if(m_signal&&(tau).DeltaR(mcTauJet)>0.3){
		    hJetEt_4->Fill(tau.Et());
		  }
		  if(!m_signal)
		    hJetEt_4->Fill(tau.Et());
		  
		  if(tauit->discriminator(0.1,0.07,0.45,10.,1.,0,0.2))//Ldg Pt>16GeV to increase purity
		    {
		      //int NbTkSigCone = (tauit->tracksInCone((*leadTk).momentum(),0.07,1.)).size();
		      if(NbTkSigCone==1||NbTkSigCone==3)
			{
			  nTauJets_2++;
			  if(tau.Et()>fasttau_2.Et())
			    fasttau_2=tau;
			  if(m_signal&&(tau).DeltaR(mcTauJet)<0.3){
			    m_module_match[2]++;
			  }
			}
		    }
		  
		  //if(m_signal)Zmass->Fill((mcTauJet+mcLeptonFromTau).M());
		}
	      }
	    }
	  }
	}
      }
    
    hnTauJets->Fill(nTauJets);
    hnTauJets_1->Fill(nTauJets_1);
    hnTauJets_2->Fill(nTauJets_2);
    if(fasttau.Et()>0)
      {
	hTauAllEt->Fill(fasttau.Et());
	if(fasttau.Et()<40)hTauAllEta->Fill(fabs(fasttau.Eta()));
	if(m_signal&&(fasttau).DeltaR(mcTauJet)<0.3)
	  hTauAllEtMatching->Fill(fasttau.Et());
      }
    if(fasttau_1.Et()>0&&nTauJets_1==1)
       {
	 hTauEt->Fill(fasttau_1.Et());
	 if(fasttau_1.Et()<40)hTauEta->Fill(fabs(fasttau_1.Eta()));
	 if(m_signal&&(fasttau_1).DeltaR(mcTauJet)<0.3){
	   hTauEtMatching->Fill(fasttau_1.Et());
	   if(fasttau_1.Et()<40)hTauEtaMatching->Fill(fabs(fasttau_1.Eta()));
	 }
       }

     if(fasttau_2.Et()>0&&nTauJets_2==1)
      {
	hTauPassingEt->Fill(fasttau_1.Et());//Yes this is correct
	if(fasttau_1.Eta()<40)hTauPassingEta->Fill(fabs(fasttau_1.Eta()));//Yes this is correct
      }

  }catch(...){;}
  
  smalltree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void ZETauRecoTauTaggingEfficiencyCalculator::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZETauRecoTauTaggingEfficiencyCalculator::endJob() {


  hEffEt->BayesDivide(hTauPassingEt,hTauEt);
  hEffEta->BayesDivide(hTauPassingEta,hTauEta);
  
  hEffAllEt->BayesDivide(hTauAllPassingEt,hTauAllEt);
  hEffAllEta->BayesDivide(hTauAllPassingEta,hTauAllEta);

  TLegend* leg=new TLegend(0.7,0.8,0.95,0.95);
  leg->AddEntry(hEffEt,"Using Z Reco","lp");
  leg->AddEntry(hEffAllEt,"Using MC truth","lp");

  TLegend* leg1=new TLegend(0.7,0.8,0.95,0.95);
  leg1->AddEntry(hEffEta,"Using Z Reco","lp");
  leg1->AddEntry(hEffAllEta,"Using MC truth","lp");

  
  hEffEt->GetYaxis()->SetRangeUser(0.,1.);
  hEffEta->GetYaxis()->SetRangeUser(0.,1.);
  
  hEffEt->GetXaxis()->SetRangeUser(20.,40.);
  hEffEta->GetXaxis()->SetRangeUser(0.,2.5);

  hEffEt->GetXaxis()->SetTitle("Reco #tau jet E_{T}");
  hEffEta->GetXaxis()->SetTitle("Reco #tau jet #eta");
  
  hEffEt->GetYaxis()->SetTitle("Efficiency");
  hEffEta->GetYaxis()->SetTitle("Efficiency");
  

  TCanvas* c= new TCanvas("c","c",600,600);
  hEffEt->Draw("AP");
  hEffAllEt->Draw("PSAME");
  leg->Draw();
  c->SaveAs("EfficiencyEt.pdf");
  //c->Write();
  TCanvas* c1= new TCanvas("c1","c1",600,600);
  hEffEta->Draw("AP");
  hEffAllEta->Draw("PSAME");
  leg1->Draw();
  c1->SaveAs("EfficiencyEta.pdf");
  //c1->Write();

  std::cout <<"nMatched::"<<m_module_match[0]<<"::"<<"nMatched_1::"<<m_module_match[1]<<"::"<<"nMatched_2::"<<m_module_match[2]<<endl;
  std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "analyzed " << nEvt << " events " << "::"<<std::endl;
  std::cout << "writing information into file: " << thefile->GetName() << std::endl;

  
  thefile->Write();
  thefile->Close();
    
}

void ZETauRecoTauTaggingEfficiencyCalculator::InitializeHistograms()
{

  Zmass=new TH1D("mcZmass","mcZmass;mZ [GeV/c^{2}];Entries",60,0,180);
  hRecoVisZmass=new TH1D("RecoVisZmass","RecoVisZmass;mZ [GeV/c^{2}];Entries",30,0,120);
  hRecoVisZmass_1=new TH1D("RecoVisZmass_1","RecoVisZmass_1;mZ [GeV/c^{2}];Entries",30,0,120);
  hRecoVisZmass_2=new TH1D("RecoVisZmass_2","RecoVisZmass_2;mZ [GeV/c^{2}];Entries",30,0,120);

  hRecoCollZmass=new TH1D("RecoCollZmass","RecoCollZmass;mZ [GeV/c^{2}];Entries",50,0,200);
  hRecoCollZmass_1=new TH1D("RecoCollZmass_1","RecoCollZmass_1;mZ [GeV/c^{2}];Entries",50,0,200);
  hRecoCollZmass_2=new TH1D("RecoCollZmass_2","RecoCollZmass_2;mZ [GeV/c^{2}];Entries",50,0,200);

  heMET=new TH1D("eMET","e+MET Invariant Mass",100,0,100);
  hnJets=new TH1D("nJets","Jet Multiplicity",10,0,10);
  hnTauJets=new TH1D("nTauJets","Tau Jet Multiplicity",10,0,10);
  hnTauJets_1=new TH1D("nTauJets_1","Tau Jet Multiplicity",10,0,10);
  hnTauJets_2=new TH1D("nTauJets_2","Tau Jet Multiplicity",10,0,10);

  float lower_bin_edge[]={20,25,30,40};int nbins=3;
  float lower_bin_edge_eta[]={0,0.5,1.,1.5,2,2.5};int nbins_eta=5;
  TString axis[]={"E_{T}^{Reco #tau jet}"};
  TString axis_eta[]={"#eta^{Reco #tau jet}"};

  hTauPassingEt=new TH1D("TauPassingEt","TauPassingEt;"+axis[0],nbins,lower_bin_edge);
  hTauEt=new TH1D("TauEt","TauEt;"+axis[0],nbins,lower_bin_edge);
  hTauEtMatching=new TH1D("TauEtMatching","TauEtMatching;"+axis[0],nbins,lower_bin_edge);

  hTauEtaMatching=new TH1D("TauEtaMatching","TauEtaMatching;"+axis_eta[0],nbins_eta,lower_bin_edge_eta);
  hTauPassingEta=new TH1D("TauPassingEta","TauPassingEta;"+axis_eta[0],nbins_eta,lower_bin_edge_eta);
  
  hTauEta=new TH1D("TauEta","TauEta;"+axis[0],nbins_eta,lower_bin_edge_eta);

  hTauAllPassingEt=new TH1D("TauAllPassingEt","TauAllPassingEt;"+axis[0],nbins,lower_bin_edge);
  hTauAllEt=new TH1D("TauAllEt","TauAllEt;"+axis[0],nbins,lower_bin_edge);
  hTauAllEtMatching=new TH1D("TauAllEtMatching","TauAllEtMatching;"+axis[0],nbins,lower_bin_edge);
  
  hTauAllPassingEta=new TH1D("TauAllPassingEta","TauAllPassingEta;"+axis_eta[0],nbins_eta,lower_bin_edge_eta);
  hTauAllEta=new TH1D("TauAllEta","TauAllEta;"+axis_eta[0],nbins_eta,lower_bin_edge_eta);


  hDeltaRJetElec=new TH1D("DeltaRJetElec","DeltaRJetElec",50,0,5);
  hDeltaRJetElec_1=new TH1D("DeltaRJetElec_1","DeltaRJetElec_1",50,0,5);

  hPtLdgTrack = new TH1D("PtLdgTrack","PtLdgTrack",100,0,100);
  hPtLdgTrack_1 = new TH1D("PtLdgTrack_1","PtLdgTrack_1",100,0,100);

  hPtLtrRel = new TH1D("PtLtrRel","PtLtrRel",5,0,50);
  hPtLtrRel_1 = new TH1D("PtLtrRel_1","PtLtrRel_1",5,0,50);

  hSSOScharge = new TH1D("SSOScharge","SSOScharge",4,-2,2);
  hSSOScharge_1 = new TH1D("SSOScharge_1","SSOScharge_1",4,-2,2);


  hJetEt=new TH1D("JetEt","JetEt",6,20,80);
  hJetEt_1=new TH1D("JetEt_1","JetEt_1",6,20,80);

  hJetEt_2=new TH1D("JetEt_2","JetEt_2",6,20,80);
  hJetEt_3=new TH1D("JetEt_3","JetEt_3",6,20,80);

  hJetEt_4=new TH1D("JetEt_4","JetEt_4",6,20,80);
  hJetEt_5=new TH1D("JetEt_5","JetEt_5",6,20,80);

  hTauPassingEt->Sumw2();
  hTauEt->Sumw2();
  hTauPassingEta->Sumw2();
  hTauEta->Sumw2();
  hTauAllPassingEt->Sumw2();
  hTauAllEt->Sumw2();
  hTauAllPassingEta->Sumw2();
  hTauAllEta->Sumw2();
  hTauEtMatching->Sumw2();

  hEffEt=new TGraphAsymmErrors;
  hEffEta=new TGraphAsymmErrors;

  hEffEt->SetName("EffEt");
  hEffEta->SetName("EffEta");
  
  
  hEffAllEt=new TGraphAsymmErrors;
  hEffAllEta=new TGraphAsymmErrors;
  
  hEffAllEt->SetName("EffAllEt");
  hEffAllEta->SetName("EffAllEta");
  
  hEffEt->SetMarkerStyle(22);
  hEffEta->SetMarkerStyle(22);

  hEffAllEt->SetMarkerStyle(23);
  hEffAllEta->SetMarkerStyle(23);
  
  hEffEt->SetMarkerColor(2);
  hEffEta->SetMarkerColor(2);

  hEffAllEt->SetMarkerColor(3);
  hEffAllEta->SetMarkerColor(3);

}

int ZETauRecoTauTaggingEfficiencyCalculator::CountNumberOfJets(Handle<CandidateCollection>& caloJetHandle,double etMin, double etaRange,
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

vector<double> ZETauRecoTauTaggingEfficiencyCalculator::CollinearMass(TLorentzVector fasttau,TLorentzVector fastelec,TLorentzVector caloMet)
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
