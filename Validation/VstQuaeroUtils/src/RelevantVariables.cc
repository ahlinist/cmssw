
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cfloat>
#include <algorithm>
#include <numeric>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/Kolmogorov.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroEvent.hh"
#include "Validation/VstQuaeroUtils/interface/RelevantVariables.hh"

using namespace std;

bool useGeneratedSumPt = (getenv("RelevantVariablesUseGeneratedSumPt")==NULL ? false : true);
bool useRunHistory = true;
bool useMonteCarloRunNumbers = false;
bool useZvtx = false;

RelevantVariables::RelevantVariables(vector<double> _resonanceMasses)
{
  resonanceMasses = _resonanceMasses;
  objectList = vector<string>(0);
  objectList.push_back("e+");
  objectList.push_back("e-");
  objectList.push_back("mu+");
  objectList.push_back("mu-");
  objectList.push_back("tau+");
  objectList.push_back("tau-");
  objectList.push_back("ph");
  objectList.push_back("j");
  objectList.push_back("b");
  objectList.push_back("pmiss");
  objectList.push_back("w");
  objectList.push_back("z");
  locked = "";

  integratedLumiMap.clear();
  privilegeLevel=0;
}

void RelevantVariables::setPrivilegeLevel(int _privilegeLevel)
{
  privilegeLevel = _privilegeLevel;
}


ostream& operator<<(ostream & fout, const RelevantVariables & relevantVariables)
{
  fout << relevantVariables.sufficientlyDifferent << endl;
  fout << relevantVariables.distributions.size() << endl;
  print(relevantVariables.distributions,fout);
  return(fout);
}

istream& operator>>(istream& fin, RelevantVariables & relevantVariables)
{
  fin >> relevantVariables.sufficientlyDifferent;
  read<int>(relevantVariables.distributions,fin);
  return(fin);
}

void RelevantVariables::identify(const vector<vector<double> >& variableValuesCommon, 
				 const vector<double>& weightsCommon,
				 const vector<vector<double> >& variableValuesUniqueToH, 
				 const vector<double>& weightsUniqueToH, 
				 const vector<vector<double> >& variableValuesUniqueToSM, 
				 const vector<double>& weightsUniqueToSM, 
				 string colliderRun, string experiment, string fs, 
				 int nVarsToUse, bool keepHighlyCorrelatedVariables)
{
  string colliderType = "";
  if(colliderRun=="lep2")
    colliderType = "lepton";
  if((colliderRun=="hera")||(colliderRun=="tev1")||
     (colliderRun=="tev2")||(colliderRun=="lhc"))
    colliderType = "hadron";
  assert(colliderType!="");
  if((colliderRun=="hera")&&(experiment=="h1")&&(!keepHighlyCorrelatedVariables))
    nVarsToUse = 1; // In order to get Quaero@H1 through the H1 collaboration, we are only allowed to use 1 variable!
  bool fullVariableList = keepHighlyCorrelatedVariables;
  vector<string> distributionNamesForThisFinalState = getAllVariableNamesForThisEvent(fs,true,fullVariableList,colliderRun);

  vector<size_t> whichVariables;
  vector<double> similarity;  
  vector<vector<double> > hVariableValues = variableValuesCommon, smVariableValues = variableValuesCommon;
  if(hVariableValues.empty())
    hVariableValues = variableValuesUniqueToH;
  else
    hVariableValues.insert(hVariableValues.end(), variableValuesUniqueToH.begin(), variableValuesUniqueToH.end());
  if(smVariableValues.empty())
    smVariableValues = variableValuesUniqueToSM;
  else
    smVariableValues.insert(smVariableValues.end(), variableValuesUniqueToSM.begin(), variableValuesUniqueToSM.end());

  vector<double> hWeights = weightsCommon, smWeights = weightsCommon;
  if(hWeights.empty())
      hWeights = weightsUniqueToH;
  else
      hWeights.insert(hWeights.end(), weightsUniqueToH.begin(), weightsUniqueToH.end());
  if(smWeights.empty())
      smWeights = weightsUniqueToSM;
  else
      smWeights.insert(smWeights.end(), weightsUniqueToSM.begin(), weightsUniqueToSM.end());

  Kolmogorov::identifyRelevantVariables(whichVariables, similarity, hVariableValues, smVariableValues, hWeights, smWeights, nVarsToUse, keepHighlyCorrelatedVariables);

  size_t nvars = whichVariables.size();
  distributions = vector<int>(nvars);
  distributionNames = vector<string>(nvars);
  for(size_t i=0; i<nvars; i++)
    {
      distributions[i] = whichVariables[i];
      if(whichVariables[i]>=distributionNamesForThisFinalState.size())
	{
	  cout << whichVariables[i] << "   " << distributionNamesForThisFinalState.size() << endl;
	  for(size_t j=0; j<whichVariables[i]; j++)
	    {
	      string s = ""; 
	      if(j<distributionNamesForThisFinalState.size()) 
		s = distributionNamesForThisFinalState[j];
	      cout << s << Math::spacePad(30-s.length());
	      if(j<smVariableValues[0].size())
		cout << smVariableValues[0][j];
	      cout << endl;
	    }
	}
      assert(whichVariables[i]<distributionNamesForThisFinalState.size());
      distributionNames[i] = distributionNamesForThisFinalState[whichVariables[i]];
    }
  double totalCommonWeight = 0;
  if(!weightsCommon.empty())
    totalCommonWeight = accumulate(weightsCommon.begin(), weightsCommon.end(), 0.);
  double totalBkgSMWeight = totalCommonWeight, totalSigBkgWeight = totalCommonWeight;
  if(!weightsUniqueToSM.empty())
    totalBkgSMWeight += accumulate(weightsUniqueToSM.begin(), weightsUniqueToSM.end(), 0.);
  if(!weightsUniqueToH.empty())
    totalSigBkgWeight += accumulate(weightsUniqueToH.begin(), weightsUniqueToH.end(), 0.);
  sufficientlyDifferent = 
    (!(
      (fabs(totalBkgSMWeight-totalSigBkgWeight)<Math::min(sqrt(totalBkgSMWeight),totalBkgSMWeight)/10.) && // is the weight of the signal negligible? //bktemp -- not quite what we want
      ((nvars==0)||(1.-similarity[0]<1./(Math::min(sqrt(totalBkgSMWeight),totalBkgSMWeight)*10.)))) // is the resulting shape equivalent to the SM background? //bktemp -- not quite what we want
    );
  locked = colliderRun + " " + experiment + " " + fs; // don't let the variables identified for this class change

  return;
}


bool RelevantVariables::compute(
				// input
				const vector<QuaeroEvent> & commonEvents, 
				const vector<QuaeroEvent> & eventsUniqueToH,
				const vector<QuaeroEvent> & eventsUniqueToSM,
				const vector<QuaeroEvent> & dataEvents,
				string colliderRun, 
				string experiment, 
				string fs,
				// output
				vector<string> & _distributionNames, 
				vector<double>& weightsCommon,
				vector<double>& weightsUniqueToH,
				vector<double>& weightsUniqueToSM,
				vector<double>& weightsData,
				vector<vector<double> >& variableValuesCommon,
				vector<vector<double> >& variableValuesUniqueToH,
				vector<vector<double> >& variableValuesUniqueToSM,
				vector<vector<double> >& variableValuesData,
				// optional input
				int nVarsToUse, 
				bool keepHighlyCorrelatedVariables)
{

  vector<vector<double> > allVariableValuesCommon, allVariableValuesUniqueToH, allVariableValuesUniqueToSM, allVariableValuesData;
  bool fullVariableList = keepHighlyCorrelatedVariables;
  getAllVariables(fs, commonEvents, allVariableValuesCommon, weightsCommon, fullVariableList);
  getAllVariables(fs, eventsUniqueToH, allVariableValuesUniqueToH, weightsUniqueToH, fullVariableList);
  getAllVariables(fs, eventsUniqueToSM, allVariableValuesUniqueToSM, weightsUniqueToSM, fullVariableList);
  getAllVariables(fs, dataEvents, allVariableValuesData, weightsData, fullVariableList);

  if(locked=="")
    identify(allVariableValuesCommon, weightsCommon, allVariableValuesUniqueToH, weightsUniqueToH, allVariableValuesUniqueToSM, weightsUniqueToSM, colliderRun, experiment, fs, nVarsToUse, keepHighlyCorrelatedVariables);
  else
    assert(locked==colliderRun + " " + experiment + " " + fs);

  size_t nvars = distributions.size();
  variableValuesCommon = vector<vector<double> >(weightsCommon.size(),vector<double>(nvars));
  variableValuesUniqueToH = vector<vector<double> >(weightsUniqueToH.size(),vector<double>(nvars));
  variableValuesUniqueToSM = vector<vector<double> >(weightsUniqueToSM.size(),vector<double>(nvars));
  variableValuesData = vector<vector<double> >(weightsData.size(),vector<double>(nvars));

  for(size_t i=0; i<variableValuesCommon.size(); i++)
    for(size_t j=0; j<nvars; j++)
      variableValuesCommon[i][j] = allVariableValuesCommon[i][distributions[j]];
  for(size_t i=0; i<variableValuesUniqueToH.size(); i++)
    for(size_t j=0; j<nvars; j++)
      variableValuesUniqueToH[i][j] = allVariableValuesUniqueToH[i][distributions[j]];
  for(size_t i=0; i<variableValuesUniqueToSM.size(); i++)
    for(size_t j=0; j<nvars; j++)
      variableValuesUniqueToSM[i][j] = allVariableValuesUniqueToSM[i][distributions[j]];
  for(size_t i=0; i<variableValuesData.size(); i++)
    for(size_t j=0; j<nvars; j++)
      variableValuesData[i][j] = allVariableValuesData[i][distributions[j]];

  _distributionNames = distributionNames;
    
  return(sufficientlyDifferent);
}


void RelevantVariables::getAllVariables(string fs, const vector<QuaeroEvent> & events, 
					vector< vector<double> > & variableValues, vector<double>& weights, bool fullVariableList)
{
  variableValues = vector< vector<double> >(0);
  weights = vector<double>(0);

  for(size_t i=0; i<events.size(); i++)
    {
      vector<double> v = getAllVariableValuesForThisEvent(fs,events[i],fullVariableList);
      if(!v.empty()) // If the event really is in the final state fs (This can fail if a systematic shift affects a particle id)
	{
	  variableValues.push_back(v);
	  weights.push_back(events[i].getWeight());
	}
    }
  return;
}

