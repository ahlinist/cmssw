//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Nov 15 17:58:16 2006 by ROOT version 5.12/00
// from TTree pippo/Analysis tree
// found on file: 31151_new.root
//////////////////////////////////////////////////////////

#ifndef invmasscomb_h
#define invmasscomb_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class invmasscomb {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Int_t           event;
   Int_t           waspedin;
   Int_t           waspedout;
   Int_t           wasbeam;
   Float_t         scint1;
   Float_t         scint2;
   Float_t         scint3;
   Float_t         scint4;
   Int_t           nx_wcA;
   Int_t           ny_wcA;
   Int_t           nx_wcB;
   Int_t           ny_wcB;
   Int_t           nx_wcC;
   Int_t           ny_wcC;
   Int_t           nx_wcD;
   Int_t           ny_wcD;
   Float_t         x_wcA[3];   //[nx_wcA]
   Float_t         y_wcA[3];   //[ny_wcA]
   Float_t         x_wcB[3];   //[nx_wcB]
   Float_t         y_wcB[2];   //[ny_wcB]
   Float_t         x_wcC[2];   //[nx_wcC]
   Float_t         y_wcC[1];   //[ny_wcC]
   Float_t         toftim1s;
   Float_t         toftim1j;
   Float_t         toftim2s;
   Float_t         toftim2j;
   Float_t         tofadc1s;
   Float_t         tofadc1j;
   Float_t         tofadc2s;
   Float_t         tofadc2j;
   Int_t           nCry;
   Float_t         amplCry[1700];   //[nCry]
   Float_t         etaCry[1700];   //[nCry]
   Float_t         phiCry[1700];   //[nCry]
   Float_t         etamaxE;
   Float_t         phimaxE;
   Float_t         E25;
   Float_t         E9;
   Float_t         E1;
   Int_t           nClu;
   Float_t         amplClu[50];   //[nClu]
   Float_t         etaClu[50];   //[nClu]
   Float_t         phiClu[50];   //[nClu]
   Float_t         xClu[50];   //[nClu]
   Float_t         yClu[50];   //[nClu]
   Float_t         zClu[50];   //[nClu]
   Int_t          nCryClu[50];   //[nClu]
   Float_t         etamaxEClu;
   Float_t         phimaxEClu;

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_waspedin;   //!
   TBranch        *b_waspedout;   //!
   TBranch        *b_wasbeam;   //!
   TBranch        *b_scint1;   //!
   TBranch        *b_scint2;   //!
   TBranch        *b_scint3;   //!
   TBranch        *b_scint4;   //!
   TBranch        *b_nx_wcA;   //!
   TBranch        *b_ny_wcA;   //!
   TBranch        *b_nx_wcB;   //!
   TBranch        *b_ny_wcB;   //!
   TBranch        *b_nx_wcC;   //!
   TBranch        *b_ny_wcC;   //!
   TBranch        *b_nx_wcD;   //!
   TBranch        *b_ny_wcD;   //!
   TBranch        *b_x_wcA;   //!
   TBranch        *b_y_wcA;   //!
   TBranch        *b_x_wcB;   //!
   TBranch        *b_y_wcB;   //!
   TBranch        *b_x_wcC;   //!
   TBranch        *b_y_wcC;   //!
   TBranch        *b_toftim1s;   //!
   TBranch        *b_toftim1j;   //!
   TBranch        *b_toftim2s;   //!
   TBranch        *b_toftim2j;   //!
   TBranch        *b_tofadc1s;   //!
   TBranch        *b_tofadc1j;   //!
   TBranch        *b_tofadc2s;   //!
   TBranch        *b_tofadc2j;   //!
   TBranch        *b_nCry;   //!
   TBranch        *b_amplCry;   //!
   TBranch        *b_etaCry;   //!
   TBranch        *b_phiCry;   //!
   TBranch        *b_etamaxE;   //!
   TBranch        *b_phimaxE;   //!
   TBranch        *b_E25;   //!
   TBranch        *b_E9;   //!
   TBranch        *b_E1;   //!
   TBranch        *b_nClu;   //!
   TBranch        *b_amplClu;   //!
   TBranch        *b_etaClu;   //!
   TBranch        *b_phiClu;   //!
   TBranch        *b_xClu;   //!
   TBranch        *b_yClu;   //!
   TBranch        *b_zClu;   //!
   TBranch        *b_nCryClu;   //!
   TBranch        *b_etamaxEClu;   //!
   TBranch        *b_phimaxEClu;   //!

   invmasscomb(TTree *tree=0);
   virtual ~invmasscomb();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef invmasscomb_cxx
