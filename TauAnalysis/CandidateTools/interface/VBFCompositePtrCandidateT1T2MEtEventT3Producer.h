#ifndef TauAnalysis_CandidateTools_VBFCompositePtrCandidateT1T2MEtEventT3Producer_h
#define TauAnalysis_CandidateTools_VBFCompositePtrCandidateT1T2MEtEventT3Producer_h

/** \class VBFCompositePtrCandidateT1T2MEtEventT3Producer
 *
 * \authors Colin Bernet,
 *          Artur Kalinowski,
 *          Christian Veelken
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Ptr.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3.h"
#include "AnalysisDataFormats/TauAnalysis/interface/VBFCompositePtrCandidateT1T2MEtEventT3Fwd.h"

template<typename T1, typename T2, typename T3>
class VBFCompositePtrCandidateT1T2MEtEventT3Producer : public edm::EDProducer 
{

  typedef VBFCompositePtrCandidateT1T2MEtEventT3<T1,T2,T3> VBFEventType;
  typedef std::vector<VBFEventType> VBFEventCollection;
  typedef CompositePtrCandidateT1T2MEt<T1,T2> diTauType;
  typedef edm::View<diTauType> diTauView;
  typedef edm::Ptr<diTauType> diTauPtr;
  typedef edm::View<T3> jetView;
  typedef edm::Ptr<T3> jetPtr;

 public:

  explicit VBFCompositePtrCandidateT1T2MEtEventT3Producer(const edm::ParameterSet& cfg)
  {
    srcTagJets_          = cfg.getParameter<edm::InputTag>("srcTagJets");
    srcCentralJets_      = cfg.getParameter<edm::InputTag>("srcCenralJets");
    srcDiTaus_           = cfg.getParameter<edm::InputTag>("srcDiTaus");

    dEtaMinTagJet_       = cfg.getParameter<double>("dEtaMinTagJet");
    massMinTagJet_       = cfg.getParameter<double>("massMinTagJet");
    dRmin12TagJet_       = cfg.getParameter<double>("dRmin12TagJet");

    etaMarginCentralJet_ = cfg.getParameter<double>("etaMarginCentralJet");
    dRmin12CentralJet_   = cfg.getParameter<double>("dRmin12CentralJet");

    produces<VBFEventCollection>("");
  }
  ~VBFCompositePtrCandidateT1T2MEtEventT3Producer() {}
  
 private:

  void produce(edm::Event& evt, const edm::EventSetup& es)
  {
    edm::Handle<jetView> tagJetCollection;
    evt.getByLabel(srcTagJets_, tagJetCollection);

    edm::Handle<jetView> centralJetCollection;
    evt.getByLabel(srcCentralJets_, centralJetCollection);

    edm::Handle<diTauView> diTauCollection;
    evt.getByLabel(srcDiTaus_, diTauCollection);

    std::auto_ptr<VBFEventCollection> vbfEventCollection(new VBFEventCollection());

    for ( unsigned idxDiTau = 0, numDiTaus = diTauCollection->size(); 
	  idxDiTau < numDiTaus; ++idxDiTau ) {
      diTauPtr diTau = diTauCollection->ptrAt(idxDiTau);
      
      for ( unsigned idxTagJet1 = 0, numTagJets1 = tagJetCollection->size(); 
	    idxTagJet1 < numTagJets1; ++idxTagJet1 ) {
	jetPtr tagJet1 = tagJetCollection->ptrAt(idxTagJet1);
	
	if ( deltaR(tagJet1->p4(), diTau->leg1()->p4()) < dRmin12TagJet_ ||
	     deltaR(tagJet1->p4(), diTau->leg2()->p4()) < dRmin12TagJet_ ) continue;

	for ( unsigned idxTagJet2 = idxTagJet1 + 1, numTagJets2 = tagJetCollection->size(); 
	      idxTagJet2 < numTagJets2; ++idxTagJet2 ) {
	  jetPtr tagJet2 = tagJetCollection->ptrAt(idxTagJet2);

	  if ( deltaR(tagJet2->p4(), diTau->leg1()->p4()) < dRmin12TagJet_ ||
	       deltaR(tagJet2->p4(), diTau->leg2()->p4()) < dRmin12TagJet_ ) continue;
	  
	  double tagJetEtaMin   = TMath::Min(tagJet1->eta(), tagJet2->eta());
	  double tagJetEtaMax   = TMath::Max(tagJet1->eta(), tagJet2->eta());
	  double tagJetDeltaEta = tagJetEtaMax - tagJetEtaMin;
	  double tagJetMass     = (tagJet1->p4() + tagJet2->p4()).mass();

	  if ( tagJetDeltaEta > dEtaMinTagJet_ && tagJetMass > massMinTagJet_ ) {
	    std::vector<jetPtr> centralJets;
	    
	    for ( unsigned idxCentralJet = 0, numCentralJets = centralJetCollection->size(); 
		  idxCentralJet < numCentralJets; ++idxCentralJet ) {
	      jetPtr centralJet = centralJetCollection->ptrAt(idxCentralJet);
	      
	      if ( centralJet->eta() < (tagJetEtaMax - etaMarginCentralJet_) &&
		   centralJet->eta() > (tagJetEtaMin + etaMarginCentralJet_) ) {
		bool isDiTauOverlap = false;
		for ( typename diTauView::const_iterator diTau = diTauCollection->begin();
		      diTau != diTauCollection->end(); ++diTau ) {
		  if ( deltaR(centralJet->p4(), diTau->leg1()->p4()) < dRmin12CentralJet_ ||
		       deltaR(centralJet->p4(), diTau->leg2()->p4()) < dRmin12CentralJet_ ) isDiTauOverlap = true;
		}
		
		if ( !isDiTauOverlap ) centralJets.push_back(centralJet);
	      }

//--- sort central jets in order of decreasing Pt
//   (use -Pt as key in multimap to get descending ordering)
	      std::multimap<double, unsigned> centralJetIndices;
	      for ( typename std::vector<jetPtr>::const_iterator centralJet = centralJets.begin();
		    centralJet != centralJets.end(); ++centralJet ) {
		centralJetIndices.insert(std::pair<double, unsigned>(-(*centralJet)->pt(), centralJet->key()));
	      }
	      std::vector<jetPtr> centralJets_sorted;
	      for ( std::multimap<double, unsigned>::const_iterator centralJetIndex = centralJetIndices.begin();
		    centralJetIndex != centralJetIndices.end(); ++centralJetIndex ) {
		centralJets_sorted.push_back(centralJetCollection->ptrAt(centralJetIndex->second));
	      }

	      VBFEventType vbfEvent(tagJet1, tagJet2, diTau, centralJets_sorted);
	      vbfEventCollection->push_back(vbfEvent);
	    }
	  }
	}
      }
    }

    evt.put(vbfEventCollection);
  }

  edm::InputTag srcTagJets_;
  edm::InputTag srcCentralJets_;
  edm::InputTag srcDiTaus_;

  double dEtaMinTagJet_;
  double massMinTagJet_;
  double dRmin12TagJet_;

  double etaMarginCentralJet_; // minimum distance in eta of central jet to tag jets
  double dRmin12CentralJet_;   // minimum distance in dR  of central jet to leg1, leg2 of diTau
};

#endif

