#ifndef TauAnalysis_BgEstimationTools_FakeRateWeightProducerBase_h  
#define TauAnalysis_BgEstimationTools_FakeRateWeightProducerBase_h

/** \class FakeRateWeightProducerBase
 *
 * Base-class for producing event weights utilized for data-driven background estimation
 * via fake-rate technique
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.10 $
 *
 * $Id: FakeRateWeightProducerBase.h,v 1.10 2010/12/14 15:47:40 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h" 

#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/PFTau.h"

#include <string>
#include <vector>

class FakeRateWeightProducerBase : public edm::EDProducer
{
 public:
  
  explicit FakeRateWeightProducerBase(const edm::ParameterSet&);
  ~FakeRateWeightProducerBase();
  
 protected:

  struct tauJetDiscrEntry
  {
    tauJetDiscrEntry(const edm::ParameterSet&);
    ~tauJetDiscrEntry();

    edm::InputTag tauJetIdEffSource_;
    edm::InputTag qcdJetFakeRateSource_;

    edm::InputTag tauJetDiscrSource_;
    double tauJetDiscrThreshold_;

    std::string tauJetCut_;
    StringCutObjectSelector<reco::CaloTau>* caloTauSelector_;
    StringCutObjectSelector<reco::PFTau>* pfTauSelector_;

    int cfgError_;
  };

  struct fakeRateTypeEntry
  {
    fakeRateTypeEntry(const edm::ParameterSet&);
    ~fakeRateTypeEntry();

    std::vector<tauJetDiscrEntry> tauJetDiscriminators_;

    int cfgError_;
  };

  void getTauJetProperties(const edm::Event&, edm::RefToBase<reco::BaseTau>&, unsigned, 
			   const edm::Handle<edm::View<reco::Candidate> >&, const fakeRateTypeEntry&, double&, double&, bool&);
  double getFakeRateJetWeight(double, double, bool, const reco::BaseTau*);

//--- configuration parameters
  std::string moduleLabel_;

  std::string method_; // "simple"/"CDF"

  edm::InputTag allTauJetSource_;
  edm::InputTag preselTauJetSource_;

  double dRmatch_;

  typedef std::map<std::string, fakeRateTypeEntry> fakeRateTypeMap;
  fakeRateTypeMap fakeRateTypes_;

  double minJetWeight_;
  double maxJetWeight_;

  double minJetPt_;
  double maxJetPt_;
  double minJetEta_;
  double maxJetEta_;

  double shiftTauIdEff_; // multiply tau id. efficiency by (1 + shift),
                         // in order to estimate systematic uncertainties due to imprecise knowledge of tau id. efficiency
                         // if using "CDF"-type weights 

  int cfgError_;

  long numJets_weightBelowMinimum_;
  long numJets_weightAboveMaximum_;
  long numJets_reverseWeightOrder_;
  long numJets_outOfRange_;
  long numJets_processed_;

  static int gVerbosity_;
};

double getJetRadius(const reco::BaseTau& tauJet);

#endif  


