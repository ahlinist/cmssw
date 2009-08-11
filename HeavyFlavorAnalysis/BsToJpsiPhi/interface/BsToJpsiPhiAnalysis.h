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

private:

	int JpsiKstarFlag(const edm::Event &iEvent);
	int JpsiKsFlag(const edm::Event &iEvent);
	int JpsiPhiFlag(const edm::Event &iEvent);
	
	BsToJpsiPhiRootTree * bsRootTree_;
	
	edm::ParameterSet theConfig_;

	const TrackerGeometry* m_tracker;

	edm::InputTag thegenParticlesLabel_;
	edm::InputTag trackLabelK_; 
	edm::InputTag trackLabelPi_; 
	edm::InputTag track1_;
	edm::InputTag track2_;
	bool usePixel_;
	bool useStrip_;
	edm::InputTag trajtracks_;
	edm::InputTag triggerTag_; 
	edm::InputTag muonTag_; 
	std::string outputFile_; // output file

	int Mu1Truth;
	int Mu2Truth;
	int K1Truth;
	int K2Truth;
	int isMatched;

	int event_counter_;
	int flag_1;
	int flag_2;
	int flag_3;
	int flag_4;
	int flag_5;
	int flag_6;
	
	int flagKstar;
	int flagKs;
	int flagPhi;

	int nBsDau;
	int momID;
	int dau_1_ID;
	int dau_2_ID;

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

	double JpsiVtxProb;
	int MuCounter1;
	int MuCounter2;
	double JpsiM_alone;
       	double JpsiPhi_alone;
	double JpsiEta_alone;
	double JpsiPt_alone;
	double JpsiMu1Pt;
	double JpsiMu2Pt;
	double JpsiMu1Phi;
	double JpsiMu2Phi;
	double JpsiMu1Eta;
	double JpsiMu2Eta;
	int JpsiMuon1Cat;
	int JpsiMuon2Cat;
	
	double BsMass_before;
	double JpsiMass_before;
	double PhiMass_before;
	double chi2_Bs;
	int ndof_Bs;
	double BsVtxProb;
	double JpsiM;
       	double JpsiPhi;
	double JpsiEta;
	double JpsiPt;
	double PhiM;
       	double PhiPhi;
	double PhiEta;
	double PhiPt;
	double BsM;
       	double BsPhi;
	double BsEta;
	double BsPt;
	double BmassC;

	double	K1Pt;
	double	K2Pt;
	double	K1Eta;
	double	K2Eta;
	double	K1Phi;
	double	K2Phi;
	double	K1Chi2;
	int	K1nHits;
	double	K2Chi2;
	int	K2nHits;
	int	K1pixH;
	int	K1trkH;
	int	K2pixH;
	int	K2trkH;

	double	Mu1Pt;
	double	Mu2Pt;
	double	Mu1Eta;
	double	Mu2Eta;
	double	Mu1Phi;
	double	Mu2Phi;
	double	Mu1Chi2;
	int	Mu1nHits;
	double	Mu2Chi2;
	int	Mu2nHits;
	int	Mu1pixH;
	int	Mu1trkH;
	int	Mu2pixH;
	int	Mu2trkH;

	int	K1trkLay;
	int	K1muDTh;
	int	K1muCSCh;
	int	K1muRPCh;
	int	K2trkLay;
	int	K2muDTh;
	int	K2muCSCh;
	int	K2muRPCh;
	int	Mu1trkLay;
	int	Mu1muDTh;
	int	Mu1muCSCh;
	int	Mu1muRPCh;
	int	Mu2trkLay;
	int	Mu2muDTh;
	int	Mu2muCSCh;
	int	Mu2muRPCh;


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

	double BdM;
	double BdmassC;
	double KstarM;
	double BdVtxProb;
	double BdPhi;
	double BdEta;
	double BdPt;
	double JpsiM_Bd;       
	double AngleBdDecayLength;

};
#endif
