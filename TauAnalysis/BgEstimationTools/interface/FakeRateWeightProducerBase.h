#ifndef TauAnalysis_BgEstimationTools_FakeRateWeightProducerBase_h  
#define TauAnalysis_BgEstimationTools_FakeRateWeightProducerBase_h

/** \class FakeRateWeightProducerBase
 *
 * Base-class for producing event weights utilized for data-driven background estimation
 * via fake-rate technique
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: FakeRateWeightProducerBase.h,v 1.3 2009/10/26 16:54:44 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//#include "PhysicsTools/Utilities/interface/StringCutObjectSelector.h" // for CMSSW_2_2_x
#include "CommonTools/Utils/interface/StringCutObjectSelector.h"    // for CMSSW_3_1_x

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

  void getTauJetProperties(const edm::Event&, edm::RefToBase<reco::BaseTau>&, unsigned, 
			   const edm::Handle<edm::View<reco::Candidate> >&, double&, double&, bool&);

//--- configuration parameters
  std::string method_; // "simple"/"CDF"

  edm::InputTag allTauJetSource_;
  edm::InputTag preselTauJetSource_;

  double dRmatch_;

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

  std::vector<tauJetDiscrEntry> tauJetDiscriminators_;

  int cfgError_;

};

#endif  


