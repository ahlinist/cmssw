
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstOptimalBinning/interface/OptimalBinning.hh"
using namespace std;

//double optimalBinning_MaximumNumberOfBins = FLT_MAX;

// Penalty terms

double OptimalBinning::nMC(const vector<double>& hwt, const vector<double>& bwt)
{
  double ans =     
    max(Math::effectiveNumberOfEvents(hwt),
	Math::effectiveNumberOfEvents(bwt));
  return(ans);
}

double OptimalBinning::wtMC(const vector<double>& wt)
{
  double ans = Math::computeSum(wt)/Math::effectiveNumberOfEvents(wt);
  return(ans);
}

double OptimalBinning::wtMC(const vector<double>& hwt, const vector<double>& bwt)
{
  double ans = 
    max(wtMC(hwt), wtMC(bwt));
  return(ans);
}

double OptimalBinning::penaltyTermPerBin_w(double hwt, double bwt, double p)
{
  if(bwt==0)
    bwt = hwt;
  // double ans = 0.5 * ((hwt + bwt)*(2 + hwt + bwt))/((1 + hwt)*(1 + bwt));
  double ans = ((p*hwt + bwt+ sqrt(sqrt(hwt*bwt))*(p-1)*(p-1))*((1+p) + p*hwt + bwt))/(2*p*(1 + p*hwt)*(1 + bwt));
  ans *= log10(exp(1.))/2; // convert analytic to numeric
  return(ans);
}

double OptimalBinning::penaltyTermPerBin_n(double wn, bool binsHaveBeenOptimizedQ)
{
  double ans = 1;
  if(binsHaveBeenOptimizedQ)
    ans = max(1., 0.5 * log(10*wn));
  return(ans);
}

double OptimalBinning::penaltyTermPerBin_k(double k, double wn)
{
  double ans = 1.;  // exp(k/sqrt(wn)) is not justified -- this penalty term factor is essentially flat
  return(ans);
}

double OptimalBinning::penaltyTermPerBin(double hwt, double bwt, double p, double n, double k, bool binsHaveBeenOptimizedQ)
{
  double w = max(hwt, bwt);
  double ans = penaltyTermPerBin_w(hwt, bwt, p) * penaltyTermPerBin_n(w*n,binsHaveBeenOptimizedQ) * penaltyTermPerBin_k(k,w*n);
  return(ans);
}

double OptimalBinning::penaltyTermPerBin(double w, double n, double k, bool binsHaveBeenOptimizedQ)
{
  double ans = penaltyTermPerBin_w(w) * penaltyTermPerBin_n(w*n,binsHaveBeenOptimizedQ) * penaltyTermPerBin_k(k,w*n);
  return(ans);
}

ExpectedEvidence OptimalBinning::binningFigureOfMerit_highStatisticsLimit(double h, double b, double dh, double db)
{
  assert((h>10)&&(b>10)); // must be in high statistics limit
  double dhSqd = dh*dh;
  double dbSqd = db*db;
  double m = ( (h-b)*(h-b)*(h+dhSqd+b+dbSqd) + pow( (b+dbSqd) - (h+dhSqd) , 2 ) )
    / ( 2 * ( h + dhSqd ) * ( b + dbSqd ) );
  m *= log10(exp(1.))/2.; // convert to decimal units
  return(ExpectedEvidence(m,m));
}

ExpectedEvidence OptimalBinning::binningFigureOfMerit(double h, double b, double dh, double db)
{
  if((h>10)&&(b>10))
    return(binningFigureOfMerit_highStatisticsLimit(h,b,dh,db)); // save time if we're in the high statistics limit
  if((h<0)&&(fabs(h)<1e-10))
    h = 0;
  if((b<0)&&(fabs(b)<1e-10))
    b = 0;
  if((h<0)||(b<0))
    cout << "h = " << h << " ; b = " << b << endl;
  assert((h>=0)&&(b>=0));

  if( (fabs(h-b)>100*db) ||
      (fabs(h-b)>100*dh) )
    return(ExpectedEvidence(100,100));

  double nSigma = 3.0;
  int d_low = (int)min(h - nSigma*sqrt(h+pow(dh,2)), b - nSigma*sqrt(b+pow(db,2)));
  if(d_low<0)
    d_low = 0;
  int d_high = (int)max(h + nSigma*sqrt(h+pow(dh,2))+4*nSigma, b + nSigma*sqrt(b+pow(db,2))+4*nSigma); 

  double expectedEvidence_h = 0.;
  double expectedEvidence_b = 0.;
  for(int d=d_low; d<=d_high; d++)
    {
      double ph = Math::probOfThisEffect(h, d, dh, "==");
      double pb = Math::probOfThisEffect(b, d, db, "==");
      double phCrossValidated = ph; //Math::poissonConvolutedWithPoissonIntegrated(max(0,h-wtMC), wtMC, d); //bktemp
      double pbCrossValidated = pb; //Math::poissonConvolutedWithPoissonIntegrated(max(0,b-wtMC), wtMC, d); //bktemp
      expectedEvidence_h += ph*log10(phCrossValidated/pb);
      expectedEvidence_b += pb*log10(pbCrossValidated/ph); 
    }
  expectedEvidence_h = min(expectedEvidence_h,100.);
  expectedEvidence_b = min(expectedEvidence_b,100.);

  ExpectedEvidence expectedEvidence = ExpectedEvidence(expectedEvidence_h,expectedEvidence_b);
  return(expectedEvidence);
}


