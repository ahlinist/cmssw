#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_CASTORGenInfoDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_CASTORGenInfoDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class CASTORGenInfoDumper : public edm::EDProducer {
public:
  CASTORGenInfoDumper( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag genpartsstableLabel_;   
 
  double CASTORParticleEnergyThreshold_;
  double CASTORPhiSliceEnergyThreshold_;
  double CASTOREtaMin_;
  double CASTOREtaMax_;

  bool useCustomPhiSeg_;

  unsigned int numberOfSlices_; 
  double slicesPhimin[50];
  double slicesPhimax[50];      
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats//Common/interface/View.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace reco;

CASTORGenInfoDumper::CASTORGenInfoDumper(const edm::ParameterSet& conf) {
	genpartsstableLabel_ = conf.getParameter<edm::InputTag>("GenParticlesLabel");
	CASTORParticleEnergyThreshold_ = conf.getUntrackedParameter<double>("CASTORParticleEnergyThreshold",-1.);
	CASTORPhiSliceEnergyThreshold_ = conf.getUntrackedParameter<double>("CASTORPhiSliceEnergyThreshold",-1.);
	CASTOREtaMin_ = conf.getUntrackedParameter<double>("CASTOREtaMin",5.2);
        CASTOREtaMax_ = conf.getUntrackedParameter<double>("CASTOREtaMax",6.6);

	useCustomPhiSeg_ = conf.getUntrackedParameter<bool>("UseCustomPhiSlices",false);

	if(useCustomPhiSeg_){
		edm::ParameterSet phiseg_pset = conf.getParameter<edm::ParameterSet>("CASTORPhiSlices");
		std::vector<double> vecdef;
    		vecdef.push_back(0);
    		std::vector<double> vecSlicesMin = phiseg_pset.getUntrackedParameter<std::vector<double> >("PhiMin",vecdef); 
		std::vector<double> vecSlicesMax = phiseg_pset.getUntrackedParameter<std::vector<double> >("PhiMax",vecdef);
		if(vecSlicesMin.size() != vecSlicesMax.size()) throw edm::Exception(edm::errors::Configuration,"CASTORGenInfoDumperError") << ">>>>>>>> PhiMin and PhiMax vectors must have same size";
		numberOfSlices_ = vecSlicesMin.size();
		for(unsigned int i = 0; i < numberOfSlices_; i++){
			slicesPhimin[i] = vecSlicesMin[i];
			slicesPhimax[i] = vecSlicesMax[i];
		}
	} else{ 
		numberOfSlices_ = 16;
 		for(unsigned int i = 0; i < numberOfSlices_; i++){
			slicesPhimin[i] = ((double)i)*(2*M_PI)/((double)numberOfSlices_);
                	slicesPhimax[i] = ((double)(i + 1))*(2*M_PI)/((double)numberOfSlices_);
                	//LogTrace("") << "Slice " << i << ": " << "(" << slicesPhimin[i] << "," << slicesPhimax[i] << ")" << "\n";
		}
	}
	for(unsigned int i = 0; i < numberOfSlices_; i++) LogTrace("") << "Slice " << i << ": " << "(" << slicesPhimin[i] << "," << slicesPhimax[i] << ")" << "\n";

	std::string alias;
	produces<std::vector<double> >( alias = "energyPhiSliceplus" ).setBranchAlias( alias );
	produces<std::vector<double> >( alias = "energyPhiSliceminus" ).setBranchAlias( alias );
	produces<std::vector<unsigned int> >( alias = "acceptedPhiSliceplus" ).setBranchAlias( alias );
	produces<std::vector<unsigned int> >( alias = "acceptedPhiSliceminus" ).setBranchAlias( alias );
}

void CASTORGenInfoDumper::produce(edm::Event& evt, const edm::EventSetup& iSetup) {
	//Gen Particles
	edm::Handle<edm::View<GenParticle> > gencands;
       	evt.getByLabel(genpartsstableLabel_,gencands);

	std::vector<double> energysum_plus;
	std::vector<double> energysum_minus;
	for(unsigned int i = 0; i < numberOfSlices_; i++){
		energysum_plus.push_back(0.);
		energysum_minus.push_back(0.);
	}
	for(edm::View<GenParticle>::const_iterator cand = (*gencands).begin(); cand != (*gencands).end(); ++cand) {
		double eta = cand->eta();
		double phi = cand->phi();
		//Loop over CASTOR phi bins
		if((cand->energy() > CASTORParticleEnergyThreshold_)&&(fabs(eta) > CASTOREtaMin_)&&(fabs(eta) < CASTOREtaMax_)){
			for(unsigned int i = 0; i < numberOfSlices_; i++){
				if(phi < 0) phi += 2*M_PI;
				if((phi >= slicesPhimin[i])&&(phi < slicesPhimax[i])){
                        		if(eta >= 0) energysum_plus[i] += cand->energy();
                        		else energysum_minus[i] += cand->energy();
				}
			}
		}
	}
	//Check phi slices above threshold
	std::vector<unsigned int> accphi_plus;
        std::vector<unsigned int> accphi_minus;
	for(unsigned int i = 0; i < numberOfSlices_; i++){
		accphi_plus.push_back((energysum_plus[i] > CASTORPhiSliceEnergyThreshold_)?1:0);
		accphi_minus.push_back((energysum_minus[i] > CASTORPhiSliceEnergyThreshold_)?1:0);
		LogTrace("") << "Slice " << i << "\n"
					 << "Energy Sum plus side= " << energysum_plus[i] << "\n"
					 << "Acc= " << accphi_plus[i] << "\n"
					 << "Energy Sum minus side= " << energysum_minus[i] << "\n"          
                                         << "Acc= " << accphi_minus[i] << "\n";  
	}	
	std::auto_ptr<std::vector<double> > energyPhiSliceplus(new std::vector<double>(energysum_plus));
	std::auto_ptr<std::vector<double> > energyPhiSliceminus(new std::vector<double>(energysum_minus));
	std::auto_ptr<std::vector<unsigned int> > acceptedPhiSliceplus(new std::vector<unsigned int>(accphi_plus));
	std::auto_ptr<std::vector<unsigned int> > acceptedPhiSliceminus(new std::vector<unsigned int>(accphi_minus));

	evt.put( energyPhiSliceplus, "energyPhiSliceplus" );
	evt.put( energyPhiSliceminus, "energyPhiSliceminus" );
	evt.put( acceptedPhiSliceplus, "acceptedPhiSliceplus" );
	evt.put( acceptedPhiSliceminus, "acceptedPhiSliceminus" );
}

DEFINE_FWK_MODULE(CASTORGenInfoDumper);
 
