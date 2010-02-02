#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "../interface/BsToJpsiPhiRootTree.h"
#include "VariableList.h"
#include "BsTreeSelector.h"

BsToJpsiPhiRootTree *tree=0;

// declare some functions. These are implemented at the end of this file
void increaseCountersByOne(int &BsJpsiPhiSignal, int& BsJpsiKKSignal, int& BdJpsiKstar, int& BsOther, int& BdOther,  int& other, 
			   int &BsJpsiEta, int &BdJpsiK10, int &BdJpsiK0, int &BpJpsiKp  );

void fillHistograms(double value, TH1F* hBsJpsiPhiSignal, TH1F* hBsOther, TH1F * hOther, TH1F *hBsJPsiKK = 0);
void fillHistograms(double value, vector<TH1F*> &histoVector);  // pass vector of histograms for the various categories


//void bookHistos();
void bookHistoVectors();
void writeHistos(TString outputfilename);
void identifyEventType(bool & isGenBsJpsiPhiMuMuKKEvent, bool &isGenBsJpsiKKEvent, bool &isGenBdJpsiKstarMuMuKpiEvent, 
		       bool &isGenBsEvent_, bool &isGenBdEvent_, bool &isGenBsJpsiEtaEvent_, bool &isGenBdJpsiK10Event_, 
		       bool &isGenBdJpsiK0Event_, bool & isGenBpJpsiKpEvent_);
void printDecayTree();
void makeVertexResolutionPlots();


// define cuts:
const double minProbability = 0.02;
const double KaonPtCut = 0.9;
const double BdKaonPtCut = 0.6;
const double nominalPhiMass = 1.0195 ;
const double nominalKstarMass = 0.896;
const double phiMassWindow = 0.007;
const double LowerBsMassCut = 5.25;
const double UpperBsMassCut = 5.65;
//const double phiMassWindow = 0.02;
const double KstarMassWindow = 0.05;
const double decayLengthCut = 3;
//const double decayLengthCut = -9999;
const double pointingCut = 0.8;
//const double pointingCut = -9999;

// maximum number of event to loop over. -1 means all events.
const int maxNumEvents = -1;
// number of categories
const int iNumberOfCategories_ = 10;

// switch to remove Jpsi events from ppMuMu sample
const bool bRemoveJpsiEvents = false;

// switch to write out selected trees for the fit
const bool bWriteSelectedTrees  = true;
const TString filenameSelectedTrees = "Selected";

