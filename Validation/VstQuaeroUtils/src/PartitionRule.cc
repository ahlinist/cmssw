/********************************
Implementation of PartitionRule, which defines the rules for 
partitioning a set of events into exclusive final states for Quaero or Sleuth.
********************************/


#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
#include "Validation/VstQuaeroUtils/interface/FinalState.hh"
#include "Validation/VstQuaeroUtils/interface/PartitionRule.hh"
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemFile.hh"
using namespace std;


/***  Constructors  ***/

PartitionRule::PartitionRule(std::string _ruleType)
{
  ruleType = _ruleType;
  globalLesserGenerationSwitch = false;
  globalChargeConjugation = false;
  quarksComeInPairs = false;
  leptonsComeInPairs = false;
  identifyWeakBosons = false;
  usePmiss = true;
  distinguishCharge = true;
  maximumNumberOfJetsToIdentify = 99;
  pmin = 0;
  pmin_leptons = 0;
  pmin_pmiss = 0;

  possibleObjects = vector<string>(0);
  possibleObjects.push_back("e+");
  possibleObjects.push_back("e-");
  possibleObjects.push_back("mu+");
  possibleObjects.push_back("mu-");
  possibleObjects.push_back("tau+");
  possibleObjects.push_back("tau-");
  possibleObjects.push_back("ph");
  possibleObjects.push_back("pmiss");
  possibleObjects.push_back("j");
  possibleObjects.push_back("b");
  possibleObjects.push_back("jj");
  possibleObjects.push_back("bb");
  possibleObjects.push_back("w+");
  possibleObjects.push_back("w-");
  possibleObjects.push_back("z");



  bool ruleTypeUnderstood = false;

  if(ruleType.find("lep2")!=string::npos)
    pmin = 10;
  if(ruleType.find("hera")!=string::npos)
    {
      usePmiss = false;
      pmin = 19.999999;
    }
  if(ruleType.find("tev1")!=string::npos)
    pmin = 15;
  if(ruleType.find("tev2")!=string::npos)
    pmin = 17.0;
  if(ruleType.find("lhc")!=string::npos)
    pmin = 25;
  if(ruleType.find("TurboSim")!=string::npos)
    {
      usePmiss = false;
      distinguishCharge = false;
    }

  pmin_leptons = (ruleType=="tev2-Vista-lowPtDileptons" ? 4 : pmin);
  pmin_pmiss = pmin;
  if(ruleType.find("jetIgnore")!=string::npos) {
    pmin=100000;
    pmin_leptons = 0;
    pmin_pmiss = 0;
  }
  if(ruleType.find("noCuts")!=string::npos) {
    pmin=0;
    pmin_leptons = 0;
    pmin_pmiss = 0;
  }
  if(ruleType.find("onlyJetCuts")!=string::npos) {
    pmin_leptons = 0;
    pmin_pmiss = 0;
  }

  if((ruleType.find("tev2-Vista")!=string::npos)||
     (ruleType.find("tev2-Quaero")!=string::npos))
    {
      distinguishCharge = true;
      globalChargeConjugation = true;
      maximumNumberOfJetsToIdentify = 4;
      for(int npmiss=0; npmiss<=1; npmiss++)
	for(int nb=0; nb<=2; nb++)
	  for(int nj=0; nj<=4; nj++)
	    {
	      if(nb+nj==0) continue;
	      string fs = "";
	      if(nb>0) fs += Math::ftoa(nb)+"b";
	      if(nj>0) fs += Math::ftoa(nj)+"j";
	      if(npmiss>0) fs += "1pmiss";
	      sumPtFinalStateBinEdges[FinalState(fs)].push_back(400); 
	    }
      // Divide the Nj1tau+ final states into above and below 400 GeV in SumPt
      sumPtFinalStateBinEdges[FinalState("1j1tau+")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("2j1tau+")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("3j1tau+")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("4j1tau+")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("1j1tau-")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("2j1tau-")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("3j1tau-")].push_back(400); 
      sumPtFinalStateBinEdges[FinalState("4j1tau-")].push_back(400); 
      finalStatesToDrop.push_back(FinalState("1j_sumPt0-400"));
      finalStatesToDrop.push_back(FinalState("1e+"));
      finalStatesToDrop.push_back(FinalState("1e-"));
      finalStatesToDrop.push_back(FinalState("1mu+"));
      finalStatesToDrop.push_back(FinalState("1mu-"));
    }
  /*
  if(ruleType.find("tev2-Quaero")!=string::npos)     
    {
      maximumNumberOfJetsToIdentify = 4;
      for(int npmiss=0; npmiss<=1; npmiss++)
	for(int nb=0; nb<=4; nb++)
	  for(int nj=0; nj<=4; nj++)
	    {
	      if(nb+nj==0) continue;
	      string fs = "";
	      if(nb>0) fs += Math::ftoa(nb)+"b";
	      if(nj>0) fs += Math::ftoa(nj)+"j";
	      if(npmiss>0) fs += "1pmiss";
	      sumPtFinalStateBinEdges[FinalState(fs)].push_back(500);
	    }
    }
  */
  if(ruleType.find("Sleuth")!=string::npos)
    {
      globalLesserGenerationSwitch = true;
      globalChargeConjugation = true;
      quarksComeInPairs = true;
      leptonsComeInPairs = true;
      maximumNumberOfJetsToIdentify = 4;
      // identifyWeakBosons = true;
    }
  if(ruleType=="lhc-Sleuth")
    {
      identifyWeakBosons = true;
      maximumNumberOfJetsToIdentify = 6;
    }
  if(ruleType=="hera-Quaero")
    {
      sumPtFinalStateBinEdges[FinalState("1e+1j")].push_back(140);
      sumPtFinalStateBinEdges[FinalState("2j")].push_back(100);
      sumPtFinalStateBinEdges[FinalState("2j1pmiss")].push_back(120);
      sumPtFinalStateBinEdges[FinalState("3j")].push_back(100);
      sumPtFinalStateBinEdges[FinalState("1e+2j")].push_back(100);
      sumPtFinalStateBinEdges[FinalState("1e+1j1pmiss")].push_back(100);
    }

  if(ruleType.find("jetInclusive")!=string::npos)
    maximumNumberOfJetsToIdentify = 2;
  if(ruleType.find("oneJetInclusive")!=string::npos)
    maximumNumberOfJetsToIdentify = 1;
  if(ruleType.find("upTo4Jets")!=string::npos) {
    maximumNumberOfJetsToIdentify = 4;
    pmin=50;
  }

  std::vector<std::string> ruleTypes;

  ruleTypes.push_back("lowPtDileptons");
  ruleTypes.push_back("oneJetInclusive");
  ruleTypes.push_back("jetInclusive");
  ruleTypes.push_back("upTo4Jets");
  ruleTypes.push_back("jetIgnore");
  ruleTypes.push_back("noCuts");
  ruleTypes.push_back("onlyJetCuts");


  if((ruleType=="lep2-Vista") ||
     (ruleType=="lep2-TurboSim") ||
     (ruleType=="lep2-Quaero") ||

     (ruleType=="tev1-Vista") ||
     (ruleType=="tev1-Quaero") ||

     (ruleType=="tev2-Vista") ||
     (ruleType=="tev2-Vista-lowPtDileptons") ||
     (ruleType=="tev2-TurboSim") ||
     (ruleType=="tev2-TurboSim-jetInclusive") ||
     (ruleType=="tev2-Vista-jetInclusive") ||
     (ruleType=="tev2-Quaero") ||
     (ruleType=="tev2-Sleuth") ||

     (ruleType=="hera-Vista") ||
     (ruleType=="hera-TurboSim") ||
     (ruleType=="hera-Quaero") ||

     (ruleType=="lhc-Vista") ||
     (ruleType=="lhc-Vista-jetInclusive") ||
     (ruleType=="lhc-Vista-oneJetInclusive") ||
     (ruleType=="lhc-Vista-jetIgnore") ||
     (ruleType=="lhc-Vista-upTo4Jets") ||
     (ruleType=="tev2-Vista-noCuts") ||
     (ruleType=="lhc-Vista-noCuts") ||
     (ruleType=="lhc-Vista-onlyJetCuts") ||
     (ruleType=="tev2-Vista-onlyJetCuts") ||
     (ruleType=="lhc-TurboSim") ||
     (ruleType=="lhc-Sleuth") ||

     (ruleType=="default"))
    ruleTypeUnderstood = true;

  if(!ruleTypeUnderstood) {
    cout << "PartitionRule rule type " << ruleType << " not understood.  Aborting." << endl;
    cout << " Try one of the following rules : " << endl;
    for(std::vector<std::string>::const_iterator cit = ruleTypes.begin(); cit!=ruleTypes.end(); ++cit) {
       cout << (*cit) << endl;
    }

  }
  assert(ruleTypeUnderstood);
  std::cout << "Partition rule is " << ruleType << std::endl;
  std::cout << "Partition cuts are " << std::endl;
  std::cout << "pmin = " << pmin << std::endl;
  std::cout << "pmin leptons = " << pmin_leptons << std::endl;
  std::cout << "pmin pmiss = " << pmin_pmiss << std::endl;
  std::cout << "max no. of jets = " << maximumNumberOfJetsToIdentify << std::endl;
  return;
}

