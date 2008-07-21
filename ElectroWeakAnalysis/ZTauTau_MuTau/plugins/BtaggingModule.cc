//C++ includes
#include <vector>
#include <functional>

//ROOT includes
#include <Math/VectorUtil.h>

//CMSSW includes
#include "UserAnalysisCode/ZTauTauAnalysis/plugins/MuTkIsolation.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryCleaning/interface/TrajectoryCleanerBySharedHits.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "TrackingTools/PatternTools/interface/TrajectoryFitter.h"
#include "RecoTracker/CkfPattern/interface/TransientInitialStateEstimator.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <algorithm>
#include <Math/VectorUtil.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <cmath>
#include "TStyle.h"
#include "Math/Vector4D.h"
#include "HepMC/GenEvent.h"
#include "Math/GenVector/VectorUtil.h"
#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <set>
#include "TTree.h"
#include "TH1F.h"
#include "TFile.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <set>
#include <TROOT.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <cmath>
#include "Math/GenVector/LorentzVector.h"

// b tagging 
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "UserAnalysisCode/ZTauTauAnalysis/plugins/BtaggingModule.h"



using namespace std ;
using namespace ROOT::Math::VectorUtil ;



BtaggingModule::BtaggingModule (const edm::Event& iEvent):
  iEvent_(iEvent)
  				    
{  
}

BtaggingModule::~BtaggingModule ()
{
}


 

/////////////////////////////      SOFT MUONS
float BtaggingModule::getDiscriminator_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ , bool _flagRecoTau) 
{  
 
  float BtagDisc = 0 ;    
  float drP4TB=-999;
  float drP3TB=-999;
  
  float drP4MB=-999;
  float drP3MB=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TB = DeltaR(BJetP4,TauP4);
      drP3TB=DeltaR(BJetP3,TauP3);
      drP4MB=DeltaR(MuP4,BJetP4);
      drP3MB=DeltaR(MuP3,BJetP3);
      
      if(_flagRecoTau==true){
//	cout<<" Jet "<<i<<": tau "<<TauP4<<" "<< "muon "<<MuP4<<" "<<" jet "<<BJetP4<<" dr T B "<<drP4TB<<" "<<" dr M B "<<drP4MB<<" "<<drP3MB<<endl;
//	cout<<" Jet "<<i<<" has SM b tag discriminator = "<<bTags[i].discriminator()<< " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
      }
      if((bTags[i].discriminator()>0)&&(BJet->pt()>15))
	{
//	  cout<<"for this jet "<<bTags[i].discriminator()<<" so found a SM b-jet"<<endl;
	  BtagDisc++;
	}
      
    }
  return BtagDisc ;
}