// ***************main function*****************************
void BsAnalysisNew() {

 
  string treefilename = "/nfs/data6/alschmid/BsJpsiPhiRootTrees/BtoJpsiMuMu10TeV_22Jan10.root";
  TString outputhistofilename = "BtoJpsiMuMuHistoFile.root";
 
//   string treefilename = "JpsiMuMu_29Dec09.root";
//   TString outputhistofilename = "JpsiMuMuHistoFile.root";


  // booking histograms
  bookHistoVectors();  

  // message for  removal of Jpsi
  if(bRemoveJpsiEvents==1) std::cout<<"Removing Jpsi events (use it only for ppmuX samples) !!!!!!" << std::endl;
  
  // get tree from file
  tree = new BsToJpsiPhiRootTree;
  tree->readTree(treefilename);

  // initialize the selector if needed
  SelectorForFit * mySelector = 0;
  if(bWriteSelectedTrees) mySelector = new SelectorForFit(tree, filenameSelectedTrees);


  int nentries = tree->bsTree_->GetEntries();
  std::cout<<"Number of entries in tree = " << nentries << std::endl;

  int maxEvents;
  if(maxNumEvents < 0) maxEvents = nentries;
  else maxEvents = maxNumEvents;
  //***********************************
  // STARTING event loop here *********
  //***********************************
  int printedTree = 0;
  for(int i=0; i < maxEvents; i++){
 
    entrycounter++;
    if( (entrycounter % 10000) == 0) std::cout<<"processing entry number " << entrycounter << std::endl;
    tree->bsTree_->GetEntry(i);
    
    // ignore Jpsi events in case of ppMuMu
    if(bRemoveJpsiEvents) if(tree->isGenJpsiEvent_ == 1) continue;

    // identify event type
    identifyEventType(isGenBsJpsiPhiMuMuKKEvent_, isGenBsJpsiKKEvent_, isGenBdJpsiKstarMuMuKpiEvent_, isGenBsEvent_, isGenBdEvent_, isGenBsJpsiEtaEvent_, isGenBdJpsiK10Event_, 
		      isGenBdJpsiK0Event_, isGenBpJpsiKpEvent_);

    // print decay tree
    //    printDecayTree();

   
    // count number of events of each type
    increaseCountersByOne(iBsJPsiPhiSignalEvents, iBsJPsiKKSignalEvents, iBdJPsiKstarSignalEvents, iBsOtherEvents, iBdOtherEvents, iOtherEvents, 
			  iBsJpsiEtaEvents, iBdJpsiK10Events, iBdJpsiK0Events, iBpJpsiKpEvents);
    
    if(tree->triggerbit_HLTdoubleMu3_ != 1) continue;      // IS TRIGGERED_HLTdoubleMu3**************
    HLT++;
    
    // count number of triggered events of each type
    increaseCountersByOne(iTriggered_DoubleMu3_BsJPsiPhiSignalEvents,  iTriggered_DoubleMu3_BsJpsiKK, 
			  iTriggered_DoubleMu3_BdJpsiKstar,            iTriggered_DoubleMu3_BsOtherEvents, 
			  iTriggered_DoubleMu3_BdOtherEvents,          iTriggered_DoubleMu3_OtherEvents,
			  iTriggered_DoubleMu3_BsJpsiEtaEvents,        iTriggered_DoubleMu3_BdJpsiK10Events,
			  iTriggered_DoubleMu3_BdJpsiK0Events,         iTriggered_DoubleMu3_BpJpsiKpEvents);
    
    fillHistograms(tree->JpsiM_alone_,     vhJPsiMass); 
     
    //***********************************************
    // Bs analysis specific cuts start here.   Bd analysis is below.
    //***********************************************
    if(tree->iPassedCutIdent_ >= 7) {   // passed  pre KinFit mass cuts
      increaseCountersByOne(iBsJPsiPhiSignalEventsPreKinFit, iBsJPsiKKSignalEventsPreKinFit, iBdJPsiKstarSignalEventsPreKinFit,
			    iBsOtherEventsPreKinFit, iBdOtherEventsPreKinFit, iOtherEventsPreKinFit,
			    iBsJpsiEtaEventsPreKinFit, iBdJpsiK10EventsPreKinFit, iBdJpsiK0EventsPreKinFit, iBpJpsiKpEventsPreKinFit);
      
      
      if(tree->iPassedCutIdent_ >= 13) { // passed kinematic fit  
	increaseCountersByOne(iBsJPsiPhiSignalEventsOfflineSel1, iBsJPsiKKSignalEventsOfflineSel1, iBdJPsiKstarSignalEventsOfflineSel1,
			      iBsOtherEventsOfflineSel1, iBdOtherEventsOfflineSel1, iOtherEventsOfflineSel1,
			      iBsJpsiEtaEventsOfflineSel1, iBdJpsiK10EventsOfflineSel1, iBdJpsiK0EventsOfflineSel1, iBpJpsiKpEventsOfflineSel1);
	
	
	// successful fit, fill histograms

	fillHistograms(tree->BsFitChi2_,                     vhChi2);
	fillHistograms(tree->BsFitChi2_/ tree->BsFitNdof_ ,  vhChi2Ndof);
	fillHistograms(tree->BsFitVtxProb_,                  vhBsVtxProb );
	
	fillHistograms(tree->K1Pt_nofit_,         vhK1Pt);
	fillHistograms(tree->K2Pt_nofit_,         vhK2Pt);
	

	
	if(tree->BsFitVtxProb_ >= minProbability) { // passed minimum vertex probability cut
	  increaseCountersByOne(iBsJPsiPhiSignalEventsProbVertex, iBsJPsiKKSignalEventsProbVertex, iBdJPsiKstarSignalEventsProbVertex,
				iBsOtherEventsProbVertex, iBdOtherEventsProbVertex, iOtherEventsProbVertex,
				iBsJpsiEtaEventsProbVertex, iBdJpsiK10EventsProbVertex, iBdJpsiK0EventsProbVertex, iBpJpsiKpEventsProbVertex);
	  
	  if(tree->K1Pt_nofit_ > KaonPtCut  && tree->K2Pt_nofit_ > KaonPtCut) {   // passed Kaon pt cut
	    increaseCountersByOne(iBsJPsiPhiSignalEventsKaonPtCut, iBsJPsiKKSignalEventsKaonPtCut, iBdJPsiKstarSignalEventsKaonPtCut, 
				  iBsOtherEventsKaonPtCut, iBdOtherEventsKaonPtCut, iOtherEventsKaonPtCut,
				  iBsJpsiEtaEventsKaonPtCut, iBdJpsiK10EventsKaonPtCut, iBdJpsiK0EventsKaonPtCut, iBpJpsiKpEventsKaonPtCut);
	    
	    
	    //    fillHistograms(JpsiMass_after_,     vhJPsiMass); 
	    fillHistograms(tree->PhiM_nofit_,      vhPhiMass); 
	    
	    
	    if( fabs(tree->PhiM_nofit_ - nominalPhiMass) < phiMassWindow &&
		tree->BsFitM_ > LowerBsMassCut && tree->BsFitM_ < UpperBsMassCut) {   // passed tight phi mass cut
	      increaseCountersByOne(iBsJPsiPhiSignalEventsPhiMassCut, iBsJPsiKKSignalEventsPhiMassCut, iBdJPsiKstarSignalEventsPhiMassCut, 
				    iBsOtherEventsPhiMassCut, iBdOtherEventsPhiMassCut, iOtherEventsPhiMassCut,
				    iBsJpsiEtaEventsPhiMassCut, iBdJpsiK10EventsPhiMassCut, iBdJpsiK0EventsPhiMassCut, iBpJpsiKpEventsPhiMassCut);
	      
	      if(isGenBsJpsiPhiMuMuKKEvent_==1) makeVertexResolutionPlots();
	      
	      fillHistograms(tree->BsDist3d_ / tree->BsDist3dErr_ , vhDistSign3D);
	      fillHistograms(tree->BsDist2d_ / tree->BsDist2dErr_    ,  vhDistSign1D);
	      fillHistograms(tree->AngleBsDecayLength_, vhPointingAngle );
	      fillHistograms(tree->BsDist3d_,             vhDist3D); 
	      fillHistograms(tree->BsTime2d_,               vhTime);
	      fillHistograms(tree->BsFitM_,           vhBsMass_NoTimeCut);
              fillHistograms(tree->BsCt_,           vhBsCt);
              fillHistograms(tree->BsCtErr_,        vhBsCtErr);
	      
	      //****************
	      if(mySelector)mySelector->fill(); // **** filling root trees for fit here!
	      //****************

	      if(tree->BsDist2d_ /tree-> BsDist2dErr_ > decayLengthCut) {  // passed transverse decay length cut
		increaseCountersByOne(iBsJPsiPhiSignalEventsDecayLengthCut, iBsJPsiKKSignalEventsDecayLengthCut, iBdJPsiKstarSignalEventsDecayLengthCut, 
				      iBsOtherEventsDecayLengthCut, iBdOtherEventsDecayLengthCut, iOtherEventsDecayLengthCut,
				      iBsJpsiEtaEventsDecayLengthCut, iBdJpsiK10EventsDecayLengthCut, iBdJpsiK0EventsDecayLengthCut, iBpJpsiKpEventsDecayLengthCut);
		
		if(tree->AngleBsDecayLength_ > pointingCut) {  // passed pointing constraint
		  increaseCountersByOne(iBsJPsiPhiSignalEventsPointingCut, iBsJPsiKKSignalEventsPointingCut, iBdJPsiKstarSignalEventsPointingCut, 
					iBsOtherEventsPointingCut, iBdOtherEventsPointingCut, iOtherEventsPointingCut,
					iBsJpsiEtaEventsPointingCut, iBdJpsiK10EventsPointingCut, iBdJpsiK0EventsPointingCut, iBpJpsiKpEventsPointingCut);
		  
		  
		  // passed all cuts: what backgrounds are left?
		  // print full decay tree
		  
		  if( !isGenBsJpsiPhiMuMuKKEvent_ && !isGenBsJpsiKKEvent_ && !isGenBdJpsiKstarMuMuKpiEvent_ && !isGenBsEvent_ && !isGenBdEvent_ && !isGenBsJpsiEtaEvent_ && !isGenBdJpsiK10Event_ && !isGenBdJpsiK0Event_ && !isGenBpJpsiKpEvent_){
		    printDecayTree();
		    printedTree++;
		    std::cout<<std::endl;
		    std::cout<<" printedTrees = "<< printedTree << std::endl << std::endl; 
		  }
		  
		  
		  
		  
		  // make plots after all cuts
		  fillHistograms(tree->PhiM_nofit_,    vhPhiMassFinal);
		  fillHistograms(tree->BsM_nofit_ ,  vhBsMassFinal);
		  fillHistograms(tree->BsFitM_,        vhBsMassFinalAfterFit);
		}
	      }
	    }
	  }
	}
      }
    } // passed  pre KinFit mass cuts

    //****************************
    // START SECOND ANALYSIS for the Bd->Jpsi Kstar analysis here:
    //***************************
 
    if(tree->iPassedCutIdentBd_ >= 7) {   // passed  pre KinFit mass cuts
      increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsPreKinFit, iBdAna_BsJPsiKKSignalEventsPreKinFit, iBdAna_BdJPsiKstarSignalEventsPreKinFit,
			    iBdAna_BsOtherEventsPreKinFit, iBdAna_BdOtherEventsPreKinFit, iBdAna_OtherEventsPreKinFit,
			    iBdAna_BsJpsiEtaEventsPreKinFit, iBdAna_BdJpsiK10EventsPreKinFit, iBdAna_BdJpsiK0EventsPreKinFit, iBdAna_BpJpsiKpEventsPreKinFit); 

      if(tree->iPassedCutIdentBd_ >= 13) { // passed kinematic fit  
	increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsOfflineSel1, iBdAna_BsJPsiKKSignalEventsOfflineSel1, iBdAna_BdJPsiKstarSignalEventsOfflineSel1,
			      iBdAna_BsOtherEventsOfflineSel1, iBdAna_BdOtherEventsOfflineSel1, iBdAna_OtherEventsOfflineSel1,
			      iBdAna_BsJpsiEtaEventsOfflineSel1, iBdAna_BdJpsiK10EventsOfflineSel1, iBdAna_BdJpsiK0EventsOfflineSel1, iBdAna_BpJpsiKpEventsOfflineSel1);
      
	 
	fillHistograms(tree->BdFitChi2_Hyp1_,            vhBdChi2);
	fillHistograms(tree->BdFitChi2_Hyp1_ / tree->BdFitNdof_Hyp1_ , vhBdChi2Ndof);
	fillHistograms(tree->BdFitVtxProb_Hyp1_,          vhBdVtxProb );
	
	fillHistograms(tree->BdK1Pt_nofit_,         vhBdK1Pt);
	fillHistograms(tree->BdK2Pt_nofit_,         vhBdK2Pt);
	
	if(tree->BdFitVtxProb_Hyp1_ >= minProbability) { // passed minimum vertex probability cut
	  increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsProbVertex, iBdAna_BsJPsiKKSignalEventsProbVertex, iBdAna_BdJPsiKstarSignalEventsProbVertex,
				iBdAna_BsOtherEventsProbVertex, iBdAna_BdOtherEventsProbVertex, iBdAna_OtherEventsProbVertex,
				iBdAna_BsJpsiEtaEventsProbVertex, iBdAna_BdJpsiK10EventsProbVertex, iBdAna_BdJpsiK0EventsProbVertex, iBdAna_BpJpsiKpEventsProbVertex);
	  
	  if(tree->BdK1Pt_nofit_ > BdKaonPtCut  && tree->BdK2Pt_nofit_ > BdKaonPtCut) {   // passed Kaon pt cut
	    increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsKaonPtCut, iBdAna_BsJPsiKKSignalEventsKaonPtCut, iBdAna_BdJPsiKstarSignalEventsKaonPtCut, 
				  iBdAna_BsOtherEventsKaonPtCut, iBdAna_BdOtherEventsKaonPtCut, iBdAna_OtherEventsKaonPtCut,
				  iBdAna_BsJpsiEtaEventsKaonPtCut, iBdAna_BdJpsiK10EventsKaonPtCut, iBdAna_BdJpsiK0EventsKaonPtCut, iBdAna_BpJpsiKpEventsKaonPtCut);
	    
	    int KstarFlag = 0;
	    double KstarMass_nofit_ = -10;
	    double deltaKstarMass1 = fabs(tree->KstarMass_nofit_Hyp1_  - nominalKstarMass);
	    double deltaKstarMass2 = fabs(tree->KstarMass_nofit_Hyp2_  - nominalKstarMass);
	    if(deltaKstarMass1 < deltaKstarMass2)  { KstarFlag =1 ; KstarMass_nofit_ = tree->KstarMass_nofit_Hyp1_; }
	    else { KstarFlag  =2;  KstarMass_nofit_ = tree->KstarMass_nofit_Hyp2_; }
	    
    
	    if(KstarFlag == 1)     fillHistograms(tree->KstarMass_nofit_Hyp1_,      vhKstarMass); 
	    else if(KstarFlag == 2)fillHistograms(tree->KstarMass_nofit_Hyp2_,      vhKstarMass); 
	    else {std::cout<<" error 1" << std::endl;  exit(1);}
    
	    if( fabs(KstarMass_nofit_ - nominalKstarMass) < KstarMassWindow) {   // passed tight Kstar mass cut
	      increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsPhiMassCut, iBdAna_BsJPsiKKSignalEventsPhiMassCut, iBdAna_BdJPsiKstarSignalEventsPhiMassCut, 
				    iBdAna_BsOtherEventsPhiMassCut, iBdAna_BdOtherEventsPhiMassCut, iBdAna_OtherEventsPhiMassCut,
				    iBdAna_BsJpsiEtaEventsPhiMassCut, iBdAna_BdJpsiK10EventsPhiMassCut, iBdAna_BdJpsiK0EventsPhiMassCut, iBdAna_BpJpsiKpEventsPhiMassCut);

    
	      fillHistograms(tree->BdDist3d_  / tree->BdDist3dErr_ , vhBdDistSign3D);
	      fillHistograms(tree->BdDist2d_ / tree->BdDist2dErr_    ,  vhBdDistSign1D);
	      
	      fillHistograms(tree->BdDist3d_,             vhBdDist3D); 
	      fillHistograms(tree->BdTime2d_,               vhBdTime);
	      if(KstarFlag == 1) fillHistograms(tree->BdFitM_Hyp1_,           vhBdMass_NoTimeCut);
	      else if(KstarFlag == 2) fillHistograms(tree->BdFitM_Hyp2_,           vhBdMass_NoTimeCut);
	      else {std::cout<<" error 2" << std::endl; exit(1);}
	      
	      if(tree->BdDist2d_/ tree->BdDist2dErr_ > decayLengthCut) { // passed transverse decay length cut
		increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsDecayLengthCut, iBdAna_BsJPsiKKSignalEventsDecayLengthCut, iBdAna_BdJPsiKstarSignalEventsDecayLengthCut, 
				      iBdAna_BsOtherEventsDecayLengthCut, iBdAna_BdOtherEventsDecayLengthCut, iBdAna_OtherEventsDecayLengthCut,
				      iBdAna_BsJpsiEtaEventsDecayLengthCut, iBdAna_BdJpsiK10EventsDecayLengthCut, iBdAna_BdJpsiK0EventsDecayLengthCut, iBdAna_BpJpsiKpEventsDecayLengthCut);
		
		//     if(AngleBsDecayLength_ < pointingCut) continue;  // passed pointing constraint
		increaseCountersByOne(iBdAna_BsJPsiPhiSignalEventsPointingCut, iBdAna_BsJPsiKKSignalEventsPointingCut, iBdAna_BdJPsiKstarSignalEventsPointingCut, 
				      iBdAna_BsOtherEventsPointingCut, iBdAna_BdOtherEventsPointingCut, iBdAna_OtherEventsPointingCut,
				      iBdAna_BsJpsiEtaEventsPointingCut, iBdAna_BdJpsiK10EventsPointingCut, iBdAna_BdJpsiK0EventsPointingCut, iBdAna_BpJpsiKpEventsPointingCut);
		
		// passed all cuts: what backgrounds are left?
		// print full decay tree
		
// 		if( !isGenBsJpsiPhiMuMuKKEvent_ && !isGenBsJpsiKKEvent_ && !isGenBdJpsiKstarMuMuKpiEvent_
// 		    && !isGenBsEvent_ && !isGenBdEvent_ && !isGenBsJpsiEtaEvent_ && !isGenBdJpsiK10Event_
// 		    && !isGenBdJpsiK0Event_ && !isGenBpJpsiKpEvent_){
// 		  printDecayTree();
// 		  printedTree++;
// 		  std::cout<<std::endl;
// 		  std::cout<<" printedTrees = "<< printedTree << std::endl << std::endl; 
// 		}
		
		// make plots after all cuts
		//    fillHistograms(PhiMass_after_,    vhPhiMassFinal);
		fillHistograms( tree-> BdM_nofit_ ,  vhBdMassFinal);
		if(KstarFlag == 1)  fillHistograms( tree->BdFitM_Hyp1_,        vhBdMassFinalAfterFit);
		else if(KstarFlag == 2)  fillHistograms( tree->BdFitM_Hyp2_,        vhBdMassFinalAfterFit);
		else {std::cout<<" error 3" << std::endl; exit(1);}

	      }
	    }
	  }
	}
      }
    }
  } // event loop
  
  //****************************
  // finished event loop *******
  //****************************
  
  
  // print out results
  std::cout<< "There are " << entrycounter << " events in the tree." << std::endl;
 std::cout<< "Events with only HLT: " << HLT << std::endl << std::endl;
 
 std::cout<< "                          | Bs->JpsiPhi | Bd->Jpsi K* | Bs->Jpsi KK | Bs->Jpsi Eta| Bd->Jpsi K10| Bd->Jpsi K0 | Bp->Jpsi Kp | Other Bs    | Other Bd    | Other "<<  std::endl;
 std::cout<< "events                    | " << setw(11) << iBsJPsiPhiSignalEvents << " | " << setw(11) << iBdJPsiKstarSignalEvents << " | " << setw(11) << iBsJPsiKKSignalEvents << " | " 
	  << setw(11) << iBsJpsiEtaEvents << " | " << setw(11) << iBdJpsiK10Events << " | " << setw(11) << iBdJpsiK0Events << " | " << setw(11) << iBpJpsiKpEvents << " | " 
	  << setw(11) << iBsOtherEvents << " | " << setw(11) << iBdOtherEvents << " | " << setw(11) << iOtherEvents << std::endl;

 std::cout<< "triggered events          | " << setw(11) << iTriggered_DoubleMu3_BsJPsiPhiSignalEvents << " | " << setw(11) << iTriggered_DoubleMu3_BdJpsiKstar << " | " << setw(11) << iTriggered_DoubleMu3_BsJpsiKK << " | " 
	  << setw(11) << iTriggered_DoubleMu3_BsJpsiEtaEvents << " | " << setw(11) << iTriggered_DoubleMu3_BdJpsiK10Events << " | " << setw(11) << iTriggered_DoubleMu3_BdJpsiK0Events << " | " << setw(11) << iTriggered_DoubleMu3_BpJpsiKpEvents << " | " 
	  << setw(11) << iTriggered_DoubleMu3_BsOtherEvents << " | " << setw(11) << iTriggered_DoubleMu3_BdOtherEvents << " | " << setw(11) << iTriggered_DoubleMu3_OtherEvents << std::endl;

 std::cout<<std::endl<<std::endl;
 std::cout<<"Bs->Jpsi Phi Analysis:" << std::endl;
 
 std::cout<< "pre kin fit               | " << setw(11) << iBsJPsiPhiSignalEventsPreKinFit << " | " << setw(11) << iBdJPsiKstarSignalEventsPreKinFit << " | " << setw(11) << iBsJPsiKKSignalEventsPreKinFit << " | " 
	  << setw(11) << iBsJpsiEtaEventsPreKinFit << " | " << setw(11) << iBdJpsiK10EventsPreKinFit << " | " << setw(11) << iBdJpsiK0EventsPreKinFit << " | " << setw(11) << iBpJpsiKpEventsPreKinFit << " | " 
	  << setw(11) << iBsOtherEventsPreKinFit << " | " << setw(11) << iBdOtherEventsPreKinFit << " | " << setw(11) << iOtherEventsPreKinFit << std::endl;


 std::cout<< "after kin fit             | " << setw(11) << iBsJPsiPhiSignalEventsOfflineSel1 << " | " << setw(11) << iBdJPsiKstarSignalEventsOfflineSel1 << " | " << setw(11) << iBsJPsiKKSignalEventsOfflineSel1 << " | " 
	  << setw(11) << iBsJpsiEtaEventsOfflineSel1 << " | " << setw(11) << iBdJpsiK10EventsOfflineSel1 << " | " << setw(11) << iBdJpsiK0EventsOfflineSel1 << " | " << setw(11) << iBpJpsiKpEventsOfflineSel1 << " | " 
	  << setw(11) << iBsOtherEventsOfflineSel1 << " | " << setw(11) << iBdOtherEventsOfflineSel1 << " | " << setw(11) << iOtherEventsOfflineSel1 << std::endl;

 std::cout<< "Prob vertex(KK)>2 percent | " << setw(11) << iBsJPsiPhiSignalEventsProbVertex << " | " << setw(11) << iBdJPsiKstarSignalEventsProbVertex << " | " << setw(11) << iBsJPsiKKSignalEventsProbVertex << " | " 
	  << setw(11) << iBsJpsiEtaEventsProbVertex << " | " << setw(11) << iBdJpsiK10EventsProbVertex << " | " << setw(11) << iBdJpsiK0EventsProbVertex << " | " << setw(11) << iBpJpsiKpEventsProbVertex << " | " 
	  << setw(11) << iBsOtherEventsProbVertex << " | " << setw(11) << iBdOtherEventsProbVertex << " | " << setw(11) << iOtherEventsProbVertex << std::endl;



 std::cout<< "Kaon pt > 0.9             | " << setw(11) << iBsJPsiPhiSignalEventsKaonPtCut << " | " << setw(11) << iBdJPsiKstarSignalEventsKaonPtCut << " | " << setw(11) << iBsJPsiKKSignalEventsKaonPtCut << " | " 
	  << setw(11) << iBsJpsiEtaEventsKaonPtCut << " | " << setw(11) << iBdJpsiK10EventsKaonPtCut << " | " << setw(11) << iBdJpsiK0EventsKaonPtCut << " | " << setw(11) << iBpJpsiKpEventsKaonPtCut << " | " 
	  << setw(11) << iBsOtherEventsKaonPtCut << " | " << setw(11) << iBdOtherEventsKaonPtCut << " | " << setw(11) << iOtherEventsKaonPtCut << std::endl;



 std::cout<< "deltaPhiMass< 7MeV        | " << setw(11) << iBsJPsiPhiSignalEventsPhiMassCut << " | " << setw(11) << iBdJPsiKstarSignalEventsPhiMassCut << " | " << setw(11) << iBsJPsiKKSignalEventsPhiMassCut << " | " 
	  << setw(11) << iBsJpsiEtaEventsPhiMassCut << " | " << setw(11) << iBdJpsiK10EventsPhiMassCut << " | " << setw(11) << iBdJpsiK0EventsPhiMassCut << " | " << setw(11) << iBpJpsiKpEventsPhiMassCut << " | " 
	  << setw(11) << iBsOtherEventsPhiMassCut << " | " << setw(11) << iBdOtherEventsPhiMassCut << " | " << setw(11) << iOtherEventsPhiMassCut << std::endl;



 std::cout<< "decayLengthSign > 3       | " << setw(11) << iBsJPsiPhiSignalEventsDecayLengthCut << " | " << setw(11) << iBdJPsiKstarSignalEventsDecayLengthCut << " | " << setw(11) << iBsJPsiKKSignalEventsDecayLengthCut << " | " 
	  << setw(11) << iBsJpsiEtaEventsDecayLengthCut << " | " << setw(11) << iBdJpsiK10EventsDecayLengthCut << " | " << setw(11) << iBdJpsiK0EventsDecayLengthCut << " | " << setw(11) << iBpJpsiKpEventsDecayLengthCut << " | " 
	  << setw(11) << iBsOtherEventsDecayLengthCut << " | " << setw(11) << iBdOtherEventsDecayLengthCut << " | " << setw(11) << iOtherEventsDecayLengthCut << std::endl;


 std::cout<< "pointing cut              | " << setw(11) << iBsJPsiPhiSignalEventsPointingCut << " | " << setw(11) << iBdJPsiKstarSignalEventsPointingCut << " | " << setw(11) << iBsJPsiKKSignalEventsPointingCut << " | " 
	  << setw(11) << iBsJpsiEtaEventsPointingCut << " | " << setw(11) << iBdJpsiK10EventsPointingCut << " | " << setw(11) << iBdJpsiK0EventsPointingCut << " | " << setw(11) << iBpJpsiKpEventsPointingCut << " | " 
	  << setw(11) << iBsOtherEventsPointingCut << " | " << setw(11) << iBdOtherEventsPointingCut << " | " << setw(11) << iOtherEventsPointingCut << std::endl;



 std::cout<< "  "<<  std::endl;
 std::cout<< "  "<<  std::endl;


 std::cout<<std::endl<<std::endl;
 std::cout<<"Bd->Jpsi Kstar Analysis:" << std::endl;
 
 std::cout<< "pre kin fit               | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsPreKinFit << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsPreKinFit << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsPreKinFit << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsPreKinFit << " | " << setw(11) << iBdAna_BdJpsiK10EventsPreKinFit << " | " << setw(11) << iBdAna_BdJpsiK0EventsPreKinFit << " | " << setw(11) << iBdAna_BpJpsiKpEventsPreKinFit << " | " 
	  << setw(11) << iBdAna_BsOtherEventsPreKinFit << " | " << setw(11) << iBdAna_BdOtherEventsPreKinFit << " | " << setw(11) << iBdAna_OtherEventsPreKinFit << std::endl;


 std::cout<< "after kin fit             | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsOfflineSel1 << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsOfflineSel1 << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsOfflineSel1 << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsOfflineSel1 << " | " << setw(11) << iBdAna_BdJpsiK10EventsOfflineSel1 << " | " << setw(11) << iBdAna_BdJpsiK0EventsOfflineSel1 << " | " << setw(11) << iBdAna_BpJpsiKpEventsOfflineSel1 << " | " 
	  << setw(11) << iBdAna_BsOtherEventsOfflineSel1 << " | " << setw(11) << iBdAna_BdOtherEventsOfflineSel1 << " | " << setw(11) << iBdAna_OtherEventsOfflineSel1 << std::endl;

 std::cout<< "Prob vertex(KK)>2 percent | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsProbVertex << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsProbVertex << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsProbVertex << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsProbVertex << " | " << setw(11) << iBdAna_BdJpsiK10EventsProbVertex << " | " << setw(11) << iBdAna_BdJpsiK0EventsProbVertex << " | " << setw(11) << iBdAna_BpJpsiKpEventsProbVertex << " | " 
	  << setw(11) << iBdAna_BsOtherEventsProbVertex << " | " << setw(11) << iBdAna_BdOtherEventsProbVertex << " | " << setw(11) << iBdAna_OtherEventsProbVertex << std::endl;



 std::cout<< "Kaon pt cut               | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsKaonPtCut << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsKaonPtCut << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsKaonPtCut << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsKaonPtCut << " | " << setw(11) << iBdAna_BdJpsiK10EventsKaonPtCut << " | " << setw(11) << iBdAna_BdJpsiK0EventsKaonPtCut << " | " << setw(11) << iBdAna_BpJpsiKpEventsKaonPtCut << " | " 
	  << setw(11) << iBdAna_BsOtherEventsKaonPtCut << " | " << setw(11) << iBdAna_BdOtherEventsKaonPtCut << " | " << setw(11) << iBdAna_OtherEventsKaonPtCut << std::endl;



 std::cout<< "deltaKstarMass            | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsPhiMassCut << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsPhiMassCut << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsPhiMassCut << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsPhiMassCut << " | " << setw(11) << iBdAna_BdJpsiK10EventsPhiMassCut << " | " << setw(11) << iBdAna_BdJpsiK0EventsPhiMassCut << " | " << setw(11) << iBdAna_BpJpsiKpEventsPhiMassCut << " | " 
	  << setw(11) << iBdAna_BsOtherEventsPhiMassCut << " | " << setw(11) << iBdAna_BdOtherEventsPhiMassCut << " | " << setw(11) << iBdAna_OtherEventsPhiMassCut << std::endl;



 std::cout<< "decayLengthSign > 3       | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsDecayLengthCut << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsDecayLengthCut << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsDecayLengthCut << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsDecayLengthCut << " | " << setw(11) << iBdAna_BdJpsiK10EventsDecayLengthCut << " | " << setw(11) << iBdAna_BdJpsiK0EventsDecayLengthCut << " | " << setw(11) << iBdAna_BpJpsiKpEventsDecayLengthCut << " | " 
	  << setw(11) << iBdAna_BsOtherEventsDecayLengthCut << " | " << setw(11) << iBdAna_BdOtherEventsDecayLengthCut << " | " << setw(11) << iBdAna_OtherEventsDecayLengthCut << std::endl;


 std::cout<< "pointing cut              | " << setw(11) << iBdAna_BsJPsiPhiSignalEventsPointingCut << " | " << setw(11) << iBdAna_BdJPsiKstarSignalEventsPointingCut << " | " << setw(11) << iBdAna_BsJPsiKKSignalEventsPointingCut << " | " 
	  << setw(11) << iBdAna_BsJpsiEtaEventsPointingCut << " | " << setw(11) << iBdAna_BdJpsiK10EventsPointingCut << " | " << setw(11) << iBdAna_BdJpsiK0EventsPointingCut << " | " << setw(11) << iBdAna_BpJpsiKpEventsPointingCut << " | " 
	  << setw(11) << iBdAna_BsOtherEventsPointingCut << " | " << setw(11) << iBdAna_BdOtherEventsPointingCut << " | " << setw(11) << iBdAna_OtherEventsPointingCut << std::endl;



 std::cout<< "  "<<  std::endl;
 std::cout<< "  "<<  std::endl;


 // message for  removal of Jpsi
  if(bRemoveJpsiEvents==1) std::cout<<"Removing Jpsi events (use it only for ppmuX samples) !!!!!!" << std::endl;

 // write histograms to output file
 writeHistos(outputhistofilename);

 
 delete mySelector;


}   // finished main function



