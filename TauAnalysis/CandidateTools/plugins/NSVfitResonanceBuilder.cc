#include "TauAnalysis/CandidateTools/plugins/NSVfitResonanceBuilder.h"

#include "FWCore/Utilities/interface/Exception.h"

#include <vector>
#include <string>

NSVfitResonanceBuilder::NSVfitResonanceBuilder(const edm::ParameterSet& cfg)
  : NSVfitResonanceBuilderBase(cfg)
{
  edm::ParameterSet cfg_daughters = cfg.getParameter<edm::ParameterSet>("daughters");
  typedef std::vector<std::string> vstring;
  vstring daughterNames = cfg_daughters.getParameterNamesForType<edm::ParameterSet>();
  for ( vstring::const_iterator daughterName = daughterNames.begin();
	daughterName != daughterNames.end(); ++daughterName ) {
    edm::ParameterSet cfg_daughter = cfg_daughters.getParameter<edm::ParameterSet>(*daughterName);
    edm::ParameterSet cfg_builder = cfg_daughter.getParameter<edm::ParameterSet>("builder");
    cfg_builder.addParameter<std::string>("prodParticleLabel", *daughterName);
    std::string pluginType = cfg_builder.getParameter<std::string>("pluginType");
    NSVfitSingleParticleBuilderBase* daughterBuilder =
      NSVfitSingleParticleBuilderPluginFactory::get()->create(pluginType, cfg_builder);
    daughterBuilders_.push_back(daughterBuilder);
    ++numDaughterBuilders_;
  }

  if ( cfg.exists("polStates") ) {
    typedef std::vector<std::string> vstring;
    vstring polStates_string = cfg.getParameter<vstring>("polStates");
    for ( vstring::const_iterator polState_string = polStates_string.begin();
	  polState_string != polStates_string.end(); ++polState_string ) {
      int polHandedness = -1;
      if      ( (*polState_string) == "undefined" ) polHandedness = NSVfitResonanceHypothesis::kPolUndefined;
      else if ( (*polState_string) == "LR"        ) polHandedness = NSVfitResonanceHypothesis::kPolLR;
      else if ( (*polState_string) == "RL"        ) polHandedness = NSVfitResonanceHypothesis::kPolRL;
      else if ( (*polState_string) == "LL"        ) polHandedness = NSVfitResonanceHypothesis::kPolLL;
      else if ( (*polState_string) == "RR"        ) polHandedness = NSVfitResonanceHypothesis::kPolRR;
      else throw cms::Exception("NSVfitResonanceBuilder")
	<< " Invalid Configuration Parameter 'polState' = " << (*polState_string) << " !!\n";
      polHandedness_.push_back(polHandedness);
    }
  } else {
    polHandedness_.push_back(NSVfitResonanceHypothesis::kPolUndefined);
  }
  numPolStates_ = polHandedness_.size();
}

NSVfitResonanceHypothesis* NSVfitResonanceBuilder::build(const inputParticleMap& inputParticles) const
{
//--- set polarization status for resonance
  NSVfitResonanceHypothesis* resonance = NSVfitResonanceBuilderBase::build(inputParticles);
  resonance->polHandedness_ = polHandedness_;
  resonance->numPolStates_ = numPolStates_;

//--- set polarization status for daughters 
  if ( resonance->numDaughters() == 2 ) {
    for ( size_t iDaughter = 0; iDaughter < resonance->numDaughters(); ++iDaughter ) {
      NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
      daughter->polHandedness_.resize(numPolStates_);
      daughter->polSign_.resize(numPolStates_);
      daughter->numPolStates_ = numPolStates_;
      for ( unsigned iPolState = 0; iPolState < numPolStates_; ++iPolState ) {
	int resonance_polHandedness = polHandedness_[iPolState];
	int daughter_polHandedness = -1;
	if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolUndefined ) {
	  daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolUndefined;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolLR ) {
	  if      ( iDaughter == 0 ) daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolL;
	  else if ( iDaughter == 1 ) daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolR;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolRL ) {
	  if      ( iDaughter == 0 ) daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolL;
	  else if ( iDaughter == 1 ) daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolR;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolLL ) {
	  daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolL;
	} else if ( resonance_polHandedness == NSVfitResonanceHypothesis::kPolRR ) {
	  daughter_polHandedness = NSVfitSingleParticleHypothesis::kPolR;
	} 
	assert(daughter_polHandedness != -1);
	daughter->polHandedness_[iPolState] = daughter_polHandedness;
	int daughter_polSign = 0;
	// CV: left-handed  tau- and right-handed tau+ are assigned polarization -1,
	//     right-handed tau- and left-handed  tau+ are assigned polarization +1
	if ( daughter_polHandedness == NSVfitSingleParticleHypothesis::kPolL ) {
	  if      ( daughter->particle()->charge() < -0.5 ) daughter_polSign = -1;
	  else if ( daughter->particle()->charge() > +0.5 ) daughter_polSign = +1;
	} else if ( daughter_polHandedness == NSVfitSingleParticleHypothesis::kPolR ) {
	  if      ( daughter->particle()->charge() < -0.5 ) daughter_polSign = +1;
	  else if ( daughter->particle()->charge() > +0.5 ) daughter_polSign = -1;
	} 
	daughter->polSign_[iPolState] = daughter_polSign;
      }
    }
  } else if ( numPolStates_ == 1 && polHandedness_[0] == NSVfitResonanceHypothesis::kPolUndefined ) {
    for ( size_t iDaughter = 0; iDaughter < resonance->numDaughters(); ++iDaughter ) {
      NSVfitSingleParticleHypothesis* daughter = resonance->daughter(iDaughter);
      daughter->polHandedness_.resize(numPolStates_);
      daughter->polHandedness_[0] = NSVfitSingleParticleHypothesis::kPolUndefined;
      daughter->polSign_.resize(numPolStates_);
      daughter->polSign_[0] = 0;
      daughter->numPolStates_ = numPolStates_;
    }
  } else throw cms::Exception("NSVfitResonanceBuilder")
      << " Support for Polarization not implemented for case of " << resonance->numDaughters() << " daughters yet !!\n";

  return resonance;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(NSVfitResonanceBuilderPluginFactory, NSVfitResonanceBuilder, "NSVfitResonanceBuilder");
