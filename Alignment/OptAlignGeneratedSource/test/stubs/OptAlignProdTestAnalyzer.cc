// -*- C++ -*-
//
// Package:    AlignmentTools/OptAlignGeneratedSource/test/stubs
// Class:      OptAlignProdTestAnalyzer.cc
// 
/**\class OptAlignProdTestAnalyzer OptAlignProdTestAnalyzer.cc AlignmentTools/OptAlignGeneratedSource/test/stubs/OptAlignProdTestAnalyzer.cc

 Description: test access to the OpticalAlignMeasurements via OpticalAlignMeasurementsGeneratedSource
    This also should demonstrate access to a geometry via the XMLIdealGeometryESSource
    for use in THE COCOA analyzer.

 Implementation:
     Iterate over retrieved alignments.
*/
//
// Original Author:  Mike Case
//         Created:  Mon Jan 17 11:47:40 CET 2006
// $Id: OptAlignProdTestAnalyzer.cc,v 1.5 2006/03/29 16:46:46 case Exp $
//
//

#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/OptAlignObjects/interface/OAQuality.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignInfo.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurements.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignMeasurementInfo.h"

#include "CondFormats/DataRecord/interface/OpticalAlignmentsRcd.h"

#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDSpecifics.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "CLHEP/Matrix/Matrix.h"

using namespace std;

  class OptAlignProdTestAnalyzer : public edm::EDAnalyzer
  {
  public:

    explicit  OptAlignProdTestAnalyzer(edm::ParameterSet const& p) 
    { }
    explicit  OptAlignProdTestAnalyzer(int i) 
    { }
    virtual ~ OptAlignProdTestAnalyzer() { }

    virtual void beginJob(const edm::EventSetup& c);
    virtual void analyze(const edm::Event& e, const edm::EventSetup& c);
    // see note on endJob() at the bottom of the file.
    // virtual void endJob() ;
  private:
    OpticalAlignments oa_;
    OpticalAlignMeasurements measList_;
    double myFetchDbl(const DDsvalues_type& dvst, 
		      const std::string& spName,
		      const size_t& vecInd ) {
      DDValue val(spName, 0.0);
      if (DDfetch(&dvst,val)) {
	if ( val.doubles().size() > vecInd ) {
	  //	  std::cout << "about to return: " << val.doubles()[vecInd] << std::endl;
	  return val.doubles()[vecInd];
	} else {
	  std::cout << "WARNING: OUT OF BOUNDS RETURNING 0 for index " << vecInd << " of SpecPar " << spName << std::endl;
	}
      }
      return 0.0;
    };
  };

