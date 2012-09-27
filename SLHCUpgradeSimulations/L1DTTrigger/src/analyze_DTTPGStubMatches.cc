// Package:    DTL1SimOperations
// Class:      DTL1SimOperations
//
// Original Author:  Ignazio Lazzizzera
//         Created:  Thu Jul 30 11:56:13 CEST 2009
//
//
#include <algorithm>
#include <vector>

#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1SimOperations.h"
#include "SimDataFormats/SLHC/interface/DTSeededStubTrack.h"

// 6/5/2010 PLZ : include to get DTTF and GEN info
//begin
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Common/interface/Handle.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "L1Trigger/GlobalMuonTrigger/test/L1MuGMTDump.h"
//end

void  DTL1SimOperations::analyze_DTTPGStubMatches(edm::Event& event, 
						  const edm::EventSetup& eventSetup, 
						  string &theRootFileNameHis)
// ************************************************************
// *** 090320 SV find DT primitives - Tracker Stubs matches ***
// ************************************************************
{

  if(theRootFileNameHis == "")
    return;

  // PLZ begin
  //   cout << endl;
  float etagen = NAN;
  float phigen = NAN;
  float ptgen  = NAN;
  
 int ptcode[140] = { 0,0,0,1,2,3,4,5,0,6,
                     0,7,0,8,0,0,9,0,0,10,
                     0,0,0,0,11,0,0,0,0,12,
                     0,0,0,0,13,0,0,0,0,14,
                     0,0,0,0,0,0,0,0,0,15,
                     0,0,0,0,0,0,0,0,0,16,
                     0,0,0,0,0,0,0,0,0,17,
                     0,0,0,0,0,0,0,0,0,18,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,19,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,20,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,21 };
  
  //    edm::Handle<edm::SimVertexContainer> simvertices_handle;
  //    event.getByLabel("g4SimHits",simvertices_handle);
  //    edm::SimVertexContainer const* simvertices = simvertices_handle.product();
  
  edm::Handle<edm::SimTrackContainer> simtracks_handle;
  event.getByLabel("g4SimHits",simtracks_handle);
  edm::SimTrackContainer const* simtracks = simtracks_handle.product();
    
  edm::SimTrackContainer::const_iterator isimtr;
  int igen = 0;
  for(isimtr = simtracks->begin(); isimtr != simtracks->end(); isimtr++) {
    if(abs((*isimtr).type()) == 13) {
      etagen = (*isimtr).momentum().eta(); 
      phigen = (*isimtr).momentum().phi();
      if(phigen < 0) phigen = 2*TMath::Pi() + phigen;
      ptgen = (*isimtr).momentum().pt();   
      float thetagen = 2.*atan(exp(-etagen));
      if(theAsciiFileName != string("")) {
	outAscii << igen << " " << (*isimtr).type() << " phi " 
		 << phigen << " theta " << thetagen << " ptgen " << ptgen << endl;  
	if(phigen > 1.3 && phigen < 1.4) outAscii << "Questo evento" << endl;
      }
      igen++;
    }
  }  
  
  // float thetagen = 2.*atan(exp(-etagen));
  eta_gen->Fill(etagen);
  if(fabs(etagen) < 1)  {
    phi_gen->Fill(phigen);
    eff_counters-> Fill(10);
  }
  else 
    return; 

  // Get GMTReadoutCollection **************************************************
  edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
  event.getByLabel("simGmtDigis", gmtrc_handle);
  L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();
  if(theAsciiFileName != string("")) 
    outAscii << " 1 GMT  entered " << endl;
  int idt = 0;
  float pt_DTTF = 0.;
  vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
  vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
  vector<L1MuRegionalCand>::const_iterator iter1;
  vector<L1MuRegionalCand> rmc;
  for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) 
    {  
      // get DTBX trigger candidates
      rmc = igmtrr->getDTBXCands();
      // look for valid DTBX trigger candidates
      for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
	if ( !(*iter1).empty() ) { 
	  int bxd = (*iter1).bx();
	  float ptd = (*iter1).ptValue();
	  // ptind = (*iter1).ptIndex();
	  if(theAsciiFileName != string("")) 
	    outAscii << " GMT " << bxd << " " << ptd  << endl;
	  if(bxd == 0 && pt_DTTF < ptd) {
	    idt++;
	    pt_DTTF = ptd;
	  }
	} // end if DTBX trigger candidates is valid
      } // end of loop over  DTBX trigger candidates
    } // end of loop over GMT records
  if(theAsciiFileName != string("")) 
    outAscii <<" number of DTTF triggers " << idt << endl; 
  //    if (idt > 1) return;
  if(idt > 0) eff_counters->Fill(14); 
  if(idt > 1) eff_counters->Fill(15); 
  if (pt_DTTF == 45.) pt_DTTF =50.;
  if (pt_DTTF == 90.) pt_DTTF =100.;
  
  int pt_code = 0; 
  if(pt_DTTF >0) pt_code = ptcode[static_cast<int>(pt_DTTF-1)]; 
    outAscii <<" number of DTTF triggers " << pt_DTTF << " " <<  pt_code << endl; 
  
  if(idt > 0 && fabs(etagen) < 1) ptval_DTTF->Fill(pt_DTTF);
	for (int i = 0 ; i <= pt_code; i++){
		ptcut_DTTF->Fill(i);
	}   
  
  // count DTMatches *******************************************************
  int numDTOK = 0;
  for (int dm = 0; dm < DTMatches->numDt(); dm++) {  
    if(theAsciiFileName != string("")) 
      outAscii << " bx " <<  DTMatches->dtmatch(dm)-> bx() 
	       << " rj " <<  DTMatches->dtmatch(dm)-> flagReject() << endl; 
    if(	DTMatches->dtmatch(dm)->flagBxOK() 
	&& ! DTMatches->dtmatch(dm)->flagReject() ) 
      numDTOK++;
  }
  if (numDTOK > 0) {
    ndt_match_BXrj -> Fill(numDTOK);
    eff_counters -> Fill(11);
  }
  
  // select best DTMatch *************************************************** 
  int b_code = 0 ;
  int b_nstubs = 0;
  float b_pt = 0;
  int best_dm = -1; 
  for(int dm = 0; dm < DTMatches->numDt(); dm++) { 
    if( DTMatches->dtmatch(dm)->flagPt() ||  DTMatches->dtmatch(dm)->flagPtTracklet()) {
      if(theAsciiFileName != string("")) 
	outAscii << " dm " << dm << " code "<<DTMatches->dtmatch(dm)->code() 
		  << " pt from stubs " << DTMatches->dtmatch(dm)->Pt_majority_bin() 
		  << " pt from tracklets " << DTMatches->dtmatch(dm)->Pt_average_bin_Tracklet() 
		  << " pt from L1tracks " << DTMatches->dtmatch(dm)->Pt_matching_track() 
		  << endl;
    }
    if(	DTMatches->dtmatch(dm)->flagBxOK() 
	&& !DTMatches->dtmatch(dm)->flagReject() 
	&& DTMatches->dtmatch(dm)->flagPt() ) {
      float phi_0	=  DTMatches->dtmatch(dm)->predPhi(0)/4096.;
      float s_phi_0	=  DTMatches->dtmatch(dm)->predSigmaPhi(0)*5./4096.;
      float dphi = fabs(phigen - phi_0);
      if(theAsciiFileName != string("")) 
	outAscii << dphi << " " << s_phi_0 <<endl;	
      if(dphi < s_phi_0) { 
	if(DTMatches->dtmatch(dm)->code() > b_code) {
	  b_code = DTMatches->dtmatch(dm)->code();
	  best_dm = dm;
	  if(theAsciiFileName != string("")) 
	    outAscii << " DTMatches " << DTMatches->numDt() 
		     << " best " << best_dm << endl;	  
	  for(int lay = 0; lay<6; lay++) {
	    // int dtphi    = DTMatches->dtmatch(best_dm)->predPhi(lay);
	    // int dttheta  = DTMatches->dtmatch(best_dm)->predTheta();
	    // int sdtphi   = DTMatches->dtmatch(best_dm)->predSigmaPhi(lay);
	    int sdttheta = DTMatches->dtmatch(best_dm)->predSigmaTheta(lay);
	    if(!DTMatches->dtmatch(best_dm)->flagTheta()) 
	      sdttheta = sdttheta + DTMatches->dtmatch(best_dm)->deltaTheta();	 
	    // int nstubss = 
	    //  DTMatches->nstubsInWindow(dtphi,dttheta,sdtphi,sdttheta,lay);
	  } // end of loop over layers
	  
	  int nmatchedlays = 0;
	  // for(int lay = 0; lay<StackedLayersInUseTotal; lay++) {
	  for(int lay = 0; lay<4; lay++) {
	    if(DTMatches->dtmatch(dm)->isMatched(lay)) nmatchedlays++;
	  }
	  if(nmatchedlays > b_nstubs){
	    b_nstubs = nmatchedlays;
	    best_dm = dm;
	    if(theAsciiFileName != string("")) 	
	      outAscii << " DTMatches " << DTMatches->numDt() 
		       << " best " << best_dm << endl;	    
	    float Ptbin = DTMatches->dtmatch(dm)->Pt_average_bin();
	    if(Ptbin > b_pt){
	      b_pt = Ptbin;
	      best_dm = dm;
	      if(theAsciiFileName != string("")) 
		outAscii << " DTMatches " << DTMatches->numDt() 
			 << " best " << best_dm << endl;
	    }
	  }
	} // end if (DTMatches->dtmatch(dm)->code() > b_code
      } // end if phigen - phi_0 < s_phi_0
    } // end if flagBxOK() && !flagReject() && flagPt() )
  } // end of loop over DTMatches
  