// define counting function
void increaseCountersByOne(int &BsJpsiPhiSignal, int& BsJpsiKKSignal, int& BdJpsiKstar, int& BsOther, int& BdOther,  int& other, 
			   int &BsJpsiEta, int &BdJpsiK10, int &BdJpsiK0, int &BpJpsiKp  ){

  if(isGenBsJpsiPhiMuMuKKEvent_ == 1)          BsJpsiPhiSignal++;
  else if (isGenBsJpsiKKEvent_ == 1)           BsJpsiKKSignal++;
  else if (isGenBdJpsiKstarMuMuKpiEvent_ == 1) BdJpsiKstar++;
  else if (isGenBsJpsiEtaEvent_ == 1)          BsJpsiEta++;
  else if (isGenBdJpsiK10Event_ == 1)          BdJpsiK10++;
  else if (isGenBdJpsiK0Event_ == 1)           BdJpsiK0++;
  else if (isGenBpJpsiKpEvent_ == 1)           BpJpsiKp++;
  else if (isGenBsEvent_ == 1)                 BsOther++;
  else if (isGenBdEvent_ == 1)                 BdOther++;		 
  else                                         other++; 
  
}  // end definition of counting function



// histogram filling function
void fillHistograms(double value, TH1F* hBsJpsiPhiSignal, TH1F* hBsOther, TH1F * hOther, TH1F *hBsJPsiKK ){

  if(isGenBsJpsiPhiMuMuKKEvent_ == 1) hBsJpsiPhiSignal->Fill(value);
  else if(isGenBsEvent_==1) hBsOther->Fill(value);
  else hOther->Fill(value);
  
  // special case to check distributions of BsJPsiKK events
  if(isGenBsJpsiKKEvent_ == 1)  if(hBsJPsiKK) hBsJPsiKK->Fill(value);
} // end histogram filling function


