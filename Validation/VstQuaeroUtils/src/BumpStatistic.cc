/******************************************
Bump Statistic
Georgios Choudalakis, September 2006
******************************************/

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/BumpStatistic.hh"
using namespace std;

void readDebugOptionFromEnvironment(bool& debug, bool& minimalDebug) {
  debug = false;
  minimalDebug = false;
  if ( getenv("bumpDebug") != NULL ) {
    string string_bumpDebug=getenv("bumpDebug");
    if (string_bumpDebug == "1" || string_bumpDebug == "true" ) 
      debug = true;
    if (string_bumpDebug == "2")
      minimalDebug = true;
  }
}

double readMinutesAllowedFromEnvironment() {
  double ans=5;
  if ( getenv("bumpMinutes") != NULL ) {
    ans=atof(getenv("bumpMinutes"));
  }
  return(ans);
}


double pval(int d, double b, double deltab=0, bool accuratePval=true) {
  if (accuratePval) {
    //    double fasterWithAdjustableTrapezoid=Math::accurateProbOfThisEffect(d,b,deltab,">="); //default will be adjustTrapezoidSize=true, which is faster, but less accurate
    //    double slowerWithFixedTrapezoid=Math::accurateProbOfThisEffect(d,b,deltab,">=",false); //adjustableTrapezoid=false means fixed trapezoidSize=1, which is the highest accuracy
    bool useAdjustableTrapezoid=true; //if true, it will gain speed, but loose some accuracy (still will be sooo much better than probOfThisEffect())
    if( d < 30  ) useAdjustableTrapezoid=false; //we can afford the extra accuracy 
    double ans = Math::accurateProbOfThisEffect(d,b,deltab,">=",useAdjustableTrapezoid); 
    return (ans);
  }
  else
    return Math::probOfThisEffect(b,d,deltab,">=");
}



//default constructor
BumpStatistic::BumpStatistic() {}
  
//constructor
BumpStatistic::BumpStatistic(const string& _finalState, const string& _massVariable, const vector<double>& _bkgpts, const vector<double>& _bkgwt, const vector<double>& _datapts, int _dataOrPseudoDataMode, int pseudoDataToProduce) {
  //dataOrPseudoDataMode : 1=use data given as _datapts, which is the default.
  // 2=ignore _datapts and use instead pseudo-data, pulled from the background in this mass variable.
  // 3=produce pseudo-data, and on top of that pseudo-data add _datapts, assuming it's pseudo-signal.
  // If dataOrPseudoDataMode=3, then we can optionally set pseudoDataToProduce to something other than the default (-1).  In that case, we fix from outside the number of pseudo-data to add.  Otherwise (with -1) it will pull a number from Poisson.
  dataOrPseudoDataMode=_dataOrPseudoDataMode;

  debug=false;
  minimalDebug=false;
  readDebugOptionFromEnvironment(debug,minimalDebug);
  debug_store = debug;

  finalState=_finalState;
  massVariable=_massVariable;

  bkgpts = _bkgpts;
  bkgwt = _bkgwt;
  assert(bkgpts.size()==bkgwt.size());

  lowestMassToConsider=0;
  highestMassToConsider=std::numeric_limits<double>::max();

  sortBackgroundVectors();
  smallestBkgPosition=bkgpts[0];
  biggestBkgPosition =bkgpts[bkgpts.size()-1];

  sidebandsSize = 1; //if 1, each sideband is as large as the central window. If 2, twice as large, etc.
  widthStep = 5; //GeV/c^2 : If we scan with width ranging from A to B, what should the increment of width be between?
  positionStep = 5; //GeV/c^2 : If we scan from A to B, what should the increment of position be between?
  useWidthOptimization = false; //If true, then it will use as width the result of ::optimizeWidth when it scans.
  if (getenv("bumpUseWidthOptimization")!=NULL)
    if(atoi(getenv("bumpUseWidthOptimization"))==1)
      useWidthOptimization=true;

  // In rare cases the background may be very narrow.
  if ( (biggestBkgPosition - smallestBkgPosition) < positionStep && (biggestBkgPosition - smallestBkgPosition) != 0 )
    positionStep = (biggestBkgPosition - smallestBkgPosition)/100;
  
  
  if (dataOrPseudoDataMode==1)
    datapts = _datapts;
  else if (dataOrPseudoDataMode==2) {
    //replace data with pseudo-data, to test expected distribution of bump significances
    pseudoDataProductionHasScannedBkgAlready = false;
    datapts = producePseudoData(bkgpts, bkgwt);
    pseudoDataProductionHasScannedBkgAlready = false;
  }
  else if (dataOrPseudoDataMode==3) {
    datapts=_datapts; //take first the pseudo-signal given to us
    //and add to it the following pseudo-data:
    pseudoDataProductionHasScannedBkgAlready = false;
    vector<double> tmp_datapts = producePseudoData(bkgpts, bkgwt, pseudoDataToProduce);
    pseudoDataProductionHasScannedBkgAlready = false;
    //do the adding now:
    for (size_t i=0; i<tmp_datapts.size(); ++i)
      datapts.push_back(tmp_datapts[i]);
  }

  //instead of sorting data here, do that when the user calls findBumps. The reason is that in this way, when running in dataOrPseudoDataMode=3, we return with getData() a vector that has at the beginning the pseudo-signal events passed, and the pseudo-data is following that in the vector.
  //  sort(datapts.begin(),datapts.end());

  if(minimalDebug && 0) {
    cout << "\nSorted data vector:\n " <<endl;
    for ( size_t i=0; i<datapts.size(); ++i) {
      printf("%.1f ",bkgpts[i]);
      if (i >= 10) break;
    }
    cout << endl;
  }

  minDataInWindow = 5.; 
  bumpynessConstantThreshold = 0.01; // the smaller the harder it is for a bump candidate to meet the shape criterion, i.e. to be considered as looking bumpy.
  weRequireObjectivelyGoodSidebands = true; // will be used when deciding quality criteria for bump candidates.

  //save time by not reading the background 100s of times to produce 100s of pseudo-data sets.
  pseudoDataProductionHasScannedBkgAlready = false;

  Neff=-1; //we will make it a positive value if we need it, and then we will remember it and not calculate it again and again.  The definition is probability=1-(1-pvalmin)^Neff.

  scannedForBumpsAlready = false; // will become true if someone calls findBumps().
  scannedForJustDataAlready = false; // will become true if someone calls findBumpsJustData().
  
  return;
}

void BumpStatistic::findBumps() {
  //This will start the scanning for interesting bumps.  The constructor will define some significant constants, and then the user will call findBumps() to do the scanning.  If the user forgets to do that and goes directly to something like getLimits() or probability() which assumes we know which one the most interesting bumps is, we will run findBumps() for him.
  if(scannedForBumpsAlready)
    return;
  //otherwise go on
  sort(datapts.begin(),datapts.end()); //this is essential.  It used to be in the constructor.
  allBumps = getAllBumps(bkgpts, bkgwt, datapts); 
  mostInterestingBump = findMostInterestingBump(allBumps);
  if(debug||minimalDebug) printf ("From the actual data: [%.1f-%.1f-%.1f], d=%.0f, b=%.2f +- %.2f, Pvalue=%g \n\n", mostInterestingBump.windowStart, mostInterestingBump.windowCenter, mostInterestingBump.windowEnd, mostInterestingBump.data, mostInterestingBump.bkg, mostInterestingBump.deltaBkg, mostInterestingBump.pval);  
  scannedForBumpsAlready=true;
  return;
}


void BumpStatistic::setBoundaries(double lower, double upper){
  //These boundaries will be used to cut any data and/or background events that have mass outside of those boundaries.  They will be chopped and the search for bumps will be limitted between the lower and upper limits.
  assert(lower<upper);
  lowestMassToConsider=lower;
  highestMassToConsider=upper;
  return; 
}


bool sortIncreasingPt(const pair<double,int>& p1, const pair<double,int>& p2)
{
  return (p1.first < p2.first);
}

bool sortIncreasingWindowCenterPosition(const BumpStatistic::BumpCandidateJustData& bc1,const BumpStatistic::BumpCandidateJustData& bc2)
{
  return ( (bc1.windowStart+bc1.windowEnd) < (bc2.windowStart+bc2.windowEnd) ); //center=(start+end)/2, but the /2 is unnecessary for the comparison.
}


void BumpStatistic::sortBackgroundVectors(){
  //Sort by ascending position, keeping of course the right weight for each event.
  if(debug) cout << "Sorting background for speed." << endl;
  if(debug) {
    cout << "\nUnsorted background vector:\n " <<endl;
    for ( size_t i=0; i<bkgpts.size(); ++i) {
      printf("(%.1f,%.1f) ",bkgpts[i],bkgwt[i]);
      if (i >= 50) break;
    }
    cout << endl;
  }
  vector<pair<double,int> > vecPtIndex;
  for (size_t i=0; i < bkgpts.size() ; ++i)
    vecPtIndex.push_back(pair<double,int>(bkgpts[i],i));
  sort(vecPtIndex.begin(), vecPtIndex.end(), sortIncreasingPt);
  vector<double> sortedBkgwt;
  for (size_t i=0; i < vecPtIndex.size(); ++i)
    sortedBkgwt.push_back(bkgwt[vecPtIndex[i].second]);
  bkgwt = sortedBkgwt;
  sort(bkgpts.begin(),bkgpts.end());
  if(debug) {
    cout << "\nSorted background vector:\n " <<endl;
    for ( size_t i=0; i<bkgpts.size(); ++i) {
      printf("(%.1f,%.1f) ",bkgpts[i],bkgwt[i]);
      if (i >= 10) break;
    }
    cout << endl;
  }
}


pair<double,double> BumpStatistic::fromWhereToWhereToScan(double smallestPosition, double biggestPosition) {
  // given the range of data [smallestPosition,biggestPosition], decide from where to where to attempt to center the bump windows, so as to have always left the desired space on the left and right of the window for the sidebands to fit.
  pair<double,double> ans(0,0);
  if (smallestPosition < lowestMassToConsider) smallestPosition=lowestMassToConsider; //masses are always positive for example.  Generally, we don't consider masses below the lowestMassToConsider.
  if (biggestPosition > highestMassToConsider) biggestPosition=highestMassToConsider; //We don't consider masses above highestMassToConsider (infinity by default).
  if (smallestPosition == biggestPosition)
    return (pair<double,double>(smallestPosition,biggestPosition));

  double position=smallestPosition; //center
  double biggestWidth=fromHowToHowWide(position).second; 
  double sidebandEdge=position-(0.5+sidebandsSize)*biggestWidth; //half a window + left sideband to the left
  while(sidebandEdge < smallestPosition) {
    //    if(debug) cout << "position= " << position << " sidebandEdge= " << sidebandEdge << endl;
    position += positionStep; // slide center to the right
    biggestWidth=fromHowToHowWide(position).second;
    sidebandEdge=position-(0.5+sidebandsSize)*biggestWidth; 
  }
  ans.first=position;
  if(debug) cout << "Scan from " << ans.first << " which on the left reaches down to " << sidebandEdge ;

  position=biggestPosition; //center
  biggestWidth=fromHowToHowWide(position).second;
  sidebandEdge=position+(0.5+sidebandsSize)*biggestWidth; //half a window + left sideband to the right
  while(sidebandEdge > biggestPosition) {
    //    if(debug) cout << "position= " << position << " sidebandEdge= " << sidebandEdge << endl;
    position -= positionStep; // slide center to the left
    biggestWidth=fromHowToHowWide(position).second;
    sidebandEdge=position+(0.5+sidebandsSize)*biggestWidth; 
  }
  ans.second=position;
  if(debug) cout << " up to " << ans.second << " which on the right reaches " << sidebandEdge << endl;
  
  if(debug) {
    cout << "Scan from: " << ans.first << " to: " << ans.second << " ";
    if(ans.first > ans.second)
      cout << "Notice this means don't scan." ;
    cout << endl;
  } 
  return(ans);
}


pair<double,double> BumpStatistic::fromHowToHowWide(const double& position){
  // given the position where the bump is (=center), decide the minimum and the maximum width that bump candidates centered on that position can have. 
  pair<double,double> ans(0,0);
  double sigmaM=massResolution(position);
  ans.first =2.0 * sigmaM;
  ans.second=2.0 * sigmaM;
  return(ans);
}


void BumpStatistic::optimizeWidth(BumpStatistic::BumpCandidate& bc, const vector<double>& _bkgpts, const vector<double>& _bkgwt) {
  if ( bc.windowSize!=0 ) return; //it has already been optimized
  //else, optimize it.
  if(debug) cout << "Optimizing width." << endl;
  double deltaM=massResolution(bc.windowCenter);
  double width=2*deltaM;; //if width is 2*deltaM it contains 68% of the signal
  double signalFraction = erf(width/(sqrt(2)*2*deltaM)); //integral from -width/2 to +width/2 around mean, with sigma=deltaM
  double backgroundContained=0;
  double deltaBackgroundContained=0;
  readWindowCentral(_bkgpts,_bkgwt,bc.windowCenter-width/2,bc.windowCenter+width/2,backgroundContained,deltaBackgroundContained);
  double expectedSignificance=signalFraction/(sqrt(backgroundContained+deltaBackgroundContained*deltaBackgroundContained)+1e-6); //the + 1e-6 is to avoid division by 0.  It doesn't matter when we maximize expectedSignificance
  //find the first size that doesn't have 0 background
  while (backgroundContained==0 && width<4*deltaM) {
    width += 0.1*deltaM;
    readWindowCentral(_bkgpts,_bkgwt,bc.windowCenter-width/2,bc.windowCenter+width/2,backgroundContained,deltaBackgroundContained);
    expectedSignificance=signalFraction/(sqrt(backgroundContained+deltaBackgroundContained*deltaBackgroundContained)+1e-6);
  }
  //find the optimal
  double optimalSignificance=expectedSignificance;
  double optimalWidth=width;
  while (width<4*deltaM) {
    signalFraction=erf(width/(sqrt(2)*2*deltaM));
    readWindowCentral(_bkgpts,_bkgwt,bc.windowCenter-width/2,bc.windowCenter+width/2,backgroundContained, deltaBackgroundContained);
    expectedSignificance=signalFraction/(sqrt(backgroundContained+deltaBackgroundContained*deltaBackgroundContained)+1e-6);
    
    if(debug) printf("width=%f S/dB=%f/%f=%f",width,signalFraction,sqrt(backgroundContained+deltaBackgroundContained*deltaBackgroundContained),expectedSignificance);
    if(expectedSignificance >= optimalSignificance && backgroundContained > 0) {
      optimalWidth=width;
      optimalSignificance=expectedSignificance;
      if(debug) printf(" <-- ");
    }
    if(debug) printf("\n");
    width += 0.1*deltaM;
  }
  if(debug) cout << "Optimal width= " << optimalWidth << endl;
  bc.windowSize=optimalWidth; //store the result into the bc object
  return;
}



