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

#include "RecoTBCalo/H2TBAnalysis/interface/TemplateAnalysis.h"

TemplateAnalysis::TemplateAnalysis() {
  m_file=0; // set to null
  evtCounter=0;

  //set parameter defaults 
  _EtaMin=-5.2;
  _EtaMax=5.2;
  _HistName="test.root";
  
}

/** Setup the analysis to put the histograms into HistoFile and input all other parameters
  */
void TemplateAnalysis::setup(const edm::ParameterSet& pSet) {

  edm::ParameterSet myParams = pSet.getParameter<edm::ParameterSet>("RunParameters") ;
  vector<std::string> parameterNames = myParams.getParameterNames() ;
  
  for ( vector<std::string>::iterator iParam = parameterNames.begin();
	iParam != parameterNames.end(); iParam++ ){
    if ( (*iParam) == "EtaMin" ) _EtaMin =  myParams.getParameter<double>( *iParam );
    else if ( (*iParam) == "EtaMax" ) _EtaMax =  myParams.getParameter<double>( *iParam );
    else if ( (*iParam) == "tempHistoFile" ) _HistName =  myParams.getParameter<string>( *iParam );
  }
  
  cout << "---------- Input Parameters ---------------------------" << endl;
  cout << "  EtaMin: " << _EtaMin << endl;    
  cout << "  EtaMax: " << _EtaMax << endl;    
  cout << "  Output histograms written to: " << _HistName << std::endl;
  cout << "-------------------------------------------------------" << endl;  
  // open the histogram file

  m_file=new TFile(_HistName.c_str(),"RECREATE");
  m_file->mkdir(EnergyDir());
  m_file->cd();
  bookHistograms();

  for(int ieta=0;ieta<NETA;ieta++){
    cout << " ieta " << ieta << " eta min " << CaloTowerEtaBoundries[ieta] <<endl;
  }

  return;
}

