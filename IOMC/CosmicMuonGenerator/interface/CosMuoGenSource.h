#ifndef CosMuoGenSource_h
#define CosMuoGenSource_h
//
// CosmicMuonGenerator by droll (01/FEB/2006)
//
#include "IOMC/CosmicMuonGenerator/interface/CosmicMuonGenerator.h"
#include "CLHEP/HepMC/GenEvent.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/GeneratedInputSource.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"

using namespace edm;
using namespace std;

namespace edm
{
  class CosMuoGenSource : public GeneratedInputSource{
  public:
    CosMuoGenSource(const ParameterSet&, const InputSourceDescription& );
    virtual ~CosMuoGenSource();

  private: 
    virtual bool produce(Event & e);
    
    void clear();
    // define the configurable generator parameters
    int32_t      RanS; // seed of random number generator (from Framework)
    double       MinE; // min. E     [GeV]
    double       MaxE; // max. E     [GeV]
    double       MinT; // min. theta [deg]
    double       MaxT; // max. theta [deg]
    double       MinP; // min. phi   [deg]
    double       MaxP; // max. phi   [deg]
    double       MinS; // min. t0    [ns]
    double       MaxS; // max. t0    [ns]
    double       ELSF; // scale factor for energy loss
    double       RTarget; // Radius of target-cylinder which cosmics HAVE to hit [mm], default is CMS-dimensions
    double       ZTarget; // z-length of target-cylinder which cosmics HAVE to hit [mm], default is CMS-dimensions
    bool         TrackerOnly; //if set to "true" detector with tracker-only setup is used, so no material or B-field outside is considerd
    bool         MTCCHalf; //if set to "true" muons are sure to hit half of CMS important for MTCC, 
                           //still material and B-field of whole CMS is considered

    CosmicMuonGenerator* CosMuoGen;
    // the event format itself
    HepMC::GenEvent* fEvt;
    bool cmVerbosity_;
  };
} 

#endif