float BtaggingModule::getDiscSum_SoftMuons(const edm::Event& iEvent, const PFTau& Tau )
{ 
  float BDiscSum=0;
  //float BDiscMean=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("softMuonJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
if((bTags[i].discriminator()>0)&&(BJet->pt()>15)){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
//cout<<"Disc Sum SM"<<BDiscSum<<endl;
// BDiscMean=BDiscSum/BTagged;
 cout<<"Disc Sum SM "<<BDiscSum<<endl;
  return BDiscSum ;
}



float BtaggingModule::getDiscriminator_SoftMuonsNoIP (const edm::Event& iEvent , const PFTau& Tau ) 
{  
 
  float BtagDisc = 0 ;    

  // Get b tag information
edm::Handle<reco::JetTagCollection> bTagHandle;
//iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
iEvent.getByLabel("softMuonNoIPJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());

//Loop over jets and study b tag info.

for (size_t i = 0; i != bTags.size(); ++i) {
 // cout<<" Jet "<< i 
//        <<" has SM NoIp b tag discriminator = "<<bTags[i].discriminator()
//        << " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
 if(bTags[i].discriminator()>0){
 //  cout<<"Found a SMNoIP b-jet"<<endl;
   BtagDisc++;
}
                
}
//cout<<"Found "<<BtagDisc<<" SMNoIP b-jets"<<endl;
  return BtagDisc ;
}


float BtaggingModule::getDiscSum_SoftMuonsNoIP(const edm::Event& iEvent, const PFTau& Tau )
{ 
  float BDiscSum=0;
  //float BDiscMean=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("softMuonNoIPJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
if(bTags[i].discriminator()>0){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
// BDiscMean=BDiscSum/BTagged;
// cout<<"Disc Sum SMNoIP "<<BDiscSum<<endl;
  return BDiscSum ;
}



float BtaggingModule::getDiscriminator_JetProbability (const edm::Event& iEvent , const PFTau& Tau ) 
{  
 
  float BtagDisc = 0 ;    

  // Get b tag information
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("jetProbabilityJetTags", bTagHandle);
//iEvent.getByLabel("softMuonJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());

//Loop over jets and study b tag info.

for (size_t i = 0; i != bTags.size(); ++i) {
 //  cout<<" Jet "<< i 
//      <<" has b tag discriminator = "<<bTags[i].discriminator()
//        << " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
 if(bTags[i].discriminator()>0){
 //  cout<<"Found a JP b-jet"<<endl;
   BtagDisc++;
}
                
}
//cout<<"Found "<<BtagDisc<<" JP b-jets"<<endl;
  return BtagDisc ;
}


float BtaggingModule::getDiscSum_JetProbability (const edm::Event& iEvent, const PFTau& Tau)
{ 


float BDiscSum=0;
//float BDiscMean=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("jetProbabilityJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
if(bTags[i].discriminator()>0){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
//cout<<"Disc Sum J"<<BDiscSum<<endl;
// BDiscMean=BDiscSum/BTagged;
// cout<<"Disc Sum JP "<<BDiscSum<<endl;
  return BDiscSum ;


}


float BtaggingModule::getDiscriminator_TrackCountingHE (const edm::Event& iEvent, const PFTau& Tau  ) 
{  
 
  float BtagDisc = 0 ;    

  // Get b tag information
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
//iEvent.getByLabel("softMuonJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());

//Loop over jets and study b tag info.

for (size_t i = 0; i != bTags.size(); ++i) {
  // cout<<" Jet "<< i 
//      <<" has TC b tag discriminator = "<<bTags[i].discriminator()
//        << " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
 if(bTags[i].discriminator()>0){
 //  cout<<"Found a TCHE b-jet"<<endl;
   BtagDisc++;
}
                
}
cout<<"Found "<<BtagDisc<<" TCHE b-jets"<<endl;
  return BtagDisc ;
}

float BtaggingModule::getDiscSum_TrackCountingHE(const edm::Event& iEvent, const PFTau& Tau )
{ 
  float BDiscSum=0;
  //float BDiscMean=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
if(bTags[i].discriminator()>0){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
//BDiscMean=BDiscSum/BTagged;
// cout<<"Disc Sum TCHE "<<BDiscSum<<endl;
  return BDiscSum ;

}


float BtaggingModule::getDiscriminator_TrackCountingHP (const edm::Event& iEvent , const PFTau& Tau) 
{  
 
  float BtagDisc = 0 ;    

  // Get b tag information
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("trackCountingHighPurJetTags", bTagHandle);
//iEvent.getByLabel("softMuonJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());

//Loop over jets and study b tag info.

for (size_t i = 0; i != bTags.size(); ++i) {
  //  cout<<" Jet "<< i 
 //      <<" has TCHP b tag discriminator = "<<bTags[i].discriminator()
 //        << " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
 if(bTags[i].discriminator()>0){
 //  cout<<"Found a SL b-jet"<<endl;
   BtagDisc++;
}
                
}

// cout<<"Found "<<BtagDisc<<" TCHP b-jets"<<endl;
 return BtagDisc ;
}



float BtaggingModule::getDiscSum_TrackCountingHP (const edm::Event& iEvent, const PFTau& Tau)
{ 
  float BDiscSum=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("trackCountingHighPurJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
if(bTags[i].discriminator()>0){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
// cout<<"Disc Sum TCHP "<<BDiscSum<<endl;
  return BDiscSum ;
}





float BtaggingModule::getDrMinTB_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  // float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;
  float drP4TB=999;
  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
      
      if((bTags[i].discriminator()>0)&&(BJet->pt()>15))
      {
	  if(drP4TBtmp<drP4TB)
	    {
	      drP4TB=drP4TBtmp  ;	    
	    }
	  }
    }
  return drP4TB ;
}


float BtaggingModule::getDrMaxMB_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
     
  float drP4TBtmp=999;
  float drP3TBtmp=999;

  float drP4MB=-999;  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
 
      if((bTags[i].discriminator()>0)&&(BJet->pt()>15))
      {
	  if(drP4MBtmp>drP4MB)
	    {
	      drP4MB=drP4MBtmp  ;
	    }
	  }
    }
  return drP4MB ;
}



float BtaggingModule::getDrMinTB_SoftMuonsNoIP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  //float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;
  float drP4TB=999;
  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonNoIPJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
      
      //if(bTags[i].discriminator()>0)
      //{
	  if(drP4TBtmp<drP4TB)
	    {
	      drP4TB=drP4TBtmp  ;	    
	    }
	  //}
    }
  return drP4TB ;
}


float BtaggingModule::getDrMinMB_SoftMuonsNoIP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  //float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;

  float drP4MB=999;  
  float drP4MBtmp=999;
  float drP3MBtmp=999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonNoIPJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
 
      //if(bTags[i].discriminator()>0)
      //{
	  if(drP4MBtmp<drP4MB)
	    {
	      drP4MB=drP4MBtmp  ;
	    }
	  //}
    }
  return drP4MB ;
}



