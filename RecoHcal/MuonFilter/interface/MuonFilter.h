// -*- C++ -*-
//
// Package:    MuonFilter
// Class:      MuonFilter
// 
/**\class MuonFilter MuonFilter.cc MyMuonFilter/MuonFilter/src/MuonFilter.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Taylan Yetkin
//         Created:  Fri May 23 22:48:48 CDT 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"

#include "TMath.h"


class MuonFilter : public edm::EDFilter {
   public:
      explicit MuonFilter(const edm::ParameterSet&);
      ~MuonFilter();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      bool Extrapolate(
	  // inputs
	  double ox, double oy, double oz, double px, double py, double pz, 
	  double radius, double zwidth,
	  // outputs
	  double *thetap_out, double *phip_out,
	  double *thetam_out, double *phim_out
	  );
      
      // ----------member data ---------------------------
      int NTotal;
      int NRejected;
      int fFilterAlgo;
      float fIP_z;
      float fIP_r;
};
