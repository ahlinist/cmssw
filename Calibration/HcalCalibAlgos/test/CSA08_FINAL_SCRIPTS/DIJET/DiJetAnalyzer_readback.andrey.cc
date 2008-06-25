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
//  jet_product = iConfig.getUntrackedParameter<std::string>("jetsProduct","DiJProd");
//  jets_ = iConfig.getUntrackedParameter<std::string>("jetsInput","DiJProd:DiJetsBackToBackCollection");
  jets_=iConfig.getParameter<edm::InputTag>("jetsInput");
  ec_=iConfig.getParameter<edm::InputTag>("ecInput");
  hbhe_=iConfig.getParameter<edm::InputTag>("hbheInput");
  ho_=iConfig.getParameter<edm::InputTag>("hoInput");
  hf_=iConfig.getParameter<edm::InputTag>("hfInput");


//  ec_=iConfig.getUntrackedParameter<std::string>("ecInput","DiJetsEcalRecHitCollection");
//  hbhe_=  iConfig.getUntrackedParameter<std::string>("hbheInput","DiJetsHBHERecHitCollection");
//  hf_=    iConfig.getUntrackedParameter<std::string>("hfInput","DiJetsHFRecHitCollection");
//  ho_ = iConfig.getUntrackedParameter<std::string>("hoInput","DiJetsHORecHitCollection");
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
//   iEvent.getByLabel(jet_product, jets_, jets);
   iEvent.getByLabel(jets_, jets);
//   cout << "JETSS SIZE=" << jets->size() << endl; 
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

//   if(jet3.et()>et_veto || jet1.et()<et_threshold){return;}

     et_jet_centr = jet1.et(); 
     eta_jet_centr = jet1.eta(); 
     phi_jet_centr = jet1.phi();

     et_jet_forw = jet2.et(); 
     eta_jet_forw = jet2.eta(); 
     phi_jet_forw = jet2.phi();
/*
             std::map<DetId,int> mapId; 
             int tow_num = 0; 
             vector<CaloTowerRef> vec1 = jet2.getConstituents();
             vector<CaloTowerRef>::const_iterator it_tow;
             cout << "Tower Collection Size=" << vec1.size() << endl; 
             for(it_tow = vec1.begin(); it_tow != vec1.end(); it_tow++){
               tow_num ++;
               double et_tow = (*it_tow)->et();
               double e_tow = (*it_tow)->energy();
               double em_tow = (*it_tow)->emEnergy();
               double had_tow = (*it_tow)->hadEnergy();
               double eta_tow = (*it_tow)->eta();
               double phi_tow = (*it_tow)->phi();
               cout << "i=" << tow_num << "et= " << et_tow << " e=" << e_tow << " em_tow=" << em_tow <<
                 " had_tow=" << had_tow <<
                 " eta=" << eta_tow << " phi=" << phi_tow << endl;
               size_t tower_size = (*it_tow)->constituentsSize();
               for(size_t i=0; i<tower_size; i++){
                 DetId ddid = (*it_tow)->constituent(i);
                 mapId[ddid] = 1; 
                 GlobalPoint position = geo->getPosition(ddid);
                 if(ddid.det()==3){
                   cout << "hit " << ddid.det() << " " << ddid.subdetId()
                        << "  " << position.eta() << " " << position.phi() << endl;
                 }
                 if(ddid.det()==4){
                   cout << "hit " << ddid.det() << " " << ddid.subdetId()
                        << "  " << position.eta() << " " << position.phi() << " " <<
                     HcalDetId(ddid).depth() << endl;
                 }

               }


             }
*/

   std::map<DetId,float> DetEMap;
   std::map<tower_id,float> DetTEMap; 

   int nH = 0; 
   float e_em_in_forw_jet = 0; 
   float e_had_in_forw_jet = 0; 
   try {
      Handle<EcalRecHitCollection> ec;
//      iEvent.getByLabel(jet_product, ec_,ec);
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
/*
       if(dR<jet_R && mapId[id]==0){nH++;}
       if(dR<jet_R &&  mapId[id]==1){DetEMap[id] = ecItr->energy(); e_em_in_forw_jet += ecItr->energy();
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}
*/
       if(dR<jet_R && ethit>0.5){DetEMap[id] = ecItr->energy(); e_em_in_forw_jet += ecItr->energy();
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) throw e;
   }
  

    float EJet = jet1.et()/sin(jet2.theta()) - e_em_in_forw_jet; 
//    if(EJet<0){return;}
//    energyVector.push_back(EJet);        

   try {
      Handle<HBHERecHitCollection> hbhe;
//      iEvent.getByLabel(jet_product, hbhe_, hbhe);
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
/*
       if(dR<jet_R  && mapId[id]==0){nH++;}
       if(dR<jet_R  && mapId[id]==1){DetEMap[id] = hbheItr->energy(); e_had_in_forw_jet += hbheItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}
*/
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
//      iEvent.getByLabel(jet_product, ho_, ho);
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
/*
       if(dR<jet_R && mapId[id]==0){nH++;}
       if(dR<jet_R && mapId[id]==1){DetEMap[id] = hoItr->energy(); e_had_in_forw_jet += hoItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}
*/
       if(dR<jet_R && ethit>0.5){DetEMap[id] = hoItr->energy(); e_had_in_forw_jet += hoItr->energy(); 
       NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) {std::cout<<" No HO collection "<<std::endl; throw e;}
   }



   try {
      Handle<HFRecHitCollection> hf;
//      iEvent.getByLabel(jet_product, hf_, hf);
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
/*
       if(dR<jet_R &&  mapId[id]==0){nH++;}
       if(dR<jet_R &&  mapId[id]==1){DetEMap[id] = hfItr->energy(); e_had_in_forw_jet += hfItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}
*/
       if(dR<jet_R && ethit>0.5){
       tower_id  tower_num = TowHitMap[id]; DetTEMap[tower_num] += hfItr->energy();
       DetEMap[id] = hfItr->energy(); e_had_in_forw_jet += hfItr->energy(); NDetEntries[id]++;
       hitE[nHits]=ehit; hitEt[nHits]=ethit; hitEta[nHits]=etahit; hitPhi[nHits]=phihit; nHits++;}

      }

   } catch (cms::Exception& e) { // can't find it!
      if (!allowMissingInputs_) {std::cout<<" No HF collection "<<std::endl; throw e;}
   }

