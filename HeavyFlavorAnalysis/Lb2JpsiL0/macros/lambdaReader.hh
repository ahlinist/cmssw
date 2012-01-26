#ifndef GUARD_LAMBDAREADER_H
#define GUARD_LAMBDAREADER_H

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

class lambdaReader : public treeReader01
{

public:
    lambdaReader(TChain *tree, TString evtClassName);
    ~lambdaReader();

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
    bool doCandStuff(const CheckedLbCand &clc);
    bool doCandFitStuff(const CheckedLbCand &clc);
    bool doTruthMatchingLb(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Pi, const TAnaTrack *Pr, const TVector3 &vtx);
    void doHLTstuff();
    void doTriggerMatching();
    void doEfficiencies();
    bool compareCands(const CheckedLbCand clc1, const CheckedLbCand clc2);
    int  getSigId(const TAnaCand *tac, const int id, int pos);
    int  getCandId(const TAnaCand *tac, const int id, int pos);
    CheckedLbCand getCheckedLbCand(const TAnaCand *tac);
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
    int CUTLbCandidate, CUTLbCandidateFit;
    int CUTMuId1, CUTMuId2;  // mu ID
    double CUTptmuMin;       // min pt of both muons
    double CUTmjpWindow;     // mass window around jp PDG mass
    double CUTptjpMin;       // min pt of jp

    double CUTml0Window;     // mass window around L0 PDG mass
    double CUTptl0Min;       // min pt of jp
    bool CUTptpGTptpi;       // require pT(p)>pT(pi)
    double CUTptprMin;       // min pt of proton
    double CUTptpiMin;       // min pt of pion

    bool CUTReadDecayMaps, CUTPrintDecayMaps; // read/print decay maps (only useful with MC)
    bool CUTgenTreeCandsOnly;
    std::string CUTDecayMap1, CUTDecayMap2;
    bool CUTuseHLTDoubleMu0, CUTuseHLTMu0TkMu0;

    // -- Variables
    TAnaCand    *fpCand1, *fpCand2;

    // -- Candidate variables
    double fmlb, fml0, fmjp; // m
    double fml0tlv, fmjptlv;
    double fptlb, fptl0, fptjp; // pt
    double fplb, fpl0, fpjp; // p
    double fetalb, fetal0, fetajp; // eta
    double fphilb, fphil0, fphijp; // phi
    double fylb; // rapidity
    double fptgenlb, fmgenlb, fphigenlb, fetagenlb, fygenlb;

    double frpt1m, frpt2m, frptpr, frptpi; // kinematic variables of granddaughters
    double freta1m, freta2m, fretapr, fretapi;
    double frphi1m, frphi2m, frphipr, frphipi;
    int    frq1m, frq2m, frqpr, frqpi;
    int    frid1m, frid2m; // muon id

    // -- sig track variables, S is capitalized intentionally for better distinction to the reco-variants
    double fSpt1m,  fSpt2m,  fSptpr, fSptpi;
    double fSp1m,  fSp2m;
    double fSeta1m, fSeta2m, fSetapr, fSetapi;
    double fSphi1m, fSphi2m, fSphipr, fSphipi;

    double fKshypo; // mass of proton as Ks

    double falphalb, falphal0; // alpha
    double fptgangDRlb, fptgangDRl0; // ptgangDR
    double fmaxdocalb, fmaxdocal0, fmaxdocajp; // maxdoca

    double fPvLip, fPvLipE, fPvLip2, fPvLipE2; // longitudianl I.P. for best and 2nd best vertex

    double fd3lb, fd3l0, fd3jp;    // 3d distance
    double fd3Elb, fd3El0, fd3Ejp;
    double fct3dlb, fct3dlbE, fct3dl0, fct3dl0E; // ct3dau
    double fctlbtruth, fd3dlbtruth, fdxylbtruth, fplbtruth, fptlbtruth;
    double fctxylb, fctxylbE, fctxyl0, fctxyl0E; // ctxyau
    double fbtlbx, fbtlby, fbtlbz; // beta vector
    double fbtl0x, fbtl0y, fbtl0z;
    double fvxl0, fvyl0, fvzl0, fvrl0;
    double fvxlb, fvylb, fvzlb, fvrlb;

