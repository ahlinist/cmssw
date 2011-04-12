#ifndef TauAnalysis_Core_PATElectronDump_h  
#define TauAnalysis_Core_PATElectronDump_h

/** \class PATElectronDump
 *
 * Print-out details of pat::Electron objects 
 * contained in collection specified by configuration parameter
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: PATElectronDump.h,v 1.3 2011/04/11 09:54:48 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

class PATElectronDump : public ObjectDumpBase
{
 public:  
  explicit PATElectronDump(const edm::ParameterSet&);
  ~PATElectronDump();

 protected:
  void print(const edm::Event&, const edm::EventSetup&) const;

 private:
  edm::InputTag patElectronSource_;
  edm::InputTag genParticleSource_;
  edm::InputTag pfCandidateSrc_;
  edm::InputTag vertexSource_;
  edm::InputTag beamSpotSource_;
  edm::InputTag dcsTag_;

  std::vector<int> skipPdgIdsGenParticleMatch_;
  
  PATElectronPFIsolationExtractor* pfCombIsoExtractor_;
  PATElectronPFIsolationExtractor* pfChargedHadronIsoExtractor_;
  PATElectronPFIsolationExtractor* pfNeutralHadronIsoExtractor_;
  PATElectronPFIsolationExtractor* pfPhotonIsoExtractor_;

};

#endif  


