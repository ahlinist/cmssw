#ifndef SusyAnalysis_AnalysisSkeleton_HemisphereSelector_h_
#define SusyAnalysis_AnalysisSkeleton_HemisphereSelector_h_
/// Selection on delta Phi Hemispheres.
///
/// Performs a number of selections based on delta Phi between the Hemisphereaxis:
/// - minimum delta-phi
/// - maximum delta-phi  

///
/// $Id $


// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class HemisphereSelector : public SusyEventSelector {
public:
  HemisphereSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~HemisphereSelector () {}
private:
  edm::InputTag hemisphereTag_; ///< tag for input collection

  
  float dPhiHemispheresMin_;     ///< Minimum distance between two Hemisphere axis
  float dPhiHemispheresMax_; ///< Maximum distance between two Hemisphere axis
 

};
#endif
