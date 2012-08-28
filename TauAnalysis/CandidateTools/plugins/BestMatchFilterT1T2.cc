#include "TauAnalysis/CandidateTools/plugins/BestMatchFilterT1T2.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"

#include <TMath.h>

template <typename T1, typename T2>
BestMatchFilterT1T2<T1, T2>::BestMatchFilterT1T2(const edm::ParameterSet& cfg)
  : moduleLabel_(cfg.getParameter<std::string>("@module_label")),
    expressionRef_(0),
    expressionTest_(0)
{
  srcRef_ = cfg.getParameter<edm::InputTag>("srcRef");
  std::string expressionRef_string = cfg.getParameter<std::string>("expressionRef");
  expressionRef_ = new StringObjectFunction<T1>(expressionRef_string);

  srcTest1_ = cfg.getParameter<edm::InputTag>("srcTest1");
  srcTest2_ = cfg.getParameter<edm::InputTag>("srcTest2");
  std::string expressionTest_string = cfg.getParameter<std::string>("expressionTest");
  expressionTest_ = new StringObjectFunction<T2>(expressionTest_string);
}

template <typename T1, typename T2>
BestMatchFilterT1T2<T1, T2>::~BestMatchFilterT1T2()
{
  delete expressionRef_;
  delete expressionTest_;
}

template <typename T1, typename T2>
bool BestMatchFilterT1T2<T1, T2>::filter(edm::Event& evt, const edm::EventSetup& es)
{
  typedef edm::View<T1> RefView;
  edm::Handle<RefView> refObjects;
  evt.getByLabel(srcRef_, refObjects);

  typedef edm::View<T2> TestView;
  edm::Handle<TestView> testObjects1;
  evt.getByLabel(srcTest1_, testObjects1);
  edm::Handle<TestView> testObjects2;
  evt.getByLabel(srcTest2_, testObjects2);

  if ( refObjects->size() != testObjects1->size() ||
       refObjects->size() != testObjects2->size() ) 
    edm::LogWarning ("BestMatchFilter::filter")
      << " Support for differences in collection size not implemented yet --> skipping Event !!";

  size_t numObjects1bestMatch = 0;
  size_t numObjects2bestMatch = 0;

  size_t numObjects = refObjects->size();
  for ( size_t iObject = 0; iObject < numObjects; ++iObject ) {
    double refValue = (*expressionRef_)(refObjects->at(iObject));

    double testValue1 = (*expressionTest_)(testObjects1->at(iObject));
    double testValue2 = (*expressionTest_)(testObjects2->at(iObject));
    
    double diff1 = TMath::Abs(testValue1 - refValue);
    double diff2 = TMath::Abs(testValue2 - refValue);
    if      ( diff1 < diff2 ) ++numObjects1bestMatch;
    else if ( diff2 < diff1 ) ++numObjects2bestMatch;
  }

  return (numObjects1bestMatch > numObjects2bestMatch);
}

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesis.h"
#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitEventHypothesisByIntegration.h"

typedef BestMatchFilterT1T2<reco::Candidate, NSVfitEventHypothesis> BestMatchFilterCandidateToNSVfitEventHypothesis;
typedef BestMatchFilterT1T2<reco::Candidate, NSVfitEventHypothesisByIntegration> BestMatchFilterCandidateToNSVfitEventHypothesisByIntegration;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(BestMatchFilterCandidateToNSVfitEventHypothesis);
DEFINE_FWK_MODULE(BestMatchFilterCandidateToNSVfitEventHypothesisByIntegration);
