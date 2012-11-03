#include "b0Reader.hh"
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
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/b0Reader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------

//==========================================================================

b0Reader::b0Reader(TChain *tree, TString evtClassName): lifetimeReader(tree, evtClassName)
{
    cout << "==> b0Reader: constructor..." << endl;
    fanatype = at_B0JpK0;
    fForcePtHa1GTPtHa2 = true; // both hadrons are pions, so we want to have them ordered
}

// ----------------------------------------------------------------------
b0Reader::~b0Reader()
{
    cout << "==> b0Reader: destructor..." << endl;
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
// check if two candidates point to the same two sig tracks for each daughter
bool b0Reader::compareCands(const CheckedCand clc1, const CheckedCand clc2)
{
    return compareCands4prong(clc1, clc2);
}

// ----------------------------------------------------------------------
// check if the candidates are complete
lifetimeReader::CheckedCand b0Reader::getCheckedCand(const TAnaCand *tac)
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

    // now let's see if we have a jp and a ks
    ret.jp = getCandId(tac,443,1);
    ret.rs = getCandId(tac,310,1);
    if (ret.jp < 0 || ret.rs < 0)
    {
        if (fVerbose > 4) cout << "Problem finding jp/ks: ret.jp: " << ret.jp << " ret.ks: " << ret.rs
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
    ret.ha1 = getSigId(fpEvt->getCand(ret.rs),211,1);
    ret.ha2 = getSigId(fpEvt->getCand(ret.rs),211,2);
    // cout << ret.jp << " " << ret.Ks << " " << ret.mu1 << " " << ret.mu2 << " " << ret.pi << " " << ret.pr << endl;
    ret.isOk = (ret.mu1 > -1) && (ret.mu2 > -1) && (ret.ha1 > -1) && (ret.ha2 > -1);
    return ret;
}

// ----------------------------------------------------------------------
bool b0Reader::doCandFitStuff(const CheckedCand &clc)
{
    if (!lifetimeReader::doCandFitStuff(clc)) return false;

    // now we do the specific stuff for B0
    const TAnaCand *tacCur = fpEvt->getCand(clc.bc);
    const TAnaCand *tacJpsi = fpEvt->getCand(clc.jp);
    const TAnaCand *tacRs = fpEvt->getCand(clc.rs);

    // get some TLorentzVector for eta and phi (and reuse it later)
    TLorentzVector tlvB0, tlvKs;
    tlvB0.SetVectM(tacCur->fPlab, MB_0);
    tlvKs.SetVectM(tacRs->fPlab, MKSHORT);

    fybc=tlvB0.Rapidity();

    // calculate beta factor as 3d vector
    const TVector3 vecJpsiRs = tacRs->fVtx.fPoint - tacJpsi->fVtx.fPoint;
    const TVector3 vecBetaKs = vecJpsiRs * (1. / tlvKs.E());

    const TVector3 vecPV = fpEvt->getPV(tacCur->fPvIdx)->fPoint;
    const TVector3 vecPVbc = tacCur->fVtx.fPoint - vecPV;
    const TVector3 vecBetaB0 = vecPVbc   * (1. / tlvB0.E());

    // beta vector components
    fbtrsx=vecBetaKs.x();
    fbtrsy=vecBetaKs.y();
    fbtrsz=vecBetaKs.z();
    fbtbcx=vecBetaB0.x();
    fbtbcy=vecBetaB0.y();
    fbtbcz=vecBetaB0.z();

    return true;
}

// ----------------------------------------------------------------------
bool b0Reader::doTruthMatching(const TAnaTrack *Mu1, const TAnaTrack *Mu2, const TAnaTrack *Ha1, const TAnaTrack *Ha2, const TVector3 &vtx)
{
    bool ret(false);
    TLorentzVector tlvMu1, tlvMu2, tlvPi1, tlvPi2;
    tlvMu1.SetVectM(Mu1->fPlab, MMUON);
    tlvMu2.SetVectM(Mu2->fPlab, MMUON);
    tlvPi1.SetVectM(Ha1->fPlab, MPION);
    tlvPi2.SetVectM(Ha2->fPlab, MPION);

    const double thrshMu(.1), thrshPi(.1);

    const int giMu1(fpEvt->getGenIndexWithDeltaR(tlvMu1,Mu1->fQ, thrshMu));
    const int giMu2(fpEvt->getGenIndexWithDeltaR(tlvMu2,Mu2->fQ, thrshMu));
    const int giPi1(fpEvt->getGenIndexWithDeltaR(tlvPi1,vtx,Ha1->fQ, thrshPi, 1.3));
    const int giPi2(fpEvt->getGenIndexWithDeltaR(tlvPi2,vtx,Ha2->fQ, thrshPi, 1.3));
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
    // Do some generator level stuff
    const int nGenCands(fpEvt->nGenCands());
    for(int gcit=0; gcit!=nGenCands; gcit++)
    {
        TGenCand *gcCur = fpEvt->getGenCand(gcit);
        if(511==TMath::Abs(gcCur->fID))
        {
            FindDaughters fd(gcCur,fpEvt);
	    // skip if we are at a B0 that does not decay into our channel, i.e. another one or one which oscillates to the interesting one
	    if ("511 -> 443 (13 13 ) 311 (211 211 ) "!=fd.getDauGrdau()
		    && "511 -> 311 (211 211 ) 443 (13 13 ) "!=fd.getDauGrdau()) continue;
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
            TLorentzVector tlvGenJp, tlvGenKs, tlvGenPi1, tlvGenPi2, tlvGenMu1, tlvGenMu2, tlvGenKshort, tlvGenB0;
            TVector3 tv3PV, tv3B0V, tv3KsV;
	    // vertices in candblock denote where the particle was produced.
	    tv3PV = getProdVtx(gcCur, 511); // this gets the production vertex of the first B0 in case of mixing
	    // now search for the daughters of the B0
	    bool got443(false), got310(false);
	    for(int gcDauit=gcCur->fDau1; gcDauit<=gcCur->fDau2; gcDauit++)
	    {
		TGenCand* gcDau=fpEvt->getGenCand(gcDauit);
		if(443==TMath::Abs(gcDau->fID))
		{
		    got443 = true;
		    tv3B0V = gcDau->fV;
		    fgvxbc = tv3B0V.x();
		    fgvybc = tv3B0V.y();
		    fgvzbc = tv3B0V.z();
		    fgvrbc = tv3B0V.XYvector().Mod();
		    fgvrbcPV = (tv3B0V-tv3PV).XYvector().Mod();
		    // ct. Observe that Lb is in gcCur
		    fgpbc = fpbctruth = gcCur->fP.Vect().Mag();
		    fgptbc = fptbctruth = gcCur->fP.Vect().Perp();
		    fgd3dbc = fd3dbctruth = (tv3B0V-tv3PV).Mag();
		    //fgctbc = (tv3B0V-tv3PV).Mag() / gcCur->fP.Vect().Mag() * MB_0 / TMath::Ccgs();
		    fctbctruth = fgctbc = fgd3dbc / fgpbc * MB_0 / TMath::Ccgs();
		    fdxybctruth = (tv3B0V-tv3PV).Perp();
		    unsigned int mucounter(0);
		    tlvGenJp=gcDau->fP;
		    fgptjp=tlvGenJp.Pt();
		    fgpjp=tlvGenJp.P();
		    fgetajp=tlvGenJp.Eta();
		    fgyjp=tlvGenJp.Rapidity();
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
		    got310 = true;
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
				fgqha1 = gcGrDau->fQ;
				tlvGenPi1=gcGrDau->fP;
				// fill in data for the Ks decay vertex
				tv3KsV = gcGrDau->fV;
				fgvxrs = tv3KsV.x();
				fgvyrs = tv3KsV.y();
				fgvzrs = tv3KsV.z();
				fgvrrs = tv3KsV.XYvector().Mod();
				fgvrrsPV = (tv3KsV-tv3PV).XYvector().Mod();
				// ct. Observe that Ks is in gcDauDau
				fgd3drs = (tv3KsV-tv3PV).Mag();
				fgd2drs = (tv3KsV-tv3PV).XYvector().Mod();
				fgctrs = fgd3drs / gcDauDau->fP.Vect().Mag() * MKSHORT / TMath::Ccgs();
			    }
			    else
			    {
				pi2Found = true;
				fgqha2 = gcGrDau->fQ;
				tlvGenPi2=gcGrDau->fP;
			    }

			}
		    }
		    if(pi1Found&&pi2Found) // we have a pr and a pi
		    {
			fgptha1 = tlvGenPi1.Pt();
			fgpha1 = tlvGenPi1.P();
			fgetaha1 = tlvGenPi1.Eta();
			fgphiha1 = tlvGenPi1.Phi();

			fgptha2 = tlvGenPi2.Pt();
			fgpha2 = tlvGenPi2.P();
			fgetaha2 = tlvGenPi2.Eta();
			fgphiha2 = tlvGenPi2.Phi();

			fgphap = ( fgqha1 > 0 ? fgpha1 : fgpha2);
			fgpham = ( fgqha1 < 0 ? fgpha1 : fgpha2);

			fgdRhaha = tlvGenPi1.DeltaR(tlvGenPi2);
			fganhaha = tlvGenPi1.Angle(tlvGenPi2.Vect());
			// make a Ks
			tlvGenKshort = tlvGenPi1 + tlvGenPi2;
			fgmrs = tlvGenKshort.M();
			fgptrs = tlvGenKshort.Pt();
			fgprs = tlvGenKshort.P();
		    }
		}
		fganrsjp = tlvGenJp.Angle(tlvGenKshort.Vect());
		fganrsbc = tlvGenB0.Angle(tlvGenKshort.Vect());
		const double anKsmu1 = tlvGenKshort.Angle(tlvGenMu1.Vect());
		const double anKsmu2 = tlvGenKshort.Angle(tlvGenMu2.Vect());
		fganrsmumin = anKsmu1<anKsmu2 ? anKsmu1 : anKsmu2;
		fganrsmuPt = tlvGenMu1.Pt() > tlvGenMu2.Pt() ? anKsmu1 : anKsmu2;
	    }
	    if (got443 && got310)
	    {
		fgdRrsbc = tlvGenJp.DeltaR(tlvGenKshort);
		// make aB0
		tlvGenB0 = tlvGenKshort+tlvGenJp;
		fgmbc = tlvGenB0.M();
		fgetabc = tlvGenB0.Eta();
		fgybc = tlvGenB0.Rapidity();
	    }
        }
    }
}

