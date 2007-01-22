#ifndef RecoBTag_V0Checker
#define RecoBTag_V0Checker

#include "DataFormats/VertexReco/interface/Vertex.h"

class V0Checker {
  /**
   *  Class that checks if vertex can be a V0
   */
   public:
     V0Checker ( double v0mass );
     bool check ( const reco::Vertex & v ) const;
   private:
     double vertexV0MassWindow_;
     double mPiPlus_;
     double mK0_;
};

#endif
