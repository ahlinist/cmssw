// -*- C++ -*-
//
// Package:    AlignmentTools/OptAlignGeneratedSource/test/stubs
// Class:      OptAlignProdTestAnalyzer.cc
// 
/**\class OptAlignProdTestAnalyzer OptAlignProdTestAnalyzer.cc AlignmentTools/OptAlignGeneratedSource/test/stubs/OptAlignProdTestAnalyzer.cc

 Description: test access to the OptAlignments via OptAlignGeneratedSource
    This also should demonstrate access to a geometry via the XMLIdealGeometryESSource
    for use in THE COCOA analyzer.

 Implementation:
     Iterate over retrieved alignments.
*/
//
// Original Author:  Mike Case
//         Created:  Mon Jan 17 11:47:40 CET 2006
// $Id: OptAlignProdTestAnalyzer.cc,v 1.1 2006/01/26 15:03:51 case Exp $
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

#include "CondFormats/OptAlignObjects/interface/OpticalAlignments.h"
#include "CondFormats/OptAlignObjects/interface/OpticalAlignInfo.h"
#include "CondFormats/OptAlignObjects/interface/OAQuality.h"

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
    double myFetchDbl(const DDsvalues_type& dvst, 
		      const std::string& spName,
		      const size_t& vecInd ) {
      DDValue val(spName, 0.0);
      if (DDfetch(&dvst,val)) {
	if ( val.doubles().size() > vecInd ) {
	  std::cout << "about to return: " << val.doubles()[vecInd] << std::endl;
	  return val.doubles()[vecInd];
	} else {
	  std::cout << "WARNING: OUT OF BOUNDS RETURNING 0 for index " << vecInd << " of SpecPar " << spName << std::endl;
	}
      }
      return 0.0;
    };
  };

void OptAlignProdTestAnalyzer::beginJob ( const edm::EventSetup& c ) {
  // STEP ONE:  I'm not sure, but I believe the initial COCOA 
  // objects will be built from a DetectorDescription geometry
  // description.  It is STILL not clear to me if this is the 
  // "standard, Ideal CMS" geometry or some special files for
  // COCOA.
  // IMPORTANT: for analysis jobs this would still be in analyze
  // only for the COCOA analysis would this be accessed via
  // beginJob.
  
  edm::ESHandle<DDCompactView> cpv;
  c.get<IdealGeometryRecord>().get(cpv);

  std::cout << cpv->root() << std::endl;

  // example of traversing the whole optical alignment geometry.
  // more can be done here, for example, at each node, one could
  // request any specpars as variables and use them in constructing
  // COCOA objects.
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
  while ( doCOCOA ){
    ++numParts;
    const DDsvalues_type params(fv.mergedSpecifics());


    oai.x_.value_ = fv.translation().x();
    oai.x_.error_ = myFetchDbl(params, "centre_X_sigma", 0);
    oai.x_.qual_  = int (myFetchDbl(params, "centre_X_quality", 0));

    oai.y_.value_ = fv.translation().y();
    oai.y_.error_ = myFetchDbl(params, "centre_Y_sigma", 0);
    oai.y_.qual_  = int (myFetchDbl(params, "centre_Y_quality", 0));

    oai.z_.value_ = fv.translation().z();
    oai.z_.error_ = myFetchDbl(params, "centre_Z_sigma", 0);
    oai.z_.qual_  = int (myFetchDbl(params, "centre_Z_quality", 0));

    oai.x_.value_ = fv.translation().x();
    oai.x_.error_ = myFetchDbl(params, "angles_X_sigma", 0);
    oai.x_.qual_  = int (myFetchDbl(params, "angles_X_quality", 0));

    oai.y_.value_ = fv.translation().y();
    oai.y_.error_ = myFetchDbl(params, "angles_Y_sigma", 0);
    oai.y_.qual_  = int (myFetchDbl(params, "angles_Y_quality", 0));

    oai.z_.value_ = fv.translation().z();
    oai.z_.error_ = myFetchDbl(params, "angles_Z_sigma", 0);
    oai.z_.qual_  = int (myFetchDbl(params, "angles_Z_quality", 0));

    std::vector<std::string> names, dims;
    std::vector<double> values, errors, quality;
    const std::vector<const DDsvalues_type *> params2(fv.specifics());
    std::vector<const DDsvalues_type *>::const_iterator spit = params2.begin();
    std::vector<const DDsvalues_type *>::const_iterator endspit = params2.end();
    for ( ; spit != endspit; ++spit ) {
      DDsvalues_type::const_iterator sit = (**spit).begin();
      DDsvalues_type::const_iterator endsit = (**spit).end();
      for ( ; sit != endsit; ++sit ) {
	if (sit->second.name() == "name") {
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
    oai.objectType_ = "fred";
    oai.objectID_ = numParts;
    if ( names.size() == dims.size() && dims.size() == values.size() 
	 && values.size() == errors.size() && errors.size() == quality.size() ) {
      for ( size_t ind = 0; ind < names.size(); ++ind ) {
	oap.value_ = values[ind];
	oap.error_ = errors[ind];
	oap.qual_ = int (quality[ind]);
	oap.name_ = names[ind];
	oai.extraEntries_.push_back (oap);
	oap.clear();
      }
      oa_.opticalAlignments_.push_back(oai);
    } else {
      std::cout << "WARNING FOR NOW: sizes of extra parameters (names, dimType, value, quality) do"
		<< " not match!  Did not add " << numParts << " item to OpticalAlignments." 
		<< std::endl;
    }
      std::cout << "sizes are values=" << values.size();
      std::cout << "  sigma(errors)=" << errors.size();
      std::cout << "  quality=" << quality.size();
      std::cout << "  names=" << names.size();
      std::cout << "  dimType=" << dims.size() << std::endl;
    oai.clear();
    doCOCOA = fv.next(); // go to next part
  } // while (doCOCOA)
  std::cout << "Finished making " << numParts << " OpticalAlignInfo objects" << std::endl;
} // end of ::beginJob

void OptAlignProdTestAnalyzer::analyze(const edm::Event& e, const edm::EventSetup& context)
{
  using namespace edm::eventsetup;
  std::cout <<" I AM IN RUN NUMBER "<<e.id().run() <<std::endl;
  std::cout <<" ---EVENT NUMBER "<<e.id().event() <<std::endl;
  
  // If one wanted to get the "initial" version from an existing
  // alignment PoolDBESSource they would do the following:
  //     edm::ESHandle<OpticalAlignments> oa;
  //     context.get<OpticalAlignmentsRcd>().get(oa);
  //     //  just iterate over all of them...
  // to see how to iterate over the OpticalAlignments, please
  // refer to the << operator of OpticalAlignments, OpticalAlignInfo
  // and OpticalAlignParam.
  //     const OpticalAlignments* myoa=oa.product();
  
  // This retrieves the OpticalAlignments via the event data.
  // for each event, a new set of alignments comes in to the 
  // framework.
  edm::Handle<OpticalAlignments> oaHandle;
  e.getByLabel("OptAlignGeneratedSource", oaHandle); 
  
  std::cout << *oaHandle << std::endl;

  //============== COCOA WORK!
  //  Each set of optical alignment corrections can be used
  //  in whatever type of analysis COCOA does. 
  //==============
  
} //end of ::analyze()

// one could use ::endJob() to write out the OpticalAlignments
// generated by the analysis. Example code is in
// CondFormats/Alignment/test/testOptAlignWriter.cc


DEFINE_FWK_MODULE(OptAlignProdTestAnalyzer)
