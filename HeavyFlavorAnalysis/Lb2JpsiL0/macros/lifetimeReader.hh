#ifndef GUARD_LIFETIMEREADER_H
#define GUARD_LIFETIMEREADER_H

#include <iostream>
#include <vector>
#include <utility>
#include <sstream>
#include <set>
#include <string>

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
#include "DecayMap.hh"

//==========================================================================
class lifetimeReader : public treeReader01
{

public:
    lifetimeReader(TChain *tree, TString evtClassName);
    ~lifetimeReader();

    enum anatype_t
    {
	at_LbJpL0    = 1,
	at_B0JpK0    = 2,
	at_B0JpKstar = 3,
	at_BpJpK     = 4,
	at_BsJpPhi   = 5
    };

    struct CheckedCand
    {
        CheckedCand () : isOk(false), bc(-1), jp(-1), rs(-1), mu1(-1), mu2(-1), ha1(-1), ha2(-1) {};
        bool isOk;
        int bc, jp, rs; // points to Cands
        int mu1, mu2, ha1, ha2; // points to SigTracks
    };

    // virtual methods from treeReader01:
    void bookHist();
    void readCuts(TString filename, int dump = 1);
    void startAnalysis();
    void endAnalysis();
    void eventProcessing();
    void initVariables();
    void fillHist();

    // methods specific to lifetimeReader and its decendants
    void bookReducedTree();
    virtual bool doCandStuff(const CheckedCand &clc);
    virtual bool doCandFitStuff(const CheckedCand &clc);
    virtual void doGenLevelStuff() = 0;
    virtual bool doTruthMatching(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Ha1, const TAnaTrack *Ha2 = 0, const TVector3 &vtx = TVector3::TVector3(-99, -99, -99)) = 0;
    virtual bool checkCuts() = 0;

    void doHLTstuff();
    void doPseudoHLTstuff();
    void doTriggerMatching();

    // helper methods
    void fillMuHist(TH1D* h, const int muId);
    void labelMuHisto(TH1D* h);
    bool isCowboy(const TAnaTrack *tr1, const TAnaTrack *tr2);

    // deltaPhi as from DataFormats/Math/interface/deltaPhi.h
    double deltaPhi(double phi1, double phi2) {
	double result = phi1 - phi2;
	while (result > M_PI) result -= 2*M_PI;
	while (result <= -M_PI) result += 2*M_PI;
	return result;
    };

    // methods for handling candidates from different origin (i.e. different kinematic fits)
    virtual bool compareCands(const CheckedCand clc1, const CheckedCand clc2) = 0;
    bool compareCands3prong(const CheckedCand clc1, const CheckedCand clc2);
    bool compareCands4prong(const CheckedCand clc1, const CheckedCand clc2);
    int  getSigId(const TAnaCand *tac, const int id, int pos);
    int  getCandId(const TAnaCand *tac, const int id, int pos);
    virtual CheckedCand getCheckedCand(const TAnaCand *tac) = 0;

    // some stuff for isolation, courtesy of Urs
    int nCloseTracks(const TAnaCand *pC, double dcaCut, double ptCut);
    int nearestPV(int pvIdx, double maxDist);
    double isoDocaTrack(const TAnaCand *pC);

    // cand check for efficiencies
    virtual void doCheckEffCands() = 0;

    // muon selection cuts test
    bool isTightMuon(const TAnaTrack *pT);
    bool isTightMuonNoTipCut(const TAnaTrack *pT);
    bool isSoftMuon(const TAnaTrack *pT);
    int numberOfPixLayers(const TAnaTrack *pTrack);

    // retrieve production vtx for oscillating particles in gen
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
    int CUTCandidate, CUTCandidateFit;
    int CUTMuId1, CUTMuId2;  // mu ID
    double CUTptmuMin;       // min pt of both muons
    double CUTmjpWindow;     // mass window around jp PDG mass
    double CUTptjpMin;       // min pt of jp

    double CUTmrsWindow;     // mass window around the resonances PDG mass
    double CUTptrsMin;       // min pt of resonance
    bool CUTptha1GTptha2;    // require pT(ha1)>pT(ha2)
    double CUTptha1Min;      // min pt of hdaron 1
    double CUTptha2Min;      // min pt of hadron 2

    double CUTisoCloseTrkDoca; // isolation closetrk: doca cut
    double CUTisoCloseTrkPt; // isolation closetrk: pT cut

    bool CUTReadDecayMaps, CUTPrintDecayMaps; // read/print decay maps (only useful with MC)
    bool CUTgenTreeCandsOnly;
    std::string CUTDecayMap1, CUTDecayMap2;

    // -- Variables
    TAnaCand    *fpCand1, *fpCand2;
    bool fForcePtHa1GTPtHa2;

    // -- Candidate variables
    int    fanatype; // type of analysis: 1: LambdaB, 2: B0, 3: Bplus, 4: Bs
    int    fnPV;
    double fmbc, fmrs, fmjp; // m
    double fmjptlv;
    double fptbc, fptrs, fptjp; // pt
    double fpbc, fprs, fpjp; // p
    double fetabc, fetars, fetajp; // eta
    double fphibc, fphirs, fphijp; // phi
    double fybc; // rapidity
    double fptgenbc, fmgenbc, fphigenbc, fetagenbc, fygenbc;

    double frptmu1, frptmu2, frptha1, frptha2; // kinematic variables of granddaughters
    double frpmu1, frpmu2, frphap, frpham; // hap: positive charged hadron, ham: minus
    double fretamu1, fretamu2, fretaha1, fretaha2;
    double frphimu1, frphimu2, frphiha1, frphiha2;
    int    frqmu1, frqmu2, frqha1, frqha2;
    int    fridmu1, fridmu2; // muon id

