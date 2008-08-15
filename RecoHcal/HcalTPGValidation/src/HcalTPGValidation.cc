// -*- C++ -*-
//
// Package:    HcalTPGValidation
// Class:      HcalTPGValidation
// 
/**\class HcalTPGValidation HcalTPGValidation.cc RecoHcal/HcalTPGValidation/src/HcalTPGValidation.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Ka Vang TSANG
//         Created:  Tue Jul 22 15:29:01 CEST 2008
// $Id: HcalTPGValidation.cc,v 1.2 2008/08/14 14:00:38 kvtsang Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "Geometry/HcalTowerAlgo/interface/HcalTrigTowerGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalTrigTowerDetId.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalTPGCoder.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//TFileService
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"
#include "TAxis.h"

//
// class decleration
//

class HcalTPGValidation : public edm::EDAnalyzer {
   public:
      explicit HcalTPGValidation(const edm::ParameterSet&);
      ~HcalTPGValidation();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
		 HcalTrigTowerGeometry theTrigTowerGeometry;

		TH2F *alldata_map;
		TH2F *allemul_map;
		TH2F *dataonly_map;
		TH2F *emulonly_map;
		TH2F *mismatch_map;
		TH1F *alldata_et_HBHE;
		TH1F *allemul_et_HBHE;
		TH1F *alldata_et_HF;
		TH1F *allemul_et_HF;
		TH1F *dataonly_et_HBHE;
		TH1F *emulonly_et_HBHE;
		TH1F *dataonly_et_HF;
		TH1F *emulonly_et_HF;
		TH1F *data_FG_HF;
		TH1F *data_FG_HBHE;
		TH1F *emul_FG_HF;
		TH1F *emul_FG_HBHE;
		TH1F *data_FGet_HF;
		TH1F *data_FGet_HBHE;
		TH1F *emul_FGet_HF;
		TH1F *emul_FGet_HBHE;

		TH1F *errflag_HF;
		TH1F *errflag_HBHE;
		TH2F *etcorr_HF;
		TH2F *etcorr_HBHE;

		bool IsDebugHF;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalTPGValidation::HcalTPGValidation(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
	edm::Service<TFileService> fs;
	TAxis *xaxis = NULL;

	alldata_map = fs->make<TH2F>("alldata_map","All Data",65,-32.5,32.5,80,0,80);
	alldata_map->SetXTitle("data.ieta");
	alldata_map->SetYTitle("data.iphi");

	allemul_map = fs->make<TH2F>("allemul_map","All Emul",65,-32.5,32.5,80,0,80);
	allemul_map->SetXTitle("emul.ieta");
	allemul_map->SetYTitle("emul.iphi");

	dataonly_map = fs->make<TH2F>("dataonly_map","Data Only",65,-32.5,32.5,80,0,80);
	dataonly_map->SetXTitle("data.ieta");
	dataonly_map->SetYTitle("data.iphi");

	emulonly_map = fs->make<TH2F>("emulonly_map","Emul Only",65,-32.5,32.5,80,0,80);
	emulonly_map->SetXTitle("emul.ieta");
	emulonly_map->SetYTitle("emul.iphi");

	mismatch_map = fs->make<TH2F>("mismatch_map","Mismatched e/et",65,-32.5,32.5,80,0,80);
	mismatch_map->SetXTitle("ieta");
	mismatch_map->SetYTitle("iphi");

	errflag_HF = fs->make<TH1F>("errflag_HF","Error Flags (HF)",5,0.5,5.5);
	xaxis = errflag_HF->GetXaxis();
	xaxis->SetBinLabel(1,"Matched");
	xaxis->SetBinLabel(2,"Mismatched et");
	xaxis->SetBinLabel(3,"Data only");
	xaxis->SetBinLabel(4,"Emul only");
	xaxis->SetBinLabel(5,"Mismatched FG bit");

	errflag_HBHE = fs->make<TH1F>("errflag_HBHE","Error Flags (HBHE)",5,0.5,5.5);
	xaxis = errflag_HBHE->GetXaxis();
	xaxis->SetBinLabel(1,"Matched");
	xaxis->SetBinLabel(2,"Mismatched e");
	xaxis->SetBinLabel(3,"Data only");
	xaxis->SetBinLabel(4,"Emul only");
	xaxis->SetBinLabel(5,"Mismatched FG bit");

	etcorr_HBHE = fs->make<TH2F>("etcorr_HBHE","HB/HE",50,0,300,50,0,300);
	etcorr_HBHE->SetXTitle("data.e");
	etcorr_HBHE->SetYTitle("emul.e");
	etcorr_HF = fs->make<TH2F>("etcorr_HF","HF",50,0,300,50,0,300);
	etcorr_HF->SetXTitle("data.e");
	etcorr_HF->SetYTitle("emul.e");

	alldata_et_HBHE = fs->make<TH1F>("alldata_et_HBHE","All Data (HB/HE)",256,0,256);
	alldata_et_HBHE->SetXTitle("data.e");
	alldata_et_HF= fs->make<TH1F>("alldata_et_HF","All Data (HF)",256,0,256);
	alldata_et_HF->SetXTitle("data.et");
	allemul_et_HBHE = fs->make<TH1F>("allemul_et_HBHE","All Emul (HB/HE)",256,0,256);
	allemul_et_HBHE->SetXTitle("emul.e");
	allemul_et_HF= fs->make<TH1F>("allemul_et_HF","All Emul (HF)",256,0,256);
	allemul_et_HF->SetXTitle("emul.et");
	dataonly_et_HBHE = fs->make<TH1F>("dataonly_et_HBHE","Data Only (HB/HE)",256,0,256);
	dataonly_et_HBHE->SetXTitle("data.e");
	dataonly_et_HF = fs->make<TH1F>("dataonly_et_HF","Data Only (HF)",256,0,256);
	dataonly_et_HF->SetXTitle("data.et");
	emulonly_et_HBHE = fs->make<TH1F>("emulonly_et_HBHE","Emul Only (HB/HE)",256,0,256);
	emulonly_et_HBHE->SetXTitle("emul.e");
	emulonly_et_HF = fs->make<TH1F>("emulonly_et_HF","Emul Only (HF)",256,0,256);
	emulonly_et_HF->SetXTitle("emul.et");

	//FG bit
	data_FG_HF = fs->make<TH1F>("data_FG_HF","Data FG bit (HF)",2,-0.5,1.5);
	xaxis = data_FG_HF->GetXaxis();
	xaxis->SetBinLabel(1,"False");
	xaxis->SetBinLabel(2,"True");
	data_FG_HBHE = fs->make<TH1F>("data_FG_HBHE","Data FG bit (HB/HE)",2,-0.5,1.5);
	xaxis = data_FG_HBHE->GetXaxis();
	xaxis->SetBinLabel(1,"False");
	xaxis->SetBinLabel(2,"True");
	emul_FG_HF = fs->make<TH1F>("emul_FG_HF","Emul FG bit (HF)",2,-0.5,1.5);
	xaxis = emul_FG_HF->GetXaxis();
	xaxis->SetBinLabel(1,"False");
	xaxis->SetBinLabel(2,"True");
	emul_FG_HBHE = fs->make<TH1F>("emul_FG_HBHE","Emul FG bit (HB/HE)",2,-0.5,1.5);
	xaxis = emul_FG_HBHE->GetXaxis();
	xaxis->SetBinLabel(1,"False");
	xaxis->SetBinLabel(2,"True");

	data_FGet_HF = fs->make<TH1F>("data_FGet_HF","FG=1 (HF)",255,0,255);
	data_FGet_HF->SetXTitle("data.et");
	data_FGet_HBHE = fs->make<TH1F>("data_FGet_HBHE","FG=1 (HB/HE)",255,0,255);
	data_FGet_HBHE->SetXTitle("data.e");
	emul_FGet_HF = fs->make<TH1F>("emul_FGet_HF","FG=1 (HF)",255,0,255);
	emul_FGet_HF->SetXTitle("emul.et");
	emul_FGet_HBHE = fs->make<TH1F>("emul_FGet_HBHE","FG=1 (HB/HE)",255,0,255);
	emul_FGet_HBHE->SetXTitle("emul.e");
	
	IsDebugHF = iConfig.getParameter<bool>("DebugHF");
}


HcalTPGValidation::~HcalTPGValidation()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HcalTPGValidation::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

	edm::Handle<HcalTrigPrimDigiCollection> hcal_tp_data;
	iEvent.getByLabel("hcalDigis",hcal_tp_data);

	edm::Handle<HcalTrigPrimDigiCollection> hcal_tp_emul;
	iEvent.getByLabel("valHcalTriggerPrimitiveDigis",hcal_tp_emul);

	edm::Handle<HBHEDigiCollection> hbheDigis;
	iEvent.getByLabel("hcalDigis",hbheDigis);

	edm::Handle<HFDigiCollection> hfDigis;
	iEvent.getByLabel("hcalDigis",hfDigis);

	edm::ESHandle<HcalTPGCoder> inputCoder;
	iSetup.get<HcalTPGRecord>().get(inputCoder);

	typedef HcalTrigPrimDigiCollection::const_iterator TPItr;
	if (hcal_tp_data.isValid() && hcal_tp_emul.isValid()){
		for(TPItr emul = hcal_tp_emul->begin(); emul != hcal_tp_emul->end(); ++emul){
		//for(TPItr emul = hcal_tp_emul->begin(), data = hcal_tp_data->begin(); emul != hcal_tp_emul->end() && data != hcal_tp_data->end(); ++emul, ++data){
			bool IsHF = (emul->id().ietaAbs()>=29);
			TPItr data = hcal_tp_data->begin();
			if(IsHF){
				for (;data != hcal_tp_data->end(); ++data){
					if(data->id().ieta()==emul->id().ieta() && (4*data->id().iphi()-3)==emul->id().iphi()) break;
					//if(data->id().ieta()==emul->id().ieta() && data->id().iphi()==emul->id().iphi()) break;
				}
			}
			else data = hcal_tp_data->find(emul->id());
			//TPItr data = hcal_tp_data->find(emul->id());
			if (data == hcal_tp_data->end()) continue;


			//--------------------
			//Error Flag
			// 1:	Matched 
			// 2:	Mismatched e/et
			// 3:	Data only
			// 4: Emul only
			// 5: Mismatched FG bit
			//--------------------

			int errflag = 0;
			int dataEt = data->SOI_compressedEt();
			int emulEt = emul->SOI_compressedEt();
			 if (dataEt == emulEt && data->SOI_fineGrain() == emul->SOI_fineGrain()){
				if (dataEt>0) errflag=1;
			}
			else{
				if (data->SOI_fineGrain() != emul->SOI_fineGrain()) errflag = 5;
				if (emulEt == 0) errflag=3;
				else {
					if (dataEt == 0) errflag=4;
					else if (dataEt != emulEt) errflag=2;
				}
			}

			//Fill occupancy plots for not zero TPG
			if (errflag != 0 ){
				alldata_map->Fill(data->id().ieta(),data->id().iphi());
				allemul_map->Fill(emul->id().ieta(),emul->id().iphi());
				if (errflag == 2) mismatch_map->Fill(emul->id().ieta(),emul->id().iphi());
			}

			//Histograms for HF
			if (IsHF && errflag>0) {
				errflag_HF->Fill(errflag);
				alldata_et_HF->Fill(dataEt);
				allemul_et_HF->Fill(emulEt);
				data_FG_HF->Fill(data->SOI_fineGrain());
				emul_FG_HF->Fill(emul->SOI_fineGrain());
				if(data->SOI_fineGrain()) data_FGet_HF->Fill(dataEt);
				if(emul->SOI_fineGrain()) emul_FGet_HF->Fill(emulEt);
				switch (errflag){
					case 1:
						etcorr_HF->Fill(dataEt,emulEt);
						break;
					case 2:
						etcorr_HF->Fill(dataEt,emulEt);
						break;
					case 3:
						dataonly_et_HF->Fill(dataEt);
						dataonly_map->Fill(data->id().ieta(),data->id().iphi());
						break;
					case 4:
						emulonly_et_HF->Fill(emulEt);
						emulonly_map->Fill(emul->id().ieta(),emul->id().iphi());
						break;
				}
			}
			//Histograms for HB/HE
			else if (errflag >0) {
				errflag_HBHE->Fill(errflag);
				alldata_et_HBHE->Fill(dataEt);
				allemul_et_HBHE->Fill(emulEt);
				data_FG_HBHE->Fill(data->SOI_fineGrain());
				emul_FG_HBHE->Fill(emul->SOI_fineGrain());
				if(data->SOI_fineGrain()) data_FGet_HBHE->Fill(dataEt);
				if(emul->SOI_fineGrain()) emul_FGet_HBHE->Fill(emulEt);
				switch (errflag){
					case 1:
						etcorr_HBHE->Fill(dataEt,emulEt);
						break;
					case 2:
						etcorr_HBHE->Fill(dataEt,emulEt);
						break;
					case 3:
						dataonly_et_HBHE->Fill(dataEt);
						dataonly_map->Fill(data->id().ieta(),data->id().iphi());
						break;
					case 4:
						emulonly_et_HBHE->Fill(emulEt);
						emulonly_map->Fill(emul->id().ieta(),emul->id().iphi());
						break;
				}
			}
			//Debug for HBHE
			if (!IsHF && errflag>1) {
				edm::LogInfo ("DebugHBHE") << "HBHE\t ieta: " << emul->id().ieta() << "\tiphi: " << emul->id().iphi() << "\tdata.et: " << data->SOI_compressedEt() << "\temul.et: " << emul->SOI_compressedEt();
				std::stringstream debugString("");
				debugString << "data:\t" << data->presamples() << '\t';
				for (int i=0; i<data->size(); ++i) debugString << data->sample(i).compressedEt() << ' ';
				debugString << '\n';
				debugString << "emul:\t" << emul->presamples() << '\t';
				for (int i=0; i<emul->size(); ++i) debugString << emul->sample(i).compressedEt() << ' ';
				debugString << '\n';
				for (HBHEDigiCollection::const_iterator frame = hbheDigis->begin(); frame != hbheDigis->end(); ++frame){
					HcalTrigTowerGeometry theTrigTowerGeometry;
					std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(frame->id());
					if (data->id().rawId() != ids[0].rawId()) continue;
					debugString << "HBHE Frames:\t";
					for (int i=0;i<frame->size(); ++i){
						debugString << frame->sample(i).adc() << ' ';
					}
					debugString << '\n';
					
					debugString << "adc2Linear\t";
					IntegerCaloSamples samples(ids[0], frame->size());
					samples.setPresamples(frame->presamples());
					inputCoder->adc2Linear(*frame, samples);
					for (int i=0;i<samples.size();++i) debugString << samples[i] << ' ';
					debugString << '\n';
					edm::LogVerbatim ("DebugHBHE") << debugString.str();	
					break;
				}	
			}
			//Debug for HF
			if (IsHF && errflag>1 && IsDebugHF) {
				edm::LogInfo ("DebugHF") << "HF\t ieta: " << emul->id().ieta() << "\tiphi: " << emul->id().iphi() << "\tdata.et: " << data->SOI_compressedEt() << "\temul.et: " << emul->SOI_compressedEt();
				std::stringstream debugString("");
				debugString << "data:\t" << data->presamples() << '\t' << data->id().ieta() << '/' << data->id().iphi() << '\t';
				for (int i=0; i<data->size(); ++i) debugString << data->sample(i).compressedEt() << ' ';
				debugString << '\n';
				//Dump FG bit
				debugString << "data FG bit:\t";
				for (int i=0; i<data->size(); ++i) debugString << data->sample(i).fineGrain() << ' ';
				debugString << '\n';

				debugString << "emul:\t" << emul->presamples() << '\t' << emul->id().ieta() << '/' << emul->id().iphi() << '\t';
				for (int i=0; i<emul->size(); ++i) debugString << emul->sample(i).compressedEt() << ' ';
				debugString << '\n';
				//Dump FG bit
				debugString << "emul FG bit:\t";
				for (int i=0; i<emul->size(); ++i) debugString << emul->sample(i).fineGrain() << ' ';
				debugString << '\n';

				IntegerCaloSamples *sum = NULL;
				for ( HFDigiCollection::const_iterator frame = hfDigis->begin(); frame != hfDigis->end(); ++frame){
					std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(frame->id());
					if (ids[0] == emul->id()){
						debugString <<  "HF Frames:\t";
						for (int i=0;i<frame->size(); ++i)
							debugString << frame->sample(i).adc() << ' ';
						debugString << '\n';

						IntegerCaloSamples samples(ids[0], frame->size());
						samples.setPresamples(frame->presamples());
						inputCoder->adc2Linear(*frame, samples);

						if(sum == NULL) {
							sum = new IntegerCaloSamples(ids[0], frame->size());
							sum->setPresamples(frame->presamples());
						}

						debugString << "adc2Linear:\t";
						for(int i = 0; i < samples.size(); ++i) {
							(*sum)[i] += samples[i];
							debugString << samples[i] << ' ';
						}
						debugString << '\n';
					}
				}

				debugString << "Sum:\t";
				for(int i = 0; i < sum->size(); ++i) {
					debugString << (*sum)[i] << ' ';
					(*sum)[i] /= 8;
				}
				debugString << '\n';
				edm::LogVerbatim ("DebugHF") << debugString.str();	
				if (sum != NULL) delete sum;
			}
		}
	}
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalTPGValidation::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTPGValidation::endJob() {
	//std::cout << std::flush;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalTPGValidation);
