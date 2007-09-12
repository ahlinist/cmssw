#ifndef FastSimulation_ProtonTaggerFilter_H
#define FastSimulation_ProtonTaggerFilter_H

/// Parametrization of near-beam detector acceptance.

/**
 * This class:
 * - reads generated data (edm::HepMCProduct) from edm::Event
 * - selects forward protons
 * - makes a fast parametrized simulation of near-beam detector (FP420 and TOTEM) acceptance
 * - returns the acceptance as a boolean
 *
 * This code is a port of the FastTotem module from ORCA-based FAMOS.
 * Thus, its physics performace is just the same as one of FastTotem.
 * Several options are available to choose between the FP420/TOTEM detectors
 * 
 * Author: Dmitry Zaborov
 */

// Version: $Id:  $

#include "FWCore/Framework/interface/EDFilter.h"

#include "FastSimulation/ProtonTaggers/interface/RPAcceptance.h"

#include "HepMC/GenEvent.h"

class ProtonTaggerFilter : public edm::EDFilter
{

 public:

  /// default constructor
  explicit ProtonTaggerFilter(edm::ParameterSet const & p);

  /// empty destructor
  virtual ~ProtonTaggerFilter();

  /// startup function of the EDFilter
  virtual void beginJob(const edm::EventSetup & c);

  /// endjob function of the EDFilter
  virtual void endJob();

  /// decide if the event is accepted by the proton taggers
  virtual bool filter(edm::Event & e, const edm::EventSetup & c);

 private:

  /// choose which of the detectors (FP420/TOTEM/both) will be used for beam 1
  unsigned int beam1mode;

  /// choose which of the detectors (FP420/TOTEM/both) will be used for beam 2
  unsigned int beam2mode;

  /// choose how to combine data from the two beams (ask for 1/2 proton)
  unsigned int beamCombiningMode;

  /// Path to text files with tabulated acceptance
  std::string pathToAcceptanceTables; // path to the acceptance tables
  
  /// The object which actually computes the acceptance
  RPAcceptance rpAcceptance;
  
};

#endif
