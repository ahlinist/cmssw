#include "IOMC/GeneratorInterface/interface/ZZ4lFilter.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>

ZZ4lFilter::ZZ4lFilter(const edm::ParameterSet& iConfig) :
  label_(iConfig.getUntrackedParameter("moduleLabel", string("source"))),
  decayMode_(iConfig.getUntrackedParameter("decayMode", 0)),
  verbosity_(iConfig.getUntrackedParameter<bool>("Verbosity", false)),
  includeTau_(iConfig.getUntrackedParameter<bool>("includeTau", true)),
  onlyTau_(iConfig.getUntrackedParameter<bool>("onlyTau", false)),
  maxEvents_(iConfig.getUntrackedParameter<int>("MaxEvents",10000))
{  

  // Note : decay mode 
  // For ZZ : (0:4l,  1:4e,  2:2e2mu, 3:4mu) 

  nev_ = 0; 

  nZZ4e_      = 0;
  nZZ2e2mu_   = 0;
  nZZ4mu_     = 0;
  nZZ2e2tau_  = 0;
  nZZ2mu2tau_ = 0;
  nZZ4tau_    = 0;

  pZZ4e_         = 0;
  pZZ2e2mu_      = 0;
  pZZ4mu_        = 0;
  pZZ_1tau_4e_    = 0;
  pZZ_1tau_2e2mu_ = 0;
  pZZ_1tau_4mu_   = 0;
  pZZ_2tau_4e_    = 0;
  pZZ_2tau_2e2mu_ = 0;
  pZZ_2tau_4mu_   = 0;

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

ZZ4lFilter::~ZZ4lFilter()
{

  if (verbosity_) {
    int nevt = nZZ4e_+nZZ2e2mu_+nZZ4mu_+nZZ2e2tau_+nZZ2mu2tau_+nZZ4tau_;

    cout<<"=== ZZ->4l filter summary ==="<<endl;
    cout<<" "<<endl;
    cout<<"=== Generated events ==="<<endl;
    cout<<"total   : "<<nevt<<endl;
    cout<<"4e      : "<<nZZ4e_<<" "<<(float)nZZ4e_/nevt<<endl;
    cout<<"2e2mu   : "<<nZZ2e2mu_<<" "<<(float)nZZ2e2mu_/nevt<<endl;
    cout<<"4mu     : "<<nZZ4mu_<<" "<<(float)nZZ4mu_/nevt<<endl;
    cout<<"2e2tau  : "<<nZZ2e2tau_<<" "<<(float)nZZ2e2tau_/nevt<<endl;
    cout<<"2mu2tau : "<<nZZ2mu2tau_<<" "<<(float)nZZ2mu2tau_/nevt<<endl;
    cout<<"4tau    : "<<nZZ4tau_<<" "<<(float)nZZ4tau_/nevt<<endl;
    cout<<"=== Selected events ==="<<endl;
    cout<<"total   : "<<nev_<<endl;
    if ((decayMode_ == 0 || decayMode_ == 1) && !onlyTau_)
      cout<<"(direct decay)  4e    : "<<pZZ4e_<<" "<<(float)pZZ4e_/nev_<<endl;
    if ((decayMode_ == 0 || decayMode_ == 2) && !onlyTau_)
      cout<<"(direct decay)  2e2mu : "<<pZZ2e2mu_<<" "<<(float)pZZ2e2mu_/nev_<<endl;
    if ((decayMode_ == 0 || decayMode_ == 3) && !onlyTau_)
      cout<<"(direct decay)  4mu   : "<<pZZ4mu_<<" "<<(float)pZZ4mu_/nev_<<endl;
    if ((decayMode_ == 0 || decayMode_ == 1) && includeTau_) {
      cout<<"(through 1tau)  4e    : "<<pZZ_1tau_4e_<<" "<<(float)pZZ_1tau_4e_/nev_<<endl;
      cout<<"(through 2tau)  4e    : "<<pZZ_2tau_4e_<<" "<<(float)pZZ_2tau_4e_/nev_<<endl;
    }
    if ((decayMode_ == 0 || decayMode_ == 2) && includeTau_) {
      cout<<"(through 1tau)  2e2mu : "<<pZZ_1tau_2e2mu_<<" "<<(float)pZZ_1tau_2e2mu_/nev_<<endl;
      cout<<"(through 2tau)  2e2mu : "<<pZZ_2tau_2e2mu_<<" "<<(float)pZZ_2tau_2e2mu_/nev_<<endl;
    }
    if ((decayMode_ == 0 || decayMode_ == 3) && includeTau_) {
      cout<<"(through 1tau)  4mu   : "<<pZZ_1tau_4mu_<<" "<<(float)pZZ_1tau_4mu_/nev_<<endl; 
      cout<<"(through 2tau)  4mu   : "<<pZZ_2tau_4mu_<<" "<<(float)pZZ_2tau_4mu_/nev_<<endl; 
    }
  }

}

bool ZZ4lFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if(nev_ >= maxEvents_)   {
    throw cms::Exception("endJob")<<"we have reached the maximum number of events ";
  }

  int count = 0;
  int mcEle = 0;
  int mcMu = 0;
  int mcTau = 0;
  int nEle = 0;
  int nMu = 0;
  Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);

  HepMC::GenEvent *myGenEvent = new HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_const_iterator p;  

  for (p = myGenEvent->particles_begin(); p != myGenEvent->particles_end(); ++p ) {    
    
    count++;
    if (count>=9 && count<=12) {
      if (abs((*p)->pdg_id()) == 11) mcEle++;
      if (abs((*p)->pdg_id()) == 13) mcMu++;
      if (abs((*p)->pdg_id()) == 15) mcTau++;
    }
    
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
	      if ((*pa)->status() != 3) continue;	      
	      if ((abs((*pa)->pdg_id()) == 23)) accepted = true;	      
	    }
	  }
	  
	  if (accepted) {
	    if (abs((*p)->pdg_id()) == 11) nEle++;
	    if (abs((*p)->pdg_id()) == 13) nMu++;
	  }
	  
	}
      }
    }
  }      
  
  delete myGenEvent;   

  if (mcEle == 4) nZZ4e_++;
  else if (mcEle == 2 && mcMu == 2) nZZ2e2mu_++;
  else if (mcMu == 4) nZZ4mu_++;
  else if (mcEle == 2 && mcTau == 2) nZZ2e2tau_++;
  else if (mcMu == 2 && mcTau == 2) nZZ2mu2tau_++;
  else if (mcTau == 4) nZZ4tau_++;

  if (nEle == 4 && mcTau == 0) pZZ4e_++;
  else if (nEle == 2 && nMu == 2 && mcTau == 0) pZZ2e2mu_++;
  else if (nMu == 4 && mcTau == 0) pZZ4mu_++;    
  else if (nEle == 4 && mcTau == 2) pZZ_1tau_4e_++;
  else if (nEle == 2 && nMu == 2 && mcTau == 2) pZZ_1tau_2e2mu_++;
  else if (nMu == 4 && mcTau == 2) pZZ_1tau_4mu_++;    
  else if (nEle == 4 && mcTau == 4) pZZ_2tau_4e_++;
  else if (nEle == 2 && nMu == 2 && mcTau == 4) pZZ_2tau_2e2mu_++;
  else if (nMu == 4 && mcTau == 4) pZZ_2tau_4mu_++;    

  bool keepIt = false;
  if (includeTau_) keepIt = true;
  else if (!includeTau_ && mcTau == 0) keepIt = true;

  if (onlyTau_ && mcTau != 0) keepIt = true;
  else if (onlyTau_ && mcTau == 0) keepIt = false;

  if (keepIt) {
    if (decayMode_ == 0) {
      if ((nEle == 4 && nMu == 0) || (nEle == 2 && nMu == 2) || (nEle == 0 && nMu == 4)) {
	nev_++;
	return true; 
      } else return false;
    } else if (decayMode_ == 1 && nEle == 4 && nMu == 0) {
      nev_++;
      return true; 
    } else if (decayMode_ == 2 && nEle == 2 && nMu == 2) {
      nev_++;
      return true; 
    } else if (decayMode_ == 3 && nEle == 0 && nMu == 4) {
      nev_++;
      return true; 
    } else {
      return false;
    }
  } else {
    return false;
  }

}

