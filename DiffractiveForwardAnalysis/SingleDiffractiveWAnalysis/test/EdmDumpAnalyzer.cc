#ifndef EdmDumpAnalyzer_h
#define EdmDumpAnalyzer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TH1F;
class TH2F;

class EdmDumpAnalyzer: public edm::EDAnalyzer{
public:
  explicit EdmDumpAnalyzer(const edm::ParameterSet&);

  ~EdmDumpAnalyzer();

  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  void fillMultiplicities(const edm::Event&, const edm::EventSetup&);
  void fillFromTowers(const edm::Event&, const edm::EventSetup&);

  edm::InputTag castorGenInfoTag_;
  edm::InputTag castorTowerInfoTag_;
  edm::InputTag castorTowersTag_;

  //edm::InputTag vertexTag_;
  
  int gapSide_;
  unsigned int thresholdIndexHF_;
  unsigned int thresholdIndexCastor_;

  bool accessCastorTowers_;
  double thresholdTower_;

  bool accessPileUpInfo_;

  TH1F* h_nCastorTowerPlusDirect_;
  TH1F* h_nCastorTowerMinusDirect_;
  TH2F* h_nCastorGenvsCastorTowerPlusDirect_;
  TH2F* h_nCastorGenvsCastorTowerMinusDirect_;
  TH1F* h_CastorTowerEnergyPlus_;
  TH1F* h_CastorTowerEnergyMinus_;

  TH1F* h_nHFTowerPlus_;
  TH1F* h_nHFTowerMinus_;

  TH1F* h_nCastorGenPlus_;
  TH1F* h_nCastorGenMinus_;
  TH1F* h_nCastorTowerPlus_;
  TH1F* h_nCastorTowerMinus_;
  TH2F* h_nCastorGenvsCastorTowerPlus_;
  TH2F* h_nCastorGenvsCastorTowerMinus_;

  TH2F* h_nHFTowerPlusvsnCastorGenPlus_;
  TH2F* h_nHFTowerMinusvsnCastorGenMinus_;
  TH2F* h_nHFTowerPlusvsnCastorTowerPlus_;
  TH2F* h_nHFTowerMinusvsnCastorTowerMinus_;

  TH1F* h_xiPlus_;
  TH1F* h_xiMinus_;

  TH1F* h_nPileUpBx0_;

};
#endif

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CastorReco/interface/CastorTower.h"
//#include "DataFormats/VertexReco/interface/VertexFwd.h"
//#include "DataFormats/VertexReco/interface/Vertex.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"

using namespace reco;

EdmDumpAnalyzer::EdmDumpAnalyzer(const edm::ParameterSet& conf){
  castorGenInfoTag_ = conf.getParameter<edm::InputTag>("CastorGenInfoTag");
  castorTowerInfoTag_ = conf.getParameter<edm::InputTag>("CastorTowerInfoTag");
  //vertexTag_ = conf.getParameter<edm::InputTag>("VertexTag");  
 
  gapSide_ = conf.getParameter<int>("GapSide");
  thresholdIndexHF_ = conf.getParameter<unsigned int>("ThresholdIndexHF");
  thresholdIndexCastor_ = conf.getParameter<unsigned int>("ThresholdIndexCastor");
 
  accessCastorTowers_ = conf.getParameter<bool>("AccessCastorTowers");
  if(accessCastorTowers_) castorTowersTag_ = conf.getParameter<edm::InputTag>("CastorTowersTag");
  if(accessCastorTowers_) thresholdTower_ = conf.getParameter<double>("TowerThreshold");

  accessPileUpInfo_ = conf.getParameter<bool>("AccessPileUpInfo");
}  
  