float BtaggingModule::getDrMinTB_JetProbability (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  //float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;
  float drP4TB=999;
  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("jetProbabilityJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
      
      //if(bTags[i].discriminator()>0)
      //	{
	  if(drP4TBtmp<drP4TB)
	    {
	      drP4TB=drP4TBtmp  ;	    
	    }
	  //}
    }
  return drP4TB ;
}


float BtaggingModule::getDrMinMB_JetProbability(const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  // float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;

  float drP4MB=999;  
  float drP4MBtmp=999;
  float drP3MBtmp=999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("jetProbabilityJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
 
      //if(bTags[i].discriminator()>0)
      //{
	  if(drP4MBtmp<drP4MB)
	    {
	      drP4MB=drP4MBtmp  ;
	    }
	  //}
    }
  return drP4MB ;
}




float BtaggingModule::getDrMinTB_TrackCountingHE (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  // float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;
  float drP4TB=999;
  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
      
      //if(bTags[i].discriminator()>0)
//	{
	  if(drP4TBtmp<drP4TB)
	    {
	      drP4TB=drP4TBtmp  ;	    
	    }
//	}
    }
  return drP4TB ;
}


float BtaggingModule::getDrMinMB_TrackCountingHE (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  //float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;

  float drP4MB=999;  
  float drP4MBtmp=999;
  float drP3MBtmp=999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
 
     // if(bTags[i].discriminator()>0)
//	{
	  if(drP4MBtmp<drP4MB)
	    {
	      drP4MB=drP4MBtmp  ;
	    }
//	}
    }
  return drP4MB ;
}



float BtaggingModule::getDrMinTB_TrackCountingHP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  // float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;
  float drP4TB=999;
  
  float drP4MBtmp=-999;
  float drP3MBtmp=-999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighPurJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
      
     // if(bTags[i].discriminator()>0)
//	{
	  if(drP4TBtmp<drP4TB)
	    {
	      drP4TB=drP4TBtmp  ;	    
	    }
//	}
    }
  return drP4TB ;
}


