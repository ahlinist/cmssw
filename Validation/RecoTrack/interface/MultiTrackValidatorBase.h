#ifndef MultiTrackValidatorBase_h
#define MultiTrackValidatorBase_h

/** \class MultiTrackValidatorBase
 *  Base class for analyzers that produces histrograms to validate Track Reconstruction performances
 *
 *  $Date: 2010/02/01 10:33:41 $
 *  $Revision: 1.26 $
 *  \author cerati
 */

#include <memory>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 

#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"


#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/RecoAlgos/interface/RecoTrackSelector.h"
#include "CommonTools/RecoAlgos/interface/TrackingParticleSelector.h"
#include "CommonTools/RecoAlgos/interface/CosmicTrackingParticleSelector.h"

#include <DQMServices/Core/interface/DQMStore.h>

#include <iostream>
#include <sstream>
#include <string>

class MultiTrackValidatorBase {
 public:

  /// Constructor
  MultiTrackValidatorBase(const edm::ParameterSet& pset);
    
  /// Destructor
  virtual ~MultiTrackValidatorBase(){ }
  
  //virtual void initialize()=0;

 protected:

  DQMStore* dbe_;

  // MTV-specific data members
  std::vector<std::string> associators;
  edm::InputTag label_tp_effic;
  edm::InputTag label_tp_fake;
  std::string sim;
  std::string parametersDefiner;


  std::vector<edm::InputTag> label;
  edm::InputTag bsSrc;

  std::string out;


  edm::ESHandle<MagneticField> theMF;
  std::vector<const TrackAssociatorBase*> associator;


  bool ignoremissingtkcollection_;
  bool skipHistoFit;



};


#endif