    double fdxylb, fdxyl0, fdxyjp; // 2d distance
    double fdxyElb, fdxyEl0, fdxyEjp;

    double fchi2lb, fchi2l0, fchi2jp; // quality of vtx fit
    double fndoflb, fndofl0, fndofjp;
    double fproblb, fprobl0, fprobjp;

    double fchi21m, fchi22m, fchi2pr, fchi2pi; // quality of track fit
    double fprob1m, fprob2m, fprobpr, fprobpi;
    int    fndof1m, fndof2m, fndofpr, fndofpi;
    int    fqual1m, fqual2m, fqualpr, fqualpi;

    double fipr1m, fipr2m, fiprpr, fiprpi; // ratio of ip and track angle
    double farmQt, farmAl; // Armenteros variables

    double fdRprpi, fdRmumu, fdRl0jp; // deltaR of pairs for convenience

    double fanglbl0; // angle between lb and l0 (from cands, not alpha)

    int fnDaughters, fnGrandDaughters; // generator info
    int fmapRef1Gen, fmapRef2Gen;
    int fIsSig; // true if the cand block for this evt contains a signal decay
    int fIsMCmatch; // true if truth matched

    // for genCand stuff
    TTree* fGenTree;
    double fgmlb, fgmlbsw; // mass of lb from ppi and swapped mass hypotheses
    double fgml0, fgml0sw;
    double fgptpr, fgptpi, fgptmu1, fgptmu2, fgptl0;
    double fgetapr, fgetapi, fgetamu1, fgetamu2;
    double fgphipr, fgphipi, fgphimu1, fgphimu2;
    double fgpmu1, fgpmu2, fgppr, fgppi, fgpl0;
    double fgvxl0, fgvyl0, fgvzl0, fgvrl0, fgctl0;
    double fgvxlb, fgvylb, fgvzlb, fgvrlb, fgctlb, fgd3dlb;
    double fgplb, fgptlb, fgetalb, fgylb;
    double fgdRprpi, fgdRmumu, fgdRl0lb;
    double fganprpi, fganmumu, fganl0lb, fganl0jp;
    double fganl0mumin, fganl0muPt;
    int    fgnDaughters, fgnGrandDaughters; // no of daughters from gen truth
    int    fgmapRef1Gen, fgmapRef2Gen; // pointers to entries in decay maps
    int    fghasCand;
    TGenCand *gcPrev;
    DecayMap myDecayMap1Gen;
    DecayMap myDecayMap2Gen;

    // for trigger stuff
    // inclusive Jpsi triggers
    bool fHLTDMu3jp, fHLTDMu6p5Jp, fHLTDMu0Jp, fHLTDMu0JpNoVtx;
    int fHLTpreDMu3jp, fHLTpreDMu6p5Jp, fHLTpreDMu0Jp, fHLTpreDMu0JpNoVtx;
    // inclusive Jpsi triggers barrel
    bool fHLTDMu6p5BarJp, fHLTDMu10BarJp, fHLTDMu13BarJp;
    int fHLTpreDMu6p5BarJp, fHLTpreDMu10BarJp, fHLTpreDMu13BarJp;
    // displaced triggers
    bool fHLTDMu3p5JpDis, fHLTDMu6p5JpDis, fHLTDMu7JpDis, fHLTDMu4JpDis, fHLTDMu5JpDis;
    int fHLTpreDMu3p5JpDis, fHLTpreDMu6p5JpDis, fHLTpreDMu7JpDis, fHLTpreDMu4JpDis, fHLTpreDMu5JpDis;
    //legacy triggers for compatibility with old code
    bool fHLTqrk; int fHLTpreqrk;
    bool fHLTDMu0, fHLTMu0TkMu0jp, fHLTMu0jp, fHLTMu5L2Mu2Jpsi, fHLTMu5Tr2Jpsi, fHLTMu5Tr7Jpsi;
    // single mu triggers for efficiencies
    bool fHLTSingleMu; // big or of the following triggers
    bool fHLTSingleIsoMu, fHLTSingleL1Mu, fHLTSingleL2Mu, fHLTSingleHLTMu;

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
    double fEffL0, fEffMu;
    double fEffMuId1, fEffMuTrk1, fEffMuTrg1, fEffMuId2, fEffMuTrk2, fEffMuTrg2;
    double fEffCuts;
};

#endif
