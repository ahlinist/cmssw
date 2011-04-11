#ifndef TauAnalysis_Core_PATMuonDump_h  
#define TauAnalysis_Core_PATMuonDump_h

/** \class PATMuonDump
 *
 * Print-out details of pat::Muon objects 
 * contained in collection specified by configuration parameter
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATMuonDump.h,v 1.2 2011/02/18 11:32:31 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Muon.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

class PATMuonDump : public ObjectDumpBase
{
 public:  
  explicit PATMuonDump(const edm::ParameterSet&);
  ~PATMuonDump();

 protected:
  void print(const edm::Event&, const edm::EventSetup&) const;

 private:
  edm::InputTag patMuonSource_;
  edm::InputTag genParticleSource_;
  edm::InputTag vertexSource_;
  edm::InputTag beamSpotSource_;

  std::vector<int> skipPdgIdsGenParticleMatch_;

  ParticlePFIsolationExtractor<pat::Muon>* pfIsolationExtractor_; 
  edm::InputTag pfIsoCandSource_;
};

#endif  


