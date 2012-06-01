#ifndef GUARD_B0READER_H
#define GUARD_B0READER_H

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
#include <TRandom3.h>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"

#include "treeReader01.hh"
#include "lifetimeReader.hh"
#include "DecayMap.hh"

class b0Reader : public lifetimeReader
{

public:
    b0Reader(TChain *tree, TString evtClassName);
    ~b0Reader();

    // virtual methods from treeReader01:
    void bookHist();
    void endAnalysis();
    void initVariables();
    void fillHist();

    // methods specific to lifetimeReader and its decendants
    void bookReducedTree();
    bool doCandFitStuff(const CheckedCand &clc);
    void doGenLevelStuff();
    bool checkCuts();

    bool doTruthMatching(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Ha1, const TAnaTrack *Ha2, const TVector3 &vtx);
    bool compareCands(const CheckedCand clc1, const CheckedCand clc2);
    CheckedCand getCheckedCand(const TAnaCand *tac);
    // cand check for efficiencies
    void doCheckEffCands();

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

    // -- Variables
    TAnaCand    *fpCand1, *fpCand2;

    // -- Candidate variables

private:
    TRandom3 ran;
    double getRan() { return ran.Uniform(); };
    bool checkRan(double thrsh) { return ran.Uniform() <= thrsh; };
    bool checkRan(bool val, double thrsh) { return val && ran.Uniform() <= thrsh; };
};

#endif
