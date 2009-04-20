//
// $Id: PATGenericParticleProducer.h,v 1.7 2009/03/26 20:44:37 vadler Exp $
//

#ifndef PhysicsTools_PatAlgos_PATGenericParticleProducer_h
#define PhysicsTools_PatAlgos_PATGenericParticleProducer_h

/**
  \class    pat::PATGenericParticleProducer PATGenericParticleProducer.h "PhysicsTools/PatAlgos/interface/PATGenericParticleProducer.h"
  \brief    Produces the pat::GenericParticle

   The PATGenericParticleProducer produces the analysis-level pat::GenericParticle starting from
   any collection of Candidates

  \author   Giovanni Petrucciani
  \version  $Id: PATGenericParticleProducer.h,v 1.7 2009/03/26 20:44:37 vadler Exp $
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Common/interface/ValueMap.h"

#include "CommonTools/Utils/interface/EtComparator.h"

#include "DataFormats/PatCandidates/interface/GenericParticle.h"

#include "PhysicsTools/PatAlgos/interface/MultiIsolator.h"
#include "PhysicsTools/PatAlgos/interface/EfficiencyLoader.h"
#include "PhysicsTools/PatAlgos/interface/VertexingHelper.h"

namespace pat {

  class PATGenericParticleProducer : public edm::EDProducer {

    public:

      explicit PATGenericParticleProducer(const edm::ParameterSet & iConfig);
      ~PATGenericParticleProducer();

      virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

    private:

      // configurables
      edm::InputTag src_;

      // embed RECo objects
      bool embedSuperCluster_, embedTrack_, embedTracks_, embedGsfTrack_, embedCaloTower_, embedStandalone_, embedCombined_;

      bool addQuality_;
      edm::InputTag qualitySrc_;

      bool addGenMatch_;
      bool embedGenMatch_;
      std::vector<edm::InputTag> genMatchSrc_;

      // tools
      GreaterByEt<GenericParticle> eTComparator_;

      pat::helper::MultiIsolator isolator_; 
      pat::helper::MultiIsolator::IsolationValuePairs isolatorTmpStorage_; // better here than recreate at each event
      std::vector<std::pair<pat::IsolationKeys,edm::InputTag> > isoDepositLabels_;

      bool addEfficiencies_;
      pat::helper::EfficiencyLoader efficiencyLoader_;

      pat::helper::VertexingHelper vertexingHelper_;

  };


}

#endif