ExpectedEvidence OptimalBinning::binningFigureOfMerit(const vector<double>& h, const vector<double>& b, 
						      const vector<double>& dh, const vector<double>& db, 
						      double wtMC, double nMC, 
						      bool binsHaveBeenOptimizedQ, bool includePenaltyFactorQ)
{
  size_t nBins = h.size();
  assert( (nBins==h.size()) &&
	  (nBins==b.size()) );
  
  ExpectedEvidence expectedEvidence;
  for(size_t k=0; k<nBins; k++)
    {
      expectedEvidence += binningFigureOfMerit(h[k],b[k],dh[k],db[k]);
      if(includePenaltyFactorQ)
	expectedEvidence -= penaltyTermPerBin(wtMC,nMC,k,binsHaveBeenOptimizedQ);
    }
  return(expectedEvidence);
}


void OptimalBinning::getBinContents(const vector<double>& a, const vector<double>& awt, 
				    const vector<double>& binPoints, 
				    vector<double>& ak, vector<double>& dak, 
				    double epsilonWt)
{
  int nBins = binPoints.size()+1;
  ak = dak = vector<double>(nBins);
  int ia1=0, ia2=0;
  for(int i=0; i<nBins; i++)
    {
      ia1 = ia2; 
      if(i<nBins-1)
	ia2 = upper_bound(a.begin(), a.end(), binPoints[i]) - a.begin();
      else
	ia2 = a.size();

      ak[i] = Math::computeSum(vector<double>(awt.begin()+ia1,awt.begin()+ia2));
      dak[i] = Math::computeMCerror(vector<double>(awt.begin()+ia1,awt.begin()+ia2));
    }
  return;
}


ExpectedEvidence OptimalBinning::binningFigureOfMerit(const vector<double>& h, const vector<double>& b, 
						      const vector<double>& hwt, const vector<double>& bwt, 
						      const vector<double>& binPoints, 
						      double epsilonWt, bool binsHaveBeenOptimizedQ, bool includePenaltyFactorQ)
{
  int nBins = binPoints.size()+1;
  vector<double> hk(nBins), bk(nBins), dhk(nBins), dbk(nBins);
  getBinContents(h, hwt, binPoints, hk, dhk, epsilonWt);
  getBinContents(b, bwt, binPoints, bk, dbk, epsilonWt);

  return(binningFigureOfMerit(hk, bk, dhk, dbk, wtMC(hwt,bwt), nMC(hwt,bwt), binsHaveBeenOptimizedQ, includePenaltyFactorQ));
}


/*
double OptimalBinning::calculateTheLikelihood(const vector<double>& h, const vector<double>& b, 
					      const vector<double>& hwt, const vector<double>& bwt, 
					      const vector<double>& d, 
					      const vector<double>& binPoints, 
					      double epsilonWt)
{
  int nBins = binPoints.size()+1;
  vector<double> hk(nBins), bk(nBins), dhk(nBins), dbk(nBins), dk(nBins), ddk(nBins);
  getBinContents(h, hwt, binPoints, hk, dhk, epsilonWt);
  getBinContents(b, bwt, binPoints, bk, dbk, epsilonWt);
  getBinContents(d, vector<double>(d.size(),1), binPoints, dk, ddk, epsilonWt);

  double ans = 0;
  for(int k=0; k<nBins; k++)
    {
      if(hk[k]<0) 
	hk[k]=0; //cout << "hk[" << k << "] = " <<  hk[k] << endl;
      if(bk[k]<0) 
	bk[k]=0; //cout << "hk[" << k << "] = " <<  bk[k] << endl; // avoid roundoff problems
      double x = log10(Math::poissonConvolutedWithGaussianIntegrated(hk[k], dhk[k], (int)dk[k])/Math::poissonConvolutedWithGaussianIntegrated(bk[k], dbk[k], (int)dk[k]));
      ans += x;
    }
  return(ans);
}
*/

