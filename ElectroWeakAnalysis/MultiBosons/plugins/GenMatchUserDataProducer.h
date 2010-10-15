#ifndef ElectroWeakAnalysis_MultiBosons_plugins_GenMatchUserDataProducer_h
#define ElectroWeakAnalysis_MultiBosons_plugins_GenMatchUserDataProducer_h
// -*- C++ -*-
//
// Package:    PatAlgos
// Class:      GenMatchUserDataProducer
//
/**\class GenMatchUserDataProducer GenMatchUserDataProducer.h ElectroWeakAnalysis/MultiBosons/plugins/GenMatchUserDataProducer.h

Description: Produce gen match info for UserData in PAT

Implementation:

*/


// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/OwnVector.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


namespace vgamma {
  //
  // class decleration
  //
  // CandType is meant to be some candidate for which the MC matching was done
  template <typename CandType>
  class GenMatchUserDataProducer : public edm::EDProducer {
  public:
    explicit GenMatchUserDataProducer(const edm::ParameterSet&);
    ~GenMatchUserDataProducer();


  private:
    virtual void produce(edm::Event&, const edm::EventSetup&);

    // ----------member data ---------------------------
    template <typename UserDataType>
    void putMap(edm::Event & iEvent,
                edm::Handle<edm::View<CandType> >& candidates,
                std::vector<UserDataType>& userData,
                const std::string& name);

    edm::InputTag src_;
    edm::InputTag match_;
  };

  //
  // constructors and destructor
  //
  template <typename CandType>
  GenMatchUserDataProducer<CandType>::GenMatchUserDataProducer (
    const edm::ParameterSet& iConfig
    ):
    src_(iConfig.getParameter<edm::InputTag>("src")),
    match_(iConfig.getParameter<edm::InputTag>("match"))
  {
    produces<edm::ValueMap<int  > > ("motherPdgId");
    produces<edm::ValueMap<int  > > ("motherStatus");
    produces<edm::ValueMap<int  > > ("grandMotherPdgId");
    produces<edm::ValueMap<int  > > ("grandMotherStatus");
  }

  template <typename CandType>
  GenMatchUserDataProducer<CandType>::~GenMatchUserDataProducer()
  {
  }

  // ------------ method called to for each event  ------------
  template <typename CandType>
  void
  GenMatchUserDataProducer<CandType>::produce (
    edm::Event& iEvent, const edm::EventSetup& iSetup
    )
  {
    using namespace std;
    using namespace edm;

    Handle<View<CandType> > candidates;
    Handle<reco::GenParticleMatch> match;

    iEvent.getByLabel(src_, candidates);
    iEvent.getByLabel(match_, match);

    vector<int  > motherPdgId;
    vector<int  > motherStatus;
    vector<int  > grandMotherPdgId;
    vector<int  > grandMotherStatus;

    // loop over source candidates
    for(size_t i = 0; i < candidates->size(); ++i) {
      // default values meaning no match found
      int   myMotherPdgId = 0;
      int   myMotherStatus = 0;
      int   myGrandMotherPdgId = 0;
      int   myGrandMotherStatus = 0;

      Ref<View<CandType> > candRef(candidates, i);
      reco::GenParticleRef matchRef = (*match)[candRef];

      if ( matchRef.isNonnull() ) {
        if (matchRef->numberOfMothers() > 0) {
          const reco::Candidate *mom = matchRef->mother(0);
          myMotherPdgId  = mom->pdgId();
          myMotherStatus = mom->status();
          if (mom->numberOfMothers() > 0) {
            const reco::Candidate *gmom = mom->mother(0);
            myGrandMotherPdgId  = gmom->pdgId();
            myGrandMotherStatus = gmom->status();
          } // if (mom->numberOfMothers() > 0)
        } // if (matchRef->numberOfMothers() > 0)
      }// if ( matchRef.isNonnull() )

      motherPdgId      .push_back(myMotherPdgId);
      motherStatus     .push_back(myMotherStatus);
      grandMotherPdgId .push_back(myGrandMotherPdgId);
      grandMotherStatus.push_back(myGrandMotherStatus);

    } // loop over source candidates

    putMap<int  >(iEvent, candidates, motherPdgId, "motherPdgId");
    putMap<int  >(iEvent, candidates, motherStatus, "motherStatus");
    putMap<int  >(iEvent, candidates, grandMotherPdgId, "grandMotherPdgId");
    putMap<int  >(iEvent, candidates, grandMotherStatus, "grandMotherStatus");

  } // GenMatchUserDataProducer<CandType>::produce

  template <typename CandType> template <typename UserDataType>
  void
  GenMatchUserDataProducer<CandType>::putMap (
    edm::Event & iEvent,
    edm::Handle<edm::View<CandType> >& candidates,
    std::vector<UserDataType>& userData,
    const std::string& name
    )
  {
    using namespace std;
    using namespace edm;

    auto_ptr<ValueMap<UserDataType> > prod( new ValueMap<UserDataType>() );
    typename ValueMap<UserDataType>::Filler filler (*prod);
    filler.insert(candidates, userData.begin(), userData.end());
    filler.fill();
    iEvent.put(prod, name);
  } // GenMatchUserDataProducer<CandType>::putMap

} // namespace::vgamma

#endif
