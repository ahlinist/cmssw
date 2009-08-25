#include "TauAnalysis/RecoTools/plugins/PATJetAlphaSelector.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

PATJetAlphaSelectorImp::PATJetAlphaSelectorImp(const edm::ParameterSet& cfg) 
{
  if ( cfg.exists("alphaMin") ) {
    alphaMin_ = cfg.getParameter<double>("alphaMin");
    applyAlphaMin_ = true;
  } else {
    applyAlphaMin_ = false;
  }

  if ( cfg.exists("alphaMax") ) {
    alphaMax_ = cfg.getParameter<double>("alphaMax");
    applyAlphaMax_ = true;
  } else {
    applyAlphaMax_ = false;
  }
}

PATJetAlphaSelectorImp::~PATJetAlphaSelectorImp() 
{
//--- nothing to be done yet...
}

void PATJetAlphaSelectorImp::select(const edm::Handle<collection>& patJetCollection,
				    edm::Event& evt, const edm::EventSetup& es) 
{
  selected_.clear();

  for ( collection::const_iterator patJet = patJetCollection->begin(); 
	patJet != patJetCollection->end(); ++patJet ) {

    double alpha = jetAlphaExtractor_(*patJet);

    if ( (!applyAlphaMin_ || alpha > alphaMin_) &&
	 (!applyAlphaMax_ || alpha < alphaMax_) ) {
      selected_.push_back(&(*patJet));
    }
  }
}

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PATJetAlphaSelectorImp> PATJetAlphaSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_ANOTHER_FWK_MODULE(PATJetAlphaSelector);

