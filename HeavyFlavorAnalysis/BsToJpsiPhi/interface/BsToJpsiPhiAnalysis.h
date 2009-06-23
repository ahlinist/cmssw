#ifndef HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiAnalysis_h
#define HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiAnalysis_h

// -*- C++ -*-
//
// Package:    BsToJpsiPhi
// Class:      BsToJpsiPhiAnalysis
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertexContainer.h"
// #include "RecoVertex/KalmanVertexFit/test/SimpleVertexTree.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByChi2.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicVertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicTree.h"
#include "HeavyFlavorAnalysis/BsToJpsiPhi/interface/BsToJpsiPhiRootTree.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
// #include "RecoVertex/KinematicFitPrimitives/interface/"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"
#include <TFile.h>
#include <TH1F.h>


class BsToJpsiPhiAnalysis : public edm::EDAnalyzer {
public:
	explicit BsToJpsiPhiAnalysis(const edm::ParameterSet&);
	~BsToJpsiPhiAnalysis();
	
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	
	virtual void beginJob(edm::EventSetup const&);
	virtual void endJob();

private:

	const TrackingVertex * getSimVertex(const TrackingVertexCollection * tVC) const;
	int JpsiKstarFlag(const edm::Event &iEvent);
	int JpsiKsFlag(const edm::Event &iEvent);
	int JpsiPhiFlag(const edm::Event &iEvent);
	double deltaRRecoSim(const reco::TransientTrack *ttk, const TrackingParticleRef *tpr);
	
	BsToJpsiPhiRootTree * bsRootTree_;
	
	edm::ParameterSet theConfig_;
	edm::ParameterSet kvfPSet_;
	TrackAssociatorByHits * associatorByHits_;

	std::string outputFile_; // output file
	edm::InputTag thegenParticlesLabel_;
	edm::InputTag bCandLabel_; // label of candidate producer
	edm::InputTag trackLabel_; // label of track producer
	edm::InputTag vertexLabel_; //label of vertex producer
	bool isSim_; // is it simulated data
	double deltaR_; //for matching sim and reco tracks
	
	int event_counter_;
	int flag_1;
	int flag_2;
	int flag_3;
	int flag_4;
	int flag_5;
	
	int flagKstar;
	int flagKs;
	int flagPhi;
	
};
#endif
