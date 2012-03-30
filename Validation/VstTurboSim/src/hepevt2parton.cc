#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <string.h>
#include "Validation/VstMathUtils/interface/Math.hh"
#include "Validation/VstQuaeroUtils/interface/HepevtBlock.hh"
#include "Validation/VstTurboSim/interface/QuaeroPartonObject.hh"
#include "KtJet/KtEvent.h"
#include "KtJet/KtUtil.h"
#include "Validation/VstTurboSim/interface/hepevt2parton.hh"
using namespace std;
using namespace CLHEP;


void hepevt2partonEvent(ofstream& fParton, const HepevtEvent& e)
{
  //cerr << "begin: ";
      string printFormat = "(m)-pt-eta-phi(deg)";
      HepLorentzVector rootS = HepLorentzVector(0,0,0,0);
      double genSumPt = 0.0;

      for(size_t j=0; j<e.particles.size(); j++)
	{
	  HepevtParticle p = e.particles[j];
	  if(p.isthep==1) {
	    rootS += p.getFourVector();
	    genSumPt += p.getFourVector().perp();
	  }
	}

      std::stringstream writeTemp;
      std::stringstream ss;
      ss << genSumPt;

      string collisionType = "", generatedSumPt = "";

      if(rootS.e()==0)
	return; // begin or end run record
      else if(rootS.e()<250)
	{
	  printFormat = "(m)-e-costheta-phi(rad)";
	  collisionType = "e+e-";
	}
      else if(rootS.e()<350)
	collisionType = "e+p";
      else if(rootS.e()<2000)
	{
	  collisionType = "ppbar";
	  generatedSumPt = "0";
	}
      else
	{
	  collisionType = "pp";
	  ss >> generatedSumPt;
	}

      //      bool event_failed = false;
      //check that there are no bad events (where |pz| > E)
      //for(int j=0; j<e.particles.size(); j++)
      //	{	 
      //	  HepevtParticle temp = e.particles[j];
      //  if(abs(temp.getFourVector().pz())>temp.getFourVector().e()) {
      //    event_failed = true;
      //    break;
      //  }
      //}
      
      double wt = 1, zvtx = 0;
      //      if (event_failed == true) //we'll mark a bad events by wt = 0
      //	wt = 0;

      //      fParton << "sig " << e.nevhep << " " << wt << " " 
      writeTemp << "sig " << e.nevhep << " " << wt << " " 
	      << collisionType << " " << rootS.e() << " " << generatedSumPt << " ";
      if(rootS.e()>250)
		writeTemp << zvtx << " ";
	//	fParton << zvtx << " ";
      
      //cerr << " cluster ";

      vector<HepLorentzVector> particlesToBeClustered = vector<HepLorentzVector>(0);
      vector<HepLorentzVector> bHadrons = vector<HepLorentzVector>(0);
      vector<HepLorentzVector> taus = vector<HepLorentzVector>(0);
      vector<string> tausCharge = vector<string>(0);
      vector<HepLorentzVector> photons = vector<HepLorentzVector>(0);
      vector<HepLorentzVector> lightLeptons = vector<HepLorentzVector>(0);

      vector<HepLorentzVector> ElectronFromHardInteraction = vector<HepLorentzVector>(0);     
      vector<HepLorentzVector> MuonFromHardInteraction = vector<HepLorentzVector>(0);     

      //this finds the electron of muons from hard interaction.
      //status = 3 is a documentation line
      //status = 123 or 124 seems to be the one used for the herwig ttbar
      for(size_t j=0; j<e.particles.size(); j++)
	{
	  HepevtParticle p = e.particles[j];
	  if(p.isthep==3 || p.isthep==123 || p.isthep==124) {
	    if(abs(p.idhep)==11 && p.getFourVector().perp()>5 )
	      ElectronFromHardInteraction.push_back(p.getFourVector());
	    if(abs(p.idhep)==13 && p.getFourVector().perp()>5 )
	      MuonFromHardInteraction.push_back(p.getFourVector());
	  }
	}
      
      //This section tries to find all the light leptons that we need to add to clustering.
      //Since we decided to cluster everything except the hard interaction ones
      //this is unnecessary
      /*
      for(int j=0; j<e.particles.size(); j++)
	{
	  HepevtParticle p = e.particles[j];
	  if(p.isthep==1)
	    if(((abs(p.idhep)>=11)||(abs(p.idhep)<=14)) && (p.getFourVector().perp()>5)) {

	      bool hardElectronOrMuon = false;
	      //get all the light leptons not from hard interaction
	      for (int k=0;k<ElectronFromHardInteraction.size();k++) {		
		if (Math::deltaR(p.getFourVector().phi(), Math::theta2eta(p.getFourVector().theta()), 
		                 ElectronFromHardInteraction[k].phi(), 
				 Math::theta2eta(ElectronFromHardInteraction[k].theta())) < 0.1)
		  hardElectronOrMuon = true;
	      }
	      for (int k=0;k<MuonFromHardInteraction.size();k++) {		
		if (Math::deltaR(p.getFourVector().phi(), Math::theta2eta(p.getFourVector().theta()), 
		                 MuonFromHardInteraction[k].phi(), 
				 Math::theta2eta(MuonFromHardInteraction[k].theta())) < 0.1)
		  hardElectronOrMuon = true;
	      }
	      
	      //only add the light leptons NOT from hard interaction
	      if (hardElectronorMuon == false)
		lightLeptons.push_back(p.getFourVector());
	    }	
	}
      */

      for(size_t j=0; j<e.particles.size(); j++)
	{	 
	  HepevtParticle p = e.particles[j];

	  string objectType = "";
	  if(p.isthep==1)
	    {
	      if(abs(p.idhep)==11) objectType = "e";
	      if(abs(p.idhep)==13) objectType = "mu";
	      if(objectType!="") 
		{
		  objectType += ( p.idhep>0 ? "-" : "+" );		  
		  bool hardElectronOrMuon = false;		  

		  //if the particle is an e or mu from the hard interaction
		  //we should output it!
		  //Otherwise we should cluster it		  

		  if (objectType == "e+" || objectType == "e-")
		    for (size_t k=0;k<ElectronFromHardInteraction.size();k++) {		      
		      if (Math::deltaR(p.getFourVector().phi(), Math::theta2eta(p.getFourVector().theta()), 
				       ElectronFromHardInteraction[k].phi(), 
				       Math::theta2eta(ElectronFromHardInteraction[k].theta())) < 0.1)
			hardElectronOrMuon = true;
		    }
		  
		  if (objectType == "mu+" || objectType == "mu-")
		    for (size_t k=0;k<MuonFromHardInteraction.size();k++) {
		      
		      if (Math::deltaR(p.getFourVector().phi(), Math::theta2eta(p.getFourVector().theta()), 
				       MuonFromHardInteraction[k].phi(), 
				       Math::theta2eta(MuonFromHardInteraction[k].theta())) < 0.1)
			hardElectronOrMuon = true;
		    }


		  if (hardElectronOrMuon == false)
		    objectType = "";
		}
	   
	      if(objectType=="")
		{
		  if((abs(p.idhep)!=12)&&
		     (abs(p.idhep)!=14)&&
		     (abs(p.idhep)!=16))
		    particlesToBeClustered.push_back(p.getFourVector());
		  if(p.idhep==22) 
		    photons.push_back(p.getFourVector());
		}
	      else
		{
		  QuaeroPartonObject o = QuaeroPartonObject(objectType, p.getFourVector());
		  if( ((rootS.e() < 250) && (o.getFourVector().e()    >  5)) ||
		      ((rootS.e() > 250) && (o.getFourVector().perp() > 10)) )
		    writeTemp << o.print(printFormat) << " ";
		    //		    fParton << o.print(printFormat) << " ";
		}
	    }
	  if(p.isthep==2)
	    {
	      if(((abs(p.idhep)> 500) && (abs(p.idhep)< 600)) || // B meson
		 ((abs(p.idhep)>5000) && (abs(p.idhep)<6000)) )
		{
		  bHadrons.push_back(p.getFourVector());
		}
	    }

	  if(p.isthep==3)
	    {
	      if(abs(p.idhep)==15) 
		{
		  taus.push_back(p.getFourVector());
		  tausCharge.push_back( (p.idhep>0) ? "-" : "+" );
		}
	    }
	}
      //cerr << " 2 ";

      int type = 0, angle = 0;
      int recom = 1;            // E recombination scheme
      //      double rParameter = 1.0;  // R parameter
      double q0res = 12.0;      // resolution, in units of GeV
      if( (rootS.e()>180) && (rootS.e()<210) )      // LEP
	{
	  type  = 1;    // ee
	  angle = 1;    // angular kt def
	  q0res = 12;   // units are GeV
	}
      else if( (rootS.e()>300) && (rootS.e()<320) ) // HERA
	{
	  type  = 3;    // pe
	  angle = 2;    // DeltaR
	  q0res = 15;   // units are GeV
	}
      else if( (rootS.e()>1750) )                   // Tevatron
	{
	  type  = 4;    // pp
	  angle = 2;    // DeltaR
	  q0res = 20;   // units are GeV
	  //q0res = 15; //this
	}
      else
	{
	  cout << "ERROR:  rootS = " << rootS.e() << " SKIPPING EVENT " << endl;
	  return;
	}

      //cerr << " 3 ";

      //for (int n=0;n<particlesToBeClustered.size();n++)
      //	if (abs(particlesToBeClustered[n].pz())>particlesToBeClustered[n].e())
      //	  cout << "stage 2: " << n << " " << particlesToBeClustered[n].px() << " " << particlesToBeClustered[n].py() << " " << particlesToBeClustered[n].pz() << " " << particlesToBeClustered[n].e() << ".............." 
	    
      //	       << particlesToBeClustered[n].perp() << " " << particlesToBeClustered[n].pseudoRapidity() << " " << particlesToBeClustered[n].phi() << endl;


      //vector<HepLorentzVector> preclustering = vector<HepLorentzVector>(0);
      vector<KtJet::KtLorentzVector> preclustering = vector<KtJet::KtLorentzVector>(0);
      while(!particlesToBeClustered.empty())
	{
	  bool q = false;
	  for(int j=particlesToBeClustered.size()-2; j>=0; j--)
	    if(
	       //(particlesToBeClustered[j]+particlesToBeClustered.back()).m()<5)
	       Math::addInQuadrature(
				     Math::deltaphi(particlesToBeClustered[j].phi(),
						    particlesToBeClustered.back().phi()),
				     (particlesToBeClustered[j].theta()-
				      particlesToBeClustered.back().theta())
				     ) < 0.2 )

	       {
		particlesToBeClustered[j] += particlesToBeClustered.back();
		
		q = true;
		break;
	      }
	  if(!q)
	    preclustering.push_back(particlesToBeClustered.back());
	  particlesToBeClustered.pop_back();
	}

      //cerr << " 4 ";

      if(!preclustering.empty())
	{
	  for (size_t n=0;n<preclustering.size();n++)
	    if (abs(preclustering[n].pz())>preclustering[n].e()) {
	      cout << "stage 3: " << n << " " << preclustering[n].px() << " " << preclustering[n].py() << " " << preclustering[n].pz() << " " << preclustering[n].e() << ".............." 

		   << preclustering[n].perp() << " " << preclustering[n].pseudoRapidity() << " " << preclustering[n].phi() << endl;	  

	      
	      //cout << e << endl;
	      //let's just fix this by hand.
	      preclustering[n].setPz(sqrt(preclustering[n].e()*preclustering[n].e() - preclustering[n].px()*preclustering[n].px() - preclustering[n].px()*preclustering[n].px()));


	      cout << "fixed: " << n << " " << preclustering[n].px() << " " << preclustering[n].py() << " " << preclustering[n].pz() << " " << preclustering[n].e() << ".............." 

		   << preclustering[n].perp() << " " << preclustering[n].pseudoRapidity() << " " << preclustering[n].phi() << endl;
	    }

	  

	  KtJet::KtEvent ktEvent(preclustering, type, angle, recom);
	  KtJet::KtFloat dcut = q0res*q0res; 
	  KtJet::KtFloat ycut = pow(q0res/rootS.e(),2.);
	  //	  bool tau_veto = false;

	  //cout << " 1\n ";
	  if(type==1)
	    ktEvent.findJetsY(ycut);
	  else
	    ktEvent.findJetsD(dcut);

	  //cerr << " 2\n ";
	  vector<KtJet::KtLorentzVector> jets = ktEvent.getJets();

	  for(size_t j=0; j<jets.size(); j++)
	    {	      

	      if( ((rootS.e() < 250) && (jets[j].e()    >  5)) ||
		  ((rootS.e() > 250) && (jets[j].perp() > 10)) )
		{
		  string objectType = "j";
		  for(size_t k=0; k<bHadrons.size(); k++)
		    if( Math::deltaR(jets[j].phi(),     Math::theta2eta(jets[j].theta()), 
				     bHadrons[k].phi(), Math::theta2eta(bHadrons[k].theta()))
			< 0.4 )
		      objectType = "b";
		  
		  for(size_t k=0; k<taus.size(); k++)
		    if( Math::deltaR(jets[j].phi(),     Math::theta2eta(jets[j].theta()), 
		  		     taus[k].phi(), Math::theta2eta(taus[k].theta()))
		  	< 0.4 )
		      //tau_veto = true; //we throw away the jet corresponding to the tau
		      objectType = "tau"+tausCharge[k];

		  for(size_t k=0; k<photons.size(); k++)
		    if( ( Math::deltaR(jets[j].phi(),     Math::theta2eta(jets[j].theta()), 
				       photons[k].phi(), Math::theta2eta(photons[k].theta()))
			  < 0.4 ) &&
			( photons[k].e() > 0.9 * jets[j].vect().mag() ) &&
			( photons[k].e() < jets[j].vect().mag()+1 ) )
		      objectType = "ph";
		  //fParton change		  		  
		  writeTemp << QuaeroPartonObject(objectType, HepLorentzVector(jets[j].vect().mag(), Hep3Vector(jets[j].px(), jets[j].py(), jets[j].pz()))).print(printFormat) << " ";
		}
	    }
	}
      
      //      fParton << "; " << endl;
      writeTemp << "; " << endl;

      fParton << writeTemp.str();
      return;
}



