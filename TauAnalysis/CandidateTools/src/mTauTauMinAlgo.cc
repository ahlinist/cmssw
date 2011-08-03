#include "TauAnalysis/CandidateTools/interface/mTauTauMinAlgo.h"

#include "CLHEP/Random/RandFlat.h"

#include <cmath>

double mTauTauAtFixedKxKy(const double kx, const double ky,
			  const double se, const double sx, const double sy, const double sz,
			  const double te, const double tx, const double ty, const double tz,
			  const double pmissx, const double pmissy,
			  const double mtau,
			  bool& wasSillyRef)
{
  wasSillyRef = true;

  const double mtausq = mtau*mtau;
  const double mssq = se*se-sx*sx-sy*sy-sz*sz;
  const double mtsq = te*te-tx*tx-ty*ty-tz*tz;
  
  const double pkx1 = pmissx + kx;
  const double pky1 = pmissy + ky;
  const double pkx2 = pmissx - kx;
  const double pky2 = pmissy - ky;
  
  const double star1 = mtausq - mssq + sx*pkx1+sy*pky1;
  const double star2 = mtausq - mtsq + tx*pkx2+ty*pky2;
  
  const double pk1sq = pkx1*pkx1 + pky1*pky1;
  const double pk2sq = pkx2*pkx2 + pky2*pky2;
  
  const double etssq = se*se-sz*sz;
  const double ettsq = te*te-tz*tz;
  
  bool good = true;
  
  const double disc1 = star1*star1 - etssq*pk1sq;
  const double disc2 = star2*star2 - ettsq*pk2sq;
  
  if ( disc1<0 || disc2<0 ) {
    good = false;
  }
  
  const double pz_cat = (star1*sz + se*sqrt(disc1))/(2*etssq);
  const double pz_dog = (star1*sz - se*sqrt(disc1))/(2*etssq);
  const double qz_cat = (star2*tz + te*sqrt(disc2))/(2*ettsq);
  const double qz_dog = (star2*tz - te*sqrt(disc2))/(2*ettsq);
  
  const double px = 0.5*(pmissx + kx);
  const double py = 0.5*(pmissy + ky);
  const double qx = 0.5*(pmissx - kx);
  const double qy = 0.5*(pmissy - ky);
  
  const double pesq_cat = px*px + py*py + pz_cat*pz_cat;
  const double pesq_dog = px*px + py*py + pz_dog*pz_dog;
  const double qesq_cat = qx*qx + qy*qy + qz_cat*qz_cat;
  const double qesq_dog = qx*qx + qy*qy + qz_dog*qz_dog;

  const double pe_cat = sqrt(pesq_cat);
  const double pe_dog = sqrt(pesq_dog);
  const double qe_cat = sqrt(qesq_cat);
  const double qe_dog = sqrt(qesq_dog);
  
  const double mhAAAsq = 2*mtausq + 2*(
    (pe_cat + se)*(qe_cat + te)
   - (px + sx)*(qx + tx)
   - (py + sy)*(qy + ty)
   - (pz_cat + sz)*(qz_cat + tz)
  );
  const bool AAAgood = (pesq_cat >= 0 && qesq_cat >= 0);

  const double mhBBBsq = 2*mtausq + 2*(
    (pe_cat + se)*(qe_dog + te)
   - (px + sx)*(qx + tx)
   - (py + sy)*(qy + ty)
   - (pz_cat + sz)*(qz_dog + tz)
  );
  const bool BBBgood = (pesq_cat >= 0 && qesq_dog >= 0);

  const double mhCCCsq = 2*mtausq + 2*(
    (pe_dog + se)*(qe_cat + te)
   - (px + sx)*(qx + tx)
   - (py + sy)*(qy + ty)
   - (pz_dog + sz)*(qz_cat + tz)
  );
  const bool CCCgood = (pesq_dog >= 0 && qesq_cat >= 0);

  const double mhDDDsq = 2*mtausq + 2*(
    (pe_dog + se)*(qe_dog + te)
   - (px + sx)*(qx + tx)
   - (py + sy)*(qy + ty)
   - (pz_dog + sz)*(qz_dog + tz)
  );
  const bool DDDgood = (pesq_dog >= 0 && qesq_dog >= 0);

  if ( !good ) {
    double power = 1; // power to raise the -ve discriminant by in attemt to get out of bad region
    if( disc1 < 0 && disc2 >= 0 ) {
      return pow(-disc1, power) + 14000;
    } else if ( disc2 < 0 && disc1 >= 0 ) {
      return pow(-disc2, power) + 14000;
    } else {
      return pow(-disc1, power) + pow(-disc2, power) + 14000;
    }
  }
  
  if ( (!AAAgood) && (!BBBgood) && (!CCCgood) && (!DDDgood) ) {
    return -2;
  }

  double mhsq;
  bool set = false;

  if ( AAAgood && (!set || mhAAAsq < mhsq) ) {
    set = true;
    mhsq = mhAAAsq;
  }
  if ( BBBgood && (!set || mhBBBsq < mhsq) ) {
    set = true;
    mhsq = mhBBBsq;
  }
  if ( CCCgood && (!set || mhCCCsq < mhsq) ) {
    set = true;
    mhsq = mhCCCsq;
  }
  if ( DDDgood && (!set || mhDDDsq < mhsq) ) {
    set = true;
    mhsq = mhDDDsq;
  }

  if ( !set ) {
    return -3;
  }

  if ( mhsq <0 ) {
    return -sqrt(-mhsq);
  }

  wasSillyRef = false;

  return sqrt(mhsq);
}

