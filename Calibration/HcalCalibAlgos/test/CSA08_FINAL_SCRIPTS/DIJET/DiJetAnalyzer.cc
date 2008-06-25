#include "Calibration/HcalCalibAlgos/interface/DiJetAnalyzer.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "Calibration/Tools/interface/GenericMinL3Algorithm.h"
#include "CondFormats/DataRecord/interface/HcalRespCorrsRcd.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"

#include "FWCore/Utilities/interface/Exception.h"
#include <fstream>

namespace cms
{
DiJetAnalyzer::DiJetAnalyzer(const edm::ParameterSet& iConfig)
{
  jets_=iConfig.getParameter<edm::InputTag>("jetsInput");
  ec_=iConfig.getParameter<edm::InputTag>("ecInput");
  hbhe_=iConfig.getParameter<edm::InputTag>("hbheInput");
  ho_=iConfig.getParameter<edm::InputTag>("hoInput");
  hf_=iConfig.getParameter<edm::InputTag>("hfInput");

  eta_1 = iConfig.getParameter<double>("eta_1");
  eta_2 = iConfig.getParameter<double>("eta_2");
  jet_R = iConfig.getParameter<double>("jet_R");
  et_threshold = iConfig.getParameter<double>("et_threshold");
  et_veto = iConfig.getParameter<double>("et_veto"); 
  m_histoFlag = iConfig.getUntrackedParameter<int>("histoFlag",0);


  // get name of output file with histogramms
  fOutputFileName = iConfig.getUntrackedParameter<std::string>("HistOutFile");

   allowMissingInputs_ = true;
}


DiJetAnalyzer::~DiJetAnalyzer()
{


}


//
// member functions
//

// ------------ method called to for each event  ------------
void
DiJetAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   nHits = 0; 
   using namespace edm;
   using namespace reco; 

   const double pi = 4.*atan(1.);

   CaloJet jet1, jet2, jet3; 
   try {
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(jets_, jets);
   if(jets->size()>1){ 
     jet1 = (*jets)[0]; 
     jet2 = (*jets)[1];
     if(fabs(jet1.eta())>fabs(jet2.eta())){
       CaloJet jet = jet1; 
       jet1 = jet2; 
       jet2 = jet; 
     } 
     if(fabs(jet1.eta())>eta_1 || (fabs(jet2.eta())-jet_R) < eta_2){ return;}
     if(jets->size()==3){jet3 = (*jets)[2];}
   } else {return;}

   }catch (cms::Exception& e) { // can't find it!
     if (!allowMissingInputs_) { throw e; }  
   }

   if(jet3.et()>et_veto || jet1.et()<et_threshold){return;}


     et_jet_centr = jet1.et(); 
     eta_jet_centr = jet1.eta(); 
     phi_jet_centr = jet1.phi();

     e_jet_forw = jet2.energy(); 
     et_jet_forw = jet2.et(); 
     eta_jet_forw = jet2.eta(); 
     phi_jet_forw = jet2.phi();

     et_jet_add = jet3.et(); 

   std::map<DetId,float> DetEMap;
   std::map<tower_id,float> DetTEMap; 

