#include "TauAnalysis/RecoTools/plugins/METRecalculator.h"
#include "FWCore/Framework/interface/MakerMacros.h"

METRecalculator::~METRecalculator() 
{}

METRecalculator::METRecalculator(const edm::ParameterSet& iConfig):
  met_(iConfig.getParameter<edm::InputTag>("met")),  
  originalObjects_(iConfig.getParameter<std::vector<edm::InputTag> >("originalObjects")),  
  smearedObjects_(iConfig.getParameter<std::vector<edm::InputTag> >("smearedObjects"))  
{
      produces<pat::METCollection >();
}

void 
METRecalculator::beginJob(const edm::EventSetup& es)
{}


void 
METRecalculator::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  //get the collection
  using namespace edm;
  using namespace reco;

  //Sum the original objects
  math::XYZTLorentzVector originalVector;
  for(unsigned int i=0;i<originalObjects_.size();++i)   {
      edm::Handle<edm::View<reco::Candidate> > objects;
      if(iEvent.getByLabel(originalObjects_[i],objects)) 
	for(unsigned int o = 0 ;o != objects->size();++o)
	  originalVector+=objects->at(o).p4();
    }

  //Sum the final objects
  math::XYZTLorentzVector finalVector;
  for(unsigned int i=0;i<smearedObjects_.size();++i)   {
      edm::Handle<edm::View<reco::Candidate> > objects;
      if(iEvent.getByLabel(smearedObjects_[i],objects)) 
	for(unsigned int o = 0 ;o != objects->size();++o)
	  finalVector+=objects->at(o).p4();
    }

  //Make the difference
  math::XYZTLorentzVector difference = finalVector - originalVector;
  //Take the tarnsverse part
  math::XYZTLorentzVector metCorrection(difference.px(),difference.py(),0.0,sqrt(difference.px()*difference.px()+difference.py()*difference.py()));

  std::auto_ptr<pat::METCollection > out(new pat::METCollection);
  Handle<pat::METCollection> srcH;
  
  if(iEvent.getByLabel(met_,srcH)) 
    for(unsigned int i=0;i<srcH->size();++i) {
      pat::MET  met = srcH->at(i);
      met.setP4(met.p4()+metCorrection);
      out->push_back(met);
    }
  iEvent.put(out);
}


void
METRecalculator::endJob()
{}

DEFINE_ANOTHER_FWK_MODULE(METRecalculator);
