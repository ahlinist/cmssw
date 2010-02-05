// 
// Basic functionality is to multiply / divide (default) / overwrite 
// one field of a tree and store it.
// The value of that field (default: "xsec") may be looked up in a
// histogram (or an equivalent flat file) indexed by another field (default: "pthat")
// 
#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TLeaf.h"
#include "TString.h"
#include "TEventList.h"
#include "TH1F.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>


// a class name that contains "set" causesd ACLiC to fail, unless the typedefs are outside the class.
class TFillXsec {
 public:
  TFillXsec ();
  ~TFillXsec ();
  
  void SetFieldNames (TString& nameToChange, TString& nameForIndex);
  void SetTreeName (TString& name);
  /// 0 (and others) = replace, 1 = multiple, 2 = divide 
  void ChooseUpdateType (int updateType) { _updateType = updateType;}

  // for all functions: - negative returns are errors, otherwise a count
  //                    - tree may be a chain
  int LoadFlatFile (TString &filename);
  void LoadHistogram (TH1F& hist) {_pHist = &hist;}
  void SetFillValue (const double value);
  // passOnlyBin are TH1F indices (starting from 1), <= 0 means pass them all. (patch to handle a bug in MC production)
  int UpdateTree (TTree &tree, const TString &newFileName = "newTree.root", int passOnlyBin = 0); 

  // for the jetid project:
  void doFile (const TString &filename, int passOnlyBin = 0); 
  void fillFile (const TString &filename, const double value);

  // NYI: TFillXsec (const TFillXsec& that);
  // NYI: TFillXsec& operator = (const TFillXsec& that);

  friend std::ostream& operator<< (std::ostream& o, const TFillXsec& filx);


 private:
  int _updateType;
  TString _nameToChange, _nameForIndex, _treeName;
  TH1F* _pHist;
};

TFillXsec::TFillXsec ()
{
  _updateType = 0;
  _nameToChange = "xsec";
  _nameForIndex = "pthat";
  _treeName = "jetid/t";
  _pHist = 0;
}

TFillXsec::~TFillXsec ()
{
  if (0 != _pHist) delete _pHist;
}

void TFillXsec::SetFieldNames (TString& nameToChange,  TString& nameForIndex)
{
  _nameToChange = nameToChange;
  _nameForIndex = nameForIndex;
}

void TFillXsec::SetTreeName (TString& name)
{
  _treeName = name;
}

void TFillXsec::doFile (const TString& fileName, int passOnlyBin)
{
  TFile fin (fileName);
  TTree* pt = (TTree*) fin.Get(_treeName);
  if (pt == 0) {
    std::cerr<<"Can't find tree!"<<endl;
    return;
  }
  TString newName (fileName);
  if (newName.EndsWith (".root")) newName.Remove (newName.Length() - 5);
  newName += "w.root";
  UpdateTree (*pt, newName, passOnlyBin);
}

  
void TFillXsec::fillFile (const TString &filename, const double value)
{
  SetFillValue( value );
  doFile( filename );
}


int TFillXsec::LoadFlatFile (TString& fileName)
{
  ifstream in;
  in.open (fileName.Data());
  if (! in.is_open() ) {
    cerr<<"Failed to open a file with name: "<<fileName<<endl;
    return -1;
  }
  float x, y;
  std::vector<float> vx, vy;
  while (in >> x >> y) {
    vx.push_back (x);
    vy.push_back (y);
  }
  int N = vx.size() - 1;
  if (N < 1) {
    cerr<<"Need at least 2 lines (--> 1 bin) in input file!"<<endl;
    return -1;
  }
  float* vfx = new float [N+1];
  for (int i=0; i<=N; ++i) {
    vfx [i] = vx[i];
  }
  delete _pHist;
  _pHist = new TH1F ("hFillXsec", fileName+" values", N, vfx);
  for (int i=0; i < N; ++i) {
    _pHist->SetBinContent (1+i, vy[i]);
  }
  delete[] vfx;
  return 0;
}

