#ifndef TauAnalysis_Core_eventDumpAuxFunctions_h
#define TauAnalysis_Core_eventDumpAuxFunctions_h

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TauAnalysis/Core/interface/genericAnalyzerAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"

#include <vector>
#include <string>

#include <iostream>

std::ostream* getOutputOptions(const edm::ParameterSet&, bool&, int&);

void printEventSelectionInfo(const GenericAnalyzer_namespace::filterResults_type&, 
                             const GenericAnalyzer_namespace::filterResults_type&, std::ostream*);

void printGenParticleInfo(const reco::GenParticleCollection&, const reco::GenJetCollection&, std::ostream*);

template<typename T>
void printTrackInfo(const edm::Ref<T>& track, const reco::Candidate::Point& vertex, bool printDxy, bool printDz, std::ostream* stream)
{
  if ( isValidRef(track) ) {
    *stream << "  Pt = " << track->pt() << std::endl;
    *stream << "  charge = " << track->charge() << std::endl;
    if ( printDxy ) *stream << "  dXY = " << track->dxy(vertex) << std::endl;
    if ( printDz  ) *stream << "  dZ = " << track->dz(vertex) << std::endl;
  } else {
    *stream << "  none." << std::endl;
  }
}

void printVertexInfo(const reco::Candidate::Point&, std::ostream*);

void printTrackIsolationInfo(const edm::Handle<reco::TrackCollection>&, const reco::Candidate::Vector&, double, double, double, const reco::Vertex::Point&, std::ostream*);
void printPFCandidateIsolationInfo(const edm::Handle<reco::PFCandidateCollection>&, std::string,
				   const reco::Candidate::Vector&, double, double, double, std::ostream*);

#endif
