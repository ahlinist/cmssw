#ifndef GUARD_LAMBDAGENREADER_H
#define GUARD_LAMBDAGENREADER_H

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


class lambdaGenReader : public treeReader01
{

public:
    lambdaGenReader(TChain *tree, TString evtClassName);
    ~lambdaGenReader();

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
    int CUTLb;

    // -- Variables
    // Lb
    double fpLb, fptLb, fetaLb, fphiLb, fd3dLb, ft3dLb;
    bool fOsc, fIsLbbar; // fOsc makes no sense in Lb but if this is true in any case there is a bug...
    // L0
    double fpL0, fptL0, fetaL0, fphiL0, fd3dL0, ft3dL0;
    double fvtxxL0, fvtxyL0, fvtxzL0, fvtxrL0; // w.r.t detector origin
    // mu
    double fpmu1, fptmu1, fetamu1, fphimu1;
    double fpmu2, fptmu2, fetamu2, fphimu2;
    // pi
    double fppr, fptpr, fetapr, fphipr;
    double fppi, fptpi, fetapi, fphipi;

    unsigned int fL0WarningCounter, fNfilled;
};

#endif
