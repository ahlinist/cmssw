#define TAUCALCULATION_PATCH
#include "b0Reader.hh"
#include "../../Bs2MuMu/interface/HFMasses.hh"

#include "TRandom.h"
#include <cmath>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Lb2JpsiL0_utils.h"
#include "DecayMap.hh"

using std::cout;
using std::endl;
using std::vector;

// ----------------------------------------------------------------------
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/b0Reader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------

//==========================================================================
class findDaughtersB0
{
public:
    findDaughtersB0() : nDaughters(0) {};
    findDaughtersB0(TGenCand* curParticle, TAna01Event* fpEvt) : nDaughters(0)
    {
        strDaughters = "";
        strDauGrdau = toString(TMath::Abs(curParticle->fID)) + " -> ";
        //strDauGrdau = "";
        const int nGenCands(fpEvt->nGenCands());
        mother = curParticle;
        if(mother->fDau1 > -1 && mother->fDau1 < nGenCands && mother->fDau2 > -1 && mother->fDau2 < nGenCands && mother->fDau1 <= mother->fDau2)
        {
	    // special case of K0:
	    // in some generators a K0 decays to Ks. For the list the particles of Ks are interesting, not the K0
	    if (TMath::Abs(mother->fID)==311 && mother->fDau1==mother->fDau2)
	    {
		TGenCand* K0 = fpEvt->getGenCand(mother->fDau1);
		if (TMath::Abs(K0->fID)==310) mother = K0;
	    }
            nDaughters = 0;
            nDauGrdau = 0;
            for(int dauit=mother->fDau1; dauit<=mother->fDau2; dauit++)
            {
                // <= is ok because fDau2 is id of last daughter in list
                TGenCand *gcDau = fpEvt->getGenCand(dauit);
                const unsigned int id = TMath::Abs(gcDau->fID);
                // check if these particles come from the right mother
                if(gcDau->fMom1==mother->fNumber&&gcDau->fMom2==mother->fNumber)
                {
                    nDaughters++;
                    strDaughters += toString(id) + " ";
                    findDaughtersB0 grdau(gcDau,fpEvt);
                    strDauGrdau += toString(id) + " (" + grdau.getDaughters() + ") " ;
                    strDauGrdaus += toString(id) + " (" + grdau.getDauGrdau() + ") " ;
                    nDauGrdau += grdau.getNDaughters();
                }
                // else: particle comes from hadronic interactions or decays in flight
                // discard this for the moment
            }
        }
    };
    std::string getDaughters()
    {
        return strDaughters;
    };
    std::string getDauGrdau()
    {
        return strDauGrdau;
    };
    std::string getDauGrdaus()
    {
        return strDauGrdaus;
    };
    unsigned int getNDaughters()
    {
        return nDaughters;
    };
    unsigned int getNDauGrdau()
    {
        return nDauGrdau;
    };

private:
    TGenCand* mother;
    std::string strDaughters;
    std::string strDauGrdau;
    std::string strDauGrdaus;
    unsigned int nDaughters; // no. of direct daughters
    unsigned int nDauGrdau; // no. of daughters of the direct daughters
};

//==========================================================================

b0Reader::b0Reader(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName)
{
    cout << "==> b0Reader: constructor..." << endl;
}

// ----------------------------------------------------------------------
b0Reader::~b0Reader()
{
    cout << "==> b0Reader: destructor..." << endl;
}

// ----------------------------------------------------------------------
void b0Reader::startAnalysis()
{
    cout << "==> b0Reader: Starting analysis..." << endl;
    // fill the map with some anchor values
    if(CUTReadDecayMaps)
    {
        cout << "Reading " << CUTDecayMap1 << " - " << myDecayMap1Gen.readFile(CUTDecayMap1.c_str()) << " entries read." << endl;
        cout << "Reading " << CUTDecayMap2 << " - " << myDecayMap2Gen.readFile(CUTDecayMap2.c_str()) << " entries read." << endl;
    }
    // book reduced trees
    bookReducedTree();
    // PidTables
    std::string path = "pidtables/LambdaB/";
    pidLambda0 = new PidTable((path+"pid_lambda0.dat").c_str());
    pidCuts = new PidTable((path+"pid_cuts.dat").c_str());
    if(!fIsMC)
    {
	pidMuId = new PidTable((path+"pid_muId.dat").c_str());
	pidMuTrk = new PidTable((path+"pid_muTrk.dat").c_str());
	pidMuTrg = new PidTable((path+"pid_muTrg_HLT2mu0_Run2010A.dat").c_str());
	pidMuTrg2 = new PidTable((path+"pid_muTrg_HLT2mu0_Run2010B.dat").c_str());
    }
    else
    {
	pidMuId = new PidTable((path+"pid_muIdMC.dat").c_str());
	pidMuTrk = new PidTable((path+"pid_muTrkMC.dat").c_str());
	pidMuTrg = new PidTable((path+"pid_muTrgMC.dat").c_str());
    }

}

// ----------------------------------------------------------------------
void b0Reader::endAnalysis()
{
    if (CUTPrintDecayMaps)
    {
        cout << "Decay map 1Gen: " << endl;
        myDecayMap1Gen.printMap();
        cout << "Decay map 2Gen: " << endl;
        myDecayMap2Gen.printMap();
    }
}

// ----------------------------------------------------------------------
void b0Reader::eventProcessing()
{
    initVariables(); // still very crude...

    ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks());
    ((TH1D*)fpHistFile->Get("h2"))->Fill(fpEvt->nCands());

    TAnaCand *pCand;

    // initialize a flag to mark in the genTree if there exists a corresponding candidate from signal
    fghasCand = 0;

    // MC only
    if (fIsMC) doGenLevelStuff();

    // do some trigger stuff
    doHLTstuff();

    // init something
    frpt1m = freta1m = frphi1m = 0;
    frpt2m = freta2m = frphi2m = 0;
    fHLTmatch = false;

    // main loop
    if (CUTB0CandidateFit == 0 || CUTB0CandidateFit == CUTB0Candidate)
    {
        // we fill everything from the same candidate
        int candcounter(0);
        for (int iC = 0; iC < fpEvt->nCands(); ++iC)
        {
            pCand = fpEvt->getCand(iC);
            ((TH1D*)fpHistFile->Get("h3"))->Fill(pCand->fType);

            if (CUTB0Candidate == pCand->fType)
            {
                CheckedB0Cand curCand = getCheckedB0Cand(pCand);
                if (curCand.isOk)
                {
		    doTriggerMatching();
		    doEfficiencies();
                    if (doCandStuff(curCand) && doCandFitStuff(curCand) && checkCuts()) fTree->Fill();
                    candcounter++;
                }
            }
        }
        ((TH1D*)fpHistFile->Get("h4"))->Fill(candcounter);
    }
    else
    {
        // we take data from different candidates, needs matching
        // populate the list of candidates
        std::vector<CheckedB0Cand> candVec, candVecFit;
        for (int iC = 0; iC < fpEvt->nCands(); ++iC)
        {
            pCand = fpEvt->getCand(iC);
            ((TH1D*)fpHistFile->Get("h3"))->Fill(pCand->fType);
            if (CUTB0Candidate == pCand->fType)
            {
                CheckedB0Cand curCand = getCheckedB0Cand(pCand);
                if (curCand.isOk) candVec.push_back(curCand);
            }
            if (CUTB0CandidateFit == pCand->fType)
            {
                CheckedB0Cand curCand = getCheckedB0Cand(pCand);
                if (curCand.isOk) candVecFit.push_back(curCand);
            }
        }
        std::vector<std::pair<CheckedB0Cand, CheckedB0Cand> > candPairVec;
        for(std::vector<CheckedB0Cand>::iterator it1 = candVec.begin(); it1!=candVec.end(); it1++)
            for(std::vector<CheckedB0Cand>::iterator it2 = candVecFit.begin(); it2!=candVecFit.end(); it2++)
            {
                if(compareCands( *it1,*it2 ))
                    candPairVec.push_back(std::make_pair((*it1),(*it2)));
            }
        ((TH1D*)fpHistFile->Get("h4"))->Fill(candVec.size());
        ((TH1D*)fpHistFile->Get("h5"))->Fill(candVecFit.size());
        ((TH1D*)fpHistFile->Get("h6"))->Fill(candPairVec.size());
        for(std::vector<std::pair<CheckedB0Cand,CheckedB0Cand> >::iterator it = candPairVec.begin();
                it != candPairVec.end(); it++)
        {
            if (doCandStuff((*it).first) && doCandFitStuff((*it).second))
	    {
		doTriggerMatching();
		doEfficiencies();
		if (checkCuts()) fTree->Fill();
	    }
        }
    }

    if (fIsMC && fIsSig) fGenTree->Fill();
    //if (fIsMC) fGenTree->Fill();
    return;
}

// ----------------------------------------------------------------------
int b0Reader::getSigId(const TAnaCand *tac, const int id, int pos)
{
    int ret(-1);
    //cout << "getSigId: " << tac->fSig1 << "-" << tac->fSig2 << " ";
    for(int i = tac->fSig1; i <= tac->fSig2; i++)
    {
        const TAnaTrack* tat = fpEvt->getSigTrack(i);
        if(TMath::Abs(tat->fMCID) == id) pos--;
        if(0 == pos)
        {
            ret = i;
            break;
        }
    }
    //cout << "found: " << ret << endl;
    return ret;
}

// ----------------------------------------------------------------------
int b0Reader::getCandId(const TAnaCand *tac, const int id, int pos)
{
    int ret(-1);
    //cout << "getCandId: " << tac->fDau1 << "-" << tac->fDau2 << " ";
    for(int i = tac->fDau1; i <= tac->fDau2; i++)
    {
        const TAnaCand* tacDau = fpEvt->getCand(i);
        const int tacDauId = tacDau->fType - int(tacDau->fType / 10000) * 10000;
        if(TMath::Abs(tacDauId) == id) pos--;
        if(0 == pos)
        {
            ret = i;
            break;
        }
    }
    //cout << "found: " << id << " at " << ret << endl;
    return ret;
}

// ----------------------------------------------------------------------
// check if two candidates point to the same two sig tracks for each daughter
bool b0Reader::compareCands(const CheckedB0Cand clc1, const CheckedB0Cand clc2)
{
    const int clc1mu1 = fpEvt->getSigTrack(clc1.mu1)->fIndex;
    const int clc1mu2 = fpEvt->getSigTrack(clc1.mu2)->fIndex;
    const int clc1pi1 = fpEvt->getSigTrack(clc1.pi1)->fIndex;
    const int clc1pi2 = fpEvt->getSigTrack(clc1.pi2)->fIndex;

    const int clc2mu1 = fpEvt->getSigTrack(clc2.mu1)->fIndex;
    const int clc2mu2 = fpEvt->getSigTrack(clc2.mu2)->fIndex;
    const int clc2pi1 = fpEvt->getSigTrack(clc2.pi1)->fIndex;
    const int clc2pi2 = fpEvt->getSigTrack(clc2.pi2)->fIndex;

    //cout << "compareCands: " << clc1mu1 << " " << clc1mu2 << " " << clc1pi << " " << clc1pr << " : "
    // << clc2mu1 << " " << clc2mu2 << " " << clc2pi << " " << clc2pr << endl;

    return ((clc1mu1 == clc2mu1)
            && (clc1mu2 == clc2mu2)
            && (clc1pi1 == clc2pi1)
            && (clc1pi2 == clc2pi2));
}

