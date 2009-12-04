#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

// important: define root tree variable list outside of main function to make it accessible from subfunctions.
#include "VariableList.h"


// declare some functions. These are implemented at the end of this file
void increaseCountersByOne(int &BsJpsiPhiSignal, int& BsJpsiKKSignal, int& BdJpsiKstar, int& BsOther, int& BdOther,  int& other, 
			   int &BsJpsiEta, int &BdJpsiK10, int &BdJpsiK0, int &BpJpsiKp  );
void fillHistograms(double value, TH1F* hBsJpsiPhiSignal, TH1F* hBsOther, TH1F * hOther, TH1F *hBsJPsiKK = 0);
void fillHistograms(double value, vector<TH1F*> &histoVector);  // pass vector of histograms for the various categories

void setBranchAddresses(TTree *tree);
//void bookHistos();
void bookHistoVectors();
void writeHistos(TString outputfilename);
void identifyEventType(bool & isGenBsJpsiPhiMuMuKKEvent, bool &isGenBsJpsiKKEvent, bool &isGenBdJpsiKstarMuMuKpiEvent, 
		       bool &isGenBsEvent_, bool &isGenBdEvent_, bool &isGenBsJpsiEtaEvent_, bool &isGenBdJpsiK10Event_, 
		       bool &isGenBdJpsiK0Event_, bool & isGenBpJpsiKpEvent_);
void printDecayTree();

// define cuts:
const double minProbability = 0.02;
const double KaonPtCut = 0.9;
const double nominalPhiMass = 1.0195 ;
const double phiMassWindow = 0.007;
const double decayLengthCut = 3;
const double pointingCut = 0.8;

// maximum number of event to loop over. -1 means all events.
const int maxNumEvents = -1;
// number of categories
const int iNumberOfCategories_ = 10;

