#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include <stdlib.h>
#include <string.h>

#include "RecoTBCalo/H2TBAnalysis/interface/JetAnalysis.h"

typedef CaloJetCollection::const_iterator CalJetIter;
typedef GenJetCollection::const_iterator GenJetIter;

JetAnalysis::JetAnalysis() {
  m_file=0; // set to null
  evtCounter=0;

  //set parameter defaults 
  _Monte=false;
  _EtaMin=-5.2;
  _EtaMax=5.2;
  _HistName="test.root";
  
}



/** Setup the analysis to put the histograms into HistoFile and focus on
      ieta,iphi for analysis.
  */
void JetAnalysis::setup(const edm::ParameterSet& pSet) {


  edm::ParameterSet myJetParams = pSet.getParameter<edm::ParameterSet>("RunParameters") ;
  vector<std::string> parameterNames = myJetParams.getParameterNames() ;
  
  for ( vector<std::string>::iterator iParam = parameterNames.begin();
	iParam != parameterNames.end(); iParam++ ){
    if  ( (*iParam) == "Monte" ) _Monte =  myJetParams.getParameter<bool>( *iParam );
    else if ( (*iParam) == "EtaMin" ) _EtaMin =  myJetParams.getParameter<double>( *iParam );
    else if ( (*iParam) == "EtaMax" ) _EtaMax =  myJetParams.getParameter<double>( *iParam );
    else if ( (*iParam) == "jetHistoFile" ) _HistName =  myJetParams.getParameter<string>( *iParam );
  }

  cout << "---------- Input Parameters ---------------------------" << endl;
  cout << "  Monte:  " << _Monte << endl;    
  cout << "  EtaMin: " << _EtaMin << endl;    
  cout << "  EtaMax: " << _EtaMax << endl;    
  cout << "  Output histograms written to: " << _HistName << std::endl;
  cout << "-------------------------------------------------------" << endl;  
  // open the histogram file

  m_file=new TFile(_HistName.c_str(),"RECREATE");
  m_file->mkdir(EnergyDir());
  m_file->mkdir(PulseDir());
  m_file->cd();
  bookHistograms();

  for(int ieta=0;ieta<NETA;ieta++){
    cout << " ieta " << ieta << " eta min " << CaloTowerEtaBoundries[ieta] <<endl;
  }

}
void JetAnalysis::fillHist(const TString& histName, const Double_t& value, const Double_t& wt) {

  hid=m_HistNames.find(histName);
  if (hid==m_HistNames.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  else
    hid->second->Fill(value,wt); 

}

void JetAnalysis::fillHist(const TString& histName, const Double_t& x,const Double_t& y,const Double_t& wt) {

  hid2D=m_HistNames2D.find(histName);
  if (hid2D==m_HistNames2D.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  else
    hid2D->second->Fill(x,y,wt); 

}

void JetAnalysis::bookHistograms() {

  bookGeneralHistograms();

  bookTBTriggerHists();

  bookCaloTowerHists();

  if (_Monte) bookJetHistograms("Gen");
  bookJetHistograms("Calo");

  if (_Monte) bookMetHists("Gen");
  bookMetHists("Calo");

  if (_Monte) bookCalculateEfficiency();

  if (_Monte) bookDiJetBalance("Gen");
  bookDiJetBalance("Calo");

  if (_Monte) bookMCParticles();

}
/** Analyze the hits */
void JetAnalysis::analyze( const CaloJetCollection& calojets,
			   const GenJetCollection& genjets,
			   const METCollection& recmets,
			   const METCollection& genmets,
			   const CaloTowerCollection& caloTowers,
			   const HepMC::GenEvent mctruth,
			   const HBHERecHitCollection& hbhe_hits, 
			   const HBHEDigiCollection& hbhe_digis, 
			   const HORecHitCollection& ho_hits, 
			   const HODigiCollection& ho_digis, 
			   const HFRecHitCollection& hf_hits, 
			   const HFDigiCollection& hf_digis, 
			   const HcalTBTriggerData& trigger,
			   const CaloGeometry& geom) {

  //std::cout << " Beginning JetAnalysis " << std::endl;

  (&calojets) ? doCaloJets=true : doCaloJets=false;
  (&genjets) ? doGenJets=true : doGenJets=false;
 
  fillHist("Nevents",1.0);

  fillTBTriggerHists(trigger);

  // Make a copy, so that you can sort

  CaloJetCollection mycalojets;
  if (doCaloJets) {
    mycalojets=calojets;
    std::sort(mycalojets.begin(),mycalojets.end(),PtGreater());
  }

  GenJetCollection mygenjets;
  if (doGenJets) {
    mygenjets=genjets;
    std::sort(mygenjets.begin(),mygenjets.end(),PtGreater());
  }

  // fill calojet and genjet hists 
  fillJetHists(mycalojets,"Calo");
  if (_Monte) fillJetHists(mygenjets,"Gen");

  // fill recmet and genjet hists
  fillMetHists(recmets,"Calo");
  if (_Monte) fillMetHists(genmets,"Gen");

  // fill CaloTower hists
  fillCaloTowerHists(caloTowers);

  if (_Monte) CalculateEfficiency(mygenjets,mycalojets);

  DiJetBalance(mycalojets,"Calo");
  if (_Monte)DiJetBalance(mygenjets,"Gen");

  if (_Monte) fillMCParticles(mctruth);

  // Plot RecHits
  fillRecHits(hbhe_hits);
  fillRecHits(ho_hits);
  fillRecHits(hf_hits);

  // Plot Digis
  fillDigis(hbhe_digis);
  fillDigis(ho_digis);
  fillDigis(hf_digis);


}

void JetAnalysis::bookGeneralHistograms() {


  TString hname="Nevents"; TString htitle="Number of events";

  m_HistNames[hname]= new TH1F(hname,htitle,5,0.0,5.0);
}

void JetAnalysis::bookTBTriggerHists() {


  TString hname="Trigger"; TString htitle="Trigger Histogram";

  TH1F* id =new TH1F(hname,htitle,5,0.0,5.0);
  id->GetXaxis()->SetBinLabel(1,trigBeam());
  id->GetXaxis()->SetBinLabel(2,trigIped());
  id->GetXaxis()->SetBinLabel(3,trigOped());
  id->GetXaxis()->SetBinLabel(4,trigLED());
  id->GetXaxis()->SetBinLabel(5,trigLaser());

  m_HistNames[hname]= id;

}

void JetAnalysis::fillTBTriggerHists(const HcalTBTriggerData& trigger){

  // kick out if there is no  HcalTBTriggerData
  if (! &trigger) return;

  TString hname="Trigger";

  hid=m_HistNames.find(hname);
  if (hid==m_HistNames.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << hname << std::endl;
  else
    {
      if (trigger.wasBeamTrigger()) hid->second->Fill(trigBeam(),1.0);
      if (trigger.wasInSpillPedestalTrigger()) hid->second->Fill(trigIped(),1.0);
      if (trigger.wasOutSpillPedestalTrigger()) hid->second->Fill(trigOped(),1.0);
      if (trigger.wasLEDTrigger()) hid->second->Fill(trigLED(),1.0);
      if (trigger.wasLaserTrigger()) hid->second->Fill(trigLaser(),1.0);
    }
}

void JetAnalysis::bookJetHistograms(const TString& prefix) {

  TString hname;
  TString htitle;

  std::ostringstream ch_eta; ch_eta << etaBarrel();
  std::ostringstream ch_etamin; ch_etamin << _EtaMin;
  std::ostringstream ch_etamax; ch_etamax << _EtaMax;

  TString h_EtaRng= ch_etamin.str() + " < #eta < " + ch_etamax.str();
  
  // book rec and gen jet histograms
  Int_t netbins=80, nengbins=100;
  Double_t etmin=0.,etmax=400.,engmin=0.,engmax=500.;

  hname=prefix + "et"; htitle=prefix+" Jet E_{T} -- " + h_EtaRng;
  m_HistNames[hname]= new TH1F(hname,htitle,netbins,etmin,etmax);
  hname=prefix + "pt"; htitle=prefix+" Jet p_{T} -- " + h_EtaRng;
  m_HistNames[hname]= new TH1F(hname,htitle,netbins,etmin,etmax);
  
  hname=prefix + "energy"; htitle=prefix+" Jet Energy -- " + h_EtaRng;
  m_HistNames[hname]= new TH1F(hname,htitle,nengbins,engmin,engmax);

  float deltaEta=0.1,deltaPhi=0.1;
  Int_t netabins=int((_EtaMax-_EtaMin)/deltaEta);
  Int_t nphibins=int(2.*M_PI/deltaPhi);

  hname=prefix + "phi" ;  htitle=prefix+" Jet #phi -- " + h_EtaRng;
  m_HistNames[hname] = new TH1F(hname,htitle,nphibins,-M_PI,M_PI);
  hname=prefix + "eta"; htitle=prefix+" Jet #eta -- " + h_EtaRng;
  m_HistNames[hname] = new TH1F(hname,htitle,netabins,_EtaMin,_EtaMax);


  hname=prefix + "et_Barrel";  htitle=prefix+" Jet E_{T} -- |#eta| < " + ch_eta.str();
  m_HistNames[hname] = new TH1F(hname,htitle,netbins,etmin,etmax);
  hname=prefix + "pt_Barrel";  htitle=prefix+" Jet p_{T} -- |#eta| < " + ch_eta.str();
  m_HistNames[hname] = new TH1F(hname,htitle,netbins,etmin,etmax);

  hname=prefix + "etmax"; htitle=prefix+" Max Jet E_{T} -- |#eta| < " + ch_eta.str();
  m_HistNames[hname] = new TH1F(hname,htitle,netbins,etmin,etmax);
  hname=prefix + "ptmax"; htitle=prefix+" Max Jet p_{T} -- |#eta| < " + ch_eta.str();
  m_HistNames[hname] = new TH1F(hname,htitle,netbins,etmin,etmax);


}

void JetAnalysis::bookForId(const HcalDetId& id) {

  std::ostringstream ss,ss_t1,ss_t2,st;

  ss << "Energy " << id;
  ss_t1 << "LowEnergy" << id;
  ss_t2 << "Energy" << id;

  //std::cout << "Booking Histograms for HCAL Subdetector: " << id.subdet() 
  //	    << " Name: " << ss.str() << std::endl;

  Int_t nbins;
  Double_t xmin,xmax;

  switch (id.subdet()){
  case (HcalBarrel):{
    nbins=300; xmin=0.; xmax=600.;
  } break;
  case (HcalOuter):{
    nbins=100; xmin=0.; xmax=100.;
  } break;
  default:{
    nbins=300; xmin=0.; xmax=600.;
  }
  }
  channelmap1[id]=new TH1F(ss_t1.str().c_str(),ss.str().c_str(),520,-3.0,10.0);
  channelmap2[id]=new TH1F(ss_t2.str().c_str(),ss.str().c_str(),nbins,xmin,xmax);  
}

void JetAnalysis::bookForId_TS(const HcalDetId& id) {

  std::ostringstream ss,st;
  ss << "ADC" << id;
  st << "fC" << id;

  digimap1[id]=new TH1I(ss.str().c_str(),ss.str().c_str(),10,-0.5,9.5);
  digimap2[id]=new TH1F(st.str().c_str(),st.str().c_str(),10,-0.5,9.5);

}

template <typename T> void JetAnalysis::fillRecHits(const T& hits) {

  // kick out if RecHit data structure does not exist
  if (! &hits) return;

  std::map<HcalDetId,TH1*>::iterator h1,h2;

  m_file->cd(EnergyDir());
  typedef typename T::const_iterator iter;
  for ( iter i=hits.begin(); i!=hits.end(); i++) {

    h1=channelmap1.find(i->id()); // look for a histogram with this hit's id
    h2=channelmap2.find(i->id()); // look for a histogram with this hit's id
    if (h1==channelmap1.end()) {
      bookForId(i->id());
      h1=channelmap1.find(i->id()); // look for a histogram with this hit's id
      h2=channelmap2.find(i->id()); // look for a histogram with this hit's id
    }
    //std::cout << "Energy: " << i->energy() << " hist1: " << h1->second->GetTitle() << endl;
    h1->second->Fill(i->energy()); // if it's there, fill it with energy
    h2->second->Fill(i->energy()); // if it's there, fill it with energy
  }
  m_file->cd();
}


template <typename T> void JetAnalysis::fillDigis(const T& digis) {

  // kick out if Digi data structure does not exist
  if (! &digis) return;

  std::map<HcalDetId,TH1*>::iterator h1,h2;

  m_file->cd(PulseDir());
  typedef typename T::const_iterator iter;

  for ( iter ii=digis.begin(); ii!=digis.end(); ii++) {
    h1=digimap1.find(ii->id()); // look for a histogram with this hit's id
    h2=digimap2.find(ii->id()); // look for a histogram with this hit's id
    if (h1==digimap1.end()) {
      bookForId_TS(ii->id());
      h1=digimap1.find(ii->id()); // look for a histogram with this hit's id
      h2=digimap2.find(ii->id()); // look for a histogram with this hit's id
    }
    for (int indx=0; indx<ii->size(); indx++) {
      int i_adc=ii->sample(indx).adc();
      //std::cout << " LA: " << ii->sample(indx) << ii->sample(indx).capid() << std::endl;
      h1->second->Fill(indx,i_adc);
      Double_t wt=ii->sample(indx).nominal_fC();
      //std::cout << i_adc << " " << wt << std::endl;
      h2->second->Fill(indx,wt);
    }
  }
  m_file->cd();
}

template <typename T> void JetAnalysis::fillJetHists(const T& jets, const TString& prefix) {

  // kick out if Jet collection does not exist
  if (! &jets) return;

  typedef typename T::const_iterator iter;
  
  Double_t maxEt=0.,maxPt=0.;
  for ( iter i=jets.begin(); i!=jets.end(); i++) {

    Double_t jetEng = i->energy();
    Double_t jetEt = i->et();
    Double_t jetPt = i->pt();
    Double_t jetEta = i->eta();
    Double_t jetPhi = i->phi();

    if (jetEta > _EtaMin && jetEta < _EtaMax){
      fillHist(prefix + "energy",jetEng);
      fillHist(prefix + "et",jetEt);
      fillHist(prefix + "pt",jetPt);

      fillHist(prefix + "eta",jetEta);
      fillHist(prefix + "phi",jetPhi);

      if (fabs(jetEta) < etaBarrel()){
	fillHist(prefix + "et_Barrel",jetEt);
	fillHist(prefix + "pt_Barrel",jetPt);
	if (jetEt > maxEt) maxEt = jetEt;
	if (jetPt > maxPt) maxPt = jetPt;
      }
    }
  }
  fillHist(prefix + "etmax",maxEt);
  fillHist(prefix + "ptmax",maxPt);
}

void JetAnalysis::bookCaloTowerHists() {

  TString hname;
  int netbins=50;
  float etmin=0.; float etmax=50.;

  hname="CaloTowerEt";
  m_HistNames[hname] = new TH1F(hname,"CaloTower E_{T}",netbins,etmin,etmax);

  hname="CaloTowerEta";
  m_HistNames[hname] = new TH1F(hname,"CaloTower #eta",110,-5.5,5.5);


  hname="CaloTowerEnergy";
  m_HistNames[hname] = new TH1F(hname,"CaloTower Energy",100,0.0,100.);


  hname="CaloTowerEmEnergy";
  m_HistNames[hname] = new TH1F(hname,"CaloTower EmEnergy",100,0.0,100.);

  hname="CaloTowerHadEnergy";
  m_HistNames[hname] = new TH1F(hname,"CaloTower HadEnergy",100,0.0,100.);

  hname="CaloTowerOuterEnergy";
  m_HistNames[hname] = new TH1F(hname,"CaloTower Outer Energy",100,0.0,100.);

  hname="CaloTowerEnergyEtaPhi";
  //  TH2F* f2d = new TH2F(hname,"CaloTower Outer Energy",100,0.0,100.,100,-3,+3);
  m_HistNames2D[hname] = new TH2F(hname,"CaloTower Energy",110,-5.5,5.5,72,-M_PI,+M_PI);


}
void JetAnalysis::fillCaloTowerHists(const CaloTowerCollection& caloTowers) {

  // kick out if there is no CaloTower collection
  if (! &caloTowers) return;

  for ( CaloTowerCollection::const_iterator tower=caloTowers.begin(); 
	tower!=caloTowers.end(); tower++) {

    Double_t et=tower->et();
    Double_t eta=tower->eta();
    Double_t phi=tower->phi();

    if (et<1.) continue; 

    Double_t  totEnergy= tower->energy();
    Double_t  emEnergy= tower->emEnergy();
    Double_t  hadEnergy= tower->hadEnergy();
    Double_t  outerEnergy= tower->outerEnergy();


    fillHist("CaloTowerEt",et);
    fillHist("CaloTowerEta",eta);
    fillHist("CaloTowerEnergy",totEnergy);
    fillHist("CaloTowerEmEnergy",emEnergy);
    fillHist("CaloTowerHadEnergy",hadEnergy);
    fillHist("CaloTowerOuterEnergy",outerEnergy);

    fillHist("CaloTowerEnergyEtaPhi",eta,phi,totEnergy);

  }
}
void JetAnalysis::bookMetHists(const TString& prefix) {

  TString hname;
  TString htitle;

  hname=prefix + "num";
  htitle = prefix+" Number of MET objects";
  m_HistNames[hname] = new TH1I(hname,htitle,10,0.,10.);

  hname=prefix+"etMiss";
  htitle=prefix+" Missing Et";
  m_HistNames[hname] = new TH1F(hname,htitle,500,0.0,500.);

  hname=prefix+"etMissX";
  htitle=prefix+" Missing Et-X";
  m_HistNames[hname] = new TH1F(hname,htitle,2000,-1000.0,1000.);

  hname=prefix+"etMissPhi";
  htitle=prefix+" Phi of Missing Et";
  m_HistNames[hname] = new TH1F(hname,htitle,100,-M_PI,M_PI);


  hname=prefix+"sumEt";
  htitle=prefix+" Sum Et";
  m_HistNames[hname] = new TH1F(hname,htitle,1400,0.0,14000.);

}
template <typename T> void JetAnalysis::fillMetHists(const T& mets, const TString& prefix) {

  // kick out if Met collection does not exist
  if (! &mets) return;

  Int_t metnum=mets.size();
  fillHist(prefix + "num",metnum);

  typedef typename T::const_iterator iter;
  for ( iter met=mets.begin(); met!=mets.end(); met++) {

    Double_t mEt=met->mEt();
    Double_t sumEt=met->sumEt();
    Double_t mEtPhi=met->phi();
    Double_t mEtX=met->mEx();

    fillHist(prefix + "etMiss",mEt);
    fillHist(prefix + "sumEt",sumEt);
    fillHist(prefix + "etMissX",mEtX);
    fillHist(prefix + "etMissPhi",mEtPhi);
  }
}



void JetAnalysis::dummyAnalyze(
			   const CaloGeometry& geom) {

  std::cout << "Inside dummyAnalyse routine" << std::endl;

}
/** Finalization (close files, etc) */
void JetAnalysis::done() {
  std::cout << "Closing up.\n";
  if (m_file!=0) { // if there was a histogram file...
    m_file->Write(); // write out the histrograms
    delete m_file; // close and delete the file
    m_file=0; // set to zero to clean up
  }
}
void JetAnalysis::bookCalculateEfficiency(){

  TString hname;
  TString htitle;

  hname="rmin";
  htitle = " Minimum Distance between Calo and Gen Jet";
  m_HistNames[hname] = new TH1F(hname,htitle,100,0.0,10.);

  hname="CalPtoGenPt";
  htitle = " CaloJet Pt/ GenJet Pt";
  m_HistNames[hname] = new TH1F(hname,htitle,100,0.0,5.);

}


void JetAnalysis::CalculateEfficiency(GenJetCollection& genjets, CaloJetCollection& calojets){

  // kick out if GenJet or CaloJet collection does not exist
  if ((! &genjets) || (! &calojets)) return;

  const float GenJetPtCut=10;
  const float GenJetEtaCut=1.0;
  const float RCUT=0.25;
 
  for(GenJetIter i=genjets.begin();i!=genjets.end(); i++) {
    Double_t GenJetPt = i->pt();
    Double_t genJetEta = i->eta();
    if(GenJetPt>GenJetPtCut) {

      if(genJetEta<GenJetEtaCut){

	if(calojets.size()>0){
	  float rmin(99);
	  CalJetIter caljet;
	  float  calpt(0);
	  for(CalJetIter j=calojets.begin();j!=calojets.end();j++){
	    float rr=JetUtil::radius(i,j);
	    if(rr<rmin){rmin=rr;caljet=j;calpt=j->pt();}
	  }
	  fillHist("rmin",rmin);
	  if(rmin<RCUT){

	    //	    cout << " radius " << rmin << " GenPt " << GenJetPt << " CalPt " << caljet->pt() <<" rr  " << calpt/GenJetPt <<endl;
	    fillHist("CalPtoGenPt",calpt/GenJetPt);
	  }
	}
      }
    }
  }
}

void JetAnalysis::bookDiJetBalance(const TString& prefix) {
  TString hname;
  TString htitle;
 
  hname=prefix + "DiJetBalance";
  htitle = prefix+" (PtTrigger-PtProbe)/PtAve ";
  m_HistNames[hname] = new TH1F(hname,htitle,100,-2.,2.);

}
template <typename T> void JetAnalysis::DiJetBalance(const T& jets, const TString& prefix) {

  // return for null jet collection
  if (! &jets) return;

  const float TrigEtaCut=1.0;
  const float AvePtCut=10;
  const float DPhiCut=2.7;
  const float PtJet3Cut=8;

  typedef typename T::const_iterator iter;

  if(jets.size()<2) return;

  float PtJet3= (jets.size()>2) ? jets[2].pt() : 0;

  float AvePt=(jets[0].pt()+jets[1].pt())/2.0;

  bool FirstJetIsCentral = (fabs(jets[0].eta())<TrigEtaCut); 
  bool SecondJetIsCentral = (fabs(jets[1].eta())<TrigEtaCut); 


  int trig(-1);
  int probe(-1);

  if( FirstJetIsCentral &&  SecondJetIsCentral) {
    if(jets[0].phi()<jets[1].phi()){ trig=0;probe=1; } // Both central pick trigger randomly, phi is random
    else{trig=1;probe=0;}
  }
  else if(FirstJetIsCentral) {trig=0;probe=1;}
  else if(SecondJetIsCentral){trig=1;probe=0;}
  else {return;}

  float B= (jets[trig].pt()-jets[probe].pt())/AvePt;

  if(AvePt>AvePtCut){
    if(fabs(JetUtil::dPhi(jets[0].phi(),jets[1].phi()))>DPhiCut){
      if(PtJet3<PtJet3Cut){
	fillHist(prefix+"DiJetBalance",B);       
      }
    }
  }
}
void JetAnalysis::bookMCParticles(){

  TString hname;
  TString htitle;
 
  const int imax=1;
  const int jmax=1;


  for(int i=0;i<imax;i++){
    std::ostringstream oi; oi << i;
 
    for(int j=0;j<jmax;++j){
      std::ostringstream oj; oj << j;
 
      istringstream ints(oj.str());

      int k;
      ints>>k;
      cout << k << endl;

      hname="VertexZ"+oi.str()+oi.str();
      m_HistNames[hname] = new TH1F(hname,hname,100,-50.,50.);

      hname="VertexX"+oi.str()+oj.str();
      m_HistNames[hname] = new TH1F(hname,hname,200,-5.,5.);

      hname="VertexY"+oi.str()+oj.str();
      m_HistNames[hname] = new TH1F(hname,hname,200,-5.,5.);

      hname="Pt"+oi.str()+oj.str();
      m_HistNames[hname] = new TH1F(hname,hname,500,0.0,500.);

      hname="Pid"+oi.str()+oj.str();
      m_HistNames[hname] = new TH1F(hname,hname,10000,0.0,10000.);

    }
  }
}

void JetAnalysis::fillMCParticles(const HepMC::GenEvent mctruth){

  // return for null mctruth collection
  if (! &mctruth) return;

  for (HepMC::GenEvent::particle_const_iterator partIter = mctruth.particles_begin(); partIter != mctruth.particles_end();
         ++partIter) {

    // Find the end vertex
    //   for (HepMC::GenEvent::vertex_const_iterator vertIter = mctruth.vertices_begin();
    //   vertIter != mctruth.vertices_end();
    //    ++vertIter) {
       CLHEP::HepLorentzVector creation = (*partIter)->CreationVertex();
       CLHEP::HepLorentzVector momentum = (*partIter)->Momentum();
       HepPDT::ParticleID id = (*partIter)->particleID();  // electrons and positrons are 11 and -11
       //   cout << "MC particle id " << id.pid() << ", creationVertex " << creation << " cm, initialMomentum " << momentum << " GeV/c" << endl;   
       fillHist("Pid00",id.pid()); 
       fillHist("VertexX00",creation.x());  
       fillHist("VertexY00",creation.y());  
       fillHist("VertexZ00",creation.z());  

       fillHist("Pt00",momentum.perp());
  }
}
void Kperp(CLHEP::Hep2Vector jet1,CLHEP::Hep2Vector jet2,float& kperp,float& kparallel){


  float AvePt=(jet1.mag()+jet2.mag())/2.0;

   // unit vector  bisecting  the angle between two leading jets

  CLHEP::Hep2Vector parallel= (jet1.unit()+jet2.unit()).unit();
  CLHEP::Hep2Vector perp = parallel.orthogonal();

   float jparallel= jet1.dot(parallel)*jet2.dot(parallel);
   float jperp = jet1.dot(perp)*jet2.dot(perp);

   kparallel=jparallel/AvePt;
   kperp=jperp/AvePt;

}