// ----------------------------------------------------------------------
// check if the candidates are complete
b0Reader::CheckedB0Cand b0Reader::getCheckedB0Cand(const TAnaCand *tac)
{
    CheckedB0Cand ret;
    ret.isOk = false;
    ret.B0 = tac->fIndex;
    // check if daughters point to something
    if (tac->fDau1 < 0 || tac->fDau2 < 0)
    {
        if (fVerbose > 4)  cout << "Problem: tac->fDau1: " << tac->fDau1 << " tac->fDau2: " << tac->fDau2
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }

    // now let's see if we have a jp and a ks
    ret.jp = getCandId(tac,443,1);
    ret.ks = getCandId(tac,310,1);
    if (ret.jp < 0 || ret.ks < 0)
    {
        if (fVerbose > 4) cout << "Problem finding jp/ks: ret.jp: " << ret.jp << " ret.ks: " << ret.ks
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }

    // check if daughters signal tracks exist
    TAnaCand *tacDau1 = fpEvt->getCand(tac->fDau1);
    if (tacDau1->fSig1 < 0 || tacDau1->fSig2 < 0)
    {
        if (fVerbose > 4) cout << "Problem: tacDau1->fSig1: " << tacDau1->fSig1 << " tacCur->fDaui->fSig2: " << tacDau1->fSig2
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }
    const TAnaTrack *tatDau1Sig1 = fpEvt->getSigTrack(tacDau1->fSig1);
    const TAnaTrack *tatDau1Sig2 = fpEvt->getSigTrack(tacDau1->fSig2);
    if (tatDau1Sig1->fIndex < 0 || tatDau1Sig2->fIndex < 0)
    {
        if (fVerbose > 4) cout << "Problem: tatDau1Sig1->fIndex: " << tatDau1Sig1->fIndex << " tatDau1Sig2->fIndex: " << tatDau1Sig2->fIndex
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }
    // and the same for the 2nd daughter
    TAnaCand *tacDau2 = fpEvt->getCand(tac->fDau2);
    if (tacDau2->fSig1 < 0 || tacDau2->fSig2 < 0)
    {
        if (fVerbose > 4) cout << "Problem: tacDau2->fSig1: " << tacDau2->fSig1 << " tacCur->fDaui->fSig2: " << tacDau2->fSig2
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }
    const TAnaTrack *tatDau2Sig1 = fpEvt->getSigTrack(tacDau2->fSig1);
    const TAnaTrack *tatDau2Sig2 = fpEvt->getSigTrack(tacDau2->fSig2);
    if (tatDau2Sig1->fIndex < 0 || tatDau2Sig2->fIndex < 0)
    {
        if (fVerbose > 4) cout << "Problem: tatDau2Sig1->fIndex: " << tatDau2Sig1->fIndex << " tatDau2Sig2->fIndex: " << tatDau2Sig2->fIndex
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }

    // ok, everything seems to point to something meaningful. Now tests we we can get our decay signals properly
    ret.mu1 = getSigId(fpEvt->getCand(ret.jp),13,1);
    ret.mu2 = getSigId(fpEvt->getCand(ret.jp),13,2);
    ret.pi1 = getSigId(fpEvt->getCand(ret.ks),211,1);
    ret.pi2 = getSigId(fpEvt->getCand(ret.ks),211,2);
    // cout << ret.jp << " " << ret.Ks << " " << ret.mu1 << " " << ret.mu2 << " " << ret.pi << " " << ret.pr << endl;
    ret.isOk = (ret.mu1 > -1) && (ret.mu2 > -1) && (ret.pi1 > -1) && (ret.pi2 > -1);
    return ret;
}

// ----------------------------------------------------------------------
bool b0Reader::doCandStuff(const CheckedB0Cand &clc)
{
    const TAnaCand *tacCur = fpEvt->getCand(clc.B0);
    // ---------------------------------------------------------
    // First we get some handier objects for later calculations
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacKs = fpEvt->getCand(clc.ks);

    // get the muons
    const TAnaTrack *tatSigMu1 = fpEvt->getSigTrack(clc.mu1);
    const TAnaTrack *tatSigMu2 = fpEvt->getSigTrack(clc.mu2);
    const TAnaTrack *tatRecMu1 = fpEvt->getRecTrack(tatSigMu1->fIndex);
    const TAnaTrack *tatRecMu2 = fpEvt->getRecTrack(tatSigMu2->fIndex);

    // require the muons to be of at least some quality
    fillMuHist((TH1D*)fpHistFile->Get("h20"), tatRecMu1->fMuID);
    fillMuHist((TH1D*)fpHistFile->Get("h21"), tatRecMu2->fMuID);
    if(tatRecMu1->fMuID==-1||(tatRecMu1->fMuID & CUTMuId1)!=CUTMuId1) return false;
    if(tatRecMu2->fMuID==-1||(tatRecMu2->fMuID & CUTMuId2)!=CUTMuId2) return false;
    fillMuHist((TH1D*)fpHistFile->Get("h22"), tatRecMu1->fMuID);
    fillMuHist((TH1D*)fpHistFile->Get("h23"), tatRecMu2->fMuID);

    // get the lambda daughters
    const TAnaTrack *tatSigPi1 = fpEvt->getSigTrack(clc.pi1);
    const TAnaTrack *tatSigPi2 = fpEvt->getSigTrack(clc.pi2);
    const TAnaTrack *tatRecPi1 = fpEvt->getRecTrack(tatSigPi1->fIndex);
    const TAnaTrack *tatRecPi2 = fpEvt->getRecTrack(tatSigPi2->fIndex);

    // get some TLorentzVector for eta and phi (and reuse it later)
    TLorentzVector tlvRecMu1, tlvRecMu2, tlvRecPi1, tlvRecPi2;
    tlvRecMu1.SetVectM(tatRecMu1->fPlab, MMUON);
    tlvRecMu2.SetVectM(tatRecMu2->fPlab, MMUON);
    tlvRecPi1.SetVectM(tatRecPi1->fPlab, MPION);
    tlvRecPi2.SetVectM(tatRecPi2->fPlab, MPION);

    TLorentzVector tlvB0, tlvKs, tlvJpsi;
    tlvB0.SetVectM(tacCur->fPlab, MB_0);
    tlvKs.SetVectM(tacKs->fPlab, MKSHORT);
    tlvJpsi.SetVectM(tacJpsi->fPlab, MJPSI);

    // sigtracks
    TLorentzVector tlvSigMu1, tlvSigMu2, tlvSigPi1, tlvSigPi2;
    tlvSigMu1.SetVectM(tatSigMu1->fPlab, MMUON);
    tlvSigMu2.SetVectM(tatSigMu2->fPlab, MMUON);
    tlvSigPi1.SetVectM(tatSigPi1->fPlab, MPION);
    tlvSigPi2.SetVectM(tatSigPi2->fPlab, MPION);

    // ---------------------------------------------------------
    // now we do some calculations

    // L0 hypothesis, strictly assuming the track with higher pT to be the proton
    // sigtracks
    TLorentzVector tlvPiAsPrHypo;
    tlvPiAsPrHypo.SetVectM((tatSigPi1->fPlab.Perp() > tatSigPi2->fPlab.Perp() ? tatSigPi1->fPlab : tatSigPi2->fPlab), MPROTON);
    const TLorentzVector tlvL0Hypoth = (tatSigPi1->fPlab.Perp() > tatSigPi2->fPlab.Perp() ? tlvSigPi2+tlvPiAsPrHypo : tlvSigPi1+tlvPiAsPrHypo );

    fghasCand = 1; // obviously we have a candidate, so we want to mark this

    // ---------------------------------------------------------
    // now comes the boring part: filling of the reduced tree

    // candidate data from fit
    fmKs=tacKs->fMass;
    fmKstlv=(tlvSigPi1+tlvSigPi2).M();
    fptKs=tacKs->fPlab.Perp();
    fpKs=tacKs->fPlab.Mag();
    fetaKs=tlvKs.Eta();
    fphiKs=tlvKs.Phi();

    fmjp=tacJpsi->fMass;
    fmjptlv=(tlvSigMu1+tlvSigMu2).M();
    fptjp=tacJpsi->fPlab.Perp();
    fpjp=tacJpsi->fPlab.Mag();
    fetajp=tlvJpsi.Eta();
    fphijp=tlvJpsi.Phi();

    // reco tracks
    frpt1m=tatRecMu1->fPlab.Perp();
    freta1m=tlvRecMu1.Eta();
    frphi1m=tlvRecMu1.Phi();
    frq1m=tatRecMu1->fQ;
    if(tatRecMu1->fMuID==-1) frid1m=0;
    else frid1m=tatRecMu1->fMuID;
    fchi21m=tatRecMu1->fChi2;
    fprob1m=TMath::Prob(tatRecMu1->fChi2,tatRecMu1->fDof);
    fndof1m=tatRecMu1->fDof;
    fqual1m=tatRecMu1->fTrackQuality;

    frpt2m=tatRecMu2->fPlab.Perp();
    freta2m=tlvRecMu2.Eta();
    frphi2m=tlvRecMu2.Phi();
    frq2m=tatRecMu2->fQ;
    if(tatRecMu2->fMuID==-1) frid2m=0;
    else frid2m=tatRecMu2->fMuID;
    fchi22m=tatRecMu2->fChi2;
    fprob2m=TMath::Prob(tatRecMu2->fChi2,tatRecMu2->fDof);
    fndof2m=tatRecMu2->fDof;
    fqual2m=tatRecMu2->fTrackQuality;

    frptpi1=tatRecPi1->fPlab.Perp();
    fretapi1=tlvRecPi1.Eta();
    frphipi1=tlvRecPi1.Phi();
    frqpi1=tatRecPi1->fQ;
    fchi2pi1=tatRecPi1->fChi2;
    fprobpi1=TMath::Prob(tatRecPi1->fChi2,tatRecPi1->fDof);
    fndofpi1=tatRecPi1->fDof;
    fqualpi1=tatRecPi1->fTrackQuality;

    frptpi2=tatRecPi2->fPlab.Perp();
    fretapi2=tlvRecPi2.Eta();
    frphipi2=tlvRecPi2.Phi();
    frqpi2=tatRecPi2->fQ;
    fchi2pi2=tatRecPi2->fChi2;
    fprobpi2=TMath::Prob(tatRecPi2->fChi2,tatRecPi2->fDof);
    fndofpi2=tatRecPi2->fDof;
    fqualpi2=tatRecPi2->fTrackQuality;

    // qualities of vertex fits
    fchi2Ks=tacKs->fVtx.fChi2;
    fchi2jp=tacJpsi->fVtx.fChi2;
    fndofKs=tacKs->fVtx.fNdof;
    fndofjp=tacJpsi->fVtx.fNdof;
    fprobKs=TMath::Prob(fchi2Ks,fndofKs);
    fprobjp=TMath::Prob(fchi2jp,fndofjp);

    // doca
    fmaxdocaB0=tacCur->fMaxDoca;
    fmaxdocaKs=tacKs->fMaxDoca;
    fmaxdocajp=tacJpsi->fMaxDoca;

    // dR and angles
    fdRpipi=tlvRecPi1.DeltaR(tlvRecPi2);
    fdRmumu=tlvRecMu1.DeltaR(tlvRecMu2);

    // K_s hypothesis
    fL0hypo=tlvL0Hypoth.M();

    return true;
}

