#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoMassInfo.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoMassInfo::ZETauRecoMassInfo(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
 
  
  m_LeptonTag = iConfig.getParameter<InputTag>("LeptonTag");
  m_CaloMETTag = iConfig.getParameter<InputTag>("CaloMETTag");
  m_CaloJetTag = iConfig.getParameter<InputTag>("CaloJetTag");

  produces< vector<float> >("ZVisMass");
  produces< vector<float> >("ZMassMET");
  produces< vector<float> >("ZMassColl");

  produces<LorentzVectorCollection>("TauTag");
  produces<LorentzVectorCollection>("LeptonTag");
    

 
}

ZETauRecoMassInfo::~ZETauRecoMassInfo(){ }

void ZETauRecoMassInfo::produce(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr< vector<float> > product_ZMassVis(new vector<float>);
  auto_ptr< vector<float> > product_ZMassMET(new vector<float>);
  auto_ptr< vector<float> > product_ZMassColl(new vector<float>);
  
  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_LeptonTagVec(new LorentzVectorCollection);
  
  
  LorentzVector tau,lepton,met,jet;

  Handle<LorentzVectorCollection> tauVecHandle;
  iEvent.getByLabel(m_TauTag,tauVecHandle);
  LorentzVectorCollection::const_iterator tauit=tauVecHandle->begin();
  HighestEtObject(tauVecHandle,tau);

  Handle<LorentzVectorCollection> leptonVecHandle;
  iEvent.getByLabel(m_LeptonTag,leptonVecHandle);
  LorentzVectorCollection::const_iterator lepit=leptonVecHandle->begin();
  HighestEtObject(leptonVecHandle,lepton);

  product_TauTagVec->push_back(tau);
  product_LeptonTagVec->push_back(lepton);


  Handle<CandidateCollection> caloMetHandle;
  iEvent.getByLabel(m_CaloMETTag,caloMetHandle);
  CandidateCollection::const_iterator metit=caloMetHandle->begin();
  met.SetXYZT(metit->px(),metit->py(),		    
	      metit->pz(),metit->energy());
  
  cout<<met.X()<<endl;
  Handle<CandidateCollection>caloJetHandle;
  iEvent.getByLabel(m_CaloJetTag,caloJetHandle);
  CandidateCollection::const_iterator jetit=caloJetHandle->begin();

  LorentzVectorCollection objVecForMass;vector<float> massVec;
  if(true)//DeltaPhi(tau,lepton)<175*acos(-1.)/180.)
    {
      objVecForMass.push_back(tau);
      objVecForMass.push_back(lepton);
      objVecForMass.push_back(met);
      CalculateMass(objVecForMass,massVec);
  
      product_ZMassVis->push_back(massVec[0]);
      product_ZMassMET->push_back(massVec[1]);
      product_ZMassColl->push_back(massVec[2]);
    
      iEvent.put(product_LeptonTagVec,"LeptonTag");
      iEvent.put(product_TauTagVec,"TauTag");
      
      iEvent.put(product_ZMassVis,"ZVisMass");
      iEvent.put(product_ZMassMET,"ZMassMET");
      iEvent.put(product_ZMassColl,"ZMassColl");
      
    }
  
}

void ZETauRecoMassInfo::HighestEtObject(Handle<LorentzVectorCollection>& objVecHandle,LorentzVector& obj)
{
  LorentzVectorCollection::const_iterator objit=objVecHandle->begin();
  double tempEt=0;
  for(;objit!=objVecHandle->end();++objit)
    {
      LorentzVector tempObj(objit->px(),objit->py(),
			    objit->pz(),objit->energy());
      if(tempObj.Et()>tempEt)
	{
	  obj=tempObj;
	  tempEt=tempObj.Et();
	}
    }
}

void ZETauRecoMassInfo::CalculateMass(LorentzVectorCollection& objVec,vector<float>& massVec)
{
  LorentzVectorCollection::const_iterator objit = objVec.begin();
  LorentzVector zMassVis,zMassMET,zMassColl;
  int count=0;
  for(;objit!=objVec.end();++objit)
    {
      if(count<2)zMassVis+=(*objit);
      zMassMET+=(*objit);
      count++;
    }

  massVec.push_back(zMassVis.M());
  massVec.push_back(zMassMET.M());
  massVec.push_back(0);//for now
}

double ZETauRecoMassInfo::DeltaPhi(LorentzVector& a, LorentzVector& b)
{
  double dphi = fabs(a.Phi()-b.Phi());
  if(dphi>acos(-1.))dphi=2*acos(-1.)-dphi;
  return dphi;
}

/*



  Handle<LorentzVectorCollection> MCinfo;
  Handle<LorentzVectorCollection> MCinfo1;
  LorentzVector mcTauJet,mcLeptonFromTau,mcNeutrina;
  int j=0;
  vector<InputTag> m_mcTauProducts;
  
  
  
  iEvent.getByLabel("Z2ETauMcInfoProducer","Leptons",MCinfo);
  iEvent.getByLabel("Z2ETauMcInfoProducer","Jets",MCinfo1);
  LorentzVectorCollection::const_iterator imc=MCinfo->begin();
  for(;imc!=MCinfo->end();++imc)
    {
      mcLeptonFromTau.SetXYZT((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
      
    }
  imc=MCinfo1->begin();
  for(;imc!=MCinfo1->end();++imc)
    {
      mcTauJet.SetXYZT((*imc).Px(),(*imc).Py(),(*imc).Pz(),(*imc).E());
      
    }






double dphi=DeltaPhi(mcLeptonFromTau,lepton);
      double dR=sqrt(dphi*dphi+(lepton.Eta()-mcLeptonFromTau.Eta())*(lepton.Eta()-mcLeptonFromTau.Eta()));
      double dphi1=DeltaPhi(mcTauJet,tau);
      double dR1=sqrt(dphi1*dphi1+(tau.Eta()-mcTauJet.Eta())*(tau.Eta()-mcTauJet.Eta()));
      if(dR<0.5&&dR1<0.5)
*/