void TemplateAnalysis::fillHist(const TString& histName, const Double_t& value, const Double_t& wt) {
  
  hid_iter=m_HistNames.find(histName);
  if (hid_iter==m_HistNames.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  else
    hid_iter->second->Fill(value,wt); 
  return;
}

void TemplateAnalysis::fillHist(const TString& histName, const Double_t& x,const Double_t& y,const Double_t& wt) {

  hid2D_iter=m_HistNames2D.find(histName);
  if (hid2D_iter==m_HistNames2D.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << histName << std::endl;
  else
    hid2D_iter->second->Fill(x,y,wt); 
  return;
}

void TemplateAnalysis::bookHistograms() {

  bookGeneralHistograms();

  bookTBHists();

  bookJetHistograms("Calo");

  return;
}

class myPtGreater {
  public:
  template <typename T> bool operator () (const T& i, const T& j) {
    return (i.pt() > j.pt());
  }
};

/** Analyze the hits */
void TemplateAnalysis::analyze(const CaloJetCollection& calojets,
			       const HBHERecHitCollection& hbhe_hits, 
			       const HORecHitCollection& ho_hits, 
			       const HFRecHitCollection& hf_hits, 
			       const HcalTBTriggerData& trigger,
			       const HcalTBEventPosition& tb_epos,
			       const HcalTBBeamCounters& tb_qadc,
			       const HcalTBTiming& tb_time){

  
  fillHist("Nevents",1.0);

  if(&trigger) fillTBTriggerHists(trigger);
  if(&tb_epos) fillTBBeamHists(tb_epos,tb_qadc,tb_time);

  // Make a copy, so that you can sort
  CaloJetCollection mycalojets=calojets;
  std::sort(mycalojets.begin(),mycalojets.end(),myPtGreater());

  // fill calojet and genjet hists 
  fillJetHists(mycalojets,"Calo");

  // Plot RecHits
  if(&hbhe_hits) fillRecHits(hbhe_hits);
  if(&ho_hits) fillRecHits(ho_hits);
  if(&hf_hits) fillRecHits(hf_hits);

  return; //
}

void TemplateAnalysis::bookGeneralHistograms() {

  TString hname="Nevents"; TString htitle="Number of events";

  m_HistNames[hname]= new TH1F(hname,htitle,5,0.0,5.0);
  return;
}

void TemplateAnalysis::bookTBHists() {

  TString hname="Trigger"; TString htitle="Trigger Histogram";

  TH1F* id =new TH1F(hname,htitle,5,0.0,5.0);
  id->GetXaxis()->SetBinLabel(1,trigBeam());
  id->GetXaxis()->SetBinLabel(2,trigIped());
  id->GetXaxis()->SetBinLabel(3,trigOped());
  id->GetXaxis()->SetBinLabel(4,trigLED());
  id->GetXaxis()->SetBinLabel(5,trigLaser());
  m_HistNames[hname]= id;

  hname="WC A"; htitle="Wire Chamber A";
  TH2F* id1 =new TH2F(hname,htitle,100,-50,50,100,-50,50);
  m_HistNames2D[hname]= id1;
  hname="WC B"; htitle="Wire Chamber B";
  TH2F* id2 =new TH2F(hname,htitle,100,-50,50,100,-50,50);
  m_HistNames2D[hname]= id2;
  hname="WC C"; htitle="Wire Chamber C";
  TH2F* id3 =new TH2F(hname,htitle,100,-50,50,100,-50,50);
  m_HistNames2D[hname]= id3;

  hname="CK 1"; htitle="Cherenkov 1";
  TH1F* id4 =new TH1F(hname,htitle,200,0,600);
  m_HistNames[hname]= id4;

  return;
}

void TemplateAnalysis::fillTBTriggerHists(const HcalTBTriggerData& trigger){

  TString hname="Trigger";

  hid_iter=m_HistNames.find(hname);
  if (hid_iter==m_HistNames.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << hname << std::endl;
  else
    {
      if (trigger.wasBeamTrigger()) hid_iter->second->Fill(trigBeam(),1.0);
      if (trigger.wasInSpillPedestalTrigger()) hid_iter->second->Fill(trigIped(),1.0);
      if (trigger.wasOutSpillPedestalTrigger()) hid_iter->second->Fill(trigOped(),1.0);
      if (trigger.wasLEDTrigger()) hid_iter->second->Fill(trigLED(),1.0);
      if (trigger.wasLaserTrigger()) hid_iter->second->Fill(trigLaser(),1.0);
    }
  return;
}

void TemplateAnalysis::fillTBBeamHists(const HcalTBEventPosition& tb_epos,
				       const HcalTBBeamCounters& tb_qadc,
				       const HcalTBTiming& tb_time){

  for(char c = 'A'; c<='C'; c++){
    vector<double> xh,yh;
    tb_epos.getChamberHits(c,xh,yh);
    //For now, assume that there are only matched pairs
    if(xh.size()!=yh.size()){
      printf("Mismatched wire chamber vector size: %d, %d\n",xh.size(),yh.size());
      continue;
    }
    
    TString hname="WC ";
    hname.Append(c);
    hid2D_iter=m_HistNames2D.find(hname);
    if (hid2D_iter==m_HistNames2D.end())
      std::cout << "%fillHist -- Could not find histogram with name: " << hname << std::endl;
    else{	  
      for(unsigned int h=0; h<xh.size(); h++)
	hid2D_iter->second->Fill(xh[h],yh[h]);
    }
  }
  
  TString hname="CK 1";
  if (hid_iter==m_HistNames.end())
    std::cout << "%fillHist -- Could not find histogram with name: " << hname << std::endl;
  else{
    hid_iter->second->Fill(tb_qadc.CK1adc());
  }
  
  return;
}

void TemplateAnalysis::bookJetHistograms(const TString& prefix) {

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

  return;
}

void TemplateAnalysis::bookForId(const HcalDetId& id) {

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
  rechitmap[id]=new TH1F(ss_t1.str().c_str(),ss.str().c_str(),520,-3.0,10.0);
}

template <typename T> void TemplateAnalysis::fillRecHits(const T& hits) {

  std::map<HcalDetId,TH1*>::iterator h1;

  m_file->cd(EnergyDir());
  typedef typename T::const_iterator iter;
  for ( iter i=hits.begin(); i!=hits.end(); i++) {
    h1=rechitmap.find(i->id()); // look for a histogram with this hit's id
    if (h1==rechitmap.end()) {
      bookForId(i->id());
      h1=rechitmap.find(i->id()); // look for a histogram with this hit's id
    }
    //std::cout << "Energy: " << i->energy() << " hist1: " << h1->second->GetTitle() << endl;
    h1->second->Fill(i->energy()); // if it's there, fill it with energy
  }
  m_file->cd();
  return;
}



void TemplateAnalysis::fillJetHists(const CaloJetCollection& jets, const TString& prefix) {

  CaloJetCollection::const_iterator i;  

  Double_t maxEt=0.,maxPt=0.;
  for ( i =jets.begin(); i!=jets.end(); i++) {
    
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

  return;
}

/** Finalization (close files, etc) */
void TemplateAnalysis::done() {
  std::cout << "Closing up.\n";
  if (m_file!=0) { // if there was a histogram file...
    m_file->Write(); // write out the histrograms
    delete m_file; // close and delete the file
    m_file=0; // set to zero to clean up
  }
  return;
}

