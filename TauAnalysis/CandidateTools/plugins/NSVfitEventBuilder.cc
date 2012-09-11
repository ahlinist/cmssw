#include "TauAnalysis/CandidateTools/plugins/NSVfitEventBuilder.h"

#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitParameter.h"
#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

#include <TMath.h>

using namespace SVfit_namespace;

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

  fixToGenVertex_        = ( cfg.exists("fixToGenVertex")        ) ? cfg.getParameter<bool>("fixToGenVertex")        : false;
  initializeToGenVertex_ = ( cfg.exists("initializeToGenVertex") ) ? cfg.getParameter<bool>("initializeToGenVertex") : fixToGenVertex_;
  if ( fixToGenVertex_ ) initializeToGenVertex_ = true;
  if ( initializeToGenVertex_ ) {
    srcGenVertex_ = cfg.getParameter<edm::InputTag>("srcGenVertex");
  }
}

void NSVfitEventBuilder::beginJob(NSVfitAlgorithmBase* algorithm)
{
  algorithm_ = algorithm;

  NSVfitEventBuilderBase::beginJob(algorithm);

   if ( verbosity_ ) {
    std::cout << "<NSVfitEventBuilder::beginJob>:" << std::endl;
    std::cout << " pluginName = " << pluginName_ << std::endl;
    std::cout << " idxFitParameter_pvShiftX = " << idxFitParameter_pvShiftX_ << std::endl;
    std::cout << " idxFitParameter_pvShiftY = " << idxFitParameter_pvShiftY_ << std::endl;
    std::cout << " idxFitParameter_pvShiftZ = " << idxFitParameter_pvShiftZ_ << std::endl;
  }

  if ( fixToGenVertex_ ) {
    if ( idxFitParameter_pvShiftX_ != -1 ) algorithm->fixFitParameter(idxFitParameter_pvShiftX_);
    if ( idxFitParameter_pvShiftY_ != -1 ) algorithm->fixFitParameter(idxFitParameter_pvShiftY_);
    if ( idxFitParameter_pvShiftZ_ != -1 ) algorithm->fixFitParameter(idxFitParameter_pvShiftZ_);
  }
}

void NSVfitEventBuilder::beginEvent(const edm::Event& evt, const edm::EventSetup& es)
{
  if ( verbosity_ ) std::cout << "<NSVfitEventBuilder::beginEvent>:" << std::endl;

  NSVfitEventBuilderBase::beginEvent(evt, es);
  
  if ( initializeToGenVertex_ ) {    
    edm::Handle<reco::Vertex> genVertex;
    evt.getByLabel(srcGenVertex_, genVertex);
    genVertexPos_(0) = genVertex->position().x();    
    genVertexPos_(1) = genVertex->position().y(); 
    genVertexPos_(2) = genVertex->position().z();     
    if ( verbosity_ >= 2 ) printVector("genVertexPos", genVertexPos_);
  }
}

NSVfitEventHypothesis* NSVfitEventBuilder::build(const inputParticleMap& inputParticles, const reco::Vertex* eventVertex) const
{
  if ( verbosity_ ) std::cout << "<NSVfitEventBuilder::build>:" << std::endl;

  NSVfitEventHypothesis* event = NSVfitEventBuilderBase::build(inputParticles, eventVertex);

//--- set polarization status for resonance
  event->polHandedness_ = polHandedness_;
  event->numPolStates_ = numPolStates_;
  //std::cout << "event: polHandedness = " << format_vint(event->polHandedness_) << std::endl;

//--- set polarization status for daughters 
  if ( event->numPolStates_ > 1 || (event->numPolStates_ == 1 && polHandedness_[0] != NSVfitEventHypothesis::kPolUndefined) ) {
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
	
	//std::cout << "resonance: polHandedness = " << format_vint(resonance->polHandedness_) << "," 
	//	  << " polSign = " << format_vint(resonance->polSign_) << std::endl;
      }
    } else throw cms::Exception("NSVfitEventBuilder")
	<< " Support for Polarization not implemented for case of " << event->numResonances() << " resonances yet !!\n";
  }

  double initialShiftX = 0.;
  double initialShiftY = 0.;
  double initialShiftZ = 0.;
  if ( initializeToGenVertex_ ) {
    initialShiftX = genVertexPos_(0) - eventVertex->position().x();
    algorithm_->setFitParameterInitialValue(idxFitParameter_pvShiftX_, initialShiftX);
    initialShiftY = genVertexPos_(1) - eventVertex->position().y();
    algorithm_->setFitParameterInitialValue(idxFitParameter_pvShiftY_, initialShiftY);
    initialShiftZ = genVertexPos_(2) - eventVertex->position().z();
    algorithm_->setFitParameterInitialValue(idxFitParameter_pvShiftZ_, initialShiftZ);
    if ( verbosity_ ) {
      if ( fixToGenVertex_) {
	std::cout << "fixing:" << std::endl;
	std::cout << " eventVertexPos: x = " << genVertexPos_(0) << ", y = " << genVertexPos_(1) << ", z = " << genVertexPos_(2) << std::endl;
      } else {
	std::cout << "initializing:" << std::endl;
	std::cout << " initialShift: x = " << initialShiftX << ", y = " << initialShiftY << ", z = " << initialShiftZ << std::endl;
      }
    }
  }

//--- set fitParameter step-size according to estimated uncertainty on vertex position
//   (event vertex position and uncertainty set in NSVfitEventBuilderBase class)
  if ( !fixToGenVertex_ ) {
    double vertexSigmaX = TMath::Sqrt(event->eventVertexCov().Diagonal()(0));
    algorithm_->setFitParameterLimit(idxFitParameter_pvShiftX_, initialShiftX - 10.*vertexSigmaX, initialShiftX + 10.*vertexSigmaX);
    algorithm_->setFitParameterStepSize(idxFitParameter_pvShiftX_, 0.25*vertexSigmaX);
    double vertexSigmaY = TMath::Sqrt(event->eventVertexCov().Diagonal()(1));
    algorithm_->setFitParameterLimit(idxFitParameter_pvShiftY_, initialShiftY - 10.*vertexSigmaY, initialShiftY + 10.*vertexSigmaY);
    algorithm_->setFitParameterStepSize(idxFitParameter_pvShiftY_, 0.25*vertexSigmaY);
    double vertexSigmaZ = TMath::Sqrt(event->eventVertexCov().Diagonal()(2));    
    algorithm_->setFitParameterLimit(idxFitParameter_pvShiftX_, initialShiftZ - 10.*vertexSigmaZ, initialShiftZ + 10.*vertexSigmaZ);
    algorithm_->setFitParameterStepSize(idxFitParameter_pvShiftZ_, 0.25*vertexSigmaZ);
  }
    
  return event;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitEventBuilderPluginFactory, NSVfitEventBuilder, "NSVfitEventBuilder");
