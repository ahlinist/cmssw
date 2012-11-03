#include "lambdaReader.hh"
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
#include "FindDaughters.hh"

using std::cout;
using std::endl;
using std::vector;

// ----------------------------------------------------------------------
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/lambdaReader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------

//==========================================================================

lambdaReader::lambdaReader(TChain *tree, TString evtClassName): lifetimeReader(tree, evtClassName)
{
    cout << "==> lambdaReader: constructor..." << endl;
    fanatype = at_LbJpL0;
    fForcePtHa1GTPtHa2 = false; // keep Ha1 as Pr
}

// ----------------------------------------------------------------------
lambdaReader::~lambdaReader()
{
    cout << "==> lambdaReader: destructor..." << endl;
}

// ----------------------------------------------------------------------
void lambdaReader::endAnalysis()
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
// check if two candidates point to the same two sig tracks for each daughter
bool lambdaReader::compareCands(const CheckedCand clc1, const CheckedCand clc2)
{
    return compareCands4prong(clc1, clc2);
}

// ----------------------------------------------------------------------
// check if the candidates are complete
lifetimeReader::CheckedCand lambdaReader::getCheckedCand(const TAnaCand *tac)
{
    CheckedCand ret;
    ret.isOk = false;
    ret.bc = tac->fIndex;
    // check if daughters point to something
    if (tac->fDau1 < 0 || tac->fDau2 < 0)
    {
        if (fVerbose > 4)  cout << "Problem: tac->fDau1: " << tac->fDau1 << " tac->fDau2: " << tac->fDau2
             << " Run: " << fRun << " Event: " << fEvent << " LS: " << fLS << " -- skipping " << endl;
        return ret;
    }

    // now let's see if we have a jp and a rs
    ret.jp = getCandId(tac,443,1);
    ret.rs = getCandId(tac,3122,1);
    if (ret.jp < 0 || ret.rs < 0)
    {
        if (fVerbose > 4) cout << "Problem finding jp/rs: ret.jp: " << ret.jp << " ret.rs: " << ret.rs
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
    ret.ha1 = getSigId(fpEvt->getCand(ret.rs),2212,1);;
    ret.ha2 = getSigId(fpEvt->getCand(ret.rs),211,1);;
    // cout << ret.jp << " " << ret.rs << " " << ret.mu1 << " " << ret.mu2 << " " << ret.ha2 << " " << ret.ha1 << endl;
    ret.isOk = (ret.mu1 > -1) && (ret.mu2 > -1) && (ret.ha2 > -1) && (ret.ha1 > -1);
    return ret;
}

// ----------------------------------------------------------------------
bool lambdaReader::doCandFitStuff(const CheckedCand &clc)
{
    if (!lifetimeReader::doCandFitStuff(clc)) return false;

    // now we do the specific stuff for Lb
    const TAnaCand *tacCur = fpEvt->getCand(clc.bc);
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacRs = fpEvt->getCand(clc.rs);

    // get some TLorentzVector for eta and phi (and reuse it later)
    TLorentzVector tlvLambdaB, tlvLambda0;
    tlvLambdaB.SetVectM(tacCur->fPlab, MLAMBDA_B);
    tlvLambda0.SetVectM(tacRs->fPlab, MLAMBDA_0);

    fybc=tlvLambdaB.Rapidity();

    // calculate beta factor as 3d vector
    const TVector3 vecJpsiRs = tacRs->fVtx.fPoint - tacJpsi->fVtx.fPoint;
    const TVector3 vecBetaL0 = vecJpsiRs * (1. / tlvLambda0.E());

    const TVector3 vecPV = fpEvt->getPV(tacCur->fPvIdx)->fPoint;
    const TVector3 vecPVbc = tacCur->fVtx.fPoint - vecPV;
    const TVector3 vecBetaLb = vecPVbc   * (1. / tlvLambdaB.E());

    // beta vector components
    fbtrsx=vecBetaL0.x();
    fbtrsy=vecBetaL0.y();
    fbtrsz=vecBetaL0.z();
    fbtbcx=vecBetaLb.x();
    fbtbcy=vecBetaLb.y();
    fbtbcz=vecBetaLb.z();

    return true;
}


// ----------------------------------------------------------------------
bool lambdaReader::doTruthMatching(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Ha1, const TAnaTrack *Ha2, const TVector3 &vtx)
{
    bool ret(false);
    TLorentzVector tlvMu1, tlvMu2, tlvPi, tlvPr;
    tlvMu1.SetVectM(Mu1->fPlab, MMUON);
    tlvMu2.SetVectM(Mu2->fPlab, MMUON);
    tlvPr.SetVectM(Ha1->fPlab, MPROTON);
    tlvPi.SetVectM(Ha2->fPlab, MPION);

    const double thrshMu(.1), thrshPi(.1), thrshPr(.1);

    const int giMu1(fpEvt->getGenIndexWithDeltaR(tlvMu1,Mu1->fQ, thrshMu));
    const int giMu2(fpEvt->getGenIndexWithDeltaR(tlvMu2,Mu2->fQ, thrshMu));
    const int giPr(fpEvt->getGenIndexWithDeltaR(tlvPr,vtx,Ha1->fQ, thrshPr, 1.3));
    const int giPi(fpEvt->getGenIndexWithDeltaR(tlvPi,vtx,Ha2->fQ, thrshPi, 1.3));
    const int nGenCands(fpEvt->nGenCands());
    if( giMu1 < nGenCands && giMu2 < nGenCands && giPi < nGenCands && giPr < nGenCands &&
            giMu1 > -1 && giMu2 > -1 && giPi > -1 && giPr > -1 )
    {
        TGenCand *gcMu1 = fpEvt->getGenCand(giMu1);
	TGenCand *gcMu1Mom = fpEvt->getGenCand(gcMu1->fMom1);
        TGenCand *gcMu2 = fpEvt->getGenCand(giMu2);
	TGenCand *gcMu2Mom = fpEvt->getGenCand(gcMu2->fMom1);
        TGenCand *gcProton = fpEvt->getGenCand(giPr);
	TGenCand *gcProtonMom = fpEvt->getGenCand(gcProton->fMom1);
        TGenCand *gcPion = fpEvt->getGenCand(giPi);
	TGenCand *gcPionMom = fpEvt->getGenCand(gcPion->fMom1);

        ret = (TMath::Abs(gcMu1->fID) == 13
	       && TMath::Abs(gcMu1Mom->fID) == 443
               && TMath::Abs(gcMu2->fID) == 13
	       && TMath::Abs(gcMu2Mom->fID) == 443
               && TMath::Abs(gcPion->fID) == 211
               && TMath::Abs(gcPionMom->fID) == 3122
               && TMath::Abs(gcProton->fID) == 2212
	       && TMath::Abs(gcProtonMom->fID) == 3122);

        if (fVerbose > 5)
        {
            cout << "Truth matching: Mu1: " << gcMu1->fID
                 << " Mu2: " << gcMu2->fID
                 << " Pion: " << gcPion->fID
                 << " Proton: " << gcProton->fID
                 << " isMatch: " << fIsMCmatch << " isSig: " << fIsSig << endl;
        }
    }
    return ret;
}

// ----------------------------------------------------------------------
void lambdaReader::doGenLevelStuff()
{
    // Do some generator level stuff
    const int nGenCands(fpEvt->nGenCands());
    for(int gcit=0; gcit!=nGenCands; gcit++)
    {
        TGenCand *gcCur = fpEvt->getGenCand(gcit);
        if(5122==abs(gcCur->fID))
        {
            FindDaughters fd(gcCur,fpEvt);
	    if (fVerbose > 3) cout << fd.getDauGrdau() << endl;
	    // skip if we are at a Lb that does not decay into our channel, i.e. another one or one which "oscillates" to the interesting one
	    if ("5122 -> 3122 (2212 211 ) 443 (13 13 ) "!=fd.getDauGrdau()
		    && "5122 -> 3122 (211 2212 ) 443 (13 13 ) "!=fd.getDauGrdau()
		    && "5122 -> 443 (13 13 ) 3122 (211 2212 ) "!=fd.getDauGrdau()
		    && "5122 -> 443 (13 13 ) 3122 (2212 211 ) "!=fd.getDauGrdau()) continue;
            fIsSig = true;
	    // some kinematic variables for comparison with reco
	    fptgenbc = gcCur->fP.Pt();
	    fmgenbc = gcCur->fMass;
	    fphigenbc = gcCur->fP.Phi();
	    fetagenbc = gcCur->fP.Eta();
	    fygenbc = gcCur->fP.Rapidity();

            // analyse the decay and extract some data to store in the tree
            fnDaughters = fgnDaughters = fd.getNDaughters();
            fnGrandDaughters = fgnGrandDaughters = fd.getNDauGrdau();
            fmapRef1Gen = fgmapRef1Gen = myDecayMap1Gen.getPos(fd.getDaughters());
            fmapRef2Gen = fgmapRef2Gen = myDecayMap2Gen.getPos(fd.getDauGrdau());
            // now extract some kinematic data
            TLorentzVector tlvGenJp, tlvGenL0, tlvGenPr, tlvGenPi, tlvGenMu1, tlvGenMu2, tlvGenLambda0, tlvGenLambdaB, tlvGenLambdaSwapped;
            TVector3 tv3PV, tv3LbV, tv3L0V;
	    tv3PV = gcCur->fV; // vertices in candblock denote where the particle was produced
	    bool got443(false), got3122(false);
	    for(int gcDauit=gcCur->fDau1; gcDauit<=gcCur->fDau2; gcDauit++)
	    {
		TGenCand* gcDau=fpEvt->getGenCand(gcDauit);
		if(443==abs(gcDau->fID))
		{
		    got443 = true;
		    tv3LbV = gcDau->fV;
		    fgvxbc = tv3LbV.x();
		    fgvybc = tv3LbV.y();
		    fgvzbc = tv3LbV.z();
		    fgvrbc = tv3LbV.XYvector().Mod();
		    fgvrbcPV = (tv3LbV-tv3PV).XYvector().Mod();
		    // ct. Observe that Lb is in gcCur
		    fgpbc = fpbctruth = gcCur->fP.Vect().Mag();
		    fgptbc = fptbctruth = gcCur->fP.Vect().Perp();
		    fgd3dbc = fd3dbctruth = (tv3LbV-tv3PV).Mag();
		    fctbctruth = fgctbc = fd3dbctruth / fpbctruth * MLAMBDA_B / TMath::Ccgs();
		    fdxybctruth = (tv3LbV-tv3PV).Perp();
		    unsigned int mucounter(0);
		    tlvGenJp=gcDau->fP;
		    fgptjp=tlvGenJp.Pt();
		    fgpjp=tlvGenJp.P();
		    fgetajp=tlvGenJp.Eta();
		    fgyjp=tlvGenJp.Rapidity();
		    for(int gcGrDauit=gcDau->fDau1; gcGrDauit<=gcDau->fDau2; gcGrDauit++)
		    {
			TGenCand* gcGrDau=fpEvt->getGenCand(gcGrDauit);
			if(13==abs(gcGrDau->fID)&&gcGrDau->fMom1==gcDau->fNumber&&gcGrDau->fMom2==gcDau->fNumber)
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
		if(3122==abs(gcDau->fID))
		{
		    got3122 = true;
		    tlvGenL0=gcDau->fP;
		    bool prFound(false), piFound(false);
		    for(int gcGrDauit=gcDau->fDau1; gcGrDauit<=gcDau->fDau2; gcGrDauit++)
		    {
			TGenCand* gcGrDau=fpEvt->getGenCand(gcGrDauit);
			if(!prFound&&2212==abs(gcGrDau->fID)&&gcGrDau->fMom1==gcDau->fNumber&&gcGrDau->fMom2==gcDau->fNumber)
			{	// in case we have more than one ha1 we get the first
			    tlvGenPr=gcGrDau->fP;
			    fgqha1 = gcGrDau->fQ;
			    prFound=true;

			    // fill in data for the L0 decay vertex
			    tv3L0V = gcGrDau->fV;
			    fgvxrs = tv3L0V.x();
			    fgvyrs = tv3L0V.y();
			    fgvzrs = tv3L0V.z();
			    fgvrrs = tv3L0V.XYvector().Mod();
			    fgvrrsPV = (tv3L0V-tv3PV).XYvector().Mod();
			    // ct. Observe that L0 is in gcDau
			    fgd3drs = (tv3L0V-tv3PV).Mag();
			    fgd2drs = (tv3L0V-tv3PV).XYvector().Mod();
			    fgctrs = fgd3drs / gcDau->fP.Vect().Mag() * MLAMBDA_0;
			}
			if(!piFound&&211==abs(gcGrDau->fID)&&gcGrDau->fMom1==gcDau->fNumber&&gcGrDau->fMom2==gcDau->fNumber)
			{	// in case we have more than one ha2 we get the first
			    tlvGenPi=gcGrDau->fP;
			    fgqha2 = gcGrDau->fQ;
			    piFound=true;
			}
		    }
		    if(prFound&&piFound) // we have a ha1 and a ha2
		    {
			fgptha1 = tlvGenPr.Pt();
			fgpha1 = tlvGenPr.P();
			fgetaha1 = tlvGenPr.Eta();
			fgphiha1 = tlvGenPr.Phi();

			fgptha2 = tlvGenPi.Pt();
			fgpha2 = tlvGenPi.P();
			fgetaha2 = tlvGenPi.Eta();
			fgphiha2 = tlvGenPi.Phi();

			fgphap = ( fgqha1 > 0 ? fgpha1 : fgpha2);
			fgpham = ( fgqha1 < 0 ? fgpha1 : fgpha2);

			fgdRhaha = tlvGenPr.DeltaR(tlvGenPi);
			fganhaha = tlvGenPr.Angle(tlvGenPi.Vect());
			// make a L0
			tlvGenLambda0 = tlvGenPr + tlvGenPi;
			fgmrs = tlvGenLambda0.M();
			fgptrs = tlvGenLambda0.Pt();
			fgprs = tlvGenLambda0.P();
			// make a L0 with swapped mass assumptions for ha1 and ha2
			TLorentzVector tlvGenPrAsPi, tlvGenPiAsPr;
			tlvGenPrAsPi.SetVectM(tlvGenPr.Vect(),MPION);
			tlvGenPiAsPr.SetVectM(tlvGenPi.Vect(),MPROTON);
			tlvGenLambdaSwapped=tlvGenPrAsPi+tlvGenPiAsPr;
			fgmrssw = tlvGenLambdaSwapped.M();

			//if (fgmrs>2) fpEvt->dumpGenBlock();
		    }
		}
		fganrsjp = tlvGenJp.Angle(tlvGenLambda0.Vect());
		fganrsbc = tlvGenLambdaB.Angle(tlvGenLambda0.Vect());
		const double anrsmu1 = tlvGenLambda0.Angle(tlvGenMu1.Vect());
		const double anrsmu2 = tlvGenLambda0.Angle(tlvGenMu2.Vect());
		fganrsmumin = anrsmu1<anrsmu2 ? anrsmu1 : anrsmu2;
		fganrsmuPt = tlvGenMu1.Pt() > tlvGenMu2.Pt() ? anrsmu1 : anrsmu2;
	    }
	    if (got443 && got3122)
	    {
		fgdRrsbc = tlvGenJp.DeltaR(tlvGenLambda0);
		// make a Lb
		tlvGenLambdaB = tlvGenLambda0+tlvGenJp;
		fgmbc = tlvGenLambdaB.M();
		fgetabc = tlvGenLambdaB.Eta();
		fgybc = tlvGenLambdaB.Rapidity();
		const TLorentzVector tlvGenLambdaBSwapped = tlvGenLambdaSwapped+tlvGenJp;
		fgmbcsw = tlvGenLambdaBSwapped.M();
	    }
        }
    }
}

// ----------------------------------------------------------------------
// Checks if events have cands needed for efficiencies of candidate creation
// requires that the n-tuple has been created with them on (larger n-tuple, so not by default)
void lambdaReader::doCheckEffCands()
{
    fghasJpCand = fghasrsCand = 0;
    TAnaCand *pCand;
    for (int iC = 0; iC < fpEvt->nCands(); ++iC)
    {
	pCand = fpEvt->getCand(iC);
	if (pCand->fType == 500443) fghasJpCand++;
	if (pCand->fType == 503122) fghasrsCand++;
    }
}

// ----------------------------------------------------------------------
bool lambdaReader::checkCuts()
{
    return ( (frptmu1 > CUTptmuMin) && (frptmu2 > CUTptmuMin)
	&& (fmjp > MJPSI-CUTmjpWindow) && (fmjp < MJPSI+CUTmjpWindow)
	&& (fptjp > CUTptjpMin)
	&& (fmrs > MLAMBDA_0-CUTmrsWindow) && (fmrs < MLAMBDA_0+CUTmrsWindow)
	&& (fptrs > CUTptrsMin)
	&& (!CUTptha1GTptha2 || (frptha1 > frptha2))
	&& (frptha1 > CUTptha1Min)
	&& (frptha2 > CUTptha2Min)
	   );
}

// ----------------------------------------------------------------------
void lambdaReader::initVariables()
{
    lifetimeReader::initVariables();
}

// ----------------------------------------------------------------------
void lambdaReader::fillHist()
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
void lambdaReader::bookHist()
{
    cout << "==> lambdaReader: bookHist " << endl;

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
void lambdaReader::bookReducedTree()
{
    cout << "==> lambdaReader: bookReducedTree" << endl;
    lifetimeReader::bookReducedTree();
}

