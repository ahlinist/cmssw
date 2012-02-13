#ifndef TauAnalysis_RecoTools_PATObjectLUTrndSelector_h
#define TauAnalysis_RecoTools_PATObjectLUTrndSelector_h

/** \class PATObjectLUTrndSelector
 *
 * Randomly select pat::Electron, pat::Muon and pat::Tau objects.
 * The probability for an object to be selected is taking from look-up tables
 * (representing e.g. Data/MC ratios of efficiencies)
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATObjectLUTrndSelector.h,v 1.1 2011/10/21 16:24:12 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <TRandom3.h>

#include <vector>
#include <string>

template<typename T, typename Textractor>
class PATObjectLUTrndSelector
{
 public:

  typedef std::vector<T> collection;

  explicit PATObjectLUTrndSelector(const edm::ParameterSet& cfg)
    : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
      extractor_(cfg)
  {
    rnd_ = new TRandom3(); 
  }
  ~PATObjectLUTrndSelector()
  {
    delete rnd_;
  }

  typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
  typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

  void select(const edm::Handle<collection>& objCollection_input, edm::Event& evt, const edm::EventSetup& es) 
  {
    selected_.clear();
    
    for ( typename collection::const_iterator obj_input = objCollection_input->begin();
	  obj_input != objCollection_input->end(); ++obj_input ) {
      double lutValue = extractor_(*obj_input);
      double rnd_u = rnd_->Rndm();
    
      if ( rnd_u < lutValue ) selected_.push_back(&(*obj_input));
    }
  }
    
  size_t size() const { return selected_.size(); }

 private:

  std::string moduleLabel_;

  Textractor extractor_;

  std::vector<const T*> selected_;

  TRandom* rnd_;
};

#endif

