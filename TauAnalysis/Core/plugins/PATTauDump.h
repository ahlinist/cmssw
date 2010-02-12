#ifndef TauAnalysis_Core_PATTauDump_h  
#define TauAnalysis_Core_PATTauDump_h

/** \class PATTauDump
 *
 * Print-out details of pat::Tau objects 
 * contained in collection specified by configuration parameter
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.12 $
 *
 * $Id: PATMuonDump.h,v 1.12 2010/02/05 14:05:30 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

class PATTauDump : public ObjectDumpBase
{
 public:  
  explicit PATTauDump(const edm::ParameterSet&);
  ~PATTauDump();

 protected:
  void print(const edm::Event&, const edm::EventSetup&) const;

 private:
  edm::InputTag patTauSource_;
  edm::InputTag genParticleSource_;

  bool printTauIdEfficiencies_;

  std::vector<int> skipPdgIdsGenParticleMatch_;
};

#endif  


