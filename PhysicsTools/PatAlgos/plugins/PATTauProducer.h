//
// $Id: PATTauProducer.h,v 1.11 2008/09/19 21:13:17 cbern Exp $
//

#ifndef PhysicsTools_PatAlgos_PATTauProducer_h
#define PhysicsTools_PatAlgos_PATTauProducer_h

/**
  \class    pat::PATTauProducer PATTauProducer.h "PhysicsTools/PatAlgos/interface/PATTauProducer.h"
  \brief    Produces pat::Tau's

   The PATTauProducer produces analysis-level pat::Tau's starting from
   a collection of objects of TauType.

  \author   Steven Lowette, Christophe Delaere
  \version  $Id: PATTauProducer.h,v 1.11 2008/09/19 21:13:17 cbern Exp $
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "PhysicsTools/Utilities/interface/PtComparator.h"

#include "PhysicsTools/PatAlgos/interface/MultiIsolator.h"
#include "PhysicsTools/PatAlgos/interface/EfficiencyLoader.h"

#include "DataFormats/PatCandidates/interface/Tau.h"

#include "DataFormats/PatCandidates/interface/UserData.h"
#include "PhysicsTools/PatAlgos/interface/PATUserDataMerger.h"

#include "DataFormats/PatCandidates/interface/UserData.h"
#include "PhysicsTools/PatAlgos/interface/PATUserDataHelper.h"

#include <string>


namespace pat {


  class ObjectResolutionCalc;

  class PATTauProducer : public edm::EDProducer {

    public:

      explicit PATTauProducer(const edm::ParameterSet & iConfig);
      ~PATTauProducer();

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:

      // configurables
      edm::InputTag tauSrc_;
      bool embedIsolationTracks_;
      bool embedLeadTrack_;
      bool embedSignalTracks_;

      bool          addGenMatch_;
      bool          embedGenMatch_;
      std::vector<edm::InputTag> genMatchSrc_;

      bool          addGenJetMatch_;
      bool          embedGenJetMatch_;
      edm::InputTag genJetMatchSrc_;

      bool          addTrigMatch_;
      std::vector<edm::InputTag> trigMatchSrc_;
      bool          addResolutions_;
      bool          useNNReso_;
      std::string   tauResoFile_;
      // tools
      ObjectResolutionCalc * theResoCalc_;
      GreaterByPt<Tau>       pTTauComparator_;

      pat::helper::MultiIsolator isolator_; 
      pat::helper::MultiIsolator::IsolationValuePairs isolatorTmpStorage_; // better here than recreate at each event
      std::vector<std::pair<pat::IsolationKeys,edm::InputTag> > isoDepositLabels_;

      bool addEfficiencies_;
      pat::helper::EfficiencyLoader efficiencyLoader_;


      bool useUserData_;
      pat::PATUserDataHelper<pat::Tau>      userDataHelper_;

  };
}

#endif