// function to fill vector of histograms
//------------------------
void fillHistograms(double value, vector<TH1F*> &vHistos){
  if(isGenBsJpsiPhiMuMuKKEvent_==1) vHistos[0]->Fill(value);
  else if(isGenBsJpsiKKEvent_==1)   vHistos[1]->Fill(value);
  else if(isGenBdJpsiKstarMuMuKpiEvent_==1) vHistos[2]->Fill(value);
  else if(isGenBsJpsiEtaEvent_ ==1) vHistos[6]->Fill(value);
  else if(isGenBdJpsiK10Event_==1)  vHistos[7]->Fill(value);
  else if(isGenBdJpsiK0Event_==1)  vHistos[8]->Fill(value);
  else if(isGenBpJpsiKpEvent_==1)  vHistos[9]->Fill(value);
  else if(isGenBsEvent_ ==1) vHistos[3]->Fill(value);
  else if(isGenBdEvent_ ==1) vHistos[4]->Fill(value);
  else vHistos[5]->Fill(value);
}






//------------------------


// find type of events based on MC info arrays
void identifyEventType(bool & isGenBsJpsiPhiMuMuKKEvent, bool &isGenBsJpsiKKEvent, bool &isGenBdJpsiKstarMuMuKpiEvent, bool &isGenBsEvent, 
		       bool &isGenBdEvent, bool &isGenBsJpsiEtaEvent, bool &isGenBdJpsiK10Event, 
		       bool &isGenBdJpsiK0Event, bool & isGenBpJpsiKpEvent){

  //set to default values
  isGenBsJpsiPhiMuMuKKEvent = isGenBsJpsiKKEvent = isGenBdJpsiKstarMuMuKpiEvent = isGenBsEvent = isGenBdEvent =
    isGenBsJpsiEtaEvent =  isGenBdJpsiK10Event = isGenBdJpsiK0Event = isGenBpJpsiKpEvent = 0 ;

 

  //loop over B mesons
  for(int i=0; i < tree->GenNumberOfBdecays_; i++){
    int B_id = abs(tree->BmesonsId_[i]);
    //    if(B_id== 531) isGenBsEvent = 1;  // only label as Bs or Bd if the decay is with Jpsi->mumu
    //    if(B_id == 511) isGenBdEvent = 1;
    
    //loop over daughters
    // helper flags
    bool BsJpsi = 0;
    bool BsExcitedJpsi = 0;
    bool BsPhi = 0;
    bool BsJpsiMuPlus = 0;
    bool BsJpsiMuMinus = 0;
    bool BsPhiKplus = 0;
    bool BsPhiKminus = 0;
    bool BsKplus = 0;
    bool BsKminus = 0;
    bool BsEta = 0;
    
    bool BdK10 = 0;
    bool BdK0 = 0;

    bool BdJpsi = 0;
    bool BdJpsiMuPlus = 0;
    bool BdJpsiMuMinus = 0;
    bool BdKstar = 0;
    bool BdKstarK = 0;
    bool BdKstarPi = 0;

  bool BdExcitedJpsi = 0;

    bool BpJpsi = 0;
    bool BpJpsiMuPlus = 0;
    bool BpJpsiMuMinus = 0;
    bool BpK1p = 0;
    bool BpExcitedJpsi = 0;
  

    for(int j=0; j < tree->GenNumberOfDaughters_[i]; j++){
      int Dau_id = tree->BDauIdMC_[i][j];
      int absDau_id = abs(Dau_id);
      if(B_id == 531 && absDau_id == 443 ) BsJpsi =1;
      if(B_id == 531 && absDau_id == 100443 ) BsExcitedJpsi =1;
      if(B_id == 531 && absDau_id == 20443 ) BsExcitedJpsi =1;
      if(B_id == 531 && absDau_id == 445 ) BsExcitedJpsi =1;
      if(B_id == 531 && absDau_id == 333) BsPhi = 1;
      if(B_id == 531 && Dau_id == 321)    BsKplus = 1;
      if(B_id == 531 && Dau_id == -321)   BsKminus = 1;
      if(B_id == 531 && absDau_id == 221) BsEta  =1;
      
      if(B_id == 511 && absDau_id == 443) BdJpsi = 1;
      if(B_id == 511 && absDau_id == 313) BdKstar = 1;   //// ATTENTION: add other excited Kaons??
      if(B_id == 511 && absDau_id == 10313 ) BdK10 = 1;
      if(B_id == 511 && absDau_id == 310 )   BdK0 = 1;
      if(B_id == 511 && absDau_id == 20443) BdExcitedJpsi = 1;
      if(B_id == 511 && absDau_id == 100443) BdExcitedJpsi = 1;
      if(B_id == 511 && absDau_id == 445) BdExcitedJpsi = 1;
      
      if(B_id == 521 && absDau_id == 443) BpJpsi =1;
      if(B_id == 521 && absDau_id == 10323) BpK1p =1;
      if(B_id == 521 && absDau_id == 20323) BpK1p =1;
      if(B_id == 521 && absDau_id == 323) BpK1p =1;
      if(B_id == 521 && absDau_id == 321) BpK1p =1;
      if(B_id == 521 && absDau_id == 325) BpK1p =1;

      if(B_id == 521 && absDau_id == 100443) BpExcitedJpsi = 1;
      if(B_id == 521 && absDau_id == 20443) BpExcitedJpsi = 1;
      if(B_id == 521 && absDau_id == 445) BpExcitedJpsi = 1;
      // loop over daughters daughters
      for(int k=0; k < tree->GenNumberOfDaughtersDaughters_[i][j]; k++){
	int DauDau_id = tree->BDauDauIdMC_[i][j][k];
	int absDauDau_id= abs(DauDau_id);
	if(B_id == 531 && absDau_id == 443 && DauDau_id == 13) BsJpsiMuMinus = 1;
	if(B_id == 531 && absDau_id == 443 && DauDau_id == -13) BsJpsiMuPlus = 1;
	if(B_id == 531 && absDau_id == 333 && DauDau_id == 321) BsPhiKplus = 1;
	if(B_id == 531 && absDau_id == 333 && DauDau_id == -321) BsPhiKminus = 1;

	if(B_id == 511 && absDau_id == 443 && DauDau_id == 13) BdJpsiMuMinus = 1;
	if(B_id == 511 && absDau_id == 443 && DauDau_id == -13) BdJpsiMuPlus = 1;
	if(B_id == 511 && absDau_id == 313 && absDauDau_id == 321) BdKstarK = 1;      ////add other excited Kaons???
	if(B_id == 511 && absDau_id == 313 && absDauDau_id == 211) BdKstarPi = 1;
      

	if(B_id == 521 && absDau_id == 443 && DauDau_id == 13) BpJpsiMuMinus = 1;
	if(B_id == 521 && absDau_id == 443 && DauDau_id == -13) BpJpsiMuPlus = 1;
      }
    }
    if(BsJpsiMuMinus && BsJpsiMuPlus) isGenBsEvent = 1;
    if(BsExcitedJpsi)                 isGenBsEvent = 1;
    if(BdJpsiMuMinus && BdJpsiMuPlus) isGenBdEvent = 1;
    if(tree->GenNumberOfDaughters_[i] == 2 && BsJpsiMuMinus && BsJpsiMuPlus && BsPhiKplus && BsPhiKminus) isGenBsJpsiPhiMuMuKKEvent = 1;
    if(tree->GenNumberOfDaughters_[i] == 3 && BsJpsiMuMinus && BsJpsiMuPlus && BsKplus && BsKminus)       isGenBsJpsiKKEvent = 1;  
    if(tree->GenNumberOfDaughters_[i] == 2 && BdJpsiMuMinus && BdJpsiMuPlus && BdKstarPi && BdKstarK )    isGenBdJpsiKstarMuMuKpiEvent = 1;
    if(BsJpsiMuMinus && BsJpsiMuPlus && BsEta) isGenBsJpsiEtaEvent = 1;
    if(BdJpsiMuMinus && BdJpsiMuPlus && BdK10) isGenBdJpsiK10Event = 1;
    if(BdJpsiMuMinus && BdJpsiMuPlus && BdK0)  isGenBdJpsiK0Event = 1;
    if(BpJpsiMuMinus && BpJpsiMuPlus && BpK1p ) isGenBpJpsiKpEvent = 1;
    if(BpExcitedJpsi && BpK1p) isGenBpJpsiKpEvent = 1;
    if(BdExcitedJpsi) isGenBdEvent = 1;
  }
   
}


