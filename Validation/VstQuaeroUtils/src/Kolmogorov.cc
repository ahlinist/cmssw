/******************************************
Implementation of the Kolmogorov class
Bruce Knuteson 2003
******************************************/

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/Kolmogorov.hh"
using namespace std;


Kolmogorov::Kolmogorov(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt, 
		       const std::vector<double>& _datapts, const std::vector<double> _datawt)
{
  bkgpts = _bkgpts;
  bkgwt = _bkgwt;
  assert(bkgpts.size()==bkgwt.size());
  datapts = _datapts;
  datawt = _datawt;
  if(datawt.empty())
    datawt = vector<double>(datapts.size(),1);
  assert(datapts.size()==datawt.size());
  return;
}



Kolmogorov::Kolmogorov(const std::vector<double>& _bkgpts, const std::vector<double>& _bkgwt) {
  //use pseudo-data
  bkgpts = _bkgpts;
  bkgwt = _bkgwt;
  assert(bkgpts.size()==bkgwt.size());
  datapts = producePseudoData(bkgpts, bkgwt);
  datawt = vector<double>(datapts.size(),1);
  assert(datapts.size()==datawt.size());
  return;
}


vector<double> Kolmogorov::producePseudoData(const vector<double>& _bkgpts, const vector<double>& _bkgwt) {
  // produce a set of pseudo-data, using the trial and error method.
  // Function taken from BumpStatistic.cc and edited everslightly.
  bool debug=false;
  double biggestBkgPosition = -1000000;
  double smallestBkgPosition = 1000000;
  for ( size_t i=0 ; i < _bkgpts.size() ; ++i ){
    if ( _bkgpts[i] < smallestBkgPosition )
      smallestBkgPosition=_bkgpts[i];
    if ( _bkgpts[i] > biggestBkgPosition )
      biggestBkgPosition=_bkgpts[i];
  }
  if(debug) cout << " bkg between " << smallestBkgPosition << " and " << biggestBkgPosition << endl;
  double bkgBinSize = (biggestBkgPosition - smallestBkgPosition)/3000; 
  int nBkgBins = (int)((biggestBkgPosition-smallestBkgPosition)/bkgBinSize) + 5; // to be sure we have enough space. The background histogram will span from 0 up to bkgBinSize*nBkgBins GeV/c^2
  if(debug) cout << " bkgBinSize= " << bkgBinSize << " nBkgBins " << nBkgBins << endl;
  assert(smallestBkgPosition+bkgBinSize*nBkgBins>=biggestBkgPosition); // assert we have enough bins to cover all the background range.

  double sum_bkg = 0;
  vector<double> bkgBins = vector<double>(nBkgBins,0);
  double biggestBkgBinContent = 0; 
  if(debug) cout << " _bkgpts.size()= " << _bkgpts.size() << endl;
  for (size_t i=0; i< _bkgpts.size(); ++i){
    //    if(debug) cout << "i= " << i ;
    sum_bkg += _bkgwt[i];
    //    if(debug) cout << " sum_bkg= " << sum_bkg ;
    int bkgBinNumber = (int)((_bkgpts[i]-smallestBkgPosition)/bkgBinSize);
    //    if(debug) cout << " bkgBinNumber= " << bkgBinNumber ;
    assert(bkgBinNumber < nBkgBins);
    bkgBins[bkgBinNumber] += _bkgwt[i];
    //    if(debug) cout << " whichHasSoFar= " << bkgBins[bkgBinNumber] << endl ;
    if(bkgBins[bkgBinNumber] > biggestBkgBinContent) biggestBkgBinContent = bkgBins[bkgBinNumber];
  }
  if(debug) cout << " biggestBkgBinContent= " << biggestBkgBinContent << endl;
  //do the cumulative vector now
  assert(sum_bkg>0);
  vector<double> cumulativeBkgBins = vector<double>(nBkgBins,0);
  cumulativeBkgBins[0]=bkgBins[0]/sum_bkg;
  for (int i=1; i<nBkgBins; ++i){
    cumulativeBkgBins[i] = cumulativeBkgBins[i-1] + bkgBins[i]/sum_bkg;
  }
  //Now that the background has been read, produce the pseudo-data
  long int numOfData = Math::poisson(sum_bkg);
  if(debug) cout << " numOfData_toProduce= " << numOfData << endl;
  vector<double> pseudoDataPts;
  long int producedData = 0;
  srand48((unsigned)( time(NULL)*getpid() ));
  while (producedData < numOfData)    {
    double r = drand48();
    //find bin that has cumulative just higher than r, using Newton's method, exploiting the monotonically increasing cumulative distribution
    int leftBin  = 0;
    int rightBin = nBkgBins-1;
    int middleBin= (leftBin+rightBin)/2;
    while (!(middleBin==leftBin+1)) {
      assert(leftBin<=middleBin && middleBin<=rightBin);
      if ( cumulativeBkgBins[middleBin]==r ) 
	break;
      else if ( cumulativeBkgBins[middleBin]>r ) rightBin = middleBin;
      else if ( cumulativeBkgBins[middleBin]<r ) leftBin = middleBin;
      middleBin= (leftBin+rightBin)/2;
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
    //Find where exactly to put the datum. Put it uniformly distributed in the plateau, i.e. between the end of left and beginning of right bin.
    double rightEdgeOfLeft = smallestBkgPosition + (leftBin+1)*bkgBinSize;
    double leftEdgeOfRight = smallestBkgPosition + (rightBin)*bkgBinSize;
    double datumPosition  = rightEdgeOfLeft + drand48()*(leftEdgeOfRight-rightEdgeOfLeft);
    pseudoDataPts.push_back(datumPosition);
    producedData++;
  }  

  return (pseudoDataPts);
}


void Kolmogorov::printBkgAndData() {

  cout << "bkg is:" << endl;
  for ( size_t i=0; i < bkgpts.size(); ++i ) cout << bkgpts[i] << " " << bkgwt[i] << endl;
  cout << "pseudodata are:" << endl;
  for ( size_t i=0; i < datapts.size(); ++i ) cout << datapts[i] << endl;
  
  return;
}

double Kolmogorov::probability()
  // Takes as input a vector of background points, a vector of the same length of the weight of those points,
  // a vector of data points, and a vector of the same length of the weight of those points.
  // Calculates the Kolmogorov statistic, and converts it to a probability
{
  double n1=0, n2=0, n=0;
  if(datawt.size()==0)
    n1 = datapts.size();
  else
    n1 = Math::effectiveNumberOfEvents(datawt);
  n2 = Math::effectiveNumberOfEvents(bkgwt);
  if((n1==0)&&(n2==0))
    return(1);
  else
    n = n1*n2/(n1+n2);
  return(probability(deviation(), n));
}


double Kolmogorov::probability(double largestDeviation, double N)
  // takes as input the KS statistic (largest deviation between the integral of the two curves), and an effective number of points.  (The KS statistic assumes that one curve is perfectly known, and the other is estimated using N points.  If both curves are estimated using a finite number of points, one can cheat slightly and use an "effective" number of points.)
  // Reference: http://www.nrbook.com/a/bookcpdf/c14-3.pdf, pg. 624-625
{
  assert(largestDeviation<=1.);
  //  if(largestDeviation<0.02)
  //    return(1.); // if fit is very very good, Kolmogorov probability is 1
  if(N<=0)
    return(1.); // if no events, Kolmogorov probability is 1
  largestDeviation *= (sqrt(N)+0.12+0.11/sqrt(N));  // approximation from Numerical Recipes
  if(largestDeviation<1.e-4)
    return(1.); // if fit is perfect, Kolmogorov probability is 1
  double p = 0.;
  for(int j=1; j<10./largestDeviation; j++)
    p += (-2)*pow(-1.,j)*exp((-2)*pow(j*largestDeviation,2));
  assert(p>=0.);
  return(p);
}

double Kolmogorov::deviation()
{
  // Takes as input a vector of background points, a vector of the same length of the weight of those points,
  // a vector of data points, and a vector of the same length of the weight of those points.
  // Calculates the Kolmogorov statistic

  assert(bkgwt.size()==bkgpts.size());
  assert(bkgwt.size()>0);
  if(datawt.empty())
    datawt = vector<double>(datapts.size(),1.);
  assert(datawt.size()==datapts.size());
  vector< pair<double,double> > bkg(bkgwt.size(),pair<double,double>(0,0));
  vector< pair<double,double> > data(datawt.size(),pair<double,double>(0,0));
  double totalbkgwt=0., totaldatawt=0.;
  for(size_t i=0; i<bkgwt.size(); i++)
    totalbkgwt += bkgwt[i];
  for(size_t i=0; i<bkgwt.size(); i++)
    bkgwt[i] /= totalbkgwt;
  for(size_t i=0; i<datawt.size(); i++)
    totaldatawt += datawt[i];
  for(size_t i=0; i<datawt.size(); i++)
    datawt[i] /= totaldatawt;

  for(size_t i=0; i<bkg.size(); i++)
    {
      bkg[i].first = bkgpts[i];
      bkg[i].second = bkgwt[i];
    }
  for(size_t i=0; i<data.size(); i++)
    {
      data[i].first = datapts[i];
      data[i].second = datawt[i];
    }
  sort(bkg.begin(), bkg.end());
  sort(data.begin(), data.end());

  double cumulativeBkg = 0., cumulativeData = 0., cumulativeData1 = 0.;
  size_t bkgcounter = 0;
  double largestDeviation = 0.;
  double placeOfLargestDeviation=0, dataAtPlaceOfLargestDeviation=0, bkgAtPlaceOfLargestDeviation=0;

  for(size_t i=0; i<data.size(); i++)
    {
      cumulativeData = cumulativeData1;
      cumulativeData1 += data[i].second;
      if(((i+1)<data.size())&&(data[i].first==data[i+1].first))
	continue; // if several events have the same variable value, compare that point to background only once
      assert(!Math::isNaNQ(bkg[bkgcounter].first));
      while((bkgcounter<bkg.size())&&(bkg[bkgcounter].first<=data[i].first))
	{
	  cumulativeBkg += bkg[bkgcounter].second;
	  bkgcounter++;
	}
      double deviation = max(fabs(cumulativeData - cumulativeBkg),
			     fabs(cumulativeData1 - cumulativeBkg));      
      if(deviation > largestDeviation)
	{
	  largestDeviation = deviation;
	  if(largestDeviation>1)   // fix possible roundoff error
	    largestDeviation = 1;  
	  placeOfLargestDeviation = data[i].first;
	  dataAtPlaceOfLargestDeviation = cumulativeData;
	  bkgAtPlaceOfLargestDeviation = cumulativeBkg;
	}
    }
  // cout << "( " << placeOfLargestDeviation << " , " << dataAtPlaceOfLargestDeviation << " , " << bkgAtPlaceOfLargestDeviation << " )" << endl;

  return(largestDeviation);
}

void Kolmogorov::identifyRelevantVariables(vector<size_t>& whichVariables, vector<double>& similarity, 
					   const vector<vector<double> > & h1, const vector<vector<double> >& h2, 
					   vector<double> h1wt, vector<double> h2wt, 
					   int nVarsToUse, bool keepAllVariables)
{

  whichVariables = vector<size_t>(0);
  similarity = vector<double>(0);
  if((h1.size()==0)||(h2.size()==0))
    return;
  if(h1wt.size()==0)
    h1wt = vector<double>(h1.size(),1);
  if(h2wt.size()==0)
    h2wt = vector<double>(h2.size(),1);
  
  int ndists = h1[0].size();
  vector<vector<double> > similarities(ndists);

  vector<double> h2v(h2.size());
  vector<double> h1v(h1.size());
  for(int i=0; i<ndists; i++)
    {
      /*for(int j=0; j<h2.size(); j++)
	h2v[j] = h2[j][i];
      for(int j=0; j<h1.size(); j++)
	h1v[j] = h1[j][i]; */
      std::cout << " i = " << i << endl;
      for(size_t j=0; j<h2.size(); j++) {
	h2v[j] = h2[j][i];
	std::cout << h2v[j] << " ";
      }
      std::cout << endl;
      for(size_t j=0; j<h1.size(); j++) {
	h1v[j] = h1[j][i];
	std::cout << h1v[j] << " ";
      }
      std::cout << endl;
      for(size_t j=0; j<h2wt.size(); j++) {
	std::cout << h2wt[j] << " ";
      }
      std::cout << endl;
      for(size_t j=0; j<h1wt.size(); j++) {
	std::cout << h1wt[j] << " ";
      }
      std::cout << endl;
      similarities[i] = vector<double>(2);
      similarities[i][0] = 1. - Kolmogorov(h2v, h2wt, h1v, h1wt).deviation();
      similarities[i][1] = i;
      
    }
  sort(similarities.begin(), similarities.end());

  vector<double> minimallyInteresting = vector<double>(2);
  minimallyInteresting[0] = 1-((1-similarities[0][0])/4);
  minimallyInteresting[1] = 0;
  vector<vector<double> >::iterator k = upper_bound(similarities.begin(), similarities.end(), minimallyInteresting);
  if(k!=similarities.end()&&(!keepAllVariables))
    similarities.erase(k, similarities.end());
    
  int nvars=0;
  if(nVarsToUse<0)
    nvars = 
      (int)Math::min( 
		     (int)(1.0*log10(Math::min(Math::effectiveNumberOfEvents(h2wt),
					       Math::effectiveNumberOfEvents(h1wt)))),
		     ndists); // should be 0.5*log10(.) for base 100, 0.7685*log10(.) for base 20, etc.
  else
    {
      nvars = (int)Math::min(ndists, nVarsToUse);
      if(!keepAllVariables)
	nvars = (int)Math::min(nvars,
			       (int)(Math::min(Math::effectiveNumberOfEvents(h2wt),
					       Math::effectiveNumberOfEvents(h1wt))
				     -2));
    }
  if(nvars<0)
    nvars=0;
  //cout << "nvars = " << nvars << "; " << Math::effectiveNumberOfEvents(h2wt) << "; " << Math::effectiveNumberOfEvents(h1wt) << endl;

  double totalh2Weight=0., totalh1Weight=0.;
  for(size_t i=0; i<h2wt.size(); i++)
    totalh2Weight += h2wt[i];
  for(size_t i=0; i<h1wt.size(); i++)
    totalh1Weight += h1wt[i];

  {
    // ensure that highly correlated variables are not used together
    size_t nVar = 1;
    while((int) nVar<=nvars)
      {
	bool thisVariableIsOkay = false;
	if(!keepAllVariables)
	  {
	    { // h2

	      matrix covarianceMatrix(nVar,nVar);
	      vector<double> average(nVar);
	      for(size_t j=0; j<nVar; j++)
		for(size_t i=0; i<h2.size(); i++)
		  average[j] += h2wt[i]*h2[i][(int)similarities[j][1]]/totalh2Weight;
	      for(size_t j=0; j<nVar; j++)
		for(size_t k=0; k<nVar; k++)
		  for(size_t i=0; i<h2.size(); i++)
		    covarianceMatrix[j][k] += h2wt[i]*(h2[i][(int)similarities[j][1]]-average[j])*(h2[i][(int)similarities[k][1]]-average[k])/totalh2Weight;
	      matrix correlationMatrix(nVar,nVar);
	      for(size_t j=0; j<nVar; j++)
		for(size_t k=0; k<nVar; k++)
		  correlationMatrix[j][k] = covarianceMatrix[j][k]/(sqrt(covarianceMatrix[j][j])*sqrt(covarianceMatrix[k][k]));
	      
	      vector<double> eigenvalues = correlationMatrix.getEigenvalues();
	      sort(eigenvalues.begin(), eigenvalues.end());
	      if(eigenvalues[0]>0.99/nVar)
		thisVariableIsOkay = true;
	    }
	    { // h1
	      matrix covarianceMatrix(nVar,nVar);
	      vector<double> average(nVar);
	      for(size_t j=0; j<nVar; j++)
		for(size_t i=0; i<h1.size(); i++)
		  average[j] += h1wt[i]*h1[i][(int)similarities[j][1]]/totalh1Weight;
	      for(size_t j=0; j<nVar; j++)
		for(size_t k=0; k<nVar; k++)
		  for(size_t i=0; i<h1.size(); i++)
		    covarianceMatrix[j][k] += h1wt[i]*(h1[i][(int)similarities[j][1]]-average[j])*(h1[i][(int)similarities[k][1]]-average[k])/totalh1Weight;
	      matrix correlationMatrix(nVar,nVar);
	      for(size_t j=0; j<nVar; j++)
		for(size_t k=0; k<nVar; k++)
		  correlationMatrix[j][k] = covarianceMatrix[j][k]/(sqrt(covarianceMatrix[j][j])*sqrt(covarianceMatrix[k][k]));
	      vector<double> eigenvalues = correlationMatrix.getEigenvalues();
	      sort(eigenvalues.begin(), eigenvalues.end());
	      if(eigenvalues[0]>0.50/nVar)
		{
		  thisVariableIsOkay = true;
		  //cout << "smallest eigenvalue = " << eigenvalues[0] << endl;
		}
	    }
	  }
	if(thisVariableIsOkay || keepAllVariables)
	  nVar++;
	else
	  {
	    // remove similarities[nVar-1] from similarities
	    for(size_t k=nVar-1; k<similarities.size()-1; k++)
	      similarities[k] = similarities[k+1];
	    similarities.pop_back();
	    if((int) similarities.size()<nvars)
	      nvars=similarities.size(); // reduce the number of variables we use if we don't have that many options
	    assert(nvars>=0);
	  }
      }
  }
  if((int) similarities.size()>nvars)
    similarities.erase(similarities.begin()+nvars,similarities.end());
  whichVariables = vector<size_t>(similarities.size());
  for(size_t i=0; i<similarities.size(); i++)
    whichVariables[i] = (size_t) similarities[i][1];
  similarity = vector<double>(similarities.size());
  for(size_t i=0; i<similarities.size(); i++)
    similarity[i] = similarities[i][0];
  return;
}
 

