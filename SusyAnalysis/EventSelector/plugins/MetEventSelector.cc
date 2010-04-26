#include "SusyAnalysis/EventSelector/interface/MetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "SusyAnalysis/EventSelector/interface/uncorrectionTypeMET.h"

#include <vector>

MetEventSelector::MetEventSelector(const edm::ParameterSet& pset) :
	SusyEventSelector(pset) {
	// input collection
	metTag_ = pset.getParameter<edm::InputTag> ("metTag");
	// lower cut on MET
	minMet_ = pset.getParameter<double> ("minMET");
	// upper cut on MET
	maxMet_ = pset.getParameter<double> ("maxMET");

	// uncorrection type
	uncorrType_ = pat::uncorrectionTypeMET(pset.getParameter<std::string> ("uncorrType"));

	// definition of the variable to be cached
	defineVariable("MET");

	edm::LogInfo("MetEventSelector") << "constructed with \n" << "  metTag = " << metTag_ << "\n" << "  minMET = "
			<< minMet_ << "  maxMET = " << maxMet_;
}

bool MetEventSelector::select(const edm::Event& event) const {
	// reset cached variables
	resetVariables();
	//
	// get the MET result
	//
	edm::Handle<edm::View<reco::MET> > metHandle;
	event.getByLabel(metTag_, metHandle);
	if (!metHandle.isValid()) {
		edm::LogWarning("MetEventSelector") << "No Met results for InputTag " << metTag_;
		return false;
	}

	//
	// sanity check on collection
	//
	if (metHandle->size() != 1) {
		edm::LogWarning("MetEventSelector") << "MET collection size is " << metHandle->size() << " instead of 1";
		return false;
	}

	//get the uncorrected/corrected MET
	//check if we are dealing with pat::MET and
	//if a specific corrected MET is requested:
	double myMET = metHandle->front().et();
	const pat::MET * pm = static_cast<const pat::MET*> (&(metHandle->front()));
	if (!pm && uncorrType_ != pat::MET::uncorrMAXN)
		myMET = pm->uncorrectedPt(uncorrType_);

	//
	// apply cut
	//
	setVariable(0, myMET);
	bool result_max = true;
	if (maxMet_ > 0)
		result_max = (myMET < maxMet_);
	bool result_min = true;
	if (minMet_ > 0)
		result_min = (myMET > minMet_);
	LogTrace("MetEventSelector") << "result = " << (result_min && result_max);
	return (result_min && result_max);
}

//________________________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"
DEFINE_EDM_PLUGIN(EventSelectorFactory, MetEventSelector, "MetEventSelector");

