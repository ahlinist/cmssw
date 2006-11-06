#ifndef MCPARTON_H
#define MCPARTON_H

// class for partons in the simulation allowing
// to get the most important info for partons

#include "RecoBTag/MCTools/interface/MCBaseParticle.h"

class MCParton : public MCBaseParticle {

public:

  MCParton();

  MCParton(const HepMC::GenParticle* particle, const HepMC::GenEvent* event,
		const HepMC::GenParticle* lastParton);

  ~MCParton(){}


  // other info (isPrimary etc.) where the complete vector of generated particles is needed
  void setParticleProperties ();

  // print
  void print () const;

  // access to data members
  int  flavour  	  () const { return flavour_; }
  int  flavourAbs	  () const { return flavourAbs_; }
  int  statusPythia	  () const { return statusPythia_; }
  const HepMC::GenParticle* mother () const { return mother_; }
  int  motherLundCode	  () const { return motherLundCode_; }
  int  motherStatusPythia () const { return motherStatusPythia_; }
  bool fromPrimaryProcess () const { return fromPrimaryProcess_; }
  bool fromRadiation	  () const { return fromRadiation_; }
  bool fromGluonSplitting () const { return fromGluonSplitting_; }
  bool isFinalParton	  () const { return isFinalParton_; }
  //  bool RadiatesGluon      () const { return radiatesGluon_; }
  bool splitsToD	  () const { return splitsToD_; }
  bool splitsToU	  () const { return splitsToU_; }
  bool splitsToS	  () const { return splitsToS_; }
  bool splitsToC	  () const { return splitsToC_; }
  bool splitsToB	  () const { return splitsToB_; }
  //  bool getSplitsToGluon      () const { return splitsToGluon ; }

  std::vector<const HepMC::GenParticle*> daughters () const { return daughterLines_ ; }
  math::XYZTLorentzVector summedDaughterMomentum () const { return summedDaughters_ ; }

  bool isInitialParton      () const { return isInitialParton_; }
  bool initialPartonHasCloseHF () const { return initialPartonHasCloseHF_; }

private:

  void reset ();
  int  flavour_;     // 0: undef._; 12345: duscb_; 21: gluon // D
  int  flavourAbs_;                                          // D
  int  statusPythia_;                                        // D
  int  motherLundCode_;                                      // D
  int  motherStatusPythia_;
  bool fromPrimaryProcess_;                                  // D
  bool fromRadiation_;                                       // NOT YET
  bool fromGluonSplitting_;                                  // D
  bool isFinalParton_;                                       // D
  //  bool radiatesGluon_;
  bool splitsToD_;                                           // D
  bool splitsToU_;                                           // D
  bool splitsToS_;                                           // D
  bool splitsToC_;                                           // D
  bool splitsToB_;                                           // D
  //  bool splitsToGluon_;                                   // D

  const HepMC::GenParticle* lastParton_;
  const HepMC::GenParticle* mother_;

  std::vector<const HepMC::GenParticle*> daughterLines_;
  math::XYZTLorentzVector summedDaughters_;

  bool isInitialParton_;
  bool initialPartonHasCloseHF_;


};
#endif