// ***************main function*****************************
void BsAnalysisNew() {
  
  //  TFile *f = new TFile("BtoJPsiMuMu_Nov30.root");
  //    TFile *f = new TFile("ppmumuX.root");
  //  TFile *f = new TFile("ppmuX.root");
    TFile *f = new TFile("JPsiMuMu_Nov30.root");
  
    // TString outputhistofilename = "BtoJPsiMuMuHistoFile.root";
  //    TString outputhistofilename = "ppmumuXHistoFile.root";
  //        TString outputhistofilename = "ppmuXHistoFile.root";
  TString outputhistofilename = "JPsiMuMuHistoFile.root";
  

  // booking histograms
  bookHistoVectors();  

 
  // get tree from file
  TTree *tree = (TTree*)f->Get("BsTree");
  
  // call function to set branch addresses
  setBranchAddresses(tree);

  int nentries = tree->GetEntries();
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
    tree->GetEntry(i);
    
    // identify event type
    identifyEventType(isGenBsJpsiPhiMuMuKKEvent_, isGenBsJpsiKKEvent_, isGenBdJpsiKstarMuMuKpiEvent_, isGenBsEvent_, isGenBdEvent_, isGenBsJpsiEtaEvent_, isGenBdJpsiK10Event_, 
		      isGenBdJpsiK0Event_, isGenBpJpsiKpEvent_);

    // print decay tree
    //    printDecayTree();

   
    // count number of events of each type
    increaseCountersByOne(iBsJPsiPhiSignalEvents, iBsJPsiKKSignalEvents, iBdJPsiKstarSignalEvents, iBsOtherEvents, iBdOtherEvents, iOtherEvents, 
			  iBsJpsiEtaEvents, iBdJpsiK10Events, iBdJpsiK0Events, iBpJpsiKpEvents);
    
    if(triggerbit_HLTdoubleMu3_ != 1) continue;      // IS TRIGGERED_HLTdoubleMu3**************
    HLT++;
    
    // count number of triggered events of each type
    increaseCountersByOne(iTriggered_DoubleMu3_BsJPsiPhiSignalEvents,  iTriggered_DoubleMu3_BsJpsiKK, 
			  iTriggered_DoubleMu3_BdJpsiKstar,            iTriggered_DoubleMu3_BsOtherEvents, 
			  iTriggered_DoubleMu3_BdOtherEvents,          iTriggered_DoubleMu3_OtherEvents,
			  iTriggered_DoubleMu3_BsJpsiEtaEvents,        iTriggered_DoubleMu3_BdJpsiK10Events,
			  iTriggered_DoubleMu3_BdJpsiK0Events,         iTriggered_DoubleMu3_BpJpsiKpEvents);
    
     
    if(iPassedCutIdent_ < 7) continue;   // passed  pre KinFit mass cuts
    increaseCountersByOne(iBsJPsiPhiSignalEventsPreKinFit, iBsJPsiKKSignalEventsPreKinFit, iBdJPsiKstarSignalEventsPreKinFit,
			  iBsOtherEventsPreKinFit, iBdOtherEventsPreKinFit, iOtherEventsPreKinFit,
			  iBsJpsiEtaEventsPreKinFit, iBdJpsiK10EventsPreKinFit, iBdJpsiK0EventsPreKinFit, iBpJpsiKpEventsPreKinFit);
    
    
    if(iPassedCutIdent_ < 13) continue; // passed kinematic fit  
    increaseCountersByOne(iBsJPsiPhiSignalEventsOfflineSel1, iBsJPsiKKSignalEventsOfflineSel1, iBdJPsiKstarSignalEventsOfflineSel1,
			  iBsOtherEventsOfflineSel1, iBdOtherEventsOfflineSel1, iOtherEventsOfflineSel1,
			  iBsJpsiEtaEventsOfflineSel1, iBdJpsiK10EventsOfflineSel1, iBdJpsiK0EventsOfflineSel1, iBpJpsiKpEventsOfflineSel1);
    
  
    // successful fit, fill histograms
    fillHistograms(JpsiMass_after_,     vhJPsiMass); 
    fillHistograms(PhiMass_after_,      vhPhiMass); 
    fillHistograms(Dist3d_,             vhDist3D); 
    fillHistograms(chi2_Bs_,            vhChi2);
    fillHistograms(chi2_Bs_ / ndof_Bs_ , vhChi2Ndof);
    fillHistograms(BsVtxProb_,          vhBsVtxProb );
    fillHistograms(K1Pt_after_,         vhK1Pt);
    fillHistograms(K2Pt_after_,         vhK2Pt);
    fillHistograms(Dist3d_ / dDist3d_ , vhDistSign3D);
    fillHistograms(Dist_ / dDist_    ,  vhDistSign1D);
    fillHistograms(AngleBsDecayLength_, vhPointingAngle );


    if(BsVtxProb_ < minProbability) continue; // passed minimum vertex probability cut
    increaseCountersByOne(iBsJPsiPhiSignalEventsProbVertex, iBsJPsiKKSignalEventsProbVertex, iBdJPsiKstarSignalEventsProbVertex,
			  iBsOtherEventsProbVertex, iBdOtherEventsProbVertex, iOtherEventsProbVertex,
			  iBsJpsiEtaEventsProbVertex, iBdJpsiK10EventsProbVertex, iBdJpsiK0EventsProbVertex, iBpJpsiKpEventsProbVertex);
    
    if(K1Pt_after_ < KaonPtCut  || K2Pt_after_ < KaonPtCut) continue;   // passed Kaon pt cut
    increaseCountersByOne(iBsJPsiPhiSignalEventsKaonPtCut, iBsJPsiKKSignalEventsKaonPtCut, iBdJPsiKstarSignalEventsKaonPtCut, 
			  iBsOtherEventsKaonPtCut, iBdOtherEventsKaonPtCut, iOtherEventsKaonPtCut,
			  iBsJpsiEtaEventsKaonPtCut, iBdJpsiK10EventsKaonPtCut, iBdJpsiK0EventsKaonPtCut, iBpJpsiKpEventsKaonPtCut);
    
    if( fabs(PhiMass_after_ - nominalPhiMass) > phiMassWindow) continue;   // passed tight phi mass cut
    increaseCountersByOne(iBsJPsiPhiSignalEventsPhiMassCut, iBsJPsiKKSignalEventsPhiMassCut, iBdJPsiKstarSignalEventsPhiMassCut, 
			  iBsOtherEventsPhiMassCut, iBdOtherEventsPhiMassCut, iOtherEventsPhiMassCut,
			  iBsJpsiEtaEventsPhiMassCut, iBdJpsiK10EventsPhiMassCut, iBdJpsiK0EventsPhiMassCut, iBpJpsiKpEventsPhiMassCut);
    
    if(Dist_/ dDist_ < decayLengthCut) continue;  // passed transverse decay length cut
    increaseCountersByOne(iBsJPsiPhiSignalEventsDecayLengthCut, iBsJPsiKKSignalEventsDecayLengthCut, iBdJPsiKstarSignalEventsDecayLengthCut, 
			  iBsOtherEventsDecayLengthCut, iBdOtherEventsDecayLengthCut, iOtherEventsDecayLengthCut,
			  iBsJpsiEtaEventsDecayLengthCut, iBdJpsiK10EventsDecayLengthCut, iBdJpsiK0EventsDecayLengthCut, iBpJpsiKpEventsDecayLengthCut);
    
    if(AngleBsDecayLength_ < pointingCut) continue;  // passed pointing constraint
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
    fillHistograms(PhiMass_after_,    vhPhiMassFinal);
    fillHistograms(  BsMass_after_ ,  vhBsMassFinal);
    fillHistograms( BfitM_KK_,        vhBsMassFinalAfterFit);
  }
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

 


 // write histograms to output file
 writeHistos(outputhistofilename);

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
  for(int i=0; i < GenNumberOfBdecays_; i++){
    int B_id = abs(BmesonsId_[i]);
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
  

    for(int j=0; j < GenNumberOfDaughters_[i]; j++){
      int Dau_id = BDauIdMC_[i][j];
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
      for(int k=0; k < GenNumberOfDaughtersDaughters_[i][j]; k++){
	int DauDau_id = BDauDauIdMC_[i][j][k];
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
    if(GenNumberOfDaughters_[i] == 2 && BsJpsiMuMinus && BsJpsiMuPlus && BsPhiKplus && BsPhiKminus) isGenBsJpsiPhiMuMuKKEvent = 1;
    if(GenNumberOfDaughters_[i] == 3 && BsJpsiMuMinus && BsJpsiMuPlus && BsKplus && BsKminus)       isGenBsJpsiKKEvent = 1;  
    if(GenNumberOfDaughters_[i] == 2 && BdJpsiMuMinus && BdJpsiMuPlus && BdKstarPi && BdKstarK )    isGenBdJpsiKstarMuMuKpiEvent = 1;
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
  for(int i=0; i<GenNumberOfBdecays_; i++){
    std::cout<<"B pdg id = " << BmesonsId_[i] << std::endl;
    for(int j=0; j<GenNumberOfDaughters_[i]; j++){
      std::cout<<"  Dau pdg id = " << BDauIdMC_[i][j] << std::endl;
      for(int k=0; k<GenNumberOfDaughtersDaughters_[i][j]; k++){
	std::cout<<"      DauDau pdg id = " << BDauDauIdMC_[i][j][k] << std::endl;
      }
    }
  }
}
//-------------------

// function to set branch addresses
void setBranchAddresses(TTree *tree){
  
   tree->SetBranchAddress("triggerbit_HLTmu3",                                 &triggerbit_HLTmu3_);    
   tree->SetBranchAddress("triggerbit_HLTmu5",				       &triggerbit_HLTmu5_);                                                  
   tree->SetBranchAddress("triggerbit_HLTmu9",				       &triggerbit_HLTmu9_);                                                  
   tree->SetBranchAddress("triggerbit_HLTdoubleIsoMu3",			       &triggerbit_HLTdoubleIsoMu3_);                                         
   tree->SetBranchAddress("triggerbit_HLTdoubleMu3",			       &triggerbit_HLTdoubleMu3_);                                            
   tree->SetBranchAddress("triggerbit_HLTdoubleMu3_JPsi",		       &triggerbit_HLTdoubleMu3_JPsi_);                                       
   tree->SetBranchAddress("BeamspotX" ,					       &BSx_) ;                                                               
   tree->SetBranchAddress("BeamspotY" ,					       &BSy_ );                                                               
   tree->SetBranchAddress("BeamspotZ" ,					       &BSz_ );                                                               
   tree->SetBranchAddress("PVx" ,					       &PVx_ );                                                               
   tree->SetBranchAddress("PVy" ,					       &PVy_ );                                                               
   tree->SetBranchAddress("PVz" ,					       &PVz_ );                                                               
   tree->SetBranchAddress("PVerrx" ,					       &PVerrx_ );                                                            
   tree->SetBranchAddress("PVerry" ,					       &PVerry_ );                                                            
   tree->SetBranchAddress("PVerrz" ,					       &PVerrz_ );                                                            
   tree->SetBranchAddress("JpsiVtxProb",				       &JpsiVtxProb_);                                                        
   tree->SetBranchAddress("JpsiM_alone",				       &JpsiM_alone_);                                                        
   tree->SetBranchAddress("JpsiPhi_alone",				       &JpsiPhi_alone_);                                                      
   tree->SetBranchAddress("JpsiEta_alone",				       &JpsiEta_alone_);                                                      
   tree->SetBranchAddress("JpsiPt_alone",				       &JpsiPt_alone_);                                                       
   tree->SetBranchAddress("JpsiMu1Pt",					       &JpsiMu1Pt_);                                                          
   tree->SetBranchAddress("JpsiMu2Pt",					       &JpsiMu2Pt_);                                                          
   tree->SetBranchAddress("JpsiMu1Phi",					       &JpsiMu1Phi_);                                                         
   tree->SetBranchAddress("JpsiMu2Phi",					       &JpsiMu2Phi_);                                                         
   tree->SetBranchAddress("JpsiMu1Eta",					       &JpsiMu1Eta_);                                                         
   tree->SetBranchAddress("JpsiMu2Eta",					       &JpsiMu2Eta_);                                                         
   tree->SetBranchAddress("JpsiMuon1Cat",				       &JpsiMuon1Cat_);                                                       
   tree->SetBranchAddress("JpsiMuon2Cat",				       &JpsiMuon2Cat_);                                                       
   tree->SetBranchAddress("BsMass_before",				       &BsMass_before_);                                                      
   tree->SetBranchAddress("BsPhi_before",				       &BsPhi_before_);                                                       
   tree->SetBranchAddress("BsEta_before",				       &BsEta_before_);                                                       
   tree->SetBranchAddress("BsPt_before",				       &BsPt_before_);                                                        
   tree->SetBranchAddress("BsPz_before",				       &BsPz_before_);                                                        
   tree->SetBranchAddress("JpsiMass_before",				       &JpsiMass_before_);                                                    
   tree->SetBranchAddress("JpsiPhi_before",				       &JpsiPhi_before_);                                                     
   tree->SetBranchAddress("JpsiEta_before",				       &JpsiEta_before_);                                                     
   tree->SetBranchAddress("JpsiPt_before",				       &JpsiPt_before_);                                                      
   tree->SetBranchAddress("JpsiPz_before",				       &JpsiPz_before_);                                                      
   tree->SetBranchAddress("PhiMass_before",				       &PhiMass_before_);                                                     
   tree->SetBranchAddress("PhiPhi_before",				       &PhiPhi_before_);                                                      
   tree->SetBranchAddress("PhiEta_before",				       &PhiEta_before_);                                                      
   tree->SetBranchAddress("PhiPt_before",				       &PhiPt_before_);                                                       
   tree->SetBranchAddress("PhiPz_before",				       &PhiPz_before_);                                                       
   tree->SetBranchAddress("K1Pt_before",				       &K1Pt_before_);                                                        
   tree->SetBranchAddress("K1Pz_before",				       &K1Pz_before_);                                                        
   tree->SetBranchAddress("K1Eta_before",				       &K1Eta_before_);                                                       
   tree->SetBranchAddress("K1Phi_before",				       &K1Phi_before_);                                                       
   tree->SetBranchAddress("K2Eta_before",				       &K2Eta_before_);                                                       
   tree->SetBranchAddress("K2Pt_before",				       &K2Pt_before_);                                                        
   tree->SetBranchAddress("K2Pz_before",				       &K2Pz_before_);                                                        
   tree->SetBranchAddress("K2Phi_before",				       &K2Phi_before_);                                                       
   tree->SetBranchAddress("chi2_Bs",					       &chi2_Bs_);                                                            
   tree->SetBranchAddress("ndof_Bs",					       &ndof_Bs_);                                                            
   tree->SetBranchAddress("BsVtxProb",					       &BsVtxProb_);                                                          
   tree->SetBranchAddress("BsVtxProbKpi",				       &BsVtxProbKpi_);                                                       
   tree->SetBranchAddress("BsVtxProbpipi",				       &BsVtxProbpipi_);                                                      
   tree->SetBranchAddress("BFitM_KK",					       &BfitM_KK_);                                                           
   tree->SetBranchAddress("BFitM_Kpi",					       &BfitM_Kpi_);                                                          
   tree->SetBranchAddress("BFitM_pipi",					       &BfitM_pipi_);                                                         
   tree->SetBranchAddress("BsVtx_x",					       &BsVtx_x_);                                                            
   tree->SetBranchAddress("BsVtx_y",					       &BsVtx_y_);                                                            
   tree->SetBranchAddress("BsVtx_z",					       &BsVtx_z_);                                                            
   tree->SetBranchAddress("BsMass_after",				       &BsMass_after_);                                                       
   tree->SetBranchAddress("BsPhi_after",				       &BsPhi_after_);                                                        
   tree->SetBranchAddress("BsEta_after",				       &BsEta_after_);                                                        
   tree->SetBranchAddress("BsPt_after",					       &BsPt_after_);                                                         
   tree->SetBranchAddress("BsPz_after",					       &BsPz_after_);                                                         
   tree->SetBranchAddress("JpsiMass_after",				       &JpsiMass_after_);                                                     
   tree->SetBranchAddress("JpsiPhi_after",				       &JpsiPhi_after_);                                                      
   tree->SetBranchAddress("JpsiEta_after",				       &JpsiEta_after_);                                                      
   tree->SetBranchAddress("JpsiPt_after",				       &JpsiPt_after_);                                                       
   tree->SetBranchAddress("JpsiPz_after",				       &JpsiPz_after_);                                                       
   tree->SetBranchAddress("PhiMass_after",				       &PhiMass_after_);                                                      
   tree->SetBranchAddress("PhiPhi_after",				       &PhiPhi_after_);                                                       
   tree->SetBranchAddress("PhiEta_after",				       &PhiEta_after_);                                                       
   tree->SetBranchAddress("PhiPt_after",				       &PhiPt_after_);                                                        
   tree->SetBranchAddress("PhiPz_after",				       &PhiPz_after_);                                                        
   tree->SetBranchAddress("K1Pt_after",					       &K1Pt_after_);                                                         
   tree->SetBranchAddress("K1Pz_after",					       &K1Pz_after_);                                                         
   tree->SetBranchAddress("K1Eta_after",				       &K1Eta_after_);                                                        
   tree->SetBranchAddress("K1Phi_after",				       &K1Phi_after_);                                                        
   tree->SetBranchAddress("K2Eta_after",				       &K2Eta_after_);                                                        
   tree->SetBranchAddress("K2Pt_after",					       &K2Pt_after_);                                                         
   tree->SetBranchAddress("K2Pz_after",					       &K2Pz_after_);                                                         
   tree->SetBranchAddress("K2Phi_after",				       &K2Phi_after_);                                                        
   tree->SetBranchAddress("K1Chi2",					       &K1Chi2_);                                                             
//    tree->SetBranchAddress("K1nHits",					       &K1nHits_);                                                            
//    tree->SetBranchAddress("K2Chi2",					       &K2Chi2_);                                                             
//    tree->SetBranchAddress("K2nHits",					       &K2nHits_);                                                            
//    tree->SetBranchAddress("K1pixH",					       &K1pixH_);                                                             
//    tree->SetBranchAddress("K1trkH",					       &K1trkH_);                                                             
//    tree->SetBranchAddress("K2pixH",					       &K2pixH_);                                                             
//    tree->SetBranchAddress("K2trkH",					       &K2trkH_);                                                             
//    tree->SetBranchAddress("Mu1Chi2",					       &Mu1Chi2_);                                                            
//    tree->SetBranchAddress("Mu1nHits",					       &Mu1nHits_);                                                           
//    tree->SetBranchAddress("Mu2Chi2",					       &Mu2Chi2_);                                                            
//    tree->SetBranchAddress("Mu2nHits",					       &Mu2nHits_);                                                           
//    tree->SetBranchAddress("Mu1pixH",					       &Mu1pixH_);                                                            
//    tree->SetBranchAddress("Mu1trkH",					       &Mu1trkH_);                                                            
//    tree->SetBranchAddress("Mu2pixH",					       &Mu2pixH_);                                                            
//    tree->SetBranchAddress("Mu2trkH",					       &Mu2trkH_);                                                            
   tree->SetBranchAddress("costheta",					       &costheta_);                                                           
   tree->SetBranchAddress("phi",					       &phi_);                                                                
   tree->SetBranchAddress("cospsi",					       &cospsi_);                                                             
   tree->SetBranchAddress("AngleBsDecayLength",				       &AngleBsDecayLength_);                                                 
   tree->SetBranchAddress("isMatched",					       &isMatched_);                                                          
   tree->SetBranchAddress("isMatchedBd",				       &isMatchedBd_);                                                        
   tree->SetBranchAddress("BLxy",					       &BLxy_);                                                               
   tree->SetBranchAddress("BLxy2",					       &BLxy2_);                                                              
   tree->SetBranchAddress("BerrX",					       &BerrX_);                                                              
   tree->SetBranchAddress("BerrY",					       &BerrY_);                                                              
   tree->SetBranchAddress("BerrXY",					       &BerrXY_);                                                             
   tree->SetBranchAddress("Bsct1",					       &Bsct1_);                                                              
   tree->SetBranchAddress("Bsct2",					       &Bsct2_);                                                              
   tree->SetBranchAddress("K1trkLay",					       &K1trkLay_);                                                           
   tree->SetBranchAddress("K1muDTh",					       &K1muDTh_);                                                            
   tree->SetBranchAddress("K1muCSCh",					       &K1muCSCh_);                                                           
   tree->SetBranchAddress("K1muRPCh",					       &K1muRPCh_);                                                           
   tree->SetBranchAddress("K2trkLay",					       &K2trkLay_);                                                           
   tree->SetBranchAddress("K2muDTh",					       &K2muDTh_);                                                            
   tree->SetBranchAddress("K2muCSCh",					       &K2muCSCh_);                                                           
   tree->SetBranchAddress("K2muRPCh",					       &K2muRPCh_);                                                           
   tree->SetBranchAddress("Mu1trkLay",					       &Mu1trkLay_);                                                          
   tree->SetBranchAddress("Mu1muDTh",					       &Mu1muDTh_);                                                           
   tree->SetBranchAddress("Mu1muCSCh",					       &Mu1muCSCh_);                                                          
   tree->SetBranchAddress("Mu1muRPCh",					       &Mu1muRPCh_);                                                          
   tree->SetBranchAddress("Mu2trkLay",					       &Mu2trkLay_);                                                          
   tree->SetBranchAddress("Mu2muDTh",					       &Mu2muDTh_);                                                           
   tree->SetBranchAddress("Mu2muCSCh",					       &Mu2muCSCh_);                                                          
   tree->SetBranchAddress("Mu2muRPCh",					       &Mu2muRPCh_);                                                          
   tree->SetBranchAddress("K1mcId",					       &K1mcId_);                                                             
   tree->SetBranchAddress("K1momId",					       &K1momId_);                                                            
   tree->SetBranchAddress("K1gmomId",					       &K1gmomId_);                                                           
   tree->SetBranchAddress("K2mcId",					       &K2mcId_);                                                             
   tree->SetBranchAddress("K2momId",					       &K2momId_);                                                            
   tree->SetBranchAddress("K2gmomId",					       &K2gmomId_);                                                           
   tree->SetBranchAddress("Mu1mcId",					       &Mu1mcId_);                                                            
   tree->SetBranchAddress("Mu1momId",					       &Mu1momId_);                                                           
   tree->SetBranchAddress("Mu1gmomId",					       &Mu1gmomId_);                                                          
   tree->SetBranchAddress("Mu2mcId",					       &Mu2mcId_);                                                            
   tree->SetBranchAddress("Mu2momId",					       &Mu2momId_);                                                           
   tree->SetBranchAddress("Mu2gmomId",					       &Mu2gmomId_);                                                          
   tree->SetBranchAddress("K1Truth",					       &K1Truth_);                                                            
   tree->SetBranchAddress("K2Truth",					       &K2Truth_);                                                            
   tree->SetBranchAddress("Mu1Truth",					       &Mu1Truth_);                                                           
   tree->SetBranchAddress("Mu2Truth",					       &Mu2Truth_);                                                           
   tree->SetBranchAddress("Dist3d",					       &Dist3d_);                                                             
   tree->SetBranchAddress("dDist3d",					       &dDist3d_);                                                            
   tree->SetBranchAddress("Time3d",					       &Time3d_);                                                             
   tree->SetBranchAddress("dTime3d",					       &dTime3d_);                                                            
   tree->SetBranchAddress("Dist",					       &Dist_);                                                               
   tree->SetBranchAddress("dDist",					       &dDist_);                                                              
   tree->SetBranchAddress("Time",					       &Time_);                                                               
   tree->SetBranchAddress("dTime",					       &dTime_);                                                              
   tree->SetBranchAddress("dedxTrk",					       &dedxTrk_);                                                            
   tree->SetBranchAddress("errdedxTrk",					       &errdedxTrk_);                                                         
   tree->SetBranchAddress("numdedxTrk",					       &numdedxTrk_);                                                         
   tree->SetBranchAddress("iPassedCutIdent",				       &iPassedCutIdent_);                                                    
   tree->SetBranchAddress("iPassedCutIdentBd",				       &iPassedCutIdentBd_);                                                  
 


 tree->SetBranchAddress("K1_par0KK", &K1_kk_par0_ );
  tree->SetBranchAddress("K1_par1KK", &K1_kk_par1_);
  tree->SetBranchAddress("K1_par2KK", &K1_kk_par2_);
  tree->SetBranchAddress("K1_par3KK", &K1_kk_par3_);
  tree->SetBranchAddress("K1_par4KK", &K1_kk_par4_);
  tree->SetBranchAddress("K1_par5KK", &K1_kk_par5_);
  tree->SetBranchAddress("K1_par6KK", &K1_kk_par6_);

  tree->SetBranchAddress("K2_par0KK", &K2_kk_par0_);
  tree->SetBranchAddress("K2_par1KK", &K2_kk_par1_);
  tree->SetBranchAddress("K2_par2KK", &K2_kk_par2_);
  tree->SetBranchAddress("K2_par3KK", &K2_kk_par3_);
  tree->SetBranchAddress("K2_par4KK", &K2_kk_par4_);
  tree->SetBranchAddress("K2_par5KK", &K2_kk_par5_);
  tree->SetBranchAddress("K2_par6KK", &K2_kk_par6_);

  tree->SetBranchAddress("K1_par0Kpi", &K1_kpi_par0_);
  tree->SetBranchAddress("K1_par1Kpi", &K1_kpi_par1_);
  tree->SetBranchAddress("K1_par2Kpi", &K1_kpi_par2_);
  tree->SetBranchAddress("K1_par3Kpi", &K1_kpi_par3_);
  tree->SetBranchAddress("K1_par4Kpi", &K1_kpi_par4_);
  tree->SetBranchAddress("K1_par5Kpi", &K1_kpi_par5_);
  tree->SetBranchAddress("K1_par6Kpi", &K1_kpi_par6_);

  tree->SetBranchAddress("K2_par0Kpi", &K2_kpi_par0_);
  tree->SetBranchAddress("K2_par1Kpi", &K2_kpi_par1_);
  tree->SetBranchAddress("K2_par2Kpi", &K2_kpi_par2_);
  tree->SetBranchAddress("K2_par3Kpi", &K2_kpi_par3_);
  tree->SetBranchAddress("K2_par4Kpi", &K2_kpi_par4_);
  tree->SetBranchAddress("K2_par5Kpi", &K2_kpi_par5_);
  tree->SetBranchAddress("K2_par6Kpi", &K2_kpi_par6_);

  tree->SetBranchAddress("K1_par0pipi", &K1_pipi_par0_);
  tree->SetBranchAddress("K1_par1pipi", &K1_pipi_par1_);
  tree->SetBranchAddress("K1_par2pipi", &K1_pipi_par2_);
  tree->SetBranchAddress("K1_par3pipi", &K1_pipi_par3_);
  tree->SetBranchAddress("K1_par4pipi", &K1_pipi_par4_);
  tree->SetBranchAddress("K1_par5pipi", &K1_pipi_par5_);
  tree->SetBranchAddress("K1_par6pipi", &K1_pipi_par6_);

  tree->SetBranchAddress("K2_par0pipi", &K2_pipi_par0_);
  tree->SetBranchAddress("K2_par1pipi", &K2_pipi_par1_);
  tree->SetBranchAddress("K2_par2pipi", &K2_pipi_par2_);
  tree->SetBranchAddress("K2_par3pipi", &K2_pipi_par3_);
  tree->SetBranchAddress("K2_par4pipi", &K2_pipi_par4_);
  tree->SetBranchAddress("K2_par5pipi", &K2_pipi_par5_);
  tree->SetBranchAddress("K2_par6pipi", &K2_pipi_par6_);




                                                  
   tree->SetBranchAddress("K1_kk_sigX_",					       &K1_kk_sigX_);                                                         
   tree->SetBranchAddress("K1_kk_sigY_",					       &K1_kk_sigY_);                                                         
   tree->SetBranchAddress("K1_kk_sigZ_",					       &K1_kk_sigZ_);                                                         
   tree->SetBranchAddress("K1_kpi_sigX_",				       &K1_kpi_sigX_);                                                        
   tree->SetBranchAddress("K1_kpi_sigY_",				       &K1_kpi_sigY_);                                                        
   tree->SetBranchAddress("K1_kpi_sigZ_",				       &K1_kpi_sigZ_);                                                        
   tree->SetBranchAddress("K1_pipi_sigX_",				       &K1_pipi_sigX_);                                                       
   tree->SetBranchAddress("K1_pipi_sigY_",				       &K1_pipi_sigY_);                                                       
   tree->SetBranchAddress("K1_pipi_sigZ_",				       &K1_pipi_sigZ_);                                                       
   tree->SetBranchAddress("K2_kk_sigX_",					       &K2_kk_sigX_);                                                         
   tree->SetBranchAddress("K2_kk_sigY_",					       &K2_kk_sigY_);                                                         
   tree->SetBranchAddress("K2_kk_sigZ_",					       &K2_kk_sigZ_);                                                         
   tree->SetBranchAddress("K2_kpi_sigX_",				       &K2_kpi_sigX_);                                                        
   tree->SetBranchAddress("K2_kpi_sigY_",				       &K2_kpi_sigY_);                                                        
   tree->SetBranchAddress("K2_kpi_sigZ_",				       &K2_kpi_sigZ_);                                                        
   tree->SetBranchAddress("K2_pipi_sigX_",				       &K2_pipi_sigX_);                                                       
   tree->SetBranchAddress("K2_pipi_sigY_",				       &K2_pipi_sigY_);                                                       
   tree->SetBranchAddress("K2_pipi_sigZ_",				       &K2_pipi_sigZ_);                                                       
   tree->SetBranchAddress("K1_kk_sigPX_",				       &K1_kk_sigPX_);                                                        
   tree->SetBranchAddress("K1_kk_sigPY_",				       &K1_kk_sigPY_);                                                        
   tree->SetBranchAddress("K1_kk_sigPZ_",				       &K1_kk_sigPZ_);                                                        
   tree->SetBranchAddress("K1_kpi_sigPX_",				       &K1_kpi_sigPX_);                                                       
   tree->SetBranchAddress("K1_kpi_sigPY_",				       &K1_kpi_sigPY_);                                                       
   tree->SetBranchAddress("K1_kpi_sigPZ_",				       &K1_kpi_sigPZ_);                                                       
   tree->SetBranchAddress("K1_pipi_sigPX_",				       &K1_pipi_sigPX_);                                                      
   tree->SetBranchAddress("K1_pipi_sigPY_",				       &K1_pipi_sigPY_);                                                      
   tree->SetBranchAddress("K1_pipi_sigPZ_",				       &K1_pipi_sigPZ_);                                                      
   tree->SetBranchAddress("K2_kk_sigPX_",				       &K2_kk_sigPX_);                                                        
   tree->SetBranchAddress("K2_kk_sigPY_",				       &K2_kk_sigPY_);                                                        
   tree->SetBranchAddress("K2_kk_sigPZ_",				       &K2_kk_sigPZ_);                                                        
   tree->SetBranchAddress("K2_kpi_sigPX_",				       &K2_kpi_sigPX_);                                                       
   tree->SetBranchAddress("K2_kpi_sigPY_",				       &K2_kpi_sigPY_);                                                       
   tree->SetBranchAddress("K2_kpi_sigPZ_",				       &K2_kpi_sigPZ_);                                                       
   tree->SetBranchAddress("K2_pipi_sigPX_",				       &K2_pipi_sigPX_);                                                      
   tree->SetBranchAddress("K2_pipi_sigPY_",				       &K2_pipi_sigPY_);                                                      
   tree->SetBranchAddress("K2_pipi_sigPZ_",				       &K2_pipi_sigPZ_);                                                      
   tree->SetBranchAddress("sigmaK1Pt",					       &K1Pt_error_);                                                         
   tree->SetBranchAddress("sigmaK2Pt",					       &K2Pt_error_);                                                         
   tree->SetBranchAddress("GenNumberOfBdecays",				       &GenNumberOfBdecays_);                                                 
   tree->SetBranchAddress("BmesonsId",					       BmesonsId_);                                                          
   tree->SetBranchAddress("BDauIdMC",					       BDauIdMC_);                                                           
   tree->SetBranchAddress("BDauDauIdMC",				       BDauDauIdMC_);                                                        
   tree->SetBranchAddress("GenNumberOfDaughters",			       GenNumberOfDaughters_);                                               
   tree->SetBranchAddress("GenNumberOfDaughtersDaughters",		       GenNumberOfDaughtersDaughters_);                                      
   tree->SetBranchAddress("BDauMMC",					       BDauMMC_);                                                            
   tree->SetBranchAddress("BDauPtMC",					       BDauPtMC_);                                                           
   tree->SetBranchAddress("BDauPzMC",					       BDauPzMC_);                                                           
   tree->SetBranchAddress("BDauEtaMC",					       BDauEtaMC_);                                                          
   tree->SetBranchAddress("BDauPhiMC",					       BDauPhiMC_);                                                          
   tree->SetBranchAddress("BDauDauMMC",					       BDauDauMMC_);                                                         
   tree->SetBranchAddress("BDauDauPtMC",				       BDauDauPtMC_);                                                        
   tree->SetBranchAddress("BDauDauPzMC",				       BDauDauPzMC_);                                                        
   tree->SetBranchAddress("BDauDauEtaMC",				       BDauDauEtaMC_);                                                       
   tree->SetBranchAddress("BDauDauPhiMC",				       BDauDauPhiMC_);                                                       
   tree->SetBranchAddress("BMMC",					       BMMC_);                                                               
   tree->SetBranchAddress("BPtMC",					       BPtMC_);                                                              
   tree->SetBranchAddress("BPzMC",					       BPzMC_);                                                              
   tree->SetBranchAddress("BEtaMC",					       BEtaMC_);                                                             
   tree->SetBranchAddress("BPhiMC",					       BPhiMC_);                                                             

   tree->SetBranchAddress("genBsVtx_z",                                       &genBsVtx_z_);
   tree->SetBranchAddress("genBsVtx_y",                                       &genBsVtx_y_);
   tree->SetBranchAddress("genBsVtx_x",                                       &genBsVtx_x_) ;                              
   tree->SetBranchAddress("genBsSVtx_z",  	                               &genBsSVtx_z_) ;                           
   tree->SetBranchAddress("genBsSVtx_y",                                      &genBsSVtx_y_);
   tree->SetBranchAddress("genBsSVtx_x",                                      &genBsSVtx_x_) ; 
   tree->SetBranchAddress("isGenJpsiEvent_",                                    &isGenJpsiEvent_);                                                     
  

}


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



}



