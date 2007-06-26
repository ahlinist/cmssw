#ifndef EgammaEfficiency_Electron_h
#define EgammaEfficiency_Electron_h

#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObjectFwd.h"

namespace EgEff
{
   class EmObject : public reco::RecoCandidate
   {
      public:
         // Default constructor
         EmObject() : reco::RecoCandidate() {}

         // Constructor from values
         EmObject(Charge q, const LorentzVector &p4, const Point &vtx = Point(0, 0, 0)) :
            reco::RecoCandidate(q, p4, vtx), hasTrack_(false), hasGsfTrack_(false), hasSc_(false) {}

         // Default destructor
         virtual ~EmObject(){};

         // Cloning
         virtual EmObject *clone() const;

         // Track references
         reco::TrackRef genericTrack() const; // Returns a reco::TrackRef instance of present track
         virtual reco::TrackRef track() const;
         virtual reco::GsfTrackRef gsfTrack() const;
         void setTrack(const reco::TrackRef &r){track_ = r; hasTrack_ = true;}
         void setGsfTrack(const reco::GsfTrackRef &r){gsfTrack_ = r; hasGsfTrack_ = true;}

         // SuperCluster references
         virtual reco::SuperClusterRef superCluster() const;
         void setSuperCluster(const reco::SuperClusterRef &r){superCluster_ = r; hasSc_ = true;}

         // Safety check for cuts
         bool hasTrack() const {return hasTrack_ || hasGsfTrack_;}
         bool hasSuperCluster() const {return hasSc_;}

	 // For histogramming help
	 bool isBarrel() const;

      private:
         virtual bool overlap(const Candidate&) const;
         reco::SuperClusterRef superCluster_;
         reco::TrackRef track_;
         reco::GsfTrackRef gsfTrack_;
         bool hasTrack_;
         bool hasGsfTrack_;
         bool hasSc_;
   };
}

#endif

