#ifndef _TangentHelix_H_
#define _TangentHelix_H_

#include "RecoTracker/NuclearSeedGenerator/interface/TangentCircle.h"

class TangentHelix {

   public :
       TangentHelix(){}

       /// Calculate the helix from 2 points on the circle (the vertex=innerPoint and the outerPoint)
       /// and the tangent direction at the inner point
       TangentHelix(const GlobalVector& direction, const GlobalPoint& innerPoint, const GlobalPoint& outerPoint) : 
            theInnerPoint(innerPoint), theOuterPoint(outerPoint), theVertexPoint(innerPoint), theCircle(direction, innerPoint, outerPoint) {}

       /// Calculate Helix from 3 points
       TangentHelix(const GlobalPoint& outerPoint, const GlobalPoint& innerPoint, const GlobalPoint& vertexPoint) : 
           theInnerPoint(innerPoint), theOuterPoint(outerPoint), theVertexPoint(vertexPoint), theCircle(outerPoint, innerPoint, vertexPoint) {}

       /// Calculate the parameters of the helix which pass by 2 points (innerPoint and outerPoint) and which is tangent to primHelix
       TangentHelix(const TangentHelix& primCircle, const GlobalPoint& outerPoint, const GlobalPoint& innerPoint);

       GlobalPoint outerPoint() const { return theOuterPoint; }

       GlobalPoint innerPoint() const { return theInnerPoint; }

       GlobalPoint vertexPoint() const { return theVertexPoint; }

       TangentCircle circle() const { return theCircle; }

       GlobalVector directionAtVertex() const;

   private :
       GlobalPoint theInnerPoint;
       GlobalPoint theOuterPoint;
       GlobalPoint theVertexPoint;

       TangentCircle theCircle;
};

#endif