//----------------
void printDecayTree(){
  for(int i=0; i<tree->GenNumberOfBdecays_; i++){
    std::cout<<"B pdg id = " << tree->BmesonsId_[i] << std::endl;
    for(int j=0; j<tree->GenNumberOfDaughters_[i]; j++){
      std::cout<<"  Dau pdg id = " << tree->BDauIdMC_[i][j] << std::endl;
      for(int k=0; k<tree->GenNumberOfDaughtersDaughters_[i][j]; k++){
	std::cout<<"      DauDau pdg id = " << tree->BDauDauIdMC_[i][j][k] << std::endl;
      }
    }
  }
}
//-------------------



void writeHistos(TString outputfilename){
  TFile *histofile = new TFile(outputfilename , "RECREATE");

  for(int i=0; i<iNumberOfCategories_; i++){
    vhJPsiMass[i]->Write();                
    vhPhiMass    [i]->Write();         
    vhPhiMassFinal[i]->Write();        
    vhBsMassFinal[i]->Write();         
    vhBsMassFinalAfterFit[i]->Write(); 
    vhK1Pt[i]->Write();                
    vhK2Pt[i]->Write();                
    vhDist3D[i]->Write();              
    vhDistSign3D[i]->Write();          
    vhDistSign1D[i]->Write();          
    vhPointingAngle[i]->Write();       
    vhChi2[i]->Write();                  
    vhChi2Ndof[i]->Write();            
    vhBsVtxProb[i]->Write(); 
    vhTime[i]->Write();
    vhBsMass_NoTimeCut[i]->Write();

    vhBdChi2[i]->Write();             
    vhBdChi2Ndof[i]->Write();         
    vhBdVtxProb[i]->Write();          
    vhBdK1Pt[i]->Write();             
    vhBdK2Pt[i]->Write();             
    vhKstarMass[i]->Write();          
    vhBdDistSign3D[i]->Write();       
    vhBdDistSign1D[i]->Write();       
    vhBdDist3D[i]->Write();           
    vhBdTime[i]->Write();            
    vhBdMass_NoTimeCut[i]->Write();   
    vhBdMassFinal[i]->Write();        
    vhBdMassFinalAfterFit[i]->Write();
    vhResoLxy[i]->Write();
    vhResoTime[i]->Write();
    vhResoCt[i]->Write();
    vhResoRefitLxy[i]->Write();
    vhResoRefitTime[i]->Write();
    vhResoRefitCt[i]->Write();

    vhBsCt[i]->Write();
    vhBsCtErr[i]->Write();

  }
 histofile->Write();
 histofile->Close();

}


