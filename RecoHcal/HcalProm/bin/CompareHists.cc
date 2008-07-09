#include <string>
#include <vector>
#include <iostream>

#include "TFile.h"
#include "TDirectory.h"
#include "TCollection.h"
#include "TKey.h"
#include "TStyle.h"
#include "TH1.h"
#include "TH2.h"
#include "TObjString.h"
#include "TPave.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TText.h"

class FullHistName {
public:
  FullHistName (const std::string& fFullName);
  std::string name () const;
  std::string path () const;
  std::string str () const;
  const char* c_str () const;
  bool match (const FullHistName& fPattern) const;
private:
  std::string mName;
};

typedef std::vector<FullHistName> FullHistNames;

class RootFileDigger {
public:
  RootFileDigger (TFile* fFile);
  FullHistNames allHists ();
  FullHistNames all1DHists ();
  FullHistNames all2DHists ();
  FullHistNames allStrings ();
  static FullHistNames match (const FullHistName& fPattern, const FullHistNames& fNames);
  TObject* getObject (const FullHistName& fName);
  TH1* getTH1 (const FullHistName& fName);
  TH2* getTH2 (const FullHistName& fName);
private:
  void addDirs (FullHistNames* fDirs, const FullHistName& fDir);
  FullHistNames getObjects (const FullHistName& fDir, const std::string& fClassName);

  TFile* mFile;
  FullHistNames mDirs;
};

class HistComparator {
public:
  HistComparator (const std::string& fTitle) : mTitle (fTitle) {}
  double dumpComparison (TH1* fHist, TH1* fRefHist, const std::string& fPictureFile);
  double dumpComparison (TH2* fHist, TH2* fRefHist, const std::string& fPictureFile);
private:
  std::string mTitle;
};

//================= FullHistName =====================

FullHistName::FullHistName (const std::string& fFullName) 
  : mName (fFullName)
{}

std::string FullHistName::name () const {
  size_t ipos = mName.find_last_of ('/');
  if (ipos != std::string::npos) {
    return std::string (mName, ++ipos);
  }
  return mName;
}

std::string FullHistName::path () const {
  size_t ipos = mName.find_last_of ('/');
  if (ipos != std::string::npos) {
    return std::string (mName, 0, ipos);
  }
  return "";
}

std::string FullHistName::str () const {return mName;}

const char* FullHistName::c_str () const {return mName.c_str();}

bool FullHistName::match (const FullHistName& fPattern) const {
  if (mName.find (fPattern.str()) != 0) return false; 
  if (mName == fPattern.str()) return true;
  if (path () == fPattern.str()) return true;
  if (path () == fPattern.path ()) return true;
  return false;
}

//================= RootFileDigger =====================
  RootFileDigger::RootFileDigger (TFile* fFile) 
    : mFile (fFile) 
{
  addDirs (&mDirs, FullHistName (""));
}

FullHistNames RootFileDigger::getObjects (const FullHistName& fDir, const std::string& fClassName) {
  FullHistNames result;
  TDirectory* currentDirectory = gDirectory;
  std::string dirName = fDir.str();
  mFile->cd (dirName.empty () ? 0 : dirName.c_str());
    TIter next (gDirectory->GetListOfKeys ());
  for (TKey* key = 0; (key = (TKey*) next());) {
    if (std::string (fClassName) == key->GetClassName ()) {
      std::string newName = dirName + std::string ("/") + std::string (key->GetName());
      result.push_back (FullHistName (newName));
    } 
  }
  currentDirectory->cd();
  return result;
}

void RootFileDigger::addDirs (FullHistNames* fDirs, const FullHistName& fDir) {
  FullHistNames newDirs = getObjects (fDir, "TDirectoryFile");
  for (size_t i = 0; i < newDirs.size(); ++i) {
    fDirs->push_back (newDirs[i]);
    addDirs (fDirs, newDirs[i]);
  }
}


 FullHistNames RootFileDigger::all1DHists () {
   FullHistNames result;
   for (size_t i = 0; i < mDirs.size(); ++i) {
     FullHistNames newHists = getObjects (mDirs[i], "TH1F");
     result.insert (result.end(), newHists.begin(), newHists.end());
   }
   return result;
 }

 TObject* RootFileDigger::getObject (const FullHistName& fName) {
   TObject* obj = 0;
   mFile->GetObject (fName.c_str(), obj);
   return obj;
 }

 TH1* RootFileDigger::getTH1 (const FullHistName& fName) {
   return dynamic_cast<TH1*> (getObject (fName));
 }

