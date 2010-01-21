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
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
// #include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h" 

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDetType.h"
#include "Geometry/CommonTopologies/interface/StripTopology.h"
#include <TFile.h>
#include <TH1F.h>



class BsToJpsiPhiAnalysis : public edm::EDAnalyzer {
public:
	explicit BsToJpsiPhiAnalysis(const edm::ParameterSet&);
	~BsToJpsiPhiAnalysis();
	
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
	virtual void beginJob(edm::EventSetup const&);
	virtual void endJob();

	void fillMCInfo( edm::Handle<reco::GenParticleCollection> & genParticles);

	void setFitParKK(RefCountedKinematicTree& myTree);
	void setFitParHyp1(RefCountedKinematicTree& myTree);
	void setFitParHyp2(RefCountedKinematicTree& myTree);

private:
	bool MCmatching(const reco::Candidate & track1,  edm::Handle<reco::GenParticleCollection> & genParticles,
			int &K1mcId, int &K1momId, int &K1gmomId,
			int condMom, int condGMom);

	reco::Vertex reVertex(const edm::Handle<reco::VertexCollection> &, const edm::Event &,const edm::EventSetup&, reco::TrackRef, 
			      reco::TrackRef, reco::TrackRef, reco::TrackRef);
	
	BsToJpsiPhiRootTree * bsRootTree_;
	
	edm::ParameterSet theConfig_;

	const TrackerGeometry* m_tracker;

	edm::InputTag thegenParticlesLabel_;
	edm::InputTag trackLabelK_;
	edm::InputTag trackLabelPi_;
	edm::InputTag triggerTag_; 
	edm::InputTag muonTag_; 
	bool StoreDeDxInfo_;
	bool saveDoubleMu3TriggeredOnly_;
	bool verbose_;

	const double nominalJpsiMass;
	const double nominalPhiMass;
	const double nominalMuonMass;
	const double nominalKaonMass;
	const double nominalPionMass;
	const double nominalKstarMass;

	double JpsiMassWindowBeforeFit_;
	double JpsiMassWindowAfterFit_;
	double JpsiPtCut_;
	double KaonTrackPtCut_;
	double BdKaonTrackPtCut_;
	double PhiMassWindowAfterFit_;
	double PhiMassWindowBeforeFit_;
	double BsLowerMassCutBeforeFit_;
	double BsUpperMassCutBeforeFit_;
	double BsLowerMassCutAfterFit_ ;
	double BsUpperMassCutAfterFit_ ;
	double KstarMassWindowBeforeFit_;
	double KstarMassWindowAfterFit_;
	double BdLowerMassCutBeforeFit_;
	double BdUpperMassCutBeforeFit_;
	double BdLowerMassCutAfterFit_;
	double BdUpperMassCutAfterFit_;

	std::string outputFile_; // output file

	int Mu1Truth;


	int event_counter_;


	double angle_costheta;
	double angle_phi;
	double angle_cospsi;
	double AngleBsDecayLength;

};
#endif
