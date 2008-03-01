/** \file
 * 
 * DQM for generic FED data: loops over feds and fills datasize histograms
 *   
 * 
 * $Date: 2007/10/09 08:08:04 $
 * $Revision: 1.1 $
 * \author E. Meschi PH/CMD
 *
*/

#include <FWCore/Framework/interface/MakerMacros.h>
#include <FWCore/Framework/interface/EDAnalyzer.h>
#include <FWCore/Framework/interface/Event.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <DataFormats/FEDRawData/interface/FEDHeader.h>
#include <DataFormats/FEDRawData/interface/FEDTrailer.h>
#include <DataFormats/FEDRawData/interface/FEDNumbering.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <iostream>

namespace dqm{
  namespace daq{
    
    class FEDMonitor: public edm::EDAnalyzer{
      
    private:
      unsigned int count_;
      MonitorElement * hfedsize;
      MonitorElement * hfedprof;
      MonitorElement ** hindfed;
      bool dqm;
      bool cleanup;
    public:

      FEDMonitor(const edm::ParameterSet& pset):count_(0), dqm(true), cleanup(false)
      {
	DQMStore *dbe = 0;
	try{
	  dbe = edm::Service<DQMStore>().operator->();
	  dbe->setCurrentFolder("FEDs/Summary");
	  hfedsize = dbe->book1D("fedsize","FED Size Distribution",100,0.,10000.);
	  hfedprof = dbe->bookProfile("fedprof","FED Size by ID", 2048,0.,2048.,
				      0,0.,5000.);
	  hindfed = new MonitorElement*[FEDNumbering::lastFEDId()];

	}
	catch(cms::Exception &e)
	  {
	    dqm = false;
	    // do nothing, it means dqm is not available
	  }
      }
      ~FEDMonitor()
      {
	if(!cleanup) endRun();
	delete [] hindfed;
      }

      void beginJob(edm::EventSetup const&es)
      {
	for(int i = 0; i<FEDNumbering::lastFEDId(); i++)
	  hindfed[i] = 0;
      }
      void endJob()
      {
	if(!cleanup) endRun();
      }
      void endRun()
      {
	cleanup = true;
	DQMStore *dbe = 0;
      
	try{
	  dbe = edm::Service<DQMStore>().operator->();
	  dbe->setCurrentFolder("FEDs/Summary");
	  dbe->removeContents();
	  dbe->setCurrentFolder("FEDs/Details");
	  dbe->removeContents();
	}
	catch(cms::Exception &e)
	  {
	    dqm = false;
	    // do nothing, it means dqm is not available
	  }


      }
      void analyze(const edm::Event & e, const edm::EventSetup& c){
	++count_;
	if(dqm)
	  {
	    edm::Handle<FEDRawDataCollection> rawdata;
	    e.getByType(rawdata);
	    for (int i = 0; i<FEDNumbering::lastFEDId(); i++){
	      const FEDRawData& data = rawdata->FEDData(i);
	      if(size_t size=data.size()) {
		hfedsize->Fill(float(size));
		hfedprof->Fill(float(i),float(size));
		if(i<1024)
		  {
		    if(hindfed[i]==0)
		      {
			DQMStore *dbe = 
			  edm::Service<DQMStore>().operator->();
			dbe->setCurrentFolder("FEDs/Details");
			std::ostringstream os1;
			std::ostringstream os2;
			os1 << "fed" << i;
			os2 << "FED #" << i << " Size Distribution";
			hindfed[i] = dbe->book1D(os1.str(),os2.str(),100,0.,3.*size);
			hindfed[i]->setResetMe(true);
		      }
		    hindfed[i]->Fill(float(size));
		  
		  }
	      }
	    }
	  }
      }
      void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
      {
	//	for (int i = 0; i<FEDNumbering::lastFEDId(); i++)
	//	  if(hindfed[i]==0)
	//	    hindfed[i]->Reset();
      }
    };

  }
}
