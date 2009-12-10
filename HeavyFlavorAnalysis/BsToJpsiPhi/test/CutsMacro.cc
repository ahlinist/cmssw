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
			   int &JpsiEta, int & JpsiK10, int & JpsiK0, int & JpsiKp);
void identifyEventType(bool & isGenBsJpsiPhiMuMuKKEvent, bool &isGenBsJpsiKKEvent, bool &isGenBdJpsiKstarMuMuKpiEvent, bool &isGenBsEvent_, bool &isGenBdEvent_,
		       bool & JpsiEta, bool & JpsiK10, bool & JpsiK0, bool & JpsiKp);
void setBranchAddresses(TTree *tree);

// define cuts:
const double minProbability = 0.02;
const double KaonPtCut = 0.9;
const double nominalPhiMass = 1.0195 ;
const double phiMassWindow = 0.007;
const double decayLengthCut = 3;
const double pointingCut = 0.8;

// maximum number of event to loop over. -1 means all events.
const int maxNumEvents = -1;
int a = 0;
// ***************main function*****************************
void CutsMacro() {
  
  TFile *f = new TFile("BtoJpsiMuMu_Nov30.root");
  //  TFile *f = new TFile("JpsiMuMu_Nov30.root");

  // get tree from file
  TTree *tree = (TTree*)f->Get("BsTree");
  
  // call function to set branch addresses
  setBranchAddresses(tree);

  int nentries = tree->GetEntries();
  std::cout<<"Number of entries in tree = " << nentries << std::endl;

  int maxEvents;
  if(maxNumEvents < 0) maxEvents = nentries;
  else maxEvents = maxNumEvents;

  for(int i=0; i < maxEvents; i++){
    entrycounter++;
    if( (entrycounter % 10000) == 0) std::cout<<"processing entry number " << entrycounter << std::endl;
    tree->GetEntry(i);

    identifyEventType(isGenBsJpsiPhiMuMuKKEvent_, isGenBsJpsiKKEvent_, isGenBdJpsiKstarMuMuKpiEvent_, isGenBsEvent_, isGenBdEvent_, 
		      isGenBsJpsiEtaEvent_, isGenBdJpsiK10Event_, isGenBdJpsiK0Event_, isGenBpJpsiKpEvent_);

    if(triggerbit_HLTdoubleMu3_ != 1) continue;      // IS TRIGGERED_HLTdoubleMu3**************
    HLT++;
    
    if(iPassedCutIdent_ < 13) continue;
    increaseCountersByOne(sig1, jpsikk1, jpsikst1, otherbs1, otherbd1, other1, jpsieta1, jpsik101, jpsik01, jpsik1p1);

    // B vtx P cut    

    if(BsVtxProb_ >= 0.01)
      increaseCountersByOne(sig2, jpsikk2, jpsikst2, otherbs2, otherbd2, other2, jpsieta2, jpsik102, jpsik02, jpsik1p2);
    if(BsVtxProb_ >= 0.02)
      increaseCountersByOne(sig3, jpsikk3, jpsikst3, otherbs3, otherbd3, other3, jpsieta3, jpsik103, jpsik03, jpsik1p3);
    if(BsVtxProb_ >= 0.03)
      increaseCountersByOne(sig4, jpsikk4, jpsikst4, otherbs4, otherbd4, other4, jpsieta4, jpsik104, jpsik04, jpsik1p4);
    
    // Kaons Pt cut

    if(K1Pt_after_ >= 0.7 && K2Pt_after_ >= 0.7)
      increaseCountersByOne(sig5, jpsikk5, jpsikst5, otherbs5, otherbd5, other5, jpsieta5, jpsik105, jpsik05, jpsik1p5);
    if(K1Pt_after_ >= 0.9 && K2Pt_after_ >= 0.9)
      increaseCountersByOne(sig6, jpsikk6, jpsikst6, otherbs6, otherbd6, other6, jpsieta6, jpsik106, jpsik06, jpsik1p6);
    if(K1Pt_after_ >= 1 && K2Pt_after_ >= 1)
      increaseCountersByOne(sig7, jpsikk7, jpsikst7, otherbs7, otherbd7, other7, jpsieta7, jpsik107, jpsik07, jpsik1p7);
    
    // Phi Mass window cut

    if( fabs(PhiMass_after_ - nominalPhiMass) <= 0.005)
      increaseCountersByOne(sig8, jpsikk8, jpsikst8, otherbs8, otherbd8, other8, jpsieta8, jpsik108, jpsik08, jpsik1p8);
    if( fabs(PhiMass_after_ - nominalPhiMass) <= 0.007)
      increaseCountersByOne(sig9, jpsikk9, jpsikst9, otherbs9, otherbd9, other9, jpsieta9, jpsik109, jpsik09, jpsik1p9);
    if( fabs(PhiMass_after_ - nominalPhiMass) <= 0.010)
      increaseCountersByOne(sig10, jpsikk10, jpsikst10, otherbs10, otherbd10, other10, jpsieta10, jpsik1010, jpsik010, jpsik1p10);
    
    // Significance cut

    if(Dist_/ dDist_ >= 1)
      increaseCountersByOne(sig11, jpsikk11, jpsikst11, otherbs11, otherbd11, other11, jpsieta11, jpsik1011, jpsik011, jpsik1p11);
    if(Dist_/ dDist_ >= 2)
      increaseCountersByOne(sig12, jpsikk12, jpsikst12, otherbs12, otherbd12, other12, jpsieta12, jpsik1012, jpsik012, jpsik1p12);
    if(Dist_/ dDist_ >= 3)
      increaseCountersByOne(sig13, jpsikk13, jpsikst13, otherbs13, otherbd13, other13, jpsieta13, jpsik1013, jpsik013, jpsik1p13);

    // cos(alpha) cut
    
    if(AngleBsDecayLength_ >= 0.8)
      increaseCountersByOne(sig14, jpsikk14, jpsikst14, otherbs14, otherbd14, other14, jpsieta14, jpsik1014, jpsik014, jpsik1p14);
    if(AngleBsDecayLength_ >= 0.9)
      increaseCountersByOne(sig15, jpsikk15, jpsikst15, otherbs15, otherbd15, other15, jpsieta15, jpsik1015, jpsik015, jpsik1p15);
    if(AngleBsDecayLength_ >= 0.95)
      increaseCountersByOne(sig16, jpsikk16, jpsikst16, otherbs16, otherbd16, other16, jpsieta16, jpsik1016, jpsik016, jpsik1p16);
    
  }
  //****************************
  // finished event loop *******
  //****************************
  
  // print out results
  std::cout<< "There are " << entrycounter << " events in the tree." << std::endl;
  std::cout<< "Events with only HLT: " << HLT << std::endl << std::endl;

  std::cout<< "                        | Bs->JpsiPhi | Bd->Jpsi K* | Bs->Jpsi KK | Bs->Jpsi Eta| Bd->Jpsi K10| Bd->Jpsi K0 | Bp->Jpsi Kp | Other Bs    | Other Bd    | Other "<<  std::endl;
  std::cout<< "Events after the fit    | " << setw(11) << sig1 << " | " << setw(11) << jpsikst1 << " | " << setw(11) << jpsikk1 << " | "
	   << setw(11) << jpsieta1 << " | " << setw(11) << jpsik101 << " | " << setw(11) << jpsik01 << " | " << setw(11) << jpsik1p1 << " | "
	   << setw(11) << otherbs1 << " | " << setw(11) << otherbd1 << " | " << setw(11) << other1 << std::endl;
  std::cout<< "P(KK)>1%                | " << setw(11) << sig2 << " | " << setw(11) << jpsikst2 << " | " << setw(11) << jpsikk2 << " | "
	   << setw(11) << jpsieta2 << " | " << setw(11) << jpsik102 << " | " << setw(11) << jpsik02 << " | " << setw(11) << jpsik1p2 << " | "
	   << setw(11) << otherbs2 << " | " << setw(11) << otherbd2 << " | " << setw(11) << other2 << std::endl;
  std::cout<< "P(KK)>2%                | " << setw(11) << sig2 << " | " << setw(11) << jpsikst2 << " | " << setw(11) << jpsikk2 << " | "
	   << setw(11) << jpsieta3 << " | " << setw(11) << jpsik103 << " | " << setw(11) << jpsik03 << " | " << setw(11) << jpsik1p3 << " | "
	   << setw(11) << otherbs3 << " | " << setw(11) << otherbd3 << " | " << setw(11) << other3 << std::endl;
  std::cout<< "P(KK)>3%                | " << setw(11) << sig4 << " | " << setw(11) << jpsikst4 << " | " << setw(11) << jpsikk4 << " | "
	   << setw(11) << jpsieta4 << " | " << setw(11) << jpsik104 << " | " << setw(11) << jpsik04 << " | " << setw(11) << jpsik1p4 << " | "
	   << setw(11) << otherbs4 << " | " << setw(11) << otherbd4 << " | " << setw(11) << other4 << std::endl;
  std::cout<< "Kaons pt>0.7 GeV/c      | " << setw(11) << sig5 << " | " << setw(11) << jpsikst5 << " | " << setw(11) << jpsikk5 << " | "
	   << setw(11) << jpsieta5 << " | " << setw(11) << jpsik105 << " | " << setw(11) << jpsik05 << " | " << setw(11) << jpsik1p5 << " | "
	   << setw(11) << otherbs5 << " | " << setw(11) << otherbd5 << " | " << setw(11) << other5 << std::endl;
  std::cout<< "Kaons pt>0.9 GeV/c      | " << setw(11) << sig6 << " | " << setw(11) << jpsikst6 << " | " << setw(11) << jpsikk6 << " | "
	   << setw(11) << jpsieta6 << " | " << setw(11) << jpsik106 << " | " << setw(11) << jpsik06 << " | " << setw(11) << jpsik1p6 << " | "
	   << setw(11) << otherbs6 << " | " << setw(11) << otherbd6 << " | " << setw(11) << other6 << std::endl;
  std::cout<< "Kaons pt>1 GeV/c        | " << setw(11) << sig7 << " | " << setw(11) << jpsikst7 << " | " << setw(11) << jpsikk7 << " | "
	   << setw(11) << jpsieta7 << " | " << setw(11) << jpsik107 << " | " << setw(11) << jpsik07 << " | " << setw(11) << jpsik1p7 << " | "
	   << setw(11) << otherbs7 << " | " << setw(11) << otherbd7 << " | " << setw(11) << other7 << std::endl;
  std::cout<< "DeltaPhiM < 5 MeV/c^2   | " << setw(11) << sig8 << " | " << setw(11) << jpsikst8 << " | " << setw(11) << jpsikk8 << " | "
	   << setw(11) << jpsieta8 << " | " << setw(11) << jpsik108 << " | " << setw(11) << jpsik08 << " | " << setw(11) << jpsik1p8 << " | "
	   << setw(11) << otherbs8 << " | " << setw(11) << otherbd8 << " | " << setw(11) << other8 << std::endl;
  std::cout<< "DeltaPhiM < 7 MeV/c^2   | " << setw(11) << sig9 << " | " << setw(11) << jpsikst9 << " | " << setw(11) << jpsikk9 << " | "
	   << setw(11) << jpsieta9 << " | " << setw(11) << jpsik109 << " | " << setw(11) << jpsik09 << " | " << setw(11) << jpsik1p9 << " | "
	   << setw(11) << otherbs9 << " | " << setw(11) << otherbd9 << " | " << setw(11) << other9 << std::endl;
  std::cout<< "DeltaPhiM < 10 MeV/c^2  | " << setw(11) << sig10 << " | " << setw(11) << jpsikst10 << " | " << setw(11) << jpsikk10 << " | "
	   << setw(11) << jpsieta10 << " | " << setw(11) << jpsik1010 << " | " << setw(11) << jpsik010 << " | " << setw(11) << jpsik1p10 << " | "
	   << setw(11) << otherbs10 << " | " << setw(11) << otherbd10 << " | " << setw(11) << other10 << std::endl;
   std::cout<< "ct/sigma_ct > 1         | " << setw(11) << sig11 << " | " << setw(11) << jpsikst11 << " | " << setw(11) << jpsikk11 << " | "
	   << setw(11) << jpsieta11 << " | " << setw(11) << jpsik1011 << " | " << setw(11) << jpsik011 << " | " << setw(11) << jpsik1p11 << " | "
	   << setw(11) << otherbs11 << " | " << setw(11) << otherbd11 << " | " << setw(11) << other11 << std::endl;
   std::cout<< "ct/sigma_ct > 2         | " << setw(11) << sig12 << " | " << setw(11) << jpsikst12 << " | " << setw(11) << jpsikk12 << " | "
	   << setw(11) << jpsieta12 << " | " << setw(11) << jpsik1012 << " | " << setw(11) << jpsik012 << " | " << setw(11) << jpsik1p12 << " | "
	   << setw(11) << otherbs12 << " | " << setw(11) << otherbd12 << " | " << setw(11) << other12 << std::endl;
   std::cout<< "ct/sigma_ct > 3         | " << setw(11) << sig13 << " | " << setw(11) << jpsikst13 << " | " << setw(11) << jpsikk13 << " | "
	   << setw(11) << jpsieta13 << " | " << setw(11) << jpsik1013 << " | " << setw(11) << jpsik013 << " | " << setw(11) << jpsik1p13 << " | "
	   << setw(11) << otherbs13 << " | " << setw(11) << otherbd13 << " | " << setw(11) << other13 << std::endl;
   std::cout<< "cos(alpha)>0.8          | " << setw(11) << sig14 << " | " << setw(11) << jpsikst14 << " | " << setw(11) << jpsikk14 << " | "
	   << setw(11) << jpsieta14 << " | " << setw(11) << jpsik1014 << " | " << setw(11) << jpsik014 << " | " << setw(11) << jpsik1p14 << " | "
	   << setw(11) << otherbs14 << " | " << setw(11) << otherbd14 << " | " << setw(11) << other14 << std::endl;
   std::cout<< "cos(alpha)>0.9          | " << setw(11) << sig15 << " | " << setw(11) << jpsikst15 << " | " << setw(11) << jpsikk15 << " | "
	   << setw(11) << jpsieta15 << " | " << setw(11) << jpsik1015 << " | " << setw(11) << jpsik015 << " | " << setw(11) << jpsik1p15 << " | "
	   << setw(11) << otherbs15 << " | " << setw(11) << otherbd15 << " | " << setw(11) << other15 << std::endl;
   std::cout<< "cos(alpha)>0.95         | " << setw(11) << sig16 << " | " << setw(11) << jpsikst16 << " | " << setw(11) << jpsikk16 << " | "
	   << setw(11) << jpsieta16 << " | " << setw(11) << jpsik1016 << " | " << setw(11) << jpsik016 << " | " << setw(11) << jpsik1p16 << " | "
	   << setw(11) << otherbs16 << " | " << setw(11) << otherbd16 << " | " << setw(11) << other16 << std::endl;  


}   // finished main function



// define counting function
void increaseCountersByOne(int &BsJpsiPhiSignal, int& BsJpsiKKSignal, int& BdJpsiKstar, int& BsOther, int& BdOther,  int& other  , 
			   int& JpsiEta, int& JpsiK10, int& JpsiK0, int &JpsiKp){

  if(isGenBsJpsiPhiMuMuKKEvent_ == 1)               BsJpsiPhiSignal++;
  else if (isGenBsJpsiKKEvent_ == 1)                BsJpsiKKSignal++;
  else if (isGenBdJpsiKstarMuMuKpiEvent_ == 1) BdJpsiKstar++;
  else if (isGenBsJpsiEtaEvent_ == 1) JpsiEta++;
  else if (isGenBdJpsiK10Event_ == 1) JpsiK10++;
  else if (isGenBdJpsiK0Event_ == 1) JpsiK0++;
  else if (isGenBpJpsiKpEvent_ == 1) JpsiKp++;
  else if (isGenBsEvent_ == 1)            BsOther++;
  else if (isGenBdEvent_ == 1)       BdOther++;		 
  else                          other++; 

}  // end definition of counting function


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

