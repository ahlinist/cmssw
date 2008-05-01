/*********************************************
This file is used for random tasks
*********************************************/

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <string>
#include "QuaeroUtils/QuaeroEvent.hh"
#include "QuaeroUtils/QuaeroItemFile.hh"
#include "OptimalBinning/OptimalBinning.hh"
#include "QuaeroUtils/Kolmogorov.hh"
using namespace std;

void replace(string& s, string a, string b)
{
  cout << s << " " << a << " " << b << endl;
  int i;
  while((i=s.find(a))!=s.size())
    s.replace(i,a.length(),b);
  cout << "done replace!" << endl;
  return;
}

int main(int argc, char* argv[])
{
  cout << Math::sigma2prob(Math::roughMagnitudeOfDiscrepancy(270, 205.4, 5.4)) << endl;

  exit(1);

  assert(argc==2);
  string filename = argv[1];

  /*
  ifstream fin(filename.c_str());
  fin >> n;
  vector<double> hD(n);
  for(int i=0; i<n; i++)
    fin >> hD[i];
  fin >> n;
  vector<double> hWt(n);
  for(int i=0; i<n; i++)
    fin >> hWt[i];
  fin >> n;
  vector<double> bD(n);
  for(int i=0; i<n; i++)
    fin >> bD[i];
  fin >> n;
  vector<double> bWt(n);
  for(int i=0; i<n; i++)
    fin >> bWt[i];

  vector<double> binPoints, improvement;
  OptimalBinning::chooseBinPoints(hD, hWt, bD, bWt, binPoints, improvement);
  */


  /*
  cout << Math::probOfThisEffect(2.35,8) << endl;
  exit(1);

  system("date");
  string blah;
  ifstream fin(filename.c_str());
  ofstream fout("tmp1.txt");
  while(fin >> blah)
    fout << blah;
  ofstream fout("tmp1.txt");
  */

  QuaeroItemFile<QuaeroEvent> ef(filename);
  QuaeroEvent e;
  while(ef.nextEvent(e))
    {
      double phi = e.getThisObject("mu+",1)->getFourVector().phi();
      if((0<phi)&&(phi<M_PI/4))
	cout << e << endl;
    }

  /*
  string requestorName = "Daniel's Quaerobot";
  int ipos;
  cout << requestorName.find("'") << endl;
  while((ipos = requestorName.find("'")) != string::npos)
    {
      requestorName.erase(ipos,1);
      requestorName.insert(ipos,"");
    }
  cout << requestorName << endl;
  string requestorInstitution = "LBL";
  while((ipos = requestorInstitution.find("'"))!= string::npos)
    {
      requestorInstitution.erase(ipos,1);
      requestorInstitution.insert(ipos,"");
    }
  cout << requestorInstitution << endl;

  return(0);

  ifstream fin;
  fin.open("tmp.txt");
  cout << fin << endl;
  string blah;
  while(fin >> blah)
    ;
  fin.close();
  fin.clear();
  fin.open("tmp1.txt");
  cout << fin << endl;
  fin.close();


  Math::ExploreMyNeighborhoodGPS exploreMyNeighborhoodGPS(2);
  for(int i=0; i<10; i++)
    {
      exploreMyNeighborhoodGPS.move();
      print(exploreMyNeighborhoodGPS.getPosition());
    }


  GlobalVariables::id = "1";

  int d = 3;
  vector<double> mu_b = vector<double>(d);
  vector<double> mu_h = vector<double>(d);
  vector<vector<double> > sigma = vector<vector<double> >(d,vector<double>(d,0));
  for(int i=0; i<d; i++)
    {
      mu_b[i] = 2*i-0.5;
      mu_h[i] = 2*i+0.5;
      sigma[i][i] = 1;
    }

  int n=1000;
  double wt = 0.01;
  vector<vector<double> > hPoints(n), bPoints(n);
  for(int i=0; i<n; i++)
    {
      bPoints[i] = Math::randMultiGauss(mu_b,sigma);
      hPoints[i] = Math::randMultiGauss(mu_h,sigma);
    }
  int nData = (int)(n*wt);
  vector<vector<double> > dPoints(nData);
  for(int i=0; i<nData; i++)
    dPoints[i] = Math::randMultiGauss(mu_h,sigma);

  vector<double> hWt, bWt;
  hWt = bWt = vector<double>(n,wt);
  
  KernelEstimate hEstimate, bEstimate;
  hEstimate.derive(hPoints,hWt);
  bEstimate.derive(bPoints,bWt);
  Discriminant discriminant(hEstimate,bEstimate);

  vector<double> hDiscriminants, bDiscriminants;
  for(int i=0; i<hPoints.size(); i++)
    hDiscriminants.push_back(discriminant.compute(hPoints[i]));
  for(int i=0; i<bPoints.size(); i++)
    bDiscriminants.push_back(discriminant.compute(bPoints[i]));

  Discriminator discriminator(hPoints, bPoints, hWt, bWt, wt);
  double hLogL, bLogL, logL;
  discriminator.computeLogLikelihoods(dPoints,hLogL,bLogL);
  logL = hLogL - bLogL;
  cout << "logL = " << logL << endl;

  vector<string> distributionNames(d);
  for(int i=0; i<d; i++)
    distributionNames[i] = "var"+Math::ftoa(i+1);
  GraphicalOutput graphicalOutput;
  vector<vector<double> > range;
  vector<int> nbins;
  Math::makeNiceHistogramRange(bWt, hWt, hPoints, bPoints, dPoints, range, nbins);
  graphicalOutput.addDiscriminatorPlots(distributionNames, discriminator, range, dPoints);
  graphicalOutput.draw();

  const int N = 40;
  const double wt = 0.1;
  vector<double> hPoints(N), bPoints(N);
  drand48();
  for(int i=0; i<N; i++)
    {
      hPoints[i] = drand48();
      bPoints[i] = drand48();
    }
  sort(hPoints.begin(), hPoints.end());
  sort(bPoints.begin(), bPoints.end());
  cout << "h = "; print(hPoints);
  cout << "b = "; print(bPoints);
  vector<double> hWt(N,wt), bWt(N,wt);

  hWt[0] = 1e-4; hPoints[0] = 0.00000017;
  double binPoint, improvement;
  vector<double> binPoints, improvements;
  OptimalBinning::chooseBinPoints_alreadySorted(hPoints, hWt, bPoints, bWt, binPoints, improvements, wt);
  //cout << binPoint << " * " << improvement << endl;
  print(binPoints);
  print(improvements);

  vector<vector<double> > hPoints(N), bPoints(N);
  for(int i=0; i<N; i++)
    hPoints[i] = vector<double>(1,Math::gasdev(1,1));
  for(int i=0; i<N; i++)
    bPoints[i] = vector<double>(1,Math::gasdev(-1,1));

  KernelEstimate hEstimate, bEstimate;
  hEstimate.derive(hPoints,hWt);
  bEstimate.derive(bPoints,bWt);
  Discriminant discriminant(hEstimate,bEstimate);

  vector<double> hDiscriminants, bDiscriminants;
  for(int i=0; i<hPoints.size(); i++)
    hDiscriminants.push_back(discriminant.compute(hPoints[i]));
  for(int i=0; i<bPoints.size(); i++)
    bDiscriminants.push_back(discriminant.compute(bPoints[i]));
  
  ofstream ftmp1("tmp1.txt");
  ofstream ftmp2("tmp2.txt");
  for(int i=0; i<hDiscriminants.size(); i++)
    ftmp1 << hDiscriminants[i] << endl;
  for(int i=0; i<bDiscriminants.size(); i++)
    ftmp2 << bDiscriminants[i] << endl;
  ftmp1.close();
  ftmp2.close();

  Discriminator discriminator(hPoints, bPoints, hWt, bWt, wt);
  cout << discriminator << endl;


  string colliderRun = "tev2";
  string experiment = "cdf";
  string fs = "1e+1e-1pmiss2j";
  
  string sigFilename = "tmp/4/-1/tev2/cdf/sig/sig_"+fs+".txt";
  string bkgFilename = "tmp/4/-1/tev2/cdf/bkg/bkg_"+fs+".txt";
  string dataFilename = "experiments/tev2/cdf/data/data_"+fs+".txt";
  
  vector<QuaeroEvent> sigEvents = QuaeroItemFile<QuaeroEvent>(sigFilename).loadEvents();
  vector<QuaeroEvent> bkgSMEvents = QuaeroItemFile<QuaeroEvent>(bkgFilename).loadEvents();
  vector<QuaeroEvent> bkgEvents = QuaeroItemFile<QuaeroEvent>(bkgFilename).loadEvents(vector<string>(1,"pyth_tt"));  
  vector<QuaeroEvent> dataEvents = QuaeroItemFile<QuaeroEvent>(dataFilename).loadEvents();
  vector<QuaeroEvent> sigbkgEvents = bkgEvents;
  sigbkgEvents.insert(sigbkgEvents.end(),sigEvents.begin(),sigEvents.end());

  vector<string> distributionNames;
  vector<double> weightsBkgSM, weightsSigBkg, weightsData;
  vector<vector<double> > variableValuesBkgSM, variableValuesSigBkg, variableValuesData;
  vector<double> hWt, bWt;
  vector<vector<double> > hPoints, bPoints;

  RelevantVariables relevantVariables;
  bool sufficientlyDifferent = relevantVariables.compute(true, bkgSMEvents, sigbkgEvents, dataEvents, colliderRun, experiment, fs, distributionNames, weightsBkgSM, weightsSigBkg, weightsData, variableValuesBkgSM, variableValuesSigBkg, variableValuesData);

  hPoints = variableValuesSigBkg;
  bPoints = variableValuesBkgSM;
  hWt = weightsSigBkg;
  bWt = weightsBkgSM;

  KernelEstimate hEstimate, bEstimate;
  hEstimate.derive(hPoints,hWt);
  bEstimate.derive(bPoints,bWt);
  Discriminant discriminant(hEstimate,bEstimate);

  vector<double> hDiscriminants, bDiscriminants;
  for(int i=0; i<hPoints.size(); i++)
    hDiscriminants.push_back(discriminant.compute(hPoints[i]));
  for(int i=0; i<bPoints.size(); i++)
    bDiscriminants.push_back(discriminant.compute(bPoints[i]));

  cout << "hEstimate" << endl << hEstimate << endl;
  cout << "bEstimate" << endl << bEstimate << endl;

  KernelEstimate bkgSMKernelEstimate;
  bkgSMKernelEstimate.derive(variableValuesBkgSM,weightsBkgSM);

  Discriminator discriminator(variableValuesSigBkg, variableValuesBkgSM, weightsSigBkg, weightsBkgSM);

  double deltaHypothesisLikelihood=0., deltasmLikelihood=0., deltaLogLikelihood=0.;
  discriminator.computeLogLikelihoods(variableValuesData,deltaHypothesisLikelihood,deltasmLikelihood);	  
  deltaLogLikelihood = deltaHypothesisLikelihood - deltasmLikelihood;

  cout << Math::computeSum(weightsSigBkg) << " " << Math::computeSum(weightsBkgSM) << " " << Math::computeSum(weightsData) << endl;
  cout << deltaLogLikelihood << endl;
  */





  /*
    int d=1;
  int n=100;
  vector<vector<double> > hPoints, bPoints;
  vector<double> hWt, bWt;
  for(int i=0; i<n; i++)
    {
      hPoints.push_back(vector<double>(d));
      bPoints.push_back(vector<double>(d));
      for(int j=0; j<d; j++)
	{
	  hPoints[i][j] = Math::gasdev(1,1);
	  bPoints[i][j] = Math::gasdev(-1,1);
	}
      hWt.push_back(1./n);
      bWt.push_back(1./n);
    }
  
  Discriminator discriminator(hPoints, bPoints, hWt, bWt);

  ofstream ffunc("tmp1.txt");
  for(double z=-3; z<3; z+=0.1)
    {
      vector<vector<double> > x(1,vector<double>(1,z));
      ffunc << z << " " << discriminator.computeLogLikelihood(x) << endl;
    }



  Kernel kernel1 = Kernel(vector<double>(1,-1),vector<vector<double> >(1,vector<double>(1,1)));
  Kernel kernel2 = Kernel(vector<double>(1,+1),vector<vector<double> >(1,vector<double>(1,1)));
  vector<Kernel> kernelList;
  kernelList.push_back(kernel1);
  kernelList.push_back(kernel2);
  KernelEstimate kernelEstimate(vector<double>(2,1),kernelList);
  double y;
  while(cin >> y)
    {
      vector<double> x = vector<double>(1,y);
      cout << "ans = " << kernelEstimate.evaluate(x) << endl;
    }

  */
  /*
  QuaeroItemFile<QuaeroEvent> eventFile("Data/AnaData/metaData_ucn.txt");
  QuaeroEvent e;
  ofstream ftmp("Data/AnaData/tmp.txt");
  while(eventFile.nextEvent(e))
    {
      string runNumber = e.getRunNumber().substr(0,6);
      if((e.numberOfObjects("ph")>=1)&&
	 (Math::MPEquality(e.getThisObject("ph")->getFourVector().phi(), 116.25*M_PI/180, 0.01*M_PI/180))&&
	 (e.getThisObject("ph")->getFourVector().perp()>100))
	cout << e << endl;
      else
	ftmp << e << endl;
    }
  ftmp.close();
  return(0);
  */
  /*
  Detector detector("cdf");
  PartitionRule partitionRule("Tevatron-exclusive",Detector("cdf"));
  partitionRule.partition("tmp1.txt");

  QuaeroItemFile<QuaeroEvent> eventFile("tmp.txt");
  QuaeroEvent e;
  while(eventFile.nextEvent(e))
    if(e.numberOfObjects("mu")==1)
      if(e.getWeight()>0.1)
	cout << e.getThisObject("mu")->getFourVector().eta() << endl;

  double minDeltaR = 10.;
  QuaeroFSObject o1, o2;
  vector<QuaeroFSObject> o = e.getObjects();
  for(int i=0; i<o.size(); i++)
    for(int j=i+1; j<o.size(); j++)
      {
	double deltaR = o[i].getFourVector().deltaR(o[j].getFourVector());
	cout << deltaR << endl;
	if(deltaR < minDeltaR)
	  {
	    minDeltaR = deltaR;
	    o1 = o[i]; o2 = o[j];
	  }
      }

  cout << minDeltaR << endl;
  cout << o1.getFourVector() << " " << o1.getFourVector().eta() << "," << o1.getFourVector().phi() << endl;
  cout << o2.getFourVector() << " " << o2.getFourVector().eta() << "," << o2.getFourVector().phi() << endl;


  //detector.getFiducialArea().mask(e); // require all objects to be in appropriate fiducial regions
  cout << detector.getTrigger().doesThisEventPass(e) << endl;
  cout << partitionRule.getFinalState(e).getTextString() << endl;
  //Trigger trigger("TevatronHighPt");
  //cout << trigger.doesThisEventPass(e) << endl;
  */
  return(0);
}
