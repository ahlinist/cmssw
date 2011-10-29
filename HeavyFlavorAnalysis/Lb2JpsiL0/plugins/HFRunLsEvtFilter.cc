#include "FWCore/Framework/interface/MakerMacros.h"
#include "HFRunLsEvtFilter.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <string>
#include <fstream>
#include <sstream>

using namespace edm;
using namespace std;

// ----------------------------------------------------------------------
HFRunLsEvtFilter::HFRunLsEvtFilter(const edm::ParameterSet& iConfig) {
    cout << "----------------------------------------------------------------------" << endl;
    cout << "--- HFRunLsEvtFilter constructor" << endl;
    cout << "----------------------------------------------------------------------" << endl;
  
    fNgood= 0;
    fNtot = 0; 
    fVerbose = 0;

    // read file
    edm::RunNumber_t run;
    edm::LuminosityBlockNumber_t ls;
    edm::EventNumber_t evt;

    const std::string filename(iConfig.getParameter<edm::FileInPath>("infile").fullPath());
    std::ifstream infile(filename.c_str());
    cout << "==> HFRunLsEvtFilter> Attempt to read from file " << filename << "...." << endl;
    if (!infile) { std::cerr << "Cannot open file " << filename << " - operation aborted." << std::endl; }
    std::string aLine;
    unsigned int counter(0);
    while (std::getline(infile,aLine))
    {
	// strip off everything after a hash
	std::string stripped = "";
	std::string::iterator iter = std::find(aLine.begin(), aLine.end(), '#');
	std::copy(aLine.begin(), iter, std::back_inserter(stripped));
	// read one measurment and add to vector if successfull
	std::istringstream iss(stripped, std::istringstream::in);
	if ( !(iss >> run >> ls >> evt).fail() )
	{
	    runLsEvtContainer_.addEntry(run, ls, evt);
	    counter++;
	}
    }
    cout << "==> HFRunLsEvtFilter> Filereading completed, read " << counter << " of lines with valid content. "
	 << runLsEvtContainer_.getFullSize() << " entries in memory" << endl;
}


// ----------------------------------------------------------------------
HFRunLsEvtFilter::~HFRunLsEvtFilter() {  
    cout << "==> HFRunLsEvtFilter> Total number of accepted/total events = " << fNgood << "/" << fNtot << endl;
    cout << "==> HFRunLsEvtFilter> Number of events successfully retrieved in list: " << runLsEvtContainer_.getFoundSize()
	 << " out of " << runLsEvtContainer_.getFullSize() << " entries in list." << endl;
}


// ----------------------------------------------------------------------
bool HFRunLsEvtFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    fNtot++;
    // cout << iEvent.id().run() << " " << iEvent.id().luminosityBlock() << " " << iEvent.id().event() << endl;
    const bool ret = runLsEvtContainer_.find(iEvent.id().run(), iEvent.id().luminosityBlock(), iEvent.id().event());
    if (ret) fNgood++;
    //if (ret) cout << iEvent.id().run() << " " << iEvent.id().luminosityBlock() << " " << iEvent.id().event() << endl;
    return ret;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFRunLsEvtFilter);
