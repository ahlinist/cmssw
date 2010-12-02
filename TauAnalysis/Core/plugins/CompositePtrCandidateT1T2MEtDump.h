#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtDump_h  
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtDump_h

/** \class PATTauDump
 *
 * Print-out details of di-Tau objects 
 * contained in collection specified by configuration parameter
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: CompositePtrCandidateT1T2MEtDump.h,v 1.3 2010/09/02 11:33:45 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

#include <vector>
#include <string>

template<typename T1, typename T2>
class CompositePtrCandidateT1T2MEtDump : public ObjectDumpBase
{
 public:  
  explicit CompositePtrCandidateT1T2MEtDump(const edm::ParameterSet&);
  ~CompositePtrCandidateT1T2MEtDump();

 protected:
  void print(const edm::Event&, const edm::EventSetup&) const;

 private:
  edm::InputTag diTauCandidateSource_;

  typedef std::vector<std::string> vstring;
  struct svFitAlgorithmType
  {
    std::string algorithmName_;
    vstring polarizationHypotheses_;
  };
  std::vector<svFitAlgorithmType> svFitAlgorithms_;
  
  struct annotationType
  {
    annotationType(const edm::ParameterSet& cfg)
    {
      std::string condition_string = cfg.getParameter<std::string>("condition");
      condition_ = new StringCutObjectSelector<CompositePtrCandidateT1T2MEt<T1, T2> >(condition_string);
      text_ = cfg.getParameter<std::string>("text");
    }
    ~annotationType()
    {
      delete condition_;
    }
    StringCutObjectSelector<CompositePtrCandidateT1T2MEt<T1, T2> >* condition_;
    std::string text_;
  };
  std::vector<annotationType*> annotations_;

  edm::InputTag genParticleSource_;
};

#endif  


