#include "ElectroWeakAnalysis/EWKTau/interface/DiscriminationByMassWithElec.h"


using namespace reco; 
using namespace edm;
using namespace std;

//
// constructors and destructor
//
DiscriminationByMassWithElec::DiscriminationByMassWithElec(const edm::ParameterSet& iConfig)
{
  pfTauTag_ = iConfig.getParameter<InputTag>("PfTauTag");
  elecTag_ = iConfig.getParameter<InputTag>("ElecTag");
  mmin_ = iConfig.getParameter<double>("MinMassCut");
  mmax_ = iConfig.getParameter<double>("MaxMassCut");
  doSel_ = iConfig.getParameter<bool>("DoSelection");
  produces<PFTauDiscriminatorByIsolation>();
}


DiscriminationByMassWithElec::~DiscriminationByMassWithElec()
{
}


// ------------ method called on each new Event  ------------
void DiscriminationByMassWithElec::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  Handle<PFTauCollection> pfTaus;
  iEvent.getByLabel(pfTauTag_,pfTaus);
  PFTauCollection::const_iterator tau = pfTaus->begin();
  
  Handle<std::vector<pat::Electron> > elecs;
  iEvent.getByLabel(elecTag_,elecs);
  

  auto_ptr<PFTauDiscriminatorByIsolation> pfTauMassWithElecDiscr(new PFTauDiscriminatorByIsolation(PFTauRefProd(pfTaus)));
  
  LorentzVector eLV,tLV;
  
  double mass=0.;int iPFTau=0;
  for(;tau!=pfTaus->end();++tau)
    {
      tLV.SetPxPyPzE(tau->px(),tau->py(),tau->pz(),tau->energy());
      if(elecs->size()==1){
	pat::Electron elec = elecs->at(0);
	eLV.SetPxPyPzE(elec.px(),elec.py(),elec.pz(),elec.energy());
	mass = (eLV+tLV).M();
	if(doSel_){
	  if(mass>10.&& (mass<mmin_||mass>mmax_)){
	    pfTauMassWithElecDiscr->setValue(iPFTau,1);
	  }
	}
	else pfTauMassWithElecDiscr->setValue(iPFTau,1);
      }
      iPFTau++;
    }
      
  
  iEvent.put(pfTauMassWithElecDiscr);
  

}