void OptimalBinning::chooseSingleBinPoint(const vector<double>& h, const vector<double>& hwt, 
					  const vector<double>& b, const vector<double>& bwt, 
					  double& binPoint, double& improvement, 
					  double epsilonWt)   // h and b must be sorted!!
{
  assert(h.size()==hwt.size());
  assert(b.size()==bwt.size());
  binPoint = improvement = 0.;

  if((h.size()==0)||
     (b.size()==0))
    return;

  vector<double> hwtCumulative(1,0), bwtCumulative(1,0);
  vector<double> hwtSqd(1,0), bwtSqd(1,0);
  for(size_t i=0; i<h.size(); i++)
      hwtCumulative.push_back(hwtCumulative[i] + hwt[i]);
  for(size_t i=0; i<b.size(); i++)
    bwtCumulative.push_back(bwtCumulative[i] + bwt[i]);
  double hwtTotal = hwtCumulative[hwtCumulative.size()-1];  
  double bwtTotal = bwtCumulative[bwtCumulative.size()-1];  
  if((hwtTotal<-0.001)||(bwtTotal<-0.001)||
     ((hwtTotal>10*bwtTotal)&&(bwtTotal>100))||
     ((bwtTotal>10*hwtTotal)&&(hwtTotal>100)))
    return; // the problem is too easy, no need for further binning
  for(size_t i=0; i<h.size(); i++)
    hwtSqd.push_back(hwtSqd.back()+pow(hwt[i],2.));
  for(size_t i=0; i<b.size(); i++)
    bwtSqd.push_back(bwtSqd.back()+pow(bwt[i],2.));

  double m0 = binningFigureOfMerit(hwtTotal, bwtTotal, 
				   Math::computeMCerror(hwtTotal,
							hwtSqd.back()==0 ? 1 : pow(hwtTotal,2.)/hwtSqd.back(),
							epsilonWt), 
				   Math::computeMCerror(bwtTotal,
							bwtSqd.back()==0 ? 1 : pow(bwtTotal,2.)/bwtSqd.back(),
							epsilonWt)
				   ).getExpectedEvidence();
  // cout << "m0 = " << m0 << endl;


  vector<double> hbin, bbin, dh, db;
  hbin = bbin = dh = db = vector<double>(2);

  double m12_best = 0;
  vector<double> cutpoints = vector<double>(0);
  cutpoints.insert(cutpoints.end(),h.begin(),h.end());
  cutpoints.insert(cutpoints.end(),b.begin(),b.end());
  sort(cutpoints.begin(),cutpoints.end());
  cutpoints.erase(unique(cutpoints.begin(),cutpoints.end()),cutpoints.end());
  //cout << "cutpoints = "; print(cutpoints);

  double minWt = minWeight(epsilonWt);
  // cout << "minWt = " << minWt << endl;

  for(size_t i=1; i<cutpoints.size()-1; i++)
    {
      int ih = upper_bound(h.begin(),h.end(),cutpoints[i])-h.begin();
      int ib = upper_bound(b.begin(),b.end(),cutpoints[i])-b.begin();
      
      hbin[0] = hwtCumulative[ih]; hbin[1] = hwtTotal - hwtCumulative[ih];
      bbin[0] = bwtCumulative[ib]; bbin[1] = bwtTotal - bwtCumulative[ib];
      
      if( (hbin[1] < minWt) &&
	  (bbin[1] < minWt) )
	break;
      if( (hbin[0] < minWt) &&
	  (bbin[0] < minWt) )
	continue;

      dh[0] = Math::computeMCerror(hbin[0], (hbin[0]==0 ? 1 : 1/(hwtSqd[ih]/pow(hbin[0],2.))), epsilonWt);
      dh[1] = Math::computeMCerror(hbin[1], (hbin[1]==0 ? 1 : 1/((hwtSqd.back()-hwtSqd[ih])/pow(hbin[1],2.))), epsilonWt);
      db[0] = Math::computeMCerror(bbin[0], (bbin[0]==0 ? 1 : 1/(bwtSqd[ib]/pow(bbin[0],2.))), epsilonWt);
      db[1] = Math::computeMCerror(bbin[1], (bbin[1]==0 ? 1 : 1/((bwtSqd.back()-bwtSqd[ib])/pow(bbin[1],2.))), epsilonWt);

      //dh = sqrt(hbin[0]*hbin[1])/(hN+1))+ 5*epsilonWt; db = bErr; //db = sqrt(bbin[0]*bbin[1]/(bN+1))+ 5*epsilonWt;

      double m12 = 
	binningFigureOfMerit( hbin[0], bbin[0], dh[0], db[0] ).getExpectedEvidence() + 
	binningFigureOfMerit( hbin[1], bbin[1], dh[1], db[1] ).getExpectedEvidence() ;

      if(m12 > m12_best)
	{
	  m12_best = m12;
	  binPoint = cutpoints[i];
	  improvement = m12 - m0;

	  // cout << "m12_1 = " << 	binningFigureOfMerit( hbin[0], bbin[0], dh[0], db[0] ) << endl;
	  // cout << "m12_2 = " <<        binningFigureOfMerit( hbin[1], bbin[1], dh[1], db[1] ) << endl;

	}
    }

  return;
}


