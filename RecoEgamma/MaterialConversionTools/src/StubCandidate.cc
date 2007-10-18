#include "RecoEgamma/MaterialConversionTools/interface/StubCandidate.h"
#include "Geometry/Vector/interface/GlobalPoint.h"
#include <TMath.h>

#define StubCandidate_cxx

// bool operator<(const StubCandidate& a, const StubCandidate& b) 
// {

//   if ( fabs(a.chi2xy - b.chi2xy)<.00001)
//     return (a.nhit > b.nhit);
  
//   return (a.chi2xy < b.chi2xy);

    
// }


bool StubCandidate::IsInZRegionOfInterest(double _SeedX, double _SeedY, double _SeedZ,
					  double _VtxZ,
					  double _TestX, double _TestY, double _TestZ,
					  double _TestXXerr, double _TestYYerr,
					  bool VERB)
{
  //Testing the Z coordinate position of hit.
  
  //Define the vertex error on Z
  double ZVtxMin = _VtxZ - _ZVTXCONSTERR;
  double ZVtxMax = _VtxZ + _ZVTXCONSTERR;
  
  double SeedZMin = _SeedZ - _CALZERR;
  double SeedZMax = _SeedZ + _CALZERR;
  
  double RCAL = sqrt(_SeedX*_SeedX + _SeedY*_SeedY);
  double slopemin = (SeedZMin - ZVtxMin) / RCAL;
  double slopemax = (SeedZMax - ZVtxMax) / RCAL;
  
  double interceptmin = ZVtxMin;
  double interceptmax = ZVtxMax;
  
  double TESTR = sqrt(_TestX*_TestX + _TestY*_TestY);
  
  double REGIONZMax = slopemax * TESTR + interceptmax + 2.*_TestYYerr;
  double REGIONZMin = slopemin * TESTR + interceptmin - 2.*_TestYYerr;
  if (VERB){
    cout << "Region Z Max: " << REGIONZMax << endl;
    cout << "Region Z Min: " << REGIONZMin << endl;
    cout << "Test Z: " << _TestZ << endl;
    cout << "TestZ - RegionZMax: " << _TestZ - REGIONZMax << endl;
    cout << "RegionZMin - TestZ: " << REGIONZMin - _TestZ << endl;
  }
  
  if (  REGIONZMax > _TestZ && REGIONZMin < _TestZ){
    if (VERB) cout << "Hit is in Z region of interest" << endl;
    return true; 
  }
  if (VERB) cout << "Hit is not within Z region of interest" << endl;
  return false;
}

double StubCandidate::QuadraticSolve(double A, double B, double C, int charge)
{
  double solve=0;
  if (A < 1e-6){
    solve = -1.*C/B;
    return solve;
  }
  
  double disc = B*B - 4. * A * C;
  if (disc < 0 ) return  -1000000;
  
  double sqrtdsc = sqrt(disc);
  if (sqrtdsc < 1e-6){ 
    solve =  (-1.*B)/(2.*A);
    return solve;
  }	
  
  if (charge == -1){
    solve = (-1.*B - sqrtdsc)/(2.*A);
    return solve;
  }
  
  if (charge == 1){
    solve = (-1.*B + sqrtdsc)/(2.*A);
    return solve;
  }
  
  return -1000000;
  
  
}


StubCandidate::StubCandidate()
{
  _SeedX = 0;
  _SeedY = 0;
  _SeedZ = 0;
  _SeedE = 0;
  _ZVtx = 0;
  _charge =0;
    
}


//Constructor with seed position, energy, Zvtx
StubCandidate::StubCandidate(double SeedX, 
              		     double SeedY, 
              		     double SeedZ,
              	             double SeedE,
              		     double ZVtx,
			     double RConv,
			     double PhiConv,
			     int charge)
{
  _SeedX = SeedX;
  _SeedY = SeedY;
  _SeedZ = SeedZ;
  _SeedE = SeedE;
  _ZVtx  = ZVtx; 
  _charge = charge;
  _ET = GetEt();
  _ETErr = GetEtErr();
  _RadiusofCurve = GetRadiusofCurvature();
  _innerRadiuscurve = GetInnerRadiusCurve();
  _outerRadiuscurve = GetOuterRadiusCurve();
  SetFirstPos(RConv, PhiConv);
  _PhiConv = PhiConv;
  _RConv = RConv;
}

