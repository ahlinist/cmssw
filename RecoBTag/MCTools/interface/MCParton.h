#ifndef MCPARTON_H
#define MCPARTON_H

// class for partons in the simulation allowing
// to get the most important info for partons
//
// inherits from BaseParticle

//  Author:             Christian.Weiser@cern.ch
//  Ported to CMSSW by: Andrea.Bocci@cern.ch
//  Last Update:        12/07/2006

#include "RecoBTag/MCTools/interface/MCBaseParticle.h"

//
// here it comes:
//
class MCParton : public MCBaseParticle {

public:
  /// default constructor
  MCParton(){}

  MCParton( const HepMC::GenParticle* particle, const HepMC::GenEvent* event, int iLP) :
    MCBaseParticle(particle,event),indexLastParton(iLP){}

  /// default destructor
  ~MCParton(){}


  // other info (isPrimary etc.) where the complete vector of generated particles is needed
  void setParticleProperties () {}; //to be filled later

  // print
  void print () const {}

  // access to data members
  int  getFlavour            () const { return flavour ; }
  int  getFlavourAbs         () const { return flavourAbs ; }
  int  getStatusPythia       () const { return statusPythia ; }
  int  getMotherLundCode     () const { return motherLundCode ; }
  int  getMotherStatusPythia () const { return motherStatusPythia ; }
  int  getMotherIndex        () const { return motherIndex ; }
  bool getFromPrimaryProcess () const { return fromPrimaryProcess ; }
  bool getFromRadiation      () const { return fromRadiation ; }
  bool getFromGluonSplitting () const { return fromGluonSplitting ; }
  bool getIsFinalParton      () const { return isFinalParton ; }
  //  bool getRadiatesGluon      () const { return radiatesGluon ; }
  bool getSplitsToD          () const { return splitsToD ; }
  bool getSplitsToU          () const { return splitsToU ; }
  bool getSplitsToS          () const { return splitsToS ; }
  bool getSplitsToC          () const { return splitsToC ; }
  bool getSplitsToB          () const { return splitsToB ; }
  //  bool getSplitsToGluon      () const { return splitsToGluon ; }

  std::vector<int> getDaughterLines () const { return daughterLines ; }
  HepLorentzVector getSummedDaughters () const { return summedDaughters ; }

  bool getIsInitialParton      () const { return isInitialParton ; }
  bool getInitialPartonHasCloseHF () const { return initialPartonHasCloseHF; }

private:

  int  flavour ;     // 0: undef. ; 12345: duscb ; 21: gluon // D
  int  flavourAbs ;                                          // D
  int  statusPythia ;                                        // D
  int  motherLundCode ;                                      // D
  int  motherStatusPythia ;
  int  motherIndex ;                                         // for unique identification
  bool fromPrimaryProcess ;                                  // D
  bool fromRadiation ;                                       // NOT YET
  bool fromGluonSplitting ;                                  // D
  bool isFinalParton ;                                       // D
  //  bool radiatesGluon ;
  bool splitsToD ;                                           // D
  bool splitsToU ;                                           // D
  bool splitsToS ;                                           // D
  bool splitsToC ;                                           // D
  bool splitsToB ;                                           // D
  //  bool splitsToGluon ;                                   // D

  int indexLastParton ;

  std::vector<int> daughterLines ;
  HepLorentzVector summedDaughters ;

  bool isInitialParton ;
  bool initialPartonHasCloseHF ;

};
#endif

