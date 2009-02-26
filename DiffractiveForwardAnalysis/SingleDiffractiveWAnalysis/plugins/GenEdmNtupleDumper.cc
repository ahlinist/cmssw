#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_GenEdmNtupleDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_GenEdmNtupleDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class GenEdmNtupleDumper : public edm::EDProducer {
public:
  GenEdmNtupleDumper( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag genpartsstableLabel_;   
 
  //bool keep4vectors_;
 
  int pdgid;
  double genpartethresh;
  double genpartetamin;
  double genpartetamax;
  double TKPtThreshold;
  double TKEtaMin;
  double TKEtaMax;
  double HFEnergyThreshold;
  double HFEtaMin;
  double HFEtaMax;
  double CASTOREnergyThreshold;
  double CASTOREtaMin;
  double CASTOREtaMax;
  double T2EnergyThreshold;
  double T2EtaMin;
  double T2EtaMax;
  double ZDCEnergyThreshold;
  double ZDCEtaMin;
  double ZDCEtaMax;
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Candidate/interface/Candidate.h"

using namespace reco;

GenEdmNtupleDumper::GenEdmNtupleDumper(const edm::ParameterSet& conf) {
	genpartsstableLabel_ = conf.getParameter<edm::InputTag>("GenParticlesLabel");
	//keep4vectors_ = conf.getUntrackedParameter<bool>("Keep4Vectors",false);

	genpartethresh = conf.getParameter<double>("GenPartEnergyThreshold");
	genpartetamin = conf.getParameter<double>("GenPartEtaMin");
	genpartetamax = conf.getParameter<double>("GenPartEtaMax");
	TKPtThreshold = conf.getUntrackedParameter<double>("TKPtThreshold",0.9);
        TKEtaMin = conf.getUntrackedParameter<double>("TKEtaMin",0.0);
        TKEtaMax = conf.getUntrackedParameter<double>("TKEtaMax",2.0);
	HFEnergyThreshold = conf.getUntrackedParameter<double>("HFEnergyThreshold",4.0);
        HFEtaMin = conf.getUntrackedParameter<double>("HFEtaMin",2.9);
        HFEtaMax = conf.getUntrackedParameter<double>("HFEtaMax",5.0);
	CASTOREnergyThreshold = conf.getUntrackedParameter<double>("CASTOREnergyThreshold",-1.);
	CASTOREtaMin = conf.getUntrackedParameter<double>("CASTOREtaMin",5.2);
        CASTOREtaMax = conf.getUntrackedParameter<double>("CASTOREtaMax",6.6);
	T2EnergyThreshold = conf.getUntrackedParameter<double>("T2EnergyThreshold",-1.);
        T2EtaMin = conf.getUntrackedParameter<double>("T2EtaMin",5.2);
        T2EtaMax = conf.getUntrackedParameter<double>("T2EtaMax",6.6);
	ZDCEnergyThreshold = conf.getUntrackedParameter<double>("ZDCEnergyThreshold",-1.);
	ZDCEtaMin = conf.getUntrackedParameter<double>("ZDCEtaMin",8.0);
        ZDCEtaMax = conf.getUntrackedParameter<double>("ZDCEtaMax",10.0);

	std::string alias;
	produces<std::vector<math::XYZTLorentzVector> >( alias = "genParticles" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenplus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenminus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenTKall" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenTKplus" ).setBranchAlias( alias );
        produces<unsigned int>( alias = "nGenTKminus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenHFplus" ).setBranchAlias( alias );
        produces<unsigned int>( alias = "nGenHFminus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenCASTORplus" ).setBranchAlias( alias );
        produces<unsigned int>( alias = "nGenCASTORminus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenT2plus" ).setBranchAlias( alias );
        produces<unsigned int>( alias = "nGenT2minus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "nGenZDCplus" ).setBranchAlias( alias );
        produces<unsigned int>( alias = "nGenZDCminus" ).setBranchAlias( alias );
	produces<double>( alias = "etaGenmax" ).setBranchAlias( alias );
	produces<double>( alias = "etaGenmin" ).setBranchAlias( alias );
	produces<double>( alias = "xiGenplus" ).setBranchAlias( alias );
        produces<double>( alias = "xiGenminus" ).setBranchAlias( alias );
}

void GenEdmNtupleDumper::produce(edm::Event& evt, const edm::EventSetup& iSetup) {
	//Gen Particles
	edm::Handle<CandidateCollection> gencands;
       	evt.getByLabel(genpartsstableLabel_,gencands);

	std::auto_ptr<std::vector<math::XYZTLorentzVector> > genParticles(new std::vector<math::XYZTLorentzVector>);
	double proton_pzmin = 0.;
	double proton_pzmax = 0.;
	double nonproton_etamax = 0.;
	double nonproton_etamin = 0.;
	int gencount_plus = 0;
	int gencount_minus = 0;
	int TKcount_all = 0;
	int TKcount_plus = 0;
        int TKcount_minus = 0;
	int HFcount_plus = 0;
        int HFcount_minus = 0;
	int CASTORcount_plus = 0;
	int CASTORcount_minus = 0;
	int T2count_plus = 0;
        int T2count_minus = 0;
	int ZDCcount_plus = 0;
        int ZDCcount_minus = 0;
	for(CandidateCollection::const_iterator cand = (*gencands).begin(); cand != (*gencands).end(); ++cand) {
		/*math::XYZTLorentzVector mygenpart(cand->px(),cand->py(),cand->pz(),cand->energy());
		genParticles->push_back(mygenpart);*/
			
		double eta = cand->eta();
		if((cand->energy() > genpartethresh)&&(fabs(eta) > genpartetamin)&&(fabs(eta) < genpartetamax)){
			if(eta >= 0) gencount_plus++;
			else gencount_minus++;
			math::XYZTLorentzVector mygenpart(cand->px(),cand->py(),cand->pz(),cand->energy());
                	genParticles->push_back(mygenpart);
		} 
		//Get tracks count (only charged)
                if((cand->charge() != 0)&&(cand->pt() > TKPtThreshold)&&(fabs(eta) > TKEtaMin)&&(fabs(eta) < TKEtaMax)){
			TKcount_all++;
                        if(eta >= 0) TKcount_plus++;
                        else TKcount_minus++;
                }
		//Get count in HF
                if((abs(cand->pdgId()) != 13)&&(cand->energy() > HFEnergyThreshold)&&(fabs(eta) > HFEtaMin)&&(fabs(eta) < HFEtaMax)){
                        if(eta >= 0) HFcount_plus++;
                        else HFcount_minus++;
                } 
		//Get count in CASTOR
		if((abs(cand->pdgId()) != 13)&&(cand->energy() > CASTOREnergyThreshold)&&(fabs(eta) > CASTOREtaMin)&&(fabs(eta) < CASTOREtaMax)){
                        if(eta >= 0) CASTORcount_plus++;
                        else CASTORcount_minus++;
                }
		//Get count in T2 (only charged)
                if((cand->charge() != 0)&&(cand->energy() > T2EnergyThreshold)&&(fabs(eta) > T2EtaMin)&&(fabs(eta) < T2EtaMax)){
                        if(eta >= 0) T2count_plus++;
                        else T2count_minus++;
                }
		//Get count in ZDC (only neutrals)
                if((cand->charge() == 0)&&(cand->energy() > ZDCEnergyThreshold)&&(fabs(eta) > ZDCEtaMin)&&(fabs(eta) < ZDCEtaMax)){
                        if(eta >= 0) ZDCcount_plus++;
                        else ZDCcount_minus++;
                }
		//Find highest pz proton
		if((cand->pdgId() == 2212)&&(cand->pz() > 5200.0)){
			if(cand->pz() > proton_pzmax) proton_pzmax = cand->pz();
		}
		//Find lowest pz proton
		if((cand->pdgId() == 2212)&&(cand->pz() < -5200.0)){
                        if(cand->pz() < proton_pzmin) proton_pzmin = cand->pz();
                }	
		if((cand->pdgId() != 2212)&&(eta > nonproton_etamax)) nonproton_etamax = eta;
		if((cand->pdgId() != 2212)&&(eta < nonproton_etamin)) nonproton_etamin = eta;
	}
	double xigenplus = 1.0 - proton_pzmax/7000.;
	double xigenminus = 1.0 + proton_pzmin/7000.;

	std::auto_ptr<unsigned int> nGenplus(new unsigned int(gencount_plus));
	std::auto_ptr<unsigned int> nGenminus(new unsigned int(gencount_minus));
	std::auto_ptr<unsigned int> nGenTKall(new unsigned int(TKcount_all));
	std::auto_ptr<unsigned int> nGenTKplus(new unsigned int(TKcount_plus));
        std::auto_ptr<unsigned int> nGenTKminus(new unsigned int(TKcount_minus));
	std::auto_ptr<unsigned int> nGenHFplus(new unsigned int(HFcount_plus));
        std::auto_ptr<unsigned int> nGenHFminus(new unsigned int(HFcount_minus));
	std::auto_ptr<unsigned int> nGenCASTORplus(new unsigned int(CASTORcount_plus));
        std::auto_ptr<unsigned int> nGenCASTORminus(new unsigned int(CASTORcount_minus));
	std::auto_ptr<unsigned int> nGenT2plus(new unsigned int(T2count_plus));
        std::auto_ptr<unsigned int> nGenT2minus(new unsigned int(T2count_minus));
	std::auto_ptr<unsigned int> nGenZDCplus(new unsigned int(ZDCcount_plus));
        std::auto_ptr<unsigned int> nGenZDCminus(new unsigned int(ZDCcount_minus));
	std::auto_ptr<double> etaGenmax(new double(nonproton_etamax));
	std::auto_ptr<double> etaGenmin(new double(nonproton_etamin));
	std::auto_ptr<double> xiGenplus(new double(xigenplus));
	std::auto_ptr<double> xiGenminus(new double(xigenminus));

	evt.put( genParticles, "genParticles" );
	evt.put( nGenplus, "nGenplus" );
	evt.put( nGenminus, "nGenminus" );
	evt.put( nGenTKall, "nGenTKall" );
	evt.put( nGenTKplus, "nGenTKplus" );
        evt.put( nGenTKminus, "nGenTKminus" );
	evt.put( nGenHFplus, "nGenHFplus" );
        evt.put( nGenHFminus, "nGenHFminus" );
	evt.put( nGenCASTORplus, "nGenCASTORplus" );
        evt.put( nGenCASTORminus, "nGenCASTORminus" );
	evt.put( nGenT2plus, "nGenT2plus" );
        evt.put( nGenT2minus, "nGenT2minus" );
	evt.put( nGenZDCplus, "nGenZDCplus" );
        evt.put( nGenZDCminus, "nGenZDCminus" );
	evt.put( etaGenmax, "etaGenmax" );
	evt.put( etaGenmin, "etaGenmin" );
	evt.put( xiGenplus, "xiGenplus" );
	evt.put( xiGenminus, "xiGenminus" );
}

DEFINE_FWK_MODULE(GenEdmNtupleDumper);
 
