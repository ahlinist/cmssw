#ifndef GUARD_B0GENREADER_H
#define GUARD_B0GENREADER_H

#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

#include <TROOT.h>
#include <TString.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"

#include "treeReader01.hh"
#include "DecayMap.hh"


class b0GenReader : public treeReader01
{

public:
    b0GenReader(TChain *tree, TString evtClassName);
    ~b0GenReader();

    void bookHist();
    void bookReducedTree();
    void startAnalysis();
    void endAnalysis();
    void eventProcessing();
    void fillHist();
    void fillTree(TGenCand *gc);
    void readCuts(TString filename, int dump = 1);
    bool checkCuts();
    void initVariables();

    // some functions to check ancestry
    int findDaughter(TGenCand* gc, int pid);
    int findDaughter(TGenCand* gc, int pid, int number);
    int countDaughters(TGenCand* gc, int pid);
    int countDaughters(TGenCand* gc);
    bool goodDecay(TGenCand* gc, bool strict);

    TVector3 getProdVtx(TGenCand* gc, int pid);

    template <typename T> void setCut(T &var, std::string value)
    {
        std::istringstream iss(value);
        iss >> var;
    };

    template <typename T> void setCut(T &var, std::string value, TH1D *hcuts, int ibin, std::string title)
    {
        std::istringstream iss(value);
        iss >> var;
        hcuts->SetBinContent(ibin, var);
        hcuts->GetXaxis()->SetBinLabel(ibin, title.c_str());
    };

    // -- Cut values
    int CUTB0;

    // -- Variables
    // B0
    double fpB0, fptB0, fetaB0, fphiB0, fd3dB0, ft3dB0;
    bool fOsc, fIsB0bar;
    // Ks
    double fpKs, fptKs, fetaKs, fphiKs, fd3dKs, ft3dKs;
    double fvtxxKs, fvtxyKs, fvtxzKs, fvtxrKs; // w.r.t detector origin
    // mu
    double fpmu1, fptmu1, fetamu1, fphimu1;
    double fpmu2, fptmu2, fetamu2, fphimu2;
    // pi
    double fppi1, fptpi1, fetapi1, fphipi1;
    double fppi2, fptpi2, fetapi2, fphipi2;

    unsigned int fKsWarningCounter, fNfilled;
};

#endif
