#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstOptimalBinning/interface/OptimalBinning.hh"
#include "Validation/VstQuaero/interface/Discriminator.hh"
#include <algorithm>
#include <numeric>
using namespace std;


istream &operator>>(istream& fin, Discriminator& discriminator)
{
  int nBins, ntags;
  fin >> ntags;
  string tag;
  for(int i=0; i<ntags; i++)
    {
      fin >> tag;
      fin >> discriminator.discriminants[tag];
    }
  fin >> discriminator.nBins;
  discriminator.binPoints = vector<double>(nBins-1);
  discriminator.hk = discriminator.bk = discriminator.hkerr = discriminator.bkerr = vector<double>(nBins);
  for(int i=0; i<nBins-1; i++)
    fin >> discriminator.binPoints[i];
  for(int i=0; i<nBins; i++)
    fin >> discriminator.hk[i];
  for(int i=0; i<nBins; i++)
    fin >> discriminator.bk[i];
  for(int i=0; i<nBins; i++)
    fin >> discriminator.hkerr[i];
  for(int i=0; i<nBins; i++)
    fin >> discriminator.bkerr[i];
  vector<bool> liveBins(nBins);
  int live;
  for(int i=0; i<nBins; i++)
    {
      fin >> live;
      discriminator.liveBins[i] = (live!=0);
    }
  fin >> discriminator.epsilonWt;
  return(fin);
}

ostream &operator<<(ostream& fout, Discriminator& discriminator)
{
  fout << discriminator.discriminants.size() << endl;
  for(map<string,Discriminant>::iterator i=discriminator.discriminants.begin(); i != discriminator.discriminants.end(); i++)
    {
      fout << i->first << endl;
      fout << i->second << endl;
    }
  fout << discriminator.nBins << endl;
  print(discriminator.binPoints, fout);
  print(discriminator.hk, fout);
  print(discriminator.bk, fout);
  print(discriminator.hkerr, fout);
  print(discriminator.bkerr, fout);
  print(discriminator.liveBins, fout);
  fout << discriminator.epsilonWt << endl;

  return(fout);
}

Discriminator::Discriminator(double _epsilonWt)
{
  epsilonWt = _epsilonWt;
  nBins=0;
}


void Discriminator::addDiscriminant(string tag, const Discriminant& discriminant)
{
  discriminants[tag] = discriminant;
}

void Discriminator::primePredictedD(string tag, 		
				    const vector<vector<double> >& hPoints, 
				    const vector<vector<double> >& bPoints, 
				    const vector<double>& _hWt, 
				    const vector<double>& _bWt)
{
  for(size_t i=0; i<hPoints.size(); i++)
    {
      hD.push_back(discriminants[tag].compute(hPoints[i]));
      hWt.push_back(_hWt[i]);
    }
  for(size_t i=0; i<bPoints.size(); i++)
    {
      bD.push_back(discriminants[tag].compute(bPoints[i]));
      bWt.push_back(_bWt[i]);
    }
}

void Discriminator::computeBinEdges()
{
  vector<double> improvement;
  binPoints = vector<double>(0);
  /*
  ofstream fout("yo.txt");
  fout << hD.size() << endl;  print(hD, fout);
  fout << hWt.size() << endl;  print(hWt, fout);
  fout << bD.size() << endl;  print(bD, fout);
  fout << bWt.size() << endl;  print(bWt, fout);
  fout.close();
  */
  
  OptimalBinning::chooseBinPoints(hD, hWt, bD, bWt, binPoints, improvement, epsilonWt);
  //cout << binPoints.size()+1 << " bins chosen." << endl;
  if(!binPoints.empty())
    sort(binPoints.begin(),binPoints.end());
  nBins = binPoints.size()+1;
  hwtk = vector<vector<double> >(nBins);
  bwtk = vector<vector<double> >(nBins);
  for(size_t i=0; i<hD.size(); i++)
    hwtk[upper_bound(binPoints.begin(),binPoints.end(),hD[i])-binPoints.begin()].push_back(hWt[i]);
  for(size_t i=0; i<bD.size(); i++)
    bwtk[upper_bound(binPoints.begin(),binPoints.end(),bD[i])-binPoints.begin()].push_back(bWt[i]);
  fillDbins();
  setLiveBins();
}

void Discriminator::fillDbins()
{
  double hTotalWeight=0, bTotalWeight=0, hEffectiveNumberOfEvents=1, bEffectiveNumberOfEvents=1;
  if(hWt.size()>0)
    {
      hTotalWeight = accumulate(hWt.begin(), hWt.end(), 0.);
      hEffectiveNumberOfEvents = Math::effectiveNumberOfEvents(hWt)+1;
    }
  if(bWt.size()>0)
    {
      bTotalWeight = accumulate(bWt.begin(), bWt.end(), 0.);
      bEffectiveNumberOfEvents = Math::effectiveNumberOfEvents(bWt)+1;
    }

  hk = bk = hkerr = bkerr = vector<double>(nBins);
  if(hwtk.empty())
    hwtk = vector<vector<double> >(nBins);
  if(bwtk.empty())
    bwtk = vector<vector<double> >(nBins);
  for(int i=0; i<nBins; i++)
    {      
      if(hwtk[i].size()>0)
	hk[i] = accumulate(hwtk[i].begin(), hwtk[i].end(), 0.);
      if(bwtk[i].size()>0)
	bk[i] = accumulate(bwtk[i].begin(), bwtk[i].end(), 0.);
      hkerr[i] = Math::computeMCerror(hwtk[i], epsilonWt);
      bkerr[i] = Math::computeMCerror(bwtk[i], epsilonWt);
    }
}

