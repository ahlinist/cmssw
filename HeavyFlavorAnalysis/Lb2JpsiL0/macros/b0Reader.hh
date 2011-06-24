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

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaJet.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/PidTable.hh"

#include "treeReader01.hh"
#include "DecayMap.hh"


class b0Reader : public treeReader01
{

public:
    b0Reader(TChain *tree, TString evtClassName);
    ~b0Reader();

    struct CheckedLbCand
    {
        CheckedLbCand () : isOk(false), jp(-1), l0(-1), mu1(-1), mu2(-1), pr(-1), pi(-1) {};
        bool isOk;
        int lb, jp, l0; // points to Cands
        int mu1, mu2, pr, pi; // points to SigTracks
    };

    struct CheckedB0Cand
    {
        CheckedB0Cand () : isOk(false), jp(-1), ks(-1), mu1(-1), mu2(-1), pi1(-1), pi2(-1) {};
        bool isOk;
        int B0, jp, ks; // points to Cands
        int mu1, mu2, pi1, pi2; // points to SigTracks
    };

    void bookHist();
    void bookReducedTree();
    void startAnalysis();
    void endAnalysis();
    void eventProcessing();
    void fillHist();
    void fillMuHist(TH1D* h, const int muId);
    void labelMuHisto(TH1D* h);
    void readCuts(TString filename, int dump = 1);
    bool checkCuts();
    void initVariables();
    void doGenLevelStuff();
    bool doCandStuff(const CheckedB0Cand &clc);
    bool doCandFitStuff(const CheckedB0Cand &clc);
    bool doTruthMatchingB0(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Pi1, const TAnaTrack *Pi2, const TVector3 &vtx);
    void doL1stuff();
    void doHLTstuff();
    void doTriggerMatching();
    void doEfficiencies();
    bool compareCands(const CheckedB0Cand clc1, const CheckedB0Cand clc2);
    int  getSigId(const TAnaCand *tac, const int id, int pos);
    int  getCandId(const TAnaCand *tac, const int id, int pos);
    CheckedB0Cand getCheckedB0Cand(const TAnaCand *tac);
    double square(double v) { return v*v; };

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
    int CUTB0Candidate, CUTB0CandidateFit;
    int CUTMuId1, CUTMuId2;  // mu ID
    double CUTptmuMin;       // min pt of both muons
    double CUTmjpWindow;     // mass window around jp PDG mass
    double CUTptjpMin;       // min pt of jp

    double CUTmKsWindow;     // mass window around Ks PDG mass
    double CUTptKsMin;       // min pt of jp
    double CUTptpiMin;       // min pt of pion

    bool CUTReadDecayMaps, CUTPrintDecayMaps; // read/print decay maps (only useful with MC)
    bool CUTgenTreeCandsOnly;
    std::string CUTDecayMap1, CUTDecayMap2;
    bool CUTuseHLTDoubleMu0, CUTuseHLTMu0TkMu0;

    // -- Variables
    TAnaCand    *fpCand1, *fpCand2;

    // -- Candidate variables
    double fmB0, fmKs, fmjp; // m
    double fmKstlv, fmjptlv;
    double fptB0, fptKs, fptjp; // pt
    double fpB0, fpKs, fpjp; // p
    double fetaB0, fetaKs, fetajp; // eta
    double fphiB0, fphiKs, fphijp; // phi
    double fyB0; // rapidity
    double fptgenB0, fmgenB0, fphigenB0, fetagenB0, fygenB0;

    double frpt1m, frpt2m, frptpi1, frptpi2; // kinematic variables of granddaughters
    double freta1m, freta2m, fretapi1, fretapi2;
    double frphi1m, frphi2m, frphipi1, frphipi2;
    int    frq1m, frq2m, frqpi1, frqpi2;
    int    frid1m, frid2m; // muon id

    // -- sig track variables, S is capi2talized intentionally for better distinction to the reco-variants
    double fSpt1m,  fSpt2m,  fSptpi1, fSptpi2;
    double fSp1m,  fSp2m;
    double fSeta1m, fSeta2m, fSetapi1, fSetapi2;
    double fSphi1m, fSphi2m, fSphipi1, fSphipi2;

    double fL0hypo; // mass of proton as Ks

    double falphaB0, falphaKs; // alpha
    double fptgangDRB0, fptgangDRKs; // ptgangDR
    double fmaxdocaB0, fmaxdocaKs, fmaxdocajp; // maxdoca

    double fPvLip, fPvLipE, fPvLip2, fPvLipE2; // longitudianl I.P. for best and 2nd best vertex

