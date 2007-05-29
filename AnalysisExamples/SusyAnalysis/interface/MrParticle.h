#ifndef MrParticle_h
#define MrParticle_h

/*  \class MrParticle
*
*  Generic particle class (to be used both for monte carlo and reconstucted objects)
*
*  Authors: Luc Pape & Filip Moortgat      Date: August 2005 
*/

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/EgammaCandidates/interface/PixelMatchGsfElectron.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

using namespace reco;
using namespace std;

class MrParticle {

public:

// constructors
MrParticle() : Px(0.), Py(0.), Pz(0.), E(0.), Charge(0.),  
Vx(0.), Vy(0.), Vz(0.), D0Error(0.), DzError(0.),
ParticleType(0), ParticleIso(true), Hemi(0),
Pt_tracks(0.), Et_em(0.), Et_had(0.),
MCParton(0), PID(0), Status(0), Mother1(0), BtagDiscriminator(-10) {};

MrParticle(float px, float py, float pz, float e) : Px(px), Py(py), Pz(pz), E(e), Charge(0.),  
Vx(0.), Vy(0.), Vz(0.), D0Error(0.), DzError(0.),
ParticleType(0), ParticleIso(true), Hemi(0),
Pt_tracks(0.), Et_em(0.), Et_had(0.),
MCParton(0), PID(0), Status(0), Mother1(0), BtagDiscriminator(-10) {};


// destructor
virtual ~MrParticle(){};

// access methods
// object kinematical quantities
float eta(){if (fabs(Pz) <1.0e-5 ) {return 0;}
            float theta = atan(sqrt(Px*Px+Py*Py)/Pz);
            if (theta < 0.) {theta = theta + 3.141592654;}
            return -log(tan(0.5*theta));}
float phi() {return atan2(Py,Px);}
float p() {return sqrt(Px*Px+Py*Py+Pz*Pz);}
float pt() {return sqrt(Px*Px+Py*Py);}
float px() {return Px;}
float py() {return Py;}
float pz() {return Pz;}
float energy() {return E;}
float mass() {return sqrt(E*E-Px*Px-Py*Py-Pz*Pz);}
float charge() {return Charge;}
// two-particle invariant mass
float invmass(MrParticle* part) {
   return sqrt( (E+part->energy())*(E+part->energy()) -
                (Px+part->px())*(Px+part->px())       -
                (Py+part->py())*(Py+part->py())       -
                (Pz+part->pz())*(Pz+part->pz()) );}
// coordinates of reference point (closest approach to beam)
float vx() {return Vx;}
float vy() {return Vy;}
float vz() {return Vz;}
// impact parameter errors
float d0Error() {return D0Error;}
float dzError() {return DzError;}
// particleType: particle identification   
//                   1 for e
//                   2 for mu
//                   3 for tau (hadronic)
//                   4 for photon
//                   5 for udscg-jet
//                   6 for b-jet
//                   7 for top
//               
//                   9 for invisible particle (only for MC data)
//                  10 for MET from calorimetry
//                  11 for MET from recoil
//
int particleType() {return ParticleType;}
// isolation flag
bool particleIso() {return ParticleIso;}
// hemisphere (1 or 2) to which the object is associated
int hemisphere() {return Hemi;} 
// number of tracks (useful for jets) and scalar sum of their pt
int numTracks() {return NumTracks;}
float pt_tracks() {return Pt_tracks;}
// electromagnetic and hadronic energy fractions
float et_em() {return Et_em;}
float et_had() {return Et_had;}
// get b-tagging discriminator
double getBtagDiscriminator(){return BtagDiscriminator;}
// for Reco: index in MCData of the matched MC particle
int partonIndex() {return MCParton;} 
// for MC: PDG particle identification
int pid() {return PID;} 
// for MC: Pythia status
int status() {return Status;}
// for MC: index in MCData of the mother
int motherIndex() {return Mother1;}

virtual const PixelMatchGsfElectron* electronCandidate() {
//                   cout << "Pointer to electron candidate not defined." << endl;
                   return NULL;}
virtual const Muon* muonCandidate() {
                   cout << "Pointer to muon candidate not defined." << endl;
                   return NULL;}
virtual const Photon* photonCandidate() {
                   cout << "Pointer to photon candidate not defined." << endl;
                   return NULL;}
virtual const Jet* jetCandidate() {
                   cout << "Pointer to jet candidate not defined." << endl;
                   return NULL;}
virtual const JetTag* jetTag() {
                   cout << "Pointer to jetTag candidate not defined." << endl;
                   return NULL;}

// set methods
void setPx(float px) {Px = px;}
void setPy(float py) {Py = py;}
void setPz(float pz) {Pz = pz;}
void setEnergy(float e) {E = e;}
void setCharge(float charge) {Charge = charge;}
void setVx(float vx) {Vx = vx;}
void setVy(float vy) {Vy = vy;}
void setVz(float vz) {Vz = vz;}
void setd0Error(float dd0) {D0Error = dd0;}
void setdzError(float ddz) {DzError = ddz;}
virtual void setParticleType(int ptype) {ParticleType = ptype;}
void setParticleIso(bool piso) {ParticleIso = piso;}
void setHemisphere(int hem) {Hemi = hem;}
void setNumTracks(int ntk) {NumTracks = ntk;}
void setPt_tracks(float apt_tracks) {Pt_tracks = apt_tracks;}
void setEt_em(float aet_em){Et_em = aet_em;} 
void setEt_had(float aet_had){Et_had = aet_had;}
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
void setBtagDiscriminator(double discri) {BtagDiscriminator = discri;}

private:

// data members
float Px, Py, Pz, E;
float Charge; 
float Vx, Vy, Vz, D0Error, DzError;
int ParticleType, ParticleIso, Hemi;
int NumTracks;
float Pt_tracks, Et_em, Et_had;
int MCParton, PID, Status, Mother1;
double BtagDiscriminator;

};




