// -*- C++ -*-
//
// Package:    Filtro
// Class:      Filtro
// 
/**\class Filtro Filtro.cc UserAnalysisCode/Filtro/src/Filtro.cc

 Description: <one line class summary>

 Implementation:
     <First implementation>

*/
//
// Original Author:  Letizia Lusito
//         Created:  Wed Jul 25 15:00:16 CEST 2007
// $Id$
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// #include "FWCore/Framework/interface/EventSetup.h"
// //#include "FWCore/Framework/interface/Handle.h"
//  #include "FWCore/Framework/interface/ESHandle.h"

 #include "FWCore/MessageLogger/interface/MessageLogger.h"

 #include "DataFormats/Candidate/interface/Particle.h"
 #include "DataFormats/MuonReco/interface/Muon.h"
 #include "DataFormats/Common/interface/EDProduct.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"

 #include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include <cmath>

#include <TSystem.h>
#include <TClass.h>


////////////////includo io secondo l'ispirazione del momento

#include "DataFormats/Candidate/interface/Candidate.h" /////////////
#include <DataFormats/HepMCCandidate/interface/GenParticleCandidate.h> 


//#include <SimG4Core/Notification/interface/GenParticleInfo.h> 
//#include "CLHEP/HepMC/defs.h"
//#include "CLHEP/HepMC/include/PythiaWrapper.h"
// #include "CLHEP/HepMC/ConvertHEPEVT.h"


#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"
#include "HepMC/Flow.h" 
//#include "Vector/LorentzVector.h"
//#include "HepMC/HepMCConfig.h"
#include "HepPDT/ParticleID.hh"
#include "HepMC/Polarization.h"
#include <FWCore/Framework/interface/GenericHandle.h> 

//#include <DataFormats/FWLite/interface/Handle.h> 

////////////////da Giuseppe

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
 #include "FWCore/Framework/interface/Event.h"
 #include "FWCore/ParameterSet/interface/ParameterSet.h"
 #include "FWCore/ParameterSet/interface/InputTag.h"
 #include "DataFormats/Common/interface/Handle.h"
 #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
 #include "HepMC/GenEvent.h"
 #include <string>
 #include <vector>
 #include <set>

//#include "RecoTauTag/HLTProducers/interface/TauJetMCFilter.h"
#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"
#include "UserAnalysisCode/ZTauTauAnalysis/plugins/Filtro.h"

using namespace edm;

using namespace std;
using namespace reco;
using namespace HepMC;

//
// constructors and destructor
//
Filtro::Filtro(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed

}