    // -- sig track variables, S is capitalized intentionally for better distinction to the reco-variants
    double fSptmu1,  fSptmu2,  fSptha1, fSptha2;
    double fSpmu1,  fSpmu2;
    double fSetamu1, fSetamu2, fSetaha1, fSetaha2;
    double fSphimu1, fSphimu2, fSphiha1, fSphiha2;

    double fKshypo, fL0hypo, fKstarhypo1, fKstarhypo2, fPhihypo; // hypothesis with different assumptions for the tagging resonance

    double falphabc, falphars, falpharsbc; // alpha
    double fptgangDRbc, fptgangDRrs; // ptgangDR
    double fmaxdocabc, fmaxdocars, fmaxdocajp; // maxdoca

    double fPvLip, fPvLipE, fPvLip2, fPvLipE2; // longitudianl I.P. for best and 2nd best vertex

    double fd3bc, fd3rs, fd3jp;    // 3d distance
    double fd3Ebc, fd3Ers, fd3Ejp;
    double fct3dbc, fct3dbcE, fct3drs, fct3drsE; // ct3dau
    double fct3dbc_jp;
    double fctbctruth, fd3dbctruth, fdxybctruth, fpbctruth, fptbctruth;
    double fctxybc, fctxybcE, fctxyrs, fctxyrsE; // ctxyau
    double fbtbcx, fbtbcy, fbtbcz; // beta vector
    double fbtrsx, fbtrsy, fbtrsz;
    double fvxrs, fvyrs, fvzrs, fvrrs, fvrrsPV;
    double fvxbc, fvybc, fvzbc, fvrbc, fvrbcPV;

    double fdxybc, fdxyrs, fdxyjp; // 2d distance
    double fdxyEbc, fdxyErs, fdxyEjp;

    double fchi2bc, fchi2rs, fchi2jp; // quality of vtx fit
    double fndofbc, fndofrs, fndofjp;
    double fprobbc, fprobrs, fprobjp;

    double fchi2mu1, fchi2mu2, fchi2ha1, fchi2ha2; // quality of track fit
    double fprobmu1, fprobmu2, fprobha1, fprobha2;
    int    fndofmu1, fndofmu2, fndofha1, fndofha2;
    int    fqualmu1, fqualmu2, fqualha1, fqualha2;
    int    falgomu1, falgomu2, falgoha1, falgoha2;
    int    fnpixmu1, fnpixmu2, fnpixha1, fnpixha2;

    bool   fIsMuTight, fIsMuTight2, fIsMuSoft;
    bool   fIsCowboy;

    double fiprmu1, fiprmu2, fiprha1, fiprha2; // ratio of ip and track angle
    double farmQt, farmAl; // Armenteros variables

    double fdRhaha, fdRmumu, fdRrsjp; // deltaR of pairs for convenience

    double fangbcrs; // angle between bc and rs (from cands, not alpha)

    int fIsoClosetrk; // Isolation: N closest tracks around candidate
    double fIsoDocaTrack; // Isolation: doca track

    int fnDaughters, fnGrandDaughters; // generator info
    int fmapRef1Gen, fmapRef2Gen;
    int fIsSig; // true if the cand block for this evt contains a signal decay
    bool fIsMCmatch; // true if truth matched

    // for genCand stuff
    TTree* fGenTree;
    double fgmbc, fgmbcsw; // mass of bc from pha2 and swapped mass hypotheses
    double fgmrs, fgmrssw;
    int fgqha1, fgqha2;
    double fgptha1, fgptha2, fgptmu1, fgptmu2, fgptrs;
    double fgetaha1, fgetaha2, fgetamu1, fgetamu2;
    double fgphiha1, fgphiha2, fgphimu1, fgphimu2;
    double fgpmu1, fgpmu2, fgpha1, fgpha2, fgprs;
    double fgphap, fgpham;
    double fgvxrs, fgvyrs, fgvzrs, fgvrrs, fgvrrsPV, fgctrs, fgd3drs, fgd2drs;
    double fgvxbc, fgvybc, fgvzbc, fgvrbc, fgvrbcPV, fgctbc, fgd3dbc;
    double fgpbc, fgptbc, fgetabc, fgybc;
    double fgpjp, fgptjp, fgetajp, fgyjp;
    double fgdRhaha, fgdRmumu, fgdRrsbc;
    double fganhaha, fganmumu, fganrsbc, fganrsjp;
    double fganrsmumin, fganrsmuPt;
    int    fgnDaughters, fgnGrandDaughters; // no of daughters from gen truth
    int    fgmapRef1Gen, fgmapRef2Gen; // pointers to entries in decay maps
    int    fghasCand, fghasJpCand, fghasrsCand;
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
    bool fHLTokBarrelJpsiMC, fHLTokDisplJpsiMC, fHLTokBarrelJpsiMCpseudo, fHLTokDisplJpsiMCpseudo;

    // Trigger matching result
    bool fHLTmatch;

    // Facility for trigger words inspection
    bool doTriggerLabelSet;
    std::set<std::string> triggerlabels;

private:
    TRandom3 ran;
    double getRan() { return ran.Uniform(); };
    bool checkRan(double thrsh) { return ran.Uniform() <= thrsh; };
    bool checkRan(bool val, double thrsh) { return val && ran.Uniform() <= thrsh; };
};

#endif
