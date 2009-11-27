#ifndef TauAnalysis_BgEstimationTools_PATDiTauLeptonIsolationExtractor_h  
#define TauAnalysis_BgEstimationTools_PATDiTauLeptonIsolationExtractor_h

/** \class PATDiTauLeptonIsolationExtractor
 *
 * Auxiliary class for extracting electron, muon or tau-jet isolation value from IsoDeposits
 * (used for Ntuple filling)
 *
 * NOTE: the values are extracted from the first leg of the diTau object
 *       specified by the "index" configuration parameter (**first** PATMuTauPair object in case "index" is not specified)
 *       contained in the collection specified by the "src" configuration parameter;
 *       in case the collection of PAT objects is empty, 
 *       a substitute value of 1.e+3 is returned by operator();
 *       the same substitute value is returned in case no IsoDeposit of type specified in configuration parameters is found
 * 
 *
 * Example for usage:
 *
 *     muonExtTrackIsoZmumu = cms.PSet(
 *         pluginType = cms.string("PATMuTauPairIsolationExtractor"),
 *         src = cms.InputTag('muTauPairsForBgEstZmumuEnriched'),
 *         sum = cms.vstring("TrackIso"), # key = either "sum" or "count";
 *                                        # value = list types of IsoDeposits defined in DataFormats/PatCandidates/interface/Isolation.h
 *         dR = cms.double(1.0),
 *         vetos = cms.vstring(""),
 *         scale = cms.string("1."),      # use "1." for absolute isolation/"1./pt" for relative isolation
 *         leg = cms.uint32(1),           # compute muon isolation
 *         indices = cms.vuint32(0,1)     #  for (up to) "first" and "second" muon + tau-jet combination found in each event
 *     ),
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATDiTauLeptonIsolationExtractor.h,v 1.1 2009/11/18 14:33:04 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/IsolationAlgos/interface/IsoDepositVetoFactory.h"
#include "DataFormats/PatCandidates/interface/Isolation.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "AnalysisDataFormats/TauAnalysis/interface/CompositePtrCandidateT1T2MEt.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <string>
#include <vector>

template <typename T1, typename T2>
class PATDiTauLeptonIsolationExtractor : public ObjValExtractorBase
{
 public:
  
  explicit PATDiTauLeptonIsolationExtractor(const edm::ParameterSet&);
  ~PATDiTauLeptonIsolationExtractor();
  
  double operator()(const edm::Event&) const;

 private:

//--- configuration parameters
  edm::InputTag src_;
  typedef std::vector<pat::IsolationKeys> vIsoKeys;
  vIsoKeys isoTypes_;
  double dR_;
  std::vector<reco::isodeposit::AbsVeto*> vetos_;
  enum { kSum, kCount };
  int mode_;
  typedef CompositePtrCandidateT1T2MEt<T1,T2> diTauType;
  StringObjectFunction<diTauType>* scale_;
  unsigned leg_;

  int error_;
  
  unsigned index_;
};

#endif  