// int StubCandidate::GetNhits() 
// {
//   const int blah = _goodhits.size();
//   return blah;
// }	

void StubCandidate::SetEnergy(double Energy) 
{
  _SeedE = Energy;
  _ET = GetEt();
  _RadiusofCurve = GetRadiusofCurvature();
  _innerRadiuscurve = GetInnerRadiusCurve();
  _outerRadiuscurve = GetOuterRadiusCurve(); 
}

void StubCandidate::SetZVtxPos(double ZVtx) 
{
  _ZVtx = ZVtx;
  _ET = GetEt();
  _RadiusofCurve = GetRadiusofCurvature();
  _innerRadiuscurve = GetInnerRadiusCurve();
  _outerRadiuscurve = GetOuterRadiusCurve();
}



/////////////////////////////////////////////////

//Set the first position.  This in turn will call
//the calculate function which will define a road
//based on the charge.  Should return true if the
//road could be created, otherwise, false.

bool StubCandidate::SetFirstPos(double RConv, double PhiConv)
{
  double XPos = RConv * cos(PhiConv);
  double YPos = RConv * sin(PhiConv);
  
  double CenterX=0;
  double CenterY=0;
  
  //With the first hit defined, I can
  //define my circle (having the radius and the
  //charge defined already).
  
  //  double RadiusSeed = sqrt(_SeedX*_SeedX + _SeedY*_SeedY);
  // double RadiusHit = RConv;
  // make a line between the two points:
      
  if (fabs(XPos - _SeedX) < 1e-6){
    if (VERBOSE) cout << "Vertical line case" << endl;
    //Line between two points is vertical
    //thus intercept is horizontal.
    double midy = YPos + ((YPos - _SeedY)/2.);
    //	  double midx = _SeedX;
    double perpslope = 0;
    double perpintercept = midy;
    double quadraticforA = (1+perpslope*perpslope);
    double quadraticforB = (-2.*_SeedX 
			    -2.*_SeedY
			    +2.*perpslope*perpintercept);
    double quadraticforC = -2.*_SeedY*perpintercept
      + perpintercept*perpintercept
      + _SeedX * _SeedX
      + _SeedY * _SeedY
      - _RadiusofCurve*_RadiusofCurve;
    
    double CenterX1 = QuadraticSolve(quadraticforA,
				     quadraticforB,
				     quadraticforC,
				     1);
    
    double CenterX2 = QuadraticSolve(quadraticforA,
				     quadraticforB,
				     quadraticforC,
				     -1);
    
    CenterY = midy;
	
    //Figure out which solution we'll call 'positive'
    double VecX1 = CenterX1 - XPos;
    double VecX2 = CenterX1 - _SeedX;
    double VecY1 = CenterY - YPos;
    double VecY2 = CenterY - _SeedY;
    double CrossProduct = (VecX1*VecY2 -VecX2*VecY1);
    
    if (CrossProduct<= 0 && _charge ==-1){
      CenterX = CenterX1;
    }
    if (CrossProduct < 0 && _charge == 1){
      CenterX = CenterX2;
    }
    
    if (CrossProduct>= 0 && _charge ==-1){
      CenterX = CenterX2;
    }
    if (CrossProduct > 0 && _charge == 1){
      CenterX = CenterX1;
    }
    
    _centerX = CenterX;
    _centerY = CenterY;
    
    return true;
  }
  else {
    if (VERBOSE) cout << "Not vertical line" << endl;
    double slopeline = (YPos - _SeedY)/(XPos - _SeedX);
    double interceptline = _SeedY - slopeline * _SeedX;
    //find the midpoint of the line:
    double midx = 0;
    double dist = fabs(_SeedX - XPos)/2.; //This is the distance to go in X.
    //to get to midpoint.
    if (XPos > _SeedX)
      midx = XPos - dist;
    if (XPos < _SeedX)
      midx = XPos + dist;
    
    double midy = slopeline*midx + interceptline;
    if (VERBOSE){
      cout << "Slope: " << slopeline << endl;
      cout << "intercept: " << interceptline << endl;
      cout << "Midpoint: " << midx << ", " <<midy << endl;
    }
    //find the line perpendicular
    if ( fabs(YPos - _SeedY) < 1e-6){
      if (VERBOSE) cout << "Horizontal line case" << endl;
      //line between two points is horizontal.
      //therefore the perpendicular slope is vertical
      CenterX = midx;
      double quadraticforA = 1;
      double quadraticforB = -2.*_SeedY;
      double quadraticforC = (_SeedX - midx)*(_SeedX - midx)
	+ _SeedY * _SeedY
	- _RadiusofCurve*_RadiusofCurve;
      CenterY=0;
      double CenterY1 = QuadraticSolve(quadraticforA,
				       quadraticforB,
				       quadraticforC,
				       1);
      double CenterY2 = QuadraticSolve(quadraticforA,
				       quadraticforB,
				       quadraticforC,
				       -1);
      //Figure out which solution we'll call 'positive'
      double VecX1 = CenterX - XPos;
      double VecX2 = CenterX - _SeedX;
      double VecY1 = CenterY1 - YPos;
      double VecY2 = CenterY1 - _SeedY;
      double CrossProduct = (VecX1*VecY2 -VecX2*VecY1);
      
      if (CrossProduct<= 0 && _charge ==-1){
	CenterY = CenterY1;
      }
      if (CrossProduct < 0 && _charge == 1){
	CenterY = CenterY2;
      }
      
      if (CrossProduct >= 0 && _charge ==-1){
	CenterY = CenterY2;
      }
      if (CrossProduct > 0 && _charge == 1){
	CenterY = CenterY1;
      }	  
      
      _centerX = CenterX;
      _centerY = CenterY;
      
      return true;
    }
    else {
      double perpslope = -1./slopeline;
      double perpintercept = midy - perpslope * midx;
      double quadraticforA = (1+perpslope*perpslope);
      double quadraticforB = (-2.*_SeedX 
			      -2.*_SeedY * perpslope
			      +2.*perpslope*perpintercept);
      double quadraticforC = -2.*_SeedY*perpintercept
	+ perpintercept*perpintercept
	+ _SeedX * _SeedX
	+ _SeedY * _SeedY
	- _RadiusofCurve*_RadiusofCurve;
      
      CenterX=0;
      
      double CenterX1 = QuadraticSolve(quadraticforA,
				       quadraticforB,
				       quadraticforC,
				       1);
      double CenterX2 = QuadraticSolve(quadraticforA,
				       quadraticforB,
				       quadraticforC,
				       -1);
      double CenterY1 = CenterX1 * perpslope + perpintercept;
      double CenterY2 = CenterX2 * perpslope + perpintercept;
      //	  CenterY = CenterX * perpslope + perpintercept;
      
      //Figure out which solution we'll call 'positive'
      double VecX1 = CenterX1 - XPos;
      double VecX2 = CenterX1 - _SeedX;
      double VecY1 = CenterY1 - YPos;
      double VecY2 = CenterY1 - _SeedY;
      double CrossProduct = (VecX1*VecY2 -VecX2*VecY1);
      
      if (CrossProduct<= 0 && _charge ==-1){
	CenterX = CenterX1;
	CenterY = CenterY1;
      }
      if (CrossProduct < 0 && _charge == 1){
	CenterX = CenterX2;
	CenterY = CenterY2;
      }
      
      if (CrossProduct>= 0 && _charge ==-1){
	CenterX = CenterX2;
	CenterY = CenterY2;
      }
      if (CrossProduct > 0 && _charge == 1){
	CenterX = CenterX1;
	CenterY = CenterY1;
      }	  
      
      _centerX = CenterX;
      _centerY = CenterY;
      
      return true;
    }
  }
  
  if (VERBOSE) cout << "No case fit!" << endl;
  return false;
  
}