// ----------------------------------------------------------------------
// Checks if events have cands needed for efficiencies of candidate creation
// requires that the n-tuple has been created with them on (larger n-tuple, so not by default)
void b0Reader::doCheckEffCands()
{
    fghasJpCand = fghasrsCand = 0;
    TAnaCand *pCand;
    for (int iC = 0; iC < fpEvt->nCands(); ++iC)
    {
	pCand = fpEvt->getCand(iC);
	if (pCand->fType == 500443) fghasJpCand++;
	if (pCand->fType == 500310) fghasrsCand++;
    }
}


// ----------------------------------------------------------------------
bool b0Reader::checkCuts()
{
    return ( (frptmu1 > CUTptmuMin) && (frptmu2 > CUTptmuMin)
	&& (fmjp > MJPSI-CUTmjpWindow) && (fmjp < MJPSI+CUTmjpWindow)
	&& (fptjp > CUTptjpMin)
	&& (fmrs > MKSHORT-CUTmrsWindow) && (fmrs < MKSHORT+CUTmrsWindow)
	&& (fptrs > CUTptrsMin)
	&& (frptha1 > CUTptha1Min)
	&& (frptha2 > CUTptha2Min)
	   );
}

// ----------------------------------------------------------------------
void b0Reader::initVariables()
{
    lifetimeReader::initVariables();
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
void b0Reader::bookHist()
{
    cout << "==> b0Reader: bookHist " << endl;

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
void b0Reader::bookReducedTree()
{
    cout << "==> b0Reader: bookReducedTree" << endl;
    lifetimeReader::bookReducedTree();
}