//
//-------------------------------------------------------------------------------
//

double mTauTauMin(const double se, const double sx, const double sy, const double sz,
		  const double te, const double tx, const double ty, const double tz,
		  const double pmissx, const double pmissy,
		  const double mtau) 
{
  double kxStart = 0;
  double kyStart = 0;
  double bestHMassSoFar;

  bool haveValidStartPoint=false;
  bool bestPointWasSilly;

  // Attempt to get valid start point

  const double distFromWall = 2; // scan size = order of magnitude spread over which cauchy vals will be distributed.
    
  for ( int i=0; i < 10000; ++i ) {
    const double theta = (CLHEP::RandFlat::shoot()-0.5)*3.14159;
    const double distToStep = distFromWall*tan(theta); 
    const double angToStep = CLHEP::RandFlat::shoot()*3.14159*2.0;
    const double kx = distToStep * cos(angToStep);
    const double ky = distToStep * sin(angToStep);
    bool wasSilly;

    const double possHMass =
      mTauTauAtFixedKxKy(kx,ky,
			 se,sx,sy,sz,
			 te,tx,ty,tz,
			 pmissx,pmissy,
			 mtau,
			 wasSilly);
    if ( (possHMass >= 0 && !haveValidStartPoint) ||  
	 (possHMass >= 0 &&  haveValidStartPoint && possHMass < bestHMassSoFar) ) {
      bestHMassSoFar = possHMass;
      haveValidStartPoint = true;
      bestPointWasSilly = wasSilly;
      kxStart = kx;
      kyStart = ky;
      if ( !wasSilly ) {
	// Don't need to work any harder ...
	break;
      }
    }
  }

  if ( haveValidStartPoint ) {
    // Now we can attempt to minimise this function.
      
    double kxOld = kxStart;
    double kyOld = kyStart;
    double oldH = bestHMassSoFar;

    double typicalStepSize = distFromWall;
  
    // for arXiv and paper were 0.99 and 1.1
    //const double shrinkageFactor = 0.99;
    //const double growthFactor = 1.1;

    const double shrinkageFactor = 0.999;
    const double growthFactor = 2.0;
    
    while ( typicalStepSize > 1.e-6 ) {
      const double theta = (CLHEP::RandFlat::shoot()-0.5)*3.14159;
      const double distToStep = typicalStepSize*tan(theta); 
      const double angToStep = CLHEP::RandFlat::shoot()*3.14159*2.0001;
      const double newkx = kxOld + distToStep * cos(angToStep);
      const double newky = kyOld + distToStep * sin(angToStep);
      bool wasSilly;
 
      const double possHMass =
	mTauTauAtFixedKxKy(newkx,newky,
			   se,sx,sy,sz,
			   te,tx,ty,tz,
			   pmissx,pmissy,
			   mtau,
			   wasSilly);
      if ( possHMass>=0 && possHMass < bestHMassSoFar) {
	bestHMassSoFar = possHMass;
	bestPointWasSilly = wasSilly;
	kxOld = newkx;
	kyOld = newky;
	typicalStepSize *= growthFactor;
      } // if point is an improvement
      else {
	typicalStepSize *= shrinkageFactor;
      } // point was not an improvement
    } // while we wantto keep going
    if ( bestPointWasSilly ) {
      return -10;
    } else {
      return bestHMassSoFar;
    }
  } // have valid start
  else {
    return -15;
  } // have invalid start
}