void RelevantVariables::computeIntegratedLumiMap()
{
  if(getenv("RELEASE_DIR")!=NULL)
    {
      string filename = getenv("RELEASE_DIR");
      string experiment = "cdf";
      if(getenv("experiment")!=NULL)
	{
	  experiment = getenv("experiment");
	  assert((experiment=="cdf")||
		 (experiment=="d0"));
	}
      filename += "/Vista_"+experiment+"/ntuple2txt/goodRuns_commonSubset.txt" ;
      ifstream fin (filename.c_str());
      if(fin) 
	{
	  double intLum = 0.0, lum;
	  string runN;
	  while(fin >> runN) 
	    {
	      fin >> lum;
	      intLum += lum;
	      integratedLumiMap[runN] = intLum;
	    }
	}
    }
  return;
}


vector<double> RelevantVariables::getAllVariableValuesForThisEvent(string finalstate, const QuaeroEvent& event, bool useOnlyMostBasicVariables)
{

  bool useMoreSophisticatedVariables = (!useOnlyMostBasicVariables);

  // Fill o with all important objects in this event
  vector<CLHEP::HepLorentzVector> o = vector<CLHEP::HepLorentzVector>(0);
  vector<double> nullVector = vector<double>(0);
  vector<string> oType = vector<string>(0);
  bool noPmissObject = true;
  size_t pmissObject = 0;

  for(size_t i=0; i<objectList.size(); i++)
    for(int j=0; j<FinalState(finalstate).getNumberOfThisObject(objectList[i]); j++)
      if((objectList[i]=="bb")||(objectList[i]=="jj"))
	{
	  if((event.getThisComponent(objectList[i].substr(0,1),2*j+1)==0)||
	     (event.getThisComponent(objectList[i].substr(0,1),2*j+1+1)==0))
	    return(nullVector);
	  o.push_back(event.getThisComponent(objectList[i].substr(0,1),2*j+1)->getFourVector());
	  o.push_back(event.getThisComponent(objectList[i].substr(0,1),2*j+1+1)->getFourVector());
	  oType.push_back(event.getThisComponent(objectList[i].substr(0,1),2*j+1)->getObjectType());
	  oType.push_back(event.getThisComponent(objectList[i].substr(0,1),2*j+1+1)->getObjectType());
	}
      else 
	if(objectList[i]=="pmiss")
	  {
	    pmissObject = o.size();
	    o.push_back(event.getPmiss());
	    oType.push_back("pmiss");
	    noPmissObject = false;
	  }
	else
	  {
	    if(event.getThisComponent(objectList[i],j+1)==0)
	      return(nullVector);
	    o.push_back(event.getThisComponent(objectList[i],j+1)->getFourVector());
	    oType.push_back(event.getThisComponent(objectList[i],j+1)->getObjectType());
	  }

  // Fill ans with appropriate variables
  vector<double> ans;

  if(event.collider()=="hera") // H1
    {
      // summed object pt
      if(event.getEventType()=="sig")
	{
	  assert(useMoreSophisticatedVariables);
	  /* We have buried both sumPt and m_all into event.getGeneratedSumPt()
	     for H1 signal events produced by TurboSim@H1.
	     Very ugly, but it works.  */ 
	  double sumPt = ((int)(event.getGeneratedSumPt()));
	  double m_all = (event.getGeneratedSumPt()-sumPt)*1000;
	  assert(!Math::isNaNQ(sumPt));
	  assert(!Math::isNaNQ(m_all));
	  ans.push_back(sumPt);
	  ans.push_back(m_all);
	  return(ans);
	}

      double sumPt = 0;
      for(size_t i=0; i<o.size(); i++)
	sumPt += o[i].perp();
      if(event.getThisObject("uncl")!=NULL)
	sumPt += event.getPmiss().perp();
      assert(!Math::isNaNQ(sumPt));
      ans.push_back(sumPt);

      // calculate neutrino 4 vector
      // fUncl is the 4 vector of the unclustered energy!
      // fHighPt is the 4 vector of the sum of high pt particles

      // derive neutrino 4 vector from unclustered energy
      CLHEP::HepLorentzVector pAllVisible;
      for(size_t i=0; i<o.size(); i++)
	if( noPmissObject || i!=pmissObject)
	  pAllVisible = pAllVisible+o[i];
      double NEUPX=0, NEUPY=0, helpPZ=0, helpE=0;
      for(size_t i=0; i<o.size(); i++)
	if( noPmissObject || i!=pmissObject)
	  {
	    NEUPX -= o[i].px();
	    NEUPY -= o[i].py();
	    helpPZ += o[i].pz();
	    helpE += o[i].e();
	  }
      if(event.getThisObject("uncl")!=NULL)
	{
	  NEUPX -= event.getThisObject("uncl")->getFourVector().px();
	  NEUPY -= event.getThisObject("uncl")->getFourVector().py();
	  helpPZ += event.getThisObject("uncl")->getFourVector().pz();
	  // cout << "uncl_pz = " << event.getThisObject("uncl")->getFourVector().pz() << endl;
	  helpE += event.getThisObject("uncl")->getFourVector().e();
	}
      // cout << NEUPX << ", " << NEUPY << ", " << helpPZ << ", " << helpE << endl;
      double NEUEmPz = helpE-helpPZ;
      double electronEnergy = 27.5; // units are GeV
      double empzbal=2.*electronEnergy-NEUEmPz;
      // cout << NEUEmPz << " , " << empzbal << endl;
      double NuPt2 = pow(NEUPX,2)+pow(NEUPY,2);
      double NEUE  =   (empzbal/2.) + (NuPt2/(2.*empzbal));
      double NEUPZ =  -(empzbal/2.) + (NuPt2/(2.*empzbal));
      //      double NEUPT = sqrt(pow(NEUPX,2)+pow(NEUPY,2));
      // cout << NuPt2 << ", " << NEUE << ", " << NEUPZ << ", " << NEUPT << endl;
      // derive invariant mass and sum pt
      double ALL_PX=NEUPX;
      double ALL_PY=NEUPY;
      double ALL_PZ=NEUPZ;
      double ALL_E=NEUE;
      //   double fNEUPTSUM= NEUPT;
      for(size_t i=0; i<o.size(); i++)
	if(i!=pmissObject)
	  {
	    ALL_PX   +=o[i].px();
	    ALL_PY   +=o[i].py();
	    ALL_PZ   +=o[i].pz();
	    ALL_E    +=o[i].e();
	  }
      // cout << ALL_PX << ", " <<ALL_PY  << ", " << ALL_PZ    << ", " << ALL_E << endl;
      double m_all_sq = pow(ALL_E,2)-pow(ALL_PX,2)-pow(ALL_PY,2)-pow(ALL_PZ,2);
      double m_all = sqrt(fabs(m_all_sq));
      // cout << m_all << endl;

      if(event.getThisObject("uncl")==NULL)
	m_all = pAllVisible.m();

      if(Math::isNaNQ(m_all))
	{
	  cerr << "Event: " << event << " has m_all_sq = " << m_all_sq << ", and other variables equal to " << flush;
	  cerr << empzbal << " , " << NEUEmPz << " , " << helpE << " , " << helpPZ << " , " 
	       << event.getThisObject("uncl")->getFourVector().pz() << " , " 
	       << event.getThisObject("uncl")->getFourVector().e() << endl;
	}

      assert(!Math::isNaNQ(m_all));
      ans.push_back(m_all);
      
      if((useMoreSophisticatedVariables)&&
	 (privilegeLevel<1))
	return(ans);

      // E-pz
      ans.push_back(NEUEmPz);
    }

  // summed object pt
  if((event.hadronMachine())&&
     (event.collider()!="hera"))
    ans.push_back(event.sumPt());

  // object e or pt
  for(size_t i=0; i<o.size(); i++)
    if(event.leptonMachine())
      ans.push_back(o[i].e());
    else
      ans.push_back(o[i].perp());

  // missing transverse energy in events without 1pmiss
  if( noPmissObject &&
     event.hadronMachine())
    {
      CLHEP::HepLorentzVector p;
      for(size_t i=0; i<o.size(); i++)
	p = p + o[i];
      // ans.push_back(p.perp());
      ans.push_back(event.getPmiss().perp());
    }

  if(useOnlyMostBasicVariables&&
     event.hadronMachine())
    {
      // unclustered pt
      if(event.getThisObject("uncl")!=NULL)
	ans.push_back(event.getThisObject("uncl")->getFourVector().perp());
      else
	ans.push_back(0);
      
      // clusteredObjectsRecoil_pt
      CLHEP::HepLorentzVector clusteredObjectsRecoil = event.getPmiss();
      if(event.getThisObject("uncl")!=NULL)
	clusteredObjectsRecoil = clusteredObjectsRecoil + event.getThisObject("uncl")->getFourVector();
      ans.push_back(clusteredObjectsRecoil.perp());
      
      // perpendicular and longitudinal components
      if((event.collider()=="tev2")&&
	 (o.size()==2)&&
	 (oType.size()==2)&&
	 (oType[0]!="pmiss")&&
	 (oType[1]!="pmiss"))
	{	  
	  CLHEP::Hep3Vector xyThrustAxis = (o[0]-o[1]).vect();
	  xyThrustAxis.setZ(0);
	  xyThrustAxis = xyThrustAxis.unit();
	  CLHEP::Hep3Vector xyClusteredObjectsRecoil = clusteredObjectsRecoil.vect();
	  xyClusteredObjectsRecoil.setZ(0);
	  double clusteredObjectsRecoil_pt_T = 0;
	  if(fabs(xyThrustAxis.x()*xyClusteredObjectsRecoil.y()-xyThrustAxis.y()*xyClusteredObjectsRecoil.x())>1e-7) // avoid a bug in this version of CLHEP 
	    clusteredObjectsRecoil_pt_T = xyClusteredObjectsRecoil.perp(xyThrustAxis);
	  if(Math::isNaNQ(clusteredObjectsRecoil_pt_T))
	    clusteredObjectsRecoil_pt_T = 0;              // xyClusteredObjectsRecoil colinear with xyThrustAxis
	  assert(!Math::isNaNQ(clusteredObjectsRecoil_pt_T));
	  double clusteredObjectsRecoil_pt_L = fabs(xyClusteredObjectsRecoil.dot(xyThrustAxis));
	  ans.push_back(clusteredObjectsRecoil_pt_L);
	  ans.push_back(clusteredObjectsRecoil_pt_T);
	}

      // deltaphi(tau,uncl) and dotProduct(tau,uncl)
      for(size_t i=0; i<o.size(); i++)
	if((oType[i]=="tau+")||(oType[i]=="tau-"))
	  {
	    if((event.getThisObject("uncl")!=NULL))
	      {
		ans.push_back(Math::deltaphi(o[i].phi(),event.getThisObject("uncl")->getFourVector().phi()));
		ans.push_back(event.getThisObject("uncl")->getFourVector().vect().dot(o[i].vect().unit()));
	      }
	    else
	      {
		ans.push_back(0);
		ans.push_back(0);
	      }
	  }

    }

  /* bktemp
  // event Q^2
  if(event.collider()=="hera")
    {
      const QuaeroRecoObject* o1 = event.getThisObject("e+",1);
      const QuaeroRecoObject* o2 = event.getThisObject("e-",1);
      if((o1!=0)||(o2!=0))
	{
	  const QuaeroRecoObject* o=0;
	  if((o1!=0)&&(o2==0))
	    o = o1;
	  else if((o1==0)&&(o2!=0))
	    o = o2;
	  else if((o1!=0)&&(o2!=0))
	    if(o1->getFourVector().perp()>o2->getFourVector().perp())
	      o = o1;
	    else
	      o = o2;
	  assert(o!=0);
	  
	  CLHEP::HepLorentzVector k(27.5,CLHEP::Hep3Vector(0,0,-27.5));
	  CLHEP::HepLorentzVector k1 = o->getFourVector();
	  double Qsqd = -(k-k1).mag2();
	  ans.push_back(Qsqd);
	}
    }
  */

  // object eta's or cos(theta)'s
  for(size_t i=0; i<o.size(); i++)
    if(event.leptonMachine())
      ans.push_back(o[i].cosTheta());
    else
      if(oType[i]!="pmiss")
	ans.push_back(Math::theta2eta(o[i].theta()));
  
  // invariant mass of W with each other object
  // and W kinematics
  if((count(oType.begin(),oType.end(),"pmiss")==1)&&
     (count(oType.begin(),oType.end(),"e+")+
      count(oType.begin(),oType.end(),"mu+")+
      count(oType.begin(),oType.end(),"e-")+
      count(oType.begin(),oType.end(),"mu-")==1))
    {
      CLHEP::HepLorentzVector l, v;
      for(size_t i=0; i<o.size(); i++)
	if(oType[i]=="pmiss")
	  v = o[i];
	else if((oType[i]=="e+")||
		(oType[i]=="e-")||
		(oType[i]=="mu+")||
		(oType[i]=="mu-"))
	  l = o[i];
      QuaeroRecoObject::ChiSqdConstrainNeutrino(l,v);
      CLHEP::HepLorentzVector w = l+v;
      CLHEP::HepLorentzVector totalSystem=w;
      for(size_t i=0; i<o.size(); i++)
	if((oType[i]!="pmiss")&&
	   (oType[i]!="e+")&&
	   (oType[i]!="e-")&&
	   (oType[i]!="mu+")&&
	   (oType[i]!="mu-"))
	  {
	    totalSystem = totalSystem+o[i];
	    ans.push_back((w+o[i]).m());
	    for(size_t j=i+1; j<o.size(); j++)
	      if((oType[j]!="pmiss")&&
		 (oType[j]!="e+")&&
		 (oType[j]!="e-")&&
		 (oType[j]!="mu+")&&
		 (oType[j]!="mu-"))
		ans.push_back((w+o[i]+o[j]).m());
	  }
      ans.push_back(totalSystem.m());
      ans.push_back(w.perp());
      ans.push_back(w.pz());
    }

  vector<CLHEP::HepLorentzVector> jets;
  for(size_t i=0; i<o.size(); i++)
    if((oType[i]=="j")||
       (oType[i]=="b"))
      jets.push_back(o[i]);
  for(size_t i=0; i<jets.size(); i++)
    for(size_t j=i+1; j<jets.size(); j++)
      if(jets[i].perp()<jets[j].perp())
	swap(jets[i],jets[j]);     // put jets into pT order
  vector<CLHEP::HepLorentzVector> leptons;
  vector<CLHEP::HepLorentzVector> leptons_positivelyCharged;
  for(size_t i=0; i<o.size(); i++)
    {
      if((oType[i]=="e+")||
	 (oType[i]=="e-")||
	 (oType[i]=="mu+")||
	 (oType[i]=="mu-")||
	 (oType[i]=="tau+")||
	 (oType[i]=="tau-"))
	leptons.push_back(o[i]);
      if((oType[i]=="e+")||
	 (oType[i]=="mu+"))
	leptons_positivelyCharged.push_back(o[i]);
    }
      

  // Angle between lepton plane and jet plane
  if((leptons.size()==2)&&
     (jets.size()==2))
    {
      CLHEP::Hep3Vector leptonPlaneNormalVector = leptons[0].vect().cross(leptons[1].vect()).unit();
      CLHEP::Hep3Vector jetPlaneNormalVector = jets[0].vect().cross(jets[1].vect()).unit();
      double angleBetweenPlanes = fabs(acos(leptonPlaneNormalVector.dot(jetPlaneNormalVector)));
      ans.push_back(angleBetweenPlanes);
    }
  // Angle between lepton plane and jet-beam plane
  if((leptons.size()==2)&&
     (jets.size()==1))
    {
      CLHEP::Hep3Vector leptonPlaneNormalVector = leptons[0].vect().cross(leptons[1].vect()).unit();
      CLHEP::Hep3Vector jetBeamPlaneNormalVector = jets[0].vect().cross(CLHEP::Hep3Vector(0,0,1)).unit();
      double angleBetweenPlanes = fabs(acos(leptonPlaneNormalVector.dot(jetBeamPlaneNormalVector)));
      ans.push_back(angleBetweenPlanes);
    }
  // cos(theta*) for lepton pairs
  if((leptons.size()==2)&&
     (leptons_positivelyCharged.size()==1))
    {
      CLHEP::HepLorentzVector Z = leptons[0]+leptons[1];
      CLHEP::Hep3Vector ZDirection = Z.vect().unit(); // get 3spatial component.
      double beta = Z.beta();
      CLHEP::HepLorentzVector positivelyChargedLepton = leptons_positivelyCharged[0];
      positivelyChargedLepton.boost(-ZDirection,beta);
      double cosThetaStar = positivelyChargedLepton.vect().unit().dot(ZDirection);
      ans.push_back(cosThetaStar);
    }

  // Montreal Opal Lep 1 4-jet angle, from http://arxiv.org/pdf/hep-ex/0611048
  if((jets.size()==3))
    {
      CLHEP::Hep3Vector jet01PlaneNormalVector = jets[0].vect().cross(CLHEP::Hep3Vector(0,0,1)).unit();
      CLHEP::Hep3Vector jet23PlaneNormalVector = jets[1].vect().cross(jets[2].vect()).unit();
      double angleBetweenPlanes = fabs(acos(jet01PlaneNormalVector.dot(jet23PlaneNormalVector)));
      if(angleBetweenPlanes>M_PI/2)
	angleBetweenPlanes = M_PI-angleBetweenPlanes;
      ans.push_back(angleBetweenPlanes);
    }


  // (XX->jjjj)
  if(jets.size()==4)
    {
      // mass(X)
      double minDeltaMjj = FLT_MAX;
      double mjj = 0;
      int theBestI=0; // it will be the index of the jet that couples best with the 0'th jet. I.e. if theBestI=2 then X_1-->02 and X_2-->13
      CLHEP::HepLorentzVector allJets;
      for(int i=0; i<4; i++)
	allJets += jets[i]; 
      for(int i=1; i<4; i++)
	{
	  CLHEP::HepLorentzVector jj1 = jets[0]+jets[i];
	  CLHEP::HepLorentzVector jj2 = allJets-jj1;
	  double mjj1 = fabs(jj1.m());
	  double mjj2 = fabs(jj2.m());
	  double deltaMjj = fabs(mjj1-mjj2);
	  if(deltaMjj<minDeltaMjj)
	    {
	      minDeltaMjj = deltaMjj;
	      mjj = (mjj1+mjj2)/2;
	      theBestI=i;
	    }
	}
      ans.push_back(minDeltaMjj);
      ans.push_back(mjj);
      ans.push_back(minDeltaMjj/mjj);

      /*
      // mass(X)_20, assuming that X is 20 GeV. This will be 
      // assuming that X is a 20 GeV particle and will find the best
      // combinatorics that are closest to this hypothesis.
      double minDeltaMjj_20 = FLT_MAX;
      double mjj_20 = 0;
      // allJets is already defined above
      for(int i=1; i<4; i++)
	{
	  CLHEP::HepLorentzVector jj1 = jets[0]+jets[i];
	  CLHEP::HepLorentzVector jj2 = allJets-jj1;
	  double mjj1 = fabs(jj1.m());
	  double mjj2 = fabs(jj2.m());
	  double deltaMjj_20 = (fabs(mjj1-20) + fabs(mjj2-20))/2;
	  if(deltaMjj_20<minDeltaMjj_20)
	    {
	      minDeltaMjj_20 = deltaMjj_20;
	      mjj_20 = (mjj1+mjj2)/2;
	    }
	}
      ans.push_back(minDeltaMjj_20);
      ans.push_back(mjj_20);
      ans.push_back(minDeltaMjj_20/mjj_20);
      */
      
      // The angle of the two planes defined by the two pairs of jets that
      // best match the hypothesis of XX.
      // allJets is already defined above
      
      CLHEP::Hep3Vector boostDirection = allJets.vect(); // get 3spatial component.
      double beta = allJets.beta();
      
      CLHEP::HepLorentzVector pJcouple1member1=jets[0];
      CLHEP::HepLorentzVector pJcouple1member2=jets[theBestI];
      CLHEP::HepLorentzVector pJcouple2member1, pJcouple2member2;
      vector<int> thoseNotSelectedAlready;
      for(int i=1; i<4; i++)
	if (i != theBestI) thoseNotSelectedAlready.push_back(i);
      assert(thoseNotSelectedAlready.size()==2);
      pJcouple2member1=jets[thoseNotSelectedAlready[0]];
      pJcouple2member2=jets[thoseNotSelectedAlready[1]];
      
      //boosts
      pJcouple1member1.boost(boostDirection,beta);
      pJcouple1member2.boost(boostDirection,beta);
      pJcouple2member1.boost(boostDirection,beta);
      pJcouple2member2.boost(boostDirection,beta);
      
      vector<double> p3_11;
      p3_11.push_back(pJcouple1member1.px());
      p3_11.push_back(pJcouple1member1.py());
      p3_11.push_back(pJcouple1member1.pz());
      vector<double> p3_12;
      p3_12.push_back(pJcouple1member2.px());
      p3_12.push_back(pJcouple1member2.py());
      p3_12.push_back(pJcouple1member2.pz());
      vector<double> p3_21;
      p3_21.push_back(pJcouple2member1.px());
      p3_21.push_back(pJcouple2member1.py());
      p3_21.push_back(pJcouple2member1.pz());
      vector<double> p3_22;
      p3_22.push_back(pJcouple2member2.px());
      p3_22.push_back(pJcouple2member2.py());
      p3_22.push_back(pJcouple2member2.pz());
      
      double angleBetweenPlanes=Math::planesAngle(p3_11,p3_12,p3_21,p3_22);
      ans.push_back(angleBetweenPlanes);
      
    }
  
  /*
  // pT relationship between lowest pT jets
  if(jets.size()>=2)
    {
      double nextToLowestJetPt = jets[jets.size()-2].perp();
      double lowestJetPt = jets[jets.size()-1].perp();
      ans.push_back(lowestJetPt/nextToLowestJetPt);
      ans.push_back(nextToLowestJetPt-lowestJetPt);
    }
  */


  // Z pt, pz
  if(event.hadronMachine())
    if( ( (count(oType.begin(),oType.end(),"e+")==1)&&
	  (count(oType.begin(),oType.end(),"e-")==1) ) ||
	( (count(oType.begin(),oType.end(),"mu+")==1)&&
	  (count(oType.begin(),oType.end(),"mu-")==1) ) )
      {
	CLHEP::HepLorentzVector z;
	for(size_t i=0; i<o.size(); i++)
	  if((oType[i]=="e+")||
	     (oType[i]=="e-")||
	     (oType[i]=="mu+")||
	     (oType[i]=="mu-"))
	    z += o[i];
	ans.push_back(z.perp());
	ans.push_back(z.pz());
      }

  // invariant masses of all combinations
  map<vector<string>, vector<double> > resonanceMassVariableValues;
  if(o.size()<=6)
  for(int i=0; i<pow((float)2,(float)o.size()); i++)
    {
      int nObs = (int)(Math::computeAverage(Math::getDigits(i,2,o.size()))*o.size()+0.5);
      if(nObs>=2) // don't use the mass of a single final state object -- only use the invariant mass of combinations 
	if( useOnlyMostBasicVariables || (nObs<=3) )  // only use pairwise or triplet masses unless we have the full variable list
	{
	  CLHEP::HepLorentzVector o1(0,CLHEP::Hep3Vector(0,0,0));
	  string massType = "invariant";
	  bool containsPmiss = false;
	  vector<string> oType1;
	  for(size_t j=0; j<o.size(); j++)
	    if(Math::getDigits(i,2,o.size())[j]==1)
	      {
		o1 = o1 + o[j];	
		oType1.push_back(oType[j]);
		if(!noPmissObject && (j==pmissObject))
		  {
		    containsPmiss = true;
		    if(event.hadronMachine())
		      massType= "transverse";
		  }
	      }
	  if( containsPmiss && event.leptonMachine() && (nObs==(int) o.size()) )
	    continue; // don't use the center of mass energy for a lepton machine, since this is a delta function and violates my five lump lemma
	  sort(oType1.begin(),oType1.end());
	  if(resonanceMassVariableValues.find(oType1)==resonanceMassVariableValues.end())
	    resonanceMassVariableValues[oType1] = vector<double>(0);
	  if(massType=="invariant")
	    {
	      ans.push_back(o1.m());	  
	      resonanceMassVariableValues[oType1].push_back(o1.m());
	      if(o1.m()<0)
		; // cout << "negative mass: " << event << " // pmiss= " << event.getPmiss().e() << " " << event.getPmiss().px() << " " << event.getPmiss().py() << " " << event.getPmiss().pz() << endl;
	    }
	  if(massType=="transverse")
	    {
	      if(nObs==2)
		{
		  ans.push_back(o1.mt());
		  resonanceMassVariableValues[oType1].push_back(o1.mt());
		}
	      else
		;
	    }
	}
    }

  // pairwise object deltaphi's
  for(size_t i=0; i<o.size(); i++)
    for(size_t j=i+1; j<o.size(); j++)
      {
	//if(useOnlyMostBasicVariables || (o.size()==2))
	  ans.push_back(Math::deltaphi(o[i].phi(),o[j].phi()));
      }
  // deltaphi of pmiss to closest jet
  for(size_t i=0; i<o.size(); i++)
    if(oType[i]=="pmiss")
      {
	double minDeltaPhi_jpmiss = FLT_MAX;
	double maxDeltaPhi_jpmiss = -FLT_MAX;
	for(size_t j=0; j<o.size(); j++)
	  if(oType[j]=="j")
	    {
	      double x = Math::deltaphi(o[i].phi(),o[j].phi());
	      if(x<minDeltaPhi_jpmiss)
		minDeltaPhi_jpmiss = x;
	      if(x>maxDeltaPhi_jpmiss)
		maxDeltaPhi_jpmiss = x;
	    }
	if(minDeltaPhi_jpmiss<FLT_MAX)
	  ans.push_back(minDeltaPhi_jpmiss);
	if(maxDeltaPhi_jpmiss>-FLT_MAX)
	  ans.push_back(maxDeltaPhi_jpmiss);
      }
  // deltaR of lepton or photon to closest jet
  for(size_t i=0; i<o.size(); i++)
    if((oType[i]=="e+")||(oType[i]=="e-")||
       (oType[i]=="mu+")||(oType[i]=="mu-")||
       (oType[i]=="tau+")||(oType[i]=="tau-")||
       (oType[i]=="ph"))
      {
	double minDeltaR_lj = FLT_MAX;
	for(size_t j=0; j<o.size(); j++)
	  if(oType[j]=="j")
	    {
	      double x = Math::deltaR(o[i].phi(), Math::theta2eta(o[i].theta()), 
				      o[j].phi(), Math::theta2eta(o[j].theta()));
	      if(x<minDeltaR_lj)
		minDeltaR_lj = x;
	    }
	if(minDeltaR_lj<FLT_MAX)
	  ans.push_back(minDeltaR_lj);
      }
  // deltaR of two closest jets
  double minDeltaR_jj = FLT_MAX;
  for(size_t i=0; i<o.size(); i++)
    if(oType[i]=="j")
      for(size_t j=i+1; j<o.size(); j++)
	if(oType[j]=="j")
	  {
	    double x = Math::deltaR(o[i].phi(), Math::theta2eta(o[i].theta()), 
				    o[j].phi(), Math::theta2eta(o[j].theta()));
	    if(x<minDeltaR_jj)
	      minDeltaR_jj = x;
	  }
  if(minDeltaR_jj<FLT_MAX)
    ans.push_back(minDeltaR_jj);

  // deltaEta of j2 and j3, and SjostrandVariable
  if(jets.size()==3)
    {
      double j2_eta = Math::theta2eta(jets[1].theta());
      double j3_eta = Math::theta2eta(jets[2].theta());
      double j2_phi = jets[1].phi();
      double j3_phi = jets[2].phi();
      ans.push_back(fabs(j2_eta-j3_eta));
      // ans.push_back(j2_eta-j3_eta);
      double sjostrandVariable = atan2(fabs(j3_phi-j2_phi),(j3_eta-j2_eta)*(j2_eta>0 ? 1 : -1));
      ans.push_back(sjostrandVariable);
    }

  if(useOnlyMostBasicVariables)
    {
      // generated sumPt
      if(useGeneratedSumPt)
	if(event.collider()=="tev2")
	  if((event.getEventType()=="data")||
	     (event.getEventType()=="jet20"))
	    ans.push_back(FLT_MAX);
      // ans.push_back(event.sumPt());
	  else
	    ans.push_back(event.getGeneratedSumPt());

      // event zvtx
      if(useZvtx)
	if(event.hadronMachine())
	  ans.push_back(event.getZVtx());

      // object phi's
      for(size_t i=0; i<o.size(); i++)
	ans.push_back(o[i].phi());
      
//MRENNA-1
      // object detector eta's
/*      for(size_t i=0; i<o.size(); i++)
	if(oType[i]!="pmiss")
    ans.push_back(QuaeroRecoObject::getDetectorEta("cdf",oType[i],Math::theta2eta(o[i].theta()),event.getZVtx())); */
      
      // sum eta of muon pair
      int nMu = 0;
      double muonSumEta = 0;
      for(size_t i=0; i<o.size(); i++)
	if((oType[i]=="mu+")||(oType[i]=="mu-"))
	  {
	    nMu++;
	    muonSumEta += Math::theta2eta(o[i].theta());
	  }
      if(nMu==2)
	ans.push_back(muonSumEta);

      // pairwise object deltaR's
      for(size_t i=0; i<o.size(); i++)
	if(oType[i]!="pmiss")
	  for(size_t j=i+1; j<o.size(); j++)
	    if(oType[j]!="pmiss")
	      ans.push_back(Math::deltaR(o[i].phi(), Math::theta2eta(o[i].theta()), o[j].phi(), Math::theta2eta(o[j].theta())));
      
	  /*
      // unclustered energy
      const QuaeroFSObject* uncl = event.getThisComponent("uncl");
      if(uncl)
	{
	  double pt = uncl->getFourVector().perp();
	  if(pt<100.)
	    ans.push_back(pt);
	  else
	    ans.push_back(101.); // restrict the range of the plot
	    if(pt>10) // units are GeV
	    ans.push_back(uncl->getFourVector().phi());
	    else
	    ans.push_back(-9);
	}
      else
	ans.push_back(0);
	  */
      
      /*
	// topological variables
	ans.push_back(aplanarity(event));
	ans.push_back(acoplanarity(event));
	ans.push_back(sphericity(event));
	ans.push_back(missingMass(event));
      */
    }

  // rootS
  if(event.leptonMachine())
    {
      int smear = 0;
      string rn = event.getRunNumber();
      for(size_t i=0; i<rn.length(); i++)
	smear += toascii(*(rn.substr(i,1).c_str()));
      if(useMoreSophisticatedVariables)
	ans.push_back(event.getRootS()+(smear%9-4)/10.);
      else
	ans.push_back(event.getRootS());
    }

  if(useMoreSophisticatedVariables)
    {
      // in 4j final state, take the jet pairings giving the closest mass 
      if((oType.size()==4))
	{
	  bool fourJetFinalState = true;
	  for(int i=0; i<4; i++)
	    if(!((oType[i]=="j")||
		 (oType[i]=="b")))
	      fourJetFinalState = false;
	  if(fourJetFinalState)
	    {
	      double minMassDifference = 1000; // units are GeV
	      double m=0;
	      assert(o.size()==4);
	      for(int i=0; i<16; i++)
		{
		  vector<int> counter = Math::getDigits(i,2,4);
		  if(accumulate(counter.begin(),counter.end(),0)!=2)
		    continue;
		  CLHEP::HepLorentzVector p1, p2;
		  for(size_t j=0; j<counter.size(); j++)
		    if(counter[j]==0)
		      p1 = p1 + o[j];
		    else
		      p2 = p2 + o[j];
		  if(fabs(p1.m()-p2.m()) < minMassDifference)
		    {
		      minMassDifference = fabs(p1.m()-p2.m());
		      m = (p1.m()+p2.m())/2;
		    }
		}
	      ans.push_back(m);
	    }
	}


      // mass of all objects other than (jj) pair that is the closest match to W or Z
      if(o.size()>=4)
	{
	  bool foundWindices = false;
	  bool foundZindices = false;
	  size_t w_ki, w_kj; 
	  size_t z_ki, z_kj;
	  w_ki = w_kj = z_ki = z_kj = 0;
	  double min_dmZ = FLT_MAX, min_dmW = FLT_MAX;
	  for(size_t i=0; i<o.size(); i++)
	    if(oType[i]=="j")
	      for(size_t j=i+1; j<o.size(); j++)
		if(oType[j]=="j")	  
		  {
		    double m = (o[i]+o[j]).m();
		    double dmZ = fabs(m-91.2);
		    double dmW = fabs(m-80.4);
		    if(dmZ < min_dmZ)
		      {
			min_dmZ = dmZ;
			z_ki = i;
			z_kj = j;
			foundZindices=true;
		      }
		    if(dmW < min_dmW)
		      {
			min_dmW = dmW;
			w_ki = i;
			w_kj = j;
			foundWindices=true;
		      }
		  }
	  if(foundWindices)
	    {
	      CLHEP::HepLorentzVector objectsOtherThanW;
	      for(size_t i=0; i<o.size(); i++)
		if((i!=w_ki)&&(i!=w_kj))
		  objectsOtherThanW = objectsOtherThanW + o[i];
	      ans.push_back(objectsOtherThanW.m());
	    }
	  if(foundZindices)
	    {
	      CLHEP::HepLorentzVector objectsOtherThanZ;
	      for(size_t i=0; i<o.size(); i++)
		if((i!=z_ki)&&(i!=z_kj))
		  objectsOtherThanZ = objectsOtherThanZ + o[i];
	      ans.push_back(objectsOtherThanZ.m());
	    }
	}

      // Object pairings closest to resonance
      for(map<vector<string>,vector<double> >::iterator i=resonanceMassVariableValues.begin(); i!=resonanceMassVariableValues.end(); i++)
	{
	  if(i->second.size()>=2)
	    {
	      for(size_t j=0; j<resonanceMasses.size(); j++)
		{
		  double closest = 1e6;
		  for(size_t k=0; k<i->second.size(); k++)
		    {
		      if(fabs(i->second[k]-resonanceMasses[j])<fabs(closest-resonanceMasses[j]))
			closest = i->second[k];		  
		    }
		  ans.push_back(closest);
		}
	    }
	}

      // y_min
      double y_min = FLT_MAX;
      for(size_t i=0; i<o.size(); i++)
	if((oType[i]=="j")||(oType[i]=="b"))
	  for(size_t j=i+1; j<o.size(); j++)
	    if((oType[j]=="j")||(oType[j]=="b"))
	      {
		double y = pow((o[i]+o[j]).m()/event.getRootS(),2.);
		if(y<y_min)
		  y_min = y;
	      }
      if(y_min<FLT_MAX)
	ans.push_back(y_min);
      
      // vector summed jet pt
      int nj = count(oType.begin(),oType.end(),"j");
      int nb = count(oType.begin(),oType.end(),"b");
      if(nj+nb>=2)
	{
	  CLHEP::HepLorentzVector p;
	  for(size_t i=0; i<o.size(); i++)
	    if((oType[i]=="j")||(oType[i]=="b"))
	      p = p + o[i];
	  ans.push_back(p.perp());
	}
      
      /*
      // di-tau mass
      int ntauPlus = count(oType.begin(),oType.end(),"tau+")+
	count(oType.begin(),oType.end(),"tau-");
      int pmiss = count(oType.begin(),oType.end(),"pmiss");
      if((ntau==2)&&(pmiss==1))
	{
	  bool diTauMass = 
	}
      */

    }

  // Georgios : Distribution of run where each event was produced.
  //            Integral from run 0 to run RunOfThisEvent L dt / Integral from 0 to LastRun L dt
  if((event.collider()=="tev2")&&
     (useRunHistory))
    {
      // If the map is empty (like for the first event) then fill it.

    if(integratedLumiMap.empty())
      computeIntegratedLumiMap();

    // If the file was there and the map filling was successful, then continue calculating
    // the runHistory and pushing it back to the ans vector. Otherwise don't do that.
    if(! integratedLumiMap.empty() ) 
      {
	double runHistory=0;
	if( (event.getEventType()=="data") || (event.getEventType()=="jet20") 
	    || ( (event.getRunNumber().substr(0,6) != "151435") &&
		 (useMonteCarloRunNumbers) )
	    ) {
	  string identifier = event.getRunNumber();  // like 123456.2341432
	  string::size_type pos = identifier.find (".",0);   // find the dot
	  if(pos!=string::npos)
	    {
	      string runNumberString = identifier.substr(0,pos); // keep only the run number string.
	      if(integratedLumiMap.find(runNumberString)==integratedLumiMap.end())
		{
		  runHistory = 0;
		  // cout << "Cannot find run number " << runNumberString << " in Vista_cdf/ntuple2txt/goodRuns_commonSubset.txt" << endl; 
		}
	      else
		runHistory = integratedLumiMap[runNumberString];
	    }
	}
	else  // For bkg events:
	  {
	    double integratedLumi = (*(--integratedLumiMap.end())).second;  // Read the integrated luminosity from the map.	    
	    runHistory = drand48()*integratedLumi; // Random number [0,integratedLumi] uniformly distributed
	    if((event.numberOfObjects("e",20,2.5)+
		event.numberOfObjects("mu",20,2.5)+
		event.numberOfObjects("ph",20,2.5)+
		event.numberOfObjects("j",200,1.0)+
		event.numberOfObjects("b",200,1.0)==0) &&
	       (!(event.numberOfObjects("b",90,1.0)>=2)) &&
	       (!((event.numberOfObjects("b",60,1.0)>=3)&&
		  (event.numberOfObjects("b",90,1.0)>=1))) )    // low-pT final state
	      {
		if(event.numberOfObjects("tau",17,1)>=2)
		  runHistory = 350+drand48()*(integratedLumi-350);
		else
		  while(((runHistory>integratedLumiMap.lower_bound("153065")->second)&&
			 (runHistory<integratedLumiMap.lower_bound("184444")->second)&&
			 (drand48()>(20.*12)/(20*25)))||
			((runHistory>integratedLumiMap.lower_bound("184444")->second)&&
			 (drand48()>(20.*12)/(50*25))))
		    runHistory = drand48()*integratedLumi;
	      }
	    else if(event.numberOfObjects("mu",20,1.0)>=1)
	      {
		bool muonInCmx=false, muonInMiniskirtOrKeystone=false, muonInBluebeam=false;
		for(size_t i=0; i<o.size(); i++)
		  if((oType[i]=="mu+")||
		     (oType[i]=="mu-"))
		    {
		      muonInCmx = (muonInCmx||(fabs(Math::theta2eta(o[i].theta()))>0.6));
		      muonInMiniskirtOrKeystone = (muonInMiniskirtOrKeystone||
						   ((fabs(Math::theta2eta(o[i].theta()))>0.6)&&
						    ((Math::deltaphi(o[i].phi(),M_PI/2)<M_PI/12)||
						     (Math::deltaphi(o[i].phi(),3*M_PI/2)<3*M_PI/12))));
		      muonInBluebeam = (muonInBluebeam||(Math::deltaphi(o[i].phi(),3*M_PI/4)<M_PI/4));
		    }
		bool cmxIsOff, miniskirtAndKeyStoneAreOff, bluebeamIsOff;
		do {
		  runHistory = drand48()*integratedLumi;
		  cmxIsOff = (runHistory<integratedLumiMap.lower_bound("150144")->second);
		  miniskirtAndKeyStoneAreOff = (runHistory<integratedLumiMap.lower_bound("186598")->second);
		  bluebeamIsOff = (runHistory<integratedLumiMap.lower_bound("154449")->second);
		} while ((muonInCmx&&cmxIsOff)||(muonInMiniskirtOrKeystone&&miniskirtAndKeyStoneAreOff)||(muonInBluebeam&&bluebeamIsOff));
	      }
	  }
	ans.push_back(runHistory);
      }
  } // End of finding runHistory

  // single top variables in lepton+pmiss+b+j final states at the Fermilab Tevatron
  if(event.collider()=="tev2")
    if((oType.size()==4)&&
       (count(oType.begin(),oType.end(),"b")==1)&&
       (count(oType.begin(),oType.end(),"j")==1)&&
       (count(oType.begin(),oType.end(),"pmiss")==1)&&
       (count(oType.begin(),oType.end(),"e+")+
	count(oType.begin(),oType.end(),"e-")+
	count(oType.begin(),oType.end(),"mu+")+
	count(oType.begin(),oType.end(),"mu-")==1))
      {
	CLHEP::HepLorentzVector b, l, v, j;
	for(size_t i=0; i<oType.size(); i++)
	  if(oType[i]=="b")
	    b = o[i];
	  else if(oType[i]=="pmiss")
	    v = o[i];
	  else if(oType[i]=="j")
	    j = o[i];
	  else // lepton
	    l = o[i];
	QuaeroRecoObject::ChiSqdConstrainNeutrino(l,v);
	//double mtop = 175., mw = 80.4;  // units are GeV
	CLHEP::HepLorentzVector w = l+v;
	CLHEP::HepLorentzVector h = j+b;
	ans.push_back((w+h).m()-w.m()-h.m());
      }

  // 2e+1e-X massClosestToZ
  if(event.collider()=="tev2")
    if((oType.size()>=3)&&
       (count(oType.begin(),oType.end(),"e+")==2)&&
       (count(oType.begin(),oType.end(),"e-")==1))
      {
	vector<CLHEP::HepLorentzVector> ePlus;
	CLHEP::HepLorentzVector eMinus;
	for(size_t i=0; i<oType.size(); i++)
	  if(oType[i]=="e+")
	    ePlus.push_back(o[i]);
	  else if(oType[i]=="e-")
	    eMinus = o[i];
	double massClosestToZ = FLT_MAX;
	for(size_t i=0; i<ePlus.size(); i++)
	  {
	    double m = (ePlus[i]+eMinus).m();
	    if(fabs(m-91.12)<fabs(massClosestToZ-91.12))
	      massClosestToZ=m;
	  }
	ans.push_back(massClosestToZ);
      }

  // ttbar variables in lepton+pmiss+b+3j final states at the Fermilab Tevatron
  if(event.collider()=="tev2")
    if((oType.size()==6)&&
       (count(oType.begin(),oType.end(),"b")==1)&&
       (count(oType.begin(),oType.end(),"j")==3)&&
       (count(oType.begin(),oType.end(),"pmiss")==1)&&
       (count(oType.begin(),oType.end(),"e+")+
	count(oType.begin(),oType.end(),"e-")+
	count(oType.begin(),oType.end(),"mu+")+
	count(oType.begin(),oType.end(),"mu-")==1))
      {
	CLHEP::HepLorentzVector b, l, v;  vector<CLHEP::HepLorentzVector> j;
	CLHEP::HepLorentzVector top_lvb, top_jjb;
	pair<vector<CLHEP::HepLorentzVector>,vector<CLHEP::HepLorentzVector> > decayProducts;
	for(size_t i=0; i<oType.size(); i++)
	  if(oType[i]=="b")
	    b = o[i];
	  else if(oType[i]=="pmiss")
	    v = o[i];
	  else if(oType[i]=="j")
	    j.push_back(o[i]);
	  else // lepton
	    l = o[i];
	assert(j.size()==3);
	QuaeroRecoObject::ChiSqdConstrainNeutrino(l,v);
	double mtop = 175., mw = 80.4;  // units are GeV
	decayProducts.first = decayProducts.second = vector<CLHEP::HepLorentzVector>(3);
	decayProducts.first[0]=l;
	decayProducts.first[1]=v;
	decayProducts.first[2]=b;
	double chiSqd = FLT_MAX;
	CLHEP::HepLorentzVector j123 = j[0]+j[1]+j[2];
	double chiSqd_lvb = pow((l+v+b).m()-mtop,2) + pow(j123.m()-mtop,2);
	for(int i=0; i<3; i++)
	  {
	    CLHEP::HepLorentzVector j23 = j123-j[i];
	    double x = pow(j23.m()-mw,2.);
	    if(chiSqd_lvb+x<chiSqd)
	      {
		chiSqd = chiSqd_lvb+x;
		decayProducts.second[0]=(i==0 ? j[1] : j[0]);
		decayProducts.second[1]=(i==2 ? j[1] : j[2]);
		decayProducts.second[2]=j[i];
		top_lvb = l+v+b;
		top_jjb = j123;
	      }
	  }
	for(int i=0; i<3; i++)
	  {
	    CLHEP::HepLorentzVector j1 = j[i], j23 = j123-j[i];	    
	    double x = pow((l+v+j1).m()-mtop,2) + pow((b+j23).m()-mtop,2) + pow(j23.m()-mw,2);
	    if(x<chiSqd)
	      {
		chiSqd=x;
		decayProducts.first[2]=j1;
		decayProducts.second[0]=(i==0 ? j[1] : j[0]);
		decayProducts.second[1]=(i==2 ? j[1] : j[2]);
		decayProducts.second[2]=b;		
		top_lvb = l+v+j1;
		top_jjb = j23+b;
	      }
	  }
	CLHEP::HepLorentzVector ttbar = top_lvb+top_jjb;
	CLHEP::HepLorentzVector t, tbar, bbar;
	if(count(oType.begin(),oType.end(),"e+")+
	   count(oType.begin(),oType.end(),"mu+")==1)
	  {
	    t = top_lvb;
	    tbar = top_jjb;
	    b = decayProducts.first[2];
	    bbar = decayProducts.second[2];
	  }
	if(count(oType.begin(),oType.end(),"e-")+
	   count(oType.begin(),oType.end(),"mu-")==1)
	  {
	    tbar = top_lvb;
	    t = top_jjb;
	    b = decayProducts.second[2];
	    bbar = decayProducts.first[2];
	  }
	CLHEP::HepLorentzVector w_lv = l+v;
	CLHEP::HepLorentzVector w_jj = decayProducts.second[0]+decayProducts.second[1];

	ans.push_back(chiSqd);
	ans.push_back(v.pz());
	// Average quantities of top and anti-top
	ans.push_back((t.m()+tbar.m())/2);
	ans.push_back((t.perp()+tbar.perp())/2);
	// Properties of the ttbar system
	ans.push_back(ttbar.m());
	ans.push_back(ttbar.pz());
	ans.push_back(ttbar.perp());
	// Properties of the individual top and anti-top
	ans.push_back(Math::theta2eta(t.theta()));
	ans.push_back(Math::theta2eta(tbar.theta()));
	//mrennaCompile ans.push_back(t.boost(ttbar.vect()/ttbar.e()).theta());
	//mrennaCompile ans.push_back(cos(t.boost(ttbar.vect()/ttbar.e()).theta()));
	// Properties of the b quarks
	ans.push_back(Math::theta2eta(b.theta()));
	ans.push_back(b.perp());
	ans.push_back(b.pz());
	ans.push_back(Math::theta2eta(bbar.theta()));
	ans.push_back(bbar.perp());
	ans.push_back(b.pz());
	// Properties of the W bosons
	ans.push_back(w_lv.m());
	ans.push_back(w_lv.perp());
	ans.push_back(w_lv.pz());
	ans.push_back(w_jj.m());
	ans.push_back(w_jj.perp());
	ans.push_back(w_jj.pz());
      }

  
  // deltaEta_jj
  if((event.collider()=="tev2" || event.collider()=="lhc")&&
     (count(oType.begin(),oType.end(),"j")==2))
      {
	vector<CLHEP::HepLorentzVector> j;
	for(size_t i=0; i<oType.size(); i++)
	  if(oType[i]=="j")
	    j.push_back(o[i]);
	assert(j.size()==2);
	if(j[0].perp()<j[1].perp())
	  swap(j[0],j[1]);
	double deltaEta_jj = Math::theta2eta(j[0].theta())-Math::theta2eta(j[1].theta());
	ans.push_back(fabs(deltaEta_jj));
      }

  // pT_jj
  if((event.collider()=="tev2" || event.collider()=="lhc")&&
     (count(oType.begin(),oType.end(),"j")>=2))
  {
     CLHEP::HepLorentzVector vpair = o[0]+o[1];
     ans.push_back( vpair.perp() );
  }
  
  // mass(j), mass(b)
  // mass(j)/j_pt
  // minMass(j)

//MRENNA-2
/*
  if(useOnlyMostBasicVariables &&
     ( (event.collider()=="tev2") ||
       (event.collider()=="lhc") ))
    {
      double minMass = FLT_MAX;
      int nJets = 0;
      for(size_t i=0; i<oType.size(); i++)
	if((oType[i]=="j")||
	   (oType[i]=="b"))
	  {
	    ans.push_back(o[i].m());      
	    nJets++;
	    // ans.push_back((o[i].perp()<=0 ? 0 : o[i].m()/o[i].perp()));
	    if(o[i].m()<minMass)
	      minMass=o[i].m();
	  }
      if((minMass<FLT_MAX)&&(nJets>1))
	ans.push_back(minMass);
    }
*/

  // extremum kinematics for dileptons
  double maxEta=0, minPt=FLT_MAX;
  double numberOfLeptons=0;
  if((event.collider()=="tev2"))
    for(size_t i=0; i<oType.size(); i++)
      if((oType[i]=="e+")||
	 (oType[i]=="e-")||
	 (oType[i]=="mu+")||
	 (oType[i]=="mu-")||
	 (oType[i]=="tau+")||
	 (oType[i]=="tau-"))
	{
	  numberOfLeptons++;
	  if(o[i].perp()<minPt)
	    minPt=o[i].perp();
	  if(fabs(Math::theta2eta(o[i].theta()))>maxEta)
	    maxEta = fabs(Math::theta2eta(o[i].theta()));
	}
  if(numberOfLeptons>=2)
    {
      ans.push_back(minPt);
      ans.push_back(maxEta);
    }

  return ans;
}

