#ifndef TauAnalysis_RecoTools_ParticlePFIsolationSelector_h
#define TauAnalysis_RecoTools_ParticlePFIsolationSelector_h

/** \class ParticlePFIsolationSelector
 *
 * Flexible selection of pat::Leptons based on charged hadrons,
 * neutral hadrons and photons/pi0s reconstructed by particle-flow algorithm;
 * a pat::Electron, pat::Muon or pat::Tau passed the selection
 * if the sum of deposits of the speficied type and with Pt > ptMin
 * in an annulus within dRvetoCone (inner cone) and dRisoCone (outer cone)
 * is more than sumPtMin and less than sumPtMax
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: ParticlePFIsolationSelector.h,v 1.6 2011/02/10 13:57:04 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "TauAnalysis/RecoTools/interface/ParticlePFIsolationExtractor.h"

#include <TMath.h>

#include <vector>
#include <string>
#include <algorithm>

template <class T>
class ParticlePFIsolationSelector
{
	public:
		typedef std::vector<T> collection;

		explicit ParticlePFIsolationSelector(const edm::ParameterSet& cfg)
			: extractor_(cfg),
			sumPtMethod_(kAbsoluteIso),
			cfgError_(0)
	{
		pfCandidateSrc_ = cfg.getParameter<edm::InputTag>("pfCandidateSource");

		sumPtMin_ = cfg.exists("sumPtMin") ?
			cfg.getParameter<double>("sumPtMin") : -1.;

		sumPtMaxEB_ = -1;
		sumPtMaxEE_ = -1;
		if (cfg.exists("sumPtMax")) {
			sumPtMaxEB_ = cfg.getParameter<double>("sumPtMax");
			sumPtMaxEE_ = cfg.getParameter<double>("sumPtMax");
		}
		else if( cfg.exists("sumPtMaxEB") && cfg.exists("sumPtMaxEE") ) {
			sumPtMaxEB_ = cfg.getParameter<double>("sumPtMaxEB");
			sumPtMaxEE_ = cfg.getParameter<double>("sumPtMaxEE");
		} else {
			edm::LogError("ParticlePFIsolationSelector")
				<< " Configuration must specify ( sumPtMaxEB AND sumPtMaxEE ) OR sumPtMax ";
			cfgError_ = 1;
		}

		if ( cfg.exists("sumPtMethod") ) {
			std::string sumPtMethod_string = cfg.getParameter<std::string>("sumPtMethod");
			if ( sumPtMethod_string == "absolute" ) {
				sumPtMethod_ = kAbsoluteIso;
			} else if ( sumPtMethod_string == "relative" ) {
				sumPtMethod_ = kRelativeIso;
			} else {
				edm::LogError("ParticlePFIsolationSelector")
					<< " Configuration parameter 'sumPtMethod' = " << sumPtMethod_string << " invalid !!";
				cfgError_ = 1;
			}
		}
	}
		~ParticlePFIsolationSelector() {}

		typename std::vector<const T*>::const_iterator begin() const { return selected_.begin(); }
		typename std::vector<const T*>::const_iterator end() const { return selected_.end(); }

		void select(const edm::Handle<collection>& isoParticleCandidates, const edm::Event& evt, const edm::EventSetup& es)
		{
			selected_.clear();

			if ( cfgError_ ) {
				edm::LogError ("select")
					<< " Error in Configuration ParameterSet --> no iso. particle candidate will pass Selection !!";
				return;
			}

			edm::Handle<reco::PFCandidateCollection> pfCandidates;
			evt.getByLabel(pfCandidateSrc_, pfCandidates);

			for ( typename collection::const_iterator isoParticleCandidate = isoParticleCandidates->begin();
					isoParticleCandidate != isoParticleCandidates->end(); ++isoParticleCandidate ) {
				double sumPt = extractor_(*isoParticleCandidate, *pfCandidates);

				// need to fix for correct eta
				if( isoParticleCandidate->eta() < 1.479) {
					if ( sumPtMethod_ == kAbsoluteIso ) {
						if ( sumPtMin_ > 0. && sumPt < sumPtMin_ ) continue;
						if ( sumPtMaxEB_ > 0. && sumPt > sumPtMaxEB_ ) continue;
					} else if ( sumPtMethod_ == kRelativeIso ) {
						double relIso = ( isoParticleCandidate->pt() > 1. ) ? (sumPt/isoParticleCandidate->pt()) : sumPt;
						if ( sumPtMin_ > 0. && relIso < sumPtMin_ ) continue;
						if ( sumPtMaxEB_ > 0. && relIso > sumPtMaxEB_ ) continue;
					}
				} else {
					if ( sumPtMethod_ == kAbsoluteIso ) {
						if ( sumPtMin_ > 0. && sumPt < sumPtMin_ ) continue;
						if ( sumPtMaxEE_ > 0. && sumPt > sumPtMaxEE_ ) continue;
					} else if ( sumPtMethod_ == kRelativeIso ) {
						double relIso = ( isoParticleCandidate->pt() > 1. ) ? (sumPt/isoParticleCandidate->pt()) : sumPt;
						if ( sumPtMin_ > 0. && relIso < sumPtMin_ ) continue;
						if ( sumPtMaxEE_ > 0. && relIso > sumPtMaxEE_ ) continue;
					}
				}

				selected_.push_back(&(*isoParticleCandidate));
			}
		}

		size_t size() const { return selected_.size(); }

	private:
		std::vector<const T*> selected_;

		edm::InputTag pfCandidateSrc_;

		ParticlePFIsolationExtractor<T> extractor_;

		double sumPtMin_;
		double sumPtMaxEB_;
		double sumPtMaxEE_;

		enum { kAbsoluteIso, kRelativeIso };
		int sumPtMethod_;

		int cfgError_;
};

#endif