EdmDumpAnalyzer::~EdmDumpAnalyzer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void EdmDumpAnalyzer::beginJob(edm::EventSetup const&iSetup){

  edm::Service<TFileService> fs;

  int nBins = 50;
  if(accessCastorTowers_){
    h_nCastorTowerPlusDirect_ = fs->make<TH1F>("nCastorTowerPlusDirect","Castor Towers mult. plus",nBins,0,nBins);
    h_nCastorTowerMinusDirect_ = fs->make<TH1F>("nCastorTowerMinusDirect","Castor Towers mult. minus",nBins,0,nBins);
    h_nCastorGenvsCastorTowerPlusDirect_ = fs->make<TH2F>("nCastorGenVsCastorTowerPlusDirect","Castor Gen vs Castor towers mult. plus",nBins,0,nBins,nBins,0,nBins);
    h_nCastorGenvsCastorTowerMinusDirect_ = fs->make<TH2F>("nCastorGenVsCastorTowerMinusDirect","Castor Gen vs Castor towers mult. minus",nBins,0,nBins,nBins,0,nBins);
    h_CastorTowerEnergyPlus_ = fs->make<TH1F>("CastorTowerEnergyPlus","Castor towers energy plus",200,0.,100.);
    h_CastorTowerEnergyMinus_ = fs->make<TH1F>("CastorTowerEnergyMinus","Castor towers energy minus",200,0.,100.);
  }

  if(accessPileUpInfo_){
    h_nPileUpBx0_ = fs->make<TH1F>("nPileUpBx0","Nr. of pile-up events in Bx0",10,0,10);
  }

  h_nHFTowerPlus_ = fs->make<TH1F>("nHFTowerPlus","HF tower mult. plus",nBins,0,nBins);
  h_nHFTowerMinus_ = fs->make<TH1F>("nHFTowerMinus","HF tower mult. minus",nBins,0,nBins);

  h_nCastorGenPlus_ = fs->make<TH1F>("nCastorGenPlus","Castor gen mult. plus",nBins,0,nBins);
  h_nCastorGenMinus_ = fs->make<TH1F>("nCastorGenMinus","Castor gen mult. minus",nBins,0,nBins);

  h_nCastorTowerPlus_ = fs->make<TH1F>("nCastorTowerPlus","Castor Towers mult. plus",nBins,0,nBins);
  h_nCastorTowerMinus_ = fs->make<TH1F>("nCastorTowerMinus","Castor Towers mult. minus",nBins,0,nBins);

  h_nCastorGenvsCastorTowerPlus_ = fs->make<TH2F>("nCastorGenVsCastorTowerPlus","Castor Gen vs Castor towers mult. plus",nBins,0,nBins,nBins,0,nBins);
  h_nCastorGenvsCastorTowerMinus_ = fs->make<TH2F>("nCastorGenVsCastorTowerMinus","Castor Gen vs Castor towers mult. minus",nBins,0,nBins,nBins,0,nBins);

  h_nHFTowerPlusvsnCastorGenPlus_ = fs->make<TH2F>("nHFTowerPlusvsnCastorGenPlus","HF plus vs Castor gen plus",nBins,0,nBins,nBins,0,nBins);
  h_nHFTowerMinusvsnCastorGenMinus_ = fs->make<TH2F>("nHFTowerMinusvsnCastorGenMinus","HF minus vs Castor gen minus",nBins,0,nBins,nBins,0,nBins);

  h_nHFTowerPlusvsnCastorTowerPlus_ = fs->make<TH2F>("nHFTowerPlusvsnCastorTowerPlus","HF plus vs Castor tower plus",nBins,0,nBins,nBins,0,nBins);
  h_nHFTowerMinusvsnCastorTowerMinus_ = fs->make<TH2F>("nHFTowerMinusvsnCastorTowerMinus","HF minus vs Castor tower minus",nBins,0,nBins,nBins,0,nBins);

  h_xiPlus_ = fs->make<TH1F>("xiPlus","Rec. xi from calo towers",200,0.,1.);
  h_xiMinus_ = fs->make<TH1F>("xiMinus","Rec. xi from calo towers",200,0.,1.);
}     

void EdmDumpAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
	fillMultiplicities(event,setup);
        if(accessCastorTowers_) fillFromTowers(event,setup);
        if(accessPileUpInfo_){
           edm::Handle<std::map<int,int> > pileUpMap;
           event.getByLabel("pileUpInfo",pileUpMap);
           std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
           if(bx0Iter == pileUpMap->end()){
              edm::LogError("Analysis") << ">>> Pile-up info does not contain bunch crossing 0 ..skipping";
              return;
           }
           int nPileUpBx0 = bx0Iter->second;
           edm::LogVerbatim("Analysis") << "  Number of pile-up events in bunch crossing 0: " << nPileUpBx0;
           h_nPileUpBx0_->Fill(nPileUpBx0);
        } 
}