double BumpStatistic::countEntries(const vector<double>& points, const vector<double>& weights, const double& windowStart, const double& windowEnd) {
  // simply count total weight between windowStart and windowEnd
  // assume (and assert) that the points are sorted ascending!
  if ( points.empty() || weights.empty() ) {
    return (0);
  }
  size_t initialIndex=0;
  size_t skipSize = max(10,(int) points.size()/100 );
  //  if(debug) cout << "skipSize= " << skipSize << " ";
  while ( (initialIndex + skipSize) < points.size() ) {
    //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
    assert(points[initialIndex+skipSize] >= points[initialIndex]);
    if (points[initialIndex+skipSize] < windowStart) 
      initialIndex += skipSize;
    else
      break;
  }
  double total=0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit vector being ordered to save time
    if ( points[i] < windowStart ) continue; 
    if ( points[i] >= windowEnd ) break;
    if ( windowStart <= points[i] && points[i] < windowEnd )
      total += weights[i];
  }
  return(total);
  
}


pair<double,double> BumpStatistic::countBkgAndDeltaBkg(const vector<double>& points, const vector<double>& weights, const double& windowStart, const double& windowEnd) {
  // simply count total weight between windowStart and windowEnd
  // return a pair where the first element is the total bkg and the second is the statistical uncertainty sqrt(sum w_i^2).
  // assume (and assert) that the points are sorted ascending!
  if ( points.empty() || weights.empty() ) {
    return (pair<double,double>(0,0));
  }
  size_t initialIndex=0;
  size_t skipSize = max(10,(int)points.size()/100 );
  //  if(debug) cout << "skipSize= " << skipSize << " ";
  while ( (initialIndex + skipSize) < points.size() ) {
    //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
    assert(points[initialIndex+skipSize] >= points[initialIndex]);
    if (points[initialIndex+skipSize] < windowStart) 
      initialIndex += skipSize;
    else
      break;
  }
  double total=0;
  double total2=0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit vector being ordered to save time
    if ( points[i] < windowStart ) continue; 
    if ( points[i] >= windowEnd ) break;
    if ( windowStart <= points[i] && points[i] < windowEnd ) {
      total += weights[i];
      total2 += weights[i]*weights[i];
    }
  }
  pair<double,double> ans(total, sqrt(total2));
  return(ans);
}



void BumpStatistic::readWindowAndSidebands(const vector<double>& points, const double& leftBandStart, const double& windowStart, const double& windowEnd, const double& rightBandEnd, double& totalLeft, double& total, double& totalRight) {
  //scan the points vector (for example datapts) and count how many events are in each sideband and in the center.
  // assume (and assert) that the points are sorted ascending!
  if ( points.empty() ) {
    totalLeft = total = totalRight = 0;
    return;
  }
  size_t initialIndex=0;
  for(size_t skipSize = max(2,(int)points.size()/2); skipSize >= 2; skipSize /= 2) {
    //  if(debug) cout << "skipSize= " << skipSize << " ";
    while ( (initialIndex + skipSize) < points.size() ) {
      //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
      assert(points[initialIndex+skipSize] >= points[initialIndex]);
      if (points[initialIndex+skipSize] < leftBandStart) 
	initialIndex += skipSize;
      else
	break;
    }
  }
  totalLeft=totalRight=total=0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit vector being ordered to save time
    if ( points[i] < leftBandStart ) continue; 
    if ( points[i] >= rightBandEnd ) break;
    if (leftBandStart <= points[i] && points[i] < windowStart )
      totalLeft += 1;
    else if ( windowStart <= points[i] && points[i] < windowEnd )
      total += 1;
    else if (windowEnd <= points[i] && points[i] <  rightBandEnd)
      totalRight += 1;
  }
}



void BumpStatistic::readWindowAndSidebands(const std::vector<double>& points, const std::vector<double>& weights, const double& leftBandStart, const double& windowStart, const double& windowEnd, const double& rightBandEnd, double& totalInWindow, double& deltaInWindow, double& totalLeft, double& deltaLeft, double& totalRight, double& deltaRight) {
  //scan the points vector (for example bkgpts) and count how much weight is in each sideband and in the center.
  // assume (and assert) that the points are sorted in ascending mass (not weight of course).
  if (points.empty() || weights.empty()) {
    totalInWindow = deltaInWindow = totalLeft = deltaLeft = totalRight = deltaRight = 0;
    return;
  }
  assert(points.size() == weights.size());

  if (points[points.size()-1] < leftBandStart) {
    // there is no background so far right.
    totalInWindow = deltaInWindow = totalLeft = deltaLeft = totalRight = deltaRight = 0;
    return;
  }
  
  size_t initialIndex=0;
  for(size_t skipSize = max(2,(int)points.size()/2); skipSize >= 2; skipSize /= 2) {
    //  if(debug) cout << "skipSize= " << skipSize << " ";
    while ( (initialIndex + skipSize) < points.size() ) {
      //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
      assert(points[initialIndex+skipSize] >= points[initialIndex]);
      if (points[initialIndex+skipSize] < leftBandStart) 
	initialIndex += skipSize;
      else
	break;
    }
  }
  totalLeft=deltaLeft=totalRight=deltaRight=totalInWindow=deltaInWindow=0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit ordered to save time
    if ( points[i] < leftBandStart ) continue; 
    if ( points[i] >= rightBandEnd ) break;
    if (leftBandStart <= points[i] && points[i] < windowStart ) {
      totalLeft += weights[i];
      deltaLeft += weights[i] * weights[i];
    }
    else if ( windowStart <= points[i] && points[i] < windowEnd ) {
      totalInWindow += weights[i];
      deltaInWindow += weights[i] * weights[i];
    }
    else if (windowEnd <= points[i] && points[i] < rightBandEnd ) {
      totalRight += weights[i];
      deltaRight += weights[i] * weights[i];
    }
  }
  // w_eff * Sqrt(N_eff), where Neff=bkg/w_eff, and w_eff=Sum((w_i / bkg) * w_i). 
  // The result is deltaBkg=Sqrt(Sum(w_i^2))
  deltaLeft = sqrt(deltaLeft);
  deltaInWindow = sqrt(deltaInWindow); 
  deltaRight = sqrt(deltaRight);
  return;
}


void BumpStatistic::readWindowCentral(const std::vector<double>& points, const std::vector<double>& weights, const double& windowStart, const double& windowEnd, double& totalInWindow, double& deltaInWindow) {
  //scan the points vector (for example bkgpts) and count how much weight is in the center.
  // assume (and assert) that the points are sorted in ascending mass (not weight of course).
  if (points.empty() || weights.empty()) {
    totalInWindow = deltaInWindow = 0;
    return;
  }
  assert(points.size() == weights.size());

  if (points[points.size()-1] < windowStart) {
    // there is no background so far right.
    totalInWindow = deltaInWindow = 0;
    return;
  }
  
  size_t initialIndex=0;
  for(size_t skipSize = max(2,(int)points.size()/2); skipSize >= 2; skipSize /= 2) {
    //  if(debug) cout << "skipSize= " << skipSize << " ";
    while ( (initialIndex + skipSize) < points.size() ) {
      //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
      assert(points[initialIndex+skipSize] >= points[initialIndex]);
      if (points[initialIndex+skipSize] < windowStart) 
	initialIndex += skipSize;
      else
	break;
    }
  }
  totalInWindow=deltaInWindow=0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit ordered to save time
    if ( points[i] < windowStart ) continue; 
    if ( points[i] >= windowEnd ) break;
    else if ( windowStart <= points[i] && points[i] < windowEnd ) {
      totalInWindow += weights[i];
      deltaInWindow += weights[i] * weights[i];
    }
  }
  // w_eff * Sqrt(N_eff), where Neff=bkg/w_eff, and w_eff=Sum((w_i / bkg) * w_i). 
  // The result is deltaBkg=Sqrt(Sum(w_i^2))
  deltaInWindow = sqrt(deltaInWindow); 
  return;
}



void BumpStatistic::readWindowJustData(const vector<double>& points, BumpCandidateJustData& bc) {
  //this is for the "just data" case.  It will calculate the interpolated total at the center, using the information in sidebands, and using also some rough shape information.
  // assume (and assert) that the points are sorted ascending!
  if ( points.empty() ) {
    bc.dataLeft = bc.data = bc.dataRight = bc.dataExpected = bc.deltaDataExpected = 0;
    bc.dataLeftLeftHalf = bc.dataLeftRightHalf = bc.dataRightLeftHalf = bc.dataRightRightHalf = 0;
    bc.pval=1;
    bc.qualifies=false;
    return;
  }
  size_t initialIndex=0;
  for(size_t skipSize = max(2,(int)points.size()/2); skipSize >= 2; skipSize /= 2) {
    //  if(debug) cout << "skipSize= " << skipSize << " ";
    while ( (initialIndex + skipSize) < points.size() ) {
      //	if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
      assert(points[initialIndex+skipSize] >= points[initialIndex]);
      if (points[initialIndex+skipSize] < bc.leftBandStart) 
	initialIndex += skipSize;
      else
	break;
    }
  }
  bc.data = bc.dataLeftLeftHalf = bc.dataLeftRightHalf = bc.dataRightLeftHalf = bc.dataRightRightHalf = 0;
  bc.averagePositionLeftLeft = bc.averagePositionLeftRight = bc.averagePositionRightLeft = bc.averagePositionRightRight = 0;
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit vector being ordered to save time
    if ( points[i] < bc.leftBandStart ) continue; 
    if ( points[i] >= bc.rightBandEnd ) break;
    //we split each sideband in two halves, in case we want to use some shape information.  
    if (bc.leftBandStart <= points[i] && points[i] < bc.leftBandSeparation ) {
      bc.dataLeftLeftHalf += 1;
      bc.averagePositionLeftLeft += points[i];
    }
    else if (bc.leftBandSeparation <= points[i] && points[i] < bc.windowStart ) {
      bc.dataLeftRightHalf += 1;
      bc.averagePositionLeftRight += points[i];
    }
    else if ( bc.windowStart <= points[i] && points[i] < bc.windowEnd ) {
      bc.data += 1;
    }
    else if (bc.windowEnd <= points[i] && points[i] < bc.rightBandSeparation) {
      bc.dataRightLeftHalf += 1;
      bc.averagePositionRightLeft += points[i];
    }
    else if (bc.rightBandSeparation <= points[i] && points[i] < bc.rightBandEnd) {
      bc.dataRightRightHalf += 1;
      bc.averagePositionRightRight += points[i];
    }
  }
  (bc.dataLeftLeftHalf==0) ? bc.averagePositionLeftLeft=(bc.leftBandStart+bc.leftBandSeparation)/2 : bc.averagePositionLeftLeft /= bc.dataLeftLeftHalf;
  (bc.dataLeftRightHalf==0) ? bc.averagePositionLeftRight=(bc.leftBandSeparation+bc.windowStart)/2 : bc.averagePositionLeftRight /= bc.dataLeftRightHalf;
  (bc.dataRightLeftHalf==0) ? bc.averagePositionRightLeft=(bc.windowEnd+bc.rightBandSeparation)/2 : bc.averagePositionRightLeft  /= bc.dataRightLeftHalf;
  (bc.dataRightRightHalf==0)? bc.averagePositionRightRight=(bc.rightBandSeparation+bc.rightBandEnd)/2 : bc.averagePositionRightRight /= bc.dataRightRightHalf;
  bc.dataLeft=bc.dataLeftLeftHalf + bc.dataLeftRightHalf;
  bc.dataRight=bc.dataRightLeftHalf + bc.dataRightRightHalf;
  (bc.dataLeft == 0) ? bc.averagePositionLeft=(bc.leftBandStart+bc.windowStart)/2 : bc.averagePositionLeft=(bc.averagePositionLeftLeft*bc.dataLeftLeftHalf + bc.averagePositionLeftRight*bc.dataLeftRightHalf)/(bc.dataLeft);
  (bc.dataRight == 0) ? bc.averagePositionRight=(bc.rightBandEnd+bc.windowEnd)/2 : bc.averagePositionRight=(bc.averagePositionRightLeft*bc.dataRightLeftHalf + bc.averagePositionRightRight*bc.dataRightRightHalf)/(bc.dataRight);

  return;
}


