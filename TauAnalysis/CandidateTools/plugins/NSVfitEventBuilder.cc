#include "TauAnalysis/CandidateTools/plugins/NSVfitEventBuilder.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"

#include <TMath.h>

NSVfitEventBuilder::NSVfitEventBuilder(const edm::ParameterSet& cfg) 
  : NSVfitEventBuilderBase(cfg)
{
  if ( cfg.exists("polStates") ) {
    typedef std::vector<std::string> vstring;
    vstring polStates_string = cfg.getParameter<vstring>("polStates");
    for ( vstring::const_iterator polState_string = polStates_string.begin();
	  polState_string != polStates_string.end(); ++polState_string ) {
      int polHandedness = -1;
      if      ( (*polState_string) == "undefined" ) polHandedness = NSVfitEventHypothesis::kPolUndefined;
      else if ( (*polState_string) == "WLWL"      ) polHandedness = NSVfitEventHypothesis::kPolWLWL;
      else if ( (*polState_string) == "WRWR"      ) polHandedness = NSVfitEventHypothesis::kPolWRWR;
      else if ( (*polState_string) == "WTWT"      ) polHandedness = NSVfitEventHypothesis::kPolWTWT;
      else throw cms::Exception("NSVfitEventBuilder")
	<< " Invalid Configuration Parameter 'polState' = " << (*polState_string) << " !!\n";
      polHandedness_.push_back(polHandedness);
    }
  } else {
    polHandedness_.push_back(NSVfitEventHypothesis::kPolUndefined);
  }
  numPolStates_ = polHandedness_.size();
}

void NSVfitEventBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  NSVfitEventBuilderBase::beginJob(algorithm);
}

void setFitParameterStepSize(NSVfitAlgorithmBase* algorithm, int fitParameterIdx, double stepSize)
{
  if ( fitParameterIdx != -1 ) {
    NSVfitParameter* fitParameter = algorithm->getFitParameter(fitParameterIdx);
    assert(fitParameter);
    fitParameter->setStepSize(stepSize);
  }
}

NSVfitEventHypothesis* NSVfitEventBuilder::build(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  NSVfitEventHypothesis* event = NSVfitEventBuilderBase::build(inputParticles, eventVertex);

//--- set polarization status for resonance
  event->polHandedness_ = polHandedness_;
  event->numPolStates_ = numPolStates_;

//--- set polarization status for daughters 
  if ( event->numResonances() == 2 ) {
    for ( size_t iResonance = 0; iResonance < event->numResonances(); ++iResonance ) {
      NSVfitResonanceHypothesis* resonance = event->resonance(iResonance);

//--- check that polarization of resonance has not yet been defined by NSVfitResonanceBuilder plugin
//   (in order not to overwrite polarization states defined by NSVfitResonanceBuilder plugin)
      if ( resonance->numPolStates_ != 1 )
	throw cms::Exception("NSVfitEventBuilder")
	  << " Simultaneous support for Polarization on event and resonance level not implemented yet !!\n";

      resonance->polHandedness_.resize(numPolStates_);
      resonance->polSign_.resize(numPolStates_);
      resonance->numPolStates_ = numPolStates_;

      for ( unsigned iPolState = 0; iPolState < numPolStates_; ++iPolState ) {
	int event_polHandedness = polHandedness_[iPolState];
	int resonance_polHandedness = -1;
	if ( event_polHandedness == NSVfitEventHypothesis::kPolUndefined ) {
	  resonance_polHandedness = NSVfitResonanceHypothesis::kPolUndefined;
	} else if ( event_polHandedness == NSVfitEventHypothesis::kPolWLWL ) {
	  resonance_polHandedness = NSVfitResonanceHypothesis::kPolWL;
	} else if ( event_polHandedness == NSVfitEventHypothesis::kPolWRWR ) {
	  resonance_polHandedness = NSVfitResonanceHypothesis::kPolWR;
	} else if ( event_polHandedness == NSVfitEventHypothesis::kPolWTWT ) {
	  resonance_polHandedness = NSVfitResonanceHypothesis::kPolWT;
	} 
	assert(resonance_polHandedness != -1);
	resonance->polHandedness_[iPolState] = resonance_polHandedness;

	double resonace_charge = 0.;
	for ( size_t iDaughter = 0; iDaughter < resonance->numDaughters(); ++iDaughter ) {
          if ( resonance->daughter(iDaughter)->particle().isNonnull() )
	    resonace_charge += resonance->daughter(iDaughter)->particle()->charge();
	}

	int resonance_polSign = 0;
	// CV: left-handed  W- and right-handed W+ are assigned polarization -1,
	//     right-handed W- and left-handed  W+ are assigned polarization +1
	if        ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolWL        ) {
	  if      ( resonace_charge < -0.5 ) resonance_polSign = -1;
	  else if ( resonace_charge > +0.5 ) resonance_polSign = +1;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolWR        ) {
	  if      ( resonace_charge < -0.5 ) resonance_polSign = +1;
	  else if ( resonace_charge > +0.5 ) resonance_polSign = -1;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolWT        ||
		    resonance_polHandedness == NSVfitResonanceHypothesis::kPolUndefined ) {
	  resonance_polSign = 0;
	} else assert(0);
	resonance->polSign_[iPolState] = resonance_polSign;
      }
    }
  } else if ( !(numPolStates_ == 1 || polHandedness_[0] == NSVfitEventHypothesis::kPolUndefined) ) 
    throw cms::Exception("NSVfitEventBuilder")
      << " Support for Polarization not implemented for case of " << event->numResonances() << " resonances yet !!\n";
//--- set fitParameter step-size according to estimated uncertainty on vertex position
  double pvPositionXerr, pvPositionYerr, pvPositionZerr;
  if ( event->eventVertexSVrefittedIsValid() ) {
    pvPositionXerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(0));
    pvPositionYerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(1));
    pvPositionZerr = TMath::Sqrt(event->eventVertexErrSVrefitted().Diagonal()(2));
  } else {
    pvPositionXerr = 0.0050; // 50 microns
    pvPositionYerr = 0.0050; // 50 microns
    pvPositionZerr = 0.1;    //  1 mm
  }

  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftX_, 0.25*pvPositionXerr);
  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftY_, 0.25*pvPositionYerr);
  setFitParameterStepSize(algorithm_, idxFitParameter_pvShiftZ_, 0.25*pvPositionZerr);

  return event;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventBuilderPluginFactory, NSVfitEventBuilder, "NSVfitEventBuilder");
