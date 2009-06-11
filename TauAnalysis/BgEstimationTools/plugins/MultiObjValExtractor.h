#ifndef TauAnalysis_BgEstimationTools_MultiObjValExtractor_h
#define TauAnalysis_BgEstimationTools_MultiObjValExtractor_h

/** \class MultiObjValExtractor
 *
 * Use extractor plugins derrived from ObjValExtractorBase 
 * to extract a set of scalar values from PAT objects
 * and return the result as a vector
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: MultiObjValExtractor.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValVectorExtractorBase.h"
#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <vector>

class MultiObjValExtractor : public ObjValVectorExtractorBase
{
 public:
  // constructor 
  explicit MultiObjValExtractor(const edm::ParameterSet&);
  
  // destructor
  ~MultiObjValExtractor();

  // number of components in vector of extracted scalar values
  unsigned size() const { return objValExtractors_.size(); }
  
  // method for extracting vector of scalar values
  std::vector<double> operator()(const edm::Event&) const;

 private:
  std::vector<ObjValExtractorBase*> objValExtractors_;
};

#endif  