void BumpStatistic::interpolateDataFromSidebands(BumpCandidateJustData& bc) {
  double dataLeftLeftHalf=bc.dataLeftLeftHalf;
  double dataLeftRightHalf=bc.dataLeftRightHalf;
  double dataRightLeftHalf=bc.dataRightLeftHalf;
  double dataRightRightHalf=bc.dataRightRightHalf;
  double dataLeft=bc.dataLeft;
  double dataRight=bc.dataRight;
  double rightBandEnd=bc.rightBandEnd;
  double rightBandSeparation=bc.rightBandSeparation;
  double leftBandSeparation=bc.leftBandSeparation;
  double leftBandStart=bc.leftBandStart;
  double windowStart=bc.windowStart;
  double windowEnd=bc.windowEnd;
  double averagePositionLeftLeft=bc.averagePositionLeftLeft;
  double averagePositionLeftRight=bc.averagePositionLeftRight;
  double averagePositionRightLeft=bc.averagePositionRightLeft;
  double averagePositionRightRight=bc.averagePositionRightRight;
  double averagePositionLeft=bc.averagePositionLeft;
  double averagePositionRight=bc.averagePositionRight;

  bc.pvalue_oneLineInterpolation=1;
  bc.pvalue_twoLineInterpolation=1;

  { //do the simple interpolation first
    //interpolate simply between left and right sidebnads.  No use of shape information within each sideband.
    //    if(debug) cout << "Searching with one-line interpolation." << endl<<flush;
    double dL = dataLeft / (windowStart - leftBandStart);
    double dR = dataRight / (rightBandEnd - windowEnd);
    double delta_dL = sqrt(dataLeft) / (windowStart - leftBandStart);
    double delta_dR = sqrt(dataRight) / (rightBandEnd - windowEnd);
    double A=averagePositionLeft;
    double B=averagePositionRight;
    bc.dataExpected_oneLineInterpolation = (dL + (windowStart-A)*(dR-dL)/(B-A) + dL + (windowEnd-A)*(dR-dL)/(B-A))*(windowEnd-windowStart)/2;
    bc.deltaDataExpected_oneLineInterpolation = sqrt(pow((-2*B + windowEnd + windowStart)/(A-B)*delta_dL,2)+
						     pow(-(-2*A + windowEnd + windowStart)/(A-B)*delta_dR,2)
						     );
    if(bc.deltaDataExpected < 1) bc.deltaDataExpected += 1;
    if(bc.dataExpected <= 0) bc.dataExpected = 1;
    bc.pvalue_oneLineInterpolation=pval((int)bc.data,bc.dataExpected_oneLineInterpolation,bc.deltaDataExpected_oneLineInterpolation,true);
    if(debug) cout << " (obs= " << bc.data << " exp.= " << bc.dataExpected_oneLineInterpolation << " +- " << bc.deltaDataExpected_oneLineInterpolation << " , pvalue1= " << bc.pvalue_oneLineInterpolation << endl;
  }//end of simple interpolation
  {//Now do the double line interpolation
    //interpolate using two points from each sideband.  Each sideband extrapolates into half of the central window with a straight line.
    //    if(debug) cout << "Searching with two-line interpolation." << flush;
    double A = averagePositionLeftLeft;
    double B = averagePositionLeftRight;
    double C = averagePositionRightLeft;
    double D = averagePositionRightRight;
    double WC=(windowStart+windowEnd)/2;
    double WE=windowEnd;
    double WS=windowStart;
    
    double dLL = dataLeftLeftHalf / (leftBandSeparation-leftBandStart);
    double dLR = dataLeftRightHalf / (windowStart-leftBandSeparation);
    double dRL = dataRightLeftHalf / (rightBandSeparation-windowEnd);
    double dRR = dataRightRightHalf / (rightBandEnd-rightBandSeparation);
    
    double delta_dLL = sqrt(dataLeftLeftHalf) / (leftBandSeparation-leftBandStart);
    double delta_dLR = sqrt(dataLeftRightHalf) / (windowStart-leftBandSeparation);
    double delta_dRL = sqrt(dataRightLeftHalf) / (rightBandSeparation-windowEnd);
    double delta_dRR = sqrt(dataRightRightHalf) / (rightBandEnd-rightBandSeparation);

    //left trapezoid (or possibly triangle, if data(m) root is before WC(=window center).
    double m0left = WC;
    double delta_m0left = 0;
    if (dLR != dLL) {
      m0left = B-dLR*(B-A)/(dLR-dLL);
      delta_m0left = sqrt(pow(-(-A + B)*dLR/(-dLL + dLR)/(-dLL + dLR)*delta_dLL,2)+
			  pow((-A + B)*dLL/(dLL - dLR)/(dLL - dLR)*delta_dLR,2)
			  );
    }
    double leftTrapezoid=0;
    double delta_leftTrapezoid=0;
    if (WS<=m0left && m0left<WC) {
      if(debug) cout << "leftTriangle= " ;
      leftTrapezoid = 0.5*(m0left-WS)*(dLR+(WS-B)*(dLR-dLL)/(B-A));
      delta_leftTrapezoid = sqrt(pow(0.5*(dLR+(-dLL + dLR)*(-B + WS)/(-A + B))*delta_m0left,2)+
				 pow(-0.5*(m0left - WS)*(-B + WS)/(-A + B)*delta_dLL,2)+
				 pow(0.5*(m0left - WS)*(1+(-B + WS)/(-A + B))*delta_dLR,2)
				 );
      if(debug) cout << leftTrapezoid << " +- " << delta_leftTrapezoid << "  ";
    }
    else if (m0left < WS && m0left >= B) {
      if(debug) cout << "leftTrapezoid = 0" ;
      leftTrapezoid = delta_leftTrapezoid = 0;
    }
    else if (m0left >= WC || m0left < B) {
      if(debug) cout << "leftTrapezoid = " ;
      leftTrapezoid = 0.5*(WC-WS)*((dLR+(WS-B)*(dLR-dLL)/(B-A))+(dLR+(WC-B)*(dLR-dLL)/(B-A)));
      delta_leftTrapezoid = sqrt(pow(0.5*(WC - WS)*(-2*B + WC + WS)/(A-B)*delta_dLL,2)+
				 pow(-0.5*(WC - WS)*(-2*A + WC + WS)/(A-B)*delta_dLR,2)
				 );
      if(debug) cout << leftTrapezoid << " +- " << delta_leftTrapezoid << "  ";
    }
    
    //right trapezoid (or possibly triangle, if data(m) root is after WC(=window center).
    double m0right = WC;
    double delta_m0right = 0;
    if (dRL != dRR) {
      m0right = C-dRL*(C-D)/(dRL-dRR);
      delta_m0right = sqrt(pow((C - D)*dRR/(dRL - dRR)/(dRL - dRR)*delta_dRL,2)+
			   pow((-C + D)*dRL/(dRL - dRR)/(dRL - dRR)*delta_dRR,2)
			   );
    }
    double rightTrapezoid=0;
    double delta_rightTrapezoid=0;
    if (WC<m0right && m0right<=WE) {
      if(debug) cout << "rightTrapezoid= " ;
      rightTrapezoid = 0.5*(WE-m0right)*(dRL+(WE-C)*(dRL-dRR)/(C-D));
      delta_leftTrapezoid = sqrt(pow(-0.5*(dRL +(dRL - dRR)*(-C + WE)/(C - D))*delta_m0right,2)+
				 pow(0.5*(C - WE)*(-m0right + WE)/(C-D)*delta_dRR,2)+
				 pow(-0.5*(m0right - WE)*(-D + WE)/(C-D)*delta_dRL,2)
				 );
      if(debug) cout << rightTrapezoid << " +- " << delta_rightTrapezoid << endl;
    }
    else if (m0right > WE && m0right <= C) {
      if(debug) cout << "rightTrapezoid = 0" << endl;
      rightTrapezoid = delta_rightTrapezoid = 0;
    }
    else if (m0right <= WC || m0right > C){
      if(debug) cout << "rightTrapezoid= ";
      rightTrapezoid = 0.5*(WE-WC)*(dRL+(WC-C)*(dRL-dRR)/(C-D)+dRL+(WE-C)*(dRL-dRR)/(C-D));
      delta_rightTrapezoid = sqrt(pow(0.5*(WC - WE)*(-2*C + WC + WE)/(C-D)*delta_dRR,2)+
				  pow(-0.5*(WC - WE)*(-2*D + WC + WE)/(C-D)*delta_dRL,2)
				  );
      if(debug) cout << rightTrapezoid << " +- " << delta_rightTrapezoid << endl;
    }
    //sum the left and right trapezoids to find the expected data in the center.
    bc.dataExpected_twoLineInterpolation=leftTrapezoid + rightTrapezoid;
    bc.deltaDataExpected_twoLineInterpolation=sqrt(pow(delta_leftTrapezoid,2)+
						   pow(delta_rightTrapezoid,2)
						   );
    if(bc.deltaDataExpected_twoLineInterpolation < 1) bc.deltaDataExpected_twoLineInterpolation += 1;
    if(bc.dataExpected_twoLineInterpolation <= 0) bc.dataExpected_twoLineInterpolation = 1;
    bc.pvalue_twoLineInterpolation=pval((int)bc.data,bc.dataExpected_twoLineInterpolation,bc.deltaDataExpected_twoLineInterpolation,true);
    if(debug) cout << " (obs= " << bc.data << " exp.= " << bc.dataExpected_twoLineInterpolation << " +- " << bc.deltaDataExpected_twoLineInterpolation << " , pvalue2= " << bc.pvalue_twoLineInterpolation << endl;
  }//end of interpolation using left and right inclinations.
  return;
}

void BumpStatistic::checkQualityCriteriaJustData(BumpCandidateJustData& bc) {
  //check quality for bump candidate justData, and assign the preferred of the two possible pvalues to the pval variable.
  bool enoughInCenter=(bc.data >= minDataInWindow);
  bool enoughInLeftSideband=(bc.dataLeft >= minDataInWindow);
  bool enoughInRightSideband=(bc.dataRight >= minDataInWindow);
  bc.qualifies = (enoughInCenter && enoughInLeftSideband && enoughInRightSideband);
  if (bc.qualifies) {
    bc.pval=max(bc.pvalue_oneLineInterpolation,bc.pvalue_twoLineInterpolation); //to be conservative, keep the min of the two
    if(debug) cout << " ** " << bc.pval << endl;
    if(bc.pval==bc.pvalue_oneLineInterpolation) {
      bc.dataExpected=bc.dataExpected_oneLineInterpolation;
      bc.deltaDataExpected=bc.deltaDataExpected_oneLineInterpolation;
    }
    else {
      bc.dataExpected=bc.dataExpected_twoLineInterpolation;
      bc.deltaDataExpected=bc.deltaDataExpected_twoLineInterpolation;
    }
  }
  else {
    bc.pval=1;
    if(debug) {
      cout << "*";
      if(!enoughInLeftSideband) cout << "DL";
      if(!enoughInCenter) cout << "DC";
      if(!enoughInRightSideband) cout << "DR";
      cout << "*" << endl;
    }
  }
  return;
}


double BumpStatistic::probQualityIsMet(BumpCandidate bc, int maxTrials) {
  // Will be used to estimate trials factor for a bump candidate. It will return the probability that a bump candidate, with its left, central and right sideband backgrounds will be meeting my all quality criteria in a pseudo-experiment. This probability needs then to be multiplied by any other factors, such as the factorDueToOverlaps.
  double probQualifying=0;
  double BL = bc.bkgLeft;
  double BC = bc.bkg;
  double BR = bc.bkgRight;
  double probAtLeastReachMinData = pval((int)minDataInWindow,BC);
  if (probAtLeastReachMinData<=(1./maxTrials)) {
    //We know that P(qual) = P(d>=minData && other conditions)
    // = P(d>=minData)*P(other conditions|d>=minData)
    // <= P(d>=minData)
    //Possibly P(d>=minData) is large and still probQualifying is small. Then there is a reason, which is that P(other conditions|d>=minData) must be tiny, so we don't want in that case to think that this bump would qualify easily, so it's fine to throw trials.
    //But if P(d>=minData) is very small, we will run out of trials. It's better to be conservative and set probQualifying=P(d>=minData) than to leave it equal to 0.  If we leave it as 0, then very small bumps that have just 5 data and 0.0001 background will have probQualifying exactly 0, though they could have something like pval(5,0.0001), which is larger than the actual probQualifying, but still better than 0, because we know that the actual probQualifying is somewhere between 0 and P(d>=minData).  So, it makes sense to be conservative and set in that case probQualifying equal to P(d>=minData), as long as the latter is less than 1/maxTrial, which is our resolution of probability using trials.
    probQualifying = probAtLeastReachMinData;
    if(debug) cout << "probQualifying(" << BL << "," << BC << "," << BR << ")<= " << probQualifying << " " << flush ;
    return(probQualifying);
  }
  //otherwise, go on and try to resolve the actual probQualifying

  int trials=0;
  int countQualifying=0;
  debug=false;
  for ( int i = 0; i < maxTrials; i++ ) {
    trials++;

    double DL,DC,DR;
    if (BL==0) DL=0;
    else if (BL < 50) DL=(double)Math::poisson(BL);
    else DL= Math::gasdev(BL,sqrt(BL));

    if (BC==0) DC=0;
    else if (BC < 50) DC=(double)Math::poisson(BC);
    else DC= Math::gasdev(BC,sqrt(BC));

    if (BR==0) DR=0;
    else if (BR < 50) DR=(double)Math::poisson(BR);
    else DR= Math::gasdev(BR,sqrt(BR));

    bc.dataLeft = DL;
    bc.data     = DC;
    bc.dataRight= DR;
    checkQualityCriteria(bc,false); //if second argument is false, it will be fast and use probOfThisEffect, which is terribly inaccurate but fast.
    if (bc.qualifies) countQualifying++;
    double relUncertainty = sqrt((double)countQualifying) / (double)countQualifying;
    if ( relUncertainty < 0.05 ) break;
  }
  debug=debug_store;
  probQualifying = (double)countQualifying / (double)trials;
  //check if probQualifying is unreasonably small, given that the requirement is that the central has at least minDataInWindow and the sidebands are less discrepant than center.
  if(debug) cout << "probQualifying(" << BL << "," << BC << "," << BR << ")= " << countQualifying << "/" << trials << " = " << probQualifying << " " << flush ;
  return probQualifying;
}



double BumpStatistic::spikeWeightInRegion(const vector<double>& points, const vector<double>& weights, const double& windowStart, const double& windowEnd) {
  // take a region and find the amount of weight that is in spikes in that region.
  double totalSpikeWeight = 0;
  //Make a vector of the weights that are in the region. Here I will copy code from countEntries();
  vector<double> weightsInRegion;
  weightsInRegion.clear();
  double initialTotalWeight = 0;
  double finalTotalWeight=0;
  // assume (and assert) that the points are sorted ascending!
  if ( points.empty() || weights.empty() ) {
    return (0);
  }
  size_t initialIndex=0;
  size_t skipSize = max(10,(int)points.size()/100 );
  //  if(debug) cout << "skipSize= " << skipSize << " ";
  while ( (initialIndex + skipSize) < points.size() ) {
    //  if(debug) cout << "i=" << initialIndex << " datapts.size=" << points.size() << " " << flush;
    assert(points[initialIndex+skipSize] >= points[initialIndex]);
    if (points[initialIndex+skipSize] < windowStart)
      initialIndex += skipSize;
    else
      break;
  }
  for (size_t i=initialIndex; i<points.size(); ++i) {
    if(i>0) assert(points[i] >= points[i-1]);
    //next 2 lines: exploit vector being ordered to save time
    if ( points[i] < windowStart ) continue;
    if ( points[i] >= windowEnd ) break;
    if ( windowStart <= points[i] && points[i] < windowEnd ) {
      weightsInRegion.push_back(weights[i]);
      initialTotalWeight += weights[i];
    }
  }
  //OK, now we have a vector of weightsInRegion
  if (weightsInRegion.empty()) return(0);
  //Now find the average and the standard deviation of these numbers
  double averageWeight = Math::computeAverage(weightsInRegion);
  double rms = Math::computeRMS(weightsInRegion);
  // Now decrease the biggest weights until none is more than "tolerance" sigmas away from the mean weight.
  sort(weightsInRegion.begin(),weightsInRegion.end());
  double initialMaxWeight = weightsInRegion[weightsInRegion.size()-1];

  if(rms==0) {
    //Either there is only one weight, or all weights are equal
    if(debug) cout << "rms=0 --> return biggest weight- " << initialMaxWeight << endl << flush;
    return(initialMaxWeight);
  }

  double maxDeviation = (weightsInRegion[weightsInRegion.size()-1] - averageWeight)/rms;
  double tolerance = 3; //sigmas. How many sigmas away from average may the largest weight be?
  double step = max(0.0075,(weightsInRegion[weightsInRegion.size()-1]-averageWeight)/5);
  if(debug) {
    cout << " initialTotalWeight= " << initialTotalWeight << " initialMaxWeight= " << initialMaxWeight << endl << flush;
  }
  while ( maxDeviation > tolerance && weightsInRegion[weightsInRegion.size()-1] > 0) {
    //keep reducing the largest weight(s) until there is no intolerable deviation any more
    if(debug) {
      finalTotalWeight=0;
      for ( size_t i=0; i < weightsInRegion.size(); ++i )
	finalTotalWeight += weightsInRegion[i];
      cout << "totalWeight= "<< finalTotalWeight<< " maxDeviation= "<< maxDeviation << " biggestWeight= " << weightsInRegion[weightsInRegion.size()-1] << endl << flush;
    }
    step = max(0.0075,(weightsInRegion[weightsInRegion.size()-1]-averageWeight)/5); //slow down the reduction to not overestimate spike weight
    weightsInRegion[weightsInRegion.size()-1] -= step;
    //make sure that as we reduce the biggest weight, it remains the biggest.
    for (int i=weightsInRegion.size()-2; i>=0; --i) 
      if ( weightsInRegion[i] > weightsInRegion[weightsInRegion.size()-1] )
	weightsInRegion[i] = weightsInRegion[weightsInRegion.size()-1];
      else
	break;
    averageWeight = Math::computeAverage(weightsInRegion);
    rms = Math::computeRMS(weightsInRegion);
    maxDeviation = (weightsInRegion[weightsInRegion.size()-1] - averageWeight)/rms;
  }
  //Now, the goal of tolerance is met. Check how much weight we had to take out in total; that will be attributed to spike-weight.
  finalTotalWeight=0;
  for ( size_t i=0; i < weightsInRegion.size(); ++i )
    finalTotalWeight += weightsInRegion[i];
  totalSpikeWeight = initialTotalWeight - finalTotalWeight;
  if(debug) cout << " Reduction over. finalTotalWeight= " << finalTotalWeight << " totalSpikeWeight= " << totalSpikeWeight << endl << flush;
  if ( totalSpikeWeight < 0 ) 
    totalSpikeWeight = 0; //Ti may happen to get otherwise something like -9.12326e-14

  //There are still cases that by eye look spikey, but this technique doesn't catch. For example, if we have a weight of 0.01 and one of 5. The number of weights is not enough to indicate the big difference.
  //To respond to such cases, take as totalSpikeWeight the largest weight. In general, it's a good idea I think to set the totalSpikeWeight to be the weight of the largest weight, if in the process above we didn't reduce the total weight at all.
  //Well, actually it's sensible to return the biggest of (1) the totalSpikeWeight if reduction was made, and (2) the initialMaxWeight.  That's because if the reduction was very small, it's still likely to have one event (the biggest) move to the center.  
  if(debug) {
    if ( weightsInRegion[weightsInRegion.size()-1] == initialMaxWeight )
      cout << "No reduction made" << endl << flush;
    else
      cout << "Reduction was " << totalSpikeWeight << endl << flush;
  }
  totalSpikeWeight=max(totalSpikeWeight,initialMaxWeight);
  if(debug) cout << "Returning as ballast: " << totalSpikeWeight << endl << flush;
  
  //return the amount of reduction, or the biggest initial weight, whichever is begger.
  return(totalSpikeWeight);
}




