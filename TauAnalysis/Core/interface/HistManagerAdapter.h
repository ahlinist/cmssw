#ifndef TauAnalysis_Core_HistManagerAdapter_h
#define TauAnalysis_Core_HistManagerAdapter_h

/** \class HistManagerAdapter
 *
 * Provide classes derrived from HistManagerBase with an EDAnalyzer interface 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: HistManagerAdapter.h,v 1.2 2009/06/12 14:49:25 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <vector>

template<typename T>
class HistManagerAdapter : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit HistManagerAdapter(const edm::ParameterSet& cfg) 
    : histManager_(cfg) 
  {
    srcWeights_ = ( cfg.exists("srcWeights") ) ?
      cfg.getParameter<vInputTag>("srcWeights") : vInputTag();
  }
    
  // destructor
  virtual ~HistManagerAdapter() {}
    
 private:
  void beginJob() { histManager_.beginJob(); }
  void analyze(const edm::Event& evt, const edm::EventSetup& es) 
  { 
    double evtWeight = 1.0;
    for ( vInputTag::const_iterator srcWeight = srcWeights_.begin();
	  srcWeight != srcWeights_.end(); ++srcWeight ) {
      edm::Handle<double> weight;
      evt.getByLabel(*srcWeight, weight);
      evtWeight *= (*weight);
    }
    
    histManager_.analyze(evt, es, evtWeight); 
  }
  void endJob() {}
  
  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  T histManager_;
};

#endif   

