#include "TauAnalysis/Core/interface/MEtHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

#include "TauAnalysis/CandidateTools/interface/candidateAuxFunctions.h"

#include <TMath.h>

MEtHistManager::MEtHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<MEtHistManager::MEtHistManager>:" << std::endl;

  metSrc_ = cfg.getParameter<edm::InputTag>("metSource");
  //std::cout << " metSrc = " << metSrc_ << std::endl;
  
  expectUniqueMEt_ = cfg.exists("expectUniqueMEt") ?
    cfg.getParameter<bool>("expectUniqueMEt") : true;

  metSignificanceSrc_ = ( cfg.exists("metSignificanceSource") ) ? 
    cfg.getParameter<edm::InputTag>("metSignificanceSource") : edm::InputTag();
  //std::cout << " metSignificanceSrc = " << metSignificanceSrc_ << std::endl;

  if ( cfg.exists("leg1Source") ) leg1Src_ = cfg.getParameter<edm::InputTag>("leg1Source");
  //std::cout << " leg1Src = " << leg1Src_ << std::endl;
  if ( cfg.exists("leg2Source") ) leg2Src_ = cfg.getParameter<edm::InputTag>("leg2Source");
  //std::cout << " leg2Src = " << leg2Src_ << std::endl;

  vertexSrc_ = cfg.getParameter<edm::InputTag>("vertexSource");
  //std::cout << " vertexSrc = " << vertexSrc_ << std::endl;
}

MEtHistManager::~MEtHistManager()
{
//--- nothing to be done yet...
}

