#ifndef TauAnalysis_CandidateTools_BestMatchFilterT1T2_h
#define TauAnalysis_CandidateTools_BestMatchFilterT1T2_h

/** \class BestMatchFilterT1T2
 *
 * Select events for which objects in collection1 
 * better match reference objects than objects in collection2
 *
 * NOTE: Objects contained in reference and test collections may be of different type,
 *       but all collections are expected to contain the same number of elements
 *      (e.g. same physical objects reconstructed by different algorithms
 *            which are to be compared to Monte Carlo truth)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: BestMatchFilter.h,v 1.1 2010/11/14 13:07:13 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <string>

template <typename T1, typename T2>
class BestMatchFilterT1T2 : public edm::EDFilter
{
 public:
  // constructor 
  explicit BestMatchFilterT1T2(const edm::ParameterSet&);
    
  // destructor
  virtual ~BestMatchFilterT1T2();
    
 private:
  bool filter(edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcRef_;
  StringObjectFunction<T1>* expressionRef_;
  
  edm::InputTag srcTest1_;
  edm::InputTag srcTest2_;
  StringObjectFunction<T2>* expressionTest_;
};

#endif   