//Inspectors for derived quantities.
double StubCandidate::GetRadiusofCurvature()
{
  double curve = (_ET * _CONSTFORCURVE / _MAGFIELD) * 100.;
  return curve;
}
double StubCandidate::GetInnerRadiusCurve()
{
  double TempET = _ETErr + _ET;
  double curve = (TempET * _CONSTFORCURVE / _MAGFIELD)*100.;
  return curve;
}
double StubCandidate::GetOuterRadiusCurve()
{
  double TempET = _ET - _ETErr;
  double curve = (TempET * _CONSTFORCURVE / _MAGFIELD)*100.;
  return curve;
}

double StubCandidate::GetEt()
{
  
  double R = sqrt ( _SeedX*_SeedX + 
		    _SeedY*_SeedY + 
		    (_ZVtx-_SeedZ)*(_ZVtx-_SeedZ));
  double SinTheta = sqrt(_SeedX*_SeedX + _SeedY*_SeedY)/R;
  return _SeedE*SinTheta;
}	
double StubCandidate::GetEtErr()
{
  double R = sqrt ( _SeedX*_SeedX + 
		    _SeedY*_SeedY + 
		    (_ZVtx-_SeedZ)*(_ZVtx-_SeedZ));
  double T = sqrt( _SeedX*_SeedX + _SeedY*_SeedY);
  double EErr = (_SeedE * _ETCONSTERR * T)/R;
//   if (_SeedE < 3.5)
//     EErr = (_SeedE * 3.*_ETCONSTERR * T)/R;
  double ZErr =  (_SeedE * T  * (_ZVtx - _SeedZ) * _ZVTXCONSTERR)/(R*R*R);
  double ETERRCalc = sqrt(EErr*EErr + ZErr*ZErr);
  return ETERRCalc;
}

