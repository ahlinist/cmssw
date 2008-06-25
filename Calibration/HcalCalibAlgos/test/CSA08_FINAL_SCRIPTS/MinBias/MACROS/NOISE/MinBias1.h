//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 23 16:14:31 2007 by        chain->Add("ROOT version 5.14/00e
// from TChain RecJet/
//////////////////////////////////////////////////////////

#ifndef MinBias1_h
#define MinBias1_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class MinBias1 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           mydet;
   Int_t           mysubd;
   Int_t           depth;
   Int_t           ieta;
   Int_t           iphi;
   Float_t         eta;
   Float_t         phi;
   Float_t         mom0;
   Float_t         mom1;
   Float_t         mom2;
   Float_t         mom3;
   Float_t         mom4;
   Float_t         mom0_cut;
   Float_t         mom1_cut;
   Float_t         mom2_cut;
   Float_t         mom3_cut;
   Float_t         mom4_cut;
   Float_t         occup;

   // List of branches
   TBranch        *b_mydet;   //!
   TBranch        *b_mysubd;   //!
   TBranch        *b_depth;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_mom0;   //!
   TBranch        *b_mom1;   //!
   TBranch        *b_mom2;   //!
   TBranch        *b_mom3;   //!
   TBranch        *b_mom4;   //!
   TBranch        *b_mom0_cut;   //!
   TBranch        *b_mom1_cut;   //!
   TBranch        *b_mom2_cut;   //!
   TBranch        *b_mom3_cut;   //!
   TBranch        *b_mom4_cut;   //!
   TBranch        *b_occup;   //!

   MinBias1(TTree *tree=0);
   virtual ~MinBias1();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MinBias1_cxx
MinBias1::MinBias1(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)g       chain->Add("ROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f) {
         f = new TFile("Memory Directory");
         f->cd("Rint:/");
      }
      tree = (TTree*)gDirectory->Get("RecJet");

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("RecJet","");
       chain->Add("ROOT/analisys_minbias_batch_0.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_10.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_11.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_12.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_13.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_14.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_15.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_16.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_17.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_18.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_19.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_1.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_20.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_21.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_22.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_23.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_24.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_25.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_26.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_27.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_28.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_29.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_2.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_30.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_31.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_32.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_33.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_34.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_35.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_36.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_37.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_38.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_39.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_3.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_40.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_41.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_42.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_43.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_44.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_45.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_46.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_47.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_48.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_49.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_4.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_50.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_51.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_52.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_53.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_54.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_55.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_56.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_57.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_58.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_59.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_5.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_60.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_61.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_62.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_63.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_64.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_65.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_66.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_67.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_68.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_69.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_6.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_70.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_71.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_72.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_73.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_74.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_75.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_76.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_77.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_78.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_79.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_7.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_80.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_81.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_82.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_83.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_84.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_85.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_86.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_87.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_88.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_89.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_8.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_90.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_91.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_92.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_93.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_94.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_95.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_96.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_97.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_98.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_99.root/RecJet");
       chain->Add("ROOT/analisys_minbias_batch_9.root/RecJet");       
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

MinBias1::~MinBias1()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MinBias1::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MinBias1::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MinBias1::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normaly not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("mydet", &mydet, &b_mydet);
   fChain->SetBranchAddress("mysubd", &mysubd, &b_mysubd);
   fChain->SetBranchAddress("depth", &depth, &b_depth);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("eta", &eta, &b_eta);
   fChain->SetBranchAddress("phi", &phi, &b_phi);
   fChain->SetBranchAddress("mom0", &mom0, &b_mom0);
   fChain->SetBranchAddress("mom1", &mom1, &b_mom1);
   fChain->SetBranchAddress("mom2", &mom2, &b_mom2);
   fChain->SetBranchAddress("mom3", &mom3, &b_mom3);
   fChain->SetBranchAddress("mom4", &mom4, &b_mom4);
   fChain->SetBranchAddress("mom0_cut", &mom0_cut, &b_mom0_cut);
   fChain->SetBranchAddress("mom1_cut", &mom1_cut, &b_mom1_cut);
   fChain->SetBranchAddress("mom2_cut", &mom2_cut, &b_mom2_cut);
   fChain->SetBranchAddress("mom3_cut", &mom3_cut, &b_mom3_cut);
   fChain->SetBranchAddress("mom4_cut", &mom4_cut, &b_mom4_cut);
   fChain->SetBranchAddress("occup", &occup, &b_occup);
   Notify();
}

Bool_t MinBias1::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MinBias1::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MinBias1::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MinBias1_cxx