   int nH = 0; 
   float e_em_in_forw_jet = 0; 
   float e_had_in_forw_jet = 0; 
   try {
      Handle<EcalRecHitCollection> ec;
      iEvent.getByLabel(ec_,ec);
      for(EcalRecHitCollection::const_iterator ecItr = (*ec).begin();
                                                ecItr != (*ec).end(); ++ecItr)
      {
       DetId id = ecItr->detid();
       GlobalPoint pos = geo->getPosition(id);
       double ehit = ecItr->energy();
       double ethit = ehit*sin(pos.theta());
       double etahit = pos.eta();
       double phihit = pos.phi();
       double deta = fabs(jet2.eta()-etahit);
       double dphi = fabs(jet2.phi()-phihit);
       if(dphi>pi){dphi = 2*pi-dphi;}
       double dR = sqrt(pow(deta,2)+pow(dphi,2));

       if(dR<jet_R && ethit>0.5){DetEMap[id] = ecItr->energy(); e_em_in_forw_jet += ecItr->energy();
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) throw e;
   }
  

    float EJet = jet1.et()/sin(jet2.theta()) - e_em_in_forw_jet; 

   try {
      Handle<HBHERecHitCollection> hbhe;
      iEvent.getByLabel(hbhe_, hbhe);
      for(HBHERecHitCollection::const_iterator hbheItr=hbhe->begin(); 
                                                 hbheItr!=hbhe->end(); hbheItr++)
      {
       DetId id = hbheItr->detid(); 
       GlobalPoint pos = geo->getPosition(id);
       double ehit = hbheItr->energy();
       double ethit = ehit*sin(pos.theta());
       double etahit = pos.eta();
       double phihit = pos.phi();
       double deta = fabs(jet2.eta()-etahit); 
       double dphi = fabs(jet2.phi()-phihit); 
       if(dphi>pi){dphi = 2*pi-dphi;}
       double dR = sqrt(pow(deta,2)+pow(dphi,2)); 

       if(dR<jet_R && ethit>0.5){
       tower_id  tower_num = TowHitMap[id]; DetTEMap[tower_num] += hbheItr->energy(); 
       DetEMap[id] = hbheItr->energy(); e_had_in_forw_jet += hbheItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) throw e;
   }

   try {
      Handle<HORecHitCollection> ho;
      iEvent.getByLabel(ho_, ho);
      for(HORecHitCollection::const_iterator hoItr=ho->begin(); 
                                               hoItr!=ho->end(); hoItr++)
      {
       DetId id = hoItr->detid();
       GlobalPoint pos = geo->getPosition(id);
       double ehit = hoItr->energy();
       double ethit = ehit*sin(pos.theta());
       double etahit = pos.eta();
       double phihit = pos.phi();
       double deta = fabs(jet2.eta()-etahit);
       double dphi = fabs(jet2.phi()-phihit);
       if(dphi>pi){dphi = 2*pi-dphi;}
       double dR = sqrt(pow(deta,2)+pow(dphi,2));

       if(dR<jet_R && ethit>0.5){DetEMap[id] = hoItr->energy(); e_had_in_forw_jet += hoItr->energy(); 
       NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) {std::cout<<" No HO collection "<<std::endl; throw e;}
   }



   try {
      Handle<HFRecHitCollection> hf;
      iEvent.getByLabel(hf_, hf);
      for(HFRecHitCollection::const_iterator hfItr=hf->begin(); 
                                               hfItr!=hf->end(); hfItr++)
      {
       DetId id = hfItr->detid();
       GlobalPoint pos = geo->getPosition(id);
       double ehit = hfItr->energy();
       double ethit = ehit*sin(pos.theta());
       double etahit = pos.eta();
       double phihit = pos.phi();
       double deta = fabs(jet2.eta()-etahit);
       double dphi = fabs(jet2.phi()-phihit);
       if(dphi>pi){dphi = 2*pi-dphi;}
       double dR = sqrt(pow(deta,2)+pow(dphi,2));

       if(dR<jet_R && ethit>0.5){
       tower_id  tower_num = TowHitMap[id]; DetTEMap[tower_num] += hfItr->energy();
       DetEMap[id] = hfItr->energy(); e_had_in_forw_jet += hfItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) {std::cout<<" No HF collection "<<std::endl; throw e;}
   }


     float ratio_em_full = e_em_in_forw_jet/(e_had_in_forw_jet + e_em_in_forw_jet);
     e_jet_reco = e_had_in_forw_jet + e_em_in_forw_jet; 

     if(ratio_em_full>0.2){return;}
     energyVector.push_back(EJet);


   nTows = 0; 
   std::vector<float> en; 
   for(std::vector<tower_id>::const_iterator id=tow_vec.begin(); id != tow_vec.end(); id++)
   {
        en.push_back(DetTEMap[(*id)]); 
        if(DetTEMap[(*id)]>0.){
         towE[nTows] = DetTEMap[(*id)]; 
         towId[nTows] = (*id).towId; 
         nTows++;
        }
   }

   eventMatrix.push_back(en); 
   
  if(m_histoFlag==1){
   myTree->Fill();
  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
DiJetAnalyzer::beginJob(const edm::EventSetup& iSetup)
{

  hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

   myTree = new TTree("RecJet","RecJet Tree");


   myTree->Branch("et_jet_centr",  &et_jet_centr, "et_jet_centr/F");
   myTree->Branch("eta_jet_centr",  &eta_jet_centr, "eta_jet_centr/F");
   myTree->Branch("phi_jet_centr",  &phi_jet_centr, "phi_jet_centr/F");
   myTree->Branch("e_jet_forw",  &e_jet_forw, "e_jet_forw/F");       
   myTree->Branch("et_jet_forw",  &et_jet_forw, "et_jet_forw/F");
   myTree->Branch("eta_jet_forw",  &eta_jet_forw, "eta_jet_forw/F");
   myTree->Branch("phi_jet_forw",  &phi_jet_forw, "phi_jet_forw/F");
   myTree->Branch("et_jet_add",  &et_jet_add, "et_jet_add/F");
   myTree->Branch("e_jet_reco",  &e_jet_reco, "e_jet_reco/F");   

/*
   myTree->Branch("nHits", &nHits, "nHits/I");
   myTree->Branch("hitE",   hitE, "hitE[nHits]/F");
   myTree->Branch("hitEt",   hitEt, "hitEt[nHits]/F");
   myTree->Branch("hitEta",  hitEta, "hitEta[nHits]/F");
   myTree->Branch("hitPhi",  hitPhi, "hitPhi[nHits]/F");
*/

   myTree->Branch("nTows", &nTows, "nTows/I");
   myTree->Branch("towId",  towId, "towId[nTows]/I");
   myTree->Branch("towE",   towE, "towE[nTows]/F");


   edm::ESHandle<CaloGeometry> pG;
   iSetup.get<IdealGeometryRecord>().get(pG);
   geo = pG.product();

   int tow_num = 1; 
   std::vector<DetId> did = geo->getValidDetIds();
   for(std::vector<DetId>::const_iterator id=did.begin(); id != did.end(); id++)
   {
      if( (*id).det() == DetId::Hcal ) {
        GlobalPoint pos = geo->getPosition(*id); 
        double eta = pos.eta();
        if(fabs(eta)>eta_2){
       
        int flag_select = 0; 
        int ieta = HcalDetId(*id).ieta();
        int iphi = HcalDetId(*id).iphi();
        int idepth = HcalDetId(*id).depth();
        int mydet = ((*id).rawId()>>28)&0xF;
        int mysubd = ((*id).rawId()>>25)&0x7;   
       
 
        int towsize = tow_vec.size(); 
        for(int i=0; i<towsize; i++){
           int hitsize = tow_vec[i].hits.size(); 
           for(int j=0; j<hitsize; j++){
             DetId hid = tow_vec[i].hits[j];
             int heta = HcalDetId(hid).ieta();
             int hphi = HcalDetId(hid).iphi();
             int hdepth = HcalDetId(hid).depth();
             int hmydet = ((hid).rawId()>>28)&0xF;
             int hmysubd = ((hid).rawId()>>25)&0x7;               
             if(ieta==heta && iphi==hphi && mydet==hmydet &&
	       mysubd==hmysubd && idepth!=hdepth && flag_select==0){
  	       tow_vec[i].hits.push_back(*id);
               flag_select = 1;  
	     }  
           }
        }
       
       
        if(flag_select==0 && mysubd != 3){
          tower_id t;
          t.towId = tow_num;
          t.hits.push_back(*id); 
          tow_vec.push_back(t);  
          tow_num++; 
        }
	}

        if(fabs(eta)>eta_2){did_selected.push_back(*id);}

      }
   }

   for(int i=0; i<tow_vec.size(); i++){
     for(int j=0; j<tow_vec[i].hits.size(); j++){
       DetId hid = tow_vec[i].hits[j];
       TowHitMap[hid] = tow_vec[i]; 
     }
   }



   calibTree = new TTree("Calib","Calib"); 
   calibTree->Branch("col_size", &col_size, "col_size/I"); 
   calibTree->Branch("calibh", calibh, "calibh[col_size]/F"); 
   calibTree->Branch("etah", etah, "etah[col_size]/F"); 
   calibTree->Branch("phih", phih, "phih[col_size]/F"); 
   calibTree->Branch("depthh", depthh, "depthh[col_size]/F"); 
   calibTree->Branch("hitNum", hitNum, "hitNum[col_size]/F"); 


   edm::ESHandle<HcalRespCorrs> r;
   iSetup.get<HcalRespCorrsRcd>().get(r);
   oldRespCorrs = new HcalRespCorrs(*r.product());
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiJetAnalyzer::endJob() {

   
   col_size = 0; 

   nIter = 10; 
   GenericMinL3Algorithm* minL3 = new GenericMinL3Algorithm(); 
   HcalRespCorrs* mycorrections = new HcalRespCorrs();
   vector<float> calib = minL3->iterate(eventMatrix,energyVector,nIter);         
   for(unsigned i=0; i<calib.size(); i++){
       tower_id id = tow_vec[i];  
       int hitsize = id.hits.size(); 
       for(int j=0; j<hitsize; j++){
          DetId hid = id.hits[j];
          int heta = HcalDetId(hid).ieta();
          int hphi = HcalDetId(hid).iphi();
          int hdepth = HcalDetId(hid).depth();
          int mydet = ((hid).rawId()>>28)&0xF;
          int mysubd = ((hid).rawId()>>25)&0x7;
          HcalRespCorr item (hid.rawId(), calib[i]);
          mycorrections->addValues(item);

          calibh[col_size] = calib[i]; 
          etah[col_size] = (float) heta;
          phih[col_size] = (float) hphi; 
          depthh[col_size] = (float) hdepth; 
          hitNum[col_size] = (float) NDetEntries[hid]; 
          col_size++;  
       }
   }


   HcalRespCorrs* newRespCorrs = new HcalRespCorrs();
   std::vector<DetId> channels = oldRespCorrs->getAllChannels ();

   for (unsigned i = 0; i < channels.size(); i++) {
       DetId id = channels[i];
       float scale;
       if (mycorrections->exists(id) )
          scale = mycorrections->getValues(id)->getValue();
       else scale = 1.;
       HcalRespCorr item(id.rawId(),scale*oldRespCorrs->getValues(id)->getValue()); 
       newRespCorrs->addValues(item);
   }
   std::ostringstream file;
   file << "corrections.txt";
   std::ofstream outStream(file.str().c_str() );
   HcalDbASCIIIO::dumpObject (outStream, (*newRespCorrs) );


   calibTree->Fill();
   hOutputFile->SetCompressionLevel(2);
   hOutputFile->cd();
   myTree->Write();
   calibTree->Write(); 
   hOutputFile->Close() ;

}
}