// ----------------------------------------------------------------------
bool b0Reader::doCandFitStuff(const CheckedB0Cand &clc)
{
    const TAnaCand *tacCur = fpEvt->getCand(clc.B0);
    // ---------------------------------------------------------
    // First we get some handier objects for later calculations
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacKs = fpEvt->getCand(clc.ks);

    // get the muons
    const TAnaTrack *tatSigMu1 = fpEvt->getSigTrack(clc.mu1);
    const TAnaTrack *tatSigMu2 = fpEvt->getSigTrack(clc.mu2);
    const TAnaTrack *tatRecMu1 = fpEvt->getRecTrack(tatSigMu1->fIndex);
    const TAnaTrack *tatRecMu2 = fpEvt->getRecTrack(tatSigMu2->fIndex);

    // require the muons to be of at least some quality
    if(tatRecMu1->fMuID==-1||(tatRecMu1->fMuID & CUTMuId1)!=CUTMuId1) return false;
    if(tatRecMu2->fMuID==-1||(tatRecMu2->fMuID & CUTMuId2)!=CUTMuId2) return false;

    // get the lambda daughters
    const TAnaTrack *tatSigPi1 = fpEvt->getSigTrack(clc.pi1);
    const TAnaTrack *tatSigPi2 = fpEvt->getSigTrack(clc.pi2);
    const TAnaTrack *tatRecPi1 = fpEvt->getRecTrack(tatSigPi1->fIndex);
    const TAnaTrack *tatRecPi2 = fpEvt->getRecTrack(tatSigPi2->fIndex);

    // get some TLorentzVector for eta and phi (and reuse it later)
    TLorentzVector tlvRecMu1, tlvRecMu2, tlvRecPi1, tlvRecPi2;
    tlvRecMu1.SetVectM(tatRecMu1->fPlab, MMUON);
    tlvRecMu2.SetVectM(tatRecMu2->fPlab, MMUON);
    tlvRecPi1.SetVectM(tatRecPi1->fPlab, MPION);
    tlvRecPi2.SetVectM(tatRecPi2->fPlab, MPION);

    TLorentzVector tlvB0, tlvKs, tlvJpsi;
    tlvB0.SetVectM(tacCur->fPlab, MB_0);
    tlvKs.SetVectM(tacKs->fPlab, MKSHORT);
    tlvJpsi.SetVectM(tacJpsi->fPlab, MJPSI);

    // sigtracks
    TLorentzVector tlvSigMu1, tlvSigMu2, tlvSigPi1, tlvSigPi2;
    tlvSigMu1.SetVectM(tatSigMu1->fPlab, MMUON);
    tlvSigMu2.SetVectM(tatSigMu2->fPlab, MMUON);
    tlvSigPi1.SetVectM(tatSigPi1->fPlab,MPION);
    tlvSigPi2.SetVectM(tatSigPi2->fPlab,MPION);

    // ---------------------------------------------------------
    // now we do some calculations

    // calculate alpha (pointing angles in rads)
    const TVector3 vecJpsiKs = tacKs->fVtx.fPoint - tacJpsi->fVtx.fPoint;
    const TVector3 vecPV = fpEvt->getPV(tacCur->fPvIdx)->fPoint;
    const TVector3 vecPVB0 = tacJpsi->fVtx.fPoint - vecPV;
    const double alphaKs = vecJpsiKs.Angle(tacKs->fPlab);
    const double alphaB0 = vecPVB0.Angle(tacCur->fPlab);
    const double ptgangDRKs = vecJpsiKs.DeltaR(tacKs->fPlab);
    const double ptgangDRB0 = vecPVB0.DeltaR(tacCur->fPlab);

    // calculate beta factor as 3d vector
    const TVector3 vecBetaKs = vecJpsiKs * (1. / tlvKs.E());
    const TVector3 vecBetaB0 = vecPVB0   * (1. / tlvB0.E());

    fghasCand = 1; // obviously we have a candidate, so we want to mark this

    // ---------------------------------------------------------
    // now comes the boring part: filling of the reduced tree

    // candidate data from fit
    fmB0=tacCur->fMass;
    fptB0=tacCur->fPlab.Perp();
    fpB0=tacCur->fPlab.Mag();
    fetaB0=tlvB0.Eta();
    fphiB0=tlvB0.Phi();
    fyB0=tlvB0.Rapidity();

    // refitted tracks, "signal" tracks
    fSpt1m=tlvSigMu1.Perp();
    fSp1m=tlvSigMu1.P();
    fSeta1m=tlvSigMu1.Eta();
    fSphi1m=tlvSigMu1.Phi();

    fSpt2m=tlvSigMu2.Perp();
    fSp2m=tlvSigMu2.P();
    fSeta2m=tlvSigMu2.Eta();
    fSphi2m=tlvSigMu2.Phi();

    fSptpi1=tlvSigPi1.Perp();
    fSetapi1=tlvSigPi1.Eta();
    fSphipi1=tlvSigPi1.Phi();

    fSptpi2=tlvSigPi2.Perp();
    fSetapi2=tlvSigPi2.Eta();
    fSphipi2=tlvSigPi2.Phi();

    // flight lengths and their errors
    fd3B0=tacCur->fVtx.fD3d;
    fd3Ks=tacKs->fVtx.fD3d;
    fd3jp=tacJpsi->fVtx.fD3d;
    fd3EB0=tacCur->fVtx.fD3dE;
    fd3EKs=tacKs->fVtx.fD3dE;
    fd3Ejp=tacJpsi->fVtx.fD3dE;

    fdxyB0=tacCur->fVtx.fDxy;
    fdxyKs=tacKs->fVtx.fDxy;
    fdxyjp=tacJpsi->fVtx.fDxy;
    fdxyEB0=tacCur->fVtx.fDxyE;
    fdxyEKs=tacKs->fVtx.fDxyE;
    fdxyEjp=tacJpsi->fVtx.fDxyE;

    // qualities of vertex fits
    fchi2B0=tacCur->fVtx.fChi2;
    fndofB0=tacCur->fVtx.fNdof;
    fprobB0=TMath::Prob(fchi2B0,fndofB0);

    // ctau - written as dist/p*m as this is numerically more stable than using
    // the TLorentzVector::Gamma() functions to write beta*gamma
    //fct3dB0 = tacCur->fTau3d; // tacCur->fVtx.fD3d / tacCur->fPlab.Mag() * MLAMBDA_B;
    fct3dB0 = tacCur->fVtx.fD3d / tacCur->fPlab.Mag() * tacCur->fMass / TMath::Ccgs() * ( tacCur->fTau3d <0 ? -1. : 1.); // FRANK patch: Test mit aktuell gemessener Masse
    //fct3dB0 = tacCur->fVtx.fD3d / tacCur->fPlab.Mag() * MB_0 / TMath::Ccgs(); // FRANK patch: Test mit aktuell gemessener Masse
    //cout << "tacCur->fTau3d: " << tacCur->fTau3d << " fct3dB0: " << fct3dB0 << " ratio: " << tacCur->fTau3d / fct3dB0 << endl;
    //cout << "tacCur->fVtx.fD3d: " << tacCur->fVtx.fD3d << " tacCur->fPlab.Mag(): " << tacCur->fPlab.Mag() << " TMath::Ccgs(): " << TMath::Ccgs();
    //cout << " mass: " << MB_0 << " " << tacCur->fMass << endl;
    fct3dB0E = tacCur->fTau3dE; // tacCur->fVtx.fD3dE / tacCur->fPlab.Mag() * MLAMBDA_B; // Ok, error not yet final. Will need error of |p|
    fct3dKs = tacKs->fTau3d; // tacKs->fVtx.fD3d / tacKs->fPlab.Mag() * MLAMBDA_0;
    fct3dKsE = tacKs->fTau3dE; // tacKs->fVtx.fD3dE / tacKs->fPlab.Mag() * MLAMBDA_0;
#ifdef TAUCALCULATION_PATCH
    // this patches the wrongly calculated lifetime errors in the NTuple with the errors scaled from the flightlength
    fct3dB0E = fd3EB0 / fd3B0 * fct3dB0;
    fct3dKsE = fd3EKs / fd3Ks * fct3dKs;
#endif
    fctxyB0 = tacCur->fTauxy;
    fctxyB0E = tacCur->fTauxyE;
    fctxyKs = tacKs->fTauxy;
    fctxyKsE = tacKs->fTauxyE;
#ifdef TAUCALCULATION_PATCH
    // this patches the wrongly calculated lifetime errors in the NTuple with the errors scaled from the flightlength
    fctxyB0E = fdxyEB0 / fdxyB0 * fctxyB0;
    fctxyKsE = fdxyEKs / fdxyKs * fctxyKs;
#endif

    // vertex positions
    fvxB0 = tacCur->fVtx.fPoint.x();
    fvyB0 = tacCur->fVtx.fPoint.y();
    fvzB0 = tacCur->fVtx.fPoint.z();
    fvrB0 = tacCur->fVtx.fPoint.XYvector().Mod();
    fvxKs = tacKs->fVtx.fPoint.x();
    fvyKs = tacKs->fVtx.fPoint.y();
    fvzKs = tacKs->fVtx.fPoint.z();
    fvrKs = tacKs->fVtx.fPoint.XYvector().Mod();

    // longitudinal I.P. w.r.t best and 2nd best PV
    fPvLip = tacCur->fPvLip;
    fPvLipE = tacCur->fPvLipE;
    fPvLip2 = tacCur->fPvLip2;
    fPvLipE2 = tacCur->fPvLipE2;

    // beta vector components
    fbtB0x=vecBetaKs.x();
    fbtB0y=vecBetaKs.y();
    fbtB0z=vecBetaKs.z();
    fbtKsx=vecBetaB0.x();
    fbtKsy=vecBetaB0.y();
    fbtKsz=vecBetaB0.z();

    // other cut values
    // pointing angles
    falphaB0=alphaB0;
    falphaKs=alphaKs;
    fptgangDRKs = ptgangDRKs;
    fptgangDRB0 = ptgangDRB0;

    // dR and angles
    fdRKsjp=tlvKs.DeltaR(tlvB0);
    fangB0Ks=tlvKs.Angle(tlvB0.Vect());

    // ratio of impact parameter with track angle (exp.val. approx. 1)
    fipr1m = fd3B0*sin(tlvRecMu1.Angle(vecPVB0))/sqrt(tatRecMu1->fLip*tatRecMu1->fLip+tatRecMu1->fTip*tatRecMu1->fTip);
    fipr2m = fd3B0*sin(tlvRecMu2.Angle(vecPVB0))/sqrt(tatRecMu2->fLip*tatRecMu2->fLip+tatRecMu2->fTip*tatRecMu2->fTip);
    fiprpi1 = fd3Ks*sin(tlvRecPi1.Angle(vecJpsiKs))/sqrt(tatRecPi1->fLip*tatRecPi1->fLip+tatRecPi1->fTip*tatRecPi1->fTip);
    fiprpi2 = fd3Ks*sin(tlvRecPi2.Angle(vecJpsiKs))/sqrt(tatRecPi2->fLip*tatRecPi2->fLip+tatRecPi2->fTip*tatRecPi2->fTip);

    // Armenteros variables
    {
	const TVector3 p1 = tatRecPi1->fQ > 0 ? tatRecPi1->fPlab : tatRecPi2->fPlab;
	const TVector3 p2 = tatRecPi1->fQ > 0 ? tatRecPi2->fPlab : tatRecPi1->fPlab;
	const TVector3 p = p1+p2;
	farmQt = (p1.Cross(p2)).Mag()/p.Mag();
	farmAl = (p1.Mag2()-p2.Mag2())/p.Mag2();
    }

    // Do truth matching
    if (fIsMC)
    {
	// get the muons
	const TAnaTrack *tatSigMu1 = fpEvt->getSigTrack(clc.mu1);
	const TAnaTrack *tatSigMu2 = fpEvt->getSigTrack(clc.mu2);
	//const TAnaTrack *tatRecMu1 = fpEvt->getRecTrack(tatSigMu1->fIndex);
	//const TAnaTrack *tatRecMu2 = fpEvt->getRecTrack(tatSigMu2->fIndex);
	// get the lambda daughters
	const TAnaTrack *tatSigPi1 = fpEvt->getSigTrack(clc.pi1);
	const TAnaTrack *tatSigPi2 = fpEvt->getSigTrack(clc.pi2);
	//const TAnaTrack *tatRecPi = fpEvt->getRecTrack(tatSigPi->fIndex);
	//const TAnaTrack *tatRecPr = fpEvt->getRecTrack(tatSigPr->fIndex);

	//fIsMCmatch = doTruthMatchingLb(tatRecMu1, tatRecMu2, tatRecPi, tatRecPr, tacKs->fVtx.fPoint);
	fIsMCmatch = doTruthMatchingB0(tatSigMu1, tatSigMu2, tatSigPi1, tatSigPi2, tacKs->fVtx.fPoint);
    }

    return true;
}

