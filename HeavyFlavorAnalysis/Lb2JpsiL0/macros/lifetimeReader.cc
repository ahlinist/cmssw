#define noTAUCALCULATION_PATCH
#include "lifetimeReader.hh"
#include "../../Bs2MuMu/interface/HFMasses.hh"

#include "TRandom3.h"
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
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/lifetimeReader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------


//==========================================================================

lifetimeReader::lifetimeReader(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName)
{
    cout << "==> lifetimeReader: constructor..." << endl;
}

// ----------------------------------------------------------------------
lifetimeReader::~lifetimeReader()
{
    cout << "==> lifetimeReader: destructor..." << endl;
}

// ----------------------------------------------------------------------
void lifetimeReader::startAnalysis()
{
    cout << "==> lifetimeReader: Starting analysis..." << endl;
    doTriggerLabelSet = true; // populate triggermap for trigger diagnosis
    // fill the map with some anchor values
    if(CUTReadDecayMaps)
    {
        cout << "Reading " << CUTDecayMap1 << " - " << myDecayMap1Gen.readFile(CUTDecayMap1.c_str()) << " entries read." << endl;
        cout << "Reading " << CUTDecayMap2 << " - " << myDecayMap2Gen.readFile(CUTDecayMap2.c_str()) << " entries read." << endl;
    }
    // book reduced trees
    bookReducedTree();
    // Initialise rng
    ran.SetSeed(25042012);
}

// ----------------------------------------------------------------------
void lifetimeReader::endAnalysis()
{
    if (CUTPrintDecayMaps)
    {
        cout << "Decay map 1Gen: " << endl;
        myDecayMap1Gen.printMap();
        cout << "Decay map 2Gen: " << endl;
        myDecayMap2Gen.printMap();
    }
    if (doTriggerLabelSet)
    {
	cout << "content of map triggerlabels: ------------" << endl;
	for (std::set<std::string>::const_iterator it = triggerlabels.begin(); it!=triggerlabels.end(); it++)
	    cout << *it << endl;
	cout << "------------------------------------------" << endl;
    }
}