/* int ptcode[140] = { 0,0,0,1,2,3,4,5,0,6,
                     0,7,0,8,0,0,9,0,0,10,
                     0,0,0,0,11,0,0,0,0,12,
                     0,0,0,0,13,0,0,0,0,14,
                     0,0,0,0,0,0,0,0,0,15,
                     0,0,0,0,0,0,0,0,0,16,
                     0,0,0,0,0,0,0,0,0,17,
                     0,0,0,0,0,0,0,0,0,18,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,19,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,20,
                     0,0,0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0,0,21 };*/
 
  pt_code = 0;
 
 if(theAsciiFileName != string("")) 
   outAscii << " DTMatches " << DTMatches->numDt() 
	    << " best " << best_dm << endl;
 
 if(best_dm >= 0) {   
   	  if (DTMatches->dtmatch(best_dm)-> code() > 15) {
//   if (DTMatches->dtmatch(best_dm)-> code() >= 0) {
     float phi_mu	=  DTMatches->dtmatch(best_dm)->phiCMS();
     float phib_mu	=  DTMatches->dtmatch(best_dm)->phib_ts();
     float phi_vx	=  (DTMatches->dtmatch(best_dm)->predPhiVx())/4096.;
     float d_phi_vx	=  (DTMatches->dtmatch(best_dm)->predSigmaPhiVx())/4096.;
     float rho_mu	=  DTMatches->dtmatch(best_dm)->rho();
     //	  float st = DTMatches->dtmatch(best_dm)->station() ;
     int wh = DTMatches->dtmatch(best_dm)->wheel()+ 2 ;
     //	  float co = DTMatches->dtmatch(best_dm)->code() ;
     if(DTMatches->dtmatch(best_dm)->station() == 1) 
       phi_rho_ST1 -> Fill(phi_mu,rho_mu);
     if(DTMatches->dtmatch(best_dm)->station() == 2) 
       phi_rho_ST2 -> Fill(phi_mu,rho_mu);
       float d_phi_mu = phi_mu-phigen ;
     if(DTMatches->dtmatch(best_dm)->station() == 1) {
       dphigen1_[wh] -> Fill(d_phi_mu);
       phib_1_[wh] -> Fill(phib_mu);
       }
     if(DTMatches->dtmatch(best_dm)->station() == 2) { 
       dphigen2_[wh] -> Fill(d_phi_mu);
       phib_2_[wh] -> Fill(phib_mu);
       }
       float ns = (phi_vx-phigen)/d_phi_vx;
       outAscii << phigen << " " << phi_vx << " nsigmas " << ns << endl;
   }
   float Ptbin_encoder = DTMatches->dtmatch(best_dm)->Pt_encoder_bin();
   float Ptbin_average = DTMatches->dtmatch(best_dm)->Pt_average_bin(); 
   float invPtvalue = 1./DTMatches->dtmatch(best_dm)->Pt_average(); 
   float Ptbin_majority = DTMatches->dtmatch(best_dm)->Pt_majority_bin();  
	 float Ptbin_majority_Full = DTMatches->dtmatch(best_dm)->Pt_majority_bin_Full(); 
	 float Ptbin_mixedmode = DTMatches->dtmatch(best_dm)->Pt_mixedmode_bin(); 
	 //	  float PT1 = DTMatches->dtmatch(best_dm)->Pt(string("Stubs_9_3_0"));
   float PT_15 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_9_8"));
   float PT_14 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_9_3"));
   float PT_13 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_9_2"));
   float PT_12 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_9_1"));
   float PT_11 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_9_0"));
   float PT_10 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_8_3"));
   float PT_9 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_8_2"));
   float PT_8 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_8_1"));
   float PT_7 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_8_0"));
   float PT_6 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_3_2"));
   float PT_5 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_3_1"));
   float PT_4 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_3_0"));
   float PT_3 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_2_1"));
   float PT_2 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_2_0"));
   float PT_1 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_1_0"));
   float Phib_best = DTMatches->dtmatch(best_dm)->phib_ts();
   float Code = DTMatches->dtmatch(best_dm)->code();
	 if(theAsciiFileName != string("")) {
	 outAscii << " Mu-9-8 "<< PT_15 << " Mu-9-3 " << PT_14  << " Mu-9-2 " << PT_13 << " Mu-9-1 " << PT_12 << " Mu-9-0 " << PT_11 << endl;
	 outAscii << " Mu-8-3 " << PT_10 << " Mu-8-2 " << PT_9 << " Mu-8-1 " << PT_8 << " Mu-8-0 " << PT_7 << endl;
     outAscii << " Mu-3-2 " << PT_6 << " Mu-3-1 " << PT_5 << " Mu-3-0 " << PT_4 << endl;
     outAscii << " Mu-2-1 " << PT_3 << " Mu-2-0 " << PT_2 << " Mu-1-0 " << PT_1 << endl;
     outAscii << " PT: priority encoder " << Ptbin_encoder
	      << " Average " << Ptbin_average << " Majority:inner " << Ptbin_majority 
		  << " MixedMode:inner " << Ptbin_mixedmode << " Majority:Full " << Ptbin_majority_Full 
	   //   << " phib " << Phib_best 
       // << " Stubs " << PT1 << " MU1 " << PT2 << " MU2 " << PT3 
	      << endl;
   }
   float Ptbin = Ptbin_majority;
   if(DTMatches->dtmatch(best_dm)->station() == 1) {
     PT_match1 -> Fill(PT_2);
     PT_match1 -> Fill(PT_3);
     PT_match1 -> Fill(PT_4);
     PT_match1 -> Fill(PT_5);
     invPT_match1 -> Fill(1./PT_2);
     invPT_match1 -> Fill(1./PT_3);
     invPT_match1 -> Fill(1./PT_4);
     invPT_match1 -> Fill(1./PT_5);
     //	  	invPT_match1 -> Fill(1./PT_1);
     //	  	invPT_match1 -> Fill(1./PT_6);
   }
   if(DTMatches->dtmatch(best_dm)->station() == 2) {
     PT_match2 -> Fill(PT_2);
     PT_match2 -> Fill(PT_3);
     PT_match2 -> Fill(PT_4);
     PT_match2 -> Fill(PT_5);
     invPT_match2 -> Fill(1./PT_2);
     invPT_match2 -> Fill(1./PT_3);
     invPT_match2 -> Fill(1./PT_4);
     invPT_match2 -> Fill(1./PT_5);
     //	  	invPT_match2 -> Fill(1./PT_1);
     //	  	invPT_match2 -> Fill(1./PT_6);
   }
   PT_match -> Fill(1./invPtvalue);
   invPT_match -> Fill(invPtvalue);
   ptval_encoder->Fill(Ptbin_encoder);
   ptval_average->Fill(Ptbin_average);
	 ptval_majority->Fill(Ptbin_majority);
	 ptval_mixedmode->Fill(Ptbin_mixedmode);
   ptval_majority_Full->Fill(Ptbin_majority_Full);
   pt_code = 0 ;
   if(Ptbin >0)  
   pt_code = ptcode[static_cast<int>(Ptbin-1)];
   ptval_phib->Fill(Phib_best,Ptbin);
   ptval_code->Fill(Code,Ptbin);
 
   pt_code = 0 ;
   if(Ptbin_encoder >0)  
   pt_code = ptcode[static_cast<int>(Ptbin_encoder-1)];
   for (int i = 0 ; i <= pt_code; i++){
     //	      if(DTMatches->dtmatch(best_dm)->station() == 1)  ptcut_ST1->Fill(i);
     //	      if(DTMatches->dtmatch(best_dm)->station() == 2)  ptcut_ST2->Fill(i);
     ptcut_encoder->Fill(i);
   }
   pt_code = 0 ;
   if(Ptbin_average >0) pt_code = ptcode[static_cast<int>(Ptbin_average-1)];
   for (int i = 0 ; i <= pt_code; i++){
     ptcut_average->Fill(i);
   }
   pt_code = 0 ;
   if(Ptbin_majority >0)
   pt_code = ptcode[static_cast<int>(Ptbin_majority-1)];
   for (int i = 0 ; i <= pt_code; i++){
     ptcut_majority->Fill(i);
   } 
	 pt_code = 0 ;
	 if(Ptbin_mixedmode >0)
		 pt_code = ptcode[static_cast<int>(Ptbin_mixedmode-1)];
	 for (int i = 0 ; i <= pt_code; i++){
		 ptcut_mixedmode->Fill(i);
	 } 
   pt_code = 0 ;
   if(Ptbin_majority_Full >0)
   pt_code = ptcode[static_cast<int>(Ptbin_majority_Full-1)];
   if(pt_code > 21) outAscii << " Warning code = " << pt_code << endl; 
   for (int i = 0 ; i <= pt_code; i++){
	 ptcut_majority_Full->Fill(i);
   } 
	 
   float Ptbin_average_Tracklet = DTMatches->dtmatch(best_dm)->Pt_average_bin_Tracklet();	 
	 float Ptbin_majority_Tracklet = DTMatches->dtmatch(best_dm)->Pt_majority_bin_Tracklet(); 
	 float Ptbin_mixedmode_Tracklet = DTMatches->dtmatch(best_dm)->Pt_mixedmode_bin_Tracklet();

   float PT_Tracklet_1 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_SL0_SL1"));
   float PT_Tracklet_2 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_SL0_SL4"));
   float PT_Tracklet_3 = DTMatches->dtmatch(best_dm)->Pt(string("Mu_SL1_SL4"));
   
   outAscii << " PT from Tracklets " << " Mu_SL0_SL1 " << PT_Tracklet_1
                                     << " Mu_SL0_SL4 " << PT_Tracklet_2
	                                 << " Mu_SL1_SL4 " << PT_Tracklet_3 << endl;
	 
   outAscii << "                   " << " PT bin average " << Ptbin_average_Tracklet
	                                 << " PT bin majority " << Ptbin_majority_Tracklet
	                                 << " PT bin mixedmode " << Ptbin_mixedmode_Tracklet
	                             << endl;
   
	 
   ptval_average_tracklet->Fill(Ptbin_average_Tracklet);
	 ptval_majority_tracklet->Fill(Ptbin_majority_Tracklet);
	 ptval_mixedmode_tracklet->Fill(Ptbin_mixedmode_Tracklet);
   pt_code = 0 ;
   if(Ptbin_average_Tracklet >0) pt_code = ptcode[static_cast<int>(Ptbin_average_Tracklet-1)];
   for (int i = 0 ; i <= pt_code; i++){
 	 ptcut_average_tracklet->Fill(i);
   }
   pt_code = 0 ;
   if(Ptbin_majority_Tracklet >0) pt_code = ptcode[static_cast<int>(Ptbin_majority_Tracklet-1)];
   for (int i = 0 ; i <= pt_code; i++){
 	 ptcut_majority_tracklet->Fill(i);
   }
	 pt_code = 0 ;
	 if(Ptbin_mixedmode_Tracklet >0) pt_code = ptcode[static_cast<int>(Ptbin_mixedmode_Tracklet-1)];
	 for (int i = 0 ; i <= pt_code; i++){
		 ptcut_mixedmode_tracklet->Fill(i);
	 }
 // PT of best L1 matching track
    
     float Pt_best_L1Track = DTMatches->dtmatch(best_dm)->Pt_matching_track(); 
     if(Pt_best_L1Track<10000) { 
      eff_counters -> Fill(8);
       invPT_track -> Fill(1./Pt_best_L1Track) ;
       float Ptbin_best_L1Track = DTMatches->dtmatch(best_dm)->Pt_matching_track_bin(); 
 //    if(Pt_best_L1Track<10000)invPT_track -> Fill(1./Pt_best_L1Track) ;
    outAscii << " Best L1Track PT " << Pt_best_L1Track << " bin "<< Ptbin_best_L1Track << endl;
       ptval_L1_track->Fill(Ptbin_best_L1Track);
       pt_code = 0 ;
//       if(Ptbin_best_L1Track > 0) pt_code = ptcode[static_cast<int>(Ptbin_best_L1Track-1)];
       pt_code = Ptbin_best_L1Track;
       for (int i = 0 ; i <= pt_code; i++){
    	  ptcut_L1_track->Fill(i);
       }
      }
	 
   eff_counters -> Fill(6);
   if(Ptbin_average_Tracklet >0)  eff_counters -> Fill(7);
   
   int nstubs[6] = { 0,0,0,0,0,0 };
   int nstubs_tot = 0;   
   for(int lay = 0; lay<6; lay++) {
     int dtphi    = DTMatches->dtmatch(best_dm)->predPhi(lay);
     int dttheta  = DTMatches->dtmatch(best_dm)->predTheta();
     int sdtphi   = DTMatches->dtmatch(best_dm)->predSigmaPhi(lay);
     int sdttheta = DTMatches->dtmatch(best_dm)->predSigmaTheta(lay);
     if(!DTMatches->dtmatch(best_dm)->flagTheta()) 
       sdttheta = sdttheta + DTMatches->dtmatch(best_dm)->deltaTheta();	 
     nstubs[lay] = DTMatches->nstubsInWindow(dtphi,dttheta,sdtphi,sdttheta,lay);
     nstubs_tot =  nstubs_tot+nstubs[lay];
   }
   if(theAsciiFileName != string(""))     outAscii << "nstubs in window "
	      << nstubs[0] << " " << nstubs[1] << " " << nstubs[2] << " " 
	      << nstubs[3] << " " << nstubs[4] << " " << nstubs[5] << endl;
   if(Ptbin > 0 ) ptbinvsnstubs -> Fill(nstubs_tot,ptcode[static_cast<int>(Ptbin-1)]);   
   for(int lay = 0; lay<6; lay++) {n_stubs[lay]-> Fill(nstubs[lay]);}
   
      
   int ntracklets[3] = { 0,0,0};
   int ntracklets_tot = 0;   
   for(int slay = 0; slay<3; slay++) {
     int lay = slay*2+1;
     int dtphi    = DTMatches->dtmatch(best_dm)->predPhi(lay);
     int dttheta  = DTMatches->dtmatch(best_dm)->predTheta();
     int sdtphi   = DTMatches->dtmatch(best_dm)->predSigmaPhi(lay);
     int sdttheta = DTMatches->dtmatch(best_dm)->predSigmaTheta(lay);
     if(!DTMatches->dtmatch(best_dm)->flagTheta()) 
       sdttheta = sdttheta + DTMatches->dtmatch(best_dm)->deltaTheta();	 
     ntracklets[slay] = DTMatches->ntrackletsInWindow(dtphi,dttheta,sdtphi,sdttheta,slay);
     ntracklets_tot =  ntracklets_tot+ntracklets[slay];
   }
   if(theAsciiFileName != string(""))
     outAscii << "ntracklets in window "
	      << ntracklets[0] << " " << ntracklets[1] << " " << ntracklets[2] << endl;  
   for(int slay = 0; slay<3; slay++) {n_tracklets[slay]-> Fill(ntracklets[slay]);}
 
     int dtphi    = DTMatches->dtmatch(best_dm)->predPhiVx();
     int dttheta  = DTMatches->dtmatch(best_dm)->predTheta();
     int sdtphi   = DTMatches->dtmatch(best_dm)->predSigmaPhiVx();
     int sdttheta = DTMatches->dtmatch(best_dm)->predSigmaThetaVx();
     if(!DTMatches->dtmatch(best_dm)->flagTheta()) 
       sdttheta = sdttheta + DTMatches->dtmatch(best_dm)->deltaTheta();	 
     int ntracks = DTMatches->ntracksInWindow(dtphi,dttheta,sdtphi,sdttheta);
     n_tracks-> Fill(ntracks);
     
   if(n_tracks >0){
     eff_counters -> Fill(9); 
     }
     
   if(theAsciiFileName != string(""))
     outAscii << "ntracks in window "<< ntracks << endl;  
//   for(int slay = 0; slay<3; slay++) {n_tracklets[slay]-> Fill(ntracklets[slay]);}
 }

}


