#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagInfo.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoTauTagInfo::ZETauRecoTauTagInfo(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_PFTauTag = iConfig.getParameter<InputTag>("PFTauTag");
  
  produces<IsolatedTauTagInfoCollection>("LooseTauTag");
  produces<IsolatedTauTagInfoCollection>("MedTauTag");
  produces<IsolatedTauTagInfoCollection>("TightTauTag");

  produces<PFIsolatedTauTagInfoCollection>("LoosePFTauTag");
  produces<PFIsolatedTauTagInfoCollection>("MedPFTauTag");
  produces<PFIsolatedTauTagInfoCollection>("TightPFTauTag");

  produces<LorentzVectorCollection>("LooseTauTag");
  produces<LorentzVectorCollection>("MedTauTag");
  produces<LorentzVectorCollection>("TightTauTag");

  produces<LorentzVectorCollection>("LoosePFTauTag");
  produces<LorentzVectorCollection>("MedPFTauTag");
  produces<LorentzVectorCollection>("TightPFTauTag");
}

ZETauRecoTauTagInfo::~ZETauRecoTauTagInfo(){ }

void ZETauRecoTauTagInfo::produce(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<IsolatedTauTagInfoCollection> product_LooseTauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<IsolatedTauTagInfoCollection> product_MedTauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<IsolatedTauTagInfoCollection> product_TightTauTag(new IsolatedTauTagInfoCollection);

  auto_ptr<PFIsolatedTauTagInfoCollection> product_LoosePFTauTag(new PFIsolatedTauTagInfoCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_MedPFTauTag(new PFIsolatedTauTagInfoCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_TightPFTauTag(new PFIsolatedTauTagInfoCollection);

  auto_ptr<LorentzVectorCollection> product_LooseTauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_MedTauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_TightTauTagVec(new LorentzVectorCollection);
  
  auto_ptr<LorentzVectorCollection> product_LoosePFTauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_MedPFTauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_TightPFTauTagVec(new LorentzVectorCollection);

  Handle<IsolatedTauTagInfoCollection> tauTagHandle;
  iEvent.getByLabel( m_TauTag , tauTagHandle );  
  IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();


  
  for(;tautagit!=tauTagHandle->end();++tautagit)
    {
      LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),
			tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
      
      
      if((*tautagit).discriminator(0.1,0.07,0.45,15.,1.,0,0.2)){
	  product_TightTauTag->push_back((*tautagit));
	  product_TightTauTagVec->push_back(tau);
      }
      
      if((*tautagit).discriminator(0.1,0.07,0.45,6.,1.,0,0.2)){//0.1,0.05,0.45,12.,1.,0,0.2)){//0.07,0.45,6.,1.,0,0.2)){
	product_MedTauTag->push_back((*tautagit));
	product_MedTauTagVec->push_back(tau);
      }
      
      if((*tautagit).discriminator(0.1,0.14,0.45,5.,1.,0,0.2)){
	product_LooseTauTag->push_back((*tautagit));
	product_LooseTauTagVec->push_back(tau);
      }
      
    }
  
  try{
    Handle<PFIsolatedTauTagInfoCollection> pfTauTagHandle;
    iEvent.getByLabel( m_PFTauTag , pfTauTagHandle );
    PFIsolatedTauTagInfoCollection::const_iterator pftautagit=pfTauTagHandle->begin();

    for(;pftautagit!=pfTauTagHandle->end();++pftautagit)
      {
	LorentzVector pftau(pftautagit->pfjetRef()->px(),pftautagit->pfjetRef()->py(),
			    pftautagit->pfjetRef()->pz(),pftautagit->pfjetRef()->energy());
	
	if((*pftautagit).discriminatorByIsolPFChargedHadrCandsN(0.1,0.07,0.45,true,15.,1.,0)){
	  product_TightPFTauTag->push_back((*pftautagit));
	  product_TightPFTauTagVec->push_back(pftau);
	}
	if((*pftautagit).discriminatorByIsolPFChargedHadrCandsN(0.1,0.07,0.45,true,6.,1.,0)){
	  product_MedPFTauTag->push_back((*pftautagit));
	  product_MedPFTauTagVec->push_back(pftau);
	}
	if((*pftautagit).discriminatorByIsolPFChargedHadrCandsN(0.1,0.14,0.45,true,5.,1.,0)){
	  product_LoosePFTauTag->push_back((*pftautagit));
	  product_LoosePFTauTagVec->push_back(pftau);
	}
	
      }
  }catch(...){;}

  iEvent.put(product_LooseTauTag,"LooseTauTag");
  iEvent.put(product_MedTauTag,"MedTauTag");
  iEvent.put(product_TightTauTag,"TightTauTag");

  iEvent.put(product_LoosePFTauTag,"LoosePFTauTag");
  iEvent.put(product_MedPFTauTag,"MedPFTauTag");
  iEvent.put(product_TightPFTauTag,"TightPFTauTag");
  
  iEvent.put(product_LooseTauTagVec,"LooseTauTag");
  iEvent.put(product_MedTauTagVec,"MedTauTag");
  iEvent.put(product_TightTauTagVec,"TightTauTag");

  iEvent.put(product_LoosePFTauTagVec,"LoosePFTauTag");
  iEvent.put(product_MedPFTauTagVec,"MedPFTauTag");
  iEvent.put(product_TightPFTauTagVec,"TightPFTauTag");
  
}