// ----------------------------------------------------------------------
void lifetimeReader::eventProcessing()
{
    initVariables();

    ((TH1D*)fpHistFile->Get("h1"))->Fill(fpEvt->nRecTracks());
    ((TH1D*)fpHistFile->Get("h2"))->Fill(fpEvt->nCands());

    TAnaCand *pCand;

    // initialize a flag to mark in the genTree if there exists a corresponding candidate from signal
    fghasCand = 0;

    // MC only
    if (fIsMC) doGenLevelStuff();

    // do some trigger stuff
    doHLTstuff();
    // check if event has efficiency candidates (partial cands)
    doCheckEffCands();

    // main loop
    if (CUTCandidateFit == 0 || CUTCandidateFit == CUTCandidate)
    {
        // we fill everything from the same candidate
        int candcounter(0);
        for (int iC = 0; iC < fpEvt->nCands(); ++iC)
        {
            pCand = fpEvt->getCand(iC);
            ((TH1D*)fpHistFile->Get("h3"))->Fill(pCand->fType);

            if (CUTCandidate == pCand->fType)
            {
                CheckedCand curCand = getCheckedCand(pCand);
                if (curCand.isOk)
                {
		    doTriggerMatching();
		    if (fIsMC) doPseudoHLTstuff();
                    if (doCandStuff(curCand) && doCandFitStuff(curCand) && checkCuts())
		    {
			fTree->Fill();
			fghasCand++; // obviously we have a candidate, so we want to mark this
		    }
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
        std::vector<CheckedCand> candVec, candVecFit;
        for (int iC = 0; iC < fpEvt->nCands(); ++iC)
        {
            pCand = fpEvt->getCand(iC);
            ((TH1D*)fpHistFile->Get("h3"))->Fill(pCand->fType);
            if (CUTCandidate == pCand->fType)
            {
                CheckedCand curCand = getCheckedCand(pCand);
                if (curCand.isOk) candVec.push_back(curCand);
            }
            if (CUTCandidateFit == pCand->fType)
            {
                CheckedCand curCand = getCheckedCand(pCand);
                if (curCand.isOk) candVecFit.push_back(curCand);
            }
        }
        std::vector<std::pair<CheckedCand, CheckedCand> > candPairVec;
        for(std::vector<CheckedCand>::iterator it1 = candVec.begin(); it1!=candVec.end(); it1++)
            for(std::vector<CheckedCand>::iterator it2 = candVecFit.begin(); it2!=candVecFit.end(); it2++)
            {
                if(compareCands( *it1,*it2 ))
                    candPairVec.push_back(std::make_pair((*it1),(*it2)));
            }
        ((TH1D*)fpHistFile->Get("h4"))->Fill(candVec.size());
        ((TH1D*)fpHistFile->Get("h5"))->Fill(candVecFit.size());
        ((TH1D*)fpHistFile->Get("h6"))->Fill(candPairVec.size());
        for(std::vector<std::pair<CheckedCand,CheckedCand> >::iterator it = candPairVec.begin();
                it != candPairVec.end(); it++)
        {
            if (doCandStuff((*it).first) && doCandFitStuff((*it).second))
	    {
		doTriggerMatching();
		if (fIsMC) doPseudoHLTstuff();
		if (checkCuts())
		{
		    fTree->Fill();
		    fghasCand++; // obviously we have a candidate, so we want to mark this
		}
	    }
        }
    }

    if (fIsMC && fIsSig) fGenTree->Fill();
    return;
}

// ----------------------------------------------------------------------
// the generic case
bool lifetimeReader::doCandStuff(const CheckedCand &clc)
{
    fnPV = fpEvt->nPV();

    const TAnaCand *tacCur = fpEvt->getCand(clc.bc);
    // ---------------------------------------------------------
    // First we get some handier objects for later calculations
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacRs = fpEvt->getCand(clc.rs);

    // get the muons, mu1 being the one with higher pt (leading muon)
    const bool swapMu = (fpEvt->getSigTrack(clc.mu1)->fPlab.Perp() < fpEvt->getSigTrack(clc.mu2)->fPlab.Perp());
    const TAnaTrack *tatSigMu1 = fpEvt->getSigTrack(swapMu ? clc.mu2 : clc.mu1);
    const TAnaTrack *tatSigMu2 = fpEvt->getSigTrack(swapMu ? clc.mu1 : clc.mu2);
    const TAnaTrack *tatRecMu1 = fpEvt->getRecTrack(tatSigMu1->fIndex);
    const TAnaTrack *tatRecMu2 = fpEvt->getRecTrack(tatSigMu2->fIndex);

    // require the muons to be of at least some quality
    fillMuHist((TH1D*)fpHistFile->Get("h20"), tatRecMu1->fMuID);
    fillMuHist((TH1D*)fpHistFile->Get("h21"), tatRecMu2->fMuID);
    if(tatRecMu1->fMuID==-1||(tatRecMu1->fMuID & CUTMuId1)!=CUTMuId1) return false;
    if(tatRecMu2->fMuID==-1||(tatRecMu2->fMuID & CUTMuId2)!=CUTMuId2) return false;
    fillMuHist((TH1D*)fpHistFile->Get("h22"), tatRecMu1->fMuID);
    fillMuHist((TH1D*)fpHistFile->Get("h23"), tatRecMu2->fMuID);

    // get the V0 daughters
    const bool swapHa = fForcePtHa1GTPtHa2 && (fpEvt->getSigTrack(clc.ha1)->fPlab.Perp() < fpEvt->getSigTrack(clc.ha2)->fPlab.Perp());
    const TAnaTrack *tatSigHa1 = fpEvt->getSigTrack(swapHa ? clc.ha2 : clc.ha1);
    const TAnaTrack *tatSigHa2 = fpEvt->getSigTrack(swapHa ? clc.ha1 : clc.ha2);
    const TAnaTrack *tatRecHa1 = fpEvt->getRecTrack(tatSigHa1->fIndex);
    const TAnaTrack *tatRecHa2 = fpEvt->getRecTrack(tatSigHa2->fIndex);

    // sigtracks
    TLorentzVector tlvSigMu1, tlvSigMu2;
    tlvSigMu1.SetVectM(tatSigMu1->fPlab, MMUON);
    tlvSigMu2.SetVectM(tatSigMu2->fPlab, MMUON);

    // ---------------------------------------------------------
    // now we do some calculations

    // particle hypotheses from sigtracks for discrimination of fake identified candidated
    TLorentzVector tlvHa1AsPr, tlvHa1AsPi, tlvHa2AsPi, tlvHa1AsK, tlvHa2AsK;
    tlvHa1AsPr.SetVectM(tatSigHa1->fPlab, MPROTON);
    tlvHa1AsPi.SetVectM(tatSigHa1->fPlab, MPION);
    tlvHa2AsPi.SetVectM(tatSigHa2->fPlab, MPION);
    tlvHa1AsK.SetVectM(tatSigHa1->fPlab, MKAON);
    tlvHa2AsK.SetVectM(tatSigHa2->fPlab, MKAON);

    const TLorentzVector tlvKsHypoth     = tlvHa1AsPi + tlvHa2AsPi;
    const TLorentzVector tlvL0Hypoth     = tlvHa1AsPr + tlvHa2AsPi;
    const TLorentzVector tlvKstar1Hypoth = tlvHa1AsK  + tlvHa2AsPi;
    const TLorentzVector tlvKstar2Hypoth = tlvHa1AsPi + tlvHa2AsK;
    const TLorentzVector tlvPhiHypoth    = tlvHa1AsK  + tlvHa2AsK;

    // ---------------------------------------------------------
    // now comes the boring part: filling of the reduced tree

    // candidate data from fit
    fmrs=tacRs->fMass;
    fptrs=tacRs->fPlab.Perp();
    fprs=tacRs->fPlab.Mag();
    fetars=tacRs->fPlab.Eta();
    fphirs=tacRs->fPlab.Phi();

    fmjp=tacJpsi->fMass;
    fmjptlv=(tlvSigMu1+tlvSigMu2).M();
    fptjp=tacJpsi->fPlab.Perp();
    fpjp=tacJpsi->fPlab.Mag();
    fetajp=tacJpsi->fPlab.Eta();
    fphijp=tacJpsi->fPlab.Phi();

    // reco tracks
    frptmu1=tatRecMu1->fPlab.Perp();
    frpmu1=tatRecMu1->fPlab.Mag();
    fretamu1=tatRecMu1->fPlab.Eta();
    frphimu1=tatRecMu1->fPlab.Phi();
    frqmu1=tatRecMu1->fQ;
    if(tatRecMu1->fMuID==-1) fridmu1=0;
    else fridmu1=tatRecMu1->fMuID;
    fchi2mu1=tatRecMu1->fChi2;
    fprobmu1=TMath::Prob(tatRecMu1->fChi2,tatRecMu1->fDof);
    fndofmu1=tatRecMu1->fDof;
    fqualmu1=tatRecMu1->fTrackQuality;
    falgomu1=tatRecMu1->fAlgorithm;
    fnpixmu1=numberOfPixLayers(tatRecMu1);

    frptmu2=tatRecMu2->fPlab.Perp();
    frpmu2=tatRecMu2->fPlab.Mag();
    fretamu2=tatRecMu2->fPlab.Eta();
    frphimu2=tatRecMu2->fPlab.Phi();
    frqmu2=tatRecMu2->fQ;
    if(tatRecMu2->fMuID==-1) fridmu2=0;
    else fridmu2=tatRecMu2->fMuID;
    fchi2mu2=tatRecMu2->fChi2;
    fprobmu2=TMath::Prob(tatRecMu2->fChi2,tatRecMu2->fDof);
    fndofmu2=tatRecMu2->fDof;
    fqualmu2=tatRecMu2->fTrackQuality;
    falgomu2=tatRecMu2->fAlgorithm;
    fnpixmu2=numberOfPixLayers(tatRecMu2);

    frptha1=tatRecHa1->fPlab.Perp();
    fretaha1=tatRecHa1->fPlab.Eta();
    frphiha1=tatRecHa1->fPlab.Phi();
    frqha1=tatRecHa1->fQ;
    fchi2ha1=tatRecHa1->fChi2;
    fprobha1=TMath::Prob(tatRecHa1->fChi2,tatRecHa1->fDof);
    fndofha1=tatRecHa1->fDof;
    fqualha1=tatRecHa1->fTrackQuality;
    falgoha1=tatRecHa1->fAlgorithm;
    fnpixha1=numberOfPixLayers(tatRecHa1);

    frptha2=tatRecHa2->fPlab.Perp();
    fretaha2=tatRecHa2->fPlab.Eta();
    frphiha2=tatRecHa2->fPlab.Phi();
    frqha2=tatRecHa2->fQ;
    fchi2ha2=tatRecHa2->fChi2;
    fprobha2=TMath::Prob(tatRecHa2->fChi2,tatRecHa2->fDof);
    fndofha2=tatRecHa2->fDof;
    fqualha2=tatRecHa2->fTrackQuality;
    falgoha2=tatRecHa2->fAlgorithm;
    fnpixha2=numberOfPixLayers(tatRecHa2);

    // momentum ordered by charge
    frphap = ( frqha1 > 0 ? tatRecHa1->fPlab.Mag() : tatRecHa2->fPlab.Mag() );
    frpham = ( frqha1 < 0 ? tatRecHa1->fPlab.Mag() : tatRecHa2->fPlab.Mag() );

    // muon quality selection
    fIsMuTight = isTightMuon(tatRecMu1) && isTightMuon(tatRecMu2);
    fIsMuTight2 = isTightMuonNoTipCut(tatRecMu1) && isTightMuonNoTipCut(tatRecMu2);
    fIsMuSoft = isSoftMuon(tatRecMu1) && isSoftMuon(tatRecMu2);
    fIsCowboy = isCowboy(tatRecMu1, tatRecMu2);

    // qualities of vertex fits
    fchi2rs=tacRs->fVtx.fChi2;
    fchi2jp=tacJpsi->fVtx.fChi2;
    fndofrs=tacRs->fVtx.fNdof;
    fndofjp=tacJpsi->fVtx.fNdof;
    fprobrs=TMath::Prob(fchi2rs,fndofrs);
    fprobjp=TMath::Prob(fchi2jp,fndofjp);

    // doca
    fmaxdocabc=tacCur->fMaxDoca;
    fmaxdocars=tacRs->fMaxDoca;
    fmaxdocajp=tacJpsi->fMaxDoca;

    // dR and angles
    fdRhaha=tatRecHa1->fPlab.DeltaR(tatRecHa2->fPlab);
    fdRmumu=tatRecMu1->fPlab.DeltaR(tatRecMu2->fPlab);

    // particle hypothesis for the "resonances"
    fKshypo=tlvKsHypoth.M();
    fL0hypo=tlvL0Hypoth.M();
    fKstarhypo1=tlvKstar1Hypoth.M();
    fKstarhypo2=tlvKstar2Hypoth.M();
    fPhihypo=tlvPhiHypoth.M();

    return true;
}

// ----------------------------------------------------------------------
bool lifetimeReader::doCandFitStuff(const CheckedCand &clc)
{
    const TAnaCand *tacCur = fpEvt->getCand(clc.bc);
    // ---------------------------------------------------------
    // First we get some handier objects for later calculations
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacRs = fpEvt->getCand(clc.rs);

    // get the muons
    const TAnaTrack *tatSigMu1 = fpEvt->getSigTrack(clc.mu1);
    const TAnaTrack *tatSigMu2 = fpEvt->getSigTrack(clc.mu2);
    const TAnaTrack *tatRecMu1 = fpEvt->getRecTrack(tatSigMu1->fIndex);
    const TAnaTrack *tatRecMu2 = fpEvt->getRecTrack(tatSigMu2->fIndex);

    // require the muons to be of at least some quality
    if(tatRecMu1->fMuID==-1||(tatRecMu1->fMuID & CUTMuId1)!=CUTMuId1) return false;
    if(tatRecMu2->fMuID==-1||(tatRecMu2->fMuID & CUTMuId2)!=CUTMuId2) return false;

    // get the resonances' daughters
    const TAnaTrack *tatSigHa1 = fpEvt->getSigTrack(clc.ha1);
    const TAnaTrack *tatSigHa2 = fpEvt->getSigTrack(clc.ha2);
    const TAnaTrack *tatRecHa1 = fpEvt->getRecTrack(tatSigHa1->fIndex);
    const TAnaTrack *tatRecHa2 = fpEvt->getRecTrack(tatSigHa2->fIndex);

    // ---------------------------------------------------------
    // now we do some calculations

    // calculate alpha (pointing angles in rads)
    const TVector3 vecJpsiRs = tacRs->fVtx.fPoint - tacJpsi->fVtx.fPoint;
    const TVector3 vecRsBc = tacRs->fVtx.fPoint - tacCur->fVtx.fPoint;
    const TVector3 vecPV = fpEvt->getPV(tacCur->fPvIdx)->fPoint;
    const TVector3 vecPVbc = tacCur->fVtx.fPoint - vecPV;

    //fghasCand++; // obviously we have a candidate, so we want to mark this

    // ---------------------------------------------------------
    // now comes the boring part: filling of the reduced tree

    // candidate data from fit
    fmbc=tacCur->fMass;
    fptbc=tacCur->fPlab.Perp();
    fpbc=tacCur->fPlab.Mag();
    fetabc=tacCur->fPlab.Eta();
    fphibc=tacCur->fPlab.Phi();

    // refitted tracks, "signal" tracks
    fSptmu1=tatSigMu1->fPlab.Perp();
    fSpmu1=tatSigMu1->fPlab.Mag();
    fSetamu1=tatSigMu1->fPlab.Eta();
    fSphimu1=tatSigMu1->fPlab.Phi();

    fSptmu2=tatSigMu2->fPlab.Perp();
    fSpmu2=tatSigMu2->fPlab.Mag();
    fSetamu2=tatSigMu2->fPlab.Eta();
    fSphimu2=tatSigMu2->fPlab.Phi();

    fSptha1=tatSigHa1->fPlab.Perp();
    fSetaha1=tatSigHa1->fPlab.Eta();
    fSphiha1=tatSigHa1->fPlab.Phi();

    fSptha2=tatSigHa2->fPlab.Perp();
    fSetaha2=tatSigHa2->fPlab.Eta();
    fSphiha2=tatSigHa2->fPlab.Phi();

    // flight lengths and their errors
    fd3bc=tacCur->fVtx.fD3d;
    fd3rs=tacRs->fVtx.fD3d;
    fd3jp=tacJpsi->fVtx.fD3d;
    fd3Ebc=tacCur->fVtx.fD3dE;
    fd3Ers=tacRs->fVtx.fD3dE;
    fd3Ejp=tacJpsi->fVtx.fD3dE;

    fdxybc=tacCur->fVtx.fDxy;
    fdxyrs=tacRs->fVtx.fDxy;
    fdxyjp=tacJpsi->fVtx.fDxy;
    fdxyEbc=tacCur->fVtx.fDxyE;
    fdxyErs=tacRs->fVtx.fDxyE;
    fdxyEjp=tacJpsi->fVtx.fDxyE;

    // qualities of vertex fits
    fchi2bc=tacCur->fVtx.fChi2;
    fndofbc=tacCur->fVtx.fNdof;
    fprobbc=TMath::Prob(fchi2bc,fndofbc);

    // ctau - written as dist/p*m as this is numerically more stable than using
    // the TLorentzVector::Gamma() functions to write beta*gamma
    fct3dbc = tacCur->fTau3d; // tacCur->fVtx.fD3d / tacCur->fPlab.Mag() * MLAMBDA_B;
    fct3dbcE = tacCur->fTau3dE; // tacCur->fVtx.fD3dE / tacCur->fPlab.Mag() * MLAMBDA_B; // Ok, error not yet final. Will need error of |p|
    fct3drs = tacRs->fTau3d; // tacRs->fVtx.fD3d / tacRs->fPlab.Mag() * MLAMBDA_0;
    fct3drsE = tacRs->fTau3dE; // tacRs->fVtx.fD3dE / tacRs->fPlab.Mag() * MLAMBDA_0;
    fct3dbc_jp = tacCur->fTau3d / MJPSI * MLAMBDA_B; // tacCur->fVtx.fD3d / tacCur->fPlab.Mag() * MLAMBDA_B;
    fctxybc = tacCur->fTauxy;
    fctxybcE = tacCur->fTauxyE;
    fctxyrs = tacRs->fTauxy;
    fctxyrsE = tacRs->fTauxyE;

    // vertex positions
    fvxbc = tacCur->fVtx.fPoint.x();
    fvybc = tacCur->fVtx.fPoint.y();
    fvzbc = tacCur->fVtx.fPoint.z();
    fvrbc = tacCur->fVtx.fPoint.XYvector().Mod();
    fvrbcPV = (tacCur->fVtx.fPoint-vecPV).XYvector().Mod();
    fvxrs = tacRs->fVtx.fPoint.x();
    fvyrs = tacRs->fVtx.fPoint.y();
    fvzrs = tacRs->fVtx.fPoint.z();
    fvrrs = tacRs->fVtx.fPoint.XYvector().Mod();
    fvrrsPV = (tacRs->fVtx.fPoint-vecPV).XYvector().Mod();

    // longitudinal I.P. w.r.t best and 2nd best PV
    fPvLip = tacCur->fPvLip;
    fPvLipE = tacCur->fPvLipE;
    fPvLip2 = tacCur->fPvLip2;
    fPvLipE2 = tacCur->fPvLipE2;

    // other cut values
    // pointing angles
    falphabc = vecPVbc.Angle(tacCur->fPlab);
    falphars = vecJpsiRs.Angle(tacRs->fPlab);
    falpharsbc = vecRsBc.Angle(tacRs->fPlab);
    fptgangDRrs = vecJpsiRs.DeltaR(tacRs->fPlab);
    fptgangDRbc = vecPVbc.DeltaR(tacCur->fPlab);

    // dR and angles
    fdRrsjp=tacRs->fPlab.DeltaR(tacCur->fPlab);
    fangbcrs=tacRs->fPlab.Angle(tacCur->fPlab);

    // ratio of impact parameter with track angle (exp.val. approx. 1)
    fiprmu1 = fd3bc*sin(tatRecMu1->fPlab.Angle(vecPVbc))/sqrt(tatRecMu1->fLip*tatRecMu1->fLip+tatRecMu1->fTip*tatRecMu1->fTip);
    fiprmu2 = fd3bc*sin(tatRecMu2->fPlab.Angle(vecPVbc))/sqrt(tatRecMu2->fLip*tatRecMu2->fLip+tatRecMu2->fTip*tatRecMu2->fTip);
    fiprha1 = fd3rs*sin(tatRecHa1->fPlab.Angle(vecJpsiRs))/sqrt(tatRecHa1->fLip*tatRecHa1->fLip+tatRecHa1->fTip*tatRecHa1->fTip);
    fiprha2 = fd3rs*sin(tatRecHa2->fPlab.Angle(vecJpsiRs))/sqrt(tatRecHa2->fLip*tatRecHa2->fLip+tatRecHa2->fTip*tatRecHa2->fTip);

    // Armenteros variables
    {
	const TVector3 p1 = tatRecHa1->fQ > 0 ? tatRecHa1->fPlab : tatRecHa2->fPlab;
	const TVector3 p2 = tatRecHa1->fQ > 0 ? tatRecHa2->fPlab : tatRecHa1->fPlab;
	const TVector3 p = p1+p2;
	farmQt = (p1.Cross(p2)).Mag()/p.Mag();
	farmAl = (p1.Mag2()-p2.Mag2())/p.Mag2();
    }

    // Candidate isolation
    fIsoClosetrk = nCloseTracks(tacCur, CUTisoCloseTrkDoca, CUTisoCloseTrkPt);
    fIsoDocaTrack = isoDocaTrack(tacCur);

    // Do truth matching
    if (fIsMC)
    {
	fIsMCmatch = doTruthMatching(tatSigMu1, tatSigMu2, tatSigHa1, tatSigHa2, tacRs->fVtx.fPoint);
    }

    return true;
}

// ----------------------------------------------------------------------
int lifetimeReader::getSigId(const TAnaCand *tac, const int id, int pos)
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
int lifetimeReader::getCandId(const TAnaCand *tac, const int id, int pos)
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
bool lifetimeReader::compareCands4prong(const CheckedCand clc1, const CheckedCand clc2)
{
    const int clc1mu1 = fpEvt->getSigTrack(clc1.mu1)->fIndex;
    const int clc1mu2 = fpEvt->getSigTrack(clc1.mu2)->fIndex;
    const int clc1ha1 = fpEvt->getSigTrack(clc1.ha1)->fIndex;
    const int clc1ha2 = fpEvt->getSigTrack(clc1.ha2)->fIndex;

    const int clc2mu1 = fpEvt->getSigTrack(clc2.mu1)->fIndex;
    const int clc2mu2 = fpEvt->getSigTrack(clc2.mu2)->fIndex;
    const int clc2ha1 = fpEvt->getSigTrack(clc2.ha1)->fIndex;
    const int clc2ha2 = fpEvt->getSigTrack(clc2.ha2)->fIndex;

    return ((clc1mu1 == clc2mu1)
            && (clc1mu2 == clc2mu2)
            && (clc1ha1 == clc2ha1)
            && (clc1ha2 == clc2ha2));
}

// ----------------------------------------------------------------------
// check if two candidates point to the same two sig tracks for each daughter
bool lifetimeReader::compareCands3prong(const CheckedCand clc1, const CheckedCand clc2)
{
    const int clc1mu1 = fpEvt->getSigTrack(clc1.mu1)->fIndex;
    const int clc1mu2 = fpEvt->getSigTrack(clc1.mu2)->fIndex;
    const int clc1ha1 = fpEvt->getSigTrack(clc1.ha1)->fIndex;

    const int clc2mu1 = fpEvt->getSigTrack(clc2.mu1)->fIndex;
    const int clc2mu2 = fpEvt->getSigTrack(clc2.mu2)->fIndex;
    const int clc2ha1 = fpEvt->getSigTrack(clc2.ha1)->fIndex;

    return ((clc1mu1 == clc2mu1)
            && (clc1mu2 == clc2mu2)
            && (clc1ha1 == clc2ha1));
}

// ----------------------------------------------------------------------
void lifetimeReader::doHLTstuff()
{
    bool HLTdim10_v3(false), HLTdim10_v5(false), HLTdim10_v6(false);

    for(int i=0; i!=NHLT; i++)
    {
	const std::string name = fpEvt->fHLTNames[i].Data();
	if (name == "") break; // if name is empty we reached the end of the list (according to Urs)
	const double prescale = fpEvt->fHLTPrescale[i];
	const bool wasrun = fpEvt->fHLTWasRun[i];
	const bool result = fpEvt->fHLTResult[i];
	const bool error = fpEvt->fHLTError[i];
	//cout << name << " - wasrun: " << wasrun << " prescale: " << prescale << " result: " << result << " error: " << error << endl;
	if (result && wasrun && !error)
	{
	    // Inclusive Jpsi triggers
	    if (name.find("HLT_DoubleMu3_Jpsi_v")           !=std::string::npos) { fHLTDMu3jp      = true; fHLTpreDMu3jp      = prescale; }
	    if (name.find("HLT_Dimuon6p5_Jpsi_v")           !=std::string::npos) { fHLTDMu6p5Jp    = true; fHLTpreDMu6p5Jp    = prescale; }
	    if (name.find("HLT_Dimuon0_Jpsi_v")             !=std::string::npos) { fHLTDMu0Jp      = true; fHLTpreDMu0Jp      = prescale; }
	    if (name.find("HLT_Dimuon0_Jpsi_NoVertexing_v") !=std::string::npos) { fHLTDMu0JpNoVtx = true; fHLTpreDMu0JpNoVtx = prescale; }

	    // Inclusive Jpsi triggers in barrel
	    if (name.find("HLT_Dimuon6p5_Barrel_Jpsi")      !=std::string::npos) { fHLTDMu6p5BarJp = true; fHLTpreDMu6p5BarJp = prescale; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel")       !=std::string::npos) { fHLTDMu10BarJp  = true; fHLTpreDMu10BarJp = prescale; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v1")    !=std::string::npos) { HLTdim10_v3 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v2")    !=std::string::npos) { HLTdim10_v3 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v3")    !=std::string::npos) { HLTdim10_v3 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v5")    !=std::string::npos) { HLTdim10_v5 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v6")    !=std::string::npos) { HLTdim10_v6 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v9")    !=std::string::npos) { HLTdim10_v6 = true; }
	    if (name.find("HLT_Dimuon10_Jpsi_Barrel_v10")   !=std::string::npos) { HLTdim10_v6 = true; }
	    if (name.find("HLT_Dimuon13_Jpsi_Barrel")       !=std::string::npos) { fHLTDMu13BarJp  = true; fHLTpreDMu13BarJp = prescale; }

	    // Displaced Jpsi triggers
	    if (name.find("HLT_Dimuon6p5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu6p5JpDis = true; fHLTpreDMu6p5JpDis = prescale; }
	    if (name.find("HLT_Dimuon7_Jpsi_Displaced")     !=std::string::npos) { fHLTDMu7JpDis   = true; fHLTpreDMu7JpDis = prescale; }
	    if (name.find("HLT_Dimuon3p5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu3p5JpDis = true; fHLTpreDMu3p5JpDis = prescale; }
	    if (name.find("HLT_DoubleMu3p5_Jpsi_Displaced") !=std::string::npos) { fHLTDMu3p5JpDis = true; fHLTpreDMu3p5JpDis = prescale; } // which spelling is correct?
	    if (name.find("HLT_DoubleMu4_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu4JpDis   = true; fHLTpreDMu4JpDis = prescale; }
	    if (name.find("HLT_DoubleMu5_Jpsi_Displaced")   !=std::string::npos) { fHLTDMu5JpDis   = true; fHLTpreDMu5JpDis = prescale; }


	    // legacy trigger information, for backwards compatibility with 2010 only.
	    if (name.find("Quarkonium")                     !=std::string::npos) { fHLTqrk = true; fHLTpreqrk = prescale; }
	    // DoubleMu triggers (usually prescaled)
	    if ("HLT_DoubleMu0"          == name) fHLTDMu0 = true;
	    // Jpsi triggers
	    if ("HLT_Mu0_TkMu0_Jpsi"     == name) fHLTMu0TkMu0jp = true;
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
	}
	fHLTokJpsi = (fHLTDMu3jp || fHLTDMu6p5Jp || fHLTDMu0Jp || fHLTDMu0JpNoVtx);
	fHLTokBarrelJpsi = (fHLTDMu6p5BarJp || fHLTDMu10BarJp || fHLTDMu13BarJp);
	fHLTokDisplJpsi = (fHLTDMu3p5JpDis || fHLTDMu6p5JpDis || fHLTDMu7JpDis || fHLTDMu4JpDis || fHLTDMu5JpDis);
	// a weighted version for MC
	fHLTokBarrelJpsiMC = (checkRan(HLTdim10_v3, .29) || checkRan(HLTdim10_v5, .29) || checkRan(HLTdim10_v6, .88) || fHLTDMu13BarJp);
	fHLTokDisplJpsiMC = (checkRan(fHLTDMu3p5JpDis,.28) || checkRan(fHLTDMu7JpDis,.29) || fHLTDMu4JpDis || fHLTDMu5JpDis);
	// --- and here the big OR for SingleMu
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

// ----------------------------------------------------------------------
void lifetimeReader::doPseudoHLTstuff()
{   // Pseudo HLT decision for MC without proper HLT simulation. We do this for triggers from Run2011A_v4 upwards
    // see journal 14 p. 6 for thresholds justification
    fHLTokBarrelJpsiMCpseudo = fHLTokDisplJpsiMCpseudo = false;
    // emulate barrel trigger
    if (fptjp > 10)
    {
	if (TMath::Abs(fretamu1) < 1.25 && TMath::Abs(fretamu2) < 1.25)
	{
	    fHLTokBarrelJpsiMCpseudo = true;
	    // weighting due to less lumi covered by HLT_Dimuon13_Jpsi_Barrel
	    if (fptjp > 13 && checkRan(.69)) fHLTokBarrelJpsiMCpseudo = true;
	}
    }
    // emulate displaced trigger
    if (falphabc < 0.45 && fd3bc/fd3Ebc > 3 && fmaxdocajp < .5) // common to all
    {
	bool dim7, dbl35, dbl4, dbl5;
	if (fptjp > 6.9 && fchi2jp < 10) { dim7 = true; } // HLT_Dimuon6p5_Jpsi_Displaced
	if (TMath::Abs(fretamu1) < 2.2 && TMath::Abs(fretamu2) < 2.2)
	{   // our probjp is tighter as it takes a V0 into account, so loser cuts
	    if (frptmu1 > 3.5 && frptmu2 > 3.5 && fprobjp > .15) dbl35 = true; // HLT_DoubleMu3p5_Jpsi_Displaced
	    if (frptmu1 > 4.0 && frptmu2 > 4.0 && fprobjp > .15) dbl4 = true; // HLT_DoubleMu4_Jpsi_Displaced
	    if (frptmu1 > 5.0 && frptmu2 > 5.0 && fprobjp > .15) dbl5 = true; // HLT_DoubleMu5_Jpsi_Displaced
	}
	// final decision
	fHLTokDisplJpsiMCpseudo = (checkRan(dim7, .29) || checkRan(dbl35, .28) || checkRan(dbl4, .99) || dbl5);
    }
}

// ----------------------------------------------------------------------
// a simple trigger matcher based on deltaR
void lifetimeReader::doTriggerMatching()
{
    const double deltaRthrsh(0.5); // threshold indpired by MuonAnalysis/MuonAssociators/python/patMuonsWithTrigger_cff.py
    int mu1match(-1), mu2match(-1);
    double deltaRminMu1(100), deltaRminMu2(100);
    TTrgObj *tto;
    TLorentzVector tlvMu1, tlvMu2;
    tlvMu1.SetPtEtaPhiM(frptmu1,fretamu1,frphimu1,MMUON);
    tlvMu2.SetPtEtaPhiM(frptmu2,fretamu2,frphimu2,MMUON);
    if (fVerbose > 5)
    {
	cout << "dump trigger objects ----------------------" << endl;
	cout << "mu1: pt,eta,phi: " << frptmu1 << " " << fretamu1 << " " << frphimu1 << " q: " << frqmu1 << endl;
	cout << "mu2: pt,eta,phi: " << frptmu2 << " " << fretamu2 << " " << frphimu2 << " q: " << frqmu2 << endl;
	cout << "HLT: fHLTMu0TkMu0jp: " << fHLTMu0TkMu0jp << " fHLTMu0jp: " << fHLTMu0jp
	     << endl;
    }

    //cout << "-----" << endl;
    for(int i=0; i!=fpEvt->nTrgObj(); i++)
    {
	tto = fpEvt->getTrgObj(i);
	if (fVerbose > 5) cout << "i: " << i << " ";
	if (fVerbose > 5) tto->dump();
	if (fVerbose > 4) cout << fRun << " " << fEvt << ": " << tto->fLabel << tto->fP.DeltaR(tlvMu1) << ":" << tto->fP.DeltaR(tlvMu2) << endl;
	if (doTriggerLabelSet) triggerlabels.insert(tto->fLabel.Data());
	// the label changes according to datataking era, so we need to distinguish them
	if ( (fRun <  160405 && tto->fLabel == "hltMu0TkMuJpsiTrackMassFiltered:HLT::") // legacy for 2010 data
	  || (fIsMC && (tto->fLabel.BeginsWith("hltDisplacedmumuFilter") || tto->fLabel.BeginsWith("hltVertexmumuFilterDimuon"))) // MC selection needs adjustment according to sample used
	  || (fRun >= 160405 && fRun < 163270 && tto->fLabel == "hltDoubleMu3QuarkoniumL3Filtered:HLT::") // First 2011 data
	  || (fRun >= 163270 && fRun < 164237 && (tto->fLabel == "hltDimuon6p5JpsiDisplacedL3Filtered:HLT::" || tto->fLabel == "hltDimuon6p5JpsiL3Filtered:HLT::" || tto->fLabel == "hltDimuon6p5BarrelJpsiL3Filtered:HLT::"))
	  || (fRun >= 164924 && (tto->fLabel.BeginsWith("hltDisplacedmumuFilter") || tto->fLabel.BeginsWith("hltVertexmumuFilterDimuon")
	      || tto->fLabel.BeginsWith("hltVertexmumuFilterJpsiBarrel"))) ) // major part of 2011 data
	{
	    const double deltaR1 = tto->fP.DeltaR(tlvMu1);
	    const double deltaR2 = tto->fP.DeltaR(tlvMu2);
	    if (fVerbose > 5) cout << fRun << " " << fEvt << ": " << tto->fLabel << deltaR1 << ":" << deltaR2 << endl;
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
// Courtesy of Urs in HeavyFlavorAnalysis/Bs2MuMu/macros2/candAna.cc rev. 1.30
double lifetimeReader::isoDocaTrack(const TAnaCand *pC)
{
    double ret(99.);
    if (pC->fNstTracks.size() != 0) ret = pC->fNstTracks[0].second.first;

    int nsize(pC->fNstTracks.size());
    TAnaTrack *tat;
    for (int i = 0; i<nsize; ++i)
    {
	const int trkId = pC->fNstTracks[i].first;
	tat = fpEvt->getRecTrack(trkId);
	// -- check that any track associated with a definitive vertex is from the same or the closest (compatible) other PV
	const int pvidx = (pC->fPvIdx > -1? pC->fPvIdx : 0);
	if ((tat->fPvIdx > -1) && (tat->fPvIdx != pvidx)) continue;

	ret = pC->fNstTracks[i].second.first;
	break;
    }
    return ret;
}

// ----------------------------------------------------------------------
// Courtesy of Urs in HeavyFlavorAnalysis/Bs2MuMu/macros2/candAna.cc rev. 1.30
int lifetimeReader::nCloseTracks(const TAnaCand *pC, double dcaCut, double ptCut)
{
    int cnt(0);
    const int nsize = pC->fNstTracks.size();
    const int pvIdx = pC->fPvIdx;
    int pvIdx2= nearestPV(pvIdx, 0.1);
    const double candPvLipS2  = pC->fPvLip2/pC->fPvLipE2;
    if (TMath::Abs(candPvLipS2) > 2) pvIdx2 = -1;

    TAnaTrack *tat;
    for (int i = 0; i<nsize; ++i)
    {
	const int trkId = pC->fNstTracks[i].first;
	const double doca = pC->fNstTracks[i].second.first;

	if (doca > dcaCut) continue; // check the doca cut

	// -- check that any track associated with a definitive vertex is from the same or the closest (compatible) other PV
	tat = fpEvt->getRecTrack(trkId);
	if ((tat->fPvIdx > -1) && (tat->fPvIdx != pvIdx)) continue;
	if (tat->fPlab.Perp() < ptCut) continue;

	++cnt;
    }
    return cnt;
}

// ----------------------------------------------------------------------
// Courtesy of Urs in HeavyFlavorAnalysis/Bs2MuMu/macros2/candAna.cc rev. 1.30
int lifetimeReader::nearestPV(int pvIdx, double maxDist)
{
    TAnaVertex *v0 = fpEvt->getPV(pvIdx);

    double zV0 = v0->fPoint.Z();

    int idx(-1);
    double z(0.), zabs(0.), zmin(99.);
    for (int i = 0; i < fpEvt->nPV(); ++i)
    {
	if (i == pvIdx) continue;
	z = fpEvt->getPV(i)->fPoint.Z();
	zabs = TMath::Abs(zV0 - z);
	if (zabs < zmin)
	{
	    idx = i;
	    zmin = zabs;
	}
    }

    return (zmin < maxDist ? idx : -1);
}

// ----------------------------------------------------------------------
// Courtesy of Urs in HeavyFlavorAnalysis/Bs2MuMu/macros2/candAna.cc rev. 1.30
bool lifetimeReader::isTightMuon(const TAnaTrack *pT)
{
    // check mu flags
    const bool muflag = ((pT->fMuID & 80) == 80); // TrackerMuonArbitrated && GlobalMuonPromptTrigger

    // check no. of matched muon stations
    bool mucuts(false);
    if (pT->fMuIndex > -1)
    {
	TAnaMuon *pM = fpEvt->getMuon(pT->fMuIndex);
	if (pM->fTimeNdof > 1) mucuts = true; // matched muon stations (misuse of fTimeNdof)
    }

    // other checks
    bool trackcuts(true);
    if (TMath::Abs(pT->fBsTip) > 0.2) trackcuts = false;
    if (numberOfPixLayers(pT) < 1) trackcuts = false;
    if (pT->fValidHits < 11) trackcuts = false;

    return (muflag && mucuts && trackcuts);
}

bool lifetimeReader::isTightMuonNoTipCut(const TAnaTrack *pT)
{
    // check mu flags
    const bool muflag = ((pT->fMuID & 80) == 80); // TrackerMuonArbitrated && GlobalMuonPromptTrigger

    // check no. of matched muon stations
    bool mucuts(false);
    if (pT->fMuIndex > -1)
    {
	TAnaMuon *pM = fpEvt->getMuon(pT->fMuIndex);
	if (pM->fTimeNdof > 1) mucuts = true; // matched muon stations (misuse of fTimeNdof)
    }

    // other checks
    bool trackcuts(true);
    //if (TMath::Abs(pT->fBsTip) > 0.2) trackcuts = false; // This is removed from the cuts
    if (numberOfPixLayers(pT) < 1) trackcuts = false;
    if (pT->fValidHits < 11) trackcuts = false;

    return (muflag && mucuts && trackcuts);
}

// ----------------------------------------------------------------------
bool lifetimeReader::isSoftMuon(const TAnaTrack *pT)
{
    // check mu flags
    const bool muflag = ((pT->fMuID & 4096) == 4096); // TMOneStationTight

    // other checks
    bool trackcuts(true);
    if (TMath::Abs(pT->fBsTip) > 3.0) trackcuts = false;
    if (TMath::Abs(pT->fBsLip) > 30.0) trackcuts = false;
    if (numberOfPixLayers(pT) < 1) trackcuts = false;
    if (pT->fValidHits < 11) trackcuts = false;
    if (pT->fChi2/pT->fDof > 1.8) trackcuts = false;

    return (muflag && trackcuts);
}

// ----------------------------------------------------------------------
// inspired by HLTrigger/Muon/src/HLTMuonDimuonL3Filter.cc#334
bool lifetimeReader::isCowboy(const TAnaTrack *tr1, const TAnaTrack *tr2)
{
    return (tr1->fQ * deltaPhi(tr1->fPlab.Phi(), tr2->fPlab.Phi()) > 0.0);
}

// ----------------------------------------------------------------------
// from Bs2MuMu/macros/treeReader01.cc
int lifetimeReader::numberOfPixLayers(const TAnaTrack *pTrack)
{
    int hits = pTrack->fValidHits;
    if (hits>20) hits=20; // pattern has only 20 locations
    int pixHits=0;
    for(int i =0; i<hits; ++i)
    {
	uint32_t pat = pTrack->fHitPattern[i];
	if( pat == 0x488 ) pixHits++; // layer1 = true;
	else if( pat == 0x490 ) pixHits++; // layer2 = true;
	else if( pat == 0x498 ) pixHits++; // layer3 = true;
	else if( pat == 0x508 ) pixHits++; // disk1 = true;
	else if( pat == 0x510 ) pixHits++; // disk2 = true;
    }

    return pixHits;
}

// ----------------------------------------------------------------------
// a recursive retriever oscillating particles to get an earlier oscillation
// eg. for B0, as B0 may come from another B0 due to oscillation
TVector3 lifetimeReader::getProdVtx(TGenCand* gc, int pid)
{
    TVector3 ret = gc->fV;
    for(int gcMomIt=gc->fMom1; gcMomIt<=gc->fMom2; gcMomIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcMomIt);
	if (TMath::Abs(gcCur->fID) == pid) ret = getProdVtx(gcCur, pid);
    }
    return ret;
}

// ----------------------------------------------------------------------
void lifetimeReader::initVariables()
{
    // init observables
    fmbc = fmrs = fmjp = 9999;
    fnPV = 0;
    fmjptlv = 9999;
    fptbc = fptrs = fptjp = 9999;
    fpbc = fprs = fpjp = 9999;
    fetabc = fetars = fetajp = 9999;
    fphibc = fphirs = fphijp = 9999;
    fybc = 9999;
    fptgenbc = fmgenbc = fphigenbc = fetagenbc = fygenbc = 9999;

    frptmu1 = frptmu2 = frptha1 = frptha2 = 9999;
    frpmu1 = frpmu2 = frphap = frpham = 9999;
    fretamu1 = fretamu2 = fretaha1 = fretaha2 = 9999;
    frphimu1 = frphimu2 = frphiha1 = frphiha2 = 9999;

    frqmu1 = frqmu2 = frqha1 = frqha2 = 9999;
    fridmu1 = fridmu2 = 9999;

    fSptmu1 = fSptmu2 = fSptha1 = fSptha2 = 9999;
    fSpmu1 = fSpmu2 = 9999;
    fSetamu1 = fSetamu2 = fSetaha1 = fSetaha2 = 9999;
    fSphimu1 = fSphimu2 = fSphiha1 = fSphiha2 = 9999;

    fKshypo = fL0hypo = fKstarhypo1 = fKstarhypo2 = fPhihypo = 9999;

    falphabc = falphars = falpharsbc = 9999;
    fptgangDRbc = fptgangDRrs = 9999;
    fmaxdocabc = fmaxdocars = fmaxdocajp = 9999;

    fPvLip = fPvLipE = fPvLip2 = fPvLipE2 = 9999;

    fd3bc = fd3rs = fd3jp = 9999;
    fd3Ebc = fd3Ers = fd3Ejp = 9999;
    fct3dbc = fct3dbcE = fct3drs = fct3drsE = 9999;
    fct3dbc_jp = 9999;
    fctbctruth = fd3dbctruth = fdxybctruth = fpbctruth = fptbctruth = 9999;
    fctxybc = fctxybcE = fctxyrs = fctxyrsE = 9999;
    fbtbcx = fbtbcy = fbtbcz = 9999;
    fbtrsx = fbtrsy = fbtrsz = 9999;
    fvxrs = fvyrs = fvzrs = fvrrs = fvrrsPV = 9999;
    fvxbc = fvybc = fvzbc = fvrbc = fvrbcPV = 9999;

    fdxybc = fdxyrs = fdxyjp = 9999;
    fdxyEbc = fdxyErs = fdxyEjp = 9999;

    fchi2bc = fchi2rs = fchi2jp = 9999;
    fndofbc = fndofrs = fndofjp = 9999;
    fprobbc = fprobrs = fprobjp = 9999;

    fchi2mu1 = fchi2mu2 = fchi2ha1 = fchi2ha2 = 9999;
    fprobmu1 = fprobmu2 = fprobha1 = fprobha2 = 9999;
    fndofmu1 = fndofmu2 = fndofha1 = fndofha2 = 9999;
    fqualmu1 = fqualmu2 = fqualha1 = fqualha2 = 9999;
    falgomu1 = falgomu2 = falgoha1 = falgoha2 = 9999;
    fnpixmu1 = fnpixmu2 = fnpixha1 = fnpixha2 = 9999;

    fIsMuTight = fIsMuTight2 = fIsMuSoft = false;
    fIsCowboy = false;

    fiprmu1 = fiprmu2 = fiprha1 = fiprha2 = 9999;
    farmQt = farmAl = 9999;

    fdRhaha = fdRmumu = fdRrsjp = 9999;

    fangbcrs = 9999;

    fIsoClosetrk = 9999;
    fIsoDocaTrack = 9999;

    fnDaughters = fnGrandDaughters = 9999;
    fmapRef1Gen = fmapRef2Gen = 9999;
    fIsSig = 0;
    fIsMCmatch = false;

    // for trigger stuff
    // first decisions
    fHLTDMu3jp = fHLTDMu6p5Jp = fHLTDMu0Jp = fHLTDMu0JpNoVtx = false;
    fHLTDMu6p5BarJp = fHLTDMu10BarJp = fHLTDMu13BarJp = false;
    fHLTDMu3p5JpDis = fHLTDMu6p5JpDis = fHLTDMu7JpDis = fHLTDMu4JpDis = fHLTDMu5JpDis = false;
    fHLTqrk = false;
    fHLTDMu0 = fHLTMu0TkMu0jp = fHLTMu0jp = fHLTMu5L2Mu2Jpsi = fHLTMu5Tr2Jpsi = fHLTMu5Tr7Jpsi = false;
    fHLTSingleMu = false;
    fHLTSingleIsoMu = fHLTSingleL1Mu = fHLTSingleL2Mu = fHLTSingleHLTMu = false;
    fHLTokJpsi = fHLTokBarrelJpsi = fHLTokDisplJpsi = false;
    fHLTokBarrelJpsiMC = fHLTokDisplJpsiMC = fHLTokBarrelJpsiMCpseudo = fHLTokDisplJpsiMCpseudo = false;

    // second prescales
    fHLTpreDMu3jp = fHLTpreDMu6p5Jp = fHLTpreDMu0Jp = fHLTpreDMu0JpNoVtx = 0;
    fHLTpreDMu6p5BarJp = fHLTpreDMu10BarJp = fHLTpreDMu13BarJp = 0;
    fHLTpreDMu3p5JpDis = fHLTpreDMu6p5JpDis = fHLTpreDMu7JpDis = fHLTpreDMu4JpDis = fHLTpreDMu5JpDis = 0;
    fHLTpreqrk = 0;

    // trigger matcher result
    fHLTmatch = false;

    // gen stuff
    fmapRef1Gen = fgmapRef1Gen = fmapRef2Gen = fgmapRef2Gen = -1;
    // reset tree variables
    fgmbc = fgmbcsw = fgmrs = fgmrssw = 9999;
    fgqha1 = fgqha2 = 9999;
    fgptha1 = fgptha2 = fgptmu1 = fgptmu2 = 9999;
    fgetaha1 = fgetaha2 = fgetamu1 = fgetamu2 = 9999;
    fgphiha1 = fgphiha2 = fgphimu1 = fgphimu2 = 9999;
    fgpmu1 = fgpmu2 = fgpha1 = fgpha2 = 9999;
    fgphap = fgpham = 9999;
    fgptrs = fgprs = 9999;
    fgdRhaha = fgdRmumu = fgdRrsbc = 9999;
    fganhaha = fganmumu = fganrsjp = 9999;
    fgctbc = 9999;
    fgnDaughters = fgnGrandDaughters = fgmapRef1Gen = -1;

    // old stuff
    fHLTmatch = false;
    fHLTokBarrelJpsiMCpseudo = fHLTokDisplJpsiMCpseudo = false;
}

// ----------------------------------------------------------------------
void lifetimeReader::fillHist()
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
void lifetimeReader::fillMuHist(TH1D* h, const int muId)
{
    for(int i = 0; i!=15; i++)
    {
	const int bit = 0x1<<i;
	if((muId&bit)==bit) h->Fill(i);
    }
}


// ----------------------------------------------------------------------
void lifetimeReader::bookHist()
{
    cout << "==> lifetimeReader: bookHist " << endl;

    TH1D *h;
    h = new TH1D("h1", "Ntrk", 500, 0., 1000.);
    h = new TH1D("h2", "NCand", 20, 0., 20.);
    h = new TH1D("h3", "cand ID", 1000100, -100., 1000000.);

    h = new TH1D("h4", ("NCand " + toString(CUTCandidate)).c_str(), 20, 0., 20.);
    h = new TH1D("h5", ("NCand fit " + toString(CUTCandidateFit)).c_str(), 20, 0., 20.);
    h = new TH1D("h6", ("NCand matched " + toString(CUTCandidate) + " to " + toString(CUTCandidateFit)).c_str(), 20, 0., 20.);

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

// ----------------------------------------------------------------------
void lifetimeReader::labelMuHisto(TH1D* h)
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
void lifetimeReader::bookReducedTree()
{
    cout << "==> lifetimeReader: bookReducedTree" << endl;

    // create the events tree ======================================
    fTree = new TTree("events", "events");

    // run info
    fTree->Branch("run",     &fRun,     "run/I");
    fTree->Branch("event",   &fEvt,   "event/I");
    fTree->Branch("LS",      &fLS,      "LS/I");
    fTree->Branch("anatype", &fanatype, "anatype/I");
    fTree->Branch("nPv",     &fnPV,     "nPV/I");

    // candidates
    fTree->Branch("mbc",     &fmbc,     "mbc/D");
    fTree->Branch("mrs",     &fmrs,     "mrs/D");
    fTree->Branch("mjp",     &fmjp,     "mjp/D");
    fTree->Branch("mjptlv",  &fmjptlv,  "mjptlv/D");
    fTree->Branch("ptbc",    &fptbc,    "ptbc/D");
    fTree->Branch("ptrs",    &fptrs,    "ptrs/D");
    fTree->Branch("pjp",     &fpjp,     "pjp/D");
    fTree->Branch("pbc",     &fpbc,     "pbc/D");
    fTree->Branch("prs",     &fprs,     "prs/D");
    fTree->Branch("ptjp",    &fptjp,    "ptjp/D");
    fTree->Branch("etabc",   &fetabc,   "etabc/D");
    fTree->Branch("etars",   &fetars,   "etars/D");
    fTree->Branch("etajp",   &fetajp,   "etajp/D");
    fTree->Branch("phibc",   &fphibc,   "phibc/D");
    fTree->Branch("phirs",   &fphirs,   "phirs/D");
    fTree->Branch("phijp",   &fphijp,   "phijp/D");
    fTree->Branch("ybc",     &fybc,     "ybc/D");

    // gen matched variables for resolution estimation
    fTree->Branch("ptgenbc", &fptgenbc, "ptgenbc/D");
    fTree->Branch("mgenbc",  &fmgenbc,  "mgenbc/D");
    fTree->Branch("phigenbc",&fphigenbc,"phigenbc/D");
    fTree->Branch("etagenbc",&fetagenbc,"etagenbc/D");
    fTree->Branch("ygenbc",  &fygenbc,  "ygenbc/D");

    // signal tracks
    fTree->Branch("rptmu1",   &frptmu1,   "rptmu1/D");
    fTree->Branch("rptmu2",   &frptmu2,   "rptmu2/D");
    fTree->Branch("rptha1",   &frptha1,   "rptha1/D");
    fTree->Branch("rptha2",   &frptha2,   "rptha2/D");
    fTree->Branch("rpmu1",    &frpmu1,    "rpmu1/D");
    fTree->Branch("rpmu2",    &frpmu2,    "rpmu2/D");
    fTree->Branch("rphap",    &frphap,    "rphap/D");
    fTree->Branch("rpham",    &frpham,    "rpham/D");
    fTree->Branch("retamu1",  &fretamu1,  "retamu1/D");
    fTree->Branch("retamu2",  &fretamu2,  "retamu2/D");
    fTree->Branch("retaha1",  &fretaha1,  "retaha1/D");
    fTree->Branch("retaha2",  &fretaha2,  "retaha2/D");
    fTree->Branch("rphimu1",  &frphimu1,  "rphimu1/D");
    fTree->Branch("rphimu2",  &frphimu2,  "rphimu2/D");
    fTree->Branch("rphiha1",  &frphiha1,  "rphiha1/D");
    fTree->Branch("rphiha2",  &frphiha2,  "rphiha2/D");
    fTree->Branch("ridmu1",   &fridmu1,   "ridmu1/I");
    fTree->Branch("ridmu2",   &fridmu2,   "ridmu2/I");
    fTree->Branch("rqmu1",    &frqmu1,    "rqmu1/I");
    fTree->Branch("rqmu2",    &frqmu2,    "rqmu2/I");
    fTree->Branch("rqha1",    &frqha1,    "rqha1/I");
    fTree->Branch("rqha2",    &frqha2,    "rqha2/I");
    fTree->Branch("chi2mu1",  &fchi2mu1,  "chi2mu1/D");
    fTree->Branch("chi2mu2",  &fchi2mu2,  "chi2mu2/D");
    fTree->Branch("chi2ha1",  &fchi2ha1,  "chi2ha1/D");
    fTree->Branch("chi2ha2",  &fchi2ha2,  "chi2ha2/D");
    fTree->Branch("probmu1",  &fprobmu1,  "probmu1/D");
    fTree->Branch("probmu2",  &fprobmu2,  "probmu2/D");
    fTree->Branch("probha1",  &fprobha1,  "probha1/D");
    fTree->Branch("probha2",  &fprobha2,  "probha2/D");
    fTree->Branch("ndofmu1",  &fndofmu1,  "ndofmu1/I");
    fTree->Branch("ndofmu2",  &fndofmu2,  "ndofmu2/I");
    fTree->Branch("ndofha1",  &fndofha1,  "ndofha1/I");
    fTree->Branch("ndofha2",  &fndofha2,  "ndofha2/I");
    fTree->Branch("qualmu1",  &fqualmu1,  "qualmu1/I");
    fTree->Branch("qualmu2",  &fqualmu2,  "qualmu2/I");
    fTree->Branch("qualha1",  &fqualha1,  "qualha1/I");
    fTree->Branch("qualha2",  &fqualha2,  "qualha2/I");
    fTree->Branch("algomu1",  &falgomu1,  "algomu1/I");
    fTree->Branch("algomu2",  &falgomu2,  "algomu2/I");
    fTree->Branch("algoha1",  &falgoha1,  "algoha1/I");
    fTree->Branch("algoha2",  &falgoha2,  "algoha2/I");
    fTree->Branch("npixmu1",  &fnpixmu1,  "npixmu1/I");
    fTree->Branch("npixmu2",  &fnpixmu2,  "npixmu2/I");
    fTree->Branch("npixha1",  &fnpixha1,  "npixha1/I");
    fTree->Branch("npixha2",  &fnpixha2,  "npixha2/I");
    // muon selection, AND for bth muons
    fTree->Branch("isMuTight",  &fIsMuTight,  "isMuTight/O");
    fTree->Branch("isMuTight2", &fIsMuTight2, "isMuTight2/O"); // case without ip cut
    fTree->Branch("isMuSoft",   &fIsMuSoft,   "isMuSoft/O");
    fTree->Branch("isCowboy",   &fIsCowboy,   "isCowboy/O");

    // impact parameter ratios
    fTree->Branch("iprmu1",   &fiprmu1,   "iprmu1/D");
    fTree->Branch("iprmu2",   &fiprmu2,   "iprmu2/D");
    fTree->Branch("iprha1",   &fiprha1,   "iprha1/D");
    fTree->Branch("iprha2",   &fiprha2,   "iprha2/D");
    // Armenteros variables
    fTree->Branch("armQt",    &farmQt,   "armQt/D");
    fTree->Branch("armAl",    &farmAl,   "armAl/D");

    fTree->Branch("Kshypo",   &fKshypo,  "Kshypo/D");
    fTree->Branch("L0hypo",   &fL0hypo,  "L0hypo/D");
    fTree->Branch("Kstarhypo1",&fKstarhypo1,  "Kstarhypo1/D");
    fTree->Branch("Kstarhypo2",&fKstarhypo2,  "Kstarhypo2/D");
    fTree->Branch("Phihypo",  &fPhihypo,  "Phihypo/D");
    fTree->Branch("alphabc",  &falphabc, "alphabc/D");
    fTree->Branch("alphars",  &falphars, "alphars/D");
    fTree->Branch("alpharsbc", &falpharsbc, "alpharsbc/D");
    fTree->Branch("ptgangDRbc", &fptgangDRbc, "ptgangDRbc/D");
    fTree->Branch("ptgangDRrs", &fptgangDRrs, "ptgangDRrs/D");
    fTree->Branch("maxdocabc",&fmaxdocabc,    "maxdocabc/D");
    fTree->Branch("maxdocars",&fmaxdocars,    "maxdocars/D");
    fTree->Branch("maxdocajp",&fmaxdocajp,    "maxdocajp/D");
    fTree->Branch("d3bc",    &fd3bc,    "d3bc/D");
    fTree->Branch("d3rs",    &fd3rs,    "d3rs/D");
    fTree->Branch("d3jp",    &fd3jp,    "d3jp/D");
    fTree->Branch("d3Ebc",   &fd3Ebc,   "d3Ebc/D");
    fTree->Branch("d3Ers",   &fd3Ers,   "d3Ers/D");
    fTree->Branch("d3Ejp",   &fd3Ejp,   "d3Ejp/D");
    fTree->Branch("dxybc",   &fdxybc,   "dxybc/D");
    fTree->Branch("dxyrs",   &fdxyrs,   "dxyrs/D");
    fTree->Branch("dxyjp",   &fdxyjp,   "dxyjp/D");
    fTree->Branch("dxyEbc",  &fdxyEbc,  "dxyEbc/D");
    fTree->Branch("dxyErs",  &fdxyErs,  "dxyErs/D");
    fTree->Branch("dxyEjp",  &fdxyEjp,  "dxyEjp/D");

    fTree->Branch("ct3dbc",    &fct3dbc,    "ct3dbc/D");
    fTree->Branch("ct3dbcE",   &fct3dbcE,   "ct3dbcE/D");
    fTree->Branch("ct3dbc_jp", &fct3dbc_jp, "ct3dbc_jp/D"); // w.r.t. jp vertex
    fTree->Branch("ct3drs",    &fct3drs,    "ct3drs/D");
    fTree->Branch("ct3drsE",   &fct3drsE,   "ct3drsE/D");
    fTree->Branch("ctxybc",    &fctxybc,    "ctxybc/D");
    fTree->Branch("ctxybcE",   &fctxybcE,   "ctxybcE/D");
    fTree->Branch("ctxyrs",    &fctxyrs,    "ctxyrs/D");
    fTree->Branch("ctxyrsE",   &fctxyrsE,   "ctxyrsE/D");

    fTree->Branch("ctbctruth", &fctbctruth, "ctbctruth/D");
    fTree->Branch("d3dbctruth", &fd3dbctruth, "d3dbctruth/D");
    fTree->Branch("dxybctruth", &fdxybctruth, "dxybctruth/D");
    fTree->Branch("pbctruth", &fpbctruth, "pbctruth/D");
    fTree->Branch("ptbctruth", &fptbctruth, "ptbctruth/D");

    fTree->Branch("PvLip",   &fPvLip,   "PvLip/D");
    fTree->Branch("PvLipE",  &fPvLipE,  "PvLipE/D");
    fTree->Branch("PvLip2",  &fPvLip2,  "PvLip2/D");
    fTree->Branch("PvLipE2", &fPvLipE2, "PvLipE2/D");

    fTree->Branch("btbcx",   &fbtbcx,   "btbcx/D");
    fTree->Branch("btbcy",   &fbtbcy,   "btbcy/D");
    fTree->Branch("btbcz",   &fbtbcz,   "btbcz/D");
    fTree->Branch("btrsx",   &fbtrsx,   "btrsx/D");
    fTree->Branch("btrsy",   &fbtrsy,   "btrsy/D");
    fTree->Branch("btrsz",   &fbtrsz,   "btrsz/D");

    fTree->Branch("vxbc",    &fvxbc,    "vxbc/D");
    fTree->Branch("vybc",    &fvybc,    "vybc/D");
    fTree->Branch("vzbc",    &fvzbc,    "vzbc/D");
    fTree->Branch("vrbc",    &fvrbc,    "vrbc/D");
    fTree->Branch("vrbcPV",  &fvrbcPV,  "vrbcPV/D");
    fTree->Branch("vxrs",    &fvxrs,    "vxrs/D");
    fTree->Branch("vyrs",    &fvyrs,    "vyrs/D");
    fTree->Branch("vzrs",    &fvzrs,    "vzrs/D");
    fTree->Branch("vrrs",    &fvrrs,    "vrrs/D");
    fTree->Branch("vrrsPV",  &fvrrsPV,  "vrrsPV/D");

    fTree->Branch("chi2bc",  &fchi2bc,  "chi2bc/D");
    fTree->Branch("chi2rs",  &fchi2rs,  "chi2rs/D");
    fTree->Branch("chi2jp",  &fchi2jp,  "chi2jp/D");
    fTree->Branch("ndofbc",  &fndofbc,  "ndofbc/D");
    fTree->Branch("ndofrs",  &fndofrs,  "ndofrs/D");
    fTree->Branch("ndofjp",  &fndofjp,  "ndofjp/D");
    fTree->Branch("probbc",  &fprobbc,  "probbc/D");
    fTree->Branch("probrs",  &fprobrs,  "probrs/D");
    fTree->Branch("probjp",  &fprobjp,  "probjp/D");

    fTree->Branch("dRhaha",  &fdRhaha,  "dRhaha/D");
    fTree->Branch("dRmumu",  &fdRmumu,  "dRmumu/D");
    fTree->Branch("dRrsjp",  &fdRrsjp,  "dRrsjp/D");

    fTree->Branch("angbcrs",&fangbcrs,"angbcrs/D");

    // isolation
    fTree->Branch("isoClostrk",&fIsoClosetrk,"isoClostrk/I");
    fTree->Branch("isoDocatrk",&fIsoDocaTrack,"isoDocatrk/D");

    // gen info to main tree
    fTree->Branch("nDau",    &fnDaughters,"nDau/I");
    fTree->Branch("nGDau",   &fnGrandDaughters,"nGDau/I");
    fTree->Branch("nRef1G",  &fmapRef1Gen,"nRef1G/I");
    fTree->Branch("nRef2G",  &fmapRef2Gen,"nRef2G/I");
    fTree->Branch("isSig",   &fIsSig,     "isSig/I");
    fTree->Branch("isMCmatch",&fIsMCmatch, "isMCmatch/O");

    // sigtrack info
    fTree->Branch("Sptmu1",   &fSptmu1,   "Sptmu1/D");
    fTree->Branch("Spmu1",    &fSpmu1,    "Spmu1/D");
    fTree->Branch("Setamu1",  &fSetamu1,  "Setamu1/D");
    fTree->Branch("Sphimu1",  &fSphimu1,  "Sphimu1/D");
    fTree->Branch("Sptmu2",   &fSptmu2,   "Sptmu2/D");
    fTree->Branch("Spmu2",    &fSpmu2,    "Spmu2/D");
    fTree->Branch("Setamu2",  &fSetamu2,  "Setamu2/D");
    fTree->Branch("Sphimu2",  &fSphimu2,  "Sphimu2/D");
    fTree->Branch("Sptha1",   &fSptha1,   "Sptha1/D");
    fTree->Branch("Setaha1",  &fSetaha1,  "Setaha1/D");
    fTree->Branch("Sphiha1",  &fSphiha1,  "Sphiha1/D");
    fTree->Branch("Sptha2",   &fSptha2,   "Sptha2/D");
    fTree->Branch("Setaha2",  &fSetaha2,  "Setaha2/D");
    fTree->Branch("Sphiha2",  &fSphiha2,  "Sphiha2/D");

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
    fTree->Branch("HLTMu0TkMu0jp",   &fHLTMu0TkMu0jp,   "HLTMu0TkMu0jp/O");
    fTree->Branch("HLTMu0jp", &fHLTMu0jp, "HLTMu0jp/O");
    fTree->Branch("HLTMu5L2Mu2Jpsi", &fHLTMu5L2Mu2Jpsi, "HLTMu5L2Mu2Jpsi/O");
    fTree->Branch("HLTMu5Tr2Jpsi", &fHLTMu5Tr2Jpsi, "HLTMu5Tr2Jpsi/O");
    fTree->Branch("HLTMu5Tr7Jpsi", &fHLTMu5Tr7Jpsi, "HLTMu5Tr7Jpsi/O");
    fTree->Branch("HLTSingleMu", &fHLTSingleMu, "HLTSingleMu/O");
    fTree->Branch("HLTSingleIsoMu", &fHLTSingleIsoMu, "HLTSingleIsoMu/O");
    fTree->Branch("HLTSingleL1Mu",  &fHLTSingleL1Mu,  "HLTSingleL1Mu/O");
    fTree->Branch("HLTSingleL2Mu",  &fHLTSingleL2Mu,  "HLTSingleL2Mu/O");
    fTree->Branch("HLTSingleHLTMu", &fHLTSingleHLTMu, "HLTSingleHLTMu/O");
    fTree->Branch("HLTokJpsi", &fHLTokJpsi, "HLTokJpsi/O");
    fTree->Branch("HLTokBarrelJpsi", &fHLTokBarrelJpsi, "HLTokBarrelJpsi/O");
    fTree->Branch("HLTokDisplJpsi", &fHLTokDisplJpsi, "HLTokDisplJpsi/O");
    // trigger weighting for MC
    fTree->Branch("HLTokBarrelJpsiMC", &fHLTokBarrelJpsiMC, "HLTokBarrelJpsiMC/O");
    fTree->Branch("HLTokDisplJpsiMC", &fHLTokDisplJpsiMC, "HLTokDisplJpsiMC/O");
    fTree->Branch("HLTokBarrelJpsiMCpseudo", &fHLTokBarrelJpsiMCpseudo, "HLTokBarrelJpsiMCpseudo/O");
    fTree->Branch("HLTokDisplJpsiMCpseudo", &fHLTokDisplJpsiMCpseudo, "HLTokDisplJpsiMCpseudo/O");

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

    // Generator tree ==========================================
    fGenTree = new TTree("genevents", "genevents");
    // run info
    fGenTree->Branch("run",     &fRun,     "run/I");
    fGenTree->Branch("event",   &fEvt,   "event/I");
    fGenTree->Branch("LS",      &fLS,      "LS/I");
    fGenTree->Branch("anatype", &fanatype, "anatype/I");

    // MC info
    fGenTree->Branch("nDau",    &fgnDaughters,"nDau/I");
    fGenTree->Branch("nGDau",   &fgnGrandDaughters,"nGDau/I");
    fGenTree->Branch("nRef1G",  &fgmapRef1Gen,"nRef1G/I");
    fGenTree->Branch("nRef2G",  &fgmapRef2Gen,"nRef2G/I");
    fGenTree->Branch("hasCand", &fghasCand,"hasCand/I");
    fGenTree->Branch("hasJpCand", &fghasJpCand,"hasJpCand/I");
    fGenTree->Branch("hasrsCand", &fghasrsCand,"hasrsCand/I");

    // Lambda_b
    fGenTree->Branch("mbc",     &fgmbc,    "gmbc/D");
    fGenTree->Branch("mbcSwap", &fgmbcsw,  "gmbcsw/D");
    fGenTree->Branch("mrs",     &fgmrs,    "gmrs/D");
    fGenTree->Branch("mrsSwap", &fgmrssw,  "gmrssw/D");

    fGenTree->Branch("vxbc",    &fgvxbc,   "vxbc/D");
    fGenTree->Branch("vybc",    &fgvybc,   "vybc/D");
    fGenTree->Branch("vzbc",    &fgvzbc,   "vzbc/D");
    fGenTree->Branch("vrbc",    &fgvrbc,   "vrbc/D");
    fGenTree->Branch("vrbcPV",  &fgvrbcPV, "vrbcPV/D");
    fGenTree->Branch("ctbc",    &fgctbc,   "ctbc/D");
    fGenTree->Branch("d3dbc",   &fgd3dbc,  "d3dbc/D");

    fGenTree->Branch("pbc",     &fgpbc,    "pbc/D");
    fGenTree->Branch("ptbc",    &fgptbc,   "ptbc/D");
    fGenTree->Branch("etabc",   &fgetabc,  "etabc/D");
    fGenTree->Branch("ybc",     &fgybc,    "ybc/D");

    fGenTree->Branch("vxrs",    &fgvxrs,   "vxrs/D");
    fGenTree->Branch("vyrs",    &fgvyrs,   "vyrs/D");
    fGenTree->Branch("vzrs",    &fgvzrs,   "vzrs/D");
    fGenTree->Branch("vrrs",    &fgvrrs,   "vrrs/D");
    fGenTree->Branch("vrrsPV",  &fgvrrsPV, "vrrsPV/D");
    fGenTree->Branch("ctrs",    &fgctrs,   "ctrs/D");
    fGenTree->Branch("d3drs",   &fgd3drs,  "d3drs/D");
    fGenTree->Branch("d2drs",   &fgd2drs,  "d2drs/D");

    // Signal tracks
    fGenTree->Branch("ptmu1",   &fgptmu1,  "ptmu1/D");
    fGenTree->Branch("pmu1",    &fgpmu1,  "pmu1/D");
    fGenTree->Branch("etamu1",  &fgetamu1, "etamu1/D");
    fGenTree->Branch("phimu1",  &fgphimu1, "phimu1/D");

    fGenTree->Branch("ptmu2",   &fgptmu2,  "ptmu2/D");
    fGenTree->Branch("pmu2",    &fgpmu2,  "pmu2/D");
    fGenTree->Branch("etamu2",  &fgetamu2, "etamu2/D");
    fGenTree->Branch("phimu2",  &fgphimu2, "phimu2/D");

    fGenTree->Branch("qha1",     &fgqha1,    "qha1/I");
    fGenTree->Branch("ptha1",    &fgptha1,   "ptha1/D");
    fGenTree->Branch("pha1",     &fgpha1,    "pha1/D");
    fGenTree->Branch("etaha1",   &fgetaha1,  "etaha1/D");
    fGenTree->Branch("phiha1",   &fgphiha1,  "phiha1/D");

    fGenTree->Branch("qha2",     &fgqha2,    "qha2/I");
    fGenTree->Branch("ptha2",    &fgptha2,   "ptha2/D");
    fGenTree->Branch("pha2",     &fgpha2,    "pha2/D");
    fGenTree->Branch("etaha2",   &fgetaha2,  "etaha2/D");
    fGenTree->Branch("phiha2",   &fgphiha2,  "phiha2/D");

    fGenTree->Branch("phap",     &fgphap,    "phap/D");
    fGenTree->Branch("pham",     &fgpham,    "pham/D");

    // Lambda_b direct daughters
    fGenTree->Branch("ptrs",    &fgptrs,   "ptrs/D");
    fGenTree->Branch("prs",     &fgprs,    "prs/D");

    fGenTree->Branch("ptjp",    &fgptjp,   "ptjp/D");
    fGenTree->Branch("pjp",     &fgpjp,    "pjp/D");
    fGenTree->Branch("etajp",   &fgetajp,  "etajp/D");
    fGenTree->Branch("yjp",     &fgyjp,    "yjp/D");

    fGenTree->Branch("dRhaha",  &fgdRhaha, "dRhaha/D");
    fGenTree->Branch("dRmumu",  &fgdRmumu, "dRmumu/D");
    fGenTree->Branch("dRrsbc",  &fgdRrsbc, "dRrsbc/D");

    // Angles between tracks
    fGenTree->Branch("anhaha",  &fganhaha, "anhaha/D");
    fGenTree->Branch("anmumu",  &fganmumu, "anmumu/D");
    fGenTree->Branch("anrsjp",  &fganrsjp, "anrsjp/D");
    fGenTree->Branch("anrsbc",  &fganrsbc, "anrsbc/D");
    fGenTree->Branch("anrsmumin",&fganrsmumin, "anrsmumin/D");
    fGenTree->Branch("anrsmuPt",&fganrsmuPt,"anrsmuPt/D");

    // some reco informtion
    fGenTree->Branch("isMCmatch",&fIsMCmatch, "isMCmatch/O");

    // some trigger information
    fGenTree->Branch("HLTmatch", &fHLTmatch, "HLTmatch/O");
    fGenTree->Branch("HLTokJpsi", &fHLTokJpsi, "HLTokJpsi/O");
    fGenTree->Branch("HLTokBarrelJpsi", &fHLTokBarrelJpsi, "HLTokBarrelJpsi/O");
    fGenTree->Branch("HLTokDisplJpsi", &fHLTokDisplJpsi, "HLTokDisplJpsi/O");
}

// ----------------------------------------------------------------------
void lifetimeReader::readCuts(TString filename, int dump)
{
    // init cut variables
    CUTCandidate = CUTCandidateFit = CUTMuId1 = CUTMuId2 = 9999;
    CUTptmuMin = CUTmjpWindow = CUTptjpMin = 9999;
    CUTmrsWindow = CUTptrsMin = CUTptha1Min = CUTptha2Min = 9999;
    CUTisoCloseTrkDoca = CUTisoCloseTrkPt = 9999;
    CUTptha1GTptha2 = CUTReadDecayMaps = CUTPrintDecayMaps = CUTgenTreeCandsOnly = false;

    // read cut file
    fCutFile = filename;
    if (dump) cout << "==> lambdaReader: Reading " << fCutFile.Data() << " for cut settings" << endl;
    ifstream infile(fCutFile.Data());

    TString fn(fCutFile.Data());

    if (dump)
    {
        cout << "====================================" << endl;
        cout << "==> lambdaReader: Cut file  " << fCutFile.Data() << endl;
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
            if("CUTCandidate" == key) // The candidate to retrieve signal values
            {
                setCut(CUTCandidate, value);
                //setCut(CUTCandidate, value, hcuts, 1, "Candidate");
                continue;
            }
            if("CUTCandidateFit" == key) // The candidate to retrieve kinematic vertex fit data
            {
                setCut(CUTCandidateFit, value);
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
            if("CUTmrsWindow" == key)
            {
                setCut(CUTmrsWindow, value, hcuts, 4, "#pmm(#Lambda^{0})");
                continue;
            }
            if("CUTptrsMin" == key)
            {
                setCut(CUTptrsMin, value, hcuts, 5, "p_{T}^{min}(#Lambda^{0})");
                continue;
            }
            if("CUTptha1GTptha2" == key)
            {
                setCut(CUTptha1GTptha2, value, hcuts, 6, "p_{T}(p)>p_{T}(#ha2)");
                continue;
            }
            if("CUTptha1Min" == key)
            {
                setCut(CUTptha1Min, value, hcuts, 7, "p_{T}^{min}(p)");
                continue;
            }
            if("CUTptha2Min" == key)
            {
                setCut(CUTptha2Min, value, hcuts, 8, "p_{T}^{min}(#ha2)");
                continue;
            }
            if("CUTisoCloseTrkDoca" == key)
            {
                setCut(CUTisoCloseTrkDoca, value, hcuts, 9, "isoCloseTrkDoca");
                continue;
            }
            if("CUTisoCloseTrkPt" == key)
            {
                setCut(CUTisoCloseTrkPt, value, hcuts, 10, "p_{T}^{min}(isoCloseTrk)");
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

            cout << "==> lambdaReader: ERROR in cutfile: Don't know what to do with " << key << "=" << value << endl;
        }
    }

    if (dump)  cout << "------------------------------------" << endl;

}