void MEtHistManager::bookHistogramsImp()
{
  //std::cout << "<MEtHistManager::bookHistogramsImp>:" << std::endl;
  
  hMEtPt_ = book1D("MEtPt", "MEtPt", 75, 0., 150.);
  hMEtPhi_ = book1D("MEtPhi", "MEtPhi", 36, -TMath::Pi(), +TMath::Pi());
  hMEtPx_ = book1D("MEtPx", "MEtPx", 150, -150., 150.);
  hMEtPy_ = book1D("MEtPy", "MEtPy", 150, -150., 150.);
  
  for ( vdouble::const_iterator vertexPtThreshold = vertexPtThresholds_.begin();
	vertexPtThreshold != vertexPtThresholds_.end(); ++vertexPtThreshold ) {
    std::ostringstream meName_ostringstream;
    meName_ostringstream << "MEtPtVsNumVerticesPtGt" << std::fixed << std::setprecision(1) << (*vertexPtThreshold);
    int errorFlag = 0;
    std::string meName_string = replace_string(meName_ostringstream.str(), ".", "_", 0, 1, errorFlag);
    MonitorElement* me = book2D(meName_string.data(), meName_string.data(), 50, 0., 250., 10, -0.5, 9.5);
    hMEtPtVsNumVertices_.push_back(me);
  }

  hMEtSignificance_ = book1D("MEtSignificance", "MEtSignificance", 101, -0.5, 100.05);
  
  hGenMEtPt_ = book1D("GenMEt_Pt", "GenMEt_Pt", 75, 0., 150.);
  hGenMEtPhi_ = book1D("GenMEt_Phi", "GenMEt_Phi", 36, -TMath::Pi(), +TMath::Pi());

  hMEtPtRecVsGen_ = book2D("MEtPtRecVsGen", "MEtPtRecVsGen", 30, 0., 150., 30, 0., 150.);
  hMEtPhiDiffGen_ = book1D("MEtPhiDiffGen", "MEtPhiDiffGen", 72, -TMath::Pi(), +TMath::Pi());
  hMEtPhiRecVsGen_ = book2D("MEtPhiRecVsGen", "MEtPhiRecVsGen", 36, -TMath::Pi(), +TMath::Pi(), 36, -TMath::Pi(), +TMath::Pi());
  hMEtSumEtRecVsGen_ = book2D("MEtSumEtRecVsGen", "MEtSumEtRecVsGen", 20, 0., 500., 20, 0., 500.);

  hMEtPtDiffVsGenMEtPt_ = bookProfile1D("MEtPtDiffVsGenMEt_Pt", "MEtPtDiffVsGenMEt_Pt", 75, 0., 150.);
  hMEtPhiDiffVsGenMEtPt_ = bookProfile1D("MEtPhiDiffVsGenMEt_Pt", "MEtPhiDiffVsGenMEt_Pt", 75, 0., 150.);

  hMEtPtDiff_ = book1D("MEtPtDiff", "MEtPtDiff", 100, -50., +50.);
  hMEtPtPull_ = book1D("MEtPtPull", "MEtPtPull", 100, -5.0, +5.0);

  hMEtPxDiff_ = book1D("MEtPxDiff", "MEtPxDiff", 100, -50., +50.);
  hMEtPxPull_ = book1D("MEtPxPull", "MEtPxPull", 100, -5.0, +5.0);
  hMEtPyDiff_ = book1D("MEtPyDiff", "MEtPyDiff", 100, -50., +50.);
  hMEtPyPull_ = book1D("MEtPyPull", "MEtPyPull", 100, -5.0, +5.0);

  hMEtPparlGenMEtDiff_ = book1D("MEtPparlGenMEtDiff", "MEtPparlGenMEtDiff", 100, -50., +50.);
  hMEtPparlGenMEtPull_ = book1D("MEtPparlGenMEtPull", "MEtPparlGenMEtPull", 100, -5.0, +5.0);
  hMEtPperpGenMEtDiff_ = book1D("MEtPperpGenMEtDiff", "MEtPperpGenMEtDiff", 100, -50., +50.);
  hMEtPperpGenMEtPull_ = book1D("MEtPperpGenMEtPull", "MEtPperpGenMEtPull", 100, -5.0, +5.0);
  
  hMEtPparlBisectorLeg1Leg2Diff_ = book1D("MEtPparlBisectorLeg1Leg2Diff", "MEtPparlBisectorLeg1Leg2Diff", 100, -50., +50.);
  hMEtPparlBisectorLeg1Leg2Pull_ = book1D("MEtPparlBisectorLeg1Leg2Pull", "MEtPparlBisectorLeg1Leg2Pull", 100, -5.0, +5.0);
  hMEtPparlLeg1Diff_ = book1D("MEtPparlLeg1Diff", "MEtPparlLeg1Diff", 100, -50., +50.);
  hMEtPparlLeg1Pull_ = book1D("MEtPparlLeg1Pull", "MEtPparlLeg1Pull", 100, -5.0, +5.0);
  hMEtPparlLeg2Diff_ = book1D("MEtPparlLeg2Diff", "MEtPparlLeg2Diff", 100, -50., +50.);
  hMEtPparlLeg2Pull_ = book1D("MEtPparlLeg2Pull", "MEtPparlLeg2Pull", 100, -5.0, +5.0);
}

void fillMEtProjectionHistograms(MonitorElement* hMEtPparlDiff, MonitorElement* hMEtPparlPull,
				 MonitorElement* hMEtPperpDiff, MonitorElement* hMEtPperpPull,
				 double metDiffPx, double metDiffPy, double phiProjectionAxis, double metResolution,
				 double evtWeight)
{
//--- compute components of difference between reconstructed and generated missing Et 
//    parallel and perpendicular to "true" MEt direction
//    by rotating (x,y) vector of difference in transverse plane
//    to coordinate system in which "true" MEt direction coincides with x'-axis
//
//    NOTE: rotation matrix 
//         = | cos(phi) sin(phi)|
//           |-sin(phi) cos(phi)|
//          where phi = azimuthal angle of generated missing Et vector; 
//          see
//           http://en.wikipedia.org/wiki/Rotation_(mathematics) 
//                 
  double cosPhiProjectionAxis = TMath::Cos(phiProjectionAxis);
  double sinPhiProjectionAxis = TMath::Sin(phiProjectionAxis);

  double metDiffPparl =  cosPhiProjectionAxis*metDiffPx + sinPhiProjectionAxis*metDiffPy;
  double metDiffPperp = -sinPhiProjectionAxis*metDiffPx + cosPhiProjectionAxis*metDiffPy;
  
  if ( hMEtPparlDiff ) hMEtPparlDiff->Fill(metDiffPparl, evtWeight);
  if ( hMEtPperpDiff ) hMEtPperpDiff->Fill(metDiffPperp, evtWeight);
  
  if ( metResolution > 0. ) {
    if ( hMEtPparlPull ) hMEtPparlPull->Fill(metDiffPparl/metResolution, evtWeight);
    if ( hMEtPperpPull ) hMEtPperpPull->Fill(metDiffPperp/metResolution, evtWeight);
  }
}

void MEtHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)

{  
  //std::cout << "<MEtHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<std::vector<pat::MET> > patMETs;
  getCollection(evt, metSrc_, patMETs);

  double metSignificance = -1.;
  if ( metSignificanceSrc_.label() != "" ) {
     edm::Handle<edm::View<reco::CaloMET> > recoMETs;
     evt.getByLabel(metSignificanceSrc_, recoMETs);

     if ( recoMETs->size() == 1 ) metSignificance = recoMETs->begin()->metSignificance();
  }

  edm::Handle<reco::CandidateView> leg1Particles;
  if ( leg1Src_.label() != "" ) evt.getByLabel(leg1Src_, leg1Particles);
  edm::Handle<reco::CandidateView> leg2Particles;
  if ( leg2Src_.label() != "" ) evt.getByLabel(leg2Src_, leg2Particles);

  edm::Handle<reco::VertexCollection> recoVertices;
  evt.getByLabel(vertexSrc_, recoVertices);

  if ( patMETs->size() == 1 ) {
    const pat::MET& theEventMET = (*patMETs->begin());

    hMEtPt_->Fill(theEventMET.pt(), evtWeight);
    hMEtPhi_->Fill(theEventMET.phi(), evtWeight);
    hMEtPx_->Fill(theEventMET.px(), evtWeight);
    hMEtPy_->Fill(theEventMET.py(), evtWeight);

    std::vector<double> trackPtSums = compTrackPtSums(*recoVertices);
    assert(vertexPtThresholds_.size() == hMEtPtVsNumVertices_.size());
    size_t numVertexPtThresholds = vertexPtThresholds_.size();
    for ( size_t iVertexPtThreshold = 0; iVertexPtThreshold < numVertexPtThresholds; ++iVertexPtThreshold ) {
      size_t numVertices = getNumVerticesPtGtThreshold(trackPtSums, vertexPtThresholds_[iVertexPtThreshold]);
      hMEtPtVsNumVertices_[iVertexPtThreshold]->Fill(theEventMET.pt(), numVertices, evtWeight);
    }
    
    hMEtSignificance_->Fill(metSignificance, evtWeight);

    if ( theEventMET.genMET() ) {
      double genMEtPt     = theEventMET.genMET()->pt();
      double genMEtPx     = theEventMET.genMET()->px();
      double genMEtPy     = theEventMET.genMET()->py();      
      double genMEtPhi    = theEventMET.genMET()->phi();
      double genMEtSumEt  = theEventMET.genMET()->sumEt();

      double recoMEtPt    = theEventMET.pt();
      double recoMEtPx    = theEventMET.px();
      double recoMEtPy    = theEventMET.py();
      double recoMEtPhi   = theEventMET.phi();
      double recoMEtSumEt = theEventMET.sumEt();

      double metDiffPt    = recoMEtPt  - genMEtPt;
      double metDiffPx    = recoMEtPx  - genMEtPx;
      double metDiffPy    = recoMEtPy  - genMEtPy;
      double metDiffPhi   = recoMEtPhi - genMEtPhi;

      double metRes1d     = TMath::Sqrt(genMEtSumEt);

      hGenMEtPt_->Fill(genMEtPt, evtWeight);
      hGenMEtPhi_->Fill(genMEtPhi, evtWeight);

      hMEtPtRecVsGen_->Fill(genMEtPt, recoMEtPt, evtWeight);
      hMEtPhiDiffGen_->Fill(metDiffPhi, evtWeight);
      hMEtPhiRecVsGen_->Fill(genMEtPhi, recoMEtPhi, evtWeight);
      hMEtSumEtRecVsGen_->Fill(genMEtSumEt, recoMEtSumEt, evtWeight);
/*
  
  CV: temporary work-around until MonitorElement::Fill(double, double, double) is fixed for TProfiles

      hMEtPtDiffVsGenMEtPt_->Fill(genMEtPt, metDiffPt, evtWeight);
      hMEtPhiDiffVsGenMEtPt_->Fill(genMEtPt, metDiffPhi, evtWeight);
 */
      hMEtPtDiffVsGenMEtPt_->getTProfile()->Fill(genMEtPt, metDiffPt, evtWeight);
      hMEtPhiDiffVsGenMEtPt_->getTProfile()->Fill(genMEtPt, metDiffPhi, evtWeight);

      hMEtPtDiff_->Fill(metDiffPt, evtWeight);
      hMEtPtPull_->Fill(metDiffPt/metRes1d, evtWeight);

      hMEtPxDiff_->Fill(metDiffPx, evtWeight);
      hMEtPxPull_->Fill(metDiffPx/metRes1d, evtWeight);
      hMEtPyDiff_->Fill(metDiffPy, evtWeight);
      hMEtPyPull_->Fill(metDiffPy/metRes1d, evtWeight);

      if ( genMEtPt > 5. ) {
	fillMEtProjectionHistograms(hMEtPparlGenMEtDiff_, hMEtPparlGenMEtPull_, 
				    hMEtPperpGenMEtDiff_, hMEtPperpGenMEtPull_,
				    metDiffPx, metDiffPy, genMEtPhi, metRes1d, evtWeight);
      }

      if ( leg1Src_.label() != "" ) {
	for ( reco::CandidateView::const_iterator leg1Particle = leg1Particles->begin();
	      leg1Particle != leg1Particles->end(); ++leg1Particle ) {
	  if ( leg1Particle->pt() > 5. )
	    fillMEtProjectionHistograms(hMEtPparlLeg1Diff_, hMEtPparlLeg1Pull_, 0, 0,
					metDiffPx, metDiffPy, leg1Particle->phi(), metRes1d, evtWeight);
	}
      }

      if ( leg2Src_.label() != "" ) {
	for ( reco::CandidateView::const_iterator leg2Particle = leg2Particles->begin();
	      leg2Particle != leg2Particles->end(); ++leg2Particle ) {
	  if ( leg2Particle->pt() > 5. )
	    fillMEtProjectionHistograms(hMEtPparlLeg2Diff_, hMEtPparlLeg2Pull_, 0, 0,
					metDiffPx, metDiffPy, leg2Particle->phi(), metRes1d, evtWeight);
	}
      }

      if ( leg1Src_.label() != "" && leg2Src_.label() != "" ) {
	for ( reco::CandidateView::const_iterator leg1Particle = leg1Particles->begin();
	      leg1Particle != leg1Particles->end(); ++leg1Particle ) {
	  for ( reco::CandidateView::const_iterator leg2Particle = leg2Particles->begin();
		leg2Particle != leg2Particles->end(); ++leg2Particle ) {
	    if ( leg1Particle->pt() > 5. && leg2Particle->pt() > 5. && 
		 reco::deltaR(leg1Particle->p4(), leg2Particle->p4()) > 0.7 ) {
	      double phiBisectorLeg1Leg2 = normalizedPhi(leg1Particle->phi() - leg2Particle->phi());
	      fillMEtProjectionHistograms(hMEtPparlBisectorLeg1Leg2Diff_, hMEtPparlBisectorLeg1Leg2Pull_, 0, 0,
					  metDiffPx, metDiffPy, phiBisectorLeg1Leg2, metRes1d, evtWeight);
	    }
	  }
	}
      }
    }
  } else {
    if ( expectUniqueMEt_ )
      edm::LogError ("MEtHistManager::fillHistograms") 
	<< " Exactly one MET object expected per event" 
	<< " (found " << patMETs->size() << " objects in collection = " << metSrc_.label() << ")" 
	<< " --> skipping !!";
  }
}