vector<string> RelevantVariables::getAllVariableNamesForThisEvent(string finalstate, bool pawFormatted, bool useOnlyMostBasicVariables, string colliderRun)
{

  string colliderType = "";
  if((colliderRun=="hera")||
     (colliderRun=="tev1")||
     (colliderRun=="tev2")||
     (colliderRun=="lhc")||
     (colliderRun=="hadron"))
    colliderType = "hadron";
  if((colliderRun=="lep2")||
     (colliderRun=="lepton"))
    colliderType = "lepton";
  assert(colliderType!="");

  bool useMoreSophisticatedVariables = (!useOnlyMostBasicVariables);

  vector<string> objectList = vector<string>(0);
  objectList.push_back("e+");
  objectList.push_back("e-");
  objectList.push_back("mu+");
  objectList.push_back("mu-");
  objectList.push_back("tau+");
  objectList.push_back("tau-");
  objectList.push_back("ph");
  objectList.push_back("j");
  objectList.push_back("b");
  objectList.push_back("pmiss");
  objectList.push_back("w");
  objectList.push_back("z");

  vector<string> pawObjectList = vector<string>(0);
  pawObjectList.push_back("e^+!");
  pawObjectList.push_back("e^-!");
  pawObjectList.push_back("[m]^+!");
  pawObjectList.push_back("[m]^-!");
  pawObjectList.push_back("[t]^+!");
  pawObjectList.push_back("[t]^-!");
  pawObjectList.push_back("[g]");
  pawObjectList.push_back("j");
  pawObjectList.push_back("b");
  pawObjectList.push_back("p&/");
  pawObjectList.push_back("W");
  pawObjectList.push_back("Z");

  vector<string> printObjectList=objectList;
  string underscore="_";
  if(pawFormatted)
    {
      printObjectList = pawObjectList;
      underscore="#-#";
    }

  vector<string> o, oType, objectType;
  size_t pmissObject = 0;
  bool noPmissObject = true;

  for(size_t i=0; i<objectList.size(); i++)
    for(int j=0; j<FinalState(finalstate).getNumberOfThisObject(objectList[i]); j++)
      if((objectList[i]=="bb")||(objectList[i]=="jj"))
	{
	  o.push_back(printObjectList[i]+Math::ftoa(2*j+1));
	  o.push_back(printObjectList[i]+Math::ftoa(2*j+2));
	  oType.push_back(printObjectList[i]);
	  oType.push_back(printObjectList[i]);
	  objectType.push_back(objectList[i]);
	}	
      else
	if(FinalState(finalstate).getNumberOfThisObject(objectList[i])==1)
	  {
	    if(objectList[i]=="pmiss") {
	      noPmissObject = false;
	      pmissObject = o.size();
	    }
	    o.push_back(printObjectList[i]);
	    oType.push_back(printObjectList[i]);
	    objectType.push_back(objectList[i]);
	  }
	else
	  {
	    o.push_back(printObjectList[i]+Math::ftoa(j+1));
	    oType.push_back(printObjectList[i]);
	    objectType.push_back(objectList[i]);
	  }
  
  // Fill ans with appropriate variables
  vector<string> ans;

  // summed object pt
  if(colliderType=="hadron")
    ans.push_back("sumPt");

  if(colliderRun=="hera") // H1
    {
      // mass of all objects
      ans.push_back("m"+underscore+"all");

      if(useMoreSophisticatedVariables)
	 return(ans);

      // E-pz
      ans.push_back("E-pz");
    }


  // object e or pt
  for(size_t i=0; i<o.size(); i++)
    if(colliderType=="lepton")
      ans.push_back(o[i]+underscore+"e");
    else
      ans.push_back(o[i]+underscore+"pt");

  // missing transverse energy in events without 1pmiss
  if( noPmissObject &&
     (colliderType=="hadron"))
    ans.push_back(printObjectList[9]+underscore+"pt"); // pmiss_pt

  if(useOnlyMostBasicVariables&&
     (colliderType=="hadron"))
    {
      // unclustered pt
      ans.push_back("uncl"+underscore+"pt");

      // clusteredObjectsRecoil_pt
      ans.push_back("clusteredObjectsRecoil"+underscore+"pt");
      // perpendicular and longitudinal components
      if((colliderRun=="tev2")&&
	 (objectType.size()==2)&&
	 (objectType[0]!="pmiss")&&
	 (objectType[1]!="pmiss"))
	{	  
	  ans.push_back("clusteredObjectsRecoil"+underscore+"pt"+underscore+"L");
	  ans.push_back("clusteredObjectsRecoil"+underscore+"pt"+underscore+"T");
	}

      // deltaphi(tau,uncl) and dotProduct(tau,uncl)
      for(size_t i=0; i<o.size(); i++)
	if((objectType[i]=="tau+")||(objectType[i]=="tau-"))
	  {
	    ans.push_back("deltaphi("+o[i]+",uncl)");
	    ans.push_back("dotProduct("+o[i]+",uncl)");
	  }

    }
 

  /* bktemp
  // event Q^2
  if(colliderType=="hadron")
    {
      QuaeroRecoObject* o1 = event.getThisObject("e+",1);
      QuaeroRecoObject* o2 = event.getThisObject("e-",1);
      if((o1!=0)||(o2!=0))
	ans.push_back("Qsqd");
    }
  */

  // object eta's or cos(theta)'s
  for(size_t i=0; i<o.size(); i++)
    {
      if(colliderType=="lepton")
	ans.push_back("cos("+o[i]+underscore+"theta)");
      if(colliderType=="hadron")
	if(objectType[i]!="pmiss")
	  ans.push_back(o[i]+underscore+"eta");
    }
      
  // invariant mass of W with each other object
  // and W kinematics
  if((count(objectType.begin(),objectType.end(),"pmiss")==1)&&
     (count(objectType.begin(),objectType.end(),"e+")+
      count(objectType.begin(),objectType.end(),"mu+")+
      count(objectType.begin(),objectType.end(),"e-")+
      count(objectType.begin(),objectType.end(),"mu-")==1))
    {
      for(size_t i=0; i<objectType.size(); i++)
	if((objectType[i]!="pmiss")&&
	   (objectType[i]!="e+")&&
	   (objectType[i]!="e-")&&
	   (objectType[i]!="mu+")&&
	   (objectType[i]!="mu-"))
	  {
	    ans.push_back("mass(W,"+o[i]+")");
	    for(size_t j=i+1; j<objectType.size(); j++)
	      if((objectType[j]!="pmiss")&&
		 (objectType[j]!="e+")&&
		 (objectType[j]!="e-")&&
		 (objectType[j]!="mu+")&&
		 (objectType[j]!="mu-"))
		ans.push_back("mass(W,"+o[i]+","+o[j]+")");
	  }
      ans.push_back("mass(all)");
      ans.push_back("W"+underscore+"pt");
      ans.push_back("W"+underscore+"pz");
    }

  int numberOfJets = count(objectType.begin(),objectType.end(),"j");
  int numberOfBjets = count(objectType.begin(),objectType.end(),"b");
  int numberOfJetsOrBjets = numberOfJets+numberOfBjets;
  int numberOfLeptons = (count(objectType.begin(),objectType.end(),"e+")+
			 count(objectType.begin(),objectType.end(),"e-")+
			 count(objectType.begin(),objectType.end(),"mu+")+
			 count(objectType.begin(),objectType.end(),"mu-")+
			 count(objectType.begin(),objectType.end(),"tau+")+
			 count(objectType.begin(),objectType.end(),"tau-")			 
			 );
  int numberOfPositivelyChargedLeptons = (count(objectType.begin(),objectType.end(),"e+")+
					  count(objectType.begin(),objectType.end(),"mu+")
					  );

  // Angle between lepton plane and jet plane
  if((numberOfLeptons==2)&&
     (numberOfJetsOrBjets==2))
    {
      ans.push_back("angle(jetPlane,leptonPlane)");
    }
  if((numberOfLeptons==2)&&
     (numberOfJetsOrBjets==1))
    {
      ans.push_back("angle(jet-beamPlane,leptonPlane)");
    }
  // cos(theta*) for lepton pairs
  if((numberOfLeptons==2)&&
     (numberOfPositivelyChargedLeptons==1))
    {
      ans.push_back("cos(theta*)");
    }

  // Montreal Opal Lep 1 4-jet angle
  if((numberOfJetsOrBjets==3))
    {
      ans.push_back("angle(plane(beam,j1),plane(j2,j3))");
    }

  // mass(X)(XX->jjjj)
  if(numberOfJetsOrBjets==4)
    {
      ans.push_back("deltaMass(X)(XX->4j)");
      ans.push_back("mass(X)(XX->4j)");
      ans.push_back("deltaMass(X)/mass(X)(XX->4j)");
      //ans.push_back("deltaMass(X20)(XX->4j)");
      //ans.push_back("mass(X20)(XX->4j)");
      //ans.push_back("deltaMass(X20)/mass(X20)(XX->4j)");
      ans.push_back("planesAngle(XX->4j)");
    }

  /*
  // pT relationship between lowest pT jets
  if(numberOfJetsOrBjets>=2)
    {
      ans.push_back("j"+Math::ftoa(numberOfJetsOrBjets)+underscore+"pt/j"+Math::ftoa(numberOfJetsOrBjets-1)+underscore+"pt");
      ans.push_back("j"+Math::ftoa(numberOfJetsOrBjets-1)+underscore+"pt-j"+Math::ftoa(numberOfJetsOrBjets)+underscore+"pt");
    }
  */

  // Z pt, pz
  if(colliderType=="hadron")
    if( ( (count(objectType.begin(),objectType.end(),"e+")==1)&&
	  (count(objectType.begin(),objectType.end(),"e-")==1) ) ||
	( (count(objectType.begin(),objectType.end(),"mu+")==1)&&
	  (count(objectType.begin(),objectType.end(),"mu-")==1) ) )
      {
	ans.push_back("Z"+underscore+"pt");
	ans.push_back("Z"+underscore+"pz");
      }
  

  // invariant masses of all combinations
  map<vector<string>, int> resonanceMassVariableValuesCount;
  if(o.size()<=6)
  for(int i=0; i<pow((float)2,(float)o.size()); i++)
    {
      int nObs = (int)(Math::computeAverage(Math::getDigits(i,2,o.size()))*o.size()+0.5);
      if(nObs>=2) // don't use the mass of a single final state object -- only use the invariant mass of combinations 
	if( ( useOnlyMostBasicVariables || (nObs<=3) ) )  // only use pairwise masses unless we have the full variable list
	  {
	    string v = "mass(";
	    bool containsPmiss = false;
	    vector<string> oType1;

	    for(size_t j=0; j<o.size(); j++)
	      if(Math::getDigits(i,2,o.size())[j]==1)
		{
		  v = v + o[j]+",";
		  oType1.push_back(oType[j]);
		  if(!noPmissObject && j==pmissObject)
		    {
		      containsPmiss = true;
		      if(colliderType=="hadron")
			{
			  if(nObs==2)
			    v = "transverse"+v;
			  else
			    {
			      v = "";
			      break;
			    }
			}
		    }
		}
	    if( containsPmiss && (colliderType!="hadron") && (nObs==(int) o.size()) )
	      continue;  // don't use the center of mass energy for a lepton machine, since this is a delta function and violates my five lump lemma
	    sort(oType1.begin(),oType1.end());
	    if(resonanceMassVariableValuesCount.find(oType1)==resonanceMassVariableValuesCount.end())
	      resonanceMassVariableValuesCount[oType1] = 0;
	    
	    if(v!="")
	      {
		resonanceMassVariableValuesCount[oType1]++;
		ans.push_back(v.substr(0,v.length()-1)+")");
	      }
	  }
    }
  


  // pairwise object delta(angle)'s
  for(size_t i=0; i<o.size(); i++)
    for(size_t j=i+1; j<o.size(); j++)
      {
	//if(useOnlyMostBasicVariables || (o.size()==2))
	  ans.push_back("deltaphi("+o[i]+","+o[j]+")");
      }
  // deltaphi of pmiss to closest jet
  for(size_t i=0; i<o.size(); i++)
    if(objectType[i]=="pmiss")
      {
	for(size_t j=0; j<o.size(); j++)
	  if(objectType[j]=="j")
	    {
	      ans.push_back("minDeltaPhi(j,pmiss)");
	      ans.push_back("maxDeltaPhi(j,pmiss)");
	      break;
	    }
      }
  // deltaR of lepton or photon to closest jet
  for(size_t i=0; i<o.size(); i++)
    if((objectType[i]=="e+")||(objectType[i]=="e-")||
       (objectType[i]=="mu+")||(objectType[i]=="mu-")||
       (objectType[i]=="tau+")||(objectType[i]=="tau-")||
       (objectType[i]=="ph"))
      {
	for(size_t j=0; j<o.size(); j++)
	  if(objectType[j]=="j")
	    {
	      ans.push_back("minDeltaR("+o[i]+",j)");
	      break;
	    }
      }
  // deltaR of two closest jets
  bool thereAreAtLeastTwoJetsInThisEvent = false;
  for(size_t i=0; (i<o.size())&&(!thereAreAtLeastTwoJetsInThisEvent); i++)
    if(objectType[i]=="j")
      for(size_t j=i+1; j<o.size(); j++)
	if(objectType[j]=="j")
	  thereAreAtLeastTwoJetsInThisEvent = true;
  if(thereAreAtLeastTwoJetsInThisEvent)
    ans.push_back("minDeltaR(j,j)");
  
  // deltaEta of j2 and j3, and SjostrandVariable
  if(numberOfJetsOrBjets==3)
    {
      ans.push_back("deltaEta(j2,j3)");
      // ans.push_back("j2"+underscore+"eta-j3"+underscore+"eta");
      ans.push_back("atan2(deltaPhi(j2,j3),+-(j2"+underscore+"eta-j3"+underscore+"eta))");
    }

  if(useOnlyMostBasicVariables)
    {
      // generated sumPt
      if(useGeneratedSumPt)
	if(colliderRun=="tev2")
	  ans.push_back("'generated sumPt'");

      // event zvtx
      if(useZvtx)
	if(colliderType=="hadron")
	  ans.push_back("zvtx");

      // object phi's
      for(size_t i=0; i<o.size(); i++)
	ans.push_back(o[i]+underscore+"phi");
      
//MRENNA-1
      // object detector eta's
/*      for(size_t i=0; i<o.size(); i++)
	if(objectType[i]!="pmiss")
    ans.push_back(o[i]+underscore+"detEta"); */

      // sum eta of muon pair
      int nMu = 0;
      string muonSumEta = "";
      for(size_t i=0; i<o.size(); i++)
	if((objectType[i]=="mu+")||(objectType[i]=="mu-"))
	  {
	    nMu++;
	    if(nMu==1)
	      muonSumEta += "'"+o[i]+underscore+"eta + ";
	    if(nMu==2)
	      muonSumEta += o[i]+underscore+"eta'";
	  }
      if(nMu==2)
	ans.push_back(muonSumEta);

      // pairwise object deltaR's
      for(size_t i=0; i<o.size(); i++)
	if(objectType[i]!="pmiss")
	  for(size_t j=i+1; j<o.size(); j++)
	    if(objectType[j]!="pmiss")
	      ans.push_back("deltaR("+o[i]+","+o[j]+")");
      
      /*
      // unclustered energy
      ans.push_back("uncl"+underscore+"pt");
      //ans.push_back("'uncl"+underscore+"phi (pt>10)'");
      
	// topological variables
	ans.push_back("aplanarity");
	ans.push_back("sphericity");
      */
    }


  // rootS
  if(colliderType=="lepton")
    ans.push_back("rootS");

  if(useMoreSophisticatedVariables)
    {

      // in 4j final state, take the jet pairings giving the closest mass 
      if((oType.size()==4))
	{
	  bool fourJetFinalState = true;
	  for(int i=0; i<4; i++)
	    if(!((oType[i]=="j")||
		 (oType[i]=="b")))
	      fourJetFinalState = false;
	  if(fourJetFinalState)
	    ans.push_back("mass(jj)");
	}


      // mass of all objects other than (jj) pair that is the closest match to W or Z
      int nj=0, nb=0;
      for(size_t i=0; i<o.size(); i++)
	if(o[i].substr(0,1)=="j")
	  nj++;
      for(size_t i=0; i<o.size(); i++)
	if(o[i].substr(0,1)=="b")
	  nb++;
      if((o.size()>=4)&&(nj>=2))
	{
	  ans.push_back("mass(all"+underscore+"but"+underscore+"W(jj))");
	  ans.push_back("mass(all"+underscore+"but"+underscore+"Z(jj))");
	}

      // Object pairings closest to resonance
      for(map<vector<string>,int>::iterator i=resonanceMassVariableValuesCount.begin(); i!=resonanceMassVariableValuesCount.end(); i++)
	{
	  if(i->second>=2)
	    {
	      for(size_t j=0; j<resonanceMasses.size(); j++)
		{
		  string v = "mass(";
		  for(size_t k=0; k<i->first.size(); k++)
		    v += i->first[k] + ( (k<i->first.size()-1) ? "," : "" );
		  for(size_t k=0; k<i->first.size(); k++)
		    if((colliderType=="hadron")&&
		       (i->first[k]=="pmiss"))
		      v = "transverse"+v;
		  v += ")"+underscore;
		  v += Math::ftoa((int)resonanceMasses[j]);
		  ans.push_back(v);
		}
	    }
	}

      // y_min
      if(nj+nb >= 2)
	ans.push_back("y"+underscore+"min");
      
      // vector summed jet pt
      if(nj+nb>=2)
	ans.push_back("vector"+underscore+"summed"+underscore+"jet"+underscore+"pt");

    }

  // Georgios : Distribution of run where each event was produced.
  //            Integral from run 0 to run RunOfThisEvent L dt / Integral from 0 to LastRun L dt
  if((colliderRun=="tev2")&&
     (useRunHistory))
    {
      // If the file with the luminosities was there and the map filling was successful, 
      // Then calculating the runHistory was successful, so push back to the ans vector the name. 
    // Otherwise don't do that; pretend that runHistory never existed.
    if(integratedLumiMap.empty())
      computeIntegratedLumiMap();
    if(! integratedLumiMap.empty()) 
      ans.push_back("runHistory");
  }

  // single top variables in lepton+pmiss+b+j final states at the Fermilab Tevatron
  if(colliderRun=="tev2")
    if((objectType.size()==4)&&
       (count(objectType.begin(),objectType.end(),"b")==1)&&
       (count(objectType.begin(),objectType.end(),"j")==1)&&
       (count(objectType.begin(),objectType.end(),"pmiss")==1)&&
       (count(objectType.begin(),objectType.end(),"e+")+
	count(objectType.begin(),objectType.end(),"e-")+
	count(objectType.begin(),objectType.end(),"mu+")+
	count(objectType.begin(),objectType.end(),"mu-")==1))
      {
	ans.push_back("mass(W,b,j)-mass(W)-mass(b,j)");
      }

  // 2e+1e-X massClosestToZ
  if(colliderRun=="tev2")
    if((objectType.size()>=3)&&
       (count(objectType.begin(),objectType.end(),"e+")==2)&&
       (count(objectType.begin(),objectType.end(),"e-")==1))
      {
	ans.push_back("mass(e+,e-)_Z");
      }

  // ttbar variables in lepton+pmiss+b+3j final states at the Fermilab Tevatron
  if(colliderRun=="tev2")
    if((objectType.size()==6)&&
       (count(objectType.begin(),objectType.end(),"b")==1)&&
       (count(objectType.begin(),objectType.end(),"j")==3)&&
       (count(objectType.begin(),objectType.end(),"pmiss")==1)&&
       (count(objectType.begin(),objectType.end(),"e+")+
	count(objectType.begin(),objectType.end(),"e-")+
	count(objectType.begin(),objectType.end(),"mu+")+
	count(objectType.begin(),objectType.end(),"mu-")==1))
      {
	// Properties of the chi^2 fit
	ans.push_back("chiSqd"+underscore+"top");
	ans.push_back("p&/"+underscore+"pz");
	// Average quantities of top and anti-top
	ans.push_back("(mass(t)+mass(tbar))/2");
	ans.push_back("(t"+underscore+"pt+tbar"+underscore+"pt)/2");
	// Properties of the ttbar system
	ans.push_back("mass(ttbar)");
	ans.push_back("ttbar"+underscore+"pz");
	ans.push_back("ttbar"+underscore+"pt");
	// Properties of the individual top and anti-top
	ans.push_back("t"+underscore+"eta");
	ans.push_back("tbar"+underscore+"eta");
	//mrennaCompile ans.push_back("t"+underscore+"thetaStar");
	//mrennaCompile ans.push_back("cos(t"+underscore+"thetaStar)");
	// Properties of the b quarks
	ans.push_back("bquark"+underscore+"eta");
	ans.push_back("bquark"+underscore+"pt");
	ans.push_back("bquark"+underscore+"pz");
	ans.push_back("bbar"+underscore+"eta");
	ans.push_back("bbar"+underscore+"pt");
	ans.push_back("bbar"+underscore+"pz");
	// Properties of the W bosons
	ans.push_back("mass(W->lv)");
	ans.push_back("(W->lv)"+underscore+"pt");
	ans.push_back("(W->lv)"+underscore+"pz");
	ans.push_back("mass(W"+underscore+"jj)");
	ans.push_back("(W->jj)"+underscore+"pt");
	ans.push_back("(W->jj)"+underscore+"pz");
      }
 
  // deltaEta_jj
  if((colliderRun=="tev2" || colliderRun=="lhc" )&&
     (count(objectType.begin(),objectType.end(),"j")==2))
    ans.push_back("deltaEta(j1,j2)");

  // pT_jj
  if((colliderRun=="tev2" || colliderRun=="lhc" )&&
     (count(objectType.begin(),objectType.end(),"j")>=2))
    ans.push_back("pT(j1,j2)");

  // mass(j), mass(b)
  // mass(j)/j_pt
  // minMass(j)
//MRENNA-2
/*  if(useOnlyMostBasicVariables &&
     ( (colliderRun=="tev2") ||
       (colliderRun=="lhc") ))
    {
      for(size_t i=0; i<objectType.size(); i++)
	if((objectType[i]=="j")||
	   (objectType[i]=="b"))
	  {
	    ans.push_back("mass("+o[i]+")");      
	    // ans.push_back("mass("+o[i]+")/"+o[i]+underscore+"pt");      
	  }
      if(numberOfJetsOrBjets>=2)
	ans.push_back("minMass(j)");
    }
*/

  // extremum kinematics for dileptons
  if((colliderRun=="tev2"))
    if(numberOfLeptons>=2)
      {
	ans.push_back("min(l"+underscore+"pt)");
	ans.push_back("max(l"+underscore+"eta)");
      }
  

  return(ans);
}




