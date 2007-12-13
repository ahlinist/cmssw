#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoSelectionEfficiencyAnalyzer.h"

using namespace edm;
using namespace reco;
using namespace std;  

ZETauRecoSelectionEfficiencyAnalyzer::ZETauRecoSelectionEfficiencyAnalyzer(const edm::ParameterSet& iConfig):
								   m_mcTauProducts(iConfig.getParameter<VInputTag>("mcTauProducts")),
								   m_Output(iConfig.getUntrackedParameter<std::string>("Output","Comparison.root")),
								   m_do_pureff(iConfig.getUntrackedParameter<bool>("UsePureEffDefn"))
								   
{                                                             
  std::string treeString="DataTree";
  const char* treeChar=treeString.c_str();
  const char* output=m_Output.c_str();
  
  thefile = new TFile(output,"recreate");
  thefile->cd();
  smalltree= new TTree(treeChar,treeChar);  
  nEvt=0;
 
  
  InitialiseEfficiencyAndPurityPlots();  
  
}

ZETauRecoSelectionEfficiencyAnalyzer::~ZETauRecoSelectionEfficiencyAnalyzer()
{
}

void ZETauRecoSelectionEfficiencyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
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


  CalculateObjectEfficiencies(iEvent,mcTauJet,mcLeptonFromTau);
  smalltree->Fill();
}


// ------------ method called once each job just before starting event loop  ------------
void ZETauRecoSelectionEfficiencyAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ZETauRecoSelectionEfficiencyAnalyzer::endJob() {

  
  MakeEfficiencyPurityHistos();

  std::cout << "++++++++++++++++++++++++++++++++++++++" << std::endl;
  std::cout << "analyzed " << nEvt << " events " << "::"<<std::endl;
  std::cout << "writing information into file: " << thefile->GetName() << std::endl;

  
  thefile->Write();
  thefile->Close();
    
}