void BumpStatistic::checkQualityCriteria(BumpCandidate& bc, bool useAccuratePvalue) { //default is to use accuratePvalue=true
  // decide whether the bump candidate passes (true) or not (false)

  //for speed, check first quickly if the data are at least minDataInWindow.  If that isn't satisfied, then there is no point in checking sidebands, or spikes etc.
  if(bc.data < minDataInWindow) {
    bc.pval=1;
    bc.qualifies=false;
    if(debug) cout << "*Less than minDataInWindow*";
    return;
  }  //if the minData criterion was met, then check all the rest.

  bc.dataSidebands = bc.dataLeft + bc.dataRight;
  bc.bkgSidebands= bc.bkgLeft + bc.bkgRight;
  bc.deltaBkgSidebands=sqrt(bc.deltaBkgLeft*bc.deltaBkgLeft + bc.deltaBkgRight*bc.deltaBkgRight);
  
  bc.pval = pval((int)bc.data,bc.bkg,bc.deltaBkg,useAccuratePvalue);

  // the following checks if either sideband (separately) is more discrepant than the center.
  double probLeft = pval((int)bc.dataLeft,bc.bkgLeft,bc.deltaBkgLeft,useAccuratePvalue);
  double probRight = pval((int)bc.dataRight,bc.bkgRight,bc.deltaBkgRight,useAccuratePvalue);
  bool leftMoreDiscrepantThanCenter = (probLeft < bc.pval);
  bool rightMoreDiscrepantThanCenter = (probRight < bc.pval);
  bool leftDiscrepantObjectively = (probLeft < 2.86652e-7) && weRequireObjectivelyGoodSidebands;
  bool rightDiscrepantObjectively = (probRight < 2.86652e-7) && weRequireObjectivelyGoodSidebands;
  // If the sideband has exactly 0 background (for example, some signal sits at so high mass that there is no background anywhere around it) then don't let just one datum disqualify the bump. Require at least minDataInWindow.
  if(bc.bkgLeft == 0 && bc.dataLeft < minDataInWindow) {
    leftMoreDiscrepantThanCenter=false;
    leftDiscrepantObjectively=false;
  }
  if(bc.bkgRight == 0 && bc.dataRight < minDataInWindow) {
    rightMoreDiscrepantThanCenter=false;
    rightDiscrepantObjectively=false;
  }
  
  bc.discrepantSidebands = leftDiscrepantObjectively || rightDiscrepantObjectively || leftMoreDiscrepantThanCenter || rightMoreDiscrepantThanCenter ;			
  
  if(debug) printf("([%.0f-%.0f-%.0f],b=%.2f+-%.2f,d=%.0f,sb=%.2f,sd=%.0f,ld=%.0f,rd=%.0f):%.1g:(%.1g:%.1g)", bc.windowStart, bc.windowCenter, bc.windowEnd, bc.bkg, bc.deltaBkg, bc.data, bc.bkgSidebands, bc.dataSidebands, bc.dataLeft, bc.dataRight, bc.pval , probLeft, probRight);

  bc.qualifies = ((bc.data >= minDataInWindow) && (!bc.discrepantSidebands));
  if(debug) {
    cout << "*";
    if (bc.data<minDataInWindow) cout << "D";
    if (bc.discrepantSidebands) cout << "S";
    cout << "* ";
  }

  if((!bc.qualifies) && bc.bkg==0 && bc.deltaBkg==0)
    bc.pval=1; //instead of nan

  //But if the bump qualifies, take a moment to try to be conservative with possible spikes in the background. To be conservative, decide how much weight is in spikes in each sideband, and assign temporarily this weight to the central background, and then check again if the bump still qualifies.
  bool beConservativeWithSpikes = (bc.pval<2.86652e-7); // Being conservative with all spikes was found to shift the expected distribution of significances by +0.5sigma, when using pseudo-data as data and throwing pseudo-data trials. So, it was abandoned. But it makes sense to use this if we have a serious bump.
  if(finalState=="2b1ph" || finalState=="3j1mu+1ph" || finalState=="3j1mu+1ph1pmiss" || finalState=="2b1j1ph" || finalState=="1j1mu+1mu-1pmiss") beConservativeWithSpikes = true; //These final states have bumps with pval > 5sigma, which still give probabilityDirectEstimation() more significant than 5sigma, and if we don't have enough time to run many trials, they appear like discoveries.  However, if we had run the spikes treatment on those "discovery" bumps, they wouldn't be even qualifying.
  if(beConservativeWithSpikes && bc.qualifies) {
    //First decide how much weight is in spikes.
    double ballastLeft  = spikeWeightInRegion(bkgpts,bkgwt,bc.leftBandStart, bc.windowStart);
    double ballastRight = spikeWeightInRegion(bkgpts,bkgwt,bc.windowEnd, bc.rightBandEnd);
    //temporarily imagine these ballasts into the central window, and check again if the quality criteria are still met
    double tmp_bkgLeft = bc.bkgLeft - ballastLeft;
    double tmp_bkgRight= bc.bkgRight - ballastRight;
    double tmp_bkg     = bc.bkg + (ballastLeft + ballastRight);
    
    double tmp_pval = pval((int)bc.data,tmp_bkg,bc.deltaBkg,useAccuratePvalue);
    
    double tmp_probLeft = pval((int)bc.dataLeft,tmp_bkgLeft,bc.deltaBkgLeft,useAccuratePvalue);
    double tmp_probRight = pval((int)bc.dataRight,tmp_bkgRight,bc.deltaBkgRight,useAccuratePvalue);

    bool tmp_leftMoreDiscrepantThanCenter = (tmp_probLeft < tmp_pval);
    bool tmp_rightMoreDiscrepantThanCenter = (tmp_probRight < tmp_pval);
    bool tmp_leftDiscrepantObjectively = (tmp_probLeft < 2.86652e-7) && weRequireObjectivelyGoodSidebands;
    bool tmp_rightDiscrepantObjectively = (tmp_probRight < 2.86652e-7) && weRequireObjectivelyGoodSidebands;
    if(tmp_bkgLeft == 0 && bc.dataLeft < minDataInWindow) {
      tmp_leftMoreDiscrepantThanCenter=false;
      tmp_leftDiscrepantObjectively=false;
    }
    if(tmp_bkgRight == 0 && bc.dataRight < minDataInWindow) {
      tmp_rightMoreDiscrepantThanCenter=false;
      tmp_rightDiscrepantObjectively=false;
    }
    
    bool tmp_discrepantSidebands= tmp_leftDiscrepantObjectively || tmp_rightDiscrepantObjectively || tmp_leftMoreDiscrepantThanCenter || tmp_rightMoreDiscrepantThanCenter ;  
    
    if (tmp_discrepantSidebands) {
      bc.qualifies = false;
      if(debug) cout << "spikey : ballasts=(" << ballastLeft << "," << ballastRight << "), pvals:(" << tmp_probLeft << "," << tmp_pval << "," << tmp_probRight << ")" << endl << flush;
    }
    else {
      //Then it still qualifies, even when treating sideband spikes conservatively. Then, since it qualifies, give it also the conservative pval
      bc.pval = tmp_pval;
      if(debug) cout << "spike-conservative pval= " << bc.pval << endl << flush;
    }
  }
  return;
}



vector<BumpStatistic::BumpCandidate> BumpStatistic::getAllBumps(const vector<double>& _bkgpts, const vector<double>& _bkgwt, const vector<double>& _datapts, const double& lowLimit, const double& highLimit) {
  // Scan and return an array with all the bumps that can be defined, between lowLimit and highLimit if given, or otherwise where data are.

  vector<BumpCandidate> allCandidates;

  if(_datapts.empty()) {
    if(debug) cout << "No data, so no candidates." << endl << flush;
    allCandidates.clear();
    return(allCandidates);
  }

  if(debug) cout << "smallestBkgPosition= " << smallestBkgPosition << " biggestBkgPosition=" << biggestBkgPosition << endl;

  //exploit the fact that datapts is sorted
  double smallestDataPosition = _datapts[0];
  double biggestDataPosition = _datapts[_datapts.size()-1];
  if(debug) cout << "smallestDataPosition= " << smallestDataPosition << " biggestDataPosition=" << biggestDataPosition << endl;

  pair<double,double> binCenterRange;
  if ( lowLimit!=-1 && (lowLimit < highLimit) ) {
    //low and high limits were defined, so we will limit the collection of bumps only between those limits.
    if(debug) cout <<"However we'll search from " << lowLimit << " to " << highLimit << endl;
    binCenterRange = pair<double,double>(lowLimit,highLimit);
  }
  else //find the limits by yourself
    binCenterRange = fromWhereToWhereToScan(min(smallestBkgPosition,smallestDataPosition),max(biggestBkgPosition,biggestDataPosition));
  //  before starting the scan, check if the positionStep should be smaller
  double localResolution = massResolution(binCenterRange.first);
  //  if(debug) cout << "Scanning with initial step size=min("<<positionStep<<","<<localResolution<<")" << endl;
  if(debug) cout << "Scanning with initial step size="<< localResolution/2 << endl;
  for ( double binCenter = binCenterRange.first; binCenter <= binCenterRange.second; binCenter += localResolution/2 ) {
    localResolution = massResolution(binCenter);
    if(debug) cout << "BinCenter= " << binCenter << " ";
    pair<double,double> widthRange = fromHowToHowWide(binCenter);
    if(debug) cout << "Scan with widths from: " << widthRange.first << " to: " << widthRange.second << endl;
    for ( double width = widthRange.first ; width <= widthRange.second ; width += widthStep ) {
      if(debug) cout << "Width: " << width << " : " << flush;      
      BumpCandidate bc;
      bc.windowCenter=binCenter;
      if(useWidthOptimization) optimizeWidth(bc,_bkgpts,_bkgwt);      
      else bc.windowSize = width;
      bc.windowStart = binCenter-0.5*bc.windowSize;
      bc.windowEnd = bc.windowStart + bc.windowSize;
      bc.leftBandStart = bc.windowStart - sidebandsSize*bc.windowSize;
      bc.rightBandEnd  = bc.windowEnd + sidebandsSize*bc.windowSize;

      // look at the central window and at sidebands 
      bc.data=0;
      bc.dataLeft = 0;
      bc.dataRight = 0;
      readWindowAndSidebands(_datapts,bc.leftBandStart,bc.windowStart,bc.windowEnd,bc.rightBandEnd,
			     bc.dataLeft,bc.data,bc.dataRight);

      bc.bkg=0;
      bc.deltaBkg=0;
      bc.bkgLeft=0;
      bc.deltaBkgLeft=0;
      bc.bkgRight=0;
      bc.deltaBkgRight=0;
      readWindowAndSidebands(_bkgpts,_bkgwt,bc.leftBandStart,bc.windowStart, bc.windowEnd, bc.rightBandEnd,
			     bc.bkg,bc.deltaBkg,bc.bkgLeft,bc.deltaBkgLeft,bc.bkgRight,bc.deltaBkgRight);
            
      bc.pval = 1;	
      bc.discrepantSidebands = true;
      bc.windowDataMoreThanExtrapolatedSidebands = false;
      bc.qualifies = false;
      checkQualityCriteria(bc);
      
      allCandidates.push_back(bc); //keep it.
      
      if(debug) cout << endl;
    } // next width
    if(debug) cout << endl;
  } // next binStart

  return(allCandidates);
}


void BumpStatistic::findBumpsJustData() {
  //public method, to put in motion the justData version of BumpStatistic.
  //just data part, where we look for bumps in the data only with respect to the data in the sidebands.
  if(scannedForJustDataAlready)
    return;
  //or else go on and scan
  sort(datapts.begin(),datapts.end()); //this is essential.  It used to be in the constructor.
  allBumpsJustData = getAllBumpsJustData(datapts); 
  mostInterestingBumpJustData = findMostInterestingBump(allBumpsJustData);
  vector<double> trimmedData = trimBumpsFromData(datapts,0.0001);
// //   cout << "data" << endl;
// //   for (int i=0; i<datapts.size(); ++i)
// //     cout << datapts[i] << endl;
// //   cout << "trimmedData" << endl;
// //   for (int i=0; i<trimmedData.size(); ++i)
// //     cout << trimmedData[i] << endl;
//   cout << "Searching now on the trimmed data." << endl;
//   vector<BumpCandidateJustData> trimmedDataBumps = getAllBumpsJustData(trimmedData);
//   BumpCandidateJustData trimmedMostInteresting = findMostInterestingBump(trimmedDataBumps);

  if(debug||minimalDebug) printf ("From JUST the actual data: [%.1f-%.1f-%.1f] Pvalue=%g \n\n", mostInterestingBumpJustData.windowStart, mostInterestingBumpJustData.windowCenter, mostInterestingBumpJustData.windowEnd, mostInterestingBumpJustData.pval);  
  scannedForJustDataAlready = true;
  return;
}


