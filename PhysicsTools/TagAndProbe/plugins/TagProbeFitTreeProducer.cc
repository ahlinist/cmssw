// -*- C++ -*-
//
// Package:    TagProbeFitTreeProducer
// Class:      TagProbeFitTreeProducer
// 
/**\class TagProbeFitTreeProducer TagProbeFitTreeProducer.cc 

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Sep 15 09:45
//         Created:  Mon Sep 15 09:49:08 CEST 2008
// $Id: TagProbeFitTreeProducer.cc,v 1.1 2009/12/15 22:45:12 gpetrucc Exp $
//
//


// system include files
#include <memory>
#include <ctype.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "PhysicsTools/TagAndProbe/interface/TPTreeFiller.h"
#include "PhysicsTools/TagAndProbe/interface/TagProbePairMaker.h"

#include <set>

#include "FWCore/ParameterSet/interface/Registry.h"

//
// class decleration
//

class TagProbeFitTreeProducer : public edm::EDAnalyzer {
   public:
      explicit TagProbeFitTreeProducer(const edm::ParameterSet&);
      ~TagProbeFitTreeProducer();


   private:
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      //---- MC truth information
      /// Is this sample MC?
      bool isMC_; 
      /// InputTag to an edm::Association<reco::GenParticle> from tags & probes to MC truth
      edm::InputTag tagMatches_, probeMatches_;
      /// Possible pdgids for the mother. If empty, any truth-matched mu will be considered good
      std::set<int32_t> motherPdgId_;
      /// Return true if ref is not null and has an ancestor with pdgId inside 'motherPdgId_'
      bool checkMother(const reco::GenParticleRef &ref) const ;

      //---- Unbiased MC truth information
      /// Do we have to compute this
      bool makeMCUnbiasTree_;
      /// Check mother pdgId in unbiased inefficiency measurement
      bool checkMotherInUnbiasEff_;
      /// InputTag to the collection of all probes
      edm::InputTag allProbes_;

      /// The object that produces pairs of tags and probes, making any arbitration needed
      tnp::TagProbePairMaker tagProbePairMaker_;
      /// The object that actually computes variables and fills the tree for T&P
      std::auto_ptr<tnp::TPTreeFiller> treeFiller_; 
      /// The object that actually computes variables and fills the tree for unbiased MC
      std::auto_ptr<tnp::BaseTreeFiller> mcUnbiasFiller_;
};

//
// constructors and destructor
//
TagProbeFitTreeProducer::TagProbeFitTreeProducer(const edm::ParameterSet& iConfig) :
    isMC_(iConfig.getParameter<bool>("isMC")),
    makeMCUnbiasTree_(isMC_ ? iConfig.getParameter<bool>("makeMCUnbiasTree") : false),
    checkMotherInUnbiasEff_(makeMCUnbiasTree_ ? iConfig.getParameter<bool>("checkMotherInUnbiasEff") : false),
    tagProbePairMaker_(iConfig),
    treeFiller_(new tnp::TPTreeFiller(iConfig))
{
    if (isMC_) { 
        // For mc efficiency we need the MC matches for tags & probes
        tagMatches_ = iConfig.getParameter<edm::InputTag>("tagMatches");
        probeMatches_ = iConfig.getParameter<edm::InputTag>("probeMatches");
        //.. and the pdgids of the possible mothers
        if (iConfig.existsAs<int32_t>("motherPdgId")) {
            motherPdgId_.insert(iConfig.getParameter<int32_t>("motherPdgId"));
        } else {
            std::vector<int32_t> motherIds = iConfig.getParameter<std::vector<int32_t> >("motherPdgId");
            motherPdgId_.insert(motherIds.begin(), motherIds.end());
        }

        // For unbiased efficiency we also need the collection of all probes
        if (makeMCUnbiasTree_) {
            allProbes_ = iConfig.getParameter<edm::InputTag>("allProbes");
            mcUnbiasFiller_.reset(new tnp::BaseTreeFiller("mcUnbias_tree",iConfig));
        }
    }
}


TagProbeFitTreeProducer::~TagProbeFitTreeProducer()
{
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
TagProbeFitTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    using namespace edm; using namespace std; 
    Handle<reco::CandidateView> src, allProbes;
    Handle<Association<vector<reco::GenParticle> > > tagMatches, probeMatches;

    treeFiller_->init(iEvent); // read out info from the event if needed (external vars, list of passing probes, ...)

    // on mc we want to load also the MC match info
    if (isMC_) {
        iEvent.getByLabel(tagMatches_, tagMatches);
        iEvent.getByLabel(probeMatches_, probeMatches);
    }

    // get the list of (tag+probe) pairs, performing arbitration 
    tnp::TagProbePairs pairs = tagProbePairMaker_.run(iEvent);
    // loop on them to fill the tree
    for (tnp::TagProbePairs::const_iterator it = pairs.begin(), ed = pairs.end(); it != ed; ++it) {
        // on mc, fill mc info (on non-mc, let it to 'true', the treeFiller will ignore it anyway
        bool mcTrue = false;
        if (isMC_) {
            reco::GenParticleRef mtag = (*tagMatches)[it->tag], mprobe = (*probeMatches)[it->probe];
            mcTrue = checkMother(mtag) && checkMother(mprobe);
        }
        // fill in the variables for this t+p pair
        treeFiller_->fill(it->probe, it->mass, mcTrue);
    } 

    if (isMC_ && makeMCUnbiasTree_) {
        // read full collection of probes
        iEvent.getByLabel(allProbes_, allProbes);
        // init the tree filler
        mcUnbiasFiller_->init(iEvent);
        // loop on probes
        for (size_t i = 0, n = allProbes->size(); i < n; ++i) {
            const reco::CandidateBaseRef & probe = allProbes->refAt(i);
            // check mc match, and possibly mother match
            reco::GenParticleRef probeMatch = (*probeMatches)[probe];
            bool probeOk = checkMotherInUnbiasEff_ ? checkMother(probeMatch) : probeMatch.isNonnull();
            // fill the tree only for good ones
            if (probeOk) mcUnbiasFiller_->fill(probe);
        }
    }
   
}

bool
TagProbeFitTreeProducer::checkMother(const reco::GenParticleRef &ref) const {
    if (motherPdgId_.empty()) return true;
    if (ref.isNull()) return false;
    if (motherPdgId_.find(abs(ref->pdgId())) != motherPdgId_.end()) return true;
    reco::GenParticle::mothers m = ref->motherRefVector();
    for (reco::GenParticle::mothers::const_iterator it = m.begin(), e = m.end(); it != e; ++it) {
        if (checkMother(*it)) return true;
    }
    return false;
}


// ------------ method called once each job just after ending the event loop  ------------
void 
TagProbeFitTreeProducer::endJob() {
    // ask to write the current PSet info into the TTree header
    treeFiller_->writeProvenance(edm::getProcessParameterSet());
}

//define this as a plug-in
DEFINE_FWK_MODULE(TagProbeFitTreeProducer);