float BtaggingModule::getDrMinMB_TrackCountingHP (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
{  
 
  //float BtagDisc = 0 ;    
  float drP4TBtmp=999;
  float drP3TBtmp=999;

  float drP4MB=999;  
  float drP4MBtmp=999;
  float drP3MBtmp=999;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighPurJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
  //Loop over jets and study b tag info.
  for (size_t i = 0; i != bTags.size(); ++i) 
    {
      math::XYZTLorentzVector  TauP4=Tau.p4();
      math::XYZVector TauP3 = Tau.momentum();
      
      math::XYZTLorentzVector  MuP4=Muon_.p4();
      math::XYZVector MuP3 = Muon_.momentum();
      
      
      edm::RefToBase<reco::Jet>  BRefJet=bTags[i].jet();
      Jet const*  BJet=BRefJet.get();
      math::XYZTLorentzVector BJetP4=BJet->p4();
      math::XYZVector BJetP3 = BJet->momentum();
      
      drP4TBtmp = DeltaR(BJetP4,TauP4);
      drP3TBtmp=DeltaR(BJetP3,TauP3);
      drP4MBtmp=DeltaR(MuP4,BJetP4);
      drP3MBtmp=DeltaR(MuP3,BJetP3);
 
     // if(bTags[i].discriminator()>0)
//	{
	  if(drP4MBtmp<drP4MB)
	    {
	      drP4MB=drP4MBtmp  ;
	    }
//	}
    }
  return drP4MB ;
}





int BtaggingModule::getNJetsFromTagInfo_SoftMuons(const edm::Event& iEvent) 
{
  int n=0;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  n=bTags.size();
  return n;
}


int BtaggingModule::getNJetsFromTagInfo_SoftMuonsNoIP(const edm::Event& iEvent) 
{
  int n=0;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("softMuonNoIPJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  n=bTags.size();
  return n;
}


int BtaggingModule::getNJetsFromTagInfo_JetProbability(const edm::Event& iEvent) 
{
  int n=0;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("jetProbabilityJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  n=bTags.size();
  return n;
}

int BtaggingModule::getNJetsFromTagInfo_TrackCountingHE(const edm::Event& iEvent) 
{
  int n=0;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighEffJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  n=bTags.size();
  return n;
}


int BtaggingModule::getNJetsFromTagInfo_TrackCountingHP(const edm::Event& iEvent) 
{
  int n=0;
  edm::Handle<reco::JetTagCollection> bTagHandle;
  iEvent.getByLabel("trackCountingHighPurJetTags", bTagHandle);
  const reco::JetTagCollection & bTags = *(bTagHandle.product());
  n=bTags.size();
  return n;
}








///////////////////////////   SOFT LEPTON
float BtaggingModule::getDiscriminator_SoftLepton (const edm::Event& iEvent , const PFTau& Tau) 
{  
 
  float BtagDisc = 0 ;    

  // Get b tag information
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("softLeptonByPtJetTags", bTagHandle);
//iEvent.getByLabel("softMuonJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());

//Loop over jets and study b tag info.

for (size_t i = 0; i != bTags.size(); ++i) {
 //  cout<<" Jet "<< i 
//      <<" has SL b tag discriminator = "<<bTags[i].discriminator()
//        << " and jet Pt = "<<bTags[i].jet()->pt()<<endl;
 if(bTags[i].discriminator()>0){
if(bTags[i].discriminator()>0){
   cout<<"Found a SL b-jet"<<endl;
   BtagDisc++;
}
}                
}

 cout<<"Found "<<BtagDisc<<" SL b-jets"<<endl;
 return BtagDisc ;
}


float BtaggingModule::getDiscMean_SoftLepton (const edm::Event& iEvent, const PFTau& Tau)
{ 
  float BDiscSum=0;
float BDiscMean=0;
  float BTagged=0;
edm::Handle<reco::JetTagCollection> bTagHandle;
iEvent.getByLabel("softLeptonByPtJetTags", bTagHandle);
const reco::JetTagCollection & bTags = *(bTagHandle.product());
for (size_t i = 0; i != bTags.size(); ++i) {
if(bTags[i].discriminator()>0){
  BDiscSum+=bTags[i].discriminator();
  BTagged++;
}
}
BDiscMean=BDiscSum/BTagged;
 cout<<"Disc Sum SL "<<BDiscSum<<" and BDiscMean "<<BDiscMean<<endl;
  return BDiscMean ;
}
















// edm::RefToBase<Jet> BtaggingModule::getRefJet_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 

// {  
//   edm::RefToBase<reco::Jet>  BRefJet; 
//   edm::Handle<reco::JetTagCollection> bTagHandle;
//   iEvent.getByLabel("softMuonJetTags", bTagHandle);
//   const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
//   //Loop over jets and study b tag info.
//   for (size_t i = 0; i != bTags.size(); ++i) 
//     {     
//        BRefJet=bTags[i].jet();
//      const  Jet*  BJet=((BRefJet.get()));

//  const PFTauTagInfo& tagInfo     = (*(Tau.pfTauTagInfoRef()));
//      const PFJet& pfJet              = (*(tagInfo.pfjetRef()));

//      //cout<<" tau jet "<<pfJet<<" b jet "<<BJet<<endl;     
//       // Get b tag information
//       if(bTags[i].discriminator()>0)
// 	{
// 	  //if((*BJet)==pfJet ) cout<<" UGUALI"<<endl;
// 	}
//     }
//   return BRefJet ;
// }



// TrackRefVector getTrackRefVector_SoftMuons (const edm::Event& iEvent, const PFTau& Tau, const Muon& Muon_ ) 
// {
//   TrackRefVector BTracks;
// edm::Handle<reco::JetTagCollection> bTagHandle;
//   iEvent.getByLabel("softMuonJetTags", bTagHandle);
//   const reco::JetTagCollection & bTags = *(bTagHandle.product());
  
//   //Loop over jets and study b tag info.
//   for (size_t i = 0; i != bTags.size(); ++i) 
//     {  

//       BTracks=bTags[i].tracks();
//       cout<<"Number of tracks in the b jet "<<BTracks.size()<<endl;  

// }
//   return BTracks;
// }


