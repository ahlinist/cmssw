#ifndef RecoTauTag_CombinedTauTag_ECALBounds_H
#define RecoTauTag_CombinedTauTag_ECALBounds_H

#include "Geometry/Surface/interface/ReferenceCounted.h"
#include "Geometry/Surface/interface/Surface.h"
class BoundCylinder;
class BoundDisk;
class Cylinder;
class Disk;

class ECALBounds {
public:
  static const Surface& barrelBound()    {check(); return *theCylinder;}
  static const Surface& negativeEndcapDisk() {check(); return *theNegativeDisk;}
  static const Surface& positiveEndcapDisk() {check(); return *thePositiveDisk;}
  /** Hard-wired numbers defining the envelope of the sensitive volumes.
   */
  static float barrel_innerradius()     {return 129.0f;}
  static float barrel_outerradius()     {return 175.f;}
  static float barrel_halfLength() {return 270.89f;}
  static float endcap_innerradius()     {return 31.6f;}
  static float endcap_outerradius()     {return 171.1f;}
  static float endcap_innerZ() {return 314.40f;}
  static float endcap_outerZ() {return 388.f;}    
 private:
  static ReferenceCountingPointer<Surface> theCylinder;
  static ReferenceCountingPointer<Surface> theNegativeDisk;
  static ReferenceCountingPointer<Surface> thePositiveDisk;
  static bool theInit;
  static void check() {if (!theInit) initialize();}
  static void initialize();
};

#endif

