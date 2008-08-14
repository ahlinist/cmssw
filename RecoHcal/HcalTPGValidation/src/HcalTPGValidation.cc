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
// $Id$
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

	alldata_et_HBHE = fs->make<TH1F>("alldata_et_HBHE","All Data (HB/HE)",255,0,255);
	alldata_et_HBHE->SetXTitle("data.e");
	alldata_et_HF= fs->make<TH1F>("alldata_et_HF","All Data (HF)",255,0,255);
	alldata_et_HF->SetXTitle("data.et");
	allemul_et_HBHE = fs->make<TH1F>("allemul_et_HBHE","All Data (HB/HE)",255,0,255);
	allemul_et_HBHE->SetXTitle("emul.e");
	allemul_et_HF= fs->make<TH1F>("allemul_et_HF","All Data (HF)",255,0,255);
	allemul_et_HF->SetXTitle("emul.et");
	dataonly_et_HBHE = fs->make<TH1F>("dataonly_et_HBHE","Data Only (HB/HE)",255,0,255);
	dataonly_et_HBHE->SetXTitle("data.e");
	dataonly_et_HF = fs->make<TH1F>("dataonly_et_HF","Data Only (HF)",255,0,255);
	dataonly_et_HF->SetXTitle("data.et");
	emulonly_et_HBHE = fs->make<TH1F>("emulonly_et_HBHE","Emul Only (HB/HE)",255,0,255);
	emulonly_et_HBHE->SetXTitle("emul.e");
	emulonly_et_HF = fs->make<TH1F>("emulonly_et_HF","Emul Only (HF)",255,0,255);
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

	typedef HcalTrigPrimDigiCollection::const_iterator TPItr;
	if (hcal_tp_data.isValid() && hcal_tp_emul.isValid() && hcal_tp_data->size() == hcal_tp_emul->size()){
		for(TPItr data = hcal_tp_data->begin(), emul = hcal_tp_emul->begin();
			data != hcal_tp_data->end() && emul != hcal_tp_emul->end();
			++data, ++emul){
			bool IsHF = (data->id().ietaAbs()>=29);

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
				if (emulEt == 0) errflag=3;
				else {
					if (dataEt == 0) errflag=4;
					else if (dataEt != emulEt) errflag=2;
					else errflag = 5;
				}
			}

			//Fill occupancy plots for not zero TPG
			if (errflag != 0 ){
				alldata_map->Fill(data->id().ieta(),data->id().iphi());
				allemul_map->Fill(emul->id().ieta(),emul->id().iphi());
				if (errflag == 2) mismatch_map->Fill(emul->id().ieta(),emul->id().iphi());
			}

			//Histograms for HF
			if (IsHF) {
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
			else {
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
			if (!IsHF && errflag>1) {
				edm::LogInfo ("HBHEDebug") << "HBHE\t ieta: " << emul->id().ieta() << "\tiphi: " << emul->id().iphi() << "\tdata.et: " << data->SOI_compressedEt() << "\temul.et: " << emul->SOI_compressedEt();
				std::stringstream debugString("");
				debugString << "data:\t" << data->presamples() << '\t';
				for (int i=0; i<data->size(); ++i) debugString << data->sample(i).compressedEt() << ' ';
				debugString << '\n';
				debugString << "emul:\t" << emul->presamples() << '\t';
				for (int i=0; i<emul->size(); ++i) debugString << emul->sample(i).compressedEt() << ' ';
				debugString << '\n';
				debugString  << "HBHE\t ieta: " << emul->id().ieta() << "\tiphi: " << emul->id().iphi() << "\tdata.et: " << data->SOI_compressedEt() << "\temul.et: " << emul->SOI_compressedEt() << '\n';;
				for (HBHEDigiCollection::const_iterator frame = hbheDigis->begin(); frame != hbheDigis->end(); ++frame){
					HcalTrigTowerGeometry theTrigTowerGeometry;
					std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(frame->id());
					if (data->id().rawId() != ids[0].rawId()) continue;
					debugString << "HBHE Frames:\t";
					for (int i=0;i<frame->size(); ++i){
						debugString << frame->sample(i).adc() << ' ';
					}
					debugString << '\n';
					
					edm::ESHandle<HcalTPGCoder> inputCoder;
					iSetup.get<HcalTPGRecord>().get(inputCoder);
					debugString << "adc2Linear\t";
					IntegerCaloSamples samples(ids[0], frame->size());
					samples.setPresamples(frame->presamples());
					inputCoder->adc2Linear(*frame, samples);
					for (int i=0;i<samples.size();++i) debugString << samples[i] << ' ';
					debugString << '\n';
					edm::LogVerbatim ("HBHEDebug") << debugString.str();	
					break;
				}	
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
