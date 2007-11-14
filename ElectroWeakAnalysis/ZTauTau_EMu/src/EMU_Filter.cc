#include "CSA07anl/Analyzer/interface/EMU_Filter.h"

EMU_Filter::EMU_Filter(const edm::ParameterSet& iConfig) :
        verbosity_(iConfig.getUntrackedParameter<bool>("Verbosity", true)),
  	muons_(iConfig.getUntrackedParameter("muonLabel", string("muons"))),
	elecs_(iConfig.getUntrackedParameter("electronLabel", string("PixelMatchGsfElectron")))  


{

  nev_ = 0;

}

EMU_Filter::~EMU_Filter()
{

  if (verbosity_) {

    cout << "=== Selected events ===" << endl;
    cout << "total   : " << nev_ << endl;
  }

}

bool EMU_Filter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
bool findmuon = false;
bool findelectron = false;

Handle<MuonCollection> mus;
iEvent.getByLabel(muons_, mus);

Handle<PixelMatchGsfElectronCollection> electrons;
iEvent.getByLabel(elecs_, electrons);

for (reco::MuonCollection::const_iterator mu = mus->begin(); mu != mus->end(); ++mu) {
if(mu->pt()>5.) findmuon = true;
}

for(PixelMatchGsfElectronCollection::const_iterator elc = electrons->begin(); elc != electrons->end(); ++elc){

if(elc->pt()>5.) findelectron = true;
}
if(findmuon&&findelectron) {
nev_++;
return true;
}
return false; 

}
