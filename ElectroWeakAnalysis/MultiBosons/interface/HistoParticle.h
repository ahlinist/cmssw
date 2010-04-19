#ifndef VgKit_HistoParticle_h
#define VgKit_HistoParticle_h

//------------------------------------------------------------
// Title: HistoParticle.h
// Purpose: To histogram Particles
//
// Authors:
// Liz Sexton-Kennedy <sexton@fnal.gov>
// Eric Vaandering <ewv@fnal.gov >
// Petar Maksimovic <petar@jhu.edu>
// Sal Rappoccio <rappocc@fnal.gov>
//------------------------------------------------------------
//
// Interface:
//
//   HistoParticle ( TFile * file );
//   Description: Constructor.
//
//   void fill( TK::Particle * );
//   Description: Fill object. Will fill relevant jet variables
//
//   void write();
//   Description: Write object to file in question.
//
//   ~HistoParticle
//    Description: Destructor. Deallocates memory.
//
//------------------------------------------------------------
//
// Modification History:
//
//   -29Nov07: Sal Rappoccio: Creation of the object
//------------------------------------------------------------


// CMSSW include files
#include "DataFormats/PatCandidates/interface/Particle.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/HistoGroup.h"

// STL include files
#include <string>

// ROOT include files
#include <TH1D.h>
#include <TFile.h>

namespace pat {

  class HistoParticle : public HistoGroup<Particle> {

  public:
    HistoParticle(std::string dir = "particle", std::string group = "Particle",
	      std::string pre ="p",
	      double pt1=0, double pt2=200, double m1=0, double m2=200,
	      TFileDirectory * parentDir=0);
    virtual ~HistoParticle() ;

    // fill a plain ol' particle:
    virtual void fill( const Particle *particle, double weight = 1.0 );
    virtual void fill( const Particle &particle, double weight = 1.0 ) { fill(&particle, weight); }

    // fill a particle that is a shallow clone, and take kinematics from 
    // shallow clone but detector plots from the particle itself
    virtual void fill( const reco::ShallowClonePtrCandidate *particle, double weight = 1.0 );
    virtual void fill( const reco::ShallowClonePtrCandidate &particle, double weight = 1.0 )
    { fill(&particle, weight); }


    // Clear ntuple cache
    void clearVec();

  };

}
#endif
