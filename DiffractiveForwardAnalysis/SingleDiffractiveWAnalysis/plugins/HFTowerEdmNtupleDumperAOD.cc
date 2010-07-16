#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_HFTowerEdmNtupleDumperAOD_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_HFTowerEdmNtupleDumperAOD_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

class CaloTower;

class HFTowerEdmNtupleDumperAOD : public edm::EDProducer {
public:
  HFTowerEdmNtupleDumperAOD( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  bool checkTowerFlags(CaloTower const&,double);

  edm::InputTag calotowersTag_;

  double eThreshMin_;
  double eThreshMax_;
  unsigned int nIter_;
  bool discardFlaggedTowers_;
  bool applyEnergyScale_;
  double energyScale_;
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/CaloTopology/interface/CaloTowerConstituentsMap.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"
//#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include <vector>

using namespace reco;

HFTowerEdmNtupleDumperAOD::HFTowerEdmNtupleDumperAOD(const edm::ParameterSet& conf) {
	calotowersTag_=conf.getParameter<edm::InputTag>("CaloTowersTag");
	eThreshMin_ = conf.getParameter<double>("TowerEnergyThresholdMin");
	eThreshMax_ = conf.getParameter<double>("TowerEnergyThresholdMax");
	nIter_ = conf.getParameter<unsigned int>("NumberOfThresholds");
        discardFlaggedTowers_ = conf.getParameter<bool>("DiscardFlaggedTowers");
        applyEnergyScale_ = conf.getParameter<bool>("ApplyEnergyScale");
        if(applyEnergyScale_) energyScale_ = conf.getParameter<double>("EnergyScaleFactor");

	std::string alias;
	produces<std::vector<unsigned int> >( alias = "nHFplus" ).setBranchAlias( alias );
	produces<std::vector<unsigned int> >( alias = "nHFminus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "sumEHFplus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "sumEHFminus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "etaHFmax" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "etaHFmin" ).setBranchAlias( alias );
	produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "mapThreshToiEtaplus" ).setBranchAlias( alias );
	produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "mapThreshToiEtaminus" ).setBranchAlias( alias );
        produces<std::vector<double> >( alias = "FBAsymmetryFromHFEnergy" ).setBranchAlias( alias );
        produces<std::vector<double> >( alias = "FBAsymmetryFromHFMult" ).setBranchAlias( alias );
        produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityPlus" ).setBranchAlias( alias );
        produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "iEtaHFMultiplicityMinus" ).setBranchAlias( alias );
        //produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumPlus").setBranchAlias( alias );
        //produces<std::map<unsigned int,std::vector<double> > >(alias = "iEtaHFEnergySumMinus").setBranchAlias( alias ); 
}

