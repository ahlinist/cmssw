#ifndef __TurboSimModifiers_h
#define __TurboSimModifiers_h

#include <algorithm>
// function definitions in Validation/VstTurboSim/interface/TurboSimModifiers.ii
// These definitions included from src/TurboSimRule.cc
// This file included from Validation/VstTurboSim/interface/TurboSimRule.hh

namespace TSModifier {

   /** 
    * Removes UNCL 'particles' from a <code>TurboSimEvent</code>'s
    * <code>partonObjs</code>.
    *
    * @returns False if event is empty after discard.
    */
   bool DiscardUncl( TurboSimEvent &event );

   /**
    * Arranges parton and reco objects in order of descending Pt.
    * First object has hightest Pt (summed Pt of individual particles)
    */
   bool SortObjects( TurboSimEvent &event );

   /**
    * Removes duplicate particles from <code>TurboSimEvent</code>'s
    * objects. Objects must be sorted first!
    *
    * @param event Event to modify.
    * @returns False if event is empty after discard.
    */
   bool DiscardDuplicateObjects( TurboSimEvent &event );

} // namespace  
      
#endif
