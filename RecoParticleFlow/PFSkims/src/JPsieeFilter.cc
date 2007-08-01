#include "RecoParticleFlow/PFSkims/interface/JPsieeFilter.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include <algorithm>

using namespace edm;
using namespace std;
using namespace HepMC;



JPsieeFilter::JPsieeFilter(const edm::ParameterSet& iConfig)
{
  label_ = iConfig.getUntrackedParameter("moduleLabel",std::string("source"));
  motherId = iConfig.getParameter<std::vector<int> > ("motherId");
  leptonCuts.type = iConfig.getParameter< int >("leptonType");
  leptonCuts.etaMin = iConfig.getParameter<double>("leptonEtaMin");
  leptonCuts.etaMax = iConfig.getParameter<double>("leptonEtaMax");
  leptonCuts.ptMin = iConfig.getParameter<double>("leptonPtMin");
  doHistos_=iConfig.getUntrackedParameter<bool>("doHistos",false);
  noAccepted = 0;
}


void  JPsieeFilter::beginJob(const edm::EventSetup & c){
  if(doHistos_)
    {
      dbe = edm::Service<DaqMonitorBEInterface>().operator->();
      h0 =  dbe->book2D("h0","Pt vs eta",120,-6,6.,120,0,30.);
      h2 =  dbe->book1D("h2","electron eta ",120,-6,6.);
      h4 =  dbe->book1D("h4","electron pt ",120,0,30.);
    }
}


JPsieeFilter::~JPsieeFilter()
{  
  std::cout << "Total number of accepted events = " << noAccepted << std::endl;
}

void JPsieeFilter::endJob()
{
  if(doHistos_)
    dbe->save("Histos.root");
}

HepMC::GenEvent::particle_const_iterator 
JPsieeFilter::getNextParticle(const HepMC::GenEvent::particle_const_iterator start, 
			      const HepMC::GenEvent::particle_const_iterator end)
{
  HepMC::GenEvent::particle_const_iterator p;
  unsigned nmothers=motherId.size();
  //  std::cout << " Starting getNextParticle"  << std::endl;
  for (p = start; p != end; p++) 
    {
      int event_particle_id = abs( (*p)->pdg_id() );
//   cout << "search "<<event_particle_id<<"\n";
//      std::cout << " Current part " << event_particle_id << std::endl;
      bool result=false;
      for(unsigned ic=0;ic<nmothers;++ic)
	{
	  if(event_particle_id==motherId[ic])
	    {
	      result=true;
	      //	      std::cout << " Found it " << std::endl;
	      return p;
	      break;
	    }
	}
    }
  return p;  
}


bool JPsieeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel(label_, evt);
  
  const HepMC::GenEvent * generated_event = evt->GetEvent();
  //  generated_event->print();
  
  bool event_passed = false;
  HepMC::GenEvent::particle_const_iterator part = getNextParticle(generated_event->particles_begin(), 
								  generated_event->particles_end());
  if(part==generated_event->particles_end()) return false;
  
  while(part!=generated_event->particles_end())
    {
      HepMC::GenVertex* outVertex = (*part)->end_vertex();
      int numChildren = outVertex->particles_out_size();
      //      std::cout << " Found a " << (*part)->pdg_id() << " " << numChildren << std::endl;
      if( numChildren==2) 
	{
	  bool foundpos=false;
	  bool foundneg=false;
	  float etaneg=0.;
	  float etapos=0.;
	  float ptneg=0.;
	  float ptpos=0.;
	  for(std::set<GenParticle*>::const_iterator p = outVertex->particles_out_const_begin();
	      p != outVertex->particles_out_const_end(); p++)
	    {
	      int event_particle_id =  (*p)->pdg_id() ;
	      //	      std::cout << " Son part id " << event_particle_id << std::endl;
	      float eta =(*p)->momentum().eta();
	      float pt =(*p)->momentum().perp();
	      if(event_particle_id==leptonCuts.type)
		{
		  if(doHistos_)  
		    {
		      h0->Fill(eta,pt);
		      h2->Fill(eta);
		      h4->Fill(pt);
		    }
		  if(eta > leptonCuts.etaMin &&
		     eta < leptonCuts.etaMax &&
		     pt > leptonCuts.ptMin)
		    {
		      foundneg=true;
		      ptneg=pt;
		      etaneg=eta;
		    }
		}
	      if(event_particle_id==-leptonCuts.type) 
		{
		  if(doHistos_)
		    {
		      h0->Fill(eta,pt);
		      h2->Fill(eta);
		      h4->Fill(pt);
		    }
		  if(eta > leptonCuts.etaMin &&
		     eta < leptonCuts.etaMax &&
		     pt > leptonCuts.ptMin)
		    {
		      foundpos=true;	  
		      ptpos=pt;
		      etapos=eta;
		    }
		}
	    }
	  if(foundneg&&foundpos)
	    {	  
	      event_passed = true;
	      break;
	    }
	}
      part = getNextParticle(++part,generated_event->particles_end());
    }  
  
  if (event_passed) noAccepted++;
  //  cout << "End filter\n";
  
  
  return event_passed;
}