string itoa(int i)
{
  char a[100];
  sprintf(a,"%i",i);
  return(a);
}

double PartitionRule::getPmin()
{
  return(pmin);
}

/***  Methods  ***/

FinalState PartitionRule::getFinalState(QuaeroEvent & event)
{
  vector<QuaeroRecoObject> objects = event.getObjects();

  bool dropTheEvent = false;

  // At CDF, events with one central electron and one plug photon should have two electrons
/*  if(ruleType.find("tev2")!=string::npos)
    {
      if((event.numberOfObjects("e",15,2.5)==1)&&
	 (event.numberOfObjects("ph",15,2.5)==1))
	{
	  for(size_t j=0; j<objects.size(); j++)
	    if(objects[j].getObjectTypeSansSign()=="e")
	      for(size_t i=0; i<objects.size(); i++)
		if(objects[i].getObjectType()=="ph")
		  if((fabs((objects[i].getFourVector()+objects[j].getFourVector()).m()-91.1)<10) ||
		     (fabs(Math::theta2eta(objects[i].getFourVector().theta()))>1))
		    objects[i] = QuaeroRecoObject((objects[j].getObjectType()=="e+" ? "e-" : "e+" ),objects[i].getFourVector());
	  event.setObjects(objects);
	}
    } */


  int e_plus = event.numberOfObjects("e+",pmin_leptons);
  int e_minus = event.numberOfObjects("e-",pmin_leptons);
  int mu_plus = event.numberOfObjects("mu+",pmin_leptons);
  int mu_minus = event.numberOfObjects("mu-",pmin_leptons);

  int tau_plus = event.numberOfObjects("tau+",pmin_leptons);
  int tau_minus = event.numberOfObjects("tau-",pmin_leptons);
  int ph = event.numberOfObjects("ph",pmin);
  int j = event.numberOfObjects("j",pmin);
  int b = event.numberOfObjects("b",pmin);
  int w_plus = event.numberOfObjects("w+");
  int w_minus = event.numberOfObjects("w-");
  int z = event.numberOfObjects("z");

  if(event.collider()=="hera")
    {
      j = event.numberOfObjects("j",pmin,2.44); // maxEta = 2.44
      ph = event.numberOfObjects("ph",pmin,2.44); // maxEta = 2.44
    }

  // do jets
  int jj=0,bb=0;
  if(quarksComeInPairs)
    {
      /*
	if(event.numberOfObjects("j",10.)>j)
	  j++;
      */
      bb = b/2; // integer arithmetic
      jj = j/2; // if j=3, for example, jj=1 (there is one jet pair)
      if(((b%2)==1)&&    // odd number of b-jets
	 (j>=1))       // and at least one other jet
	{
	  bb++;
	  jj = (j-1)/2;  // move one of the jets to a b-jet
	}
      if(((2*jj+2*bb)>maximumNumberOfJetsToIdentify)&&
	 (!((event.collider()=="tev2")&&
	    (e_plus+e_minus+mu_plus+mu_minus+tau_plus+tau_minus+ph==0)&&
	    (event.numberOfObjects("j",200,1)+event.numberOfObjects("b",200,1)==0))))	
	jj = (maximumNumberOfJetsToIdentify - 2*bb)/2;
      if(jj<0)
	jj=0;
    }
  /*
  if(((j+b)>maximumNumberOfJetsToIdentify)&&
     (!((event.collider()=="tev2")&&
	(e_plus+e_minus+mu_plus+mu_minus+tau_plus+tau_minus+ph==0)&&
	(event.numberOfObjects("j",200,1)+event.numberOfObjects("b",200,1)==0))))	
    j = maximumNumberOfJetsToIdentify-b;
  */
  if( j+b > maximumNumberOfJetsToIdentify ) j= maximumNumberOfJetsToIdentify;
  if(j<0) 
    j=0;

  double sumpt = 0;
  for(size_t i=0; i<objects.size(); i++)
    sumpt += objects[i].getFourVector().perp();

  if(!distinguishCharge)
    {
      e_plus += e_minus;
      mu_plus += mu_minus;
      tau_plus += tau_minus;
      e_minus = mu_minus = tau_minus = 0;
      for(size_t i=0; i<objects.size(); i++)
	{
	  string objectType = objects[i].getObjectType();
	  if(objectType.substr(objectType.length()-1,1)=="-")
	    objects[i].chargeConjugate();
	}
    }
	
      
  CLHEP::HepLorentzVector pmiss = event.getPmiss();

  int met = ((pmiss.perp() > pmin_pmiss) &&
	     (pmiss.e() > pmin_pmiss)); // 3*sqrt(sumpt));
//Mrenna:  remove these TeV specific cuts
  if(event.collider()=="tev2" && false )
    {
      if((event.numberOfObjects("e")==0) &&
	 (event.numberOfObjects("mu")==0) &&
	 (event.numberOfObjects("tau")==0) &&
	 (event.numberOfObjects("ph")==0) &&
	 (event.sumPt()>400))
	{
	  met = (pmiss.perp() > 50);
	  //if MET opposite the strongest jet, don't believe in it
	  if((event.numberOfObjects("j",pmin)>1)&&(event.numberOfObjects("b",pmin)==0)&&
	     (Math::deltaphi(event.getThisObject("j",1)->getFourVector().phi(),pmiss.phi())>3))
	    met=0;
	  //same for b and no j
	  if((event.numberOfObjects("b",pmin)>1)&&(event.numberOfObjects("j",pmin)==0)&&
	     (Math::deltaphi(event.getThisObject("b",1)->getFourVector().phi(),pmiss.phi())>3))
	    met=0;
	  //same for both
	  if((event.numberOfObjects("j",pmin)>=1)&&(event.numberOfObjects("b",pmin)>=1)) {
	    double biggestJetsPhi=(event.getThisObject("j",1)->getFourVector().perp() > event.getThisObject("b",1)->getFourVector().perp() ) ? event.getThisObject("j",1)->getFourVector().phi() : event.getThisObject("b",1)->getFourVector().phi() ;
	    if(Math::deltaphi(biggestJetsPhi,pmiss.phi())>3)
	      met=0;
	  }
	  //for events with 2j back to back and nothing else, don't believe in met.
	  if((event.getThisObject("j",1)!=NULL)&&
	     (event.getThisObject("j",2)!=NULL)&&
	     (event.numberOfObjects("j",pmin)==2)&&
	     (event.numberOfObjects("b",pmin)==0)&&
	     (Math::deltaphi(event.getThisObject("j",1)->getFourVector().phi(),
			     event.getThisObject("j",2)->getFourVector().phi())>2.75))
	    met=0;
	  //the same for bj
	  if( (event.numberOfObjects("j",pmin)==1)&&
	      (event.numberOfObjects("b",pmin)==1) &&
	      (Math::deltaphi(event.getThisObject("j",1)->getFourVector().phi(),
			      event.getThisObject("b",1)->getFourVector().phi())>2.75))
	    met=0;
	  //the same for bb
	  if( (event.numberOfObjects("b",pmin)==2)&&
	      (event.numberOfObjects("j",pmin)==0)&&
	      (Math::deltaphi(event.getThisObject("b",1)->getFourVector().phi(),
			      event.getThisObject("b",2)->getFourVector().phi())>2.75))
	    met=0;
	}

      vector< pair<double,double> > objectsPtPhi;
      for(size_t i=0; i<objects.size(); i++)
	if((objects[i].getObjectType()!="uncl")&&
	   (objects[i].getFourVector().perp()>pmin))
	  objectsPtPhi.push_back(pair<double,double>(objects[i].getFourVector().perp(),objects[i].getFourVector().phi()));
      if(!objectsPtPhi.empty())
	{
	  sort(objectsPtPhi.begin(),objectsPtPhi.end());
	  reverse(objectsPtPhi.begin(),objectsPtPhi.end());
	}
      if( (objectsPtPhi.size()==2) &&
	  (event.numberOfObjects("e",pmin,2.5)-event.numberOfObjects("e",pmin,1.0)==1) &&
	  (event.numberOfObjects("j",pmin)==1) )
	met = 0;
      if( (objectsPtPhi.size()==2)&&
	  ( (Math::deltaphi(objectsPtPhi[0].second,objectsPtPhi[1].second) > 3) &&
	    ( (Math::deltaphi(objectsPtPhi[0].second,pmiss.phi()) > 3) ||
	      (Math::deltaphi(objectsPtPhi[1].second,pmiss.phi()) > 3) ) ) )
	met = 0;

      if((met==1)&&
	 (event.numberOfObjects("e+",15,2.5)==1)&&
	 (event.numberOfObjects("e-",15,2.5)==1))
	{
	  QuaeroRecoObject* e1 = event.getThisObject("e+");
	  QuaeroRecoObject* e2 = event.getThisObject("e-");
	  if((e1!=NULL)&&(e2!=NULL))
	    {
	      double mee = (e1->getFourVector()+e2->getFourVector()).m();
	      if((75<mee)&&(mee<105))
		if(pmiss.perp()<25)
		  met=0;
	    }
	}

      /*
      if( (event.getThisObject("j",1)!=NULL) &&
	  (Math::deltaphi(event.getThisObject("j",1)->getFourVector().phi(),
			  pmiss.phi()) > 3.0) )
	met = 0;
      */

      if(met==1)
	{
	  double hadEnergyTowards=0, hadEnergyAway=0;
	  for(size_t i=0; i<objects.size(); i++)
	    if((objects[i].getObjectType()=="j")||
	       (objects[i].getObjectType()=="b")||
	       (objects[i].getObjectTypeSansSign()=="tau")||
	       (objects[i].getObjectType()=="uncl"))
	      {
		double hadEnergyParallel = pmiss.vect().unit().dot(objects[i].getFourVector().vect());
		if(hadEnergyParallel<0)
		  hadEnergyAway+=abs(hadEnergyParallel);
		else
		  hadEnergyTowards+=hadEnergyParallel;
	      }
	  if(2.5*sqrt(hadEnergyAway)+2.5*sqrt(hadEnergyTowards)
	     > (pmiss.perp()-10))
	    met=0;
	}

    }
  if(event.collider()=="tev1")
    if(event.numberOfObjects("e")>=2)
      met=0; // pmiss was not identified for two lepton events in Quaero@D0RunI
  if(event.collider()=="hera") // see email from Caron 2/21/2005
    {
      double e = 0, pz = 0;
      for(size_t i=0; i<objects.size(); i++)
	{
	  e += objects[i].getFourVector().e();
	  pz += objects[i].getFourVector().pz();
	}
      if(e-pz>48)
	met=0;             // the final state is not characterized by missing pt

      /*** Missing momentum should not be within deltaR<1 of one of the clustered objects ***/
      // calculate neutrino 4 vector
      CLHEP::HepLorentzVector pAllVisible;
      for(size_t i=0; i<objects.size(); i++)
	pAllVisible = pAllVisible+objects[i].getFourVector();
      double electronEnergy = 27.5; // units are GeV
      // use E-PZ constrain to derive neutrino 4-vector
      // Assumption Sum_i E_i-P_Z,i= 55.2 (including neutrino!)
      // (This assumption is wrong in our case !!!)
      double fEmPz = pAllVisible.e() - pAllVisible.pz();
      double empzbal=2*electronEnergy-fEmPz;
      double NuPtSqd = pAllVisible.perp()*pAllVisible.perp();
      double fNuE  =   (empzbal/2.) + (NuPtSqd/(2.*empzbal));
      double fNuPz =  -(empzbal/2.) + (NuPtSqd/(2.*empzbal)); // E+pz = P_T^2 / (E-Pz)  
      CLHEP::HepLorentzVector ppmiss(fNuE,CLHEP::Hep3Vector(-pAllVisible.px(),-pAllVisible.py(),fNuPz));
      for(size_t i=0; i<objects.size(); i++)
	if((objects[i].getObjectType()!="uncl")&&
	   (objects[i].getFourVector().perp()>pmin)&&
	   (Math::deltaR(ppmiss.phi(), Math::theta2eta(ppmiss.theta()),
			 objects[i].getFourVector().phi(), Math::theta2eta(objects[i].getFourVector().theta()))<1))
	  met=0;
    }
  if((event.collider()=="lep2"))
    {
      for(size_t i=0; i<objects.size(); i++)
	if((objects[i].getFourVector().perp()>20)&&
	   (Math::deltaphi(objects[i].getFourVector().phi(),
			   pmiss.phi())>3.0))
	  met = 0;
    }


  if(!usePmiss)
    {
      met = 0;
      // remove unclustered energy
      for(size_t i=0; i<objects.size(); i++)
	if(objects[i].getObjectType()=="uncl")
	  {
	    objects.erase(objects.begin()+i);
	    i--;
	  }
    }

  // do leptons

  // leptons come in pairs
  if(leptonsComeInPairs)
    {
      int totalNumberOfChargedLeptons = (e_plus + e_minus +
					 mu_plus + mu_minus +
					 tau_plus + tau_minus);
      if((totalNumberOfChargedLeptons==1)&&
	 (met==0)) {
	e_plus = e_minus = mu_plus =  mu_minus = tau_plus = tau_minus = 0;
	dropTheEvent=true;
      }
      if((totalNumberOfChargedLeptons==3)&&
	 (met==0))
	met=1;
    }

  // global lesser generation switch
  if(globalLesserGenerationSwitch &&
  (e_plus+e_minus < mu_plus+mu_minus))
    {
      swap(e_plus,mu_plus);
      swap(e_minus,mu_minus);
      for(size_t i=0; i<objects.size(); i++)
	{
	  if(objects[i].getComponentType()=="e+")
	    objects[i].changeComponentType("mu+");
	  else
	    if(objects[i].getComponentType()=="mu+")
	      objects[i].changeComponentType("e+");
	  if(objects[i].getComponentType()=="e-")
	    objects[i].changeComponentType("mu-");
	  else
	    if(objects[i].getComponentType()=="mu-")
	      objects[i].changeComponentType("e-");
	}
    }

  // global charge conjugation
  if(globalChargeConjugation &&
     ( (e_plus < e_minus) ||
       ((e_plus == e_minus) &&
	(mu_plus < mu_minus)) ||
       ((e_plus == e_minus) &&
	(mu_plus == mu_minus) &&
	(tau_plus < tau_minus)) 
       ) )
    {
      swap(e_plus,e_minus);
      swap(mu_plus,mu_minus);
      swap(tau_plus,tau_minus);
      for(size_t i=0; i<objects.size(); i++)
	{
	  if(objects[i].getComponentType()=="e+")
	    objects[i].changeComponentType("e-");
	  else if(objects[i].getComponentType()=="e-")
	    objects[i].changeComponentType("e+");

	  if(objects[i].getComponentType()=="mu+")
	    objects[i].changeComponentType("mu-");
	  else if(objects[i].getComponentType()=="mu-")
	    objects[i].changeComponentType("mu+");

	  if(objects[i].getComponentType()=="tau+")
	    objects[i].changeComponentType("tau-");
	  else if(objects[i].getComponentType()=="tau-")
	    objects[i].changeComponentType("tau+");
	}
    }

  if((w_plus>0)||(w_minus>0)||(z>0))
    if(!((e_plus==0)&&(met==0)))
      {
	cout << event << endl;
	assert(false);
      }
  assert((w_plus+w_minus<=1)&&(z<=1)&&(met<=1));
  
  QuaeroEvent modifiedEvent = event;
  modifiedEvent.changeComponents(objects); 
  
  if(identifyWeakBosons)
    {
      // combine e+e- into a Z if 82<mee<100
      if((e_plus==1)&&(e_minus==1)&&(mu_plus==0)&&(mu_minus==0)&&(met==0))
	{
	  CLHEP::HepLorentzVector ee = modifiedEvent.getThisObject("e+")->getFourVector() + modifiedEvent.getThisObject("e-")->getFourVector();
	  if((82<ee.m())&&(ee.m()<100))
	    {
	      QuaeroRecoObject zobj = QuaeroRecoObject("z",ee);
	      for(size_t i=0; i<objects.size(); i++)
		if((objects[i]==*modifiedEvent.getThisObject("e+"))||
		   (objects[i]==*modifiedEvent.getThisObject("e-")))
		  {
		    objects.erase(objects.begin()+i);
		    i--;
		  }
	      objects.push_back(zobj);
	      z++; e_plus--; e_minus--;
	    }
	  // combine e+e-ph into a Z if 82<meeph<100
	  if((ee.m()<82)&&(ph==1))
	    {
	      CLHEP::HepLorentzVector eeph = 
		modifiedEvent.getThisObject("e+")->getFourVector() + 
		modifiedEvent.getThisObject("e-")->getFourVector() + 
		modifiedEvent.getThisObject("ph")->getFourVector();
	      if((82<eeph.m())&&(ee.m()<100))
		{
		  QuaeroRecoObject zobj = QuaeroRecoObject("z",eeph);
		  for(size_t i=0; i<objects.size(); i++)
		    if((objects[i]==*modifiedEvent.getThisObject("e+"))||
		       (objects[i]==*modifiedEvent.getThisObject("e-"))||
		       (objects[i]==*modifiedEvent.getThisObject("ph")))
		      {
			objects.erase(objects.begin()+i);
			i--;
		      }
		  objects.push_back(zobj);
		  z++; e_plus--; e_minus--; ph--;
		}
	    }
	}
      
      // combine e+ met into a W if the enu transverse mass < 110 GeV
      if((e_plus+e_minus+mu_plus+mu_minus==1)&&(met==1))
	{
	  CLHEP::HepLorentzVector nu = modifiedEvent.getPmiss();
	  string w = "";
	  string lepton="";
	  if((e_plus==1)||(mu_plus==1))
	    {
	      if(e_plus==1)
		lepton = "e+";
	      if(mu_plus==1)
		lepton = "mu+";
	      w="w+";
	    }
	  if((e_minus==1)||(mu_minus==1))
	    {
	      if(e_minus==1)
		lepton = "e-";
	      if(mu_minus==1)
		lepton = "mu-";
	      w="w-";
	    }	  
	  CLHEP::HepLorentzVector l = modifiedEvent.getThisObject(lepton)->getFourVector();
	  CLHEP::HepLorentzVector enu = l+nu;
	  if(enu.mt()<110)
	    {
	      QuaeroRecoObject::ChiSqdConstrainNeutrino(l,nu); // this function changes nu
	      enu = l+nu;
	      QuaeroRecoObject wobj = QuaeroRecoObject(w,enu); 
	      for(size_t i=0; i<objects.size(); i++)
		if((objects[i]==*modifiedEvent.getThisObject(lepton)))
		  {
		    objects.erase(objects.begin()+i);
		    i--;
		  }
	      objects.push_back(wobj);
	      if(w=="w+")
		w_plus++;
	      if(w=="w-")
		w_minus++; 
	      e_plus = e_minus = mu_plus = mu_minus = met = 0;
	    }
	}
    }

  event.changeComponents(objects);

  vector<string> o;
  vector<int> n;
  if(e_plus>0)
    {      o.push_back("e+");      n.push_back(e_plus);    }
  if(e_minus>0)
    {      o.push_back("e-");      n.push_back(e_minus);    }
  if(mu_plus>0)
    {      o.push_back("mu+");      n.push_back(mu_plus);    }
  if(mu_minus>0)
    {      o.push_back("mu-");      n.push_back(mu_minus);    }
  if(tau_plus>0)
    {      o.push_back("tau+");      n.push_back(tau_plus);    }
  if(tau_minus>0)
    {      o.push_back("tau-");      n.push_back(tau_minus);    }
  if(ph>0)
    {      o.push_back("ph");      n.push_back(ph);    }
  if(met>0)
    {      o.push_back("pmiss");      n.push_back(met);    }
  if(quarksComeInPairs)
    {
      if(jj>0)
	{      o.push_back("jj");      n.push_back(jj);    }
      if(bb>0)
	{      o.push_back("bb");      n.push_back(bb);    }
    }
  else
    {
      if(j>0)
	{      o.push_back("j");      n.push_back(j);    }
      if(b>0)
	{      o.push_back("b");      n.push_back(b);    }
    }
  if(identifyWeakBosons)
    {
      if(w_plus>0)
	{      o.push_back("w+");      n.push_back(w_plus);    }
      if(w_minus>0)
	{      o.push_back("w-");      n.push_back(w_minus);    }
      if(z>0)
	{      o.push_back("z");      n.push_back(z);    }
    }

  FinalState fs=FinalState(o,n);

  if((!finalStatesToDrop.empty())&&
     (find(finalStatesToDrop.begin(),finalStatesToDrop.end(),fs)!=finalStatesToDrop.end()))
    dropTheEvent = true;

  if ( dropTheEvent ) 
    fs = FinalState("");
  
  if(sumPtFinalStateBinEdges.find(fs)!=sumPtFinalStateBinEdges.end())
    {
      vector<double> sumPtRange(2,0);
      sumPtRange[1] = FLT_MAX;
      double sumPt = event.sumPt();
      for(size_t i=0; i<sumPtFinalStateBinEdges[fs].size(); i++)
	{
	  if( (sumPtRange[0] < sumPtFinalStateBinEdges[fs][i]) && (sumPtFinalStateBinEdges[fs][i] < sumPt) )
	    sumPtRange[0] = sumPtFinalStateBinEdges[fs][i];
	  if( (sumPt < sumPtFinalStateBinEdges[fs][i]) && (sumPtFinalStateBinEdges[fs][i] < sumPtRange[1]) )
	    sumPtRange[1] = sumPtFinalStateBinEdges[fs][i];
	}
      fs = FinalState(o,n,sumPtRange);
    }
      
  return(fs);
}


