#ifndef CSCRecHit_CSCWireCluster_h
#define CSCRecHit_CSCWireCluster_h
 
/** \class CSCWireCluster
 * A cluster of adjacent wire(group)s with signals 
 *
 * \author R. Wilkinson
 */
 
class CSCWireDigi;
class CSCLayerGeometry;

class CSCWireCluster {
 public:
   /** ctor from range of wires
    */
   CSCWireCluster(int firstWire, int lastWire, int beamCrossingTag, float wireSpacing)
   : theFirstWire(firstWire), theLastWire(lastWire), 
     theBeamCrossing(beamCrossingTag), theWireSpacing(wireSpacing) {}

   /** ctor from wire digi
    */
   CSCWireCluster(const CSCWireDigi &, const CSCLayerGeometry *);
 
   ///  Return true if this digi can be added to the cluster
   bool add(const CSCWireDigi &, const CSCLayerGeometry *);
 
   /// The central wire of the cluster (possibly not an actual wire)
   float centerWire() const {return (theFirstWire+theLastWire)/2.;}

   /// The width of the cluster (distance units: cm)
   float width() const {return (theLastWire-theFirstWire) * theWireSpacing;}

   /// The beam crossing tag for the cluster
   int getBeamCrossingTag() const {return theBeamCrossing;}
 
 private:
   int theFirstWire;
   int theLastWire;
   int theBeamCrossing;
   int theLastChannel;
   float theWireSpacing;
};
  
#endif
