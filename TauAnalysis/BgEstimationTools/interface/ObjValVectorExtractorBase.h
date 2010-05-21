#ifndef TauAnalysis_BgEstimationTools_ObjValVectorExtractorBase_h
#define TauAnalysis_BgEstimationTools_ObjValVectorExtractorBase_h

/** \class ObjValVectorExtractorBase
 *
 * Pure virtual base-class for extracting vectors of scalar values from PAT objects
 * (derived classes are auxiliary classes used for binning and Ntuple filling purposes)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjValVectorExtractorBase.h,v 1.1 2009/06/11 07:23:28 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class ObjValVectorExtractorBase
{
 public:
  // constructor 
  explicit ObjValVectorExtractorBase() {}
  
  // destructor
  virtual ~ObjValVectorExtractorBase() {}

  // abstract method for extracting scalar value
  virtual std::vector<double> operator()(const edm::Event&) const = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<ObjValVectorExtractorBase* (const edm::ParameterSet&)> ObjValVectorExtractorPluginFactory;

#endif  

