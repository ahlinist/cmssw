#ifndef TauAnalysis_BgEstimationTools_ObjValExtractorBase_h
#define TauAnalysis_BgEstimationTools_ObjValExtractorBase_h

/** \class ObjValExtractorBase
 *
 * Base-class for extracting scalar values from PAT objects
 * (derived classes are auxiliary classes used for binning and Ntuple filling purposes)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjValExtractorBase.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class ObjValExtractorBase
{
 public:
  // constructor 
  explicit ObjValExtractorBase() {}
  
  // destructor
  virtual ~ObjValExtractorBase() {}

  // method for extracting scalar value
  virtual double operator()(const edm::Event&) const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<ObjValExtractorBase* (const edm::ParameterSet&)> ObjValExtractorPluginFactory;

#endif  