std::vector<double> PartitionRule::getSleuthVariables(const QuaeroEvent& event)
{
  double sumPt = event.sumPt();
  return(vector<double>(1,sumPt));
}


void PartitionRule::partition(string filename)
{
  istream* fin;
  if((filename.length()>1)&&
     (filename.substr(0,1)=="-"))
    {
      filename = filename.substr(1);
      fin = &cin;
    }
  else
    {
      fin = new ifstream(filename.c_str());
    }
  system(("rm "+filename+"_*.txt 2> /dev/null").c_str());
  map<FinalState,stringstream*> partitioning = partition(*fin);
  for(map<FinalState,stringstream*>::iterator i=partitioning.begin(); i!=partitioning.end(); i++)
    {
      if(i->first.getTextString()!="")
	{
	  string foutFilename = filename+"_"+i->first.getTextString()+".txt";
	  if(i->first.getTextString()=="")
	    foutFilename = filename+".null";
	  ofstream fout(foutFilename.c_str());	  
	  string blah;
	  while(getline(*(i->second),blah))
	    fout << (string)blah << endl;
	  delete i->second;
	  i->second = NULL;
	}
    }
  return;
}

std::map<FinalState,std::stringstream*> PartitionRule::partition(std::istream& metaStream)
{
  QuaeroItemFile<QuaeroEvent> fin(metaStream);
  QuaeroEvent event;
  std::map<FinalState,std::stringstream*> ans;

  int count2=0;
  while(fin.nextEvent(event))
    {
      count2++;
      if(count2%10000==0) cout << count2/1000 << "k " << flush;
      FinalState fs = getFinalState(event);
      if(ans.find(fs)==ans.end())
	ans[fs] = new stringstream;
      (*ans[fs]) << event.print() << endl;
    }
  return(ans);
}


