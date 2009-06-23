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
	std::vector<unsigned int> nhf_plus(n_iter);
        std::vector<unsigned int> nhf_minus(n_iter);
	std::vector<double> sumehf_plus(n_iter);
	std::vector<double> sumehf_minus(n_iter);
	std::vector<double> etagapmax(n_iter);
	std::vector<double> etagapmin(n_iter);
	std::vector<std::vector<int> > towersiEta(n_iter);
	std::vector<std::vector<unsigned int> > towersiEta_plus(n_iter);
	std::vector<std::vector<unsigned int> > towersiEta_minus(n_iter);
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
	
		//Loop over tower tresholds
                for(unsigned int i = 0; i < n_iter; i++){
                        double etresh = etresh_min + i*((etresh_max - etresh_min)/n_iter);
                        bool etreshHF = (energy >= etresh);
                        if(etreshHF&&hasHF&&(!hasHE)){
                                if(zside > 0){
					nhf_plus[i]++;
					sumehf_plus[i] += energy;
          
                                        unsigned int abs_ieta = ieta;
					towersiEta_plus[i].push_back(abs_ieta);
                                        if(iEtaHFMultiplicity_plus.find(abs_ieta) == iEtaHFMultiplicity_plus.end()){
                                           iEtaHFMultiplicity_plus[abs_ieta] = std::vector<unsigned int>(n_iter);
                                           iEtaHFEnergySum_plus[abs_ieta] = std::vector<double>(n_iter);
                                        } 
                                        ++iEtaHFMultiplicity_plus[abs_ieta][i];
                                        iEtaHFEnergySum_plus[abs_ieta][i] += energy;
				} else{
					nhf_minus[i]++;
					sumehf_minus[i] += energy;

                                        unsigned int abs_ieta = -ieta; 
					towersiEta_minus[i].push_back(abs_ieta);
                                        if(iEtaHFMultiplicity_minus.find(abs_ieta) == iEtaHFMultiplicity_minus.end()){
                                           iEtaHFMultiplicity_minus[abs_ieta] = std::vector<unsigned int>(n_iter);
                                           iEtaHFEnergySum_minus[abs_ieta] = std::vector<double>(n_iter);
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

        std::vector<double> fbAsymmetryEnergy(n_iter);
        std::vector<double> fbAsymmetryMult(n_iter);
        for(unsigned int ithresh = 0; ithresh < n_iter; ++ithresh){
                fbAsymmetryEnergy[ithresh] = (sumehf_plus[ithresh] - sumehf_minus[ithresh])/(sumehf_plus[ithresh] + sumehf_minus[ithresh]); 
                fbAsymmetryMult[ithresh] = ((double)nhf_plus[ithresh] - (double)nhf_minus[ithresh])/((double)nhf_plus[ithresh] + (double)nhf_minus[ithresh]);
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
	evt.put( mapTreshToiEtaplus, "mapTreshToiEtaplus" );
	evt.put( mapTreshToiEtaminus, "mapTreshToiEtaminus" );
        evt.put( FBAsymmetryFromHFEnergy, "FBAsymmetryFromHFEnergy" ); 
        evt.put( FBAsymmetryFromHFMult, "FBAsymmetryFromHFMult" );
        evt.put( iEtaHFMultiplicityPlus, "iEtaHFMultiplicityPlus" );
        evt.put( iEtaHFMultiplicityMinus, "iEtaHFMultiplicityMinus" );
        //evt.put( iEtaHFEnergySumPlus, "iEtaHFEnergySumPlus" );
        //evt.put( iEtaHFEnergySumMinus, "iEtaHFEnergySumMinus" );
}

DEFINE_FWK_MODULE(HFTowerEdmNtupleDumperAOD);
 
