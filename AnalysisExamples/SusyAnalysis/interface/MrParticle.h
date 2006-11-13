#ifndef MrParticle_h
#define MrParticle_h

/*  \class MrParticle
*
*  Generic particle class (to be used both for monte carlo and reconstucted objects)
*
*  Authors: Luc Pape & Filip Moortgat      Date: August 2005 
*/

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"


using namespace reco;
using namespace std;

class MrParticle {

public:

// constructors
MrParticle() : Px(0), Py(0), Pz(0), E(0), Charge(0),  
ParticleType(0), ParticleIso(true), Hemi(0),
MCParton(0), PID(0), Status(0), Mother1(0) {};

MrParticle(float px, float py, float pz, float e) : Px(px), Py(py), Pz(pz), E(e), Charge(0),  
ParticleType(0), ParticleIso(true), Hemi(0),
MCParton(0), PID(0), Status(0), Mother1(0) {};


// destructor
virtual ~MrParticle(){};

// access methods
float eta(){if (fabs(Pz) <1.0e-5 ) {return 0;}
            float theta = atan(sqrt(Px*Px+Py*Py)/Pz);
            if (theta < 0.) {theta = theta + 3.141592654;}
            return -log(tan(0.5*theta));}
float phi(){return atan2(Py,Px);}
float p(){return sqrt(Px*Px+Py*Py+Pz*Pz);}
float pt(){return sqrt(Px*Px+Py*Py);}
float px() {return Px;}
float py() {return Py;}
float pz() {return Pz;}
float energy() {return E;}
float mass() {return sqrt(E*E-Px*Px-Py*Py-Pz*Pz);}
float charge() {return Charge;}
// particleType: particle identification   
//                   1 for e
//                   2 for mu
//                   3 for tau (hadronic)
//                   4 for photon
//                   5 for udscg-jet
//                   6 for b-jet
//                   7 for top
//               
//                   9 for MET 
//
int particleType() {return ParticleType;}
bool particleIso() {return ParticleIso;}
int hemisphere() {return Hemi;} 
int partonIndex() {return MCParton;} 
// PDG particle identification
int pid() {return PID;} 
int status() {return Status;}
int motherIndex() {return Mother1;}
virtual const Electron* electronCandidate() {
                   cout << "Pointer to electron candidate not defined." << endl;
                   return NULL;}
virtual const Muon* muonCandidate() {
                   cout << "Pointer to muon candidate not defined." << endl;
                   return NULL;}
virtual const Jet* jetCandidate() {
                   cout << "Pointer to jet candidate not defined." << endl;
                   return NULL;}


// set methods
void setPx(float px) {Px = px;}
void setPy(float py) {Py = py;}
void setPz(float pz) {Pz = pz;}
void setEnergy(float e) {E = e;}
void setCharge(float charge) {Charge = charge;}
virtual void setParticleType(int ptype) {ParticleType = ptype;}
void setParticleIso(bool piso) {ParticleIso = piso;}
void setHemisphere(int hem) {Hemi = hem;}
void setPartonIndex(int mcpart) {MCParton = mcpart;}
void setPID(int pid) {PID = pid; 
                      if (abs(PID) == 11) {ParticleType = 1;} 
                      if (abs(PID) == 13) {ParticleType = 2;} 
                      if (abs(PID) == 15) {ParticleType = 3;} 
                      if (abs(PID) == 22) {ParticleType = 4;} 
                      if (abs(PID) >= 1 && abs(PID) < 5) {ParticleType = 5;} 
		      if (abs(PID) == 5) {ParticleType = 6;} 
		      if (abs(PID) == 6) {ParticleType = 7;} 
} 
void setStatus(int stat) {Status = stat;}
void setMotherIndex(int m1) {Mother1 = m1;} 

private:

// data members
float Px, Py, Pz, E;
float Charge; 
int ParticleType, ParticleIso, Hemi; 
int MCParton, PID, Status, Mother1;


};




class MrElectron : public MrParticle {

public:

// constructors
MrElectron() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(1);};

MrElectron(float px, float py, float pz, float e, const Electron* mycand) : MrParticle(px,py,pz,e), 
 PCandidate(mycand) {MrParticle::setParticleType(1);};

virtual ~MrElectron() {};

//access methods
virtual const Electron* electronCandidate() {return PCandidate;}

//set methods
void setCandidate (const Electron* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype != 1){ cout << "Changing type to non-electron not allowed." << endl;}
                }

private:

// data members
const Electron* PCandidate;


};



class MrMuon : public MrParticle {

public:

// constructors
MrMuon() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(2);};

MrMuon(float px, float py, float pz, float e, const Muon* mycand) : MrParticle(px,py,pz,e), 
 PCandidate(mycand) {MrParticle::setParticleType(2);};

virtual ~MrMuon() {};

//access methods
const Muon* muonCandidate() {return PCandidate;}

//set methods
void setCandidate (const Muon* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype != 2){ cout << "Changing type to non-muon not allowed." << endl;}
                }

private:

// data members
const Muon* PCandidate;

};


class MrJet : public MrParticle {

public:

// constructors
MrJet() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(5);};

MrJet(float px, float py, float pz, float e, const Jet* mycand) : MrParticle(px,py,pz,e), 
 PCandidate(mycand) {MrParticle::setParticleType(5);};


virtual ~MrJet() {};

//access methods
const Jet* jetCandidate() {return PCandidate;}

//set methods
void setCandidate (const Jet* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype <= 2 || ptype == 4 ){ cout << "Changing type to non-jet not allowed." << endl;}
                }

private:

// data members
const Jet* PCandidate;


};



#endif