class MrElectron : public MrParticle {

public:

// constructors
MrElectron() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(1);};

MrElectron(float px, float py, float pz, float e, const PixelMatchGsfElectron* mycand) : 
 MrParticle(px,py,pz,e), PCandidate(mycand) 
 {MrParticle::setParticleType(1);
 MrParticle::setCharge(mycand->charge());}

virtual ~MrElectron() {};

//access methods
virtual const PixelMatchGsfElectron* electronCandidate() {return PCandidate;}

//set methods
void setCandidate (const PixelMatchGsfElectron* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype != 1){ cout << "Changing type to non-electron not allowed." << endl;}
                else { MrParticle::setParticleType(ptype); }
                }

private:

// data members
const PixelMatchGsfElectron* PCandidate;


};



class MrMuon : public MrParticle {

public:

// constructors
MrMuon() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(2);};

MrMuon(float px, float py, float pz, float e, const Muon* mycand) : 
 MrParticle(px,py,pz,e), PCandidate(mycand) 
 {MrParticle::setParticleType(2);
 MrParticle::setCharge(mycand->charge());}

virtual ~MrMuon() {};

//access methods
const Muon* muonCandidate() {return PCandidate;}

//set methods
void setCandidate (const Muon* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype != 2){ cout << "Changing type to non-muon not allowed." << endl;}
                else { MrParticle::setParticleType(ptype); }      
                }

private:

// data members
const Muon* PCandidate;

};



class MrPhoton : public MrParticle {

public:

// constructors
MrPhoton() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(4);};

MrPhoton(float px, float py, float pz, float e, const Photon* mycand) : 
 MrParticle(px,py,pz,e), PCandidate(mycand) 
 {MrParticle::setParticleType(4);
 MrParticle::setCharge(0.);}

virtual ~MrPhoton() {};

//access methods
virtual const Photon* photonCandidate() {return PCandidate;}

//set methods
void setCandidate (const Photon* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype != 4){ cout << "Changing type to non-photon not allowed." << endl;}
                else { MrParticle::setParticleType(ptype); }
                }

private:

// data members
const Photon* PCandidate;


};


class MrJet : public MrParticle {

public:

// constructors
MrJet() : MrParticle(),  PCandidate(0) {MrParticle::setParticleType(5);};

MrJet(float px, float py, float pz, float e, const Jet* mycand, const JetTag* myjettag) : MrParticle(px,py,pz,e), 
 PCandidate(mycand), PJetTag(myjettag) {MrParticle::setParticleType(5);};


virtual ~MrJet() {};

//access methods
const Jet* jetCandidate() {return PCandidate;}
const JetTag* jetTag() {return PJetTag;}

//set methods
void setCandidate (const Jet* mycand) {PCandidate = mycand;}
virtual void setParticleType(int ptype) {
               if (ptype <= 2 || ptype == 4 || ptype > 7){ cout << "Changing type to non-jet not allowed." << endl;}
               else { MrParticle::setParticleType(ptype); }
                }
void setBtagDiscriminator(double discri) {MrParticle::setBtagDiscriminator(discri);}

private:

// data members
const Jet* PCandidate;
const JetTag* PJetTag;

};

// The structure Config_t has been introduced 
// to hold the entries to the constants,
// because the entry to the config cannot be transmitted to classes
// invoked from SusyAnalyzer.
// It may be a temporary solution.

struct Config_t {
     edm::ParameterSet rejectEvent_params;
     edm::ParameterSet acceptance_cuts;
     edm::ParameterSet cleaner_params;
     edm::ParameterSet isolator_params;
     edm::ParameterSet objectmatch_params;
     edm::ParameterSet mcproc_params;
     edm::ParameterSet useranalysis_params;
    
};




#endif