vector<BumpStatistic::BumpCandidateJustData> BumpStatistic::getAllBumpsJustData(const vector<double>& _datapts, const double& lowLimit, const double& highLimit) {
  vector<BumpCandidateJustData> allCandidates;
  //exploit the fact that datapts is sorted
  double smallestDataPosition = _datapts[0];
  double biggestDataPosition = _datapts[_datapts.size()-1];
  if(debug) cout << "smallestDataPosition= " << smallestDataPosition << " biggestDataPosition=" << biggestDataPosition << endl;

  pair<double,double> binCenterRange;
  if ( lowLimit!=-1 && (lowLimit < highLimit) ) {
    //low and high limits were defined, so we will limit the collection of bumps only between those limits.
    if(debug) cout <<"However we'll search from " << lowLimit << " to " << highLimit << endl;
    binCenterRange = pair<double,double>(lowLimit,highLimit);
  }
  else //find the limits by yourself
    binCenterRange = fromWhereToWhereToScan(smallestDataPosition,biggestDataPosition);
  //  before starting the scan, check if the positionStep should be smaller
  double localResolution = massResolution(binCenterRange.first);
  for ( double binCenter = binCenterRange.first; binCenter <= binCenterRange.second; binCenter += min(positionStep,localResolution) ) {
    localResolution = massResolution(binCenter);
    if(debug) cout << "BinCenter= " << binCenter << " ";
    pair<double,double> widthRange = fromHowToHowWide(binCenter);
    if(debug) cout << "Scan with widths from: " << widthRange.first << " to: " << widthRange.second << endl;
    for ( double width = widthRange.first ; width <= widthRange.second ; width += widthStep ) {
      if(debug) cout << "Width: " << width << " : " << flush;      
      BumpCandidateJustData bc;
      bc.windowCenter=binCenter;
      bc.windowSize = width;
      bc.windowStart = binCenter-0.5*bc.windowSize;
      bc.windowEnd = bc.windowStart + bc.windowSize;
      bc.leftBandStart = bc.windowStart - sidebandsSize*bc.windowSize;
      bc.rightBandEnd  = bc.windowEnd + sidebandsSize*bc.windowSize;
      bc.leftBandSeparation = (bc.leftBandStart + bc.windowStart)/2.;
      bc.rightBandSeparation = (bc.windowEnd + bc.rightBandEnd)/2.;

      // look at the central window and at sidebands 
      readWindowJustData(_datapts,bc);
      interpolateDataFromSidebands(bc);  //specify dataExpected_oneLineInterpolation, deltaDataExpected_oneLineInterpolation, dataExpected_twoLineInterpolation, deltaDataExpected_twoLineInterpolation, pvalue_oneLineInterpolation, pvalue_twoLineInterpolation
      checkQualityCriteriaJustData(bc);  //specify qualifies, dataExpected, deltaDataExpected, pval

      allCandidates.push_back(bc); //keep it.
    }//next width
  }//next window center
  sort(allCandidates.begin(),allCandidates.end(),sortIncreasingWindowCenterPosition); //by construction this shouldn't be necessary, but just to make sure.
  return(allCandidates);
}


struct islandToTrim {
  //will be used by trimBumpsFromData()
  double start, end;
  double leftAveragePosition, rightAveragePosition;
  double leftHeight, rightHeight;
};

vector<double> BumpStatistic::trimBumpsFromData(const std::vector<double>& _datapts, double pvalue) {
  //Take the data points and make a new series of points, which are devoid of justData bumps with pvalue <= given pvalue.  That is done by going where such significant bumps are and reducing the data there to be equal to the expected data.
  if(debug) cout << "Trimming bumps of pval <= " << pvalue << endl;
  vector<double> answer;
  answer.clear();

  vector<islandToTrim> islands;
  islands.clear();
  for(size_t i=0; i<allBumpsJustData.size(); ++i) {
    //assume that the bumpsJustData are sorted by increasing window center position.
    if (allBumpsJustData[i].pval <= pvalue) {
      //start the island in which we will substitute the data
      islandToTrim thisIsland;
      thisIsland.start=allBumpsJustData[i].windowStart;
      thisIsland.leftHeight=allBumpsJustData[i].dataLeft/(allBumpsJustData[i].windowStart-allBumpsJustData[i].leftBandStart);
      thisIsland.leftAveragePosition=allBumpsJustData[i].averagePositionLeft;
//      int j=i;
      size_t j=i;
      while(j<allBumpsJustData.size() && allBumpsJustData[j].pval <= pvalue) {
	++j;
      }
      i=j; //put i at the end of the island to continue later.
      --j; //j will now be the last member of the island.
      thisIsland.end=allBumpsJustData[j].windowEnd;
      thisIsland.rightHeight=allBumpsJustData[j].dataRight/(allBumpsJustData[j].rightBandEnd-allBumpsJustData[j].windowEnd);
      thisIsland.rightAveragePosition=allBumpsJustData[j].averagePositionRight;
      islands.push_back(thisIsland);
    }//go find next island if any
  }
  if(debug) cout << "Found in total " << islands.size() << " islands." << endl;
  for(size_t i=0; i<_datapts.size(); ++i){
    bool shouldBeKept=true;
    for(size_t j=0; j<islands.size(); ++j) {
      if(islands[j].start<=_datapts[i] && _datapts[i]<=islands[j].end) {
	shouldBeKept=false;
	break;
      }
    }
    if(shouldBeKept) answer.push_back(_datapts[i]);
  }
  if(debug) cout << "Kept insignificant part: " << answer.size() << " of " << _datapts.size() << endl;
  //So far we have kept the non-bumpy regions.  We must fill the bumpy regions with smooth inclined p.d.f.
  for(size_t i=0; i<islands.size(); ++i) {
    if(debug) cout << "Filling island " << i << endl;
    double dL=islands[i].leftHeight;
    double A=islands[i].leftAveragePosition;
    double dR=islands[i].rightHeight;
    double B=islands[i].rightAveragePosition;
    double start=islands[i].start;
    double end=islands[i].end;
    
    //integrate the straight line accross the island, which will give the epxected population in it.
    double expectedData = (dL+(start-A)*(dR-dL)/(B-A)+dL+(end-A)*(dR-dL)/(B-A)) * (end-start)/2;
    assert(expectedData>=0);
    if(debug) cout << " Expected data= " << expectedData << endl;
    //now generate the random data according to this inclined p.d.f.
    double ymax=max(dL+(start-A)*(dR-dL)/(B-A),dL+(end-A)*(dR-dL)/(B-A));
    int generatedAlready=0;
    while(generatedAlready<expectedData) {
      double x=drand48()*(end-start)+start;
      double y=dL+(x-A)*(dR-dL)/(B-A);
      double r=drand48()*ymax;
      //      if(debug) cout << "(x,y,r)=("<<x<<","<<y<<","<<r<<")"<<endl;
      if(r<=y)	{ 
	answer.push_back(x);
	++generatedAlready;
      }
    }
    if(debug) cout << " Filled. Now we have " << answer.size() << endl;    
  }
  sort(answer.begin(),answer.end());
  return(answer);
}


BumpStatistic::BumpCandidate BumpStatistic::findMostInterestingBump(const vector<BumpStatistic::BumpCandidate>& allCandidates) {
  // Returns the most interesting qualifying bump in the given vector.

  BumpCandidate mostInterestingBumpFound;

  vector<BumpCandidate> allQualifyingCandidates;
  allQualifyingCandidates.clear();

  for (size_t i=0; i < allCandidates.size(); ++i) 
    if (allCandidates[i].qualifies)
      allQualifyingCandidates.push_back(allCandidates[i]);

  // Print out the most interesting bin, with the smallest probability.
  double smallestProb = 1;

  if ( allQualifyingCandidates.empty() ) {
    if(debug) cout << "No good candidate was found." << endl;
    mostInterestingBumpFound.pval=1;
    mostInterestingBumpFound.windowStart=0;
    mostInterestingBumpFound.windowEnd=0;
  }
  else {
    for ( size_t i = 0; i < allQualifyingCandidates.size(); ++i) {
      assert(allQualifyingCandidates[i].qualifies);
      if ( allQualifyingCandidates[i].pval <= smallestProb ) {
	mostInterestingBumpFound = allQualifyingCandidates[i];
	smallestProb=mostInterestingBumpFound.pval;
      }
    }
  }
  if(debug) cout << "Most interesting bump: leftLimit=" << mostInterestingBumpFound.windowStart << " rightLimit=" << mostInterestingBumpFound.windowEnd << " Pvalue=" << mostInterestingBumpFound.pval << "." << endl;

  return(mostInterestingBumpFound);
}


BumpStatistic::BumpCandidateJustData BumpStatistic::findMostInterestingBump(const vector<BumpStatistic::BumpCandidateJustData>& allCandidates) {
  BumpCandidateJustData mostInterestingBumpFound;
  vector<BumpCandidateJustData> allQualifyingCandidates;
  allQualifyingCandidates.clear();

  for (size_t i=0; i < allCandidates.size(); ++i) 
    if (allCandidates[i].qualifies)
      allQualifyingCandidates.push_back(allCandidates[i]);
  
  double smallestProb = 1;

  if ( allQualifyingCandidates.empty() ) {
    if(debug) cout << "No good justData candidate was found." << endl;
    mostInterestingBumpFound.pval=1;
    mostInterestingBumpFound.windowStart=0;
    mostInterestingBumpFound.windowEnd=0;
  }
  else {
    for ( size_t i = 0; i < allQualifyingCandidates.size(); ++i) {
      assert(allQualifyingCandidates[i].qualifies);
      if ( allQualifyingCandidates[i].pval <= smallestProb ) {
	mostInterestingBumpFound = allQualifyingCandidates[i];
	smallestProb=mostInterestingBumpFound.pval;
      }
    }
  }

  if(debug) cout << "Most interesting justData bump: leftLimit=" << mostInterestingBumpFound.windowStart << " rightLimit=" << mostInterestingBumpFound.windowEnd << " Pvalue=" << mostInterestingBumpFound.pval << "." << endl;

  return(mostInterestingBumpFound);
}


bool BumpStatistic::isThereAnyMoreInterestingBumpQ(const vector<double>& _bkgpts, const vector<double>& _bkgwt, const vector<double>& _datapts, const double& targetPvalmin) {
  // Scan all the bumps, in the same way that getAllBumps() does, and if any is found that qualifies and is more interesting than targetPvalmin, stop and return true.
  bool ans = false;
  if(debug) cout << "Anything more interesting? " << endl;;
  
  if (_datapts.empty() || (_datapts.size() < minDataInWindow)) {
    // This can happen, especially when we throw pseudo-experiments where the sum_bkg is small.
    if(debug) {
      if (_datapts.empty()) cout << "No data, so no." << endl;
      else if(_datapts.size() < minDataInWindow) cout << "Data < minimum of " << minDataInWindow << " so no." << endl;
    }
    return(false);
  }
  
  //exploit the fact that datapts is sorted
  double smallestDataPosition = _datapts[0];
  double biggestDataPosition = _datapts[_datapts.size()-1];
  if(debug) cout << "smallestDataPosition= " << smallestDataPosition << " biggestDataPosition=" << biggestDataPosition << endl;

  pair<double,double> binCenterRange = fromWhereToWhereToScan(min(smallestBkgPosition,smallestDataPosition),max(biggestBkgPosition,biggestDataPosition));
  //  before starting the scan, check if the positionStep should be smaller
  double localResolution = massResolution(binCenterRange.first);
  for ( double binCenter = binCenterRange.first; binCenter <= binCenterRange.second; binCenter += localResolution/2 ) {
    localResolution = massResolution(binCenter);
    if(debug) cout << "BinCenter " << binCenter << " ";
    pair<double,double> widthRange = fromHowToHowWide(binCenter);
    if(debug) cout << "Scan with widths from: " << widthRange.first << " to: " << widthRange.second << endl;
    for ( double width = widthRange.first ; width <= widthRange.second ; width += widthStep ) {
      if(debug) cout << "Width: " << width << " : " << flush;      
      BumpCandidate bc;
      bc.windowCenter=binCenter;
      if(useWidthOptimization) optimizeWidth(bc,_bkgpts,_bkgwt);
      else bc.windowSize = width;
      bc.windowStart = binCenter - 0.5*bc.windowSize;
      bc.windowEnd = bc.windowStart + bc.windowSize;
      bc.leftBandStart = bc.windowStart - sidebandsSize*bc.windowSize;
      bc.rightBandEnd  = bc.windowEnd + sidebandsSize*bc.windowSize;

      // look at the central window and at sidebands 
      bc.data=0;
      bc.dataLeft = 0;
      bc.dataRight = 0;
      readWindowAndSidebands(_datapts,bc.leftBandStart,bc.windowStart,bc.windowEnd,bc.rightBandEnd,
			     bc.dataLeft,bc.data,bc.dataRight);

      bc.bkg=0;
      bc.deltaBkg=0;
      bc.bkgLeft=0;
      bc.deltaBkgLeft=0;
      bc.bkgRight=0;
      bc.deltaBkgRight=0;
      readWindowAndSidebands(_bkgpts,_bkgwt,bc.leftBandStart,bc.windowStart, bc.windowEnd, bc.rightBandEnd,
			     bc.bkg,bc.deltaBkg,bc.bkgLeft,bc.deltaBkgLeft,bc.bkgRight,bc.deltaBkgRight);
            
      bc.pval = 1;	
      bc.discrepantSidebands = true;
      bc.windowDataMoreThanExtrapolatedSidebands = false;
      bc.qualifies = false;
      checkQualityCriteria(bc);
      
      if (bc.qualifies && bc.pval <= targetPvalmin) {
	if(debug) cout << "Found a pval=" << bc.pval << " <= " << targetPvalmin << endl;
	return(true);
      }
      
      if(debug) cout << endl;
    } // next width
    if(debug) cout << endl;
  } // next binStart
  
  //if we have reached so far, nothing more interesting was found, so we return the default (=false)
  return(ans);
}




double BumpStatistic::probabilityDirectEstimation(const double& pvalmin) {
  //Don't think of trials factor. Just estimate the probability that at least one candidate will fluctuate to have pval <= pvalmin.
  // P = 1 - probAllBinsDontFluctuate
  // P = 1 - Product(bin i doesn't fluctuate)*(factor because they are not independent, they overlap a little)
  if ( pvalmin == 1 ) {
    if(debug) cout << "Don't care about direct estimation, since pvalmin = 1 anyway" << endl << flush;
    return(1.0);
  }

  double probAllBinsDontFluctuate = 1;
  if(debug) cout << "Estimating directly Probability(allBinsDontFluctuate) = Prod(1-thisGuyFluctuates) : " << flush;
  for ( size_t i = 0 ; i < allBumps.size() ; ++i ) {
    double probToPassQuality = probQualityIsMet(allBumps[i],20*(int)allBumps.size()); //had it 2*allBumps.size.  More would offer us some stability I hope.
    double probToPassPoisson = 0;
    //    probToPassPoisson = pvalmin; //ignoring the effect of integer data. Makes bumps look a little less significant than they are, by slightly overestimating P_a (=P=what we try to find here).
    //if you don't want to ignore it, use this:
    if(allBumps[i].bkg >= 20) 
      probToPassPoisson = pvalmin; //ignoring the effect of integer data
    else
      probToPassPoisson = pval(Math::criticalD(allBumps[i].bkg,pvalmin,allBumps[i].deltaBkg),allBumps[i].bkg); //this takes into account the effect of discrete nature of data.
    double probThisGuyFluctuates;
    probThisGuyFluctuates = probToPassPoisson*probToPassQuality;
    if(probThisGuyFluctuates < 0) probThisGuyFluctuates=0;
    if(probThisGuyFluctuates > 1) probThisGuyFluctuates=1;
    if(debug) printf("* (%.2g)=%.2g ",probToPassPoisson,probThisGuyFluctuates);
    probAllBinsDontFluctuate *= 1-probThisGuyFluctuates; //this guy doesn't fluctuate and this doesn't and this doesn't...
  }
  if(debug) cout << ", so probAllBinsDontFluctuate=" << probAllBinsDontFluctuate << endl << flush;
  double penalizedPvalue = 1 - probAllBinsDontFluctuate; //at least one guy fluctuates
  if(debug||minimalDebug) cout << "Estimated P=" << penalizedPvalue << endl << flush;
  return(penalizedPvalue);
}


