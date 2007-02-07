#ifndef StubCandidate_h
#define StubCandidate_h
#include "Geometry/Vector/interface/GlobalPoint.h"
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;
/////////////////////////////////////////////////////
//These quantities say ERR, but
//in reality will be something like
//2.5 * sigma, or whatever the MC tells us the 
//appropriate error is.
////////////////////////////////////////////////////

const static double _CALZERR = 5;//cm 
const static double _ETCONSTERR = 0.15;//%error
const static double _ZVTXCONSTERR = 15.3;//cm
const static double _CALHEIGHT = 120;//cm
const static double _TRACKERHEIGHT = 110;//cm
const static double _RADIUSOFINTEREST = 50;//cm
const static double _PI = 3.141592653;
const static double _SPEEDOFLIGHT = 299792458;
const static double _MAGFIELD = 4.;
const static double _CONSTFORCURVE = 1e9/_SPEEDOFLIGHT;
const static double _ERRORPOSSIGMA = 1.5;
const static bool VERBOSE = false;

/* bool IsInRegionOfInterest(double _SeedX, double _SeedY, double _SeedZ, */
/* 	    	          double _ZVtx,  */
/* 		          double _TestX, double _TestY, double _TestZ, */
/* 		          double _TestXErr, double _TestYErr, double _TestZErr); */







class StubCandidate{
  //Main idea:
  //Build up a library of functions that determine a given position's consistency
  //with a road based on ECAL barrel quantities.

 public :
  int nhit;
  double chi2xy;

  //Default Constructor, it avails you naught.
  StubCandidate();

  //Constructor with seed position, energy, Zvtx, charge.
  //Note that this doesn't define a road, one has to set the
  //first position for that.
  StubCandidate(double SeedX, 
		double SeedY, 
		double SeedZ,
		double SeedE,
		double ZVtx,
		double ConvR,
		double ConvPhi,
		int charge);
  
  //////////////////////////////////////////////////
  //Setters for seed values
  //////////////////////////////////////////////////
    
  //Set the energy 
  void SetEnergy(double Energy);
  
  //Set the seed position.
  void SetSeedPos(double SeedX, double SeedY, double SeedZ){
    _SeedX = SeedX;
    _SeedY = SeedY;
    _SeedZ = SeedZ;
  };
  
  //Set the vertex position.
  void SetZVtxPos(double ZVtx);
  
  //Set the charge.  Determines path for road.
  void SetCharge(int charge) {_charge = charge;};
  
  
  ////////////////////////////////////////////////
  //Set the first position.  This in turn will call
  //the calculate function which will define a road
  //based on the charge.  Should return true if the
  //road could be created, otherwise, false.
  //////////////////////////////////////////////
  
  bool SetFirstPos(double RConv, double PhiConv);
  
  ////////////////////////////////////////////////
  //Inspectors for some derived quantities.
  ////////////////////////////////////////////////
  
  //Curvature is based on the ET of the object.  Calculated
  //from the energy of the cluster, and the position of the
  //ZVtx.  This is the central value.
  double GetRadiusofCurvature();
  
  //The inner curvature represents the inner bound of the
  //curve based on the ZVtx and Energy.  If the energy fluxuated
  //up, and the ZVtx were misreconstructed to give a higher
  //value for ET, you get this curvature.  These effects
  //are set in the constants _ETCONSTERR and _ZVTXCONSTERR.
  double GetInnerRadiusCurve();
  
  //As above, but for the case where the ET is underestimated.
  double GetOuterRadiusCurve();
  
  //Return the number of hits in the StubCandidate
  int GetNhits();
  
  //Get the Et.  Calculated from seed energy, position and ZVtx.
  double GetEt();
  
  //Get the error assessed on the calculation of the ET.	
  double GetEtErr();
  
  void GetSeedPos(double &x, double &y, double &z){
    x= _SeedX;
    y= _SeedY;
    z= _SeedZ;
  };

  double GetZVtx() {return _ZVtx;};
  //  vector <RecHit> GetHits(){ return _goodhits;};
  
  //  const RecHit GetFirstPos() { return _FirstPos;};
  //  bool HasThisHit(RecHit hit);
  //  bool SameRoadAs(StubCandidate stub);

  
  void GetCenter(double &x, double &y);

  int GetCharge() {return _charge;};

  //Check if a hit is in the road.  Returns true if the hit
  //is consistent with the road.
  bool  IsInRoad(GlobalPoint hit, bool VERBOSE=false);
  
  //Add hit to the stub.  Returns 0
  //if successful, otherwise, Returns 1
  //bool  AddRecHit(RecHit hit, bool VERBOSE=false);
  
  //Is it a good stub?  This will be determined by some
  //set criteria like number of hits, number of inside
  //misses, chi2.
  //bool IsGoodCandidate();
  
  //Has this RecHit already been included in this stub?
  bool HasThisHit(RecHit *rec);
  
  //Is this the same road as the supplied candidate?
  //bool SameRoadAs(StubCandidate *stub);
  //void OrderHits();
  
  
  ~StubCandidate();
  //First position.  Defines the road.
  //RecHit _FirstPos;
  double Chi2XY(vector <GlobalPoint> pts);
  double Chi2Hit(GlobalPoint hit);
  double QuadraticSolve(double A, double B, double C, int charge);
  bool IsInZRegionOfInterest(double _SeedX, double _SeedY, double _SeedZ,
			     double _VtxZ,
			     double _TestX, double _TestY, double _TestZ,
			     bool VERB=false);
  

 private :
  
  
  static const int _MINSTUBHITS = 3;
  //EGammaBasicCluster quantities:
  double _SeedX;
  double _SeedY;
  double _SeedZ;
  double _SeedE;
  //Charge of course, isn't REALLY charge, it is simply
  //whether the road goes clockwise (-1) or counterclockwise (+1).
  int _charge;
  //Event vertex.
  double _ZVtx;
  
  //Center of circle
  double _centerX;
  double _centerY;

  double _outercenterX;
  double _outercenterY;

  double _innercenterX;
  double _innercenterY;
  

  
  //Radius of Curvature defined by ET
  double _RadiusofCurve;
	
  //'Bands' of curvature due to uncertainties.
  double _innerRadiuscurve;
  double _outerRadiuscurve;
  
  //Corresponding ET, and error.
  double _ET;
  double _ETErr;
  
  //This vector holds pointers to all the hits that fall in this
  //road.	
  //vector <RecHit> _goodhits;	
  //vector <const DetLayer*> g_layers;
};
//bool operator<(const StubCandidate& a, const StubCandidate& b) ;
#endif




