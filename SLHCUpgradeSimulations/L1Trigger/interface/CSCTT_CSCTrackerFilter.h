#ifndef __CSCTT_CSCTRACKERFILTER_HH
#define __CSCTT_CSCTRACKERFILTER_HH (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_CSCTrackerFilter
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Runs CSCTF-track to Tracker-stub matching algorithm 
////////////////////////////////////////////////////////////////

#include "../interface/CSCTT_CSCTFCable.h"
#include "../interface/CSCTT_CSCMatchCable.h"
#include "../interface/CSCTT_TrackerCable.h"
#include "../interface/CSCTT_CSCMatchLookupTable.h"
#include <TH1.h>



class CSCTT_CSCTrackerFilter{

public:

  CSCTT_CSCTrackerFilter();
  
  //keeps input angle in range: (-pi,pi) rad
  static float ConvertAngle ( float angle ) 
     {
      float newangle=angle;
      
        if (angle > 3.14)
	  {
	    while(newangle>3.14) 
	       {newangle=newangle-6.28;}
	    
	    return(newangle);
	  }
	
        if (angle < -3.14)
	  {
	    while(newangle<-3.14) 
	       {newangle=newangle+6.28;}
	    
	    return(newangle);
	  }	
	
	
	return(angle);
     }
  
  void AddTrackerCable ( CSCTT_TrackerCable *cable, int station ){ _trackerCables[station] = cable; }
  void AddCSCTFCable   ( CSCTT_CSCTFCable   *cable ){ _cscCable = cable; }
    
  void AddMatchCable   ( CSCTT_CSCMatchCable *cable ){ _outCable = cable; }

  void SetLookupTable  ( CSCTT_CSCMatchLookupTable *table ) { _lookupTable = table; }

  void ProcessEvent ();

  void PostProcessEvent () { } // noop for now
  
  int totalHits;
  int matchHits;
   
  bool match[5];
  bool matched1;
  TH1F *h_windowEta,*h_windowPhi,*h_windowPt,*h_windowZ;
  
  
  private:
 
  CSCTT_TrackerCable *_trackerCables[5];
  CSCTT_CSCTFCable   *_cscCable;
  
  CSCTT_CSCMatchCable *_outCable;
  CSCTT_CSCMatchLookupTable *_lookupTable;
  

};

#endif

