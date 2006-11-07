#include "Alignment/OptAlignGeneratedSource/interface/OptAlignGeneratedSource.h"

#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurements.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurementInfo.h"
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
  produces<OpticalAlignMeasurements>();
}
  
OptAlignGeneratedSource::~OptAlignGeneratedSource () { }


bool OptAlignGeneratedSource::produce ( edm::Event& e ) {
  std::auto_ptr<OpticalAlignMeasurements> result(new OpticalAlignMeasurements);
  // ... fill the collection ... assign numObjects_ random values for now.
  OpticalAlignParam temp;
  OpticalAlignMeasurementInfo xxxxinfo;

  for ( size_t i = 0; i < numObjects_ ; i++ ) {
    // make up some length value for x
    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;
    temp.quality_ = int (oa_fixed); // arbitrary
    /*
    xxxxinfo.x1_ = temp;

    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;

    xxxxinfo.x2_ = temp;

    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;

    xxxxinfo.x3_ = temp;

    temp.value_  = RandFlat::shoot(1.0, 5.0);
    temp.error_  = temp.value_ / 20;

    xxxxinfo.x4_ = temp;

    temp.value_  = RandFlat::shoot(0.0, 3.14);
    temp.error_  = RandFlat::shoot(0.0, 0.003);

    xxxxinfo.extraEntries_.push_back( temp );

    temp.value_  = RandFlat::shoot(0.0, 10.14);
    temp.error_  = RandFlat::shoot(0.0, 0.3);

    xxxxinfo.extraEntries_.push_back( temp );

    temp.value_  = RandFlat::shoot(0.0, 100.4);
    temp.error_  = RandFlat::shoot(0.0, 1.3);

    xxxxinfo.extraEntries_.push_back( temp );

    xxxxinfo.objectType_ = type_;
    xxxxinfo.objectID_ = i;


    result->xxxxMeasurements_.push_back(xxxxinfo);
    xxxxinfo.clear();
    */
  }

  e.put(result);
  return true;
}

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_INPUT_SOURCE(OptAlignGeneratedSource)
