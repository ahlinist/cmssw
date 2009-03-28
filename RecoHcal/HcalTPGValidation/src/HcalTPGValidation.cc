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
// $Id: HcalTPGValidation.cc,v 1.4 2008/08/21 19:39:27 kvtsang Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>

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
#include "Geometry/CaloTopology/interface/HcalTopology.h"

//FE Error
#include "DataFormats/FEDRawData/interface/FEDRawDataCollection.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "EventFilter/HcalRawToDigi/interface/HcalDCCHeader.h"
#include "EventFilter/HcalRawToDigi/interface/HcalHTRData.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

//FileService
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
            std::multimap<HcalTrigTowerDetId, HcalDetId> MapTrigerTower;
		const HcalElectronicsMap *emap;
		std::set<uint32_t> FrontEndErrors;

		//--------------------
		//Error Flag
		// 1:	Matched 
		// 2:	Mismatched e/et
		// 3:	Data only
		// 4: Emul only
		// 5: Mismatched FG bit
		// 6: Z.S. error
		//--------------------
		enum ErrorFlag {
			ErrFlag_ZeroTP,
			ErrFlag_Matched,
			ErrFlag_MismatchedE,
			ErrFlag_DataOnly,
			ErrFlag_EmulOnly,
			ErrFlag_MismatchedFG,
			ErrFlag_ZS
		};

		TH2F *alldata_map;
		TH2F *allemul_map;
		TH2F *dataonly_map;
		TH2F *emulonly_map;
		TH2F *mismatch_map;
		TH2F *mismatch_FG_map;
		TH1F *alldata_et_HBHE;
		TH1F *allemul_et_HBHE;
		TH1F *alldata_et_HF;
		TH1F *allemul_et_HF;
		TH1F *dataonly_et_HBHE;
		TH1F *emulonly_et_HBHE;
		TH1F *dataonly_et_HF;
		TH1F *emulonly_et_HF;
		TH1F *emulZS_et_HBHE;
		TH1F *emulZS_et_HF;
		TH1F *data_FG_HF;
		TH1F *data_FG_HBHE;
		TH1F *emul_FG_HF;
		TH1F *emul_FG_HBHE;
		TH1F *data_FGet_HF;
		TH1F *data_FGet_HBHE;
		TH1F *emul_FGet_HF;
		TH1F *emul_FGet_HBHE;
		TH2F *data_FG_et_HF;
		TH2F *emul_FG_et_HF;

		TH1F *errflag_HF;
		TH1F *errflag_HBHE;
		TH2F *etcorr_HF;
		TH2F *etcorr_HBHE;
		TH2F *etcorr2_HF;
		TH2F *etcorr2_HBHE;

		TH2F *emul_ZS_map;
		TH1F *dataCollectionSize;
		TH1F *emulCollectionSize;
		TH1F *HFTSCollectionSize;
		TH1F *HBHETSCollectionSize;

		TH1F *dataSOI_HF;
		TH1F *emulSOI_HF;
		TH1F *dataSOI_HBHE;
		TH1F *emulSOI_HBHE;

		TH2F *HTRErrMap;
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

	mismatch_FG_map = fs->make<TH2F>("mismatch_FG_map","Mismatched FG bit",65,-32.5,32.5,80,0,80);
	mismatch_FG_map->SetXTitle("ieta");
	mismatch_FG_map->SetYTitle("iphi");

	emul_ZS_map = fs->make<TH2F>("emul_ZS_map","Z.S. error",65,-32.5,32.5,80,0,80);
	emul_ZS_map->SetXTitle("emul.ieta");
	emul_ZS_map->SetYTitle("emul.iphi");

	HTRErrMap = fs->make<TH2F>("HTRErrMap","HTR Errors",65,-32.5,32.5,80,0,80);
	HTRErrMap->SetXTitle("ieta");
	HTRErrMap->SetYTitle("iphi");

	errflag_HF = fs->make<TH1F>("errflag_HF","Error Flags (HF)",6,0.5,6.5);
	xaxis = errflag_HF->GetXaxis();
	xaxis->SetBinLabel(1,"Matched");
	xaxis->SetBinLabel(2,"Mismatched et");
	xaxis->SetBinLabel(3,"Data only");
	xaxis->SetBinLabel(4,"Emul only");
	xaxis->SetBinLabel(5,"Mismatched FG bit");
	xaxis->SetBinLabel(6,"Z.S.");

	errflag_HBHE = fs->make<TH1F>("errflag_HBHE","Error Flags (HBHE)",6,0.5,6.5);
	xaxis = errflag_HBHE->GetXaxis();
	xaxis->SetBinLabel(1,"Matched");
	xaxis->SetBinLabel(2,"Mismatched e");
	xaxis->SetBinLabel(3,"Data only");
	xaxis->SetBinLabel(4,"Emul only");
	xaxis->SetBinLabel(5,"Mismatched FG bit");
	xaxis->SetBinLabel(6,"Z.S.");

	etcorr_HBHE = fs->make<TH2F>("etcorr_HBHE","HB/HE",50,0,256,50,0,256);
	etcorr_HBHE->SetXTitle("data.e");
	etcorr_HBHE->SetYTitle("emul.e");
	etcorr_HF = fs->make<TH2F>("etcorr_HF","HF",56,0,56,56,0,56);
	etcorr_HF->SetXTitle("data.et");
	etcorr_HF->SetYTitle("emul.et");
	etcorr2_HBHE = fs->make<TH2F>("etcorr2_HBHE","HB/HE",50,0,256,10,0,10);
	etcorr2_HBHE->SetXTitle("data.e");
	etcorr2_HBHE->SetYTitle("data.e-emul.e");
	etcorr2_HF = fs->make<TH2F>("etcorr2_HF","HF",56,0,56,10,0,10);
	etcorr2_HF->SetXTitle("data.et");
	etcorr2_HF->SetYTitle("data.et-emul.et");

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
	emulZS_et_HBHE = fs->make<TH1F>("emulZS_et_HBHE","ZS Errors (HB/HE)",256,0,256);
	emulZS_et_HBHE->SetXTitle("emul.e");
	emulZS_et_HF = fs->make<TH1F>("emulZS_et_HF","ZS Errors (HF)",256,0,256);
	emulZS_et_HF->SetXTitle("emul.e");

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

	data_FGet_HF = fs->make<TH1F>("data_FGet_HF","FG=1 (HF)",256,0,256);
	data_FGet_HF->SetXTitle("data.et");
	data_FGet_HBHE = fs->make<TH1F>("data_FGet_HBHE","FG=1 (HB/HE)",256,0,256);
	data_FGet_HBHE->SetXTitle("data.e");
	emul_FGet_HF = fs->make<TH1F>("emul_FGet_HF","FG=1 (HF)",256,0,256);
	emul_FGet_HF->SetXTitle("emul.et");
	emul_FGet_HBHE = fs->make<TH1F>("emul_FGet_HBHE","FG=1 (HB/HE)",256,0,256);
	emul_FGet_HBHE->SetXTitle("emul.e");

	data_FG_et_HF = fs->make<TH2F>("data_FG_et_HF","FG bit vs et",256,0,256,2,-0.5,2.5);
	data_FG_et_HF->SetXTitle("data.et");
	data_FG_et_HF->SetYTitle("FG bit");
	emul_FG_et_HF = fs->make<TH2F>("emul_FG_et_HF","FG bit vs et",256,0,256,2,-0.5,2.5);
	emul_FG_et_HF->SetXTitle("emul.et");
	emul_FG_et_HF->SetYTitle("FG bit");

	dataCollectionSize = fs->make<TH1F>("dataCollectionSize","Data Collection Size",8000,0,8000);
	emulCollectionSize = fs->make<TH1F>("emulCollectionSize","Emul Collection Size",8000,0,8000);
	HFTSCollectionSize = fs->make<TH1F>("HFTSCollectionSize","HF Collection Size",8000,0,8000);
	HBHETSCollectionSize = fs->make<TH1F>("HBHETSCollectionSize","HB/HE Collection Size",8000,0,8000);

	dataSOI_HF = fs->make<TH1F>("dataSOI_HF","data SOI (HF)",4,-0.5,3.5);
	emulSOI_HF = fs->make<TH1F>("emulSOI_HF","emul SOI (HF)",10,-0.5,9.5);
	dataSOI_HBHE = fs->make<TH1F>("dataSOI_HBHE","data SOI (HB/HE)",4,-0.5,3.5);
	emulSOI_HBHE = fs->make<TH1F>("emulSOI_HBHE","emul SOI (HB/HE)",10,-0.5,9.5);

      //Build Triger Tower Map
      HcalTopology theTopo;
      for (int ieta=-41; ieta <= 41; ieta++) {
         for (int depth = 1; depth <= 3; depth++) {
            for (int iphi = 1; iphi <= 72; iphi++) {
               HcalDetId did(HcalBarrel,ieta,iphi,depth);
               if (!theTopo.valid(did)) continue;
               std::vector<HcalTrigTowerDetId> trigtowers = theTrigTowerGeometry.towerIds(did);
               for (std::vector<HcalTrigTowerDetId>::const_iterator trigId=trigtowers.begin(); trigId!=trigtowers.end(); ++trigId){
                  //std::cerr << *trigId << "\t\t" << did << std::endl;
                  MapTrigerTower.insert(std::make_pair(*trigId,did));
               }
            }
         }
      }
      for (int ieta=-41; ieta <= 41; ieta++) {
         for (int depth = 1; depth <= 3; depth++) {
            for (int iphi = 1; iphi <= 72; iphi++) {
               HcalDetId did(HcalEndcap,ieta,iphi,depth);
               if (!theTopo.valid(did)) continue;
               std::vector<HcalTrigTowerDetId> trigtowers = theTrigTowerGeometry.towerIds(did);
               for (std::vector<HcalTrigTowerDetId>::const_iterator trigId=trigtowers.begin(); trigId!=trigtowers.end(); ++trigId){
                  MapTrigerTower.insert(std::make_pair(*trigId,did));
               }
            }
         }
      }
      for (int ieta=-41; ieta <= 41; ieta++) {
         for (int depth = 1; depth <= 3; depth++) {
            for (int iphi = 1; iphi <= 72; iphi++) {
               HcalDetId did(HcalForward,ieta,iphi,depth);
               if (!theTopo.valid(did)) continue;
               std::vector<HcalTrigTowerDetId> trigtowers = theTrigTowerGeometry.towerIds(did);
               for (std::vector<HcalTrigTowerDetId>::const_iterator trigId=trigtowers.begin(); trigId!=trigtowers.end(); ++trigId){
                  MapTrigerTower.insert(std::make_pair(*trigId,did));
               }
            }
         }
      }
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

	HFTSCollectionSize->Fill(hfDigis->size());
	HBHETSCollectionSize->Fill(hbheDigis->size());
	
	//Front End Format Errors
	Handle<FEDRawDataCollection> rawraw;
	iEvent.getByType(rawraw);
	if(rawraw.isValid()){
		for(int i=FEDNumbering::getHcalFEDIds().first; i<=FEDNumbering::getHcalFEDIds().second; ++i){
			const FEDRawData& raw = rawraw->FEDData(i);
			if (raw.size()<12) continue;
			const HcalDCCHeader* dccHeader=(const HcalDCCHeader*)(raw.data());
			if(!dccHeader) continue;
			HcalHTRData htr;
			for (int spigot=0; spigot<HcalDCCHeader::SPIGOT_COUNT; spigot++) {
				if (!dccHeader->getSpigotPresent(spigot)) continue;
				dccHeader->getSpigotData(spigot,htr,raw.size());
				int dccid = dccHeader->getSourceId();
				int errWord = htr.getErrorsWord() & 0x1FFFF;
				bool HTRError = (!htr.check() || htr.isHistogramEvent() || (errWord ^ 0x8000)!=0);
				
				if(HTRError){
					bool valid =false;
					for(int fchan=0; fchan<3 && !valid; fchan++){
						for(int fib=0; fib<9 && !valid; fib++){
							HcalElectronicsId eid(fchan,fib,spigot,dccid-FEDNumbering::getHcalFEDIds().first);
							eid.setHTR(htr.readoutVMECrateId(),htr.htrSlot(),htr.htrTopBottom());
							DetId detId = emap->lookup(eid);
							if(detId.null()) continue;
							HcalSubdetector subdet=(HcalSubdetector(detId.subdetId()));
							if (detId.det()!=4|| 
									(subdet!=HcalBarrel && subdet!=HcalEndcap && 
									subdet!=HcalForward )) continue;
							std::vector<HcalTrigTowerDetId> ids = theTrigTowerGeometry.towerIds(detId);
							for (std::vector<HcalTrigTowerDetId>::const_iterator triggerId=ids.begin(); triggerId != ids.end(); ++triggerId){
								FrontEndErrors.insert(triggerId->rawId());
							}
							//valid = true;
						}
					}
				}
			}
		}
	}

	//Loop over TP collection
	typedef HcalTrigPrimDigiCollection::const_iterator TPItr;
	if (hcal_tp_data.isValid() && hcal_tp_emul.isValid()){
		dataCollectionSize->Fill(hcal_tp_data->size());
		emulCollectionSize->Fill(hcal_tp_emul->size());
		for(TPItr emul = hcal_tp_emul->begin(); emul != hcal_tp_emul->end(); ++emul){

			//Need a better method to determine HF ieta
			bool IsHF = (emul->id().ietaAbs()>=29);
			TPItr data = hcal_tp_data->find(emul->id());

			if (data == hcal_tp_data->end()) continue;

			ErrorFlag errflag = ErrFlag_ZeroTP;
			bool FEError=false;
			for (int i=0; i<data->size() && !FEError; ++i){
				int dataEt = data->sample(i).compressedEt();
				int emulEt = emul->sample(i).compressedEt();
				int dataFG = data->sample(i).fineGrain();
				int emulFG = emul->sample(i).fineGrain();

				//exclude zero TPs
				if (dataEt==0 && emulEt==0) continue;

				if (dataEt==emulEt && dataFG==emulFG && data->zsMarkAndPass()==emul->zsMarkAndPass()) errflag=ErrFlag_Matched;
				else if (dataEt==0) errflag=ErrFlag_EmulOnly;
				else if (emulEt==0) errflag=ErrFlag_DataOnly;
				else if (dataEt!=emulEt) errflag=ErrFlag_MismatchedE;
				else if (dataFG!=emulFG) errflag=ErrFlag_MismatchedFG;
                        else errflag = ErrFlag_ZS;

				//Check Front End Format Errors
				if (errflag != ErrFlag_Matched) {
					std::set<uint32_t>::const_iterator itr = FrontEndErrors.find(data->id().rawId());
					if (itr != FrontEndErrors.end()) {
						FEError = true;
						HTRErrMap->Fill(data->id().ieta(),data->id().iphi());
						continue;
					}
				}

				alldata_map->Fill(data->id().ieta(),data->id().iphi());
				allemul_map->Fill(emul->id().ieta(),emul->id().iphi());

				//Histograms for HF
				if (IsHF) {
					errflag_HF->Fill(errflag);
					alldata_et_HF->Fill(dataEt);
					allemul_et_HF->Fill(emulEt);
					data_FG_HF->Fill(dataFG);
					emul_FG_HF->Fill(emulFG);
					data_FG_et_HF->Fill(dataEt,dataFG);
					emul_FG_et_HF->Fill(emulEt,emulFG);
					dataSOI_HF->Fill(data->presamples());
					emulSOI_HF->Fill(emul->presamples());
					if(dataFG) data_FGet_HF->Fill(dataEt);
					if(emulFG) emul_FGet_HF->Fill(emulEt);
					switch (errflag){
						case ErrFlag_Matched:
							etcorr_HF->Fill(dataEt,emulEt);
							etcorr2_HF->Fill(dataEt,dataEt-emulEt);
							break;
						case ErrFlag_MismatchedE:
							etcorr_HF->Fill(dataEt,emulEt);
							etcorr2_HF->Fill(dataEt,dataEt-emulEt);
							mismatch_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
						case ErrFlag_DataOnly:
							dataonly_et_HF->Fill(dataEt);
							dataonly_map->Fill(data->id().ieta(),data->id().iphi());
							break;
						case ErrFlag_EmulOnly:
							emulonly_et_HF->Fill(emulEt);
							emulonly_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
						case ErrFlag_MismatchedFG:
							etcorr_HF->Fill(dataEt,emulEt);
							etcorr2_HF->Fill(dataEt,dataEt-emulEt);
							mismatch_FG_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
                                    case ErrFlag_ZS:
                                          emul_ZS_map->Fill(emul->id().ieta(),emul->id().iphi());
                                    default:
                                          break;
					}
				}
				//Histograms for HB/HE
				else {
					errflag_HBHE->Fill(errflag);
					alldata_et_HBHE->Fill(dataEt);
					allemul_et_HBHE->Fill(emulEt);
					data_FG_HBHE->Fill(dataFG);
					emul_FG_HBHE->Fill(emulFG);
					dataSOI_HBHE->Fill(data->presamples());
					emulSOI_HBHE->Fill(emul->presamples());
					if(dataFG) data_FGet_HBHE->Fill(dataEt);
					if(emulFG) emul_FGet_HBHE->Fill(emulEt);
					switch (errflag){
						case ErrFlag_Matched:
							etcorr_HBHE->Fill(dataEt,emulEt);
							etcorr2_HBHE->Fill(dataEt,dataEt-emulEt);
							break;
						case ErrFlag_MismatchedE:
							etcorr_HBHE->Fill(dataEt,emulEt);
							etcorr2_HBHE->Fill(dataEt,dataEt-emulEt);
							mismatch_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
						case ErrFlag_DataOnly:
							dataonly_et_HBHE->Fill(dataEt);
							dataonly_map->Fill(data->id().ieta(),data->id().iphi());
							break;
						case ErrFlag_EmulOnly:
							emulonly_et_HBHE->Fill(emulEt);
							emulonly_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
						case ErrFlag_MismatchedFG:
							etcorr_HBHE->Fill(dataEt,emulEt);
							etcorr2_HBHE->Fill(dataEt,dataEt-emulEt);
							mismatch_FG_map->Fill(emul->id().ieta(),emul->id().iphi());
							break;
                                    case ErrFlag_ZS:
                                          emul_ZS_map->Fill(emul->id().ieta(),emul->id().iphi());
                                    default:
                                          break;
					}
				}
			}
		}
	}
	FrontEndErrors.clear();
}


// ------------ method called once each job just before starting event loop  ------------
void 
HcalTPGValidation::beginJob(const edm::EventSetup& eventSetup)
{
	edm::ESHandle<HcalDbService> pSetup;
	eventSetup.get<HcalDbRecord>().get( pSetup );
	emap = pSetup->getHcalMapping();
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HcalTPGValidation::endJob() {
	//std::cout << std::flush;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HcalTPGValidation);