void EdmDumpAnalyzer::fillMultiplicities(const edm::Event& event, const edm::EventSetup& setup)
{
	//Edm dumper output
        edm::Handle<std::vector<unsigned int> > nHFTowerPlus;
        event.getByLabel("hfTower","nHFplus",nHFTowerPlus);

        edm::Handle<std::vector<unsigned int> > nHFTowerMinus;
        event.getByLabel("hfTower","nHFminus",nHFTowerMinus); 

	edm::Handle<std::vector<unsigned int> > nCastorTowerPlus;
        event.getByLabel(castorTowerInfoTag_.label(),"nCastorTowerPlus",nCastorTowerPlus);

	edm::Handle<std::vector<unsigned int> > nCastorTowerMinus;
        event.getByLabel(castorTowerInfoTag_.label(),"nCastorTowerMinus",nCastorTowerMinus);

	edm::Handle<std::vector<unsigned int> > acceptedPhiSliceplus;
        event.getByLabel(castorGenInfoTag_.label(),"acceptedPhiSliceplus",acceptedPhiSliceplus);

        edm::Handle<double> xiTowerPlus;
        event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);

        edm::Handle<double> xiTowerMinus;
        event.getByLabel("xiTower","xiTowerminus",xiTowerMinus);

	unsigned int nAccPhiSliceplus = 0;
	for(unsigned int i = 0; i < (*acceptedPhiSliceplus.product()).size(); i++){
		if((*acceptedPhiSliceplus.product())[i] == 1) nAccPhiSliceplus++;
	}

	edm::Handle<std::vector<unsigned int> > acceptedPhiSliceminus;
        event.getByLabel(castorGenInfoTag_.label(),"acceptedPhiSliceminus",acceptedPhiSliceminus);

	unsigned int nAccPhiSliceminus = 0;
        for(unsigned int i = 0; i < (*acceptedPhiSliceminus.product()).size(); i++){
                if((*acceptedPhiSliceminus.product())[i] == 1) nAccPhiSliceminus++;
        }

        unsigned int nHF_plus = (*nHFTowerPlus.product())[thresholdIndexHF_];
        unsigned int nHF_minus = (*nHFTowerMinus.product())[thresholdIndexHF_];

        unsigned int nCastorTwr_plus = (*nCastorTowerPlus.product())[thresholdIndexCastor_];
        unsigned int nCastorTwr_minus = (*nCastorTowerMinus.product())[thresholdIndexCastor_];
        unsigned int nCastorGen_plus = nAccPhiSliceplus;
        unsigned int nCastorGen_minus = nAccPhiSliceminus;
       
        h_nHFTowerPlus_->Fill(nHF_plus);
        h_nHFTowerMinus_->Fill(nHF_minus);

        h_nCastorTowerPlus_->Fill(nCastorTwr_plus);
        h_nCastorTowerMinus_->Fill(nCastorTwr_minus);
	h_nCastorGenPlus_->Fill(nCastorGen_plus);
        h_nCastorGenMinus_->Fill(nCastorGen_minus);	
        h_nCastorGenvsCastorTowerPlus_->Fill(nCastorGen_plus,nCastorTwr_plus);
        h_nCastorGenvsCastorTowerMinus_->Fill(nCastorGen_minus,nCastorTwr_minus);

        h_nHFTowerPlusvsnCastorGenPlus_->Fill(nHF_plus,nCastorGen_plus);
        h_nHFTowerMinusvsnCastorGenMinus_->Fill(nHF_minus,nCastorGen_minus);
        h_nHFTowerPlusvsnCastorTowerPlus_->Fill(nHF_plus,nCastorTwr_plus);
        h_nHFTowerMinusvsnCastorTowerMinus_->Fill(nHF_minus,nCastorTwr_minus);

        h_xiPlus_->Fill(*xiTowerPlus.product());
        h_xiMinus_->Fill(*xiTowerMinus.product());
}

void EdmDumpAnalyzer::fillFromTowers(const edm::Event& event, const edm::EventSetup& setup)
{
        // Castor Towers
        edm::Handle<CastorTowerCollection> castorTowers;  
        event.getByLabel(castorTowersTag_,castorTowers);

        int nCastor_plus = 0;
        int nCastor_minus = 0;
        for(CastorTowerCollection::const_iterator castorTwr = castorTowers->begin(); castorTwr != castorTowers->end(); ++castorTwr){
          if(castorTwr->eta() >= 0.) h_CastorTowerEnergyPlus_->Fill(castorTwr->energy());
          else h_CastorTowerEnergyMinus_->Fill(castorTwr->energy());

          if(castorTwr->energy() >= thresholdTower_){
            if(castorTwr->eta() >= 0.) ++nCastor_plus;
            else ++nCastor_minus;
          }
        }

        // Gen Info
        edm::Handle<std::vector<unsigned int> > acceptedPhiSliceplus;
        event.getByLabel(castorGenInfoTag_.label(),"acceptedPhiSliceplus",acceptedPhiSliceplus);

        unsigned int nAccPhiSliceplus = 0;
        for(unsigned int i = 0; i < (*acceptedPhiSliceplus.product()).size(); i++){
                if((*acceptedPhiSliceplus.product())[i] == 1) nAccPhiSliceplus++;
        }

        edm::Handle<std::vector<unsigned int> > acceptedPhiSliceminus;
        event.getByLabel(castorGenInfoTag_.label(),"acceptedPhiSliceminus",acceptedPhiSliceminus);

        unsigned int nAccPhiSliceminus = 0;
        for(unsigned int i = 0; i < (*acceptedPhiSliceminus.product()).size(); i++){
                if((*acceptedPhiSliceminus.product())[i] == 1) nAccPhiSliceminus++;
        }

        h_nCastorTowerPlusDirect_->Fill(nCastor_plus);
        h_nCastorTowerMinusDirect_->Fill(nCastor_minus); 
        
        h_nCastorGenvsCastorTowerPlusDirect_->Fill(nAccPhiSliceplus,nCastor_plus);
        h_nCastorGenvsCastorTowerMinusDirect_->Fill(nAccPhiSliceminus,nCastor_minus); 
}

void EdmDumpAnalyzer::endJob(){}

DEFINE_FWK_MODULE(EdmDumpAnalyzer);