void OptAlignProdTestAnalyzer::beginJob ( const edm::EventSetup& c ) {
  // STEP ONE:  Initial COCOA objects will be built from a DDL geometry
  // description.  
  // NOTE: For analysis jobs access to eventsetup DDD is only in analyze.
  // Only for the COCOA analysis would this be accessed via
  // beginJob.
  
  edm::ESHandle<DDCompactView> cpv;
  c.get<IdealGeometryRecord>().get(cpv);
  
  std::cout << " OptAlignProdTestAnalyzer " << cpv->root() << std::endl;
  
  // Example of traversing the whole optical alignment geometry.
  // At each node we get specpars as variables and use them in 
  // constructing COCOA objects.  Right now this only loads 
  // OpticalAlignments and does not create any COCOA objects.
  //  It stores these objects in a private data member oa_
  std::string attribute = "COCOA"; 
  std::string value     = "COCOA";
  DDValue val(attribute, value, 0.0);
  
  // get all parts labelled with COCOA using a SpecPar
  DDSpecificsFilter filter;
  filter.setCriteria(val,  // name & value of a variable 
		     DDSpecificsFilter::matches,
		     DDSpecificsFilter::AND, 
		     true, // compare strings otherwise doubles
		     true  // use merged-specifics or simple-specifics
		     );
  DDFilteredView fv(*cpv);
  fv.addFilter(filter);
  bool doCOCOA = fv.firstChild();
  
  // Loop on parts
  int numParts=0;
  OpticalAlignInfo oai;
  OpticalAlignParam oap;
  OpticalAlignMeasurementInfo meas;
  
  while ( doCOCOA ){
    ++numParts;
    const DDsvalues_type params(fv.mergedSpecifics());
    
    const DDLogicalPart lv = fv.logicalPart();
    oai.name_ = lv.name();
    
    oai.x_.quality_  = int (myFetchDbl(params, "centre_X_quality", 0));
    
    oai.x_.value_ = fv.translation().x();
    oai.x_.error_ = myFetchDbl(params, "centre_X_sigma", 0);
    oai.x_.quality_  = int (myFetchDbl(params, "centre_X_quality", 0));
    
    oai.y_.value_ = fv.translation().y();
    oai.y_.error_ = myFetchDbl(params, "centre_Y_sigma", 0);
    oai.y_.quality_  = int (myFetchDbl(params, "centre_Y_quality", 0));

    oai.z_.value_ = fv.translation().z();
    oai.z_.error_ = myFetchDbl(params, "centre_Z_sigma", 0);
    oai.z_.quality_  = int (myFetchDbl(params, "centre_Z_quality", 0));

    oai.x_.value_ = fv.translation().x();
    oai.x_.error_ = myFetchDbl(params, "angles_X_sigma", 0);
    oai.x_.quality_  = int (myFetchDbl(params, "angles_X_quality", 0));

    oai.y_.value_ = fv.translation().y();
    oai.y_.error_ = myFetchDbl(params, "angles_Y_sigma", 0);
    oai.y_.quality_  = int (myFetchDbl(params, "angles_Y_quality", 0));

    oai.z_.value_ = fv.translation().z();
    oai.z_.error_ = myFetchDbl(params, "angles_Z_sigma", 0);
    oai.z_.quality_  = int (myFetchDbl(params, "angles_Z_quality", 0));

    std::cout << " OBJECT " <<  oai.name_ << " pos/angle read " << std::endl;
    
    std::vector<std::string> names, dims;
    std::vector<double> values, errors, quality;
    const std::vector<const DDsvalues_type *> params2(fv.specifics());
    std::vector<const DDsvalues_type *>::const_iterator spit = params2.begin();
    std::vector<const DDsvalues_type *>::const_iterator endspit = params2.end();
    for ( ; spit != endspit; ++spit ) {
      DDsvalues_type::const_iterator sit = (**spit).begin();
      DDsvalues_type::const_iterator endsit = (**spit).end();
      for ( ; sit != endsit; ++sit ) {
 	if (sit->second.name() == "cocoa_type") {
          oai.type_ = sit->second.strings()[0];
	} else if (sit->second.name() == "extra_entry") {
	  names = sit->second.strings();
	} else if (sit->second.name() == "dimType") {
	  dims = sit->second.strings();
	} else if (sit->second.name() == "value") {
	  values = sit->second.doubles();
	} else if (sit->second.name() == "sigma") {
	  errors = sit->second.doubles();
	} else if (sit->second.name() == "quality") {
	  quality = sit->second.doubles();
	}
	
      }
    }
    
    oai.ID_ = numParts;
    if ( names.size() == dims.size() && dims.size() == values.size() 
	 && values.size() == errors.size() && errors.size() == quality.size() ) {
      for ( size_t ind = 0; ind < names.size(); ++ind ) {
	oap.value_ = values[ind];
	oap.error_ = errors[ind];
	oap.quality_ = int (quality[ind]);
	oap.name_ = names[ind];
	oai.extraEntries_.push_back (oap);
	oap.clear();
      }

     std::cout << " OBJECT " <<  oai.name_ << " extra entries read " << std::endl;

      oa_.opticalAlignments_.push_back(oai);
    } else {
      std::cout << "WARNING FOR NOW: sizes of extra parameters (names, dimType, value, quality) do"
		<< " not match!  Did not add " << numParts << " item to OpticalAlignments." 
		<< std::endl;
    }
//       std::cout << "sizes are values=" << values.size();
//       std::cout << "  sigma(errors)=" << errors.size();
//       std::cout << "  quality=" << quality.size();
//       std::cout << "  names=" << names.size();
//       std::cout << "  dimType=" << dims.size() << std::endl;
    oai.clear();
    doCOCOA = fv.next(); // go to next part
  } // while (doCOCOA)
  std::cout << "Finished making " << numParts << " OpticalAlignInfo objects" << std::endl;
} // end of ::beginJob

void OptAlignProdTestAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context)
{
  std::cout <<" I " << std::endl;
  using namespace edm::eventsetup;
  std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
  std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
 
  // just a quick dump of the private OpticalAlignments object
  std::cout << oa_ << std::endl;
 
  // STEP 2:
  // Get calibrated OpticalAlignments.  In this case I'm using
  // some sqlite database examples that are generated using
  // testOptAlignWriter.cc
  // from CondFormats/OptAlignObjects/test/

//   edm::ESHandle<OpticalAlignments> oaESHandle;
//   context.get<OpticalAlignmentsRcd>().get(oaESHandle);

//   // This assumes they ALL come in together.  This may not be
//   // the "real" case.  One could envision different objects coming
//   // in and we would get by label each one (type).

//   std::cout << "========== eventSetup data changes with IOV =========" << std::endl;
//   std::cout << *oaESHandle << std::endl;
//   //============== COCOA WORK!
//   //  calibrated values should be used to "correct" the ones read in during beginJob
//   //==============
  
//   // 
//   // to see how to iterate over the OpticalAlignments, please
//   // refer to the << operator of OpticalAlignments, OpticalAlignInfo
//   // and OpticalAlignParam.
//   //     const OpticalAlignments* myoa=oa.product();
  
//   // STEP 3:
//   // This retrieves the Measurements
//   // for each event, a new set of measurements is available.
  edm::Handle<OpticalAlignMeasurements> measHandle;
  e.getByLabel("OptAlignGeneratedSource", measHandle); 
  
  std::cout << "========== event data product changes with every event =========" << std::endl;
  std::cout << *measHandle << std::endl;

  //============== COCOA WORK!
  //  Each set of optical alignment measurements can be used
  //  in whatever type of analysis COCOA does. 
  //==============

} //end of ::analyze()

// STEP 4:  one could use ::endJob() to write out the OpticalAlignments
// generated by the analysis. Example code of writing is in
// CondFormats/Alignment/test/testOptAlignWriter.cc


DEFINE_FWK_MODULE(OptAlignProdTestAnalyzer)