vector<double> BumpStatistic::producePseudoDataTrialAndError(const vector<double>& _bkgpts, const vector<double>& _bkgwt, long int numOfData) {
  // produce a set of pseudo-data, using the trial and error method.
  double bkgBinSize = positionStep / 50; // in GeV/c^2, the size of the bin used to sample the bkg distribution.
  int nBkgBins = (int)((biggestBkgPosition+10)/bkgBinSize); // to be sure we have enough space. The background histogram will span from 0 up to bkgBinSize*nBkgBins GeV/c^2
  assert(bkgBinSize*nBkgBins>=biggestBkgPosition); // assert we have enough bins to cover all the background range.
  if (!pseudoDataProductionHasScannedBkgAlready) {
    if(debug) cout << "Pseudo-data production needs to read background. " ;
    sum_bkg = 0;
    bkgBins = vector<double>(nBkgBins,0);
    biggestBkgBinContent = 0; 
    smallestBkgPosition=100000;
    biggestBkgPosition=0;
    for (size_t i=0; i< _bkgpts.size(); ++i){
      sum_bkg += _bkgwt[i];
      int bkgBinNumber = (int)(_bkgpts[i]/bkgBinSize);
      assert(bkgBinNumber < nBkgBins);
      bkgBins[bkgBinNumber] += _bkgwt[i];
      
      if ( _bkgpts[i] < smallestBkgPosition ) smallestBkgPosition = _bkgpts[i];
      if ( _bkgpts[i] > biggestBkgPosition ) biggestBkgPosition = _bkgpts[i];
      if(bkgBins[bkgBinNumber] > biggestBkgBinContent) biggestBkgBinContent = bkgBins[bkgBinNumber];
    }
    //do the cumulative vector now
    assert(sum_bkg>0);
    cumulativeBkgBins = vector<double>(nBkgBins,0);
    cumulativeBkgBins[0]=bkgBins[0]/sum_bkg;
    for (int i=1; i<nBkgBins; ++i){
      cumulativeBkgBins[i] = cumulativeBkgBins[i-1] + bkgBins[i]/sum_bkg;
    }
    if(debug) cout << "Done. " << endl;
    pseudoDataProductionHasScannedBkgAlready = true; // don't scan the background again. Its profile is available in private members of the BumpStatistic class.
  }
  
  if(numOfData==-1) numOfData = Math::poisson(sum_bkg);
  if(debug) printf("total bkg: %.1f, data to generate: %d\n", sum_bkg, (int) numOfData);
  vector<double> pseudoDataPts;
  pseudoDataPts.clear();
  long int producedData = 0;
  while (producedData < numOfData)    {
    double datumPosition = drand48()*(biggestBkgPosition-smallestBkgPosition)+smallestBkgPosition;
    double r1 = drand48()*biggestBkgBinContent;
    
    int binNumber = (int)(datumPosition / bkgBinSize);
    if (r1 <= bkgBins[binNumber]){
      pseudoDataPts.push_back(datumPosition);
      producedData++;
    }
  }  
  sort(pseudoDataPts.begin(),pseudoDataPts.end());
//   if(debug) { //Print pseudo-data and background, to plot and compare.
//     cout << "bkg is:" << endl;
//     for ( int i=0; i < _bkgpts.size(); ++i ) cout << _bkgpts[i] << " " << _bkgwt[i] << endl;
//     cout << "pseudodata are:" << endl;
//     for ( int i=0; i < pseudoDataPts.size(); ++i ) cout << pseudoDataPts[i] << endl;
//   }
  return (pseudoDataPts);
}



vector<double> BumpStatistic::producePseudoData(const vector<double>& _bkgpts, const vector<double>& _bkgwt, long int numOfData) { 
  // produce a set of pseudo-data, using the trial and error method.
  //numOfData = -1 by default
  // if numOfData == -1, then pull pseudo-data population from a Poisson with mean the background population.  Otherwise, produce specified numOfData data.
  double bkgBinSize = positionStep / 50; // in GeV/c^2, the size of the bin used to sample the bkg distribution. 
  int nBkgBins = (int)((biggestBkgPosition+10)/bkgBinSize); // to be sure we have enough space. The background histogram will span from 0 up to bkgBinSize*nBkgBins GeV/c^2
  assert(bkgBinSize*nBkgBins>=biggestBkgPosition); // assert we have enough bins to cover all the background range.
  if (!pseudoDataProductionHasScannedBkgAlready) {
    if(debug) cout << "Pseudo-data production needs to read background. " ;
    sum_bkg = 0;
    bkgBins = vector<double>(nBkgBins,0);
    biggestBkgBinContent = 0; 
    smallestBkgPosition=100000;
    biggestBkgPosition=0;
    for (size_t i=0; i< _bkgpts.size(); ++i){
      sum_bkg += _bkgwt[i];
      int bkgBinNumber = (int)(_bkgpts[i]/bkgBinSize);
      assert(bkgBinNumber < nBkgBins);
      bkgBins[bkgBinNumber] += _bkgwt[i];
      if ( _bkgpts[i] < smallestBkgPosition ) smallestBkgPosition = _bkgpts[i];
      if ( _bkgpts[i] > biggestBkgPosition ) biggestBkgPosition = _bkgpts[i];
      if(bkgBins[bkgBinNumber] > biggestBkgBinContent) biggestBkgBinContent = bkgBins[bkgBinNumber];
    }
    //do the cumulative vector now
    assert(sum_bkg>0);
    cumulativeBkgBins = vector<double>(nBkgBins,0);
    cumulativeBkgBins[0]=bkgBins[0]/sum_bkg;
    for (int i=1; i<nBkgBins; ++i){
      cumulativeBkgBins[i] = cumulativeBkgBins[i-1] + bkgBins[i]/sum_bkg;
    }
    if(debug) cout << "Done. " << endl;
    pseudoDataProductionHasScannedBkgAlready = true; // don't scan the background again. Its profile is available in private members of the BumpStatistic class.
  }
  
  if(numOfData==-1) numOfData = Math::poisson(sum_bkg);
  if(debug) printf("total bkg: %.1f, data to generate: %d\n", sum_bkg, (int) numOfData);
  vector<double> pseudoDataPts;
  pseudoDataPts.clear();
  long int producedData = 0;
  while (producedData < numOfData)    {
    double r = drand48();
    //find bin that has cumulative just higher than r, using Newton's method, exploiting the monotonically increasing cumulative distribution
    int leftBin  = (int)(smallestBkgPosition / bkgBinSize);
    int rightBin = (int)(biggestBkgPosition / bkgBinSize);
    int middleBin= (leftBin+rightBin)/2;
    while (!(middleBin==leftBin+1)) {
      assert(leftBin<=middleBin && middleBin<=rightBin);
      //      if(debug) cout << "(L,M,R)=(" << leftBin <<","<< middleBin <<","<< rightBin << ") " << flush; 
      if ( cumulativeBkgBins[middleBin]==r ) {
	break;
      }
      else if ( cumulativeBkgBins[middleBin]>r ) rightBin = middleBin;
      else if ( cumulativeBkgBins[middleBin]<r ) leftBin = middleBin;
      middleBin= (leftBin+rightBin)/2;
      //      if(debug) cout << "==> (" << leftBin <<","<< middleBin <<","<< rightBin << ") " << endl << flush; 
    }
    // make sure the middleBin is the first bin whose cumulative content is just above r.
    while( cumulativeBkgBins[middleBin]<r ) 
      ++middleBin;
    // if the neighbors of middleBin have the same cumulative content, spread
    // the left and right to contain the plateau
    leftBin = middleBin;
    rightBin= middleBin;
    while( (cumulativeBkgBins[leftBin]==cumulativeBkgBins[middleBin]) && (leftBin>0) )
      --leftBin;
    while( (cumulativeBkgBins[rightBin]==cumulativeBkgBins[middleBin]) && (rightBin<(nBkgBins-1)) )
      ++rightBin;
    //    if(debug) cout << "plateau ==> (" << leftBin <<","<< middleBin <<","<< rightBin << ") " << endl << flush; 
    //Find where exactly to put the datum. Put it uniformly distributed in the plateau, i.e. between the end of left and beginning of right bin.
    double rightEdgeOfLeft = (leftBin+1)*bkgBinSize;
    double leftEdgeOfRight = (rightBin)*bkgBinSize;
    double datumPosition  = rightEdgeOfLeft + drand48()*(leftEdgeOfRight-rightEdgeOfLeft);
//     if(debug) {
//       printf("r=%g rightEdgeOfLeft=%g leftEdgeOfRight=%g cumLeft=%g cumMid=%g cumRight=%g \n",r,rightEdgeOfLeft,leftEdgeOfRight,cumulativeBkgBins[leftBin],cumulativeBkgBins[middleBin],cumulativeBkgBins[rightBin]);
//       cout << "pseudo-datum position: " << datumPosition << endl << flush;
//     }
    pseudoDataPts.push_back(datumPosition);
    producedData++;
  }  
  sort(pseudoDataPts.begin(),pseudoDataPts.end());
//   if(debug) { //Print pseudo-data and background, to plot and compare.
//     cout << "bkg is:" << endl;
//     for ( int i=0; i < _bkgpts.size(); ++i ) cout << _bkgpts[i] << " " << _bkgwt[i] << endl;
//     cout << "pseudodata are:" << endl;
//     for ( int i=0; i < pseudoDataPts.size(); ++i ) cout << pseudoDataPts[i] << endl;
//   }
  return (pseudoDataPts);
}



void BumpStatistic::addPseudoSignal(vector<double>& _datapts, int numberOfPseudoSignal, double mean, double sigma) {
  //Will add numberOfPseudoSignal events to the _datapts vector, pulled from a gaussian distribution (energy resolution dominates over natural decay width which is Breit-Wigner). The result will be sorted.
  if (sigma==0)
    sigma=massResolution(mean); 
  for ( int i=0; i<numberOfPseudoSignal; ++i ){
    double pseudoSignalMass=Math::gasdev(mean,sigma);
    while (pseudoSignalMass <= 0) pseudoSignalMass=Math::gasdev(mean,sigma);
    _datapts.push_back(pseudoSignalMass);
  }
  sort(_datapts.begin(),_datapts.end());
}



int BumpStatistic::approximateSignalNeededForDiscovery(const double& targetSigma, const double& signalMass) {
  //Estimate quickly, without actually injecting anything, using just the background information.
  if(debug) cout << "Finding approximate signal needed for discovery at signalMass " << signalMass << endl;

  //be conservative and keep the alternative that gives the largest background.
  double approximateWidthOfWindowHere = fromHowToHowWide(signalMass).second;
  //find the approximate background, taking into account that depending on the pseudo-data, the most interesting bump candidate that contains signalMass may be a little left or right.
  pair<double,double> bkgAndDeltaBkg1 = countBkgAndDeltaBkg(bkgpts,bkgwt,signalMass-0.5*approximateWidthOfWindowHere,signalMass+0.5*approximateWidthOfWindowHere);
  pair<double,double> bkgAndDeltaBkg = bkgAndDeltaBkg1;
  pair<double,double> bkgAndDeltaBkgLeft = bkgAndDeltaBkg1;
  pair<double,double> bkgAndDeltaBkgRight = bkgAndDeltaBkg1;
  //Take the biggest background that is likely to include the signalMass, depending on where the bump window will be starting from.
  for (double binCenter = signalMass - 0.5*approximateWidthOfWindowHere ; binCenter <= signalMass + 0.5*approximateWidthOfWindowHere ; binCenter += approximateWidthOfWindowHere/5 ){
    pair<double,double> bkgAndDeltaBkg2 = countBkgAndDeltaBkg(bkgpts,bkgwt,binCenter-0.5*approximateWidthOfWindowHere,binCenter+0.5*approximateWidthOfWindowHere);
    if ( bkgAndDeltaBkg2.first > bkgAndDeltaBkg.first ) {
      bkgAndDeltaBkg=bkgAndDeltaBkg2;
      bkgAndDeltaBkgLeft = countBkgAndDeltaBkg(bkgpts,bkgwt,binCenter-(0.5+sidebandsSize)*approximateWidthOfWindowHere,binCenter-0.5*approximateWidthOfWindowHere);
      bkgAndDeltaBkgRight= countBkgAndDeltaBkg(bkgpts,bkgwt,binCenter+0.5*approximateWidthOfWindowHere,binCenter+(0.5+sidebandsSize)*approximateWidthOfWindowHere);
      if(debug) cout << "Biggest possible background around signal: limits: " << binCenter-(0.5+sidebandsSize)*approximateWidthOfWindowHere << " " << binCenter-(0.5)*approximateWidthOfWindowHere << " " << binCenter+(0.5)*approximateWidthOfWindowHere << " " << binCenter+(0.5+sidebandsSize)*approximateWidthOfWindowHere <<" left: " << bkgAndDeltaBkgLeft.first << "+-" << bkgAndDeltaBkgLeft.second << " center: " << bkgAndDeltaBkg.first << "+-" << bkgAndDeltaBkg.second << " right: " << bkgAndDeltaBkgRight.first << "+-" << bkgAndDeltaBkgRight.second << endl;
    }
  }
  //OK, now we have the largest likely background and its uncertainty.
  if(debug) cout << "Approximate bkg = " << bkgAndDeltaBkg.first << " +- " << bkgAndDeltaBkg.second << endl;
  double approximateBackgroundHere = bkgAndDeltaBkg.first;
  double approximateDeltaBackgroundHere = bkgAndDeltaBkg.second;
  if (approximateBackgroundHere == 0)
    return((int)minDataInWindow);

  double targetProbability = Math::sigma2prob(targetSigma);
  //estimate how small the minimum pval needs to be in order to obtain the targetProbability through probabilityDirectEstimation(pvalmin).
  //For that, first estimate the effective trials factor (Neff), which was found to describe the probabilityDirectEstimation(pval) very nicely.  Think of:
  //probability = 1 - (1-pvalmin)^Neff
  if(Neff==-1) {
    double pval=1e-5;
    Neff=log(1-probabilityDirectEstimation(pval))/log(1-pval);
    if(debug) cout << "Neff= " << Neff << endl;
  }
  double targetPval=1-pow((1-targetProbability),(1/Neff));
  if(debug) cout << "targetProbability= " << targetProbability << " targetPval= " << targetPval << endl;
  //Now estimate the data needed to be in the central window for this target pvalue to be produced in this window
  int signalNeededInCenter=Math::criticalD(approximateBackgroundHere,targetPval,approximateDeltaBackgroundHere)-(int)approximateBackgroundHere;
  
  int maximumToleratedSignalLeft=10000000; //large = ineffective
  int maximumToleratedSignalRight=10000000; //large=ineffective
  if(weRequireObjectivelyGoodSidebands) {
    //Also find the data that would need to go into the sidebands to make the whole bump disqualify. 
    maximumToleratedSignalLeft=Math::criticalD(bkgAndDeltaBkgLeft.first,2.86652e-7,bkgAndDeltaBkgLeft.second)-(int)bkgAndDeltaBkgLeft.first;
    maximumToleratedSignalRight=Math::criticalD(bkgAndDeltaBkgRight.first,2.86652e-7,bkgAndDeltaBkgRight.second)-(int)bkgAndDeltaBkgRight.first;
  }
  //this is an exception, to not easily discard (with just one event in the sideband) bumps that lie far away, where all bkg==0.
  if (bkgAndDeltaBkgLeft.first==0) maximumToleratedSignalLeft=(int)minDataInWindow;
  if (bkgAndDeltaBkgRight.first==0) maximumToleratedSignalRight=(int)minDataInWindow;
  //Consider that about 68% of the total signal will be at the center, if the center spans 2*Resolution, and the signal is distributed as a gaussian of sigma=resolution.  That means we would need to produce more than what we need just in the center, to get what we need in the center.
  int signalNeeded=(int)(signalNeededInCenter/0.68)+1;
  //At the same time, none of the sidebands is allowed to have an effect larger than 5sigma (see checkQualityCriteria()) which means that we must make sure we don't add too much on any sideband.
  if (signalNeeded*0.16 >= maximumToleratedSignalLeft || signalNeeded*0.16 >= maximumToleratedSignalRight)
    signalNeeded=-1; //we are not sensitive at this position.
  if(debug) cout << "Returning needed signal= " << signalNeeded << endl;
  return(signalNeeded);
}