void PartitionRule::justKeepTheRelevantStuff(vector<string> _relevantFinalStates, 
					     string metaBkgFilename, string relevantBkgFilename, 
					     bool keepOnlyThirdThird)
{

  bool everythingIsRelevant = false;

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

  vector<FinalState> relevantFinalStates;
  for(size_t i=0; i<_relevantFinalStates.size(); i++)
    relevantFinalStates.push_back(FinalState(_relevantFinalStates[i]));

  ifstream fmetaBkg(metaBkgFilename.c_str());
  ofstream frelevantBkg(relevantBkgFilename.c_str());
  string event;
  int counter = -1;
  while(getline(fmetaBkg,event))
    {
      if(event=="")
	continue;
      counter++;
      if(keepOnlyThirdThird && (counter%3!=2))
	continue;
      vector<vector<string> > possibleObjects(0);
      for(size_t marker1=0; marker1<event.length()-1; marker1++)
	{
	  if(!((event.substr(marker1,1)==" ")&&(event.substr(marker1+1,1)!=" ")))
	    continue;
	  size_t marker2 = marker1+1;
	  while((marker2<event.length())&&(event.substr(marker2,1)!=" ")&&(event.substr(marker2,1)!="{"))
	    marker2++;
	  size_t iObject = find(objectList.begin(),objectList.end(),event.substr(marker1+1,marker2-marker1-1))-objectList.begin();
	  if(iObject<objectList.size())
	    {
	      possibleObjects.push_back(vector<string>(1,""));
	      possibleObjects[possibleObjects.size()-1].push_back(objectList[iObject]);
	      if(event.substr(marker2,1)=="{")
		{
		  size_t marker3 = marker2;
		  while(event.substr(marker3,1)!="}")
		    {
		      marker3++;
		      if(event.substr(marker3,1)=="/")
			{
			  size_t marker4 = marker3+1;
			  while(event.substr(marker4,1)!="/")
			    marker4++;
			  int jObject = find(objectList.begin(),objectList.end(),event.substr(marker3+1,marker4-marker3-1))-objectList.begin();
			  possibleObjects[possibleObjects.size()-1].push_back(objectList[jObject]);
			  marker3=marker4;
			}
		    }
		}
	    }
	}
      vector<int> base(possibleObjects.size());
      int nPossibleFinalStates = 1;
      for(size_t i=0; i<base.size(); i++)
	{
	  base[i] = possibleObjects[i].size();
	  nPossibleFinalStates *= base[i];
	}
      if(nPossibleFinalStates>200)
	frelevantBkg << event << endl;
      else
	for(int i=0; i<nPossibleFinalStates; i++)
	  {
	    vector<int> whichObjects = Math::getDigits(i, base, base.size());
	    vector<string> objects;
	    for(size_t j=0; j<whichObjects.size(); j++)
	      if(possibleObjects[j][whichObjects[j]]!="")
		objects.push_back(possibleObjects[j][whichObjects[j]]);
	    FinalState fs1 = FinalState(objects);
	    objects.push_back("pmiss");
	    FinalState fs2 = FinalState(objects);
	    if((find(relevantFinalStates.begin(),relevantFinalStates.end(),fs1)!=relevantFinalStates.end())||
	       (find(relevantFinalStates.begin(),relevantFinalStates.end(),fs2)!=relevantFinalStates.end())||
	       everythingIsRelevant)
	      {
		frelevantBkg << event << endl;
		break;
	      }
	  }
    }
  fmetaBkg.close();
  frelevantBkg.close();

  return;
}