void HFTowerEdmNtupleDumperAOD::produce(edm::Event& evt, const edm::EventSetup& stp) {
	//Calo Towers
	/*edm::Handle<CandidateCollection> calotowercands;  
	evt.getByLabel(calotowersLabel_,calotowercands);

	CandidateCollection::const_iterator cand = calotowercands->begin();
	CaloTowerRef towerRef = cand->get<CaloTowerRef>();

	const CaloTowerCollection *towerCollection = towerRef.product();*/

        edm::Handle<CaloTowerCollection> towerCollectionH;
        evt.getByLabel(calotowersTag_,towerCollectionH);
        const CaloTowerCollection& towerCollection = *(towerCollectionH.product());
	
	//Loop over CaloTowers
	std::vector<unsigned int> nhf_plus(nIter_);
        std::vector<unsigned int> nhf_minus(nIter_);
	std::vector<double> sumehf_plus(nIter_);
	std::vector<double> sumehf_minus(nIter_);
	std::vector<double> etagapmax(nIter_);
	std::vector<double> etagapmin(nIter_);
	std::vector<std::vector<int> > towersiEta(nIter_);
	std::vector<std::vector<unsigned int> > towersiEta_plus(nIter_);
	std::vector<std::vector<unsigned int> > towersiEta_minus(nIter_);
        std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_plus;
        std::map<unsigned int,std::vector<unsigned int> > iEtaHFMultiplicity_minus;
        std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_plus;
        std::map<unsigned int,std::vector<double> > iEtaHFEnergySum_minus;
	for(CaloTowerCollection::const_iterator calotower = towerCollection.begin(); calotower != towerCollection.end(); calotower++) {	
		bool hasHF = false;
		bool hasHE = false;
		bool hasHO = false;
		bool hasHB = false;
		bool hasECAL = false; 
		for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
			DetId detId = calotower->constituent(iconst);
			if(detId.det()==DetId::Hcal){
				HcalDetId hcalDetId(detId);
				if(hcalDetId.subdet()==HcalForward){  
					 hasHF = true;		
				} else if(hcalDetId.subdet()==HcalOuter){
					 hasHO = true;
                                } else if(hcalDetId.subdet()==HcalEndcap){
					 hasHE = true;
				} else if(hcalDetId.subdet()==HcalBarrel){
					 hasHB = true;
                                } else{
				} 
			} else if(detId.det()==DetId::Ecal){
				hasECAL = true;
			}	 	  
		}
				 	
		int zside = calotower->id().zside();
		int ieta = calotower->id().ieta();
		double eta = calotower->eta();
		double energy = calotower->energy();
                if(applyEnergyScale_) energy *= energyScale_; 

                bool isHFTower = hasHF&&(!hasHE);
                bool acceptTwr = isHFTower;
                if(acceptTwr && discardFlaggedTowers_ && checkTowerFlags(*calotower,0.99)) acceptTwr = false;
                if(!acceptTwr) continue;
 
		//Loop over tower thresholds
                for(unsigned int i = 0; i < nIter_; i++){
                        double ethresh = eThreshMin_ + i*((eThreshMax_ - eThreshMin_)/nIter_);
                        //bool ethreshHF = (energy >= ethresh);
                        if(energy >= ethresh){
                                if(zside > 0){
					++nhf_plus[i];
					sumehf_plus[i] += energy;
 
                                        unsigned int abs_ieta = ieta;
					towersiEta_plus[i].push_back(abs_ieta);
                                        if(iEtaHFMultiplicity_plus.find(abs_ieta) == iEtaHFMultiplicity_plus.end()){
                                           iEtaHFMultiplicity_plus[abs_ieta] = std::vector<unsigned int>(nIter_);
                                           iEtaHFEnergySum_plus[abs_ieta] = std::vector<double>(nIter_);
                                        } 
                                        ++iEtaHFMultiplicity_plus[abs_ieta][i];
                                        iEtaHFEnergySum_plus[abs_ieta][i] += energy;
				} else{
					++nhf_minus[i];
					sumehf_minus[i] += energy;

                                        unsigned int abs_ieta = -ieta; 
					towersiEta_minus[i].push_back(abs_ieta);
                                        if(iEtaHFMultiplicity_minus.find(abs_ieta) == iEtaHFMultiplicity_minus.end()){
                                           iEtaHFMultiplicity_minus[abs_ieta] = std::vector<unsigned int>(nIter_);
                                           iEtaHFEnergySum_minus[abs_ieta] = std::vector<double>(nIter_);
                                        }
                                        ++iEtaHFMultiplicity_minus[abs_ieta][i];
                                        iEtaHFEnergySum_minus[abs_ieta][i] += energy;
				}	
                                if(eta > etagapmax[i]) etagapmax[i] = eta;
                                if(eta < etagapmin[i]) etagapmin[i] = eta;
				towersiEta[i].push_back(ieta);
                        }
                }
	}

        std::vector<double> fbAsymmetryEnergy(nIter_);
        std::vector<double> fbAsymmetryMult(nIter_);
        for(unsigned int ithresh = 0; ithresh < nIter_; ++ithresh){
                fbAsymmetryEnergy[ithresh] = (sumehf_plus[ithresh] - sumehf_minus[ithresh])/(sumehf_plus[ithresh] + sumehf_minus[ithresh]); 
                fbAsymmetryMult[ithresh] = ((double)nhf_plus[ithresh] - (double)nhf_minus[ithresh])/((double)nhf_plus[ithresh] + (double)nhf_minus[ithresh]);
        }

	std::map<unsigned int, std::vector<unsigned int> > threshieta_plus;
	threshieta_plus.clear();
	std::map<unsigned int, std::vector<unsigned int> > threshieta_minus;
        threshieta_minus.clear();
	for(unsigned int i = 0; i < nIter_; i++){
		std::sort(towersiEta[i].begin(),towersiEta[i].end());
		std::sort(towersiEta_plus[i].begin(),towersiEta_plus[i].end());
		std::sort(towersiEta_minus[i].begin(),towersiEta_minus[i].end());
		threshieta_plus.insert(std::pair<unsigned int, std::vector<unsigned int> >(i,towersiEta_plus[i]));
		threshieta_minus.insert(std::pair<unsigned int, std::vector<unsigned int> >(i,towersiEta_minus[i]));
	}

	std::auto_ptr<std::vector<unsigned int> > nHFplus(new std::vector<unsigned int>(nhf_plus));
	std::auto_ptr<std::vector<unsigned int> > nHFminus(new std::vector<unsigned int>(nhf_minus));
	std::auto_ptr<std::vector<double> > sumEHFplus(new std::vector<double>(sumehf_plus));
	std::auto_ptr<std::vector<double> > sumEHFminus(new std::vector<double>(sumehf_minus));
	std::auto_ptr<std::vector<double> > etaHFmax(new std::vector<double>(etagapmax));
	std::auto_ptr<std::vector<double> > etaHFmin(new std::vector<double>(etagapmin));
	std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaplus(new std::map<unsigned int, std::vector<unsigned int> >(threshieta_plus));
	std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaminus(new std::map<unsigned int, std::vector<unsigned int> >(threshieta_minus));
        std::auto_ptr<std::vector<double> > FBAsymmetryFromHFEnergy(new std::vector<double>(fbAsymmetryEnergy));
        std::auto_ptr<std::vector<double> > FBAsymmetryFromHFMult(new std::vector<double>(fbAsymmetryMult));
        std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus(new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_plus));
        std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus(new std::map<unsigned int, std::vector<unsigned int> >(iEtaHFMultiplicity_minus));
        //std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumPlus(new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_plus));
        //std::auto_ptr<std::map<unsigned int,std::vector<double> > > iEtaHFEnergySumMinus(new std::map<unsigned int,std::vector<double> >(iEtaHFEnergySum_minus));

	evt.put( nHFplus, "nHFplus" );
	evt.put( nHFminus, "nHFminus" );
	evt.put( sumEHFplus, "sumEHFplus" );
	evt.put( sumEHFminus, "sumEHFminus" );
	evt.put( etaHFmax, "etaHFmax" );
	evt.put( etaHFmin, "etaHFmin" );
	evt.put( mapThreshToiEtaplus, "mapThreshToiEtaplus" );
	evt.put( mapThreshToiEtaminus, "mapThreshToiEtaminus" );
        evt.put( FBAsymmetryFromHFEnergy, "FBAsymmetryFromHFEnergy" ); 
        evt.put( FBAsymmetryFromHFMult, "FBAsymmetryFromHFMult" );
        evt.put( iEtaHFMultiplicityPlus, "iEtaHFMultiplicityPlus" );
        evt.put( iEtaHFMultiplicityMinus, "iEtaHFMultiplicityMinus" );
        //evt.put( iEtaHFEnergySumPlus, "iEtaHFEnergySumPlus" );
        //evt.put( iEtaHFEnergySumMinus, "iEtaHFEnergySumMinus" );
}

bool HFTowerEdmNtupleDumperAOD::checkTowerFlags(CaloTower const& tower, double emFracThreshold){
   bool flagged = false;

   double emEnergy = tower.emEnergy();
   double hadEnergy = tower.hadEnergy();
   double eTot = emEnergy + hadEnergy;
   double emFrac = fabs(emEnergy/eTot);
   if(eTot > 50.0 && emFrac > emFracThreshold) flagged = true;

   return flagged;  
}

DEFINE_FWK_MODULE(HFTowerEdmNtupleDumperAOD);
 
