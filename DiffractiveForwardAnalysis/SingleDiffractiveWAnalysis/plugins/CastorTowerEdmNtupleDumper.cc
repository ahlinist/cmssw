#ifndef CastorTowerEdmNtupleDumper_h
#define CastorTowerEdmNtupleDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>

class CastorTowerEdmNtupleDumper: public edm::EDProducer {
public:
  CastorTowerEdmNtupleDumper(const edm::ParameterSet&);
private:
  void produce(edm::Event&,const edm::EventSetup&);
  edm::InputTag castorTowersTag_;

  double eTowerThreshMin_;
  double eTowerThreshMax_;
  unsigned int nThreshIter_;

  std::vector<double> vecEThresh_;
}; 
#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/CastorReco/interface/CastorTower.h"
 
#include <string>

using namespace reco;

CastorTowerEdmNtupleDumper::CastorTowerEdmNtupleDumper(const edm::ParameterSet& conf) {
    castorTowersTag_=conf.getParameter<edm::InputTag>("CastorTowersTag");
    eTowerThreshMin_ = conf.getUntrackedParameter<double>("TowerEnergyThresholdMin",0.0);
    eTowerThreshMax_ = conf.getUntrackedParameter<double>("TowerEnergyThresholdMax",20.0);
    nThreshIter_ = conf.getUntrackedParameter<unsigned int>("NumberOfThresholds",50);

    double deltaEThresh = (eTowerThreshMax_ - eTowerThreshMin_)/nThreshIter_;
    edm::LogVerbatim("Analysis") << ">>> Looking at " << nThreshIter_ << "threshold values w/ delta(threshold) = " << deltaEThresh;
    vecEThresh_ = std::vector<double>(nThreshIter_,0.);
    for(size_t i = 0; i < vecEThresh_.size(); ++i){
      vecEThresh_[i] = eTowerThreshMin_ + i*deltaEThresh;
      edm::LogVerbatim("Analysis") << ">>> Threshold " << i << ": " << vecEThresh_[i];
    }

    std::string alias;
    produces<std::vector<unsigned int> >( alias = "nCastorTowerPlus" ).setBranchAlias( alias );
    produces<std::vector<unsigned int> >( alias = "nCastorTowerMinus" ).setBranchAlias( alias );
    produces<std::vector<double> >( alias = "sumECastorTowerPlus" ).setBranchAlias( alias );
    produces<std::vector<double> >( alias = "sumECastorTowerMinus" ).setBranchAlias( alias );
}

void CastorTowerEdmNtupleDumper::produce(edm::Event& event, const edm::EventSetup& stp) {
    //Castor Towers
    edm::Handle<CastorTowerCollection> castorTowers;  
    event.getByLabel(castorTowersTag_,castorTowers);

    std::vector<unsigned int> nCastor_plus(nThreshIter_,0);
    std::vector<unsigned int> nCastor_minus(nThreshIter_,0);
    std::vector<double> sumECastor_plus(nThreshIter_,0.);
    std::vector<double> sumECastor_minus(nThreshIter_,0.);
    
    for(CastorTowerCollection::const_iterator castorTwr = castorTowers->begin(); castorTwr != castorTowers->end(); ++castorTwr){
      for(size_t i = 0; i < vecEThresh_.size(); ++i){
        if(castorTwr->energy() >= vecEThresh_[i]){
          if(castorTwr->eta() >= 0.){
            ++nCastor_plus[i];
            sumECastor_plus[i] += castorTwr->energy();
          } else{
            ++nCastor_minus[i];
            sumECastor_minus[i] += castorTwr->energy();
          } 
        }
      }
    } 

    std::auto_ptr<std::vector<unsigned int> > nCastorTowerPlus(new std::vector<unsigned int>(nCastor_plus));
    std::auto_ptr<std::vector<unsigned int> > nCastorTowerMinus(new std::vector<unsigned int>(nCastor_minus));
    std::auto_ptr<std::vector<double> > sumECastorTowerPlus(new std::vector<double>(sumECastor_plus));
    std::auto_ptr<std::vector<double> > sumECastorTowerMinus(new std::vector<double>(sumECastor_minus));

    event.put(nCastorTowerPlus,"nCastorTowerPlus");
    event.put(nCastorTowerMinus,"nCastorTowerMinus");
    event.put(sumECastorTowerPlus,"sumECastorTowerPlus");
    event.put(sumECastorTowerMinus,"sumECastorTowerMinus");
}

DEFINE_FWK_MODULE(CastorTowerEdmNtupleDumper);
 