//Check if a hit is in the road
bool StubCandidate::IsInRoad(GlobalPoint hit, float xx_err, float yy_err, bool VERBOSE)
{
  double XPos = hit.x();
  double YPos = hit.y();
  double ZPos = hit.z();
  bool IsInZ = false;
//   if (yy_err > 10)
//     IsInZ = true;
  IsInZ = IsInZRegionOfInterest(_SeedX,_SeedY,_SeedZ,
				_ZVtx,
				XPos, YPos, ZPos, 
				xx_err, yy_err,VERBOSE);
  if (!IsInZ) return false;
  else {
    //Here's another sanity check.  
    //I want to eliminate hits that fall into the road in an unphysical region (pi away on the other
    //side of the detector).  So I'm going to require that a candidate hit can be no more than
    //pi/2. away from the conversion phi.  That's a big window, should only eliminate
    //our false hits.
    float phi_hit = atan2(YPos,XPos);
    float df = (phi_hit - _PhiConv);
    if (df > TMath::Pi()) df -=2.*TMath::Pi();
    if (df < -1.*TMath::Pi()) df +=2*TMath::Pi();
    if (fabs(df) > TMath::Pi()/2.) return false;

    //Another check for something that should never happen.

    double coordR = sqrt(XPos*XPos + YPos*YPos);
    double SeedR = sqrt(_SeedX*_SeedX + _SeedY*_SeedY);
    if (coordR > SeedR ) return false;

    //Should also be on the same side of detector:
    //Calculate R from seed to point in question. 
    float distR = sqrt((_SeedX-XPos)*(_SeedX-XPos) + (_SeedY-YPos)*(_SeedY-YPos));
    if (distR > SeedR) return false;
    
    //Calculate R from point to center of
    //circle.  If within the errorbands on curve,
    //then its in.  If within some sigma of the
    //errors on the points, it is in.  else, no.
    double PointR = sqrt( (XPos-_centerX)*(XPos-_centerX) + 
			  (YPos-_centerY)*(YPos-_centerY));

    if (PointR <_innerRadiuscurve && PointR > _outerRadiuscurve)
      return true;
    
  }
  return false;	
}

