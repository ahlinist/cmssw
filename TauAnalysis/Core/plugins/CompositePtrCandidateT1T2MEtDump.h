#ifndef TauAnalysis_Core_CompositePtrCandidateT1T2MEtDump_h  
#define TauAnalysis_Core_CompositePtrCandidateT1T2MEtDump_h

/** \class PATTauDump
 *
 * Print-out details of di-Tau objects 
 * contained in collection specified by configuration parameter
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.12 $
 *
 * $Id: CompositePtrCandidateT1T2MEtDump.h,v 1.12 2010/02/05 14:05:30 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

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
};

#endif  


