#include "IOMC/GeneratorInterface/interface/PythiaFilterGammaJetWithIsoPi0.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include <iostream>
#include<list>
#include<vector>
#include<cmath>

using namespace edm;
using namespace std;

namespace{

  double deltaR2(double eta0, double phi0, double eta, double phi){
    double dphi=phi-phi0;
    if(dphi>M_PI) dphi-=2*M_PI;
    else if(dphi<=-M_PI) dphi+=2*M_PI;
    return dphi*dphi+(eta-eta0)*(eta-eta0);
  }

  double deltaPhi(double phi0, double phi){
    double dphi=phi-phi0;
    if(dphi>M_PI) dphi-=2*M_PI;
    else if(dphi<=-M_PI) dphi+=2*M_PI;
    return dphi;
  }

  class ParticlePtGreater{
  public:
    int operator()(const HepMC::GenParticle * p1, 
		   const HepMC::GenParticle * p2) const{
      return p1->momentum().perp() > p2->momentum().perp();
    }
  };
}


PythiaFilterGammaJetWithIsoPi0::PythiaFilterGammaJetWithIsoPi0(const edm::ParameterSet& iConfig) :
label_(iConfig.getUntrackedParameter("moduleLabel",std::string("source"))),
etaMin(iConfig.getUntrackedParameter<double>("MinPi0Eta", 1.65)),
energyMin(iConfig.getUntrackedParameter<double>("MinPi0Energy", 50.)),
etaMax(iConfig.getUntrackedParameter<double>("MaxPi0Eta", 2.5)),
energyMax(iConfig.getUntrackedParameter<double>("MaxPi0Energy", 100.)),
isocone(iConfig.getUntrackedParameter<double>("IsoCone",0.3)),
isodr(iConfig.getUntrackedParameter<double>("IsoDR",0.1)),
isodeta(iConfig.getUntrackedParameter<double>("IsoDeta",1.)),
ebEtaMax(1.479),
maxnumberofeventsinrun(iConfig.getUntrackedParameter<int>("MaxEvents",10)){ 
  
  deltaEB=0.01745/2  *5; // delta_eta, delta_phi
  deltaEE=2.93/317/2 *5; // delta_x/z, delta_y/z
  theNumberOfTestedEvt = 0;
  theNumberOfSelected = 0;

  cout << " Cut Definition: " << endl;
  cout << " MinPi0Eta = " << etaMin << endl;
  cout << " MinPi0Energy = " << energyMin << endl;
  cout << " MaxPi0Eta = " << etaMax << endl;
  cout << " MaxPi0Energy = " << energyMax << endl;
  cout << " Max dR between pi0 and jet axis = " << isodr << endl;
  cout << " Min deta between pi0 and photon = " << isodeta  << endl;
  cout << " Isolation Cone around pi0 axis = " << isocone << endl;
  cout << " 5x5 crystal cone  around pi0 axis in ECAL Barrel = " << deltaEB << endl;
  cout << " 5x5 crystal cone  around pi0 axis in ECAL Endcap = " << deltaEE << endl;

}


PythiaFilterGammaJetWithIsoPi0::~PythiaFilterGammaJetWithIsoPi0(){}