void bookHistoVectors(){

// vector<TH1F*> vhJPsiMass;
// vector<TH1F*> vhPhiMass    ;   
// vector<TH1F*> vhPhiMassFinal;      
// vector<TH1F*> vhBsMassFinal;  
// vector<TH1F*> vhBsMassFinalAfterFit; 
// vector<TH1F*> vhK1Pt;  
// vector<TH1F*> vhK2Pt;     
// vector<TH1F*> vhDist3D;      
// vector<TH1F*> vhDistSign3D; 
// vector<TH1F*> vhDistSign1D;
// vector<TH1F*> vhPointingAngle;    
// vector<TH1F*> vhChi2;
// vector<TH1F*> vhChi2Ndof;
// vector<TH1F*> vhBsVtxProb;           

vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BsJPsiPhiSignal"             ,"hJPsiMass_BsJPsiPhiSignal"      , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BsJPsiKKSignal"		   ,"hJPsiMass_BsJPsiKKSignal"       , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BdJPsiKstarSignal"	   ,"hJPsiMass_BdJPsiKstarSignal"    , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BsOther"			   ,"hJPsiMass_BsOther"              , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BdOther"			   ,"hJPsiMass_BdOther"              , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_Other"			   ,"hJPsiMass_Other"                , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BsJpsiEta"		   ,"hJPsiMass_BsJpsiEta"            , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BdJpsiK10"		   ,"hJPsiMass_BdJpsiK10"            , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BdJpsiK0"			   ,"hJPsiMass_BdJpsiK0"             , 100, 2.8, 3.4                ) );
vhJPsiMass           .push_back( new TH1F ("hJPsiMass_BpJpsiKp"           	   ,"hJPsiMass_BpJpsiKp"             , 100, 2.8, 3.4                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BsJPsiPhiSignal"    	   ,"hPhiMass_BsJPsiPhiSignal"       , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BsJPsiKKSignal"     	   ,"hPhiMass_BsJPsiKKSignal"        , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BdJPsiKstarSignal"  	   ,"hPhiMass_BdJPsiKstarSignal"     , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BsOther"            	   ,"hPhiMass_BsOther"               , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BdOther"            	   ,"hPhiMass_BdOther"               , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_Other"              	   ,"hPhiMass_Other"                 , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BsJpsiEta"          	   ,"hPhiMass_BsJpsiEta"             , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BdJpsiK10"          	   ,"hPhiMass_BdJpsiK10"             , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BdJpsiK0"           	   ,"hPhiMass_BdJpsiK0"              , 100, 1, 1.035                ) );
vhPhiMass            .push_back( new TH1F ("hPhiMass_BpJpsiKp"           	   ,"hPhiMass_BpJpsiKp"              , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BsJPsiPhiSignal"    	   ,"hPhiMassFinal_BsJPsiPhiSignal"  , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BsJPsiKKSignal"     	   ,"hPhiMassFinal_BsJPsiKKSignal"   , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BdJPsiKstarSignal"  	   ,"hPhiMassFinal_BdJPsiKstarSignal", 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BsOther"            	   ,"hPhiMassFinal_BsOther"          , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BdOther"            	   ,"hPhiMassFinal_BdOther"          , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_Other"              	   ,"hPhiMassFinal_Other"            , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BsJpsiEta"          	   ,"hPhiMassFinal_BsJpsiEta"        , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BdJpsiK10"          	   ,"hPhiMassFinal_BdJpsiK10"        , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BdJpsiK0"           	   ,"hPhiMassFinal_BdJpsiK0"         , 100, 1, 1.035                ) );
vhPhiMassFinal       .push_back( new TH1F ("hPhiMassFinal_BpJpsiKp"           	   ,"hPhiMassFinal_BpJpsiKp"         , 100, 1, 1.035                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BsJPsiPhiSignal"    	   ,"hBsMassFinal_BsJPsiPhiSignal"   , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BsJPsiKKSignal"     	   ,"hBsMassFinal_BsJPsiKKSignal"    , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BdJPsiKstarSignal"  	   ,"hBsMassFinal_BdJPsiKstarSignal" , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BsOther"            	   ,"hBsMassFinal_BsOther"           , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BdOther"            	   ,"hBsMassFinal_BdOther"           , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_Other"              	   ,"hBsMassFinal_Other"             , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BsJpsiEta"          	   ,"hBsMassFinal_BsJpsiEta"         , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BdJpsiK10"          	   ,"hBsMassFinal_BdJpsiK10"         , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BdJpsiK0"           	   ,"hBsMassFinal_BdJpsiK0"          , 100, 5.2, 5.7                ) );
vhBsMassFinal        .push_back( new TH1F ("hBsMassFinal_BpJpsiKp"           	   ,"hBsMassFinal_BpJpsiKp"          , 100, 5.2, 5.7                ) );
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BsJPsiPhiSignal"  ,"hBsMassFinalAfterFit_BsJPsiPhiSignal"   , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BsJPsiKKSignal"   ,"hBsMassFinalAfterFit_BsJPsiKKSignal"    , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BdJPsiKstarSignal","hBsMassFinalAfterFit_BdJPsiKstarSignal" , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BsOther"          ,"hBsMassFinalAfterFit_BsOther"           , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BdOther"          ,"hBsMassFinalAfterFit_BdOther"           , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_Other"            ,"hBsMassFinalAfterFit_Other"             , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BsJpsiEta"        ,"hBsMassFinalAfterFit_BsJpsiEta"         , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BdJpsiK10"        ,"hBsMassFinalAfterFit_BdJpsiK10"         , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BdJpsiK0"         ,"hBsMassFinalAfterFit_BdJpsiK0"          , 100, 5.2, 5.7        ) ); 
vhBsMassFinalAfterFit.push_back( new TH1F ("hBsMassFinalAfterFit_BpJpsiKp"         ,"hBsMassFinalAfterFit_BpJpsiKp"          , 100, 5.2, 5.7        ) ); 
vhK1Pt               .push_back( new TH1F ("hK1Pt_BsJPsiPhiSignal"    		   ,"hK1Pt_BsJPsiPhiSignal"        , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BsJPsiKKSignal"     		   ,"hK1Pt_BsJPsiKKSignal"         , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BdJPsiKstarSignal"  		   ,"hK1Pt_BdJPsiKstarSignal"      , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BsOther"            		   ,"hK1Pt_BsOther"                , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BdOther"            		   ,"hK1Pt_BdOther"                , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_Other"              		   ,"hK1Pt_Other"                  , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BsJpsiEta"          		   ,"hK1Pt_BsJpsiEta"              , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BdJpsiK10"          		   ,"hK1Pt_BdJpsiK10"              , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BdJpsiK0"           		   ,"hK1Pt_BdJpsiK0"               , 100, 0, 5                  ) );
vhK1Pt               .push_back( new TH1F ("hK1Pt_BpJpsiKp"           		   ,"hK1Pt_BpJpsiKp"               , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BsJPsiPhiSignal"    		   ,"hK2Pt_BsJPsiPhiSignal"        , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BsJPsiKKSignal"     		   ,"hK2Pt_BsJPsiKKSignal"         , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BdJPsiKstarSignal"  		   ,"hK2Pt_BdJPsiKstarSignal"      , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BsOther"            		   ,"hK2Pt_BsOther"                , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BdOther"            		   ,"hK2Pt_BdOther"                , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_Other"              		   ,"hK2Pt_Other"                  , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BsJpsiEta"          		   ,"hK2Pt_BsJpsiEta"              , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BdJpsiK10"          		   ,"hK2Pt_BdJpsiK10"              , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BdJpsiK0"           		   ,"hK2Pt_BdJpsiK0"               , 100, 0, 5                  ) );
vhK2Pt               .push_back( new TH1F ("hK2Pt_BpJpsiKp"           		   ,"hK2Pt_BpJpsiKp"               , 100, 0, 5                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BsJPsiPhiSignal"               ,"hDist3D_BsJPsiPhiSignal"      , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BsJPsiKKSignal"                ,"hDist3D_BsJPsiKKSignal"       , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BdJPsiKstarSignal"             ,"hDist3D_BdJPsiKstarSignal"    , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BsOther"                       ,"hDist3D_BsOther"              , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BdOther"                       ,"hDist3D_BdOther"              , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_Other"                         ,"hDist3D_Other"                , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BsJpsiEta"                     ,"hDist3D_BsJpsiEta"            , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BdJpsiK10"                     ,"hDist3D_BdJpsiK10"            , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BdJpsiK0"                      ,"hDist3D_BdJpsiK0"             , 100, 0, 1                  ) );
vhDist3D             .push_back( new TH1F ("hDist3D_BpJpsiKp"                  	   ,"hDist3D_BpJpsiKp"             , 100, 0, 1                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BsJPsiPhiSignal"           ,"hDistSign3D_BsJPsiPhiSignal"  , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BsJPsiKKSignal"            ,"hDistSign3D_BsJPsiKKSignal"   , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BdJPsiKstarSignal"         ,"hDistSign3D_BdJPsiKstarSignal", 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BsOther"                   ,"hDistSign3D_BsOther"          , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BdOther"                   ,"hDistSign3D_BdOther"          , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_Other"                     ,"hDistSign3D_Other"            , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BsJpsiEta"                 ,"hDistSign3D_BsJpsiEta"        , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BdJpsiK10"                 ,"hDistSign3D_BdJpsiK10"        , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BdJpsiK0"                  ,"hDistSign3D_BdJpsiK0"         , 100, 0, 50                  ) );
vhDistSign3D         .push_back( new TH1F ("hDistSign3D_BpJpsiKp"                  ,"hDistSign3D_BpJpsiKp"         , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BsJPsiPhiSignal"           ,"hDistSign1D_BsJPsiPhiSignal"  , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BsJPsiKKSignal"            ,"hDistSign1D_BsJPsiKKSignal"   , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BdJPsiKstarSignal"         ,"hDistSign1D_BdJPsiKstarSignal", 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BsOther"                   ,"hDistSign1D_BsOther"          , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BdOther"                   ,"hDistSign1D_BdOther"          , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_Other"                     ,"hDistSign1D_Other"            , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BsJpsiEta"                 ,"hDistSign1D_BsJpsiEta"        , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BdJpsiK10"                 ,"hDistSign1D_BdJpsiK10"        , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BdJpsiK0"                  ,"hDistSign1D_BdJpsiK0"         , 100, 0, 50                  ) );
vhDistSign1D         .push_back( new TH1F ("hDistSign1D_BpJpsiKp"                  ,"hDistSign1D_BpJpsiKp"         , 100, 0, 50                  ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BsJPsiPhiSignal"        ,"hPointingAngle_BsJPsiPhiSignal"   , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BsJPsiKKSignal"         ,"hPointingAngle_BsJPsiKKSignal"    , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BdJPsiKstarSignal"      ,"hPointingAngle_BdJPsiKstarSignal" , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BsOther"                ,"hPointingAngle_BsOther"           , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BdOther"                ,"hPointingAngle_BdOther"           , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_Other"                  ,"hPointingAngle_Other"             , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BsJpsiEta"              ,"hPointingAngle_BsJpsiEta"         , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BdJpsiK10"              ,"hPointingAngle_BdJpsiK10"         , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BdJpsiK0"               ,"hPointingAngle_BdJpsiK0"          , 100, -1, 1              ) );
vhPointingAngle      .push_back( new TH1F ("hPointingAngle_BpJpsiKp"               ,"hPointingAngle_BpJpsiKp"          , 100, -1, 1              ) );
vhChi2               .push_back( new TH1F ("hChi2_BsJPsiPhiSignal"                 ,"hChi2_BsJPsiPhiSignal"       , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BsJPsiKKSignal"                  ,"hChi2_BsJPsiKKSignal"        , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BdJPsiKstarSignal"               ,"hChi2_BdJPsiKstarSignal"     , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BsOther"                         ,"hChi2_BsOther"               , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BdOther"                         ,"hChi2_BdOther"               , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_Other"                           ,"hChi2_Other"                 , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BsJpsiEta"                       ,"hChi2_BsJpsiEta"             , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BdJpsiK10"                       ,"hChi2_BdJpsiK10"             , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BdJpsiK0"                        ,"hChi2_BdJpsiK0"              , 100, 0,30                   ) );
vhChi2               .push_back( new TH1F ("hChi2_BpJpsiKp"                        ,"hChi2_BpJpsiKp"              , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BsJPsiPhiSignal"             ,"hChi2Ndof_BsJPsiPhiSignal"   , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BsJPsiKKSignal"              ,"hChi2Ndof_BsJPsiKKSignal"    , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BdJPsiKstarSignal"           ,"hChi2Ndof_BdJPsiKstarSignal" , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BsOther"                     ,"hChi2Ndof_BsOther"           , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BdOther"                     ,"hChi2Ndof_BdOther"           , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_Other"                       ,"hChi2Ndof_Other"             , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BsJpsiEta"                   ,"hChi2Ndof_BsJpsiEta"         , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BdJpsiK10"                   ,"hChi2Ndof_BdJpsiK10"         , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BdJpsiK0"                    ,"hChi2Ndof_BdJpsiK0"          , 100, 0,30                   ) );
vhChi2Ndof           .push_back( new TH1F ("hChi2Ndof_BpJpsiKp"                    ,"hChi2Ndof_BpJpsiKp"          , 100, 0,30                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BsJPsiPhiSignal"            ,"hBsVtxProb_BsJPsiPhiSignal"  , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BsJPsiKKSignal"             ,"hBsVtxProb_BsJPsiKKSignal"   , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BdJPsiKstarSignal"          ,"hBsVtxProb_BdJPsiKstarSignal", 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BsOther"                    ,"hBsVtxProb_BsOther"          , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BdOther"                    ,"hBsVtxProb_BdOther"          , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_Other"                      ,"hBsVtxProb_Other"            , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BsJpsiEta"                  ,"hBsVtxProb_BsJpsiEta"        , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BdJpsiK10"                  ,"hBsVtxProb_BdJpsiK10"        , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BdJpsiK0"                   ,"hBsVtxProb_BdJpsiK0"         , 100, -0.5, 1.5                   ) );
vhBsVtxProb          .push_back( new TH1F ("hBsVtxProb_BpJpsiKp"                   ,"hBsVtxProb_BpJpsiKp"         , 100, -0.5, 1.5                   ) );
vhTime               .push_back( new TH1F ("hTime_BsJPsiPhiSignal"                 ,"hTime_BsJPsiPhiSignal"   , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BsJPsiKKSignal"                  ,"hTime_BsJPsiKKSignal"    , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BdJPsiKstarSignal"               ,"hTime_BdJPsiKstarSignal" , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BsOther"                         ,"hTime_BsOther"           , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BdOther"                         ,"hTime_BdOther"           , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_Other"                           ,"hTime_Other"             , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BsJpsiEta"                       ,"hTime_BsJpsiEta"         , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BdJpsiK10"                       ,"hTime_BdJpsiK10"         , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BdJpsiK0"                        ,"hTime_BdJpsiK0"          , 100, -2,10                   ) );
vhTime               .push_back( new TH1F ("hTime_BpJpsiKp"                        ,"hTime_BpJpsiKp"          , 100, -2,10                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BsJPsiPhiSignal"            ,"hBsMass_NoTimeCut_BsJPsiPhiSignal"  , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BsJPsiKKSignal"             ,"hBsMass_NoTimeCut_BsJPsiKKSignal"   , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BdJPsiKstarSignal"          ,"hBsMass_NoTimeCut_BdJPsiKstarSignal", 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BsOther"                    ,"hBsMass_NoTimeCut_BsOther"          , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BdOther"                    ,"hBsMass_NoTimeCut_BdOther"          , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_Other"                      ,"hBsMass_NoTimeCut_Other"            , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BsJpsiEta"                  ,"hBsMass_NoTimeCut_BsJpsiEta"        , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BdJpsiK10"                  ,"hBsMass_NoTimeCut_BdJpsiK10"        , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BdJpsiK0"                   ,"hBsMass_NoTimeCut_BdJpsiK0"         , 100, 5.2, 5.7                   ) );
vhBsMass_NoTimeCut          .push_back( new TH1F ("hBsMass_NoTimeCut_BpJpsiKp"                   ,"hBsMass_NoTimeCut_BpJpsiKp"         , 100, 5.2, 5.7                   ) );



vhKstarMass            .push_back( new TH1F ("hKstarMass_BsJPsiPhiSignal"    	   ,"hKstarMass_BsJPsiPhiSignal"       , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BsJPsiKKSignal"     	   ,"hKstarMass_BsJPsiKKSignal"        , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BdJPsiKstarSignal"  	   ,"hKstarMass_BdJPsiKstarSignal"     , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BsOther"            	   ,"hKstarMass_BsOther"               , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BdOther"            	   ,"hKstarMass_BdOther"               , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_Other"              	   ,"hKstarMass_Other"                 , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BsJpsiEta"          	   ,"hKstarMass_BsJpsiEta"             , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BdJpsiK10"          	   ,"hKstarMass_BdJpsiK10"             , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BdJpsiK0"           	   ,"hKstarMass_BdJpsiK0"              , 100, 0.8, 1.1                ) );
vhKstarMass            .push_back( new TH1F ("hKstarMass_BpJpsiKp"           	   ,"hKstarMass_BpJpsiKp"              , 100, 0.8, 1.1                ) );

vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BsJPsiPhiSignal"    	   ,"hBdMassFinal_BsJPsiPhiSignal"   , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BsJPsiKKSignal"     	   ,"hBdMassFinal_BsJPsiKKSignal"    , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BdJPsiKstarSignal"  	   ,"hBdMassFinal_BdJPsiKstarSignal" , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BsOther"            	   ,"hBdMassFinal_BsOther"           , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BdOther"            	   ,"hBdMassFinal_BdOther"           , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_Other"              	   ,"hBdMassFinal_Other"             , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BsJpsiEta"          	   ,"hBdMassFinal_BsJpsiEta"         , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BdJpsiK10"          	   ,"hBdMassFinal_BdJpsiK10"         , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BdJpsiK0"           	   ,"hBdMassFinal_BdJpsiK0"          , 100, 4.9, 5.7                ) );
vhBdMassFinal        .push_back( new TH1F ("hBdMassFinal_BpJpsiKp"           	   ,"hBdMassFinal_BpJpsiKp"          , 100, 4.9, 5.7                ) );
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BsJPsiPhiSignal"  ,"hBdMassFinalAfterFit_BsJPsiPhiSignal"   , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BsJPsiKKSignal"   ,"hBdMassFinalAfterFit_BsJPsiKKSignal"    , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BdJPsiKstarSignal","hBdMassFinalAfterFit_BdJPsiKstarSignal" , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BsOther"          ,"hBdMassFinalAfterFit_BsOther"           , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BdOther"          ,"hBdMassFinalAfterFit_BdOther"           , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_Other"            ,"hBdMassFinalAfterFit_Other"             , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BsJpsiEta"        ,"hBdMassFinalAfterFit_BsJpsiEta"         , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BdJpsiK10"        ,"hBdMassFinalAfterFit_BdJpsiK10"         , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BdJpsiK0"         ,"hBdMassFinalAfterFit_BdJpsiK0"          , 100, 4.9, 5.7        ) ); 
vhBdMassFinalAfterFit.push_back( new TH1F ("hBdMassFinalAfterFit_BpJpsiKp"         ,"hBdMassFinalAfterFit_BpJpsiKp"          , 100, 4.9, 5.7        ) ); 
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BsJPsiPhiSignal"    		   ,"hBdK1Pt_BsJPsiPhiSignal"        , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BsJPsiKKSignal"     		   ,"hBdK1Pt_BsJPsiKKSignal"         , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BdJPsiKstarSignal"  		   ,"hBdK1Pt_BdJPsiKstarSignal"      , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BsOther"            		   ,"hBdK1Pt_BsOther"                , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BdOther"            		   ,"hBdK1Pt_BdOther"                , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_Other"              		   ,"hBdK1Pt_Other"                  , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BsJpsiEta"          		   ,"hBdK1Pt_BsJpsiEta"              , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BdJpsiK10"          		   ,"hBdK1Pt_BdJpsiK10"              , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BdJpsiK0"           		   ,"hBdK1Pt_BdJpsiK0"               , 100, 0, 5                  ) );
vhBdK1Pt               .push_back( new TH1F ("hBdK1Pt_BpJpsiKp"           		   ,"hBdK1Pt_BpJpsiKp"               , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BsJPsiPhiSignal"    		   ,"hBdK2Pt_BsJPsiPhiSignal"        , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BsJPsiKKSignal"     		   ,"hBdK2Pt_BsJPsiKKSignal"         , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BdJPsiKstarSignal"  		   ,"hBdK2Pt_BdJPsiKstarSignal"      , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BsOther"            		   ,"hBdK2Pt_BsOther"                , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BdOther"            		   ,"hBdK2Pt_BdOther"                , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_Other"              		   ,"hBdK2Pt_Other"                  , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BsJpsiEta"          		   ,"hBdK2Pt_BsJpsiEta"              , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BdJpsiK10"          		   ,"hBdK2Pt_BdJpsiK10"              , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BdJpsiK0"           		   ,"hBdK2Pt_BdJpsiK0"               , 100, 0, 5                  ) );
vhBdK2Pt               .push_back( new TH1F ("hBdK2Pt_BpJpsiKp"           		   ,"hBdK2Pt_BpJpsiKp"               , 100, 0, 5                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BsJPsiPhiSignal"               ,"hBdDist3D_BsJPsiPhiSignal"      , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BsJPsiKKSignal"                ,"hBdDist3D_BsJPsiKKSignal"       , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BdJPsiKstarSignal"             ,"hBdDist3D_BdJPsiKstarSignal"    , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BsOther"                       ,"hBdDist3D_BsOther"              , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BdOther"                       ,"hBdDist3D_BdOther"              , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_Other"                         ,"hBdDist3D_Other"                , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BsJpsiEta"                     ,"hBdDist3D_BsJpsiEta"            , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BdJpsiK10"                     ,"hBdDist3D_BdJpsiK10"            , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BdJpsiK0"                      ,"hBdDist3D_BdJpsiK0"             , 100, 0, 1                  ) );
vhBdDist3D             .push_back( new TH1F ("hBdDist3D_BpJpsiKp"                  	   ,"hBdDist3D_BpJpsiKp"             , 100, 0, 1                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BsJPsiPhiSignal"           ,"hBdDistSign3D_BsJPsiPhiSignal"  , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BsJPsiKKSignal"            ,"hBdDistSign3D_BsJPsiKKSignal"   , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BdJPsiKstarSignal"         ,"hBdDistSign3D_BdJPsiKstarSignal", 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BsOther"                   ,"hBdDistSign3D_BsOther"          , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BdOther"                   ,"hBdDistSign3D_BdOther"          , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_Other"                     ,"hBdDistSign3D_Other"            , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BsJpsiEta"                 ,"hBdDistSign3D_BsJpsiEta"        , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BdJpsiK10"                 ,"hBdDistSign3D_BdJpsiK10"        , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BdJpsiK0"                  ,"hBdDistSign3D_BdJpsiK0"         , 100, 0, 50                  ) );
vhBdDistSign3D         .push_back( new TH1F ("hBdDistSign3D_BpJpsiKp"                  ,"hBdDistSign3D_BpJpsiKp"         , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BsJPsiPhiSignal"           ,"hBdDistSign1D_BsJPsiPhiSignal"  , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BsJPsiKKSignal"            ,"hBdDistSign1D_BsJPsiKKSignal"   , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BdJPsiKstarSignal"         ,"hBdDistSign1D_BdJPsiKstarSignal", 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BsOther"                   ,"hBdDistSign1D_BsOther"          , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BdOther"                   ,"hBdDistSign1D_BdOther"          , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_Other"                     ,"hBdDistSign1D_Other"            , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BsJpsiEta"                 ,"hBdDistSign1D_BsJpsiEta"        , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BdJpsiK10"                 ,"hBdDistSign1D_BdJpsiK10"        , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BdJpsiK0"                  ,"hBdDistSign1D_BdJpsiK0"         , 100, 0, 50                  ) );
vhBdDistSign1D         .push_back( new TH1F ("hBdDistSign1D_BpJpsiKp"                  ,"hBdDistSign1D_BpJpsiKp"         , 100, 0, 50                  ) );

vhBdChi2               .push_back( new TH1F ("hBdChi2_BsJPsiPhiSignal"                 ,"hBdChi2_BsJPsiPhiSignal"       , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BsJPsiKKSignal"                  ,"hBdChi2_BsJPsiKKSignal"        , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BdJPsiKstarSignal"               ,"hBdChi2_BdJPsiKstarSignal"     , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BsOther"                         ,"hBdChi2_BsOther"               , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BdOther"                         ,"hBdChi2_BdOther"               , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_Other"                           ,"hBdChi2_Other"                 , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BsJpsiEta"                       ,"hBdChi2_BsJpsiEta"             , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BdJpsiK10"                       ,"hBdChi2_BdJpsiK10"             , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BdJpsiK0"                        ,"hBdChi2_BdJpsiK0"              , 100, 0,30                   ) );
vhBdChi2               .push_back( new TH1F ("hBdChi2_BpJpsiKp"                        ,"hBdChi2_BpJpsiKp"              , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BsJPsiPhiSignal"             ,"hBdChi2Ndof_BsJPsiPhiSignal"   , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BsJPsiKKSignal"              ,"hBdChi2Ndof_BsJPsiKKSignal"    , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BdJPsiKstarSignal"           ,"hBdChi2Ndof_BdJPsiKstarSignal" , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BsOther"                     ,"hBdChi2Ndof_BsOther"           , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BdOther"                     ,"hBdChi2Ndof_BdOther"           , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_Other"                       ,"hBdChi2Ndof_Other"             , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BsJpsiEta"                   ,"hBdChi2Ndof_BsJpsiEta"         , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BdJpsiK10"                   ,"hBdChi2Ndof_BdJpsiK10"         , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BdJpsiK0"                    ,"hBdChi2Ndof_BdJpsiK0"          , 100, 0,30                   ) );
vhBdChi2Ndof           .push_back( new TH1F ("hBdChi2Ndof_BpJpsiKp"                    ,"hBdChi2Ndof_BpJpsiKp"          , 100, 0,30                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BsJPsiPhiSignal"            ,"hBdVtxProb_BsJPsiPhiSignal"  , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BsJPsiKKSignal"             ,"hBdVtxProb_BsJPsiKKSignal"   , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BdJPsiKstarSignal"          ,"hBdVtxProb_BdJPsiKstarSignal", 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BsOther"                    ,"hBdVtxProb_BsOther"          , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BdOther"                    ,"hBdVtxProb_BdOther"          , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_Other"                      ,"hBdVtxProb_Other"            , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BsJpsiEta"                  ,"hBdVtxProb_BsJpsiEta"        , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BdJpsiK10"                  ,"hBdVtxProb_BdJpsiK10"        , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BdJpsiK0"                   ,"hBdVtxProb_BdJpsiK0"         , 100, -0.5, 1.5                   ) );
vhBdVtxProb          .push_back( new TH1F ("hBdVtxProb_BpJpsiKp"                   ,"hBdVtxProb_BpJpsiKp"         , 100, -0.5, 1.5                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BsJPsiPhiSignal"                 ,"hBdTime_BsJPsiPhiSignal"   , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BsJPsiKKSignal"                  ,"hBdTime_BsJPsiKKSignal"    , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BdJPsiKstarSignal"               ,"hBdTime_BdJPsiKstarSignal" , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BsOther"                         ,"hBdTime_BsOther"           , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BdOther"                         ,"hBdTime_BdOther"           , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_Other"                           ,"hBdTime_Other"             , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BsJpsiEta"                       ,"hBdTime_BsJpsiEta"         , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BdJpsiK10"                       ,"hBdTime_BdJpsiK10"         , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BdJpsiK0"                        ,"hBdTime_BdJpsiK0"          , 100, -2,10                   ) );
vhBdTime               .push_back( new TH1F ("hBdTime_BpJpsiKp"                        ,"hBdTime_BpJpsiKp"          , 100, -2,10                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BsJPsiPhiSignal"            ,"hBdMass_NoTimeCut_BsJPsiPhiSignal"  , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BsJPsiKKSignal"             ,"hBdMass_NoTimeCut_BsJPsiKKSignal"   , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BdJPsiKstarSignal"          ,"hBdMass_NoTimeCut_BdJPsiKstarSignal", 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BsOther"                    ,"hBdMass_NoTimeCut_BsOther"          , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BdOther"                    ,"hBdMass_NoTimeCut_BdOther"          , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_Other"                      ,"hBdMass_NoTimeCut_Other"            , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BsJpsiEta"                  ,"hBdMass_NoTimeCut_BsJpsiEta"        , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BdJpsiK10"                  ,"hBdMass_NoTimeCut_BdJpsiK10"        , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BdJpsiK0"                   ,"hBdMass_NoTimeCut_BdJpsiK0"         , 100, 4.9, 5.7                   ) );
vhBdMass_NoTimeCut          .push_back( new TH1F ("hBdMass_NoTimeCut_BpJpsiKp"                   ,"hBdMass_NoTimeCut_BpJpsiKp"         , 100, 4.9, 5.7                   ) );


vhResoLxy         .push_back( new TH1F ("hResoLxy_BsJPsiPhiSignal"           ,"hResoLxy_BsJPsiPhiSignal"  , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BsJPsiKKSignal"            ,"hResoLxy_BsJPsiKKSignal"   , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BdJPsiKstarSignal"         ,"hResoLxy_BdJPsiKstarSignal", 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BsOther"                   ,"hResoLxy_BsOther"          , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BdOther"                   ,"hResoLxy_BdOther"          , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_Other"                     ,"hResoLxy_Other"            , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BsJpsiEta"                 ,"hResoLxy_BsJpsiEta"        , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BdJpsiK10"                 ,"hResoLxy_BdJpsiK10"        , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BdJpsiK0"                  ,"hResoLxy_BdJpsiK0"         , 100, -0.15, 0.15      ) );
vhResoLxy         .push_back( new TH1F ("hResoLxy_BpJpsiKp"                  ,"hResoLxy_BpJpsiKp"         , 100, -0.15, 0.15      ) );

vhResoCt         .push_back( new TH1F ("hResoCt_BsJPsiPhiSignal"           ,"hResoCt_BsJPsiPhiSignal"  , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BsJPsiKKSignal"            ,"hResoCt_BsJPsiKKSignal"   , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BdJPsiKstarSignal"         ,"hResoCt_BdJPsiKstarSignal", 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BsOther"                   ,"hResoCt_BsOther"          , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BdOther"                   ,"hResoCt_BdOther"          , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_Other"                     ,"hResoCt_Other"            , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BsJpsiEta"                 ,"hResoCt_BsJpsiEta"        , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BdJpsiK10"                 ,"hResoCt_BdJpsiK10"        , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BdJpsiK0"                  ,"hResoCt_BdJpsiK0"         , 100, -0.15, 0.15      ) );
vhResoCt         .push_back( new TH1F ("hResoCt_BpJpsiKp"                  ,"hResoCt_BpJpsiKp"         , 100, -0.15, 0.15      ) );

vhResoTime         .push_back( new TH1F ("hResoTime_BsJPsiPhiSignal"           ,"hResoTime_BsJPsiPhiSignal"  , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BsJPsiKKSignal"            ,"hResoTime_BsJPsiKKSignal"   , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BdJPsiKstarSignal"         ,"hResoTime_BdJPsiKstarSignal", 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BsOther"                   ,"hResoTime_BsOther"          , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BdOther"                   ,"hResoTime_BdOther"          , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_Other"                     ,"hResoTime_Other"            , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BsJpsiEta"                 ,"hResoTime_BsJpsiEta"        , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BdJpsiK10"                 ,"hResoTime_BdJpsiK10"        , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BdJpsiK0"                  ,"hResoTime_BdJpsiK0"         , 100, -0.1, 0.1      ) );
vhResoTime         .push_back( new TH1F ("hResoTime_BpJpsiKp"                  ,"hResoTime_BpJpsiKp"         , 100, -0.1, 0.1      ) );

vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BsJPsiPhiSignal"           ,"hResoRefitLxy_BsJPsiPhiSignal"  , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BsJPsiKKSignal"            ,"hResoRefitLxy_BsJPsiKKSignal"   , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BdJPsiKstarSignal"         ,"hResoRefitLxy_BdJPsiKstarSignal", 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BsOther"                   ,"hResoRefitLxy_BsOther"          , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BdOther"                   ,"hResoRefitLxy_BdOther"          , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_Other"                     ,"hResoRefitLxy_Other"            , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BsJpsiEta"                 ,"hResoRefitLxy_BsJpsiEta"        , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BdJpsiK10"                 ,"hResoRefitLxy_BdJpsiK10"        , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BdJpsiK0"                  ,"hResoRefitLxy_BdJpsiK0"         , 100, -0.15, 0.15      ) );
vhResoRefitLxy         .push_back( new TH1F ("hResoRefitLxy_BpJpsiKp"                  ,"hResoRefitLxy_BpJpsiKp"         , 100, -0.15, 0.15      ) );

vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BsJPsiPhiSignal"           ,"hResoRefitCt_BsJPsiPhiSignal"  , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BsJPsiKKSignal"            ,"hResoRefitCt_BsJPsiKKSignal"   , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BdJPsiKstarSignal"         ,"hResoRefitCt_BdJPsiKstarSignal", 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BsOther"                   ,"hResoRefitCt_BsOther"          , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BdOther"                   ,"hResoRefitCt_BdOther"          , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_Other"                     ,"hResoRefitCt_Other"            , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BsJpsiEta"                 ,"hResoRefitCt_BsJpsiEta"        , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BdJpsiK10"                 ,"hResoRefitCt_BdJpsiK10"        , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BdJpsiK0"                  ,"hResoRefitCt_BdJpsiK0"         , 100, -0.15, 0.15      ) );
vhResoRefitCt         .push_back( new TH1F ("hResoRefitCt_BpJpsiKp"                  ,"hResoRefitCt_BpJpsiKp"         , 100, -0.15, 0.15      ) );

vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BsJPsiPhiSignal"           ,"hResoRefitTime_BsJPsiPhiSignal"  , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BsJPsiKKSignal"            ,"hResoRefitTime_BsJPsiKKSignal"   , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BdJPsiKstarSignal"         ,"hResoRefitTime_BdJPsiKstarSignal", 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BsOther"                   ,"hResoRefitTime_BsOther"          , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BdOther"                   ,"hResoRefitTime_BdOther"          , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_Other"                     ,"hResoRefitTime_Other"            , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BsJpsiEta"                 ,"hResoRefitTime_BsJpsiEta"        , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BdJpsiK10"                 ,"hResoRefitTime_BdJpsiK10"        , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BdJpsiK0"                  ,"hResoRefitTime_BdJpsiK0"         , 100, -0.1, 0.1      ) );
vhResoRefitTime         .push_back( new TH1F ("hResoRefitTime_BpJpsiKp"                  ,"hResoRefitTime_BpJpsiKp"         , 100, -0.1, 0.1      ) );

vhBsCt             .push_back( new TH1F ("hBsCt_BsJPsiPhiSignal"               ,"hBsCt_BsJPsiPhiSignal"      , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BsJPsiKKSignal"                ,"hBsCt_BsJPsiKKSignal"       , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BdJPsiKstarSignal"             ,"hBsCt_BdJPsiKstarSignal"    , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BsOther"                       ,"hBsCt_BsOther"              , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BdOther"                       ,"hBsCt_BdOther"              , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_Other"                         ,"hBsCt_Other"                , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BsJpsiEta"                     ,"hBsCt_BsJpsiEta"            , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BdJpsiK10"                     ,"hBsCt_BdJpsiK10"            , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BdJpsiK0"                      ,"hBsCt_BdJpsiK0"             , 100, -0.05, 0.5                  ) );
vhBsCt             .push_back( new TH1F ("hBsCt_BpJpsiKp"                          ,"hBsCt_BpJpsiKp"             , 100, -0.05, 0.5                  ) );

vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BsJPsiPhiSignal"               ,"hBsCtErr_BsJPsiPhiSignal"      , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BsJPsiKKSignal"                ,"hBsCtErr_BsJPsiKKSignal"       , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BdJPsiKstarSignal"             ,"hBsCtErr_BdJPsiKstarSignal"    , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BsOther"                       ,"hBsCtErr_BsOther"              , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BdOther"                       ,"hBsCtErr_BdOther"              , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_Other"                         ,"hBsCtErr_Other"                , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BsJpsiEta"                     ,"hBsCtErr_BsJpsiEta"            , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BdJpsiK10"                     ,"hBsCtErr_BdJpsiK10"            , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BdJpsiK0"                      ,"hBsCtErr_BdJpsiK0"             , 100, 0., 0.03                  ) );
vhBsCtErr             .push_back( new TH1F ("hBsCtErr_BpJpsiKp"                            ,"hBsCtErr_BpJpsiKp"             , 100, 0., 0.03                  ) );

}



void makeVertexResolutionPlots(){

  double genDx = tree->genBsVtx_x_ - tree->genBsSVtx_x_;
  double genDy = tree->genBsVtx_y_ - tree->genBsSVtx_y_;
 
  double genLxy = sqrt(genDx*genDx + genDy*genDy);

  double genM, genPt;
  if(abs(tree->BmesonsId_[0]) == 531){
    genM = tree->BMMC_[0];
    genPt= tree->BPtMC_[0];
  }
  else{
    genM = tree->BMMC_[1];
    genPt= tree->BPtMC_[1];
  }
  //  double genTime = genLxy * genM / genPt;
  double genCt   =  genLxy * genM / genPt;

  double recoLxyRefit = fabs(tree->BsLxy_ );
  double recoCtRefit  = fabs(tree->BsCt_ );
  // double recoTime = recoLxy * tree->BsFitM_ / tree->BsFitPt_;

  fillHistograms(recoLxyRefit - genLxy, vhResoRefitLxy );
  fillHistograms(recoCtRefit - genCt,   vhResoRefitCt );
  

  // now for the vertex without refit
  double recDx = tree->PVx_ - tree->BsFitVtx_x_;
  double recDy = tree->PVy_ - tree->BsFitVtx_y_;
  double recLxy= sqrt(recDx*recDx + recDy*recDy);
  double recCt = recLxy *  tree->BsFitM_ / tree->BsFitPt_;

  fillHistograms(recLxy - genLxy , vhResoLxy);
  fillHistograms(recCt - genCt , vhResoCt);

 //  fillHistograms(recoTime - genTime, vhResoTime);

}