int BumpStatistic::signalNeededForDiscovery(const double& targetSigma, const double& signalMass, const vector<double>& dataBasis) {
  //For a given signalMass and a basis of data (i.e. pseudo-data) inject pseudo-signal at signalMass until the probability that such a pvalue would be observed anywhere in this bumphuntable variable reaches the level of a targetSigma effect.
  double targetProbability = Math::sigma2prob(targetSigma);
  
  double approximateWidthOfWindowHere = fromHowToHowWide(signalMass).second;
  double approximateBackgroundHere = max( 1.0 , countEntries(bkgpts,bkgwt,signalMass-0.5*approximateWidthOfWindowHere,signalMass+0.5*approximateWidthOfWindowHere) );
  double prob=1;

  vector<double> thisSetOfPseudoData = dataBasis; //start from the basis

  int nSignal = max((int)minDataInWindow,(int)floor(8*sqrt(approximateBackgroundHere)));
  addPseudoSignal(thisSetOfPseudoData,(dataBasis.size()+nSignal)-thisSetOfPseudoData.size(),signalMass); //increment to have nSignal additional, which means dataBasis.size()+nSignal in total, which means that in this step we need to add (dataBasis.size()+nSignal)-thisSetOfPseudoData.size().
  vector<BumpCandidate> allPseudoBumps; allPseudoBumps.clear();
  debug=false; allPseudoBumps = getAllBumps(bkgpts, bkgwt, thisSetOfPseudoData, signalMass-5*approximateWidthOfWindowHere,signalMass+5*approximateWidthOfWindowHere); debug=debug_store;
  BumpCandidate mostInterestingPseudoBump;
  mostInterestingPseudoBump = findMostInterestingBump(allPseudoBumps);
  debug=false; prob = probabilityDirectEstimation(mostInterestingPseudoBump.pval); debug=debug_store;

  while (prob < targetProbability && nSignal>=0) {
    if(debug) cout << "Decreasing nSignal at " << signalMass << " from " << nSignal;
    nSignal -= max((int)minDataInWindow,(int)floor(sqrt(approximateBackgroundHere)));
    if(debug) cout << " to " << nSignal << "  ";
    thisSetOfPseudoData = dataBasis;
    addPseudoSignal(thisSetOfPseudoData,nSignal,signalMass); 
    allPseudoBumps.clear();
    debug=false; allPseudoBumps = getAllBumps(bkgpts, bkgwt, thisSetOfPseudoData, signalMass-5*approximateWidthOfWindowHere,signalMass+5*approximateWidthOfWindowHere); debug=debug_store;
    BumpCandidate mostInterestingPseudoBump;
    mostInterestingPseudoBump = findMostInterestingBump(allPseudoBumps);
    debug=false; prob = probabilityDirectEstimation(mostInterestingPseudoBump.pval); debug=debug_store;
    if(debug) cout << " => prob= " << prob << endl;
  }

  while (prob > targetProbability) {
    if(debug) cout << "Increasing nSignal at " << signalMass << " from " << nSignal;
    nSignal += max((int)minDataInWindow,(int)floor(sqrt(approximateBackgroundHere)/10));
    if(debug) cout << " to " << nSignal << "  ";
    addPseudoSignal(thisSetOfPseudoData,(dataBasis.size()+nSignal)-thisSetOfPseudoData.size(),signalMass); //increment to have nSignal additional, which means dataBasis.size()+nSignal in total, which means that in this step we need to add (dataBasis.size()+nSignal)-thisSetOfPseudoData.size().
    allPseudoBumps.clear();
    debug=false; allPseudoBumps = getAllBumps(bkgpts, bkgwt, thisSetOfPseudoData, signalMass-5*approximateWidthOfWindowHere,signalMass+5*approximateWidthOfWindowHere); debug=debug_store;
    BumpCandidate mostInterestingPseudoBump;
    mostInterestingPseudoBump = findMostInterestingBump(allPseudoBumps); 
    debug=false; prob = probabilityDirectEstimation(mostInterestingPseudoBump.pval); debug=debug_store;
    if(debug) cout << " => prob= " << prob << endl;
    if ( mostInterestingPseudoBump.windowStart > signalMass || mostInterestingPseudoBump.windowEnd < signalMass ) {
      // the most interesting bump isn't where we added the pseudo-signal.
      if(debug) {
	cout << "Signal passed unnoticed. Let's see what happened in the populated window." << endl;
	for ( size_t bcIndex = 0; bcIndex < allPseudoBumps.size(); ++bcIndex ) {
	  BumpCandidate thisBC = allPseudoBumps[bcIndex];
	  if(thisBC.windowStart<=signalMass && thisBC.windowEnd>signalMass) {
	    checkQualityCriteria(thisBC);
	    cout << " pval= " << thisBC.pval << endl;
	  }
	}
      }
      if ( nSignal > 2*targetSigma*sqrt(approximateBackgroundHere) && nSignal > 30 ) {
	// no matter how much we add here, we can't attract attention.
	if(debug) cout << "Give up. We can't attract attention." << endl;
	break; // try next signalMass
      }
      else {
	nSignal += max(1,(int)floor(sqrt(approximateBackgroundHere)));
	continue; // with more nSignal
      }
    }
  }

  if(prob <= targetProbability) 
    return(nSignal);
  else
    return(-1);
}



int BumpStatistic::signalNeededForDiscovery(const double& targetSigma, const double& signalMass) {
  //Create pseudo-data and then find how much signal is needed at signalMass.
  if(debug) cout << "Probe sensitivity, using pseudo-data as a basis, at signalMass= " << signalMass << endl;
  vector<double> dataBasis;
  //  double targetProbability = Math::sigma2prob(targetSigma);
  pseudoDataProductionHasScannedBkgAlready = false;
  dataBasis = producePseudoData(bkgpts, bkgwt); //it's sorted.
  pseudoDataProductionHasScannedBkgAlready = false;
  
  int nSignalNeeded = signalNeededForDiscovery(targetSigma, signalMass, dataBasis);
  if(debug) cout << "Found signalMass= " << signalMass << " nSignalNeeded= " << nSignalNeeded << endl;
  return(nSignalNeeded);
}




string BumpStatistic::probeSensitivityAlongBackground(const double& targetSigma) {
  //Take the background, make pseudo-data, and find how much pseudo-signal needs to be added at each position accross the background spectrum, in order to give a targetSigma effect. This effect takes into account the number of bump candidates in this bumphuntable distribution.
  string ans;
  if(debug) cout << "Probe sensitivity, using pseudo-data as a basis." << endl;
  vector<double> dataBasis;
  //double targetProbability = Math::sigma2prob(targetSigma);
  pseudoDataProductionHasScannedBkgAlready = false;
  dataBasis = producePseudoData(bkgpts, bkgwt); //it's sorted.
  pseudoDataProductionHasScannedBkgAlready = false;
  
  pair<double,double> rangeForSignalMass = fromWhereToWhereToScan(smallestBkgPosition,biggestBkgPosition);
  for (double signalMass = rangeForSignalMass.first ; signalMass <= rangeForSignalMass.second ; signalMass += (rangeForSignalMass.second - rangeForSignalMass.first)/20) {
    //for a given signalMass:
    int nSignalNeeded = signalNeededForDiscovery(targetSigma, signalMass, dataBasis);
    if(debug) cout << "Found signalMass= " << signalMass << " nSignalNeeded= " << nSignalNeeded << endl;
    char buffer [50];
    sprintf(buffer, "%g  %i \n", signalMass, nSignalNeeded);
    ans += buffer;
  }
  return(ans);
}


string BumpStatistic::probeApproximateSensitivityAlongBackground(const double& targetSigma) {
  //Take the background, make pseudo-data, and find how much pseudo-signal needs to be added at each position accross the background spectrum, in order to give a targetSigma effect. This effect takes into account the number of bump candidates in this bumphuntable distribution.
  string ans;
  if(debug) cout << "Probe sensitivity, using pseudo-data as a basis." << endl;
  
  pair<double,double> rangeForSignalMass = fromWhereToWhereToScan(smallestBkgPosition,biggestBkgPosition);
  for (double signalMass = rangeForSignalMass.first ; signalMass <= rangeForSignalMass.second ; signalMass += (rangeForSignalMass.second - rangeForSignalMass.first)/20) {
    //for a given signalMass:
    int nSignalNeeded = approximateSignalNeededForDiscovery(targetSigma, signalMass);
    if(debug) cout << "Found signalMass= " << signalMass << " nSignalNeeded= " << nSignalNeeded << endl;
    char buffer [50];
    sprintf(buffer, "%g  %i \n", signalMass, nSignalNeeded);
    ans += buffer;
  }
  return(ans);
}




double BumpStatistic::probabilityWithPseudoData(const double& targetPvalmin, double minutesAllowed) {
  // will throw pseudo-data to estimate the fraction of pseudo-data distributions that had their most interesting bump be of pval<=targetPvalmin.
  if ( targetPvalmin == 1 ) {
    if(debug) cout << "There was no real bump candidate here, so return 1=uninteresting." << endl;
    return(1.0);
  }
  double fractionOfMoreInterestingPseudodata = 1;  
  double deltaFraction = 1;
  int nTrials = 0;
  int nMoreInteresting = 0;
  int initialTime=time(0);
  double minutesSoFar=0;
  int auxiliaryTime=initialTime;

  //during these trials, if debug is true turn it into minimalDebug
  bool initialDebugStatus=false;
  if(debug) {initialDebugStatus=true; debug=false; minimalDebug=true;}

  while ( deltaFraction==0 ||
	  deltaFraction/fractionOfMoreInterestingPseudodata > 0.05 
	  ) {
    if(minimalDebug) {
      if(nTrials==0) cout << "Trials progress:" << flush;
      int currentTime=time(0);
      if( currentTime-auxiliaryTime >= 60 ) { //report every minute
	printf(" %d / %d=%g +- %.1f   ", nMoreInteresting, nTrials, fractionOfMoreInterestingPseudodata, deltaFraction/fractionOfMoreInterestingPseudodata*100);
	cout << flush;
	auxiliaryTime=currentTime; //reset timer
      }
    }
    ++nTrials;
    if(debug) cout << "This is pseudo-data trial " << nTrials << endl;
    vector<double> pseudoDataPts = producePseudoData(bkgpts, bkgwt);
    //    vector<double> pseudoDataPts = producePseudoData(bkgpts, bkgwt, (long int)datapts.size()); // This is to generate in pseudo-trials as many pseudo-data as the actual data was.  This option was not used officially.
    bool thisTrialIsAsOrMoreInteresting = isThereAnyMoreInterestingBumpQ(bkgpts, bkgwt, pseudoDataPts, targetPvalmin);
    if(thisTrialIsAsOrMoreInteresting)
      ++nMoreInteresting;
    fractionOfMoreInterestingPseudodata = ((double)nMoreInteresting/(double)nTrials);  
    deltaFraction = sqrt(((double)nTrials)*fractionOfMoreInterestingPseudodata*(1-fractionOfMoreInterestingPseudodata))/(double)nTrials;
    //decide if we should exit before reaching the precision goal.
    minutesSoFar=(time(0)-initialTime)/60.0;
    if(minutesSoFar >= minutesAllowed) {
      if(debug || minimalDebug) printf(" Took %.2f minutes. ", minutesSoFar);
      //decide whether to give it an extension or kill it here
      //estimate how many trials it would take to reduce deltaFraction enough to make the difference between of the current fractionOfMoreInterestingPseudodata and 2.86652e-7 be 2*deltaFraction
      //solve basically 2*deltap = fabs(p-A), where A=2.86652e-7, p=successes/trials and deltap=sqrt(trials*p*(1-p)).
      double estimatedTrialsNeeded = 0;
      double threshold=2.86652e-7;
      if(deltaFraction>0 && fractionOfMoreInterestingPseudodata!=threshold)
	estimatedTrialsNeeded=4*fractionOfMoreInterestingPseudodata*(1-fractionOfMoreInterestingPseudodata)/((fractionOfMoreInterestingPseudodata-threshold)*(fractionOfMoreInterestingPseudodata-threshold));
      if(estimatedTrialsNeeded>0 && estimatedTrialsNeeded<1.5*(double)nTrials) {
	minutesAllowed *= 1.5;
	if(debug || minimalDebug) printf(" Extend minutes to %.2f. Expect to need %.2f minutes. ", minutesAllowed, minutesSoFar/(double)nTrials*estimatedTrialsNeeded);
      }
      else {
	if(debug || minimalDebug) printf(" No more time. Expected to take %.2f minutes. ", minutesSoFar/(double)nTrials*estimatedTrialsNeeded);
	break; //we can't afford an extension
      }
    }
    if (deltaFraction>0 && 
 	fractionOfMoreInterestingPseudodata - 2*deltaFraction > 2.86652e-7) { 
      // we can be 95% certain that fraction is not 5-sigma level significant
      if(debug || minimalDebug) printf(" After %d trials it's uninteresting. ", nTrials);
      break;
    }
    if (deltaFraction>0 && 
	fractionOfMoreInterestingPseudodata + 2*deltaFraction < 2.86652e-7) { 
      // we can be 95% certain that fraction is 5-sigma level significant
      if(debug || minimalDebug) printf(" After %d trials it's interesting. ", nTrials);
      break;
    }
    //end of considering to break
  }//end of throwing pseudo-data
  if(debug || minimalDebug) printf("Fraction of more interesting pseudo-data: P=%d/%d=%g +- %g\n", nMoreInteresting, nTrials, fractionOfMoreInterestingPseudodata, deltaFraction);

  if(initialDebugStatus) {debug=true; minimalDebug=false;} //in case we had deactivated debug, turn it back on.

  return(fractionOfMoreInterestingPseudodata);
}