invmasscomb::invmasscomb(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("31151_new.root");
      if (!f) {
         f = new TFile("31151_new.root");
      }
      tree = (TTree*)gDirectory->Get("pippo");

   }
   Init(tree);
}

invmasscomb::~invmasscomb()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t invmasscomb::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t invmasscomb::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void invmasscomb::Init(TTree *tree)
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

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("waspedin", &waspedin, &b_waspedin);
   fChain->SetBranchAddress("waspedout", &waspedout, &b_waspedout);
   fChain->SetBranchAddress("wasbeam", &wasbeam, &b_wasbeam);
   fChain->SetBranchAddress("scint1", &scint1, &b_scint1);
   fChain->SetBranchAddress("scint2", &scint2, &b_scint2);
   fChain->SetBranchAddress("scint3", &scint3, &b_scint3);
   fChain->SetBranchAddress("scint4", &scint4, &b_scint4);
   fChain->SetBranchAddress("nx_wcA", &nx_wcA, &b_nx_wcA);
   fChain->SetBranchAddress("ny_wcA", &ny_wcA, &b_ny_wcA);
   fChain->SetBranchAddress("nx_wcB", &nx_wcB, &b_nx_wcB);
   fChain->SetBranchAddress("ny_wcB", &ny_wcB, &b_ny_wcB);
   fChain->SetBranchAddress("nx_wcC", &nx_wcC, &b_nx_wcC);
   fChain->SetBranchAddress("ny_wcC", &ny_wcC, &b_ny_wcC);
   fChain->SetBranchAddress("nx_wcD", &nx_wcD, &b_nx_wcD);
   fChain->SetBranchAddress("ny_wcD", &ny_wcD, &b_ny_wcD);
   fChain->SetBranchAddress("x_wcA", x_wcA, &b_x_wcA);
   fChain->SetBranchAddress("y_wcA", y_wcA, &b_y_wcA);
   fChain->SetBranchAddress("x_wcB", x_wcB, &b_x_wcB);
   fChain->SetBranchAddress("y_wcB", y_wcB, &b_y_wcB);
   fChain->SetBranchAddress("x_wcC", x_wcC, &b_x_wcC);
   fChain->SetBranchAddress("y_wcC", y_wcC, &b_y_wcC);
   fChain->SetBranchAddress("toftim1s", &toftim1s, &b_toftim1s);
   fChain->SetBranchAddress("toftim1j", &toftim1j, &b_toftim1j);
   fChain->SetBranchAddress("toftim2s", &toftim2s, &b_toftim2s);
   fChain->SetBranchAddress("toftim2j", &toftim2j, &b_toftim2j);
   fChain->SetBranchAddress("tofadc1s", &tofadc1s, &b_tofadc1s);
   fChain->SetBranchAddress("tofadc1j", &tofadc1j, &b_tofadc1j);
   fChain->SetBranchAddress("tofadc2s", &tofadc2s, &b_tofadc2s);
   fChain->SetBranchAddress("tofadc2j", &tofadc2j, &b_tofadc2j);
   fChain->SetBranchAddress("nCry", &nCry, &b_nCry);
   fChain->SetBranchAddress("amplCry", amplCry, &b_amplCry);
   fChain->SetBranchAddress("etaCry", etaCry, &b_etaCry);
   fChain->SetBranchAddress("phiCry", phiCry, &b_phiCry);
   fChain->SetBranchAddress("etamaxE", &etamaxE, &b_etamaxE);
   fChain->SetBranchAddress("phimaxE", &phimaxE, &b_phimaxE);
   fChain->SetBranchAddress("E25", &E25, &b_E25);
   fChain->SetBranchAddress("E9", &E9, &b_E9);
   fChain->SetBranchAddress("E1", &E1, &b_E1);
   fChain->SetBranchAddress("nClu", &nClu, &b_nClu);
   fChain->SetBranchAddress("amplClu", amplClu, &b_amplClu);
   fChain->SetBranchAddress("etaClu", etaClu, &b_etaClu);
   fChain->SetBranchAddress("phiClu", phiClu, &b_phiClu);
   fChain->SetBranchAddress("xClu", xClu, &b_xClu);
   fChain->SetBranchAddress("yClu", yClu, &b_yClu);
   fChain->SetBranchAddress("zClu", zClu, &b_zClu);
   fChain->SetBranchAddress("nCryClu", nCryClu, &b_nCryClu);
   fChain->SetBranchAddress("etamaxEClu", &etamaxEClu, &b_etamaxEClu);
   fChain->SetBranchAddress("phimaxEClu", &phimaxEClu, &b_phimaxEClu);
   Notify();
}

Bool_t invmasscomb::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void invmasscomb::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t invmasscomb::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef invmasscomb_cxx
