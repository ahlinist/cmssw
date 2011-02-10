#include "TauAnalysis/RecoTools/plugins/PfCandidateIpSelectorImp.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Math/GenVector/VectorUtil.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

PfCandidateIpSelectorImp::PfCandidateIpSelectorImp(const edm::ParameterSet& cfg) 
{
  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSrc");

  keepNeutrals_ =  (cfg.exists("keepNeutrals") ) ? cfg.getParameter<bool>("keepNeutrals") : 1;

  if ( cfg.exists("rhoMax") ) {
    rhoMax_ = cfg.getParameter<double>("rhoMax");
  } else {
    edm::LogError ("PfCandidateIpSelectorImp::PfCandidateIpSelectorImp") << " Must specify parameter rhoMax";
  }
}

PfCandidateIpSelectorImp::~PfCandidateIpSelectorImp() 
{
//--- nothing to be done yet...
}

void PfCandidateIpSelectorImp::select(const edm::Handle<collection >& pfCandCollection,
		edm::Event& evt, const edm::EventSetup& es) 
{
	selected_.clear();

	edm::Handle<reco::VertexCollection> primaryEventVertexCollection;
	evt.getByLabel(vertexSrc_, primaryEventVertexCollection);
	if ( primaryEventVertexCollection->size() < 1 ) {
		return;
	} else if ( primaryEventVertexCollection->size() > 1 ) {
		edm::LogError ("PfCandidateIpSelectorImp::select") << " Cannot have more than one primary event vertex --> skipping !!";
		return;
	} 

	const reco::Vertex& thePrimaryEventVertex = (*primaryEventVertexCollection->begin());

	for ( collection::const_iterator pfCand = pfCandCollection->begin(); 
			pfCand != pfCandCollection->end(); ++pfCand ) {
		
		//   non-PFChargedHadrons
		if( reco::PFCandidate::h != pfCand->particleId() ) {
			if( keepNeutrals_ )
				selected_.push_back(&(*pfCand));
			continue;
		}
			
		reco::TrackRef trackRef = pfCand->trackRef();
		if ( !trackRef.isNull() ) {
			
			double dRho = (trackRef->innerPosition().Rho() - thePrimaryEventVertex.position().Rho());
			if (  rhoMax_ > dRho ) 
				selected_.push_back(&(*pfCand));
		}
	}
	std::cout << std::endl;
}

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

typedef ObjectSelector<PfCandidateIpSelectorImp> PfCandidateIpSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PfCandidateIpSelector);