void ZETauRecoSelectionEfficiencyAnalyzer::CalculateObjectEfficiencies(const edm::Event& iEvent,TLorentzVector tauJetMc, TLorentzVector leptonFromTauMc)
{
  Handle<LorentzVectorCollection> hltElectronHandle;
  Handle<LorentzVectorCollection> gsfElectronHandle;
  Handle<LorentzVectorCollection> gsfEtCutElectronHandle;
  Handle<LorentzVectorCollection> allTauJetHandle;
  Handle<LorentzVectorCollection> cleanTauJetHandle;
  Handle<LorentzVectorCollection> isoTauJetHandle;
  Handle<LorentzVectorCollection> prongTauJetHandle;
  
  hAllElecEt->Fill(leptonFromTauMc.Et());
  hAllElecEta->Fill(leptonFromTauMc.Eta());
  hAllElecPhi->Fill(leptonFromTauMc.Phi());
  bool hlt_pure=false;
  try{
    iEvent.getByLabel("SingleElectronHLTFilter","HLTElectronObjVec",hltElectronHandle);
    hlt_pure = FillPlotsForEfficiencyAndPurityCalc(hltElectronHandle,leptonFromTauMc,hltElectronHistVec,0.005,m_do_pureff);  
  
    bool mc_ecal_fiducial=true;
    double absTrackAtVertexEta = fabs(leptonFromTauMc.Eta());
    if ( absTrackAtVertexEta <0.018 || (absTrackAtVertexEta>0.423 && absTrackAtVertexEta<0.461) || 
	 (absTrackAtVertexEta>0.770 && absTrackAtVertexEta<0.806) || 
	 (absTrackAtVertexEta>1.127 && absTrackAtVertexEta<1.163) || 
	 (absTrackAtVertexEta>1.460 && absTrackAtVertexEta<1.558))mc_ecal_fiducial=false;
    
    if(mc_ecal_fiducial){
      hAllElecEt_Fid->Fill(leptonFromTauMc.Et());
      hAllElecEta_Fid->Fill(leptonFromTauMc.Eta());
      hAllElecPhi_Fid->Fill(leptonFromTauMc.Phi());
    }
  }catch(...){;}

  try{
    iEvent.getByLabel("ZETauRecoElectronIDFilter","LeptonTag",gsfElectronHandle);
    LorentzVectorCollection::const_iterator it = gsfElectronHandle->begin();
    bool matches=false;
    int n_m=0;
    
    bool mc_ecal_fiducial=true;
    double absTrackAtVertexEta = fabs(leptonFromTauMc.Eta());
    if ( absTrackAtVertexEta <0.018 || (absTrackAtVertexEta>0.423 && absTrackAtVertexEta<0.461) || 
	 (absTrackAtVertexEta>0.770 && absTrackAtVertexEta<0.806) || 
	 (absTrackAtVertexEta>1.127 && absTrackAtVertexEta<1.163) || 
	 (absTrackAtVertexEta>1.460 && absTrackAtVertexEta<1.558))mc_ecal_fiducial=false;
    
    if(/*hlt_pure&&*/mc_ecal_fiducial){
      FillPlotsForEfficiencyAndPurityCalc(gsfElectronHandle,leptonFromTauMc,gsfElectronHistVec,0.005,m_do_pureff);  
    }
  }catch(...){;}
  
  try{
    bool mc_ecal_fiducial=true;
    double absTrackAtVertexEta = fabs(leptonFromTauMc.Eta());
    if ( absTrackAtVertexEta <0.018 || (absTrackAtVertexEta>0.423 && absTrackAtVertexEta<0.461) || 
	 (absTrackAtVertexEta>0.770 && absTrackAtVertexEta<0.806) || 
	 (absTrackAtVertexEta>1.127 && absTrackAtVertexEta<1.163) || 
	 (absTrackAtVertexEta>1.460 && absTrackAtVertexEta<1.558))mc_ecal_fiducial=false;
    
    iEvent.getByLabel("ZETauRecoElectronEtFilter","LeptonTag",gsfEtCutElectronHandle);
    if(/*hlt_pure&&*/mc_ecal_fiducial){
      FillPlotsForEfficiencyAndPurityCalc(gsfEtCutElectronHandle,leptonFromTauMc,gsfEtCutElectronHistVec,0.005,m_do_pureff);    
    }
    hAllTauEt->Fill(tauJetMc.Et());
    hAllTauEta->Fill(tauJetMc.Eta());
    hAllTauPhi->Fill(tauJetMc.Phi());
  }catch(...){;}
  
  
  try{
    iEvent.getByLabel("ZETauRecoMinEtFilter","TauTag",allTauJetHandle);
    if(tauJetMc.Et()>15)FillPlotsForEfficiencyAndPurityCalc(allTauJetHandle,tauJetMc,allTauJetHistVec,0.3,m_do_pureff);
  }catch(...){;}
  try{
    iEvent.getByLabel("ZETauRecoMedCollinearityFilter","TauTag",cleanTauJetHandle);
   if(tauJetMc.Et()>15) FillPlotsForEfficiencyAndPurityCalc(cleanTauJetHandle,tauJetMc,cleanTauJetHistVec,0.3,m_do_pureff);
  }catch(...){;}
  try{
    iEvent.getByLabel("ZETauRecoMedTauTagInfoFilter","TauTag",isoTauJetHandle);
    if(tauJetMc.Et()>15)FillPlotsForEfficiencyAndPurityCalc(isoTauJetHandle,tauJetMc,isoTauJetHistVec,0.3,m_do_pureff);
  }catch(...){;}
  try{
    iEvent.getByLabel("ZETauRecoMedSignalProngFilter","TauTag",prongTauJetHandle);
    if(tauJetMc.Et()>15)FillPlotsForEfficiencyAndPurityCalc(prongTauJetHandle,tauJetMc,prongTauJetHistVec,0.3,m_do_pureff);
  }catch(...){;}
}



bool ZETauRecoSelectionEfficiencyAnalyzer::FillPlotsForEfficiencyAndPurityCalc(Handle<LorentzVectorCollection>& Handle,TLorentzVector mc,vector<TH1D*>& histvec,double deltaR,bool do_pureff)
{
  bool matches=false;
  LorentzVectorCollection::const_iterator it = Handle->begin();
  for(;it!=Handle->end();++it)
    {
      if(!do_pureff){
	histvec[0]->Fill(mc.Et());
	histvec[1]->Fill(mc.Eta());
	histvec[2]->Fill(mc.Phi());
      }
      double dPhi=fabs(it->Phi()-mc.Phi());
      if(dPhi>acos(-1.))dPhi=2*acos(-1.)-dPhi;
      double dR=sqrt(dPhi*dPhi+(it->Eta()-mc.Eta())*(it->Eta()-mc.Eta()));
      if(dR<deltaR)matches=true;
    }

  if(matches){
    histvec[3]->Fill(mc.Et());
    histvec[4]->Fill(mc.Eta());
    histvec[5]->Fill(mc.Phi());
    if(do_pureff){
      histvec[0]->Fill(mc.Et());
      histvec[1]->Fill(mc.Eta());
      histvec[2]->Fill(mc.Phi());
    }
    
  }
  return matches;
}

