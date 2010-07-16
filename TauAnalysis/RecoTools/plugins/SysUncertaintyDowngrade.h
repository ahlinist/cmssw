#ifndef TauAnalysis_RecoTools_SysUncertaintyDowngrade_h
#define TauAnalysis_RecoTools_SysUncertaintyDowngrade_h

/** \class SysUncertaintyDowngrade
 *
 * Randomly kill certain fraction of objects
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: SysUncertaintyDowngrade.h,v 1.3 2010/04/28 14:51:15 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include <TRandom3.h>

template <typename T>
class SysUncertaintyDowngrade : public edm::EDProducer 
{
  typedef std::vector<T> vT;

 public:
  explicit SysUncertaintyDowngrade(const edm::ParameterSet& cfg)
    : src_(cfg.getParameter<edm::InputTag>("src")),
      pDowngrade_(cfg.getParameter<double>("pDowngrade"))
  {
    produces<vT>();
  }
  ~SysUncertaintyDowngrade() {}
    
  void produce(edm::Event& evt, const edm::EventSetup& es) 
  {
    std::auto_ptr<vT> outputCollection(new vT);

    edm::Handle<vT> inputCollection;
    evt.getByLabel(src_, inputCollection);

    for ( typename vT::const_iterator inputObject = inputCollection->begin();
	  inputObject != inputCollection->end(); ++inputObject ) {
      if ( u_.Rndm() > pDowngrade_ ) {
        T outputObject = (*inputObject);
        outputCollection->push_back(outputObject);
      }
    }

    evt.put(outputCollection);
  }

 private:
  edm::InputTag src_;

  double pDowngrade_;
  
  TRandom3 u_;
};

#endif