Filtro::~Filtro()
{ 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
Filtro::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


 bool atleast1_ZtoTauTauMuJet=false;
  bool foundZ = false;
  
  foundjetfromtaup=false;
  
  foundlepttaup=false;
  
  foundtaup=false;
  foundtaun=false;
  
  foundjetfromtaun=false;

  foundlepttaun=false;

  bool foundmufromtaup=false;
  bool foundneumufromtaup=false;
  bool foundneutaufromtaup=false;
  bool  notfoundelectronp=true;
  
  bool foundmufromtaun=false;
  bool foundneumufromtaun=false;
  bool foundneutaufromtaun=false;
  bool  notfoundelectronn=true;		      
  
 //  Handle<HepMCProduct> simevt;
//   iEvent.getByLabel("VtxSmeared",simevt);
//   HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(simevt->GetEvent()));
//   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); 
// 	p != myGenEvent->particles_end(); ++p)


//  Handle<HepMCProduct> simevt;
//   iEvent.getByLabel("VtxSmeared",simevt);
//   HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(simevt->GetEvent()));
//   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); 
// 	p != myGenEvent->particles_end(); ++p)

//  Handle<GenParticleCandidate> simevt;
//   iEvent.getByLabel("VtxSmeared",simevt);
//   HepMC::GenEvent * myGenEvent = new HepMC::GenEvent(*(simevt->GetEvent()));
//   for ( HepMC::GenEvent::particle_const_iterator p = myGenEvent->particles_begin(); 
// 	p != myGenEvent->particles_end(); ++p)

 
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("source", evt);

  HepMC::GenEvent *generated_event = new HepMC::GenEvent(*(evt->GetEvent()));
  HepMC::GenEvent::particle_iterator p;
  for (p = generated_event->particles_begin(); p!=generated_event->particles_end(); p++)
   

  {
            
      //const HepMC::GenParticle * part = * p;
      
      if ( ((*p) ->pdg_id()== 23) && ((*p)->status() == 3) )
	{
	  foundZ = true;
	  //cout<<"foundZ ="<< foundZ<<endl;
	  
	  //cout << "Found a Z with a status "<<(*p)->status()  << endl;

	  //////////////////////

	  // std::vector<GenParticleCandidate*> children=(*p)->listDaughter();
	  // std::vector<HepMC::GenParticle*> children=(*p)->listChildren();
	  // vector<HepMC::GenParticle*>::const_iterator aDaughter;
	  
	  //std::vector<HepMC::GenParticle*> children=(*p)->listChildren();
	  //vector<HepMC::GenParticle*>::const_iterator aDaughter;	  
	  // if((*p)->numChildren()>2)
	  //if (children.size() >= 2)
	  
	   if ((*p)->end_vertex()->particles_out_size()>2) 
	  
	  {
	      for( HepMC::GenVertex::particle_iterator aDaughter=(*p)->end_vertex()->particles_begin(HepMC::descendants); aDaughter !=(*p)->end_vertex()->particles_end(HepMC::descendants);aDaughter++)
		
		{		
		  /////////////////////////////
		  
		  //if (children.size() >= 2)
		  //{
		  
		  //for (aDaughter = children.begin(); aDaughter != children.end(); aDaughter++) 
		  
		  //if (aDaughter.size()>2)
		  //{
		  //cout << "\n livello decadimento 1: id = "<<(*aDaughter)->pdg_id()<<" e status = "<<(*aDaughter)->status();
		  
		  ///////////////////////////////////////copia di qui per neg

		  if( (*aDaughter)->pdg_id()==15 ) 
		    {     
		      //cout<< " -> found a tau with status = " << (* aDaughter)->status() <<endl;
		      
		      


		      // vector<HepMC::GenParticle*> bychildren = (*aDaughter)->listChildren();
		      //vector<HepMC::GenParticle*>::const_iterator byaDaughter;
		      
		      //cout << "Taus from Z with charge " <<  (*aDaughter)->pdg_id() <<endl; 
		      
		      for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)

			//for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)
			{ 
			  ///cout <<"\n livello decadimento 2: id =: "<<(*byaDaughter)->pdg_id()<< " e status = "<<(*byaDaughter)->status();
		

			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==15) 
			    {
			      
			      //cout << "-> found a tau minus decaying in itself" << endl;
			      foundtaup=true;
			      
			      //vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
			      //vector<HepMC::GenParticle*>::const_iterator tauDaughter;
			      
			      //for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)

 for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)
				{				  
				  
				  if ( (*tauDaughter)->pdg_id()==13) 
				    {
				      //cout << "Found a muon from tau decay p" << endl;
				      foundmufromtaup=true;
				      //cout<<""<<endl;
				    }
				  if ( (*tauDaughter)->pdg_id()==-14) 
				    {
				      //cout << "Found a neu muon from tau decay p" << endl;
				      foundneumufromtaup=true;
				      //cout<<"found neu mu"<<endl;
				    }
				  if ( (*tauDaughter)->pdg_id()==16)
				    { 
				      //cout << "Found a neu tau from tau decay p" << endl;
				      foundneutaufromtaup=true;
				    }
				   if ( (*tauDaughter)->pdg_id()==11)
				    { 
				      //cout << "Found an electron from tau decay p" << endl;
				     
				      notfoundelectronp=false;
				    }


				  }

			      if (foundneutaufromtaup && foundneumufromtaup && foundmufromtaup)
				{
				  //cout << "found a tau decaying in mu neumu neutau p" << endl;
				  foundlepttaup=true;
				 
				}
			      
			      else if ((foundneutaufromtaup==1)&&(foundmufromtaup==0)&&(notfoundelectronp==1))
				{
				 
				  foundjetfromtaup=true;  
				  
				}//else
			      
			      
			    } // if byaDaughter=tau with status 2 closed
			  
			}// for closed
		      
		      
		    } //if aDaughter=tau closed daq cui inizi a cop

		  /////////////////////////////////////////////////////////////////////////////////

		  if( (*aDaughter)->pdg_id()==-15 ) 
		    {     
		      //cout<< " -> found a tau with status = " << (* aDaughter)->status() <<endl;
		      
		      ///////////////////// 
		      //vector<GenParticle*> bychildren = (*aDaughter)->listChildren();
		      //vector<HepMC::GenParticle*>::const_iterator byaDaughter;
		      
		      //cout << "Taus from Z with charge " <<  (*aDaughter)->pdg_id() <<endl; 
		     
		     
		      
		      //for (byaDaughter = bychildren.begin();byaDaughter != bychildren.end();byaDaughter++)


 for( HepMC::GenVertex::particle_iterator byaDaughter=(*aDaughter)->end_vertex()->particles_begin(HepMC::descendants); byaDaughter !=(*aDaughter)->end_vertex()->particles_end(HepMC::descendants);byaDaughter++)
			{ 
			  //cout <<"\n livello decadimento 2: id =: "<<(*byaDaughter)->pdg_id()<< " e status = "<<(*byaDaughter)->status();
	       	  
			  if ((*byaDaughter)->status()==2 && (*byaDaughter)->pdg_id()==-15) 
			    {
			      
			      //cout << "-> found a tau plus decaying in itself" << endl;
			     foundtaun=true;
			      
			     //vector<HepMC::GenParticle*> tauchildren = (*byaDaughter)->listChildren();
			     //vector<HepMC::GenParticle*>::const_iterator tauDaughter;
			      
			     //for (tauDaughter = tauchildren.begin();tauDaughter != tauchildren.end();tauDaughter++)

for( HepMC::GenVertex::particle_iterator tauDaughter=(*byaDaughter)->end_vertex()->particles_begin(HepMC::descendants); tauDaughter !=(*byaDaughter)->end_vertex()->particles_end(HepMC::descendants);tauDaughter++)

				{
				  
				  if ( (*tauDaughter)->pdg_id()==-13) 
				    {
				      //cout << "Found a muon from tau decay n" << endl;
				      foundmufromtaun=true;
				    }
				  if ( (*tauDaughter)->pdg_id()==14) 
				    {
				      // cout << "Found a neu muon from tau decay n" << endl;
				      foundneumufromtaun=true;
				    }
				  if ( (*tauDaughter)->pdg_id()==-16)
				    { 
				      //cout << "Found a neu tau from tau decay n" << endl;
				      foundneutaufromtaun=true;
				    }
				   if ( (*tauDaughter)->pdg_id()==-11)
				    { 
				      //cout << "Found an electron from tau decay n" << endl;
				     
				      notfoundelectronn=false;
				    }


				  }
			      if (foundneutaufromtaun && foundneumufromtaun && foundmufromtaun)
				{
				  //cout << "found a tau decaying in mu neumu neutau n" << endl;
				  foundlepttaun=true;
				  
				}
			      
			      else if ((foundneutaufromtaun==1)&&(foundmufromtaun==0)&&(notfoundelectronn==1))
				{
				  
				  foundjetfromtaun=true;
				  
				}//else
			      
			    } // if byaDaughter=tau with status 2 closed
			  
			}// for closed
		      

		     
		      
		      
		    } //if aDaughter=tau closed daq cui inizi a cop
		  
 ///////////////////////////////////////////////////////////////////////////////////////////////
		  
		  
		}	//   for aDaughter closed 
	      

	      
	    }    //if closed
	}       //if Z conditions
  
      
    } 
  
   if (foundZ==1&&foundtaup==1&&foundtaun==1)
     {
        if((foundlepttaun&&foundjetfromtaup)||(foundlepttaup&&foundjetfromtaun))
       {
	   atleast1_ZtoTauTauMuJet = true;
	   cout <<" Interesting event reconstructed:"<< atleast1_ZtoTauTauMuJet<<endl;
	 }   
     }
   ///////////////////////////////////
  //
  // delete myGenEvent; ////??????????
  //
  // eventCount_++;
  cout << "atleast1_Sim_ZtoTauTauMuJet = " << atleast1_ZtoTauTauMuJet << endl;
  
  //
  // if ((eventCount_ <= maxNumEvents_) && atleast1_mu && atleast1_ZtoTauTauMuJet )
  /// if ((eventCount_ <= maxNumEvents_) && atleast1_ZtoTauTauMuJet ) 
  
  if ( atleast1_ZtoTauTauMuJet ) 
    {
      return true;
    }
  else 
    {
      return false;
    }
  


}

// ------------ method called once each job just before starting event loop  ------------
void 
Filtro::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Filtro::endJob() {
}