//Add hit to the stub.  Returns 0
//if successful, otherwise, Returns 1
// bool StubCandidate::AddRecHit(RecHit hit, bool VERBOSE)
// {
//   double hxpos = hit.globalPosition().x();
//   double hypos = hit.globalPosition().y();
//   double hzpos = hit.globalPosition().z();
  
//   for (int i=0;i<int(_goodhits.size());++i){
//     RecHit temp = _goodhits[i];
//     double txpos = temp.globalPosition().x();
//     double typos = temp.globalPosition().y();
//     double tzpos = temp.globalPosition().z();
//     double rdist = sqrt( (hxpos-txpos)*(hxpos-txpos)
// 			 + (hypos-typos)*(hypos-typos)
// 			 + (hzpos-tzpos)*(hzpos-tzpos) );
//     if (rdist < 0.0000001)
//       return false;
//   }

//   if (IsInRoad(hit, VERBOSE)){
//     _goodhits.push_back(hit);
//     bool has_layer=false;
//     for (int qot=0;qot<int(g_layers.size());++qot){
//       if (hit.layer()==g_layers[qot])
// 	has_layer=true;
//     }
//     if (!has_layer)
//       g_layers.push_back(hit.layer());
//     return true;
//   }
//   else return false;
//   nhit = _goodhits.size();
// }

//Is it a good stub?
// bool StubCandidate::IsGoodCandidate()
// {
//   //right now good stubs just have 3 hits
//   if (int(_goodhits.size()) > _MINSTUBHITS
//       //&& int(g_layers.size()) > _MINSTUBHITS
//       ) return true;
//   else return false;
// }

void StubCandidate::GetCenter(double &x, double &y)
{
  x = _centerX;
  y = _centerY;
  
}

StubCandidate::~StubCandidate(){
  //  _goodhits.clear();
}

double StubCandidate::Chi2XY(vector <GlobalPoint> pts)
{
  //Calculate consistency of hits with the road
  //based on the 'central value' for the curvature.
  //divide by the error 'band' about the center.
  double chi2=0;

  //  double RErr = ((_innerRadiuscurve - _outerRadiuscurve)*(_innerRadiuscurve - _outerRadiuscurve));///4.;
  for (int i=0;i<int(pts.size());++i){
    
    double XPos = pts[i].x();

    double YPos = pts[i].y();
    double PointR = sqrt( (XPos-_centerX)*(XPos-_centerX) + 
			  (YPos-_centerY)*(YPos-_centerY));
    double chi2cont = (PointR - _RadiusofCurve)*(PointR - _RadiusofCurve);
    //cout << chi2cont << endl;
    //    chi2+=chi2cont/RErr;
    chi2+=chi2cont;
  }

  if (pts.size() <=2){
    chi2 = 999999;
    chi2xy = chi2;
  }
  else{
    chi2/=(pts.size()-1.);
    chi2xy = chi2;
  }
  //cout << "chi2: " << chi2 << endl;
  //  nhit = _goodhits.size();
  return chi2;

}

// bool StubCandidate::SameRoadAs(StubCandidate stub)
// {
//   vector <RecHit> hit = stub.GetHits();
//   bool HasUnique = false;
//   for (int i=0;i<int(hit.size());++i){
//     if (!HasThisHit(hit[i])) HasUnique = true;
//   }
//   if (HasUnique || (stub.GetCharge()!=_charge)) return false;
//   return true;

// }

// bool StubCandidate::HasThisHit(RecHit hit)
// {

