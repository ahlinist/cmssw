#ifndef Z2TAUEMUFILTER_h
#define Z2TAUEMUFILTER_h

/** \class Z2TAUEMUFilter
 *
 * For Higgs -> tau tau -> e mu 
 * 
 * \author V. Brigljevic, IRB-ZAGREB
 *
 ************************************************************/


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace std;

class Z2TAUEMUFILTER : public EDFilter {

 public:

  explicit Z2TAUEMUFILTER(const edm::ParameterSet&);
  ~Z2TAUEMUFILTER();
    
  virtual bool filter(Event&, const EventSetup&);

 private:
      
  string label_;
  bool Verbosity_;
  int maxEvents_;
  int nev_;

};
#endif
