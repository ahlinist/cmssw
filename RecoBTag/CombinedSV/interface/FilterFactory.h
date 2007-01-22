#ifndef RecoBTag_CombinedBTag_FilterFactory
#define RecoBTag_CombinedBTag_FilterFactory

#include "RecoBTag/CombinedSV/interface/JetFilter.h"
#include "RecoBTag/CombinedSV/interface/TrackFilter.h"
#include "RecoBTag/CombinedSV/interface/VertexFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "MagneticField/Engine/interface/MagneticField.h"

class FilterFactory {
  /**
   *  Class that produces and stores all three filters needed
   *  for combined-sv btagging
   */
   public:
      FilterFactory ( const edm::ParameterSet &,
                      const MagneticField * m=0 );

      FilterFactory ( const TrackFilter &,
                      const VertexFilter &,
                      const JetFilter & );
      FilterFactory();

      const TrackFilter & trackFilter() const;
      VertexFilter & vertexFilter();
      const JetFilter & jetFilter() const;

      const VertexFilter & constVertexFilter() const;

    private:
      void createTrackFilter ( const edm::ParameterSet & );
      void createVertexFilter ( const edm::ParameterSet &,
                                const MagneticField * m );
      void createJetFilter ( const edm::ParameterSet & );

   private:
      TrackFilter track_;
      VertexFilter vertex_;
      JetFilter jet_;
};

#endif