void OptimalBinning::chooseBinPoints_alreadySorted(const vector<double>& h, const vector<double>& hwt, 
						   const vector<double>& b, const vector<double>& bwt, 
						   vector<double>& binPoints, vector<double>& improvement, 
						   double epsilonWt)
{  
  double binPoint_best=0., improvement_best=0.;
  vector<double> _binPoints = binPoints;
  sort(_binPoints.begin(),_binPoints.end());

  //  double _wtMC = wtMC(hwt,bwt);
  double _nMC = nMC(hwt,bwt);
  //cout << "hwt = "; print(hwt);
  //cout << "bwt = "; print(bwt);
  //cout << "wtMC = " << wtMC(hwt,bwt) << " ; nMC = " << nMC(hwt,bwt) << " ; P = " << penaltyTermPerBin(_wtMC, _nMC, 1) << endl;

  for(size_t i=0; i<_binPoints.size()+1; i++)
    {      

      double _binPoint, _improvement;
      vector<double> _h = vector<double>(i==0 ? h.begin() : upper_bound(h.begin(), h.end(), _binPoints[i-1]), 
					 i==_binPoints.size() ? h.end() : upper_bound(h.begin(), h.end(), _binPoints[i]));
      vector<double> _b = vector<double>(i==0 ? b.begin() : upper_bound(b.begin(), b.end(), _binPoints[i-1]), 
					 i==_binPoints.size() ? b.end() : upper_bound(b.begin(), b.end(), _binPoints[i]));
      vector<double> _hwt = vector<double>(i==0 ? hwt.begin() : hwt.begin()+(upper_bound(h.begin(), h.end(), _binPoints[i-1])-h.begin()), 
					   i==_binPoints.size() ? hwt.end() : hwt.begin()+(upper_bound(h.begin(), h.end(), _binPoints[i])-h.begin()));
      vector<double> _bwt = vector<double>(i==0 ? bwt.begin() : bwt.begin()+(upper_bound(b.begin(), b.end(), _binPoints[i-1])-b.begin()), 
					   i==_binPoints.size() ? bwt.end() : bwt.begin()+(upper_bound(b.begin(), b.end(), _binPoints[i])-b.begin()));
      chooseSingleBinPoint(_h, _hwt, _b, _bwt, _binPoint, _improvement, epsilonWt);
      if(_improvement > improvement_best)
	{
	  improvement_best = _improvement;
	  binPoint_best = _binPoint;
	}
    }
  //  improvement_best -= penaltyTermPerBin(_wtMC, _nMC, binPoints.size());
  improvement_best -= penaltyTermPerBin(wtMC(hwt), wtMC(bwt), 
					accumulate(hwt.begin(), hwt.end(), 0.)/accumulate(bwt.begin(), bwt.end(), 0.), 
					_nMC, binPoints.size());

  if((improvement_best>0.02)) //&&(binPoints.size()<optimalBinning_MaximumNumberOfBins))
    {
      cout << "                    " << Math::nice(binPoint_best,+2) << "         " << Math::nice(improvement_best) << endl;
      binPoints.push_back(binPoint_best);
      improvement.push_back(improvement_best);
      chooseBinPoints_alreadySorted(h, hwt, b, bwt, binPoints, improvement, epsilonWt);
    }
  return;
}

void OptimalBinning::chooseBinPoints(vector<double> h, vector<double> hwt, 
				     vector<double> b, vector<double> bwt, 
				     vector<double>& binPoints, vector<double>& improvement, 
				     double epsilonWt)
{
  Math::parallelQuickSort(h,hwt);
  Math::parallelQuickSort(b,bwt);
  binPoints = improvement = vector<double>(0);
  chooseBinPoints_alreadySorted(h, hwt, b, bwt, binPoints, improvement, epsilonWt);
  return;
}

  
void OptimalBinning::chooseBinPoints(const vector<double>& h, const vector<double>& b, 
				     double wtMC, 
				     vector<double>& binPoints, vector<double>& improvement)
{
  vector<double> hwt, bwt;
  for(size_t i=0; i<h.size(); i++)
    hwt.push_back(wtMC);
  for(size_t i=0; i<b.size(); i++)
    bwt.push_back(wtMC);
  chooseBinPoints(h, hwt, b, bwt, binPoints, improvement, wtMC);
  return;
}


double OptimalBinning::minWeight(double epsilonWt)
{
  double ans = 10 * epsilonWt;
  return(ans);
}

