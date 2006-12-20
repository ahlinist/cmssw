#ifndef RecoTBCalo_EcalTBRecProducers_EcalTBH2LocalReadout_HH
#define RecoTBCalo_EcalTBRecProducers_EcalTBH2LocalReadout_HH

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"


// forward declaration
class EcalTBH2LocalReadout : public edm::EDProducer {

  public:
    typedef std::vector<double> EcalRecoAmplitudes;
    explicit EcalTBH2LocalReadout(const edm::ParameterSet& ps);
    ~EcalTBH2LocalReadout();
    virtual void produce(edm::Event& evt, const edm::EventSetup& es);

  private:

    std::string digiProducer_; // name of module/plugin/producer making digis
    std::string EBdigiCollection_; // secondary name given to collection of digis
    int tableEtaPosition_;
    int tablePhiPosition_;
    
    /// search for the TT to be read
    void findTTlist(const int & crysId, const EcalTrigTowerConstituentsMap& etmap);
    
    /// read only the digis from the selected TT
    void readOut(const EBDigiCollection & input, EBDigiCollection & output, const EcalTrigTowerConstituentsMap& etmap);
    
    /// master function to be called once per event
    void performReadout(const EcalTrigTowerConstituentsMap & theTTmap, const EBDigiCollection & input, EBDigiCollection & output);

    int theTargetCrystal_;
    
    std::vector<EcalTrigTowerDetId> theTTlist_;
    
    static const int NCRYMATRIX = 17;
    
    std::vector<DetId> theDetIds;

    //    std::string EBreducedDigiCollection_; // secondary name to be given to collection of hit

};
#endif
