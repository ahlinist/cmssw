#ifndef RecoBTag_CombinedSV_CombinedTrack_h
#define RecoBTag_CombinedSV_CombinedTrack_h

#include "Geometry/CommonDetAlgo/interface/Measurement1D.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"

namespace combsv {
  class CombinedTrack : public reco::TransientTrack {
    public:
      /**
       * A data class used for storing b-tagging info
       * that can be associated with a track
       */
      CombinedTrack();

      CombinedTrack( const reco::TransientTrack &, double jetDistance,
                 const Measurement1D & ip2d, const Measurement1D & ip3d,
                 double rapidity=-1., bool usedInSVX=false, bool aboveCharmMass=false );

      void print() const;
      int nHitsTotal() const;
      int nHitsPixel() const;
      bool firstHitPixel() const;
      double rapidity() const;
      double jetDistance() const;
      Measurement1D ip2D() const;
      Measurement1D ip3D() const;
      bool aboveCharmMass() const;
      bool isValid() const;

      void setAboveCharmMass ( bool );

    private:
      bool   usedInSVX_;    // part of a secondary vertex?
      double rapidity_;
      double jetDistance_;
      Measurement1D ip2D_; // lifetime-signed 2D impact parameter
      Measurement1D ip3D_; // lifetime-signed 3D impact parameter
      bool aboveCharmMass_;  /**
         * tracks are sorted by lifetime-signed 2D impact
         * parameter significance. Starting from the
         * highest significance, the invariant mass
         * of the tracks is calculated (using Pion mass
         * hypothesis). If the mass exceeds a threshold,
         * this flag is set to true.
         */
      bool isValid_;
  };
}

#endif