//   double hxpos = hit.globalPosition().x();
//   double hypos = hit.globalPosition().y();
//   double hzpos = hit.globalPosition().z();
//   for (int i=0;i<int(_goodhits.size());++i){
//     RecHit temp = _goodhits[i];
//     double txpos = temp.globalPosition().x();
//     double typos = temp.globalPosition().y();
//     double tzpos = temp.globalPosition().z();
//     double rdist = sqrt( (hxpos-txpos)*(hxpos-txpos)
// 			 + (hypos-typos)*(hypos-typos)
// 			 + (hzpos-tzpos)*(hzpos-tzpos) );
//     if (rdist < 0.0000001)
//       return true;
//   }
//   return false;
// }


// void StubCandidate::OrderHits(){
//   //sort good hits vector in increasing R

//   vector <RecHit> OrderedHit;
// //   cout << "Unordered" << endl;
// //   for (int i=0;i<int(_goodhits.size());++i){
// //     double hxpos = _goodhits[i].globalPosition().x();
// //     double hypos = _goodhits[i].globalPosition().y();
// //     double rtemp = sqrt(hxpos*hxpos + hypos*hypos);
// //     cout << "Hit: " << i << " Rval: " << rtemp << endl;
// //   }
	 
//   while (OrderedHit.size() < _goodhits.size()){
    
//     double smallestHitR=0;
//     int smallidx=-1;
    
//     for (int i=0;i<int(_goodhits.size());++i){
//       bool usedhit = false;
//       double hxpos = _goodhits[i].globalPosition().x();
//       double hypos = _goodhits[i].globalPosition().y();
//       double hzpos = _goodhits[i].globalPosition().z();
//       for (int j=0;j<int(OrderedHit.size());++j){
// 	RecHit temp = OrderedHit[j];
// 	double txpos = temp.globalPosition().x();
// 	double typos = temp.globalPosition().y();
// 	double tzpos = temp.globalPosition().z();
// 	double rdist = sqrt( (hxpos-txpos)*(hxpos-txpos)
// 			     + (hypos-typos)*(hypos-typos)
// 			     + (hzpos-tzpos)*(hzpos-tzpos) );
// 	if (rdist < 0.0000001)
// 	  usedhit = true;
//       }
//       if (!usedhit){
// 	double rtemp = sqrt(hxpos*hxpos + hypos*hypos);
// 	if (rtemp > smallestHitR){
// 	  smallestHitR = rtemp;
// 	  smallidx=i;
// 	}
//       }
//     }
//     if (smallidx !=-1)
//       OrderedHit.push_back(_goodhits[smallidx]);
//   }

//   _goodhits.clear();
//   for (int i=0;i<int(OrderedHit.size());++i){
//     _goodhits.push_back(OrderedHit[i]);
//   }

// //   cout << "Ordered" << endl;
// //   for (int i=0;i<int(_goodhits.size());++i){
// //     double hxpos = _goodhits[i].globalPosition().x();
// //     double hypos = _goodhits[i].globalPosition().y();
// //     double rtemp = sqrt(hxpos*hxpos + hypos*hypos);
// //     cout << "Hit: " << i << " Rval: " << rtemp << endl;
// //   }

// }

double StubCandidate::Chi2Hit(GlobalPoint hit)
{
  //Calculate consistency of hit with the road
  //based on the 'central value' for the curvature.
  //divide by the error 'band' about the center.
  double chi2=0;
  
  //  double RErr = ((_innerRadiuscurve - _outerRadiuscurve)*(_innerRadiuscurve - _outerRadiuscurve));//4.;
      
  double XPos = hit.x();
  double YPos = hit.y();
  double PointR = sqrt( (XPos-_centerX)*(XPos-_centerX) + 
			(YPos-_centerY)*(YPos-_centerY));
  double chi2cont = (PointR - _RadiusofCurve)*(PointR - _RadiusofCurve);
  chi2=chi2cont;
  return chi2;

}
