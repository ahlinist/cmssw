#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_HFTowerEdmNtupleDumperAOD_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_HFTowerEdmNtupleDumperAOD_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

class HFTowerEdmNtupleDumperAOD : public edm::EDProducer {
public:
  HFTowerEdmNtupleDumperAOD( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag calotowersTag_;

  //double e_tresholdHF;        
  double etresh_min;
  double etresh_max;
  unsigned int n_iter;                  
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

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

using namespace reco;

HFTowerEdmNtupleDumperAOD::HFTowerEdmNtupleDumperAOD(const edm::ParameterSet& conf) {
	calotowersTag_=conf.getParameter<edm::InputTag>("CaloTowersTag");
	//e_tresholdHF = conf.getParameter<double>("TowerEnergyTresholdHF");
	etresh_min = conf.getUntrackedParameter<double>("TowerEnergyTresholdMin",0.0);
	etresh_max = conf.getUntrackedParameter<double>("TowerEnergyTresholdMax",5.0);
	n_iter = conf.getUntrackedParameter<unsigned int>("NumberOfTresholds",50);

	std::string alias;
	produces<std::vector<unsigned int> >( alias = "nHFplus" ).setBranchAlias( alias );
	produces<std::vector<unsigned int> >( alias = "nHFminus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "sumEHFplus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "sumEHFminus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "etaHFmax" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "etaHFmin" ).setBranchAlias( alias );
	produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "mapTreshToiEtaplus" ).setBranchAlias( alias );
	produces<std::map<unsigned int, std::vector<unsigned int> > >( alias = "mapTreshToiEtaminus" ).setBranchAlias( alias );
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
	std::vector<unsigned int> nhf_plus;
        std::vector<unsigned int> nhf_minus;
	std::vector<double> sumehf_plus;
	std::vector<double> sumehf_minus;
	std::vector<double> etagapmax;
	std::vector<double> etagapmin;
	std::vector<std::vector<int> > towersiEta;
	std::vector<std::vector<unsigned int> > towersiEta_plus;
	std::vector<std::vector<unsigned int> > towersiEta_minus;
	for(unsigned int i = 0; i < n_iter; i++){
		nhf_plus.push_back(0);
		nhf_minus.push_back(0);
		sumehf_plus.push_back(0);
		sumehf_minus.push_back(0);
		etagapmax.push_back(0.);
		etagapmin.push_back(0.);
		towersiEta.push_back(std::vector<int>());
		towersiEta_plus.push_back(std::vector<unsigned int>());
		towersiEta_minus.push_back(std::vector<unsigned int>());
	} 
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
	
		//Loop over tower tresholds
                for(unsigned int i = 0; i < n_iter; i++){
                        double etresh = etresh_min + i*((etresh_max - etresh_min)/n_iter);
                        bool etreshHF = (energy >= etresh);
                        if(etreshHF&&hasHF&&(!hasHE)){
                                if(zside > 0){
					nhf_plus[i]++;
					sumehf_plus[i] += energy;
					towersiEta_plus[i].push_back(ieta);
				} else{
					nhf_minus[i]++;
					sumehf_minus[i] += energy;
					towersiEta_minus[i].push_back(-ieta);
				}	
                                if(eta > etagapmax[i]) etagapmax[i] = eta;
                                if(eta < etagapmin[i]) etagapmin[i] = eta;
				towersiEta[i].push_back(ieta);
                        }
                }
	}

	std::map<unsigned int, std::vector<unsigned int> > treshieta_plus;
	treshieta_plus.clear();
	std::map<unsigned int, std::vector<unsigned int> > treshieta_minus;
        treshieta_minus.clear();
	for(unsigned int i = 0; i < n_iter; i++){
		std::sort(towersiEta[i].begin(),towersiEta[i].end());
		std::sort(towersiEta_plus[i].begin(),towersiEta_plus[i].end());
		std::sort(towersiEta_minus[i].begin(),towersiEta_minus[i].end());
		treshieta_plus.insert(std::pair<unsigned int, std::vector<unsigned int> >(i,towersiEta_plus[i]));
		treshieta_minus.insert(std::pair<unsigned int, std::vector<unsigned int> >(i,towersiEta_minus[i]));
	}

	std::auto_ptr<std::vector<unsigned int> > nHFplus(new std::vector<unsigned int>(nhf_plus));
	std::auto_ptr<std::vector<unsigned int> > nHFminus(new std::vector<unsigned int>(nhf_minus));
	std::auto_ptr<std::vector<double> > sumEHFplus(new std::vector<double>(sumehf_plus));
	std::auto_ptr<std::vector<double> > sumEHFminus(new std::vector<double>(sumehf_minus));
	std::auto_ptr<std::vector<double> > etaHFmax(new std::vector<double>(etagapmax));
	std::auto_ptr<std::vector<double> > etaHFmin(new std::vector<double>(etagapmin));
	std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > mapTreshToiEtaplus(new std::map<unsigned int, std::vector<unsigned int> >(treshieta_plus));
	std::auto_ptr<std::map<unsigned int, std::vector<unsigned int> > > mapTreshToiEtaminus(new std::map<unsigned int, std::vector<unsigned int> >(treshieta_minus));

	evt.put( nHFplus, "nHFplus" );
	evt.put( nHFminus, "nHFminus" );
	evt.put( sumEHFplus, "sumEHFplus" );
	evt.put( sumEHFminus, "sumEHFminus" );
	evt.put( etaHFmax, "etaHFmax" );
	evt.put( etaHFmin, "etaHFmin" );
	evt.put(mapTreshToiEtaplus, "mapTreshToiEtaplus" );
	evt.put(mapTreshToiEtaminus, "mapTreshToiEtaminus" );
}

DEFINE_FWK_MODULE(HFTowerEdmNtupleDumperAOD);
 
