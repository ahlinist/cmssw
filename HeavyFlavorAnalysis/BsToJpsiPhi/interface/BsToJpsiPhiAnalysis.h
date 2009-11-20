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
	void fillMCInfoBd( edm::Handle<reco::GenParticleCollection> & genParticles);

private:

	BsToJpsiPhiRootTree * bsRootTree_;
	
	edm::ParameterSet theConfig_;

	const TrackerGeometry* m_tracker;

	edm::InputTag thegenParticlesLabel_;
	edm::InputTag trackLabelK_;
	edm::InputTag trackLabelPi_;
	edm::InputTag triggerTag_; 
	edm::InputTag muonTag_; 
	bool StoreDeDxInfo_;
	double JpsiMassWindowBeforeFit_;
	double JpsiMassWindowAfterFit_;
	double JpsiPtCut_;
	double KaonTrackPtCut_;
	double PhiMassWindowAfterFit_;
	double PhiMassWindowBeforeFit_;
	double BsLowerMassCutBeforeFit_;
	double BsUpperMassCutBeforeFit_;
	double BsLowerMassCutAfterFit_ ;
	double BsUpperMassCutAfterFit_ ;

	std::string outputFile_; // output file

	int Mu1Truth;
	int Mu2Truth;
	int K1Truth;
	int K2Truth;
	int K3Truth;
	int K4Truth;
	int isMatched;
	int isMatchedBd;

	int event_counter_;

	double BSx;
	double BSy;
	double BSz;
	double PVx;
	double PVy;
	double PVz;
	double PVerrx;
	double PVerry;
	double PVerrz;
	
	// single Jpsi

	int MuCounter1;
	int MuCounter2;

	int TrkCounter1;
	int TrkCounter2;
	int K1mcId;
	int K1momId;
	int K1gmomId;
	int K2mcId;
	int K2momId;
	int K2gmomId;
	int Mu1mcId;
	int Mu1momId;
	int Mu1gmomId;
	int Mu2mcId;
	int Mu2momId;
	int Mu2gmomId;

	int K1mcIdBd;
	int K1momIdBd;
	int K1gmomIdBd;
	int K2mcIdBd;
	int K2momIdBd;
	int K2gmomIdBd;
	int Mu1mcIdBd;
	int Mu1momIdBd;
	int Mu1gmomIdBd;
	int Mu2mcIdBd;
	int Mu2momIdBd;
	int Mu2gmomIdBd;
	
	double BLxy;
	double BLxy2;
	double BerrX;
	double BerrY;
	double BerrXY;
	double Bsct1;
	double Bsct2;

	double angle_costheta;
	double angle_phi;
	double angle_cospsi;
	double AngleBsDecayLength;

};
#endif
