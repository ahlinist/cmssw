#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSetfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/JetReco/interface/GenJet.h" 
//#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "PhysicsTools/CandAlgos/interface/ShallowCloneProducer.h"

#include <string>
#include <vector>
#include <set>
#include <fstream>

using namespace edm;
using namespace reco;
using namespace std;


class GenParticleSelector : public edm::EDProducer {
 public:
  /// constructor
  GenParticleSelector( const edm::ParameterSet & );
  /// destructor
  ~GenParticleSelector( );

 private:
  /// process one event
  void produce( edm::Event& e, const edm::EventSetup& );
  unsigned int switchSignalDefinition;
  edm::InputTag selgenjetsrc_;
};