// ----------------------------------------------------------------------
bool b0Reader::doTruthMatchingB0(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Pi1, const TAnaTrack *Pi2, const TVector3 &vtx)
{
    bool ret(false);
    TLorentzVector tlvMu1, tlvMu2, tlvPi1, tlvPi2;
    tlvMu1.SetVectM(Mu1->fPlab, MMUON);
    tlvMu2.SetVectM(Mu2->fPlab, MMUON);
    tlvPi1.SetVectM(Pi1->fPlab, MPION);
    tlvPi2.SetVectM(Pi2->fPlab, MPION);

    const int giMu1(fpEvt->getGenIndexWithDeltaR(tlvMu1,Mu1->fQ));
    const int giMu2(fpEvt->getGenIndexWithDeltaR(tlvMu2,Mu2->fQ));
    const int giPi1(fpEvt->getGenIndexWithDeltaR(tlvPi1,vtx,Pi1->fQ,0.2,1.3));
    const int giPi2(fpEvt->getGenIndexWithDeltaR(tlvPi2,vtx,Pi2->fQ,0.2,1.3));
    const int nGenCands(fpEvt->nGenCands());
    if( giMu1 < nGenCands && giMu2 < nGenCands && giPi1 < nGenCands && giPi2 < nGenCands &&
            giMu1 > -1 && giMu2 > -1 && giPi1 > -1 && giPi2 > -1 )
    {
        TGenCand *gcMu1 = fpEvt->getGenCand(giMu1);
	TGenCand *gcMu1Mom = fpEvt->getGenCand(gcMu1->fMom1);
	TGenCand *gcMu1MomMom = fpEvt->getGenCand(gcMu1Mom->fMom1);
        TGenCand *gcMu2 = fpEvt->getGenCand(giMu2);
	TGenCand *gcMu2Mom = fpEvt->getGenCand(gcMu2->fMom1);
	TGenCand *gcMu2MomMom = fpEvt->getGenCand(gcMu2Mom->fMom1);
        TGenCand *gcPion1 = fpEvt->getGenCand(giPi1);
	TGenCand *gcPion1Mom = fpEvt->getGenCand(gcPion1->fMom1);
	TGenCand *gcPion1MomMom = fpEvt->getGenCand(gcPion1Mom->fMom1);
	TGenCand *gcPion1MomMomMom = fpEvt->getGenCand(gcPion1MomMom->fMom1);
        TGenCand *gcPion2 = fpEvt->getGenCand(giPi2);
	TGenCand *gcPion2Mom= fpEvt->getGenCand(gcPion2->fMom1);
	TGenCand *gcPion2MomMom = fpEvt->getGenCand(gcPion2Mom->fMom1);
	TGenCand *gcPion2MomMomMom = fpEvt->getGenCand(gcPion2MomMom->fMom1);

	// first stage of check: are the tracks correctly identified?
        ret = (TMath::Abs(gcMu1->fID) == 13
               && TMath::Abs(gcMu2->fID) == 13
               && TMath::Abs(gcPion1->fID) == 211
               && TMath::Abs(gcPion2->fID) == 211);
	if (!ret && fVerbose>1) cout << "Truth matching failed at stage 1 (tracks do not match)" << endl;
	if (!ret) return ret;

	// second stage: are they from the correct ancestors?
	ret = (ret
	       && TMath::Abs(gcMu1Mom->fID) == 443
	       && TMath::Abs(gcMu2Mom->fID) == 443
               && TMath::Abs(gcPion1Mom->fID) == 310
	       && TMath::Abs(gcPion2Mom->fID) == 310);
	if (!ret && fVerbose>1) cout << "Truth matching failed at stage 2 (tracks have wrong ancestors)" << endl;
	if (!ret) return ret;

	// thirs stage: do particles from same vtx really come from same ancestor?
	ret = (ret
		&& gcMu1Mom->fNumber == gcMu2Mom->fNumber
		&& gcPion1Mom->fNumber == gcPion2Mom->fNumber);
	if (!ret && fVerbose>1) cout << "Truth matching failed at stage 3 (tracks do not come from same ancestors)" << endl;
	if (!ret) return ret;

	// fourth stage: do the J/psi tracks come from the same B0?
	ret = (ret
	    && TMath::Abs(gcMu1MomMom->fID) == 511
	    && TMath::Abs(gcMu2MomMom->fID) == 511
	    && gcMu1MomMom->fNumber == gcMu2MomMom->fNumber);
	if (!ret && fVerbose>1) cout << "Truth matching failed at stage 4 (J/psi tracks do not come from same B0)" << endl;
	if (!ret) return ret;

	// fifth stage: do the Ks tracks come from the same B0?
	// Here we have to skip one ancestor as the sequence is B0 -> K0 -> Ks -> pipi (511 -> 311 -> 310 -> 211 211)
	ret = (ret
		&& TMath::Abs(gcPion1MomMomMom->fID) == 511
		&& TMath::Abs(gcPion2MomMomMom->fID) == 511
		&& gcPion1MomMomMom->fNumber == gcPion2MomMomMom->fNumber);
	if (!ret && fVerbose>1) cout << "Truth matching failed at stage 5 (Ks tracks do not come from same B0)" << endl;
	if (ret && fVerbose>1) cout << "Truth matching succesful" << endl;

        if (fVerbose > 5)
        {
            cout << "Truth matching: Mu1: " << gcMu1->fID
                 << " Mu2: " << gcMu2->fID
                 << " Pion1: " << gcPion1->fID
                 << " Pion2: " << gcPion2->fID
                 << " isMatch: " << fIsMCmatch << " isSig: " << fIsSig << endl;
        }
    }
    return ret;
}

// ----------------------------------------------------------------------
void b0Reader::doGenLevelStuff()
{
    fmapRef1Gen = fgmapRef1Gen = fmapRef2Gen = fgmapRef2Gen = -1;
    fIsSig = false;
    // Do some generator level stuff
    const int nGenCands(fpEvt->nGenCands());
    for(int gcit=0; gcit!=nGenCands; gcit++)
    {
        TGenCand *gcCur = fpEvt->getGenCand(gcit);
        if(511==TMath::Abs(gcCur->fID))
        {
            findDaughtersB0 fd(gcCur,fpEvt);
	    // skip if we are at a B0 that does not decay into our channel, i.e. another one or one which oscillates to the interesting one
	    if ("511 -> 443 (13 13 ) 311 (211 211 ) "!=fd.getDauGrdau()) continue;
            fIsSig = true;
	    // some kinematic variables for comparison with reco
	    fptgenB0 = gcCur->fP.Pt();
	    fmgenB0 = gcCur->fMass;
	    fphigenB0 = gcCur->fP.Phi();
	    fetagenB0 = gcCur->fP.Eta();
	    fygenB0 = gcCur->fP.Rapidity();

            // reset tree variables
            fgmB0 = fgmB0sw = fgmKs = fgmKssw = 9999;
            fgptpi1 = fgptpi2 = fgptmu1 = fgptmu2 = 9999;
            fgetapi1 = fgetapi2 = fgetamu1 = fgetamu2 = 9999;
            fgphipi1 = fgphipi2 = fgphimu1 = fgphimu2 = 9999;
            fgpmu1 = fgpmu2 = fgppi1 = fgppi2 = 9999;
            fgptKs = fgpKs = 9999;
            fgdRpipi = fgdRmumu = fgdRKsB0 = 9999;
            fganpipi = fganmumu = fganKsjp = 9999;
	    fctB0truth = fd3dB0truth = fdxyB0truth = fpB0truth = fptB0truth = 9999;
            fgnDaughters = fgnGrandDaughters = fgmapRef1Gen = -1;
	    fgctB0 = 9999;
            // analyse the decay and extract some data to store in the tree
            fnDaughters = fgnDaughters = fd.getNDaughters();
            fnGrandDaughters = fgnGrandDaughters = fd.getNDauGrdau();
            fmapRef1Gen = fgmapRef1Gen = myDecayMap1Gen.getPos(fd.getDaughters());
            fmapRef2Gen = fgmapRef2Gen = myDecayMap2Gen.getPos(fd.getDauGrdau());
            // now extract some kinematic data
            TLorentzVector tlvGenJp, tlvGenKs, tlvGenPi1, tlvGenPi2, tlvGenMu1, tlvGenMu2, tlvGenKshort, tlvGenB0;
            TVector3 tv3PV, tv3B0V, tv3KsV;
	    // vertices in candblock denote where the particle was produced.
	    tv3PV = getProdVtx(gcCur, 511); // this gets the production vertex of the first B0 in case of mixing
	    // now search for the daughters of the B0
	    for(int gcDauit=gcCur->fDau1; gcDauit<=gcCur->fDau2; gcDauit++)
	    {
		TGenCand* gcDau=fpEvt->getGenCand(gcDauit);
		if(443==TMath::Abs(gcDau->fID))
		{
		    tv3B0V = gcDau->fV;
		    fgvxB0 = tv3B0V.x();
		    fgvyB0 = tv3B0V.y();
		    fgvzB0 = tv3B0V.z();
		    fgvrB0 = tv3B0V.XYvector().Mod();
		    // ct. Observe that Lb is in gcCur
		    fgpB0 = fpB0truth = gcCur->fP.Vect().Mag();
		    fgptB0 = fptB0truth = gcCur->fP.Vect().Perp();
		    fgd3dB0 = fd3dB0truth = (tv3B0V-tv3PV).Mag();
		    //fgctB0 = (tv3B0V-tv3PV).Mag() / gcCur->fP.Vect().Mag() * MB_0 / TMath::Ccgs();
		    fctB0truth = fgctB0 = fgd3dB0 / fgpB0 * MB_0 / TMath::Ccgs();
		    fdxyB0truth = (tv3B0V-tv3PV).Perp();
		    unsigned int mucounter(0);
		    tlvGenJp=gcDau->fP;
		    for(int gcGrDauit=gcDau->fDau1; gcGrDauit<=gcDau->fDau2; gcGrDauit++)
		    {
			TGenCand* gcGrDau=fpEvt->getGenCand(gcGrDauit);
			if(13==TMath::Abs(gcGrDau->fID)&&gcGrDau->fMom1==gcDau->fNumber&&gcGrDau->fMom2==gcDau->fNumber)
			{
			    mucounter++;
			    if (mucounter==1) tlvGenMu1=gcGrDau->fP;
			    if (mucounter==2) tlvGenMu2=gcGrDau->fP;
			}
		    }
		    if(mucounter>=2)
		    {
			fgptmu1 = tlvGenMu1.Pt();
			fgptmu2 = tlvGenMu2.Pt();
			fgpmu1 = tlvGenMu1.P();
			fgpmu2 = tlvGenMu2.P();
			fgetamu1 = tlvGenMu1.Eta();
			fgetamu2 = tlvGenMu2.Eta();
			fgphimu1 = tlvGenMu1.Phi();
			fgphimu2 = tlvGenMu2.Phi();
			fgdRmumu = tlvGenMu1.DeltaR(tlvGenMu2);
			fganmumu = tlvGenMu1.Angle(tlvGenMu2.Vect());
		    }
		}
		if(311==TMath::Abs(gcDau->fID))
		{
		    // The K0 needs to decay into a Ks
		    TGenCand* gcDauDau=fpEvt->getGenCand(gcDau->fDau1);
		    tlvGenKs=gcDauDau->fP;
		    bool pi1Found(false), pi2Found(false);
		    for(int gcGrDauit=gcDauDau->fDau1; gcGrDauit<=gcDauDau->fDau2; gcGrDauit++)
		    {
			TGenCand* gcGrDau=fpEvt->getGenCand(gcGrDauit);
			if(!pi2Found&&211==TMath::Abs(gcGrDau->fID)&&gcGrDau->fMom1==gcDauDau->fNumber&&gcGrDau->fMom2==gcDauDau->fNumber)
			{	// in case we have more than one pr we get the first
			    if (!pi1Found)
			    {
				pi1Found = true;
				tlvGenPi1=gcGrDau->fP;
				// fill in data for the Ks decay vertex
				tv3KsV = gcGrDau->fV;
				fgvxKs = tv3KsV.x();
				fgvyKs = tv3KsV.y();
				fgvzKs = tv3KsV.z();
				fgvrKs = tv3KsV.XYvector().Mod();
				// ct. Observe that Ks is in gcDauDau
				fgctKs = (tv3KsV-tv3PV).Mag() / gcDauDau->fP.Vect().Mag() * MKSHORT / TMath::Ccgs();
			    }
			    else
			    {
				pi2Found = true;
				tlvGenPi2=gcGrDau->fP;
			    }

			}
		    }
		    if(pi1Found&&pi2Found) // we have a pr and a pi
		    {
			fgptpi1 = tlvGenPi1.Pt();
			fgppi1 = tlvGenPi1.P();
			fgetapi1 = tlvGenPi1.Eta();
			fgphipi1 = tlvGenPi1.Phi();

			fgptpi2 = tlvGenPi2.Pt();
			fgppi2 = tlvGenPi2.P();
			fgetapi2 = tlvGenPi2.Eta();
			fgphipi2 = tlvGenPi2.Phi();

			fgdRpipi = tlvGenPi1.DeltaR(tlvGenPi2);
			fganpipi = tlvGenPi1.Angle(tlvGenPi2.Vect());
			// make a Ks
			tlvGenKshort = tlvGenPi1 + tlvGenPi2;
			fgmKs = tlvGenKshort.M();
			fgptKs = tlvGenKshort.Pt();
			fgpKs = tlvGenKshort.P();
			// make a Ks with swapped mass assumptions for pr and pi
			/*
			TLorentzVector tlvGenPrAsPi, tlvGenPiAsPr;
			tlvGenPrAsPi.SetVectM(tlvGenPr.Vect(),MPION);
			tlvGenPiAsPr.SetVectM(tlvGenPi.Vect(),MPROTON);
			const TLorentzVector tlvGenLambdaSwapped=tlvGenPrAsPi+tlvGenPiAsPr;
			fgmKssw = tlvGenLambdaSwapped.M();
			*/
			// make a Lb
			tlvGenB0 = tlvGenKshort+tlvGenJp;
			fgmB0 = tlvGenB0.M();
			//fgptB0 = tlvGenB0.Pt();
			fgetaB0 = tlvGenB0.Eta();
			fgyB0 = tlvGenB0.Rapidity();
			//const TLorentzVector tlvGenLambdaBSwapped = tlvGenLambdaSwapped+tlvGenJp;
			//fgmlbsw = tlvGenLambdaBSwapped.M();

			//
			//if (fgmKs>2) fpEvt->dumpGenBlock();
		    }
		}
		fgdRKsB0 = tlvGenJp.DeltaR(tlvGenKshort);
		fganKsjp = tlvGenJp.Angle(tlvGenKshort.Vect());
		fganKsB0 = tlvGenB0.Angle(tlvGenKshort.Vect());
		const double anKsmu1 = tlvGenKshort.Angle(tlvGenMu1.Vect());
		const double anKsmu2 = tlvGenKshort.Angle(tlvGenMu2.Vect());
		fganKsmumin = anKsmu1<anKsmu2 ? anKsmu1 : anKsmu2;
		fganKsmuPt = tlvGenMu1.Pt() > tlvGenMu2.Pt() ? anKsmu1 : anKsmu2;
	    }
        }
    }
}