void TFillXsec::SetFillValue (const double value)
{
  delete _pHist;
  _pHist = new TH1F ("hFillXsec", "", 1, 0, 1);
  _pHist->SetBinContent (1, value);
  return;
}

int TFillXsec::UpdateTree (TTree &tree, const TString &newFileName, int passOnlyBin)
{
  if (_pHist == 0) {std::cerr<<"Need to load lookup hist/flat file first!"<<std::endl; return -3;}

  bool index_needed = _pHist->GetNbinsX() > 1;

  TFile *fn = new TFile( newFileName, "recreate" );
  TTree *newTree = tree.GetTree()->CloneTree(0);
  if (newTree == 0) {std::cerr<<"Can't clone tree."<<std::endl; return -1;}

  TLeaf* plc = 0, *pli = 0; // avoids warning messages
  Int_t prevTree = -1;
  TEventList* list = tree.GetEventList();
  const Long64_t N = list ? Long64_t (list->GetN()) : Long64_t (tree.GetEntriesFast());
   
  for (Int_t i=0; i<N; ++i) {
    Long64_t ientry = list ? list->GetEntry (i) : Long64_t (i);
    if (ientry < 0) {std::cerr<<"bug! ientry: "<<ientry<<std::endl; return -2;}

    Int_t itmp = tree.LoadTree (ientry); // switch to the right tree in the chain
    if (itmp < 0) {std::cerr<<"bug? what's going on with itmp: "<<itmp<<std::endl; return -3;}

    // leaves need to be found after loading the tree (happens several times in a chain)
    if (tree.GetTreeNumber () != prevTree) {
      prevTree = tree.GetTreeNumber ();
      plc = tree.GetLeaf (_nameToChange);
      if (plc == 0) {std::cerr<<"Can't find key ("<<_nameToChange<<") in tree."<<std::endl; return -1;}
      if (index_needed) {
	pli = tree.GetLeaf (_nameForIndex);
	if (pli == 0) {std::cerr<<"Can't find key ("<<_nameForIndex<<") in tree."<<std::endl; return -1;}
      }
    }

    tree.GetEntry(ientry); 

    Float_t* pVal = (Float_t *) plc -> GetValuePointer();;
    Int_t index;
    if( index_needed ) {
      Float_t fIndex  = (Float_t) pli -> GetValue();
      index = _pHist -> FindBin (fIndex);
      if (passOnlyBin > 0 && index != passOnlyBin) continue;
    } else {
      index = 1;
    }
    Float_t updateWith = _pHist -> GetBinContent (index);
    //    std::cout<<"DBG i: "<<i<<", val: "<<*pVal<<", index: "<<index<<", bin #"<<_pHist->FindBin(index)
    //     <<", with: "<<updateWith<<std::endl;
    if (_updateType == 1) {
      (*pVal) *= updateWith;
    } else if (_updateType == 2) {
      if (updateWith != 0) (*pVal) /= updateWith;
    } else {
      (*pVal) = updateWith;
    }
    newTree->Fill();
  }
  newTree->AutoSave( "FlushBaskets" );
  fn = newTree->GetCurrentFile(); // TTree can change the file, and automatically deallocates the old one!
  fn->Write();
  fn->Close();
  return newTree->GetEntriesFast();
}

std::ostream& operator<< (std::ostream& o, const TFillXsec& filx)
{
  o <<"For TreeName: "<<filx._treeName<<", UpdateType: "<<filx._updateType<<" of "<<filx._nameToChange
    <<" according to "<<filx._nameForIndex<<" using values:\n";
  TH1F* ph = filx._pHist;
  if (ph) {
    for (int ib=1; ib <= ph->GetNbinsX(); ++ib) {
      o <<"bin: "<<ib<<" @ ("<<ph->GetBinLowEdge(ib)<<" to "
	<<ph->GetBinLowEdge(1+ib)<<") = "<<ph->GetBinContent(ib)<<"\n";
    }
    o << std::flush;
  } else {
    o << "No values read in!"<<std::endl;
  }
  return o;
}