double BumpStatistic::probability(double pvalmin) {
  //What the final user sees.
  //It takes the pvalmin, which by default will be the pvalue of the most interesting bump found, and it will find the probability that a pseudo-epxeriment would have in this bumphuntable variable any bump more interesting than this pvalmin.
  if(!scannedForBumpsAlready)
    findBumps();

  if ( pvalmin < 0 ) { //this is the default
    pvalmin = mostInterestingBump.pval;
  }

  /////////////////////////////////
  //This is for testing the expected distribution of significances using fast or pseudo-data estimation:
  /////////////////////////////////
//   double probWithTF=probabilityDirectEstimation(pvalmin);
//   double probWithPseudoData=probabilityWithPseudoData(pvalmin,readMinutesAllowedFromEnvironment());
//   if (debug||minimalDebug) {
//     cout << "With TF: " << setprecision(16) << probWithTF << " ";
//     cout << "With PseudoData: " << setprecision(16) << probWithPseudoData << " " << setprecision(8);
//     printf("Rel. difference= %g per cent \n", 100.0*(probWithTF-probWithPseudoData)/probWithPseudoData);
//     cout << flush;
//   }
//   return(probWithTF);
  ////////////////////////////////


  //This is the real thing the user ought to see
  double prob=probabilityDirectEstimation(pvalmin);
  //  if (prob < 2.86652e-7) { //if it's more interesting than 5sigma
  //we seem to be in a very interesting case  (5sigma, which with a 5000 trials factor will become 1-(1-2.86652e-7)^5000=0.00143223358=2.98sigma). Let's throw pseudo-data trials:
  //no no, I decide to throw pseudo-data if we are even close to the discovery threshold, not just beyond it.
  if (prob < 3.39767e-06) { //if it's more interesting than 4.5sigma
    //we seem to be in a kind-of interesting case (4.5sigma, which with a 5000 trials factor will become 1-(1-3.39767e-06)^5000=0.01684489=2.1sigma). Let's throw pseudo-data trials:

    //    srand48((unsigned)(time(NULL)*getpid())); //this would be useful if I wanted a totally random seed.
    if(getenv("bumpSeed")!=NULL) //allow user to specify seed for pseudo-data.  That will allow to run trials in parallel jobs, to obtain high statistics.
      srand48((unsigned)atof(getenv("bumpSeed")));
    double probTrials = probabilityWithPseudoData(pvalmin,readMinutesAllowedFromEnvironment());
    if (probTrials!=0) //as long as we didn't just run out of patience, use that result
      prob=probTrials;
  }
  if(minimalDebug) cout << "Returning P= " << prob << endl;
  return(prob);
  //////////////////

}



std::vector<double> BumpStatistic::getData() {
  //return the vector of positions of data events.
  //Useful if the client called bumpStatistic passing it pseudo-signal, in dataOrPseudoDataMode=3, and wants to get back the generated pseudo-data + the given pseudo-signal.
  //  cout << "Returning vector of " << datapts.size() << " data." << endl;
  return (datapts);
}

std::pair<double, double> BumpStatistic::getLimits() {
  if(!scannedForBumpsAlready)
    findBumps();
  pair<double, double> ans(mostInterestingBump.windowStart, mostInterestingBump.windowEnd);
  return(ans);
}

std::pair<double, double> BumpStatistic::getLimitsJustData() {
  if(!scannedForJustDataAlready)
    findBumpsJustData();
  pair<double, double> ans(mostInterestingBumpJustData.windowStart, mostInterestingBumpJustData.windowEnd);
  return(ans);
}

double BumpStatistic::getPvalmin() {
  if(!scannedForBumpsAlready)
    findBumps();
  return(mostInterestingBump.pval);
}

double BumpStatistic::getPvalminJustData() {
  if(!scannedForJustDataAlready)
    findBumpsJustData();
  return(mostInterestingBumpJustData.pval);
}

string BumpStatistic::reportPvalOfAllBumps() {
  //make a string that lists the position of each bump candidate and the corresponding pvalue.
  string answer="";
  if ( allBumps.empty() ) {
    if(debug) cout << "No report of all bumps, because none was found." << endl;
  }
  else {
    answer = finalState+"\n"+massVariable+"\n";
    for ( size_t i = 0; i < allBumps.size(); ++i) {
      answer += Math::ftoa(allBumps[i].windowCenter);
      answer += " ";
      if (isnan(allBumps[i].pval))
	answer += "1";
      else
	answer += Math::ftoa(allBumps[i].pval) ;
      answer += "\n";
    }
  }
  return(answer);
}


string BumpStatistic::reportContentsOfAllBumps() {
  //make a string that lists the position of each bump candidate and the corresponding data, background, and deltaBkg.
  string answer="";
  if ( allBumps.empty() ) {
    if(debug) cout << "No report of contents of all bumps, because none was found." << endl;
  }
  else {
    answer = finalState+"\n"+massVariable+"\n";
    for ( size_t i = 0; i < allBumps.size(); ++i) {
      answer += Math::ftoa((allBumps[i].windowStart + allBumps[i].windowEnd)/2);
      answer += " ";
      answer += Math::ftoa(allBumps[i].data) ;
      answer += " ";
      answer += Math::ftoa(allBumps[i].bkg) ;
      answer += " ";
      answer += Math::ftoa(allBumps[i].deltaBkg) ;
      answer += "\n";
    }
  }
  return(answer);
}


string BumpStatistic::reportPvalOfAllBumpsJustData() {
  //make a string that lists the position of each bump candidate (justData) and the corresponding pvalue.
  string answer="";
  if ( allBumpsJustData.empty() ) {
    if(debug) cout << "No report of all bumps just data, because none was found." << endl;
  }
  else {
    answer = finalState+"\n"+massVariable+"\n";
    for ( size_t i = 0; i < allBumpsJustData.size(); ++i) {
      answer += Math::ftoa((allBumpsJustData[i].windowStart + allBumpsJustData[i].windowEnd)/2);
      answer += " ";
      answer += Math::ftoa(allBumpsJustData[i].pval) ;
      answer += "\n";
    }
  }
  return(answer);
}



double BumpStatistic::massResolution(double mass) {
  bool internalFunctionDebug = false;
  double uncertainty = sqrt(mass);
  string massVariableEffective=massVariable;
  //treat exceptions before parsing variable name.  Otherwise, the variable name will not be understood, or we will not know what resolution to assume, without using information from the final state.
  if ( massVariable=="mass(X)(XX->4j)")
    massVariableEffective="mass(j1,j2)";
  if ( massVariable=="mass(W->lv)" ) {
    if ( finalState.find("e+",0) != string::npos || finalState.find("e-",0) != string::npos )
      massVariableEffective="mass(e+,pmiss)";
    else if ( finalState.find("mu+",0) != string::npos || finalState.find("mu-",0) != string::npos )
      massVariableEffective="mass(mu+,pmiss)";
    else if ( finalState.find("tau+",0) != string::npos || finalState.find("tau-",0) != string::npos )
      massVariableEffective="mass(tau+,pmiss)";
    assert(massVariableEffective!="mass(W->lv)"); //in the end we must have changed this name.
  }
  if ( massVariable=="mass(ttbar)" ) {
    if ( finalState.find("e+",0) != string::npos || finalState.find("e-",0) != string::npos )
      massVariableEffective="mass(j1,j2,b1,b2,e+,pmiss)";
    else if ( finalState.find("mu+",0) != string::npos || finalState.find("mu-",0) != string::npos )
      massVariableEffective="mass(j1,j2,b1,b2,mu+,pmiss)";
  }
  if ( massVariable=="mass(W_jj)" ) {
    massVariableEffective="mass(j1,j2)";
  }
  if ( massVariable=="mass(W#-#jj)" ) {
    massVariableEffective="mass(j1,j2)";
  }
  // end of renaming mass variable to treat special cases.
  // Now we are ready to start the parsing and then the calculation.

  // Here is the logic:
  // sigma_m = sqrt( alpha_1^2 * E_1  +  alpha_2^2 * E_2  +  alpha_3^2 * E_3 ... )
  // where sigmaE_i = alpha_i * sqrt(E_i),  from the known formula DE/E = a/sqrt(E).

  //So, if m=E1+E2, but sigma^2_m = a^2_1*E1 + a^2_2*E2, then sigma_m depends on each specific energy.  
  //We can have E1 and E2 swap values, which would keep m the same, but change sigma_m.
  // To avoid having to know the individual energies of the objects here, let's instead assume that E1=E2=...=E_i=E.  
  // Then, sigma^2_m = (a^2_1 + a^2_2 + ... )E  and  m=N*E  ,  where N=number of objects.
  // which simplifies things, because then sigma_m = sqrt(a^2_1 + a^2_2 + ... ) * sqrt(m/N)

  if(internalFunctionDebug) cout << "massVariable= " << massVariable << " massVariableEffective= " << massVariableEffective << endl;
  //Assume the name of the variable has (object1,object2,...,objectN)
  int openPar;
  int closePar;
  //locate parentheses
  openPar = massVariableEffective.find("(",0);
  closePar = massVariableEffective.rfind(")",massVariableEffective.size());

  if(internalFunctionDebug) cout << "Parentheses found at: " << openPar << ".." << closePar << endl;

  vector<int> commaLocations;
  size_t nextComma = massVariableEffective.find(",",openPar);
  while ( nextComma != string::npos ) {
    if(internalFunctionDebug) cout << "nextComma= " << nextComma << " " ;
    commaLocations.push_back(nextComma);
    nextComma=massVariableEffective.find(",",nextComma+1);
  }
  if(internalFunctionDebug) cout << endl;
  //separations are parentheses and intermediate commas
  vector<int> separations;
  separations.push_back(openPar);
  for (size_t i=0; i < commaLocations.size(); ++i) {
    separations.push_back(commaLocations[i]);
    if(internalFunctionDebug) cout << "Comma at " << commaLocations[i] << endl;
  }
  separations.push_back(closePar);
    
  //isolate objects between separations
  vector<string> objects;
  for ( size_t i=0; i<=separations.size()-2; ++i) {
    string thisObject=massVariableEffective.substr(separations[i]+1,separations[i+1]-separations[i]-1);
    if(internalFunctionDebug) cout << "thisObject= " << thisObject << endl;
    objects.push_back(thisObject);
  }

  int nTotal = objects.size();
  if(internalFunctionDebug) cout << "Total objects= " << nTotal << endl;
  assert (nTotal>=1);

  //Count categories: 
  //category0=b, j, tau
  //category1=electron, photon
  //category2=muon
  //category3=pmiss (just in case we are hunting in a transverse mass variable, using pmiss as an object)

  int category[4] = {0,0,0,0};

  for ( size_t i=0; i < objects.size(); ++i ) {
    if (objects[i].find("j",0) != string::npos)
      category[0]++;
    if (objects[i].find("b",0) != string::npos)
      category[0]++;
    if (objects[i].find("tau",0) != string::npos || objects[i].find("[t]",0) != string::npos)
      category[0]++;
    if (objects[i].find("e",0) != string::npos)
      category[1]++;
    if (objects[i].find("ph",0) != string::npos || objects[i].find("[g]",0) != string::npos)
      category[1]++;
    if (objects[i].find("mu",0) != string::npos || objects[i].find("[m]",0) != string::npos)
      category[2]++;
    if (objects[i].find("pmiss",0) != string::npos || objects[i].find("p&/",0) != string::npos)
      category[3]++;
    if (objects[i].find("W",0) != string::npos) {
      nTotal++; //W means actually 2 objects, like e and pmiss, or mu and pmiss.
      if ( finalState.find("e+",0) != string::npos || finalState.find("e-",0) != string::npos ) {
	category[1]++;
	category[3]++;
      }
      else if ( finalState.find("mu+",0) != string::npos || finalState.find("mu-",0) != string::npos ) {
	category[2]++;
	category[3]++;
      }
      else if ( finalState.find("tau+",0) != string::npos || finalState.find("tau-",0) != string::npos ) {
	category[0]++;
	category[3]++;
      }
    }
  }//end of categorization

  assert ( (category[0] + category[1] + category[2] + category[3]) == nTotal ); //otherwise, we have no idea what this combination includes.

  if(internalFunctionDebug) cout << "categories (j|b|tau , e|ph , mu , pmiss) : " << category[0] << " " << category[1] << " " << category[2] << " " << category[3] << endl;

  //assume again all objects having energy E = mass/nTotal.  Then:
  //m=(E + E + ... category[0] such energies) + (E + E + ... category[1] such energies) + ....
  // the mean mass is m = totalNumberOfObjects * E
  // where E= defined as m/totalCountOfObjects.
  // All these E's can fluctuate according to detector resolution.
  //delta_m=sqrt( category[hadrons](1*DeltaEhad)^2 + category[EM]*(DeltaEem)^2 + ... )
  if(internalFunctionDebug) cout << "mass= " << mass << flush;
  if(mass==0) {
    //then E=0 and then we have nan from dividing /E.  So give mass a little kick to be above but very near 0.
    mass=1e-6;
    if(internalFunctionDebug) cout << "-->" << mass << flush;
  }
  double E=(mass/nTotal);
  if(internalFunctionDebug) cout << " E= " << E;
  double DeltaEem  = 0.14*sqrt(E) + 0.015*E; //Chris Hays said this 1.5%.
  if(internalFunctionDebug) cout << " DeltaEem= " << DeltaEem << flush;
  //  double DeltaEhad = 1.00*sqrt(E);
  double DeltaEhad = E * ( sqrt( 0.457/E + 20.3/E/E + 0.00834 ) ); //full formula given by Sasha Pronko
  if(internalFunctionDebug) cout << " DeltaEhad= " << DeltaEhad << flush;
  double DeltaEmu  = 0.0005*E*E; //because they're beam-constrainted.
  if(internalFunctionDebug) cout << " DeltaEmu= " << DeltaEmu << flush;
  double DeltaMET  = 3*sqrt(E); //we are pretty uncertain about it.
  if(internalFunctionDebug) cout << " DeltaMET= " << DeltaMET << flush;
  uncertainty=sqrt( category[0]*pow(DeltaEhad,2) + 
		    category[1]*pow(DeltaEem ,2) + 
		    category[2]*pow(DeltaEmu ,2) + 
		    category[3]*pow(DeltaMET ,2)
		    );
  
  assert(uncertainty>=0);
  
  if(internalFunctionDebug) cout << "\nResolution= " << uncertainty << endl << flush;
  return uncertainty;
}









bool Bump::searchForBumpsHereOrNot(std::string variableName) {
  bool debug = false;
  bool minimalDebug = false;
  readDebugOptionFromEnvironment(debug, minimalDebug);
  bool ans = false;
  if (variableName.find("mass")!=string::npos) 
    ans = true;
  
  // No hope of finding a narrow bump in transverse mass distributions.
  //  if (variableName.find("transverse")!=string::npos) 
  //    ans = false;

  // They're typically hugely discrepant, and they typically don't look
  // like bumps, but rather like the whole data are shifted, so omit them.
  if ( variableName == "mass(b)" || 
       variableName == "mass(b1)" || 
       variableName == "mass(b2)" || 
       variableName == "mass(b3)" || 
       variableName == "mass(b4)" || 
       variableName == "mass(b5)" || 
       variableName == "mass(b6)" || 
       variableName == "mass(b7)" || 
       variableName == "mass(b8)" || 
       variableName == "mass(b9)" || 
       variableName == "mass(j)" || 
       variableName == "mass(j1)" || 
       variableName == "mass(j2)" || 
       variableName == "mass(j3)" || 
       variableName == "mass(j4)" ||
       variableName == "mass(j5)" || 
       variableName == "mass(j6)" || 
       variableName == "mass(j7)" || 
       variableName == "mass(j8)" ||
       variableName == "mass(j9)" ||
       variableName.find("deltaMass")!=string::npos ||
       variableName == "mass(all)" ||
       variableName == "mass(W,b,j)-mass(W)-mass(b,j)" 
       )  
    ans = false;    

  if (debug||minimalDebug) if (ans) cout << " bumphuntable variable: " << variableName << endl;

  return ans;
}

 

