#ifndef HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiAnalysis_h
#define HeavyFlavorAnalysis_BsToJpsiPhi_BsToJpsiPhiAnalysis_h

// -*- C++ -*-
//
// Package:    BsToJpsiPhi
// Class:      BsToJpsiPhiAnalysis
// 
/**\class BsToJpsiPhiAnalysis BsToJpsiPhiAnalysis.cc HeavyFlavorAnalysis/BsToJpsiPhi/src/BsToJpsiPhiAnalysis.cc
*
* Analysis of th eBs to Jpsi phi decay
* Lotte Wilke October 2007
* 
*/
//
// Original Author:  Lotte Wilke
//         Created:  18.10.2007
//
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

// 	void fillhistograms(const RefCountedKinematicTree& myTree) const;
	const TrackingVertex * getSimVertex(const TrackingVertexCollection * tVC) const;
	double deltaRRecoSim(const reco::TransientTrack *ttk, const TrackingParticleRef *tpr);
	
	BsToJpsiPhiRootTree * bsRootTree_;
	
	edm::ParameterSet theConfig_;
	edm::ParameterSet kvfPSet_;
	TrackAssociatorByHits * associatorByHits_;

	std::string outputFile_; // output file
	std::string bCandLabel_; // label of candidate producer
	std::string trackLabel_; // label of track producer
	std::string vertexLabel_; //label of vertex producer
	bool isSim_; // is it simulated data
	double deltaR_; //for matching sim and reco tracks
	
// 	TFile* hFile_;
// 	TH1F * h_Bs_mass;
// 	TH1F * h_l_xy;
// 	TH1F * h_l;
// 	TH1F * h_t_xy;
// 	TH1F * h_t;
// 	TH1F * h_pT_muon;
// 	TH1F * h_pT_Kaon;
// 	TH1F * h_pT_Jpsi;
// 	TH1F * h_pT_phi;
// 	TH1F * h_pT_Bs;
// 	TH1F * h_fit_prob;
	
	int event_counter_;
};
#endif