void Discriminator::setLiveBins()
{
  double minWt = OptimalBinning::minWeight(epsilonWt);
  liveBins = vector<bool>(nBins,false);
  // Live bins are those that contain at least minWt events predicted
  // by either h or b
  for(int i=0; i<nBins; i++)
    if( (hk[i] > minWt) ||
	(bk[i] > minWt) )
      liveBins[i] = true;
  // Throw away populous bins that do not greatly distinguish between h and b
  // This helps us perform the integral over systematic shifts by
  // reducing tight constraints from the data.
  for(int i=0; i<nBins; i++)
    if( (hk[i]+bk[i] > 50) &&
	( fabs(hk[i]-bk[i]) < sqrt(min(hk[i],bk[i])) ) )
      liveBins[i] = false;

}

vector<vector<double> > Discriminator::addToPredictedD(std::string tag, 		
				    const std::vector<std::vector<double> >& hPoints, 
				    const std::vector<std::vector<double> >& bPoints, 
				    const std::vector<double>& hWt, 
				    const std::vector<double>& bWt)
{
  vector<vector<double> > ans(2,vector<double>(nBins));
  if(hwtk.empty())
    hwtk = vector<vector<double> >(nBins);
  if(bwtk.empty())
    bwtk = vector<vector<double> >(nBins);

  for(size_t i=0; i<hPoints.size(); i++)
    {
      int iBin = upper_bound(binPoints.begin(),binPoints.end(),discriminants[tag].compute(hPoints[i]))-binPoints.begin();
      hwtk[iBin].push_back(hWt[i]);
      ans[0][iBin] += hWt[i];
    }
  for(size_t i=0; i<bPoints.size(); i++)
    {
      int iBin = upper_bound(binPoints.begin(),binPoints.end(),discriminants[tag].compute(bPoints[i]))-binPoints.begin();
      bwtk[iBin].push_back(bWt[i]);
      ans[1][iBin] += bWt[i];
    }
  return(ans);
}

vector<double> Discriminator::addToPredictedD(std::string tag, 
				    std::vector<std::vector<double> >& data)
{
  vector<double> ans(nBins);
  if(dk.empty())
    dk = vector<int>(nBins);
  for(size_t i=0; i<data.size(); i++)
    {
      int iBin = upper_bound(binPoints.begin(),binPoints.end(),discriminants[tag].compute(data[i]))-binPoints.begin();
      dk[iBin]++;
      ans[iBin]++;
    }
  return(ans);
}

void Discriminator::clearBinContents()
{
  hD.clear(); bD.clear();
  hWt.clear(); bWt.clear();
  hwtk.clear(); bwtk.clear();
  hk.clear(); bk.clear();
  hkerr.clear(); bkerr.clear(); dk.clear();
}

double Discriminator::computeRelevantLogLikelihoods(double& hLogLikelihood, double& bLogLikelihood, bool verbose)
{
  fillDbins();
  for(int i=0; i<nBins; i++)
    if(liveBins[i])
      {
	hLogLikelihood += log10(Math::probOfThisEffect(hk[i],dk[i],hkerr[i],"=="));
	bLogLikelihood += log10(Math::probOfThisEffect(bk[i],dk[i],bkerr[i],"=="));
      }
  if(verbose)
    for(int i=0; i<nBins; i++)
      {
	cout << "                  bin# " << i << ": ";
	if(liveBins[i])
	  cout << "d = " << dk[i] << " ; "
	       << "h = " << Math::nice(hk[i]) << " +- " << Math::nice(hkerr[i]) 
	    // << " (" << Math::nice(Math::probOfThisEffect(hk[i],dk[i],hkerr[i],"==")) << ")"
	       << " ; "
	       << "b = " << Math::nice(bk[i]) << " +- " << Math::nice(bkerr[i]) 
	    // << " (" << Math::nice(Math::probOfThisEffect(bk[i],dk[i],bkerr[i],"==")) << ")" 
	       << endl;
	else
	  cout << "dead" << endl;
      }
  double logLikelihoodRatio = hLogLikelihood - bLogLikelihood;
  return(logLikelihoodRatio);
}


void Discriminator::computeLogLikelihoods(double& hLogLikelihood, double& bLogLikelihood, bool verbose)
{
  computeRelevantLogLikelihoods(hLogLikelihood, bLogLikelihood, verbose);
  return;
}

double Discriminator::computeLogLikelihoodRatio(bool verbose)
{
  double hLogLikelihood, bLogLikelihood;
  return(computeRelevantLogLikelihoods(hLogLikelihood, bLogLikelihood, verbose));
}
  
double Discriminator::getExpectedEvidence(string _type)
{
  double nMC = (accumulate(hk.begin(),hk.end(),0.)+accumulate(bk.begin(),bk.end(),0.))/(2*epsilonWt); 
  double expectedEvidence = 
    OptimalBinning::binningFigureOfMerit(hk, bk, hkerr, bkerr, 
					 epsilonWt, nMC, true, false   // the expected evidence does not include the penalty factor
					 ).getExpectedEvidence(_type); 
  return(expectedEvidence);
}