void b0Reader::doHLTstuff()
{
    fHLTDMu3jp = fHLTDMu6p5Jp = fHLTDMu0Jp = fHLTDMu0JpNoVtx = false;
    fHLTDMu6p5BarJp = fHLTDMu10BarJp = fHLTDMu13BarJp = false;
    fHLTDMu3p5JpDis = fHLTDMu6p5JpDis = fHLTDMu7JpDis = fHLTDMu4JpDis = fHLTDMu5JpDis = false;
    fHLTDMu0 = fHLTMu0TkMu0jp = fHLTMu0jp = fHLTMu5L2Mu2Jpsi = fHLTMu5Tr2Jpsi = fHLTMu5Tr7Jpsi = false;
    fHLTSingleIsoMu = fHLTSingleL1Mu = fHLTSingleL2Mu = fHLTSingleHLTMu = false;
    fHLTokJpsi = fHLTokBarrelJpsi = fHLTokDisplJpsi = false;
    fHLTmatch = fHLTSingleMu = fHLTqrk = false;

    fHLTpreDMu3jp = fHLTpreDMu6p5Jp = fHLTpreDMu0Jp = fHLTpreDMu0JpNoVtx = 0;
    fHLTpreDMu6p5BarJp = fHLTpreDMu10BarJp = fHLTpreDMu13BarJp = 0;
    fHLTpreDMu3p5JpDis = fHLTpreDMu6p5JpDis = fHLTpreDMu7JpDis = fHLTpreDMu4JpDis = fHLTpreDMu5JpDis = 0;
    fHLTpreqrk = 0;

    for(int i=0; i!=NHLT; i++)
    {
	const std::string name = fpEvt->fHLTNames[i].Data();
	if (name == "") break; // if name is empty we reached the end of the list (according to Urs)
	const double prescale = fpEvt->fHLTPrescale[i];
	const bool wasrun = fpEvt->fHLTWasRun[i];
	const bool result = fpEvt->fHLTResult[i];
	const bool error = fpEvt->fHLTError[i];
	//if (name == "HLT_DoubleMu3p5_Jpsi_Displaced_v2")
	//cout << "HLT " << i << ": " << name << " result: " << result << " wasrun: " << wasrun << " prescale: " << prescale << " error: " << error << endl;
	//cout << name << " - wasrun: " << wasrun << " prescale: " << prescale << " result: " << result << " error: " << error << endl;
	if (result && wasrun && !error)
	{
	    // Inclusive Jpsi triggers
	    if (name.find("HLT_DoubleMu3_Jpsi_v")           !=std::string::npos) { fHLTDMu3jp      = true; fHLTpreDMu3jp      = prescale; }
	    if (name.find("HLT_Dimuon6p5_Jpsi_v")           !=std::string::npos) { fHLTDMu6p5Jp    = true; fHLTpreDMu6p5Jp    = prescale; }
	    if (name.find("HLT_Dimuon0_Jpsi_v")             !=std::string::npos) { fHLTDMu0Jp      = true; fHLTpreDMu0Jp      = prescale; }
	    if (name.find("HLT_Dimuon0_Jpsi_NoVertexing_v") !=std::string::npos) { fHLTDMu0JpNoVtx = true; fHLTpreDMu0JpNoVtx = prescale; }
	    fHLTokJpsi = (fHLTDMu3jp || fHLTDMu6p5Jp || fHLTDMu0Jp || fHLTDMu0JpNoVtx);

	    // Inclusive Jpsi triggers in barrel
	    if (name.find("HLT_Dimuon6p5_Barrel_Jpsi")      !=std::string::npos) { fHLTDMu6p5BarJp = true; fHLTpreDMu6p5BarJp = prescale; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel")       !=std::string::npos) { fHLTDMu10BarJp  = true; fHLTpreDMu10BarJp = prescale; }
	    if (name.find("HLT_Dimuon13_Jpsi_Barrel")       !=std::string::npos) { fHLTDMu13BarJp  = true; fHLTpreDMu13BarJp = prescale; }
	    fHLTokBarrelJpsi = (fHLTDMu6p5BarJp || fHLTDMu10BarJp || fHLTDMu13BarJp);

	    // Displaced Jpsi triggers
	    if (name.find("HLT_Dimuon3p5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu3p5JpDis = true; fHLTpreDMu3p5JpDis = prescale; }
	    if (name.find("HLT_DoubleMu3p5_Jpsi_Displaced") !=std::string::npos) { fHLTDMu3p5JpDis = true; fHLTpreDMu3p5JpDis = prescale; } // which spelling is correct?
	    if (name.find("HLT_Dimuon6p5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu6p5JpDis = true; fHLTpreDMu6p5JpDis = prescale; }
	    if (name.find("HLT_Dimuon7_Jpsi_Displaced")     !=std::string::npos) { fHLTDMu7JpDis   = true; fHLTpreDMu7JpDis = prescale; }
	    if (name.find("HLT_DoubleMu4_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu4JpDis   = true; fHLTpreDMu4JpDis = prescale; }
	    if (name.find("HLT_DoubleMu5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu5JpDis   = true; fHLTpreDMu5JpDis = prescale; }
	    fHLTokDisplJpsi = (fHLTDMu3p5JpDis || fHLTDMu6p5JpDis || fHLTDMu7JpDis || fHLTDMu4JpDis || fHLTDMu5JpDis);

	    // legacy trigger information, for backwards compatibility with 2010 only.
	    if (name.find("Quarkonium")                     !=std::string::npos) { fHLTqrk = true; fHLTpreqrk = prescale; }
	    // DoubleMu triggers (usually prescaled)
	    if ("HLT_DoubleMu0" == name) fHLTDMu0 = true;
	    // Jpsi triggers
	    if ("HLT_Mu0_TkMu0_Jpsi" == name) fHLTMu0TkMu0jp = true;
	    // Jpsi with OST
	    if (name.find("HLT_Mu0_TkMu0_OST_Jpsi")    !=std::string::npos) fHLTMu0jp = true;
	    // triggers from Run2011A v2 on
	    if ("HLT_Mu5_L2Mu2_Jpsi_v3" == name)  fHLTMu5L2Mu2Jpsi = true;
	    if ("HLT_Mu5_Track2_Jpsi_v2" == name) fHLTMu5Tr2Jpsi = true;
	    if ("HLT_Mu7_Track7_Jpsi_v3" == name) fHLTMu5Tr7Jpsi = true;

	    // A big OR of single Mu triggers for efficiency studies
	    // Put as many triggers as possible to collect as much events as possible
	    // --- isolated Mu triggers
	    if (name.find("HLT_IsoMu12_v")    !=std::string::npos) fHLTSingleIsoMu = true;
	    if (name.find("HLT_IsoMu15_v")    !=std::string::npos) fHLTSingleIsoMu = true;
	    if (name.find("HLT_IsoMu17_v")    !=std::string::npos) fHLTSingleIsoMu = true;
	    if (name.find("HLT_IsoMu24_v")    !=std::string::npos) fHLTSingleIsoMu = true;
	    if (name.find("HLT_IsoMu30_v")    !=std::string::npos) fHLTSingleIsoMu = true;
	    // --- L1 based triggers
	    if (name.find("HLT_L1SingleMu10_v") !=std::string::npos) fHLTSingleL1Mu = true;
	    if (name.find("HLT_L1SingleMu20_v") !=std::string::npos) fHLTSingleL1Mu = true;
	    // --- L2 based triggers
	    if (name.find("HLT_L2Mu10_v") !=std::string::npos) fHLTSingleL2Mu = true;
	    if (name.find("HLT_L2Mu20_v") !=std::string::npos) fHLTSingleL2Mu = true;
	    // --- HLT single mu triggers
	    if (name.find("HLT_Mu3_v")   !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu5_v")   !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu8_v")   !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu12_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu15_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu20_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu24_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu30_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu40_v")  !=std::string::npos) fHLTSingleHLTMu = true;
	    if (name.find("HLT_Mu100_v") !=std::string::npos) fHLTSingleHLTMu = true;
	    // --- and here the big OR
	    fHLTSingleMu = (fHLTSingleIsoMu || fHLTSingleL1Mu || fHLTSingleL2Mu || fHLTSingleHLTMu);
	    // a run number dependent summary result -- final decision -- CAUTION! They do/can not take prescales into account!!!!
	    if (!fIsMC)
	    {
		if (fRun >= 140058 && fRun < 147196) fHLTokJpsi = fHLTDMu0; // Run2010A
		else if (fRun >= 146428 && fRun <= 149294) fHLTokJpsi = fHLTMu0jp; // Run2010B
		else if (fRun >= 160405 && fRun <= 163261) fHLTokJpsi = fHLTDMu3jp; // Run2011A_v1
	    }
	    else
	    {
		// to be adjusted depending on the version of CMSSW used for MC
		// fHLTokJpsi = fHLTMu0jp; // MC for 2010
		fHLTokJpsi = fHLTDMu3jp; // official MC production 2011
	    }
	}
    }
}

// ----------------------------------------------------------------------
// a simple trigger matcher based on deltaR
void b0Reader::doTriggerMatching()
{
    const double deltaRthrsh(0.5); // threshold indpired by MuonAnalysis/MuonAssociators/python/patMuonsWithTrigger_cff.py
    int mu1match(-1), mu2match(-1);
    double deltaRminMu1(100), deltaRminMu2(100);
    TTrgObj *tto;
    TLorentzVector tlvMu1, tlvMu2;
    tlvMu1.SetPtEtaPhiM(frpt1m,freta1m,frphi1m,MMUON);
    tlvMu2.SetPtEtaPhiM(frpt2m,freta2m,frphi2m,MMUON);
    if (fVerbose > 5)
    {
	cout << "dump trigger objects ----------------------" << endl;
	cout << "mu1: pt,eta,phi: " << frpt1m << " " << freta1m << " " << frphi1m << " q: " << frq1m << endl;
	cout << "mu2: pt,eta,phi: " << frpt2m << " " << freta2m << " " << frphi2m << " q: " << frq2m << endl;
	cout << "HLT: fHLTMu0TkMu0jp: " << fHLTMu0TkMu0jp << " fHLTMu0jp: " << fHLTMu0jp
	     << endl;
    }

    for(int i=0; i!=fpEvt->nTrgObj(); i++)
    {
	tto = fpEvt->getTrgObj(i);
	if (fVerbose > 5) cout << "i: " << i << " ";
	if (fVerbose > 5) tto->dump();
	// if (fRun >= 164924) cout << fRun << ": " << tto->fLabel << tto->fP.DeltaR(tlvMu1) << ":" << tto->fP.DeltaR(tlvMu2) << endl;
	// the label changes according to datataking era, so we need to distinguish them
	if ( (fRun <  160405 && tto->fLabel == "hltMu0TkMuJpsiTrackMassFiltered:HLT::")
	  || (fRun >= 160405 && fRun < 163270 && tto->fLabel == "hltDoubleMu3QuarkoniumL3Filtered:HLT::")
	  || (fRun >= 163270 && fRun < 164237 && (tto->fLabel == "hltDimuon6p5JpsiDisplacedL3Filtered:HLT::" || tto->fLabel == "hltDimuon6p5JpsiL3Filtered:HLT::" || tto->fLabel == "hltDimuon6p5BarrelJpsiL3Filtered:HLT::"))
	  || (fRun >= 164924 && (tto->fLabel == "hltJpsiDisplacedL3Filtered:HLT::" || tto->fLabel == "hltBarrelJpsiL3Filtered:HLT::"))
	  || (fRun >= 164924 && (tto->fLabel == "hltDisplacedmumuFilterDoubleMu4Jpsi:HLT::" || tto->fLabel == "hltDoubleMu4BarrelBsL3Filtered:HLT::")))
	{
	    const double deltaR1 = tto->fP.DeltaR(tlvMu1);
	    const double deltaR2 = tto->fP.DeltaR(tlvMu2);
	    if (deltaR1<deltaRthrsh && deltaR1<deltaRminMu1)
	    {
		deltaRminMu1 = deltaR1;
		mu1match = i;
	    }
	    if (deltaR2<deltaRthrsh && deltaR2<deltaRminMu2)
	    {
		deltaRminMu2 = deltaR2;
		mu2match = i;
	    }
	}
    }
    fHLTmatch = (mu1match>=0 && mu2match >=0);
    if (fVerbose > 5) cout << "trigger matched: " << fHLTmatch
	<< " - result Mu1: " << mu1match << " dR: " << deltaRminMu1
	<< " Mu2: " << mu2match << " dR: " << deltaRminMu2 << endl;
}

// ----------------------------------------------------------------------
bool b0Reader::checkCuts()
{
    return ( (frpt1m > CUTptmuMin) && (frpt2m > CUTptmuMin)
	&& (fmjp > MJPSI-CUTmjpWindow) && (fmjp < MJPSI+CUTmjpWindow)
	&& (fptjp > CUTptjpMin)
	&& (fmKs > MKSHORT-CUTmKsWindow) && (fmKs < MKSHORT+CUTmKsWindow)
	&& (fptKs > CUTptKsMin)
	&& (frptpi1 > CUTptpiMin)
	&& (frptpi2 > CUTptpiMin)
	   );
}

// ----------------------------------------------------------------------
void b0Reader::doEfficiencies()
{
    bool effOk = true;
    fEffMuId1 = fEffMuId2 = fEffMuTrk1 = fEffMuTrk2 = fEffMuTrg1 = fEffMuTrg2 = -99;
    fEffMu = fEffKs = fEffCuts = fEfficiency = -99;
    double effErrSq(0);
    // get muon efficiencies
    fEffMuId1 = pidMuId->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
    effErrSq += square(pidMuId->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
    fEffMuId2 = pidMuId->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
    effErrSq += square(pidMuId->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
    fEffMuTrk1 = pidMuTrk->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
    effErrSq += square(pidMuTrk->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
    fEffMuTrk2 = pidMuTrk->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
    effErrSq += square(pidMuTrk->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
    if (CUTuseHLTMu0TkMu0)
    {
	fEffMuTrg1 = pidMuTrg->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
	effErrSq += square(pidMuTrg->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
	fEffMuTrg2 = pidMuTrg->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
	effErrSq += square(pidMuTrg->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
    }
    if (CUTuseHLTDoubleMu0)
    {
	if (!fIsMC)
	{
	    if (fRun >= 140058 && fRun < 147196) // use efficiencies for HLT_DoubleMu0 (Run2010A + part of B)
	    {
		fEffMuTrg1 = pidMuTrg->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
		effErrSq += square(pidMuTrg->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
	        fEffMuTrg2 = pidMuTrg->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
		effErrSq += square(pidMuTrg->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
	    }
	    if (fRun >= 147196 && fRun <= 149294) // use efficiencies for HLT_DoubleMu0_Quarkonium_v1 (Run2010B, most of)
	    {
		fEffMuTrg1 = pidMuTrg2->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
		effErrSq += square(pidMuTrg2->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
		fEffMuTrg2 = pidMuTrg2->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
		effErrSq += square(pidMuTrg2->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
	    }
	}
	else
	{   // be cautious! this must match the trigger. use pidMuTrg for older, pidMuTrg2 for newer MC
	    fEffMuTrg1 = pidMuTrg->effD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m);
	    effErrSq += square(pidMuTrg->errD(fSpt1m,TMath::Abs(fSeta1m),fSphi1m));
	    fEffMuTrg2 = pidMuTrg->effD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m);
	    effErrSq += square(pidMuTrg->errD(fSpt2m,TMath::Abs(fSeta2m),fSphi2m));
	}
    }
    //if (fIsMCmatch && fIsSig)
    //{
	//cout << "fRun: " << fRun << " fLS: " << fLS << " fEvt: " << fEvt << endl;
	//cout << "  fEffMuId1: " << fEffMuId1 << " fEffMuTrk1: " << fEffMuTrk1 << " fEffMuTrg1: " << fEffMuTrg1 << " (" << fSpt1m << "," << TMath::Abs(fSeta1m) << "," << fSphi1m << ")" << endl;
	//cout << "  fEffMuId2: " << fEffMuId2 << " fEffMuTrk2: " << fEffMuTrk2 << " fEffMuTrg2: " << fEffMuTrg2 << " (" << fSpt2m << "," << TMath::Abs(fSeta2m) << "," << fSphi2m << ")" << endl;
    //}
    fEffMu = fEffMuId1 * fEffMuId2 * fEffMuTrk1 * fEffMuTrk2 * fEffMuTrg1 * fEffMuTrg2;
    effOk = effOk && (fEffMuId1  >= 0) && (fEffMuId2  >= 0);
    effOk = effOk && (fEffMuTrk1 >= 0) && (fEffMuTrk2 >= 0);
    effOk = effOk && (fEffMuTrg1 >= 0) && (fEffMuTrg2 >= 0);
    // get lambda reco efficiency
    fEffKs = pidLambda0->effD(fptKs,TMath::Abs(fetaKs),fphiKs);
    effOk = effOk && (fEffKs >= 0);
    // get cuts efficiency
    fEffCuts = pidCuts->effD(fptB0,TMath::Abs(fetaB0),fphiB0);
    effOk = effOk && (fEffCuts >= 0);
    // calculate final efficiency
    if (effOk)
    {
	fEfficiency = fEffMu * fEffKs * fEffCuts;
	fEffErr = TMath::Sqrt(effErrSq);
    }
    else
    {
	fEfficiency = -99;
	fEffErr = -99;
    }
}

// ----------------------------------------------------------------------
void b0Reader::initVariables()
{
    fIsSig = false;
    fIsMCmatch = false;
}

// ----------------------------------------------------------------------
void b0Reader::fillHist()
{

    if (fVerbose > 5) cout << "fillHist()" << endl;
    ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks());

    if (0 != fpCand1)
    {
        ((TH1D*)fpHistFile->Get("h10"))->Fill(fpCand1->fPlab.Perp());
        ((TH1D*)fpHistFile->Get("h11"))->Fill(fpCand1->fMass);
        ((TH1D*)fpHistFile->Get("h12"))->Fill(fpCand1->fVtx.fChi2);

        fTree->Fill();
    }

}

// ----------------------------------------------------------------------
void b0Reader::fillMuHist(TH1D* h, const int muId)
{
    for(int i = 0; i!=15; i++)
    {
	const int bit = 0x1<<i;
	if((muId&bit)==bit) h->Fill(i);
    }
}


// ----------------------------------------------------------------------
void b0Reader::bookHist()
{
    cout << "==> b0Reader: bookHist " << endl;

    TH1D *h;
    h = new TH1D("h1", "Ntrk", 500, 0., 1000.);
    h = new TH1D("h2", "NCand", 20, 0., 20.);
    h = new TH1D("h3", "cand ID", 1000100, -100., 1000000.);

    h = new TH1D("h4", ("NCand " + toString(CUTB0Candidate)).c_str(), 20, 0., 20.);
    h = new TH1D("h5", ("NCand fit " + toString(CUTB0CandidateFit)).c_str(), 20, 0., 20.);
    h = new TH1D("h6", ("NCand matched " + toString(CUTB0Candidate) + " to " + toString(CUTB0CandidateFit)).c_str(), 20, 0., 20.);

    h = new TH1D("h10", "pT", 40, 0., 20.);
    h = new TH1D("h11", "mass", 50, 1.6, 2.1);
    h = new TH1D("h12", "chi2", 50, 0., 10.);

    h = new TH1D("h20", "muID1before", 15, 0., 15.);
    labelMuHisto(h);
    h = new TH1D("h21", "muID2before", 15, 0., 15.);
    labelMuHisto(h);
    h = new TH1D("h22", "muID1after", 15, 0., 15.);
    labelMuHisto(h);
    h = new TH1D("h23", "muID2after", 15, 0., 15.);
    labelMuHisto(h);

    return;
}

void b0Reader::labelMuHisto(TH1D* h)
{
    // bin 1 corresponds to entry 0
    h->GetXaxis()->SetBinLabel(1,"standalone");
    h->GetXaxis()->SetBinLabel(2,"global");
    h->GetXaxis()->SetBinLabel(3,"tracker");
    h->GetXaxis()->SetBinLabel(4,"?");
    h->GetXaxis()->SetBinLabel(5,"tr arb");
    h->GetXaxis()->SetBinLabel(6,"?");
    h->GetXaxis()->SetBinLabel(7,"gl prmpt tight");
    h->GetXaxis()->SetBinLabel(8,"TM last loose");
    h->GetXaxis()->SetBinLabel(9,"TM last tight");
    h->GetXaxis()->SetBinLabel(10,"TM2D loose");
    h->GetXaxis()->SetBinLabel(11,"TM2D tight");
    h->GetXaxis()->SetBinLabel(12,"TMone loose");
    h->GetXaxis()->SetBinLabel(13,"TMone tight");
    h->GetXaxis()->SetBinLabel(14,"TMlastLowPtLoose");
    h->GetXaxis()->SetBinLabel(15,"TMlastLowPtTight");
}

// ----------------------------------------------------------------------
void b0Reader::bookReducedTree()
{
    cout << "==> b0Reader: bookReducedTree" << endl;

    // create the events tree ======================================
    fTree = new TTree("events", "events");

    // run info
    fTree->Branch("run",     &fRun,     "run/I");
    fTree->Branch("event",   &fEvt,   "event/I");
    fTree->Branch("LS",      &fLS,      "LS/I");

    // candidates
    fTree->Branch("mB0",     &fmB0,     "mB0/D");
    fTree->Branch("mKs",     &fmKs,     "mKs/D");
    fTree->Branch("mKstlv",  &fmKstlv,  "mKstlv/D");
    fTree->Branch("mjp",     &fmjp,     "mjp/D");
    fTree->Branch("mjptlv",  &fmjptlv,  "mjptlv/D");
    fTree->Branch("ptB0",    &fptB0,    "ptB0/D");
    fTree->Branch("ptKs",    &fptKs,    "ptKs/D");
    fTree->Branch("pjp",     &fpjp,     "pjp/D");
    fTree->Branch("pB0",     &fpB0,     "pB0/D");
    fTree->Branch("pKs",     &fpKs,     "pKs/D");
    fTree->Branch("ptjp",    &fptjp,    "ptjp/D");
    fTree->Branch("etaB0",   &fetaB0,   "etaB0/D");
    fTree->Branch("etaKs",   &fetaKs,   "etaKs/D");
    fTree->Branch("etajp",   &fetajp,   "etajp/D");
    fTree->Branch("phiB0",   &fphiB0,   "phiB0/D");
    fTree->Branch("phiKs",   &fphiKs,   "phiKs/D");
    fTree->Branch("phijp",   &fphijp,   "phijp/D");
    fTree->Branch("yB0",     &fyB0,     "yB0/D");

    // gen matched variables for resolution estimation
    fTree->Branch("ptgenB0", &fptgenB0, "ptgenB0/D");
    fTree->Branch("mgenB0",  &fmgenB0,  "mgenB0/D");
    fTree->Branch("phigenB0",&fphigenB0,"phigenB0/D");
    fTree->Branch("etagenB0",&fetagenB0,"etagenB0/D");
    fTree->Branch("ygenB0",  &fygenB0,  "ygenB0/D");

    // signal tracks
    fTree->Branch("rpt1m",   &frpt1m,   "rpt1m/D");
    fTree->Branch("rpt2m",   &frpt2m,   "rpt2m/D");
    fTree->Branch("rptpi1",   &frptpi1,   "rptpi1/D");
    fTree->Branch("rptpi2",   &frptpi2,   "rptpi2/D");
    fTree->Branch("reta1m",  &freta1m,  "reta1m/D");
    fTree->Branch("reta2m",  &freta2m,  "reta2m/D");
    fTree->Branch("retapi1",  &fretapi1,  "retapi1/D");
    fTree->Branch("retapi2",  &fretapi2,  "retapi2/D");
    fTree->Branch("rphi1m",  &frphi1m,  "rphi1m/D");
    fTree->Branch("rphi2m",  &frphi2m,  "rphi2m/D");
    fTree->Branch("rphipi1",  &frphipi1,  "rphipi1/D");
    fTree->Branch("rphipi2",  &frphipi2,  "rphipi2/D");
    fTree->Branch("rid1m",   &frid1m,   "rid1m/I");
    fTree->Branch("rid2m",   &frid2m,   "rid2m/I");
    fTree->Branch("rq1m",    &frq1m,    "rq1m/I");
    fTree->Branch("rq2m",    &frq2m,    "rq2m/I");
    fTree->Branch("rqpi1",    &frqpi1,    "rqpi1/I");
    fTree->Branch("rqpi2",    &frqpi2,    "rqpi2/I");
    fTree->Branch("chi21m",  &fchi21m,  "chi21m/D");
    fTree->Branch("chi22m",  &fchi22m,  "chi22m/D");
    fTree->Branch("chi2pi1",  &fchi2pi1,  "chi2pi1/D");
    fTree->Branch("chi2pi2",  &fchi2pi2,  "chi2pi2/D");
    fTree->Branch("prob1m",  &fprob1m,  "prob1m/D");
    fTree->Branch("prob2m",  &fprob2m,  "prob2m/D");
    fTree->Branch("probpi1",  &fprobpi1,  "probpi1/D");
    fTree->Branch("probpi2",  &fprobpi2,  "probpi2/D");
    fTree->Branch("ndof1m",  &fndof1m,  "ndof1m/I");
    fTree->Branch("ndof2m",  &fndof2m,  "ndof2m/I");
    fTree->Branch("ndofpi1",  &fndofpi1,  "ndofpi1/I");
    fTree->Branch("ndofpi2",  &fndofpi2,  "ndofpi2/I");
    fTree->Branch("qual1m",  &fqual1m,  "qual1m/I");
    fTree->Branch("qual2m",  &fqual2m,  "qual2m/I");
    fTree->Branch("qualpi1",  &fqualpi1,  "qualpi1/I");
    fTree->Branch("qualpi2",  &fqualpi2,  "qualpi2/I");

    // impact parameter ratios
    fTree->Branch("ipr1m",   &fipr1m,   "ipr1m/D");
    fTree->Branch("ipr2m",   &fipr2m,   "ipr2m/D");
    fTree->Branch("iprpi1",   &fiprpi1,   "iprpi1/D");
    fTree->Branch("iprpi2",   &fiprpi2,   "iprpi2/D");
    // Armenteros variables
    fTree->Branch("armQt",   &farmQt,   "armQt/D");
    fTree->Branch("armAl",   &farmAl,   "armAl/D");

    fTree->Branch("L0hypo",  &fL0hypo,  "L0hypo/D");
    fTree->Branch("alphaB0", &falphaB0, "alphaB0/D");
    fTree->Branch("alphaKs", &falphaKs, "alphaKs/D");
    fTree->Branch("ptgangDRB0", &fptgangDRB0, "ptgangDRB0/D");
    fTree->Branch("ptgangDRKs", &fptgangDRKs, "ptgangDRKs/D");
    fTree->Branch("maxdocaB0",&fmaxdocaB0,    "maxdocaB0/D");
    fTree->Branch("maxdocaKs",&fmaxdocaKs,    "maxdocaKs/D");
    fTree->Branch("maxdocajp",&fmaxdocajp,    "maxdocajp/D");
    fTree->Branch("d3B0",    &fd3B0,    "d3B0/D");
    fTree->Branch("d3Ks",    &fd3Ks,    "d3Ks/D");
    fTree->Branch("d3jp",    &fd3jp,    "d3jp/D");
    fTree->Branch("d3EB0",   &fd3EB0,   "d3EB0/D");
    fTree->Branch("d3EKs",   &fd3EKs,   "d3EKs/D");
    fTree->Branch("d3Ejp",   &fd3Ejp,   "d3Ejp/D");
    fTree->Branch("dxyB0",   &fdxyB0,   "dxyB0/D");
    fTree->Branch("dxyKs",   &fdxyKs,   "dxyKs/D");
    fTree->Branch("dxyjp",   &fdxyjp,   "dxyjp/D");
    fTree->Branch("dxyEB0",  &fdxyEB0,  "dxyEB0/D");
    fTree->Branch("dxyEKs",  &fdxyEKs,  "dxyEKs/D");
    fTree->Branch("dxyEjp",  &fdxyEjp,  "dxyEjp/D");

    fTree->Branch("ct3dB0",    &fct3dB0,    "ct3dB0/D");
    fTree->Branch("ct3dB0E",   &fct3dB0E,   "ct3dB0E/D");
    fTree->Branch("ct3dKs",    &fct3dKs,    "ct3dKs/D");
    fTree->Branch("ct3dKsE",   &fct3dKsE,   "ct3dKsE/D");
    fTree->Branch("ctxyB0",    &fctxyB0,    "ctxyB0/D");
    fTree->Branch("ctxyB0E",   &fctxyB0E,   "ctxyB0E/D");
    fTree->Branch("ctxyKs",    &fctxyKs,    "ctxyKs/D");
    fTree->Branch("ctxyKsE",   &fctxyKsE,   "ctxyKsE/D");

    fTree->Branch("ctB0truth", &fctB0truth, "ctB0truth/D");
    fTree->Branch("d3dB0truth", &fd3dB0truth, "d3dB0truth/D");
    fTree->Branch("dxyB0truth", &fdxyB0truth, "dxyB0truth/D");
    fTree->Branch("pB0truth", &fpB0truth, "pB0truth/D");
    fTree->Branch("ptB0truth", &fptB0truth, "ptB0truth/D");

    fTree->Branch("PvLip",   &fPvLip,   "PvLip/D");
    fTree->Branch("PvLipE",  &fPvLipE,  "PvLipE/D");
    fTree->Branch("PvLip2",  &fPvLip2,  "PvLip2/D");
    fTree->Branch("PvLipE2", &fPvLipE2, "PvLipE2/D");

    fTree->Branch("btB0x",   &fbtB0x,   "btB0x/D");
    fTree->Branch("btB0y",   &fbtB0y,   "btB0y/D");
    fTree->Branch("btB0z",   &fbtB0z,   "btB0z/D");
    fTree->Branch("btKsx",   &fbtKsx,   "btKsx/D");
    fTree->Branch("btKsy",   &fbtKsy,   "btKsy/D");
    fTree->Branch("btKsz",   &fbtKsz,   "btKsz/D");

    fTree->Branch("vxB0",    &fvxB0,    "vxB0/D");
    fTree->Branch("vyB0",    &fvyB0,    "vyB0/D");
    fTree->Branch("vzB0",    &fvzB0,    "vzB0/D");
    fTree->Branch("vrB0",    &fvrB0,    "vrB0/D");
    fTree->Branch("vxKs",    &fvxKs,    "vxKs/D");
    fTree->Branch("vyKs",    &fvyKs,    "vyKs/D");
    fTree->Branch("vzKs",    &fvzKs,    "vzKs/D");
    fTree->Branch("vrKs",    &fvrKs,    "vrKs/D");

    fTree->Branch("chi2B0",  &fchi2B0,  "chi2B0/D");
    fTree->Branch("chi2Ks",  &fchi2Ks,  "chi2Ks/D");
    fTree->Branch("chi2jp",  &fchi2jp,  "chi2jp/D");
    fTree->Branch("ndofB0",  &fndofB0,  "ndofB0/D");
    fTree->Branch("ndofKs",  &fndofKs,  "ndofKs/D");
    fTree->Branch("ndofjp",  &fndofjp,  "ndofjp/D");
    fTree->Branch("probB0",  &fprobB0,  "probB0/D");
    fTree->Branch("probKs",  &fprobKs,  "probKs/D");
    fTree->Branch("probjp",  &fprobjp,  "probjp/D");

    fTree->Branch("dRpipi",  &fdRpipi,  "dRpipi/D");
    fTree->Branch("dRmumu",  &fdRmumu,  "dRmumu/D");
    fTree->Branch("dRKsjp",  &fdRKsjp,  "dRKsjp/D");

    fTree->Branch("angB0Ks",&fangB0Ks,"angB0Ks/D");
    // gen info to main tree
    fTree->Branch("nDau",    &fnDaughters,"nDau/I");
    fTree->Branch("nGDau",   &fnGrandDaughters,"nGDau/I");
    fTree->Branch("nRef1G",  &fmapRef1Gen,"nRef1G/I");
    fTree->Branch("nRef2G",  &fmapRef2Gen,"nRef2G/I");
    fTree->Branch("isSig",   &fIsSig,     "isSig/I");
    fTree->Branch("isMCmatch",&fIsMCmatch, "isMCmatch/I");

    // sigtrack info
    fTree->Branch("Spt1m",   &fSpt1m,   "Spt1m/D");
    fTree->Branch("Sp1m",    &fSp1m,    "Sp1m/D");
    fTree->Branch("Seta1m",  &fSeta1m,  "Seta1m/D");
    fTree->Branch("Sphi1m",  &fSphi1m,  "Sphi1m/D");
    fTree->Branch("Spt2m",   &fSpt2m,   "Spt2m/D");
    fTree->Branch("Sp2m",    &fSp2m,    "Sp2m/D");
    fTree->Branch("Seta2m",  &fSeta2m,  "Seta2m/D");
    fTree->Branch("Sphi2m",  &fSphi2m,  "Sphi2m/D");
    fTree->Branch("Sptpi1",   &fSptpi1,   "Sptpi1/D");
    fTree->Branch("Setapi1",  &fSetapi1,  "Setapi1/D");
    fTree->Branch("Sphipi1",  &fSphipi1,  "Sphipi1/D");
    fTree->Branch("Sptpi2",   &fSptpi2,   "Sptpi2/D");
    fTree->Branch("Setapi2",  &fSetapi2,  "Setapi2/D");
    fTree->Branch("Sphipi2",  &fSphipi2,  "Sphipi2/D");

    // Trigger info
    fTree->Branch("HLTqrk", &fHLTqrk, "HLTqrk/O");
    fTree->Branch("HLTDMu3jp", &fHLTDMu3jp, "HLTDMu3jp/O");
    fTree->Branch("HLTDMu6p5Jp", &fHLTDMu6p5Jp, "HLTDMu6p5Jp/O");
    fTree->Branch("HLTDMu0Jp", &fHLTDMu0Jp, "HLTDMu0Jp/O");
    fTree->Branch("HLTDMu0JpNoVtx", &fHLTDMu0JpNoVtx, "HLTDMu0JpNoVtx/O");
    fTree->Branch("HLTDMu6p5BarJp", &fHLTDMu6p5BarJp, "HLTDMu6p5BarJp/O");
    fTree->Branch("HLTDMu10BarJp", &fHLTDMu10BarJp, "HLTDMu10BarJp/O");
    fTree->Branch("HLTDMu13BarJp", &fHLTDMu13BarJp, "HLTDMu13BarJp/O");
    fTree->Branch("HLTDMu3p5JpDis", &fHLTDMu3p5JpDis, "HLTDMu3p5JpDis/O");
    fTree->Branch("HLTDMu6p5JpDis", &fHLTDMu6p5JpDis, "HLTDMu6p5JpDis/O");
    fTree->Branch("HLTDMu7JpDis", &fHLTDMu7JpDis, "HLTDMu7JpDis/O");
    fTree->Branch("HLTDMu4JpDis", &fHLTDMu4JpDis, "HLTDMu4JpDis/O");
    fTree->Branch("HLTDMu5JpDis", &fHLTDMu5JpDis, "HLTDMu5JpDis/O");
    fTree->Branch("HLTDMu0", &fHLTDMu0, "HLTDMu0/O");
    fTree->Branch("HLTMu0TkMu0jp", &fHLTMu0TkMu0jp, "HLTMu0TkMu0jp/O");
    fTree->Branch("HLTMu0jp", &fHLTMu0jp, "HLTMu0jp/O");
    fTree->Branch("HLTMu5L2Mu2Jpsi", &fHLTMu5L2Mu2Jpsi, "HLTMu5L2Mu2Jpsi/O");
    fTree->Branch("HLTMu5Tr2Jpsi", &fHLTMu5Tr2Jpsi, "HLTMu5Tr2Jpsi/O");
    fTree->Branch("HLTMu5Tr7Jpsi", &fHLTMu5Tr7Jpsi, "HLTMu5Tr7Jpsi/O");
    fTree->Branch("HLTSingleMu", &fHLTSingleMu, "HLTSingleMu/O");
    fTree->Branch("HLTSingleIsoMu", &fHLTSingleIsoMu, "HLTSingleIsoMu/O");
    fTree->Branch("HLTSingleL1Mu", &fHLTSingleL1Mu, "HLTSingleL1Mu/O");
    fTree->Branch("HLTSingleL2Mu", &fHLTSingleL2Mu, "HLTSingleL2Mu/O");
    fTree->Branch("HLTSingleHLTMu", &fHLTSingleHLTMu, "HLTSingleHLTMu/O");
    fTree->Branch("HLTokJpsi", &fHLTokJpsi, "HLTokJpsi/O");
    fTree->Branch("HLTokBarrelJpsi", &fHLTokBarrelJpsi, "HLTokBarrelJpsi/O");
    fTree->Branch("HLTokDisplJpsi", &fHLTokDisplJpsi, "HLTokDisplJpsi/O");

    fTree->Branch("HLTpreqrk", &fHLTpreqrk, "HLTpreqrk/I");
    fTree->Branch("HLTpreDMu3jp", &fHLTpreDMu3jp, "HLTpreDMu3jp/I");
    fTree->Branch("HLTpreDMu6p5Jp", &fHLTpreDMu6p5Jp, "HLTpreDMu6p5Jp/I");
    fTree->Branch("HLTpreDMu0Jp", &fHLTpreDMu0Jp, "HLTpreDMu0Jp/I");
    fTree->Branch("HLTpreDMu0JpNoVtx", &fHLTpreDMu0JpNoVtx, "HLTpreDMu0JpNoVtx/I");
    fTree->Branch("HLTpreDMu6p5BarJp", &fHLTpreDMu6p5BarJp, "HLTpreDMu6p5BarJp/I");
    fTree->Branch("HLTpreDMu10BarJp", &fHLTpreDMu10BarJp, "HLTpreDMu10BarJp/I");
    fTree->Branch("HLTpreDMu13BarJp", &fHLTpreDMu13BarJp, "HLTpreDMu13BarJp/I");
    fTree->Branch("HLTpreDMu3p5JpDis", &fHLTpreDMu3p5JpDis, "HLTpreDMu3p5JpDis/I");
    fTree->Branch("HLTpreDMu6p5JpDis", &fHLTpreDMu6p5JpDis, "HLTpreDMu6p5JpDis/I");
    fTree->Branch("HLTpreDMu7JpDis", &fHLTpreDMu7JpDis, "HLTpreDMu7JpDis/I");
    fTree->Branch("HLTpreDMu4JpDis", &fHLTpreDMu4JpDis, "HLTpreDMu4JpDis/I");
    fTree->Branch("HLTpreDMu5JpDis", &fHLTpreDMu5JpDis, "HLTpreDMu5JpDis/I");

    fTree->Branch("HLTmatch", &fHLTmatch, "HLTmatch/O");

    // efficiencies
    fTree->Branch("EffKs",     &fEffKs,      "EffKs/D");
    fTree->Branch("EffMu",     &fEffMu,      "EffMu/D");
    fTree->Branch("EffMuId1",  &fEffMuId1,   "EffMuId1/D");
    fTree->Branch("EffMuTrk1", &fEffMuTrk1,  "EffMuTrk1/D");
    fTree->Branch("EffMuTrg1", &fEffMuTrg1,  "EffMuTrg1/D");
    fTree->Branch("EffMuId2",  &fEffMuId2,   "EffMuId2/D");
    fTree->Branch("EffMuTrk2", &fEffMuTrk2,  "EffMuTrk2/D");
    fTree->Branch("EffMuTrg2", &fEffMuTrg2,  "EffMuTrg2/D");
    fTree->Branch("EffCuts",   &fEffCuts,    "EffCuts/D");
    fTree->Branch("Eff",       &fEfficiency, "Eff/D");
    fTree->Branch("EffErr",    &fEffErr,     "EffErr/D");

    // Generator tree ==========================================
    fGenTree = new TTree("genevents", "genevents");
    // run info
    fGenTree->Branch("run",     &fRun,     "run/I");
    fGenTree->Branch("event",   &fEvt,   "event/I");
    fGenTree->Branch("LS",      &fLS,      "LS/I");

    // MC info
    fGenTree->Branch("nDau",    &fgnDaughters,"nDau/I");
    fGenTree->Branch("nGDau",   &fgnGrandDaughters,"nGDau/I");
    fGenTree->Branch("nRef1G",  &fgmapRef1Gen,"nRef1G/I");
    fGenTree->Branch("nRef2G",  &fgmapRef2Gen,"nRef2G/I");
    fGenTree->Branch("hasCand", &fghasCand,"hasCand/I");
    fGenTree->Branch("isSig",   &fIsSig,   "isSig/I");

    // Lambda_b
    fGenTree->Branch("mB0",     &fgmB0,    "gmB0/D");
    fGenTree->Branch("mB0Swap", &fgmB0sw,  "gmB0sw/D");
    fGenTree->Branch("mKs",     &fgmKs,    "gmKs/D");
    fGenTree->Branch("mKsSwap", &fgmKssw,  "gmKssw/D");

    fGenTree->Branch("vxB0",    &fgvxB0,   "vxB0/D");
    fGenTree->Branch("vyB0",    &fgvyB0,   "vyB0/D");
    fGenTree->Branch("vzB0",    &fgvzB0,   "vzB0/D");
    fGenTree->Branch("vrB0",    &fgvrB0,   "vrB0/D");
    fGenTree->Branch("ctB0",    &fgctB0,   "ctB0/D");
    fGenTree->Branch("d3dB0",   &fgd3dB0,  "d3dB0/D");

    fGenTree->Branch("pB0",     &fgpB0,    "pB0/D");
    fGenTree->Branch("ptB0",    &fgptB0,   "ptB0/D");
    fGenTree->Branch("etaB0",   &fgetaB0,  "etaB0/D");
    fGenTree->Branch("yB0",     &fgyB0,    "yB0/D");

    fGenTree->Branch("vxKs",    &fgvxKs,   "vxKs/D");
    fGenTree->Branch("vyKs",    &fgvyKs,   "vyKs/D");
    fGenTree->Branch("vzKs",    &fgvzKs,   "vzKs/D");
    fGenTree->Branch("vrKs",    &fgvrKs,   "vrKs/D");
    fGenTree->Branch("ctKs",    &fgctKs,   "ctKs/D");

    // Signal tracks
    fGenTree->Branch("ptmu1",   &fgptmu1,  "ptmu1/D");
    fGenTree->Branch("pmu1",    &fgpmu1,  "pmu1/D");
    fGenTree->Branch("etamu1",  &fgetamu1, "etamu1/D");
    fGenTree->Branch("phimu1",  &fgphimu1, "phimu1/D");

    fGenTree->Branch("ptmu2",   &fgptmu2,  "ptmu2/D");
    fGenTree->Branch("pmu2",    &fgpmu2,  "pmu2/D");
    fGenTree->Branch("etamu2",  &fgetamu2, "etamu2/D");
    fGenTree->Branch("phimu2",  &fgphimu2, "phimu2/D");

    fGenTree->Branch("ptpi1",    &fgptpi1,   "ptpi1/D");
    fGenTree->Branch("ppi1",     &fgppi1,    "ppi1/D");
    fGenTree->Branch("etapi1",   &fgetapi1,  "etapi1/D");
    fGenTree->Branch("phipi1",   &fgphipi1,  "phipi1/D");

    fGenTree->Branch("ptpi2",    &fgptpi2,   "ptpi2/D");
    fGenTree->Branch("ppi2",     &fgppi2,    "ppi2/D");
    fGenTree->Branch("etapi2",   &fgetapi2,  "etapi2/D");
    fGenTree->Branch("phipi2",   &fgphipi2,  "phipi2/D");

    // Lambda_b direct daughters
    fGenTree->Branch("ptKs",    &fgptKs,   "ptKs/D");
    fGenTree->Branch("pKs",     &fgpKs,    "pKs/D");

    fGenTree->Branch("dRpipi",  &fgdRpipi, "dRpipi/D");
    fGenTree->Branch("dRmumu",  &fgdRmumu, "dRmumu/D");
    fGenTree->Branch("dRKsB0",  &fgdRKsB0, "dRKsB0/D");

    // Angles between tracks
    fGenTree->Branch("anpipi",  &fganpipi, "anpipi/D");
    fGenTree->Branch("anmumu",  &fganmumu, "anmumu/D");
    fGenTree->Branch("anKsjp",  &fganKsjp, "anKsjp/D");
    fGenTree->Branch("anKsB0",  &fganKsB0, "anKsB0/D");
    fGenTree->Branch("anKsmumin",&fganKsmumin, "anKsmumin/D");
    fGenTree->Branch("anKsmuPt",&fganKsmuPt,"anKsmuPt/D");

}

// ----------------------------------------------------------------------
void b0Reader::readCuts(TString filename, int dump)
{
    // set some default cuts
    CUTB0CandidateFit = 0; // indicates non set value
    // read cut file
    fCutFile = filename;
    if (dump) cout << "==> b0Reader: Reading " << fCutFile.Data() << " for cut settings" << endl;
    ifstream infile(fCutFile.Data());

    TString fn(fCutFile.Data());

    if (dump)
    {
        cout << "====================================" << endl;
        cout << "==> b0Reader: Cut file  " << fCutFile.Data() << endl;
        cout << "------------------------------------" << endl;
    }

    TH1D *hcuts = new TH1D("hcuts", "", 40, 0., 40.);
    hcuts->GetXaxis()->SetBinLabel(1, fn.Data());

    // loop over lines of input file
    std::string strLine;
    while (std::getline(infile,strLine))
    {
        // strip off everything after a hash or a slash
        strLine = stripOff(strLine, '#');
        strLine = stripOff(strLine, '/');
        std::istringstream iss(strLine, std::istringstream::in);
        // intermediate storage of the entries
        std::string key, value;
        if (iss >> key >> value)
        {
            if (dump) cout << key << ": " << value << endl;
            // a switch statement doesn't work as C++ does not allow to take strings as switch argument
            // http://www.codeguru.com/cpp/cpp/cpp_mfc/article.php/c4067
            if("CUTB0Candidate" == key) // The candidate to retrieve signal values
            {
                setCut(CUTB0Candidate, value);
                //setCut(CUTB0Candidate, value, hcuts, 1, "Candidate");
                continue;
            }
            if("CUTB0CandidateFit" == key) // The candidate to retrieve kinematic vertex fit data
            {
                setCut(CUTB0CandidateFit, value);
                continue;
            }
            if("CUTMuId1" == key)
            {
                setCut(CUTMuId1, value);
                continue;
            }
            if("CUTMuId2" == key)
            {
                setCut(CUTMuId2, value);
                continue;
            }
            if("CUTptmuMin" == key)
            {
                setCut(CUTptmuMin, value, hcuts, 1, "p_{T)^{min}(#mu_{i})");
                continue;
            }
            if("CUTmjpWindow" == key)
            {
                setCut(CUTmjpWindow, value, hcuts, 2, "#pmm(J/#psi)");
                continue;
            }
            if("CUTptjpMin" == key)
            {
                setCut(CUTptjpMin, value, hcuts, 3, "p_{T}^{min}(J/#psi)");
                continue;
            }
            if("CUTmKsWindow" == key)
            {
                setCut(CUTmKsWindow, value, hcuts, 4, "#pmm(#Lambda^{0})");
                continue;
            }
            if("CUTptKsMin" == key)
            {
                setCut(CUTptKsMin, value, hcuts, 5, "p_{T}^{min}(#Lambda^{0})");
                continue;
            }
            if("CUTptpiMin" == key)
            {
                setCut(CUTptpiMin, value, hcuts, 8, "p_{T}^{min}(#pi)");
                continue;
            }
            if("CUTReadDecayMaps" == key)
            {
                setCut(CUTReadDecayMaps, value);
                continue;
            }
            if("CUTPrintDecayMaps" == key)
            {
                setCut(CUTPrintDecayMaps, value);
                continue;
            }
            if("CUTDecayMap1" == key)
            {
                setCut(CUTDecayMap1, value);
                continue;
            }
            if("CUTDecayMap2" == key)
            {
                setCut(CUTDecayMap2, value);
                continue;
            }

            cout << "==> b0Reader: ERROR in cutfile: Don't know what to do with " << key << "=" << value << endl;
        }
    }

    if (dump)  cout << "------------------------------------" << endl;

}

// ----------------------------------------------------------------------
// a recursive retriever for B0, as B0 may come from another B0 due to oscillation
TVector3 b0Reader::getProdVtx(TGenCand* gc, int pid)
{
    TVector3 ret = gc->fV;
    for(int gcMomIt=gc->fMom1; gcMomIt<=gc->fMom2; gcMomIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcMomIt);
	if (TMath::Abs(gcCur->fID) == pid) ret = getProdVtx(gcCur, pid);
    }
    return ret;
}