void hepevt2parton(string hepevtFileName, string partonFileName)
{
  ifstream fHepevt(hepevtFileName.c_str());
  ofstream fParton(partonFileName.c_str());

  //  int nEvents = 1000;
  // fHepevt >> nEvents;

  HepevtEvent e;
  while(      fHepevt >> e  )
    {
      //cout << e << endl;
      hepevt2partonEvent(fParton, e);
    }
  fHepevt.close();
  fParton.close();

  return;
}

void hepevt_prepare(string infilename, string outfilename)
{

  ofstream fout(outfilename.c_str());
  ifstream fin(infilename.c_str());

  cout << outfilename.c_str() << endl;

  /*  int nEvents;
  fin >> nEvents; //first thing should be the number of Events
  fout << nEvents << endl;
  cout << "Total Number of Events: " << nEvents << endl; */

  string tempin;
  char tempchar[256];
  //  char templine[80];
  string nevhep;
  int nparticles;
  int p_isthep, p_idhep, p_jmohep0, p_jmohep1, p_jdahep0, p_jdahep1;
  string p_px, p_py, p_pz, p_E, p_M;
  string p_vx, p_vy, p_vz, p_vt;

  int i=0;
  while(fin >> tempin) {

    //for (int i = 0; i < nEvents; i++) { //loop over events
    //for (int i = 0; i < 1; i++) {

    //fin.getline(templine,80); //There's a line of ___ in the first line of each event
    //fin >> tempin;
    fin >> tempin;
    //cout << tempin << " - ";
    fin  >> tempin;
    //cout << tempin << " - ";
    fin  >> tempin;
    //cout << tempin << " - ";
    fin  >> tempin;
    //cout << tempin << " - ";
    assert(tempin == "Event#:");

    
    //get the event#
    fin >> tempchar;
    //cout << tempchar << " " << endl;
    nevhep = strtok(tempchar, ",");
    //fout << nevhep << " ";
    fout << i++ << " "; //let's use our count instead
    cout << "Event:" << i << " ";

    //get the #of particles
    fin >> nparticles;
    fout << nparticles << endl;
    cout << nparticles << endl;

    fin >> tempin;
    //cout << tempin << " - ";
    fin >> tempin;
    //cout << tempin << " - ";
    fin >> tempin;
    //cout << tempin << " - ";
    fin >> tempin;
    //cout << tempin << " - ";
    fin >> tempin;
    //cout << tempin << " - ";
    fin >> tempin;
    //cout << tempin << " - ";
    assert(tempin == "Precision");

    //fin.getline(templine,1); //this reads the end of the last line ending at precision
    //fin >> tempin;
    //cout << "here" << tempin << endl;
    //fin.getline(templine,80);
    //cout << templine << endl;
    //fin.getline(templine,80);
    //cout << templine << endl;
    //fin.getline(templine,80);
    //cout << templine << endl;
    //fin >> tempin >> tempin >> tempin; //the getline doesn't work well 

    fin >> tempin;
    //cout << tempin << " ";

    //skip the header
    fin >> tempin  >> tempin  >> tempin >> tempin >> tempin >> tempin >> tempin  >> tempin  >> tempin >> tempin >> tempin >> tempin  >> tempin  >> tempin  >> tempin  >> tempin  >> tempin  >> tempin  >> tempin  >> tempin  >> tempin ;

    assert (tempin == "[mm]");
    fin >> tempin;

    //cout << tempin << endl;


 //read in particles
    for (int p=0;p<nparticles;p++) {
      
      //particle index
      int p_index;
      fin >> p_index;
      //cout << p_index << " ";
      //cout << p+1 << endl;

      //assert(p_index == p+1);
      
      fin >> p_isthep;
     
      //cout << p_isthep << " ";
      fin >> p_jmohep0;
     
      //cout << p_jmohep0 << " ";
      fin >> p_jdahep0;
     
      //cout << p_jdahep0 << " ";

      fin >> tempin;
//sometimes there is no space between the bracket and the number
      if (tempin == "(") {
	fin >> tempin;
	p_px = tempin.substr(0,tempin.length()-1);
	//cout << "px=" << tempin << " " << "p_px=" << p_px << " ";

      } else {
	p_px = tempin.substr(1,tempin.length()-2); //remove the comma too
	//cout << "px=" << tempin << " ";
      }
      //assert(tempin == "(");


      //read in the momenta
      //fin >> tempchar;
      //p_px = strtok(tempchar, ",");

      //cout << tempin << " ";
      //cout << "p_px=" << p_px << " ";
      

      fin >> tempin;
      p_py = tempin.substr(0,tempin.length()-1);
     
      
      fin >> tempin;
      p_pz = tempin.substr(0,tempin.length()-1);
     

      fin >> tempin;
      p_E = tempin.substr(0,tempin.length()-1);
     

      fin >> tempin;
      p_M = tempin.substr(0,tempin.length()-1); //remove the ')'
     

      //cout << " p_M=" << p_M << " ";

      //idhep
      fin >> p_idhep;
      fin >> p_jmohep1;
      fin >> p_jdahep1;

      
      //cout << p_jdahep1 << " ";

      fin >> tempin;

      //sometimes there is no space between the bracket and the number
      if (tempin == "(") {
	fin >> tempin;
	p_vx = tempin.substr(0,tempin.length()-1);
      } else {
	p_vx = tempin.substr(1,tempin.length()-2); //remove the comma too
      }
      //assert(tempin == "(");

      //read in the vertex
      //fin >> tempchar;
      //p_vx = strtok(tempchar, ",");
    
      fin >> tempin;
      p_vy = tempin.substr(0,tempin.length()-1);
      fin >> tempin;
      p_vz = tempin.substr(0,tempin.length()-1);
      fin >> tempin;
      p_vt = tempin.substr(0,tempin.length()-1);
      //cout << "p_vt=" << p_vt << " ";

      //cout << endl;

      fout << p_isthep << " " << p_idhep << " " << p_jmohep0 << " " << p_jmohep1 << " " << p_jdahep0 << " " << p_jdahep1 << " " << p_px << " "<< p_py << " "<< p_pz << " " << p_E << " "<< p_M << " " << p_vx << " " << p_vy << " " << p_vz << " " << p_vt << endl;


    } //next particle

    //fin.getline(templine,80); //There's a line of ___ at the end
    fin >> tempin;
  } //next Event


  fin.close();
  fout.close();
  return;
}
