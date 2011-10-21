#ifndef TauAnalysis_RecoTools_PATObjectLUTvalueEmbedder_h
#define TauAnalysis_RecoTools_PATObjectLUTvalueEmbedder_h

/** \class PATObjectLUTvalueEmbedder
 *
 * Extract efficiency/fake-rate values for pat::Electron, pat::Muon and pat::Tau objects
 * and embed the extracted values as userFloats.
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATObjectLUTvalueEmbedder.h,v 1.1 2011/08/31 12:18:02 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <vector>
#include <string>

template<typename T, typename Textractor>
class PATObjectLUTvalueEmbedder : public edm::EDProducer 
{
  typedef std::vector<T> TCollection;

 public:

  explicit PATObjectLUTvalueEmbedder(const edm::ParameterSet& cfg)
    : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
      extractor_(cfg)
  {
    src_ = cfg.getParameter<edm::InputTag>("src");

    userFloatLabel_ = cfg.getParameter<std::string>("userFloatLabel");

    produces<TCollection>("");
  }

  ~PATObjectLUTvalueEmbedder() {}

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    edm::Handle<TCollection> objCollection_input;
    evt.getByLabel(src_, objCollection_input);
    
    std::auto_ptr<TCollection> objCollection_output(new TCollection());
    
    for ( typename TCollection::const_iterator obj_input = objCollection_input->begin();
	  obj_input != objCollection_input->end(); ++obj_input ) {
      T obj_output(*obj_input);

      double value = extractor_(*obj_input);
      obj_output.addUserFloat(userFloatLabel_, value);

      objCollection_output->push_back(obj_output);
    }

    evt.put(objCollection_output);
  }

 private:

  std::string moduleLabel_;

  Textractor extractor_;

  edm::InputTag src_;

  std::string userFloatLabel_;
};

#endif

