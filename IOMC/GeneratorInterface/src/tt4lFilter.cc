#include "IOMC/GeneratorInterface/interface/tt4lFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

tt4lFilter::tt4lFilter(const edm::ParameterSet& iConfig) :
  label_(iConfig.getUntrackedParameter("moduleLabel", string("source"))),
  decayMode_(iConfig.getUntrackedParameter("decayMode", 0)),
  verbosity_(iConfig.getUntrackedParameter<bool>("Verbosity", false)),
  maxEvents_(iConfig.getUntrackedParameter<int>("MaxEvents",10000))
{  

  // Note : decay mode 
  // For tt : (0:4l,  1:4e,  2:2e2mu, 3:4mu) 

  nev_ = 0; 

  pZZ4e_         = 0;
  pZZ2e2mu_      = 0;
  pZZ4mu_        = 0;

  vector<int> def_lepID;
  def_lepID.push_back(0);
  leptonID = iConfig.getUntrackedParameter< vector<int> >("LeptonID", def_lepID);  

  vector<double> def_minPt;
  def_minPt.push_back(0.);
  minPt = iConfig.getUntrackedParameter< vector<double> >("MinPt", def_minPt);

  vector<double> def_maxPt;
  def_maxPt.push_back(10000.);
  maxPt = iConfig.getUntrackedParameter< vector<double> >("MaxPt", def_maxPt);

  vector<double> def_minEta;
  def_minEta.push_back(-10.);
  minEta = iConfig.getUntrackedParameter< vector<double> >("MinEta", def_minEta);

  vector<double> def_maxEta;
  def_maxEta.push_back(10.);
  maxEta = iConfig.getUntrackedParameter< vector<double> >("MaxEta", def_maxEta);

  if (minPt.size() > 1 &&  leptonID.size() != minPt.size() 
      || maxPt.size() > 1 && leptonID.size() != maxPt.size()) {
    cout<<"WARNING: VV filter : size of MinPt and/or MaxPt not matching with LeptonID size!!"<<endl;
  }

  if (minEta.size() > 1 &&  leptonID.size() != minEta.size() 
      || maxEta.size() > 1 && leptonID.size() != maxEta.size()) {
    cout<<"WARNING: VV filter : size of MinEta and/or MaxEta not matching with LeptonID size!!"<<endl;
  }  
  
}

tt4lFilter::~tt4lFilter()
{

  if (verbosity_) {
    cout<<"=== tt->4l filter summary ==="<<endl;
    cout<<" "<<endl;
    cout<<"=== Generated events ==="<<endl;
    cout<<"4e    : "<<pZZ4e_<<endl;
    cout<<"2e2mu : "<<pZZ2e2mu_<<endl;
    cout<<"4mu   : "<<pZZ4mu_<<endl;
  }

}

bool tt4lFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if(nev_ >= maxEvents_)   {
    throw cms::Exception("endJob")<<"we have reached the maximum number of events ";
  }

  int nEle = 0;
  int nPos = 0;
  int nMum = 0;
  int nMup = 0;
  Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);

  HepMC::GenEvent *myGenEvent = new HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_const_iterator p;  

  for (p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {       
    
    if ((*p)->status() == 1) {
      
      bool accepted = true;      
      
      for (unsigned int i=0; i<leptonID.size(); ++i) {
	if (abs((*p)->pdg_id()) == leptonID[i]) {
	  
	  if ((*p)->momentum().perp() > maxPt[i]) 
	    accepted = false;
	  if ((*p)->momentum().perp() < minPt[i]) 
	    accepted = false;
	  if ((*p)->momentum().eta() > maxEta[i]) 
	    accepted = false;
	  if ((*p)->momentum().eta() < minEta[i]) 
	    accepted = false;
	  
	  if (accepted) {
	    
	    accepted = false;
	    vector<HepMC::GenParticle*> ancestors = (*p)->listAncestors();
	    vector<HepMC::GenParticle*>::const_iterator pa;
	    for (pa = ancestors.begin(); pa != ancestors.end(); ++pa) {	 
	      if ((*pa)->status() == 2) continue;
	      if ((abs((*pa)->pdg_id()) == 6) && (*pa)->status() == 3) accepted = true;	      
	    }
	  }
	  
	  if (accepted) {
	    if ((*p)->pdg_id() == 11)  nEle++;
	    if ((*p)->pdg_id() == -11) nPos++;
	    if ((*p)->pdg_id() == 13)  nMum++;
	    if ((*p)->pdg_id() == -13) nMup++;
	  }
	  
	}
      }
    }
  }      
  
  delete myGenEvent;   

  if (nEle>=2 && nPos>=2) pZZ4e_++;
  else if (nEle >= 1 && nPos >= 1 && nMum >= 1 && nMup >= 1) pZZ2e2mu_++;
  else if (nMum >= 2 && nMup >= 2) pZZ4mu_++;

  if (decayMode_ == 0) {
    if ((nEle >= 2 && nPos >= 2) || (nEle >= 1 && nPos >= 1 && nMum >= 1 && nMup >= 1) || (nMum >= 2 && nMup >= 2)) {
      nev_++;
      return true; 
    } else return false;
  } else if (decayMode_ == 1 && nEle >= 2 && nPos >= 2) {
    nev_++;
    return true; 
  } else if (decayMode_ == 2 && nEle >= 1 && nPos >= 1 && nMum >= 1 && nMup >= 1) {
    nev_++;
    return true; 
  } else if (decayMode_ == 3 && nMum >= 2 && nMup >= 2) {
    nev_++;
    return true; 
  } else {
    return false;
  }  

}