//     cout << "nH=" << nH << " nHits=" << nHits << endl; 
     float ratio_em_full = e_em_in_forw_jet/(e_had_in_forw_jet + e_em_in_forw_jet);
//     cout << " RATIO = " << ratio_em_had << endl; 
//     if(ratio_em_full>0.2){return;}
//     energyVector.push_back(EJet);
     float joo; 
     if((EJet-e_em_in_forw_jet)/e_had_in_forw_jet>40){
       cout << EJet << " " << e_em_in_forw_jet << " " << e_had_in_forw_jet << endl; 
       for(int yy=0; yy<10000000; yy++){} 
     }

     /*
   std::vector<float> en; 
   for(std::vector<DetId>::const_iterator id=did_selected.begin(); id != did_selected.end(); id++)
   {
        en.push_back(DetEMap[(*id)]); 
   }
     */
/*
   std::vector<float> en; 
   for(std::vector<tower_id>::const_iterator id=tow_vec.begin(); id != tow_vec.end(); id++)
   {
        en.push_back(DetTEMap[(*id)]); 
   }

   eventMatrix.push_back(en); 
*/   
//  if(m_histoFlag==1){
//   myTree->Fill();
//  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
DiJetAnalyzer::beginJob(const edm::EventSetup& iSetup)
{

  hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

/*
   myTree = new TTree("RecJet","RecJet Tree");


   myTree->Branch("et_jet_centr",  &et_jet_centr, "et_jet_centr/F");
   myTree->Branch("eta_jet_centr",  &eta_jet_centr, "eta_jet_centr/F");
   myTree->Branch("phi_jet_centr",  &phi_jet_centr, "phi_jet_centr/F");
   myTree->Branch("et_jet_forw",  &et_jet_forw, "et_jet_forw/F");
   myTree->Branch("eta_jet_forw",  &eta_jet_forw, "eta_jet_forw/F");
   myTree->Branch("phi_jet_forw",  &phi_jet_forw, "phi_jet_forw/F");

   myTree->Branch("nHits", &nHits, "nHits/I");
   myTree->Branch("hitE",   hitE, "hitE[nHits]/F");
   myTree->Branch("hitEt",   hitEt, "hitEt[nHits]/F");
   myTree->Branch("hitEta",  hitEta, "hitEta[nHits]/F");
   myTree->Branch("hitPhi",  hitPhi, "hitPhi[nHits]/F");
*/

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


   /*
   for(int i=0; i<did_selected.size(); i++){
     DetId hid = did_selected[i]; 
     int heta = HcalDetId(hid).ieta();
     int hphi = HcalDetId(hid).iphi();
     int hdepth = HcalDetId(hid).depth();
     int hmydet = ((hid).rawId()>>28)&0xF;
     int hmysubd = ((hid).rawId()>>25)&0x7;    
     cout << heta << " " << hphi << " " << hdepth << " " << hmydet << 
       " " << hmysubd << " " << TowHitMap[hid].towId << endl;  
   }
   */

   /*
   for(int i=0; i<tow_vec.size(); i++){
     cout << " Tower Number = " << tow_vec[i].towId << endl; 
     for(int j=0; j<tow_vec[i].hits.size(); j++){
       DetId hid = tow_vec[i].hits[j]; 
       int heta = HcalDetId(hid).ieta();
       int hphi = HcalDetId(hid).iphi();
       int hdepth = HcalDetId(hid).depth();
       int mydet = ((hid).rawId()>>28)&0xF;
       int mysubd = ((hid).rawId()>>25)&0x7;
       //       if(heta==28){   
             cout << j << " " << heta << " " << hphi << " " << hdepth << 
   	    " " << mydet << " " << mysubd << endl; 
       //       }
     }
     cout << "----------------------------" << endl; 
   }
   */
   
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

 
  ifstream in("vector.txt");
  float cal_coef;
  vector<float> calib;
//  for(int i=0; i<tow_vec.size(); i++){calib[i]=1.}  
  while(in >> cal_coef){
   calib.push_back(cal_coef); 
   cout << "coef=" << cal_coef << endl;  
  }

 

   HcalRespCorrs* mycorrections = new HcalRespCorrs();


   for(unsigned i=0; i<calib.size(); i++){
       tower_id id = tow_vec[i];  
        cout << "id=" << id.towId << " " << calib[i] << endl; 
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

//       std::cout << "ieta=" << i_eta << " iphi=" << i_phi << " idepth=" << i_depth << 
//       " mydet=" << mydet << " mysubd=" << mysubd << " calib=" << calib[i] << "NEntries=" 
//       << NDetEntries[id] << std::endl;

          calibh[col_size] = calib[i]; 
          etah[col_size] = (float) heta;
          phih[col_size] = (float) hphi; 
          depthh[col_size] = (float) hdepth; 
          hitNum[col_size] = (float) NDetEntries[hid]; 
          std::cout << col_size << " " << calibh[col_size] << " " << etah[col_size] << " " << phih[col_size] << 
          hitNum[col_size] << std::endl; 
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
//   myTree->Write();
   calibTree->Write(); 
   hOutputFile->Close() ;

}
}