FullHistNames RootFileDigger::match (const FullHistName& fPattern, const FullHistNames& fNames) {
  FullHistNames result;
  for (size_t i = 0; i < fNames.size(); ++i) if (fNames[i].match (fPattern)) result.push_back (fNames[i]);
  return result;
}


//================= HistComparator =====================
double HistComparator::dumpComparison (TH1* fHist, 
				       TH1* fRefHist, 
				       const std::string& fPictureFile)
{
  std::cout << "HistComparator::dumpComparison-> " << fHist->GetName() << '/' << fRefHist->GetName() << '/' << fPictureFile << std::endl;
  gStyle->SetOptStat (kFALSE);
  TCanvas canvas ("HistComparator","HistComparator",800,600);
  double pv = 0;
  if (fHist->GetSumOfWeights () > 0) {
    pv = fHist->KolmogorovTest (fRefHist, "OU");
    // set style
    TPad pad ("pad", "pad", 0, 0, 1, 0.97, 0);
    pad.Draw();
    
    char buf [1024];
    TPaveText title (0.,0.97,1,1);
    title.SetFillColor(pv > 0.01 ? 3 : 2);
    sprintf (buf, "%s PV = %6.4f", mTitle.c_str(), pv);
    title.AddText (buf);
    title.Draw();
    
    pad.cd();
    
    fHist->Sumw2 ();
    if (fHist->GetSumOfWeights () > 0 && fRefHist->GetSumOfWeights () > 0) {
      fHist->Scale (fRefHist->GetSumOfWeights () / fHist->GetSumOfWeights ());
    }
    
    fHist->SetMarkerStyle (21);
    fHist->SetMarkerSize (0.7);
    fRefHist->SetLineColor (2);
    fRefHist->SetFillColor (42);
    
    fRefHist->Draw ();
    fHist->Draw ("e1p,same");
    canvas.Print (fPictureFile.c_str());
  }
  else {
    TPaveText title (0.,0.,1,1);
    title.AddText ("EMPTY");
    title.Draw();
    canvas.Print (fPictureFile.c_str());
  }
  return pv;}

//================= main =====================
int main (int argn, char* argv []) {
  if (argn < 5) {
    std::cout << "Usage: " << argv[0] << " <file_name> <reference file_name> <pattern> <description" << std::endl;
    return 1;
  }
  
  std::string inputFileName (argv[1]);
  std::string refFileName (argv[2]);
  std::string pattern (argv[3]);
  std::string globalTitle = argv[4];
  std::cout << "Processing file " << inputFileName << std::endl;
  TFile* inputFile = TFile::Open (inputFileName.c_str());
  if (!inputFile) {
    std::cerr << "Cannot open file " << inputFileName << std::endl;
    return 1;
  }
  std::cout << "Reference file " << refFileName << std::endl;
  TFile* refFile = TFile::Open (refFileName.c_str());
  if (!refFile) {
    std::cerr << "Cannot open reference file " << refFileName << std::endl;
    return 1;
  }
  RootFileDigger histFile (inputFile);
  RootFileDigger refHistFile (refFile);
  FullHistNames allHists = histFile.all1DHists ();
  FullHistNames hists = RootFileDigger::match (FullHistName (pattern), allHists); 

  HistComparator comparator (globalTitle);

  for (size_t iHist = 0; iHist < hists.size(); ++iHist) {
    TH1* hist = histFile.getTH1 (hists[iHist]);
    if (!hist) {
      std::cerr << "Can not find 1D histogram " << hists[iHist].str() << " in file " << inputFileName << std::endl;
      continue;
    }
    TH1* refHist = refHistFile.getTH1 (hists[iHist]);
    if (!refHist) {
      std::cerr << "Can not find 1D histogram " << hists[iHist].str() << " in file " << refFileName << std::endl;
      continue;
    }
    std::string name = hists[iHist].str();
    for (size_t i = 0; i < name.size(); ++i) if (name[i] == ' ' || name[i] == '/') name[i] = '_';
    comparator.dumpComparison (hist, refHist, name+".gif");
  }
  return 0;
}
  
