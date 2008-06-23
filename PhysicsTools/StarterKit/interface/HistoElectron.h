#ifndef StarterKit_HistoElectron_h
#define StarterKit_HistoElectron_h

//------------------------------------------------------------
// Title: HistoElectron.h
// Purpose: To histogram Electrons
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
//   HistoElectron ( TFile * file );
//   Description: Constructor.
//
//   void fill( TK::Electron * );
//   Description: Fill object. Will fill relevant electron variables
//
//   void write();
//   Description: Write object to file in question.
//
//   ~HistoElectron
//    Description: Destructor. Deallocates memory.
//
//------------------------------------------------------------
//
// Modification History:
//
//   -29Nov07: Sal Rappoccio: Creation of the object
//------------------------------------------------------------


// CMSSW include files
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "PhysicsTools/StarterKit/interface/HistoGroup.h"

// STL include files
#include <string>

// ROOT include files
#include <TH1D.h>
#include <TFile.h>

namespace pat {

  class HistoElectron : public HistoGroup<Electron> {

  public:
    HistoElectron( std::string dir = "electron", std::string group = "Electron",
		   std::string pre = "e",
		   double pt1=0, double pt2=200, double m1=0, double m2=200 ,
		   TFileDirectory * parentDir=0);
    virtual ~HistoElectron();

    // fill a plain ol' electron:
    virtual void fill( const Electron *electron, uint iPart = 1, double weight = 1.0 );
    virtual void fill( const Electron &electron, uint iPart = 1, double weight = 1.0 ) { fill(&electron, iPart,weight); }

    // fill a electron that is a shallow clone, and take kinematics from 
    // shallow clone but detector plots from the electron itself
    virtual void fill( const reco::ShallowClonePtrCandidate *electron, uint iPart = 1, double weight = 1.0 );
    virtual void fill( const reco::ShallowClonePtrCandidate &electron, uint iPart = 1, double weight = 1.0 )
    { fill(&electron, iPart,weight); }

    virtual void fillCollection( const std::vector<Electron> & coll, double weight = 1.0 );

    // Clear ntuple cache
    void clearVec();
  protected:

    PhysVarHisto *    h_trackIso_;
    PhysVarHisto *    h_caloIso_;
    PhysVarHisto *    h_leptonID_;

  };

}
#endif
