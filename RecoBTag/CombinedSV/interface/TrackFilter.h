#ifndef RecoBTag_TrackFilter
#define RecoBTag_TrackFilter

#include "RecoBTag/CombinedSV/interface/CombinedTrack.h"

class TrackFilter {
  /**
   *  Class that determines if track is being considered for btagging.
   */

   public:
     /**
      *  \param pTMin minimum transverse momentum of track
      *  \param nHitAllMin minimum number of total (silicon plus strips) hits
      *  \param nHitPixelMin minimum number of hits in the pixels
      *  \param normed_chi2Max max chi^2 / ndof of track
      *  \param etaMin cuts on rapidities
      *  \param etaMax cuts on rapidities
      *  \param ip2DMin cut on transverse impact parameter
      *  \param ip2DMax cut on transverse impact parameter
      *  \param jetDistanceMax signed impact parameter in 3d
      *
      */
     TrackFilter ( float pTMin=1.0, int nHitAllMin=8, int nHitPixelMin=2, 
         float normed_chi2Max=10., float etaMin=-9999.0, float etaMax=9999.0, float ip2DMin=-.2, 
         float ip2DMax=0.2, float jetDistanceMax=9999. );

     /**
      *  returns true if track is acceptible, false for a veto!
      */
     bool operator() ( const combsv::CombinedTrack &, 
                       double ipSigni2DCut, bool print=false ) const;
     bool operator() ( const combsv::CombinedTrack &, 
                       bool print=false ) const;

     void print ( const combsv::CombinedTrack &, double ipSigni2DCut ) const;
   private:
     float pTMin_;
     int nHitAllMin_;
     int nHitPixelMin_;
     float normedChi2Max_;
     float etaMin_;
     float etaMax_;
     float ip2DMin_;
     float ip2DMax_;
     float jetDistanceMax_;
};

#endif