// ------------ method called to produce the data  ------------
bool PythiaFilterGammaJetWithIsoPi0::filter(edm::Event& iEvent, const edm::EventSetup& iSetup){

  if(theNumberOfSelected>=maxnumberofeventsinrun)   {
    throw cms::Exception("endJob")<<"we have reached the maximum number of events ";
  }
  
  theNumberOfTestedEvt++;
  if(theNumberOfTestedEvt%1000 == 0) cout << "Number of tested events = " << theNumberOfTestedEvt <<  endl;
  
  bool accepted = false;
  Handle<edm::HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);

  list<const HepMC::GenParticle *> pi0_seeds;
  const HepMC::GenEvent * myGenEvent = evt->GetEvent();

  int particle_id = 1;
  int pi0_id = -1;
  for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();  
                                                    p != myGenEvent->particles_end(); ++p ) {
   
    double Particle_eng = sqrt((*p)->momentum().perp() * (*p)->momentum().perp() +
			 (*p)->momentum().pz() * (*p)->momentum().pz());
			  
    if ( (*p)->pdg_id()==111 
	 &&  Particle_eng > energyMin
	 && Particle_eng < energyMax 
	 && abs((*p)->momentum().eta()) < etaMax  
	 && abs((*p)->momentum().eta()) > etaMin ) {
          pi0_id = particle_id;
	  pi0_seeds.push_back(*p);
    }

    particle_id++;
  }

  pi0_seeds.sort(ParticlePtGreater());
  
  int photon_id = 7;
  int jet_id = 8;
  if(myGenEvent->particle(8)->pdg_id() == 22) { 
    photon_id = 8;
    jet_id = 7;
  }

  if(pi0_seeds.size() == 1) {
    double eta_photon = myGenEvent->particle(photon_id)->momentum().eta();
    double phi_photon = myGenEvent->particle(photon_id)->momentum().eta();      
    double eta_jet = myGenEvent->particle(jet_id)->momentum().eta();
    double phi_jet = myGenEvent->particle(jet_id)->momentum().phi();
    double eta_pi0 = myGenEvent->particle(pi0_id)->momentum().eta();
    double phi_pi0 = myGenEvent->particle(pi0_id)->momentum().phi();

    double dr_pi0_jet = sqrt(deltaR2(eta_jet,phi_jet, eta_pi0, phi_pi0)); 
    double deta_pi0_photon = fabs(eta_pi0 - eta_photon);
    
// check if pi0 comes from the jet and is far from the photon
// ----------------------------------------------------------    
    if(dr_pi0_jet < isodr && deta_pi0_photon > isodeta ) { 
      cout <<" Number of pi0 seeds " << pi0_seeds.size() << " ProccessID " 
         << myGenEvent->signal_process_id() << endl;
      cout <<" Particel 7 ID = "<<myGenEvent->particle(7)->pdg_id()
           <<" pT "<< myGenEvent->particle(7)->momentum().perp()
           <<" Eta "<< myGenEvent->particle(7)->momentum().eta()
           <<" Phi "<< myGenEvent->particle(7)->momentum().phi()<< endl;
      cout <<" Particle 8 ID = "<< myGenEvent->particle(8)->pdg_id()
           <<" pT "<< myGenEvent->particle(8)->momentum().perp()
           <<" Eta "<< myGenEvent->particle(8)->momentum().eta()
           <<" Phi "<<myGenEvent->particle(8)->momentum().phi() << endl;
      cout <<" Photon ID = "<<myGenEvent->particle(photon_id)->pdg_id()
           <<" pT "<< myGenEvent->particle(photon_id)->momentum().perp()
           <<" Eta "<< myGenEvent->particle(photon_id)->momentum().eta()
           <<" Phi "<< myGenEvent->particle(photon_id)->momentum().phi()<< endl;
      cout <<" Pi0 ID = "<< myGenEvent->particle(pi0_id)->pdg_id()
           <<" Energy "<< sqrt(myGenEvent->particle(pi0_id)->momentum().perp()*
	                  myGenEvent->particle(pi0_id)->momentum().perp() +
			  myGenEvent->particle(pi0_id)->momentum().pz()*
			  myGenEvent->particle(pi0_id)->momentum().pz())
           <<" pT "<< myGenEvent->particle(pi0_id)->momentum().perp()
           <<" Eta "<< myGenEvent->particle(pi0_id)->momentum().eta()
           <<" Phi "<<myGenEvent->particle(pi0_id)->momentum().phi() << endl;
      cout << "----> dr_pi0_jet = " << dr_pi0_jet << " and deta_pi0_photon = " << deta_pi0_photon << endl;

      bool inEB(0);
      double tgx(0);
      double tgy(0);
      if( std::abs(eta_pi0) < ebEtaMax) inEB=1;
      else{
        tgx=myGenEvent->particle(pi0_id)->momentum().px()/myGenEvent->particle(pi0_id)->momentum().pz();
        tgy=myGenEvent->particle(pi0_id)->momentum().py()/myGenEvent->particle(pi0_id)->momentum().pz();
      }

      double etPi0=0;
      double etPi0Charged=0;
      double etCone=0;
      double etConeCharged=0;
      double ptMaxHadron=0;
        
      for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin();
                                                 p != myGenEvent->particles_end(); ++p ) {
    
        if ( (*p)->status()!=1 ) continue; // use only stable particles
        int pid= (*p)->pdg_id();
        int apid= std::abs(pid);
        if (apid>11 &&  apid<21) continue; //get rid of muons and neutrinos
        double eta=(*p)->momentum().eta();
        double phi=(*p)->momentum().phi();
        if ( sqrt(deltaR2(eta_pi0, phi_pi0, eta, phi)) > isocone) continue;
        double pt=(*p)->momentum().perp();
        etCone+=pt; // add the pt if particle inside the cone

        int charge3=(*p)->particleID().threeCharge();
        if(charge3 && pt<2) etConeCharged+=pt;

      //select particles matching a crystal array centered on pi0 direction
        if(inEB) {
	  if(  std::abs(eta-eta_pi0)> deltaEB ||
	       std::abs(deltaPhi(phi,phi_pi0)) > deltaEB) continue;
        }
        else if( fabs((*p)->momentum().px()/(*p)->momentum().pz() - tgx) > deltaEE || 
	         fabs((*p)->momentum().py()/(*p)->momentum().pz() - tgy) > deltaEE) continue;

        cout << " Particle : " << (*p)->pdg_id() << " with Pt = " << pt << " is inside 5x5 ECAL matrix: So add it " << endl;
        etPi0+=pt;
	
        if(charge3 && pt<2) etPi0Charged+=pt;
	
        if(apid>100 && apid!=310 && pt>ptMaxHadron) ptMaxHadron=pt;

      } // for ( HepMC::GenEvent::particle_const_iterator
      
    //isolation cuts

      double iso_cut1 = 5 + etPi0/20 - etPi0*etPi0/1e4;
      double iso_cut2 = 3 + etPi0/20 - etPi0*etPi0*etPi0/1e6;
      double iso_cut3 = 4.5 + etPi0/40;
      if (etPi0>165.)
      {
        iso_cut1 = 5.+165./20.-165.*165./1e4;
        iso_cut2 = 3.+165./20.-165.*165.*165./1e6;
        iso_cut3 = 4.5+165./40.;
      }

      double iso_val1 = etCone - etPi0; 
      double iso_val2 = iso_val1 - (etConeCharged - etPi0Charged);

      cout << " etcone = " << etCone << " etConeCharged = " << etConeCharged;
      cout << " etPi0 = " << etPi0 << " etPi0Charged = " << etPi0Charged;
      cout << " ptMaxHadron = " << ptMaxHadron <<  endl;

      cout << " First Isolation Cut: (etCone-etPi0) = " << iso_val1 
           << " iso_cut1 value = " << iso_cut1 << endl;
      cout << " Second Isolation Cut: (etCone-etPhoton-(etConeCharged-etPhotonCharged)) = " 
           << iso_val2 << " iso_cut2 value = " << iso_cut2 << endl;
      cout << " Third Isolation Cut PtHadron " << ptMaxHadron 
           <<"  iso_cut3 value = "<< iso_cut3 <<endl;
    
      if( iso_val1 < iso_cut1) {
        cout<<"-> Accept Iso Cut1 " << endl;
        if( iso_val2 < iso_cut2) {
           cout<<"--> Accept Iso Cut2 " << endl;
           if(ptMaxHadron < iso_cut3) {
             cout<<"---> Accept Iso Cut3 " << endl;
             accepted=true;
           } // if(ptMaxHadron < iso_cut3)
	} // if( iso_val2 < iso_cut2) 
      } // if( iso_val1 < iso_cut1)	        
    } // if(dr_pi0_jet < 0.1 && deta_pi0_photon > 1)
  } //if(pi0_seeds.size() == 1) {
  
 
  if (accepted) {
    theNumberOfSelected++;
    cout << "========>  Event preselected " << theNumberOfSelected
         << " Proccess ID " << myGenEvent->signal_process_id() << endl;
    return true; 
  }
  else return false;

}