void ZETauRecoSelectionEfficiencyAnalyzer::InitialiseEfficiencyAndPurityPlots()
{
  int histo_nbins[]={40,60,60,40,60,60};
  double histo_llim[]={0,-3,-3.2,0,-3,-3.2};
  double histo_ulim[]={80,3,3.2,80,3,3.2};
  
  std::vector<TString> hltElectronStringVec,gsfElectronStringVec,gsfEtCutElectronStringVec,
    allTauJetStringVec,cleanTauJetStringVec,isoTauJetStringVec,prongTauJetStringVec;
  
  hltElectronStringVec.push_back("hltElecEt");hltElectronStringVec.push_back("hltElecEta");
  hltElectronStringVec.push_back("hltElecPhi");hltElectronStringVec.push_back("hltMatchElecEt");
  hltElectronStringVec.push_back("hltMatchElecEta");hltElectronStringVec.push_back("hltMatchElecPhi");
  gsfElectronStringVec.push_back("gsfElecEt");gsfElectronStringVec.push_back("gsfElecEta");
  gsfElectronStringVec.push_back("gsfElecPhi");gsfElectronStringVec.push_back("gsfMatchElecEt");
  gsfElectronStringVec.push_back("gsfMatchElecEta");gsfElectronStringVec.push_back("gsfMatchElecPhi");
  gsfEtCutElectronStringVec.push_back("gsfEtCutElecEt");gsfEtCutElectronStringVec.push_back("gsfEtCutElecEta");
  gsfEtCutElectronStringVec.push_back("gsfEtCutElecPhi");gsfEtCutElectronStringVec.push_back("gsfMatchEtCutElecEt");
  gsfEtCutElectronStringVec.push_back("gsfMatchEtCutElecEta");gsfEtCutElectronStringVec.push_back("gsfMatchEtCutElecPhi");

  allTauJetStringVec.push_back("allTauJetEt");allTauJetStringVec.push_back("allTauJetEta");allTauJetStringVec.push_back("allTauJetPhi");
  allTauJetStringVec.push_back("allMatchTauJetEt");allTauJetStringVec.push_back("allMatchTauJetEta");allTauJetStringVec.push_back("allMatchTauJetPhi");
  cleanTauJetStringVec.push_back("cleanTauJetEt");cleanTauJetStringVec.push_back("cleanTauJetEta");cleanTauJetStringVec.push_back("cleanTauJetPhi");
  cleanTauJetStringVec.push_back("cleanMatchTauJetEt");cleanTauJetStringVec.push_back("cleanMatchTauJetEta");cleanTauJetStringVec.push_back("cleanMatchTauJetPhi");
  isoTauJetStringVec.push_back("isoTauJetEt");isoTauJetStringVec.push_back("isoTauJetEta");isoTauJetStringVec.push_back("isoTauJetPhi");
  isoTauJetStringVec.push_back("isoMatchTauJetEt");isoTauJetStringVec.push_back("isoMatchTauJetEta");isoTauJetStringVec.push_back("isoMatchTauJetPhi");
  prongTauJetStringVec.push_back("prongTauJetEt");prongTauJetStringVec.push_back("prongTauJetEta");prongTauJetStringVec.push_back("prongTauJetPhi");
  prongTauJetStringVec.push_back("prongMatchTauJetEt");prongTauJetStringVec.push_back("prongMatchTauJetEta");prongTauJetStringVec.push_back("prongMatchTauJetPhi");

  hltElectronHistVec.push_back(hltElecEt);hltElectronHistVec.push_back(hltElecEta);
  hltElectronHistVec.push_back(hltElecPhi);hltElectronHistVec.push_back(hltMatchElecEt);
  hltElectronHistVec.push_back(hltMatchElecEta);hltElectronHistVec.push_back(hltMatchElecPhi);
  gsfElectronHistVec.push_back(gsfElecEt);gsfElectronHistVec.push_back(gsfElecEta);
  gsfElectronHistVec.push_back(gsfElecPhi);gsfElectronHistVec.push_back(gsfMatchElecEt);
  gsfElectronHistVec.push_back(gsfMatchElecEta);gsfElectronHistVec.push_back(gsfMatchElecPhi);
  gsfEtCutElectronHistVec.push_back(gsfEtCutElecEt);gsfEtCutElectronHistVec.push_back(gsfEtCutElecEta);
  gsfEtCutElectronHistVec.push_back(gsfEtCutElecPhi);gsfEtCutElectronHistVec.push_back(gsfMatchEtCutElecEt);
  gsfEtCutElectronHistVec.push_back(gsfMatchEtCutElecEta);gsfEtCutElectronHistVec.push_back(gsfMatchEtCutElecPhi);

  allTauJetHistVec.push_back(allTauJetEt);allTauJetHistVec.push_back(allTauJetEta);allTauJetHistVec.push_back(allTauJetPhi);
  allTauJetHistVec.push_back(allMatchTauJetEt);allTauJetHistVec.push_back(allMatchTauJetEta);allTauJetHistVec.push_back(allMatchTauJetPhi);
  cleanTauJetHistVec.push_back(cleanTauJetEt);cleanTauJetHistVec.push_back(cleanTauJetEta);cleanTauJetHistVec.push_back(cleanTauJetPhi);
  cleanTauJetHistVec.push_back(cleanMatchTauJetEt);cleanTauJetHistVec.push_back(cleanMatchTauJetEta);cleanTauJetHistVec.push_back(cleanMatchTauJetPhi);
  isoTauJetHistVec.push_back(isoTauJetEt);isoTauJetHistVec.push_back(isoTauJetEta);isoTauJetHistVec.push_back(isoTauJetPhi);
  isoTauJetHistVec.push_back(isoMatchTauJetEt);isoTauJetHistVec.push_back(isoMatchTauJetEta);isoTauJetHistVec.push_back(isoMatchTauJetPhi);
  prongTauJetHistVec.push_back(prongTauJetEt);prongTauJetHistVec.push_back(prongTauJetEta);prongTauJetHistVec.push_back(prongTauJetPhi);
  prongTauJetHistVec.push_back(prongMatchTauJetEt);prongTauJetHistVec.push_back(prongMatchTauJetEta);prongTauJetHistVec.push_back(prongMatchTauJetPhi);

  TString tau_xaxis[]={"MC #tau jet E_{T}","MC #tau_jet #eta","MC #tau_jet #phi"} ;
  TString e_xaxis[]={"MC e E_{T}","MC e #eta","MC e #phi"} ;
  for(int i=0;i<6;i++)
    {
      hltElectronHistVec[i]=new TH1D(hltElectronStringVec[i],hltElectronStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      gsfElectronHistVec[i]=new TH1D(gsfElectronStringVec[i],gsfElectronStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      gsfEtCutElectronHistVec[i]=new TH1D(gsfEtCutElectronStringVec[i],gsfEtCutElectronStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      allTauJetHistVec[i]=new TH1D(allTauJetStringVec[i],allTauJetStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      cleanTauJetHistVec[i]=new TH1D(cleanTauJetStringVec[i],cleanTauJetStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      isoTauJetHistVec[i]=new TH1D(isoTauJetStringVec[i],isoTauJetStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
      prongTauJetHistVec[i]=new TH1D(prongTauJetStringVec[i],prongTauJetStringVec[i],histo_nbins[i],histo_llim[i],histo_ulim[i]);
    }
  
  hAllElecEt=new TH1D("AllElecEt","AllElecEt;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  hAllElecEta=new TH1D("AllElecEta","AllElecEta;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  hAllElecPhi=new TH1D("AllElecPhi","AllElecPhi;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);

  hAllElecEt_Fid=new TH1D("AllElecEt_Fid","AllElecEt_Fid;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  hAllElecEta_Fid=new TH1D("AllElecEta_Fid","AllElecEta_Fid;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  hAllElecPhi_Fid=new TH1D("AllElecPhi_Fid","AllElecPhi_Fid;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);

  hAllTauEt=new TH1D("AllTauEt","AllTauEt;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  hAllTauEta=new TH1D("AllTauEta","AllTauEta;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  hAllTauPhi=new TH1D("AllTauPhi","AllTauPhi;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);

  hltElecEtEff=new TH1D("hltElecEtEff","hltElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  hltElecEtaEff=new TH1D("hltElecEtaEff","hltElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  hltElecPhiEff=new TH1D("hltElecPhiEff","hltElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  gsfElecEtEff=new TH1D("gsfElecEtEff","gsfElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  gsfElecEtaEff=new TH1D("gsfElecEtaEff","gsfElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  gsfElecPhiEff=new TH1D("gsfElecPhiEff","gsfElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  gsfEtCutElecEtEff=new TH1D("gsfEtCutElecEtEff","gsfEtCutElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  gsfEtCutElecEtaEff=new TH1D("gsfEtCutElecEtaEff","gsfEtCutElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  gsfEtCutElecPhiEff=new TH1D("gsfEtCutElecPhiEff","gsfEtCutElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);  

  allTauJetEtEff=new TH1D("allTauJetEtEff","allTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  allTauJetEtaEff=new TH1D("allTauJetEtaEff","allTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  allTauJetPhiEff=new TH1D("allTauJetPhiEff","allTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  cleanTauJetEtEff=new TH1D("cleanTauJetEtEff","cleanTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  cleanTauJetEtaEff=new TH1D("cleanTauJetEtaEff","cleanTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  cleanTauJetPhiEff=new TH1D("cleanTauJetPhiEff","cleanTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  isoTauJetEtEff=new TH1D("isoTauJetEtEff","isoTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  isoTauJetEtaEff=new TH1D("isoTauJetEtaEff","isoTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  isoTauJetPhiEff=new TH1D("isoTauJetPhiEff","isoTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  prongTauJetEtEff=new TH1D("prongTauJetEtEff","prongTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  prongTauJetEtaEff=new TH1D("prongTauJetEtaEff","prongTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  prongTauJetPhiEff=new TH1D("prongTauJetPhiEff","prongTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);

  hltMatchElecEtEff=new TH1D("hltMatchElecEtEff","hltMatchElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  hltMatchElecEtaEff=new TH1D("hltMatchElecEtaEff","hltMatchElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  hltMatchElecPhiEff=new TH1D("hltMatchElecPhiEff","hltMatchElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  gsfMatchElecEtEff=new TH1D("gsfMatchElecEtEff","gsfMatchElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  gsfMatchElecEtaEff=new TH1D("gsfMatchElecEtaEff","gsfMatchElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  gsfMatchElecPhiEff=new TH1D("gsfMatchElecPhiEff","gsfMatchElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  gsfMatchEtCutElecEtEff=new TH1D("gsfMatchEtCutElecEtEff","gsfMatchEtCutElecEtEff;"+e_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  gsfMatchEtCutElecEtaEff=new TH1D("gsfMatchEtCutElecEtaEff","gsfMatchEtCutElecEtaEff;"+e_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  gsfMatchEtCutElecPhiEff=new TH1D("gsfMatchEtCutElecPhiEff","gsfMatchEtCutElecPhiEff;"+e_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);  


  allMatchTauJetEtEff=new TH1D("allMatchTauJetEtEff","allMatchTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  allMatchTauJetEtaEff=new TH1D("allMatchTauJetEtaEff","allMatchTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  allMatchTauJetPhiEff=new TH1D("allMatchTauJetPhiEff","allMatchTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  cleanMatchTauJetEtEff=new TH1D("cleanMatchTauJetEtEff","cleanMatchTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  cleanMatchTauJetEtaEff=new TH1D("cleanMatchTauJetEtaEff","cleanMatchTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  cleanMatchTauJetPhiEff=new TH1D("cleanMatchTauJetPhiEff","cleanMatchTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  isoMatchTauJetEtEff=new TH1D("isoMatchTauJetEtEff","isoMatchTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  isoMatchTauJetEtaEff=new TH1D("isoMatchTauJetEtaEff","isoMatchTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  isoMatchTauJetPhiEff=new TH1D("isoMatchTauJetPhiEff","isoMatchTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);
  prongMatchTauJetEtEff=new TH1D("prongMatchTauJetEtEff","prongMatchTauJetEtEff;"+tau_xaxis[0],histo_nbins[0],histo_llim[0],histo_ulim[0]);
  prongMatchTauJetEtaEff=new TH1D("prongMatchTauJetEtaEff","prongMatchTauJetEtaEff;"+tau_xaxis[1],histo_nbins[1],histo_llim[1],histo_ulim[1]);
  prongMatchTauJetPhiEff=new TH1D("prongMatchTauJetPhiEff","prongMatchTauJetPhiEff;"+tau_xaxis[2],histo_nbins[2],histo_llim[2],histo_ulim[2]);

  hltElecEtEff->Sumw2();hltElecEtaEff->Sumw2();hltElecPhiEff->Sumw2();
  gsfElecEtEff->Sumw2();gsfElecEtaEff->Sumw2();gsfElecPhiEff->Sumw2();
  gsfEtCutElecEtEff->Sumw2();gsfEtCutElecEtaEff->Sumw2();gsfEtCutElecPhiEff->Sumw2();
  hltMatchElecEtEff->Sumw2();hltMatchElecEtaEff->Sumw2();hltMatchElecPhiEff->Sumw2();
  gsfMatchElecEtEff->Sumw2();gsfMatchElecEtaEff->Sumw2();gsfMatchElecPhiEff->Sumw2();
  gsfMatchEtCutElecEtEff->Sumw2();gsfMatchEtCutElecEtaEff->Sumw2();gsfMatchEtCutElecPhiEff->Sumw2();

  allTauJetEtEff->Sumw2();allTauJetEtaEff->Sumw2();allTauJetPhiEff->Sumw2();
  cleanTauJetEtEff->Sumw2();cleanTauJetEtaEff->Sumw2();cleanTauJetPhiEff->Sumw2();
  isoTauJetEtEff->Sumw2();isoTauJetEtaEff->Sumw2();isoTauJetPhiEff->Sumw2();
  prongTauJetEtEff->Sumw2();prongTauJetEtaEff->Sumw2();prongTauJetPhiEff->Sumw2();

  allMatchTauJetEtEff->Sumw2();allMatchTauJetEtaEff->Sumw2();allMatchTauJetPhiEff->Sumw2();
  cleanMatchTauJetEtEff->Sumw2();cleanMatchTauJetEtaEff->Sumw2();cleanMatchTauJetPhiEff->Sumw2();
  isoMatchTauJetEtEff->Sumw2();isoMatchTauJetEtaEff->Sumw2();isoMatchTauJetPhiEff->Sumw2();
  prongMatchTauJetEtEff->Sumw2();prongMatchTauJetEtaEff->Sumw2();prongMatchTauJetPhiEff->Sumw2();


  hltDRMatching=new TH1D("hltDRMatching","hltDRMatching",1000,0,0.5);
  gsfDRMatching=new TH1D("gsfDRMatching","gsfDRMatching",1000,0,0.5);

}

void ZETauRecoSelectionEfficiencyAnalyzer::MakeEfficiencyPurityHistos()
{
  hltElecEtEff->Divide(hltElectronHistVec[3],hAllElecEt,1.,1.,"b");
  hltElecEtaEff->Divide(hltElectronHistVec[4],hAllElecEta,1.,1.,"b");
  hltElecPhiEff->Divide(hltElectronHistVec[5],hAllElecPhi,1.,1.,"b");

  hltMatchElecEtEff->Divide(hltElectronHistVec[3],hltElectronHistVec[0],1.,1.,"b");
  hltMatchElecEtaEff->Divide(hltElectronHistVec[4],hltElectronHistVec[1],1.,1.,"b");
  hltMatchElecPhiEff->Divide(hltElectronHistVec[5],hltElectronHistVec[2],1.,1.,"b");

  gsfElecEtEff->Divide(gsfElectronHistVec[3],hltElectronHistVec[0],1.,1.,"b");
  gsfElecEtaEff->Divide(gsfElectronHistVec[4],hltElectronHistVec[1],1.,1.,"b");
  gsfElecPhiEff->Divide(gsfElectronHistVec[5],hltElectronHistVec[2],1.,1.,"b");

  gsfMatchElecEtEff->Divide(gsfElectronHistVec[3],gsfElectronHistVec[0],1.,1.,"b");
  gsfMatchElecEtaEff->Divide(gsfElectronHistVec[4],gsfElectronHistVec[1],1.,1.,"b");
  gsfMatchElecPhiEff->Divide(gsfElectronHistVec[5],gsfElectronHistVec[2],1.,1.,"b");
  
  gsfEtCutElecEtEff->Divide(gsfEtCutElectronHistVec[3],hltElectronHistVec[0],1.,1.,"b");
  gsfEtCutElecEtaEff->Divide(gsfEtCutElectronHistVec[4],hltElectronHistVec[1],1.,1.,"b");
  gsfEtCutElecPhiEff->Divide(gsfEtCutElectronHistVec[5],hltElectronHistVec[2],1.,1.,"b");

  gsfMatchEtCutElecEtEff->Divide(gsfEtCutElectronHistVec[3],gsfElectronHistVec[0],1.,1.,"b");
  gsfMatchEtCutElecEtaEff->Divide(gsfEtCutElectronHistVec[4],gsfElectronHistVec[1],1.,1.,"b");
  gsfMatchEtCutElecPhiEff->Divide(gsfEtCutElectronHistVec[5],gsfElectronHistVec[2],1.,1.,"b");

  allTauJetEtEff->Divide(allTauJetHistVec[3],allTauJetHistVec[0],1.,1.,"b");
  allTauJetEtaEff->Divide(allTauJetHistVec[4],allTauJetHistVec[1],1.,1.,"b");
  allTauJetPhiEff->Divide(allTauJetHistVec[5],allTauJetHistVec[2],1.,1.,"b");

  allMatchTauJetEtEff->Divide(allTauJetHistVec[3],allTauJetHistVec[0],1.,1.,"b");
  allMatchTauJetEtaEff->Divide(allTauJetHistVec[4],allTauJetHistVec[1],1.,1.,"b");
  allMatchTauJetPhiEff->Divide(allTauJetHistVec[5],allTauJetHistVec[2],1.,1.,"b");

  cleanTauJetEtEff->Divide(cleanTauJetHistVec[3],allTauJetHistVec[0],1.,1.,"b");
  cleanTauJetEtaEff->Divide(cleanTauJetHistVec[4],allTauJetHistVec[1],1.,1.,"b");
  cleanTauJetPhiEff->Divide(cleanTauJetHistVec[5],allTauJetHistVec[2],1.,1.,"b");

  cleanMatchTauJetEtEff->Divide(cleanTauJetHistVec[3],cleanTauJetHistVec[0],1.,1.,"b");
  cleanMatchTauJetEtaEff->Divide(cleanTauJetHistVec[4],cleanTauJetHistVec[1],1.,1.,"b");
  cleanMatchTauJetPhiEff->Divide(cleanTauJetHistVec[5],cleanTauJetHistVec[2],1.,1.,"b");

  isoTauJetEtEff->Divide(isoTauJetHistVec[3],allTauJetHistVec[0],1.,1.,"b");
  isoTauJetEtaEff->Divide(isoTauJetHistVec[4],allTauJetHistVec[1],1.,1.,"b");
  isoTauJetPhiEff->Divide(isoTauJetHistVec[5],allTauJetHistVec[2],1.,1.,"b");

  isoMatchTauJetEtEff->Divide(isoTauJetHistVec[3],isoTauJetHistVec[0],1.,1.,"b");
  isoMatchTauJetEtaEff->Divide(isoTauJetHistVec[4],isoTauJetHistVec[1],1.,1.,"b");
  isoMatchTauJetPhiEff->Divide(isoTauJetHistVec[5],isoTauJetHistVec[2],1.,1.,"b");

  prongTauJetEtEff->Divide(prongTauJetHistVec[3],allTauJetHistVec[0],1.,1.,"b");
  prongTauJetEtaEff->Divide(prongTauJetHistVec[4],allTauJetHistVec[1],1.,1.,"b");
  prongTauJetPhiEff->Divide(prongTauJetHistVec[5],allTauJetHistVec[2],1.,1.,"b");

  prongMatchTauJetEtEff->Divide(prongTauJetHistVec[3],prongTauJetHistVec[0],1.,1.,"b");
  prongMatchTauJetEtaEff->Divide(prongTauJetHistVec[4],prongTauJetHistVec[1],1.,1.,"b");
  prongMatchTauJetPhiEff->Divide(prongTauJetHistVec[5],prongTauJetHistVec[2],1.,1.,"b");

}
