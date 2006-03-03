#include "OpticalAlignment/OptAlignGeneratedSource/interface/OptAlignGeneratedSource.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignInfo.h"
#include "CondFormats/OptAlignObjects/interface/OAQuality.h"

#include "FWCore/Framework/interface/ExternalInputSource.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "CLHEP/Random/RandFlat.h"
#include <boost/cstdint.hpp>

OptAlignGeneratedSource::OptAlignGeneratedSource ( edm::ParameterSet const& ps
					   , edm::InputSourceDescription const& desc) 
  : edm::GeneratedInputSource::GeneratedInputSource(ps, desc)
{
  // ... read any configuration parameters specific to your input source
  // number of play alighments per "payload"
  numObjects_ = ps.getParameter<boost::uint32_t>("numObjects");
  type_ = ps.getParameter<std::string>("type");

  // note: no argument in the call to produces
  // the standard module label is assumed
  produces<OpticalAlignments>();
}
  
OptAlignGeneratedSource::~OptAlignGeneratedSource () { }


bool OptAlignGeneratedSource::produce ( edm::Event& e ) {
  std::auto_ptr<OpticalAlignments> result(new OpticalAlignments);
  // ... fill the collection ... assign numObjects_ random values for now.
  OpticalAlignParam temp;
  OpticalAlignInfo oainfo;

  for ( size_t i = 0; i < numObjects_ ; i++ ) {
    // make up some length value for x
    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;
    temp.qual_ = int (oa_unknown); // arbitrary

    oainfo.x_ = temp;

    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;

    oainfo.y_ = temp;

    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;

    oainfo.z_ = temp;

    // make it more of an angle
    temp.value_  = RandFlat::shoot(0.0, 3.14);
    temp.error_  = RandFlat::shoot(0.0, 0.003);

    oainfo.angx_ = temp;

    temp.value_  = RandFlat::shoot(0.0, 3.14);
    temp.error_  = RandFlat::shoot(0.0, 0.003);

    oainfo.angy_ = temp;

    temp.value_  = RandFlat::shoot(0.0, 3.14);
    temp.error_  = RandFlat::shoot(0.0, 0.003);

    oainfo.angz_ = temp;

    oainfo.objectType_ = type_;
    oainfo.objectID_ = i;

    // fill in one BS. entry for the ExtraEntries.
    temp.value_  = RandFlat::shoot(1.0, 10.0);
    temp.error_  = temp.value_ / 20;
    temp.qual_ = int (oa_fixed);
    oainfo.extraEntries_.push_back(temp);
    oainfo.extraEntries_.push_back(temp);

    result->opticalAlignments_.push_back(oainfo);
    oainfo.extraEntries_.clear();
  }

  e.put(result);
  return true;
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(OptAlignGeneratedSource)
