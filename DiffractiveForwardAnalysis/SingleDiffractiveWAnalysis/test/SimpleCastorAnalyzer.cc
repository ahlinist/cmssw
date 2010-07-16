#ifndef SimpleCastorAnalyzer_h
#define SimpleCastorAnalyzer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TH1F;
class TH2F;

class SimpleCastorAnalyzer: public edm::EDAnalyzer{
public:
  explicit SimpleCastorAnalyzer(const edm::ParameterSet&);

  ~SimpleCastorAnalyzer();

  virtual void beginJob(edm::EventSetup const&);
  virtual void endJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
private:
  void fillMultiplicities(const edm::Event&, const edm::EventSetup&);
  void fillFromTowers(const edm::Event&, const edm::EventSetup&);

  edm::InputTag castorGenInfoTag_;
  edm::InputTag castorTowerInfoTag_;
  edm::InputTag castorTowersTag_;
  int gapSide_;
  unsigned int thresholdIndexHF_;
  unsigned int thresholdIndexCastor_;

  bool accessCastorTowers_;
  double thresholdTower_;

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

};
#endif

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CastorReco/interface/CastorTower.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"

using namespace reco;

SimpleCastorAnalyzer::SimpleCastorAnalyzer(const edm::ParameterSet& conf){
  castorGenInfoTag_ = conf.getParameter<edm::InputTag>("CastorGenInfoTag");
  castorTowerInfoTag_ = conf.getParameter<edm::InputTag>("CastorTowerInfoTag");
  gapSide_ = conf.getParameter<int>("GapSide");
  thresholdIndexHF_ = conf.getParameter<unsigned int>("ThresholdIndexHF");
  thresholdIndexCastor_ = conf.getParameter<unsigned int>("ThresholdIndexCastor");
 
  accessCastorTowers_ = conf.getParameter<bool>("AccessCastorTowers");
  if(accessCastorTowers_) castorTowersTag_ = conf.getParameter<edm::InputTag>("CastorTowersTag");
  if(accessCastorTowers_) thresholdTower_ = conf.getParameter<double>("TowerThreshold");
}  
  
SimpleCastorAnalyzer::~SimpleCastorAnalyzer()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void SimpleCastorAnalyzer::beginJob(edm::EventSetup const&iSetup){

  edm::Service<TFileService> fs;

  if(accessCastorTowers_){
    h_nCastorTowerPlusDirect_ = fs->make<TH1F>("nCastorTowerPlusDirect","Castor Towers mult. plus",12,0,12);
    h_nCastorTowerMinusDirect_ = fs->make<TH1F>("nCastorTowerMinusDirect","Castor Towers mult. minus",12,0,12);
    h_nCastorGenvsCastorTowerPlusDirect_ = fs->make<TH2F>("nCastorGenVsCastorTowerPlusDirect","Castor Gen vs Castor towers mult. plus",12,0,12,12,0,12);
    h_nCastorGenvsCastorTowerMinusDirect_ = fs->make<TH2F>("nCastorGenVsCastorTowerMinusDirect","Castor Gen vs Castor towers mult. minus",12,0,12,12,0,12);
    h_CastorTowerEnergyPlus_ = fs->make<TH1F>("CastorTowerEnergyPlus","Castor towers energy plus",200,0.,100.);
    h_CastorTowerEnergyMinus_ = fs->make<TH1F>("CastorTowerEnergyMinus","Castor towers energy minus",200,0.,100.);
  }

  h_nHFTowerPlus_ = fs->make<TH1F>("nHFTowerPlus","HF tower mult. plus",12,0,12);
  h_nHFTowerMinus_ = fs->make<TH1F>("nHFTowerMinus","HF tower mult. minus",12,0,12);

  h_nCastorGenPlus_ = fs->make<TH1F>("nCastorGenPlus","Castor gen mult. plus",12,0,12);
  h_nCastorGenMinus_ = fs->make<TH1F>("nCastorGenMinus","Castor gen mult. minus",12,0,12);

  h_nCastorTowerPlus_ = fs->make<TH1F>("nCastorTowerPlus","Castor Towers mult. plus",12,0,12);
  h_nCastorTowerMinus_ = fs->make<TH1F>("nCastorTowerMinus","Castor Towers mult. minus",12,0,12);

  h_nCastorGenvsCastorTowerPlus_ = fs->make<TH2F>("nCastorGenVsCastorTowerPlus","Castor Gen vs Castor towers mult. plus",12,0,12,12,0,12);
  h_nCastorGenvsCastorTowerMinus_ = fs->make<TH2F>("nCastorGenVsCastorTowerMinus","Castor Gen vs Castor towers mult. minus",12,0,12,12,0,12);

  h_nHFTowerPlusvsnCastorGenPlus_ = fs->make<TH2F>("nHFTowerPlusvsnCastorGenPlus","HF plus vs Castor gen plus",12,0,12,12,0,12);
  h_nHFTowerMinusvsnCastorGenMinus_ = fs->make<TH2F>("nHFTowerMinusvsnCastorGenMinus","HF minus vs Castor gen minus",12,0,12,12,0,12);

  h_nHFTowerPlusvsnCastorTowerPlus_ = fs->make<TH2F>("nHFTowerPlusvsnCastorTowerPlus","HF plus vs Castor tower plus",12,0,12,12,0,12);
  h_nHFTowerMinusvsnCastorTowerMinus_ = fs->make<TH2F>("nHFTowerMinusvsnCastorTowerMinus","HF minus vs Castor tower minus",12,0,12,12,0,12);
}     

void SimpleCastorAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
	fillMultiplicities(event,setup);
        if(accessCastorTowers_) fillFromTowers(event,setup);
}

void SimpleCastorAnalyzer::fillMultiplicities(const edm::Event& event, const edm::EventSetup& setup)
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

        h_nHFTowerPlus_->Fill((*nHFTowerPlus.product())[thresholdIndexHF_]);
        h_nHFTowerMinus_->Fill((*nHFTowerMinus.product())[thresholdIndexHF_]);

        h_nCastorTowerPlus_->Fill((*nCastorTowerPlus.product())[thresholdIndexCastor_]);
        h_nCastorTowerMinus_->Fill((*nCastorTowerMinus.product())[thresholdIndexCastor_]);
	h_nCastorGenPlus_->Fill(nAccPhiSliceplus);
        h_nCastorGenMinus_->Fill(nAccPhiSliceminus);	
 
        h_nCastorGenvsCastorTowerPlus_->Fill(nAccPhiSliceplus,(*nCastorTowerPlus.product())[thresholdIndexCastor_]);
        h_nCastorGenvsCastorTowerMinus_->Fill(nAccPhiSliceminus,(*nCastorTowerMinus.product())[thresholdIndexCastor_]);

        h_nHFTowerPlusvsnCastorGenPlus_->Fill((*nHFTowerPlus.product())[thresholdIndexHF_],nAccPhiSliceplus);
        h_nHFTowerMinusvsnCastorGenMinus_->Fill((*nHFTowerMinus.product())[thresholdIndexHF_],nAccPhiSliceminus);

        h_nHFTowerPlusvsnCastorTowerPlus_->Fill((*nHFTowerPlus.product())[thresholdIndexHF_],(*nCastorTowerPlus.product())[thresholdIndexCastor_]);
        h_nHFTowerMinusvsnCastorTowerMinus_->Fill((*nHFTowerMinus.product())[thresholdIndexHF_],(*nCastorTowerMinus.product())[thresholdIndexCastor_]);
}

void SimpleCastorAnalyzer::fillFromTowers(const edm::Event& event, const edm::EventSetup& setup)
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

void SimpleCastorAnalyzer::endJob(){}

DEFINE_FWK_MODULE(SimpleCastorAnalyzer);
