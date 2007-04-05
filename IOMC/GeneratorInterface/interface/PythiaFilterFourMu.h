#ifndef PythiaFilterFourMu_h
#define PythiaFilterFourMu_h

/** \class PythiaFilterFourMu
 *
 *  PythiaFilterFourMu filter implements generator-level preselections 
 *  for "4-mu in final state" events.
 * 
 * \author D.Trocino - A.Giacobbe - C.Botta
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


class PythiaFilterFourMu : public edm::EDFilter {
public:
  explicit PythiaFilterFourMu(const edm::ParameterSet&);
  ~PythiaFilterFourMu();
  
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
private:
      
  std::string label_;
  double etaMuMax;
  double etaMuMin;
  double ptMuMin;
  int theNumberOfSelected;
  int maxnumberofeventsinrun;
  int NumberOfAllEvents;
};
#endif