    double fd3B0, fd3Ks, fd3jp;    // 3d distance
    double fd3EB0, fd3EKs, fd3Ejp;
    double fct3dB0, fct3dB0E, fct3dKs, fct3dKsE; // ct3dau
    double fctxyB0, fctxyB0E, fctxyKs, fctxyKsE; // ctxyau
    double fbtB0x, fbtB0y, fbtB0z; // beta vector
    double fbtKsx, fbtKsy, fbtKsz;
    double fvxKs, fvyKs, fvzKs, fvrKs;
    double fvxB0, fvyB0, fvzB0, fvrB0;

    double fdxyB0, fdxyKs, fdxyjp; // 2d distance
    double fdxyEB0, fdxyEKs, fdxyEjp;

    double fchi2B0, fchi2Ks, fchi2jp; // quality of vtx fit
    double fndofB0, fndofKs, fndofjp;
    double fprobB0, fprobKs, fprobjp;

    double fchi21m, fchi22m, fchi2pi1, fchi2pi2; // quality of track fit
    double fprob1m, fprob2m, fprobpi1, fprobpi2;
    int    fndof1m, fndof2m, fndofpi1, fndofpi2;
    int    fqual1m, fqual2m, fqualpi1, fqualpi2;

    double fipr1m, fipr2m, fiprpi1, fiprpi2; // ratio of ip and track angle
    double farmQt, farmAl; // Armenteros variables

    double fdRpipi, fdRmumu, fdRKsjp; // deltaR of pairs for convenience

    double fangB0Ks; // angle between B0 and Ks (from cands, not alpha)

    int fnDaughters, fnGrandDaughters; // generator info
    int fmapRef1Gen, fmapRef2Gen;
    int fIsSig; // true if the cand block for this evt contains a signal decay
    int fIsMCmatch; // true if truth matched

    // for genCand stuff
    TTree* fGenTree;
    double fgmB0, fgmB0sw; // mass of B0 from ppi and swapped mass hypotheses
    double fgmKs, fgmKssw;
    double fgptpi1, fgptpi2, fgptmu1, fgptmu2, fgptKs;
    double fgetapi1, fgetapi2, fgetamu1, fgetamu2;
    double fgphipi1, fgphipi2, fgphimu1, fgphimu2;
    double fgpmu1, fgpmu2, fgppi1, fgppi2, fgpKs;
    double fgvxKs, fgvyKs, fgvzKs, fgvrKs, fgctKs;
    double fgvxB0, fgvyB0, fgvzB0, fgvrB0, fgctB0;
    double fgptB0, fgetaB0, fgyB0;
    double fgdRpipi, fgdRmumu, fgdRKsB0;
    double fganpipi, fganmumu, fganKsB0, fganKsjp;
    double fganKsmumin, fganKsmuPt;
    int    fgnDaughters, fgnGrandDaughters; // no of daughters from gen truth
    int    fgmapRef1Gen, fgmapRef2Gen; // pointers to entries in decay maps
    int    fghasCand;
    TGenCand *gcPrev;
    DecayMap myDecayMap1Gen;
    DecayMap myDecayMap2Gen;

    // for trigger stuff
    bool fL1TDMu0, fL1TDMu3;
    bool fL1TMuBH, fL1TMu0, fL1TMu3, fL1TMu5, fL1TMu7, fL1TMu10, fL1TMu14, fL1TMu20;

    bool fHLTqrk;
    bool fHLTDMu0;
    bool fHLTMu0TkMu0jp;
    bool fHLTDMu3jp;
    bool fHLTMu0jp;
    bool fHLTDMu6p5BarJp, fHLTDMu6p5JpDis, fHLTDMu6p5Jp, fHLTMu5L2Mu2Jpsi, fHLTMu5Tr2Jpsi, fHLTMu5Tr7Jpsi;
    int fHLTpreDMu6p5BarJp, fHLTpreDMu6p5JpDis, fHLTpreDMu6p5Jp, fHLTpreMu5L2Mu2Jpsi, fHLTpreMu5Tr2Jpsi, fHLTpreMu5Tr7Jpsi;
    bool fHLTDMu10BarJp, fHLTDMu7JpDis, fHLTDMu0Jp;
    int fHLTpreDMu10BarJp, fHLTpreDMu7JpDis, fHLTpreDMu0Jp;

    // summary trigger results
    bool fHLTokJpsi, fHLTokBarrelJpsi, fHLTokDisplJpsi;

    // Trigger matching result
    bool fHLTmatch;

    // PidTables
    PidTable *pidLambda0;
    PidTable *pidMuId, *pidMuTrk, *pidMuTrg, *pidMuTrg2;
    PidTable *pidCuts;

    // Efficiencies
    double fEfficiency, fEffErr;
    double fEffKs, fEffMu;
    double fEffMuId1, fEffMuTrk1, fEffMuTrg1, fEffMuId2, fEffMuTrk2, fEffMuTrg2;
    double fEffCuts;
};

#endif
