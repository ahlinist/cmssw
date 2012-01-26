#define TAUCALCULATION_PATCH
#include "lambdaGenReader.hh"
#include "../../Bs2MuMu/interface/HFMasses.hh"

#include "TRandom.h"
#include <cmath>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "Lb2JpsiL0_utils.h"

using std::cout;
using std::endl;
using std::vector;

// ----------------------------------------------------------------------
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/lambdaGenReader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------


//==========================================================================

lambdaGenReader::lambdaGenReader(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName)
{
    cout << "==> lambdaGenReader: constructor..." << endl;
}

// ----------------------------------------------------------------------
lambdaGenReader::~lambdaGenReader()
{
    cout << "==> lambdaGenReader: destructor..." << endl;
}

// ----------------------------------------------------------------------
void lambdaGenReader::startAnalysis()
{
    cout << "==> lambdaGenReader: Starting analysis..." << endl;
    if (!fIsMC) cout << "Oooops, probably you'd forgot a -m in the call? This reader works on MC only. But I'll try my best..." << endl;
    // book reduced trees
    bookReducedTree();
    fL0WarningCounter = fNfilled = 0;
}

// ----------------------------------------------------------------------
void lambdaGenReader::endAnalysis()
{
    cout << "==> lambdaGenReader: Filled " << fNfilled << " candidates into tree" << endl;
    if (fL0WarningCounter != 0 ) cout << "    WARNING: candidates without L0->prpi: " << fL0WarningCounter << " of " << fNfilled << endl;
}

// ----------------------------------------------------------------------
void lambdaGenReader::eventProcessing()
{
    initVariables(); // still very crude...
    const bool reqL0Strict(true); // true: L0 needs to decay into exactly one proton and one pion

    if (fVerbose>2) cout << "-------------------------------" << endl;
    const int nGenCands(fpEvt->nGenCands());
    for(int gcit=0; gcit!=nGenCands; gcit++)
    {
        TGenCand *gcCur = fpEvt->getGenCand(gcit);
        if(5122==TMath::Abs(gcCur->fID))
	{
	    //fpEvt->dumpGenBlock();
	    const bool isGoodDecay = goodDecay(gcCur, reqL0Strict);
	    if (fVerbose > 2)
	    {
		if (isGoodDecay) cout << " SUCCESS! good decay" << endl;
		else cout << " FAIL. Decay not ok" << endl;
	    }
	    if (isGoodDecay) // we got a decay in the desired channel
	    {
		//fpEvt->dumpGenBlock();
		fillTree(gcCur);
		fillHist();
	    }
	}
    }

    return;
}

// ----------------------------------------------------------------------
// checks if the decay fulfills all requirements
bool lambdaGenReader::goodDecay(TGenCand* gc, bool strict)
{
    bool ret(false);
    //fpEvt->dumpGenBlock();
    const int nLbdaughters = countDaughters(gc);
    const int nJpsi = countDaughters(gc, 443);
    const int nL0 = countDaughters(gc, 3122);
    if (fVerbose>5) cout << "This Lb has " << nLbdaughters << " daughters: " << nJpsi << " Jpsi and " << nL0 << " L0" << endl;
    if (nLbdaughters == 2 && nJpsi == 1 && nL0 ==1)
    {
	if (fVerbose>5) cout << "--> possibly a good one, let's see..." << endl;
	// now check Jpsi
	const int gcIdJpsi = findDaughter(gc, 443);
	TGenCand *gcJpsi = fpEvt->getGenCand(gcIdJpsi);
	const int nJpsiDaughters = countDaughters(gcJpsi);
	const int nMu = countDaughters(gcJpsi, 13);
	if (fVerbose>5)
	{
	    cout << "   Jpsi has " << nJpsiDaughters << " daughters, " << nMu << " of them are muons";
	    if (nJpsiDaughters == 2 && nMu == 2 ) cout << " -- ok" << endl;
	    else cout << " -- not ok" << endl;
	}
	if (nJpsiDaughters == 2 && nMu == 2)
	{
	    if (!strict) ret = true;
	    // now check L0
	    const int gcIdL0 = findDaughter(gc, 3122);
	    TGenCand *gcL0 = fpEvt->getGenCand(gcIdL0);
	    const int nL0Daughters = countDaughters(gcL0);
	    const int nPr = countDaughters(gcL0, 2212);
	    const int nPi = countDaughters(gcL0, 211);
	    if (fVerbose>5)
	    {
		cout << "   L0 has " << nL0Daughters << " daughters, " << nPr << " of them are protons and " << nPi << " of them are pions";
		if (nL0Daughters == 2 && nPr == 1 && nPi == 1) cout << " -- ok" << endl;
		else cout << " -- not ok" << endl;
	    }
	    if (nL0Daughters == 2 && nPr == 1 && nPi == 1) ret = true;
	}
    }
    return ret;
}

// ----------------------------------------------------------------------
void lambdaGenReader::fillTree(TGenCand *gc)
{
    // NB: We don't need to do any boundary check here as this code runs only when the decay is checked already
    // Lb
    fpLb = gc->fP.P();
    fptLb = gc->fP.Pt();
    fetaLb = gc->fP.Eta();
    fphiLb = gc->fP.Phi();
    // Lb decay
    const int gcIdJpsi = findDaughter(gc, 443);
    TGenCand *gcJpsi = fpEvt->getGenCand(gcIdJpsi);
    const TVector3 tv3ProdLb = getProdVtx(gc, 5122);
    const TVector3 tv3DecayLb = gcJpsi->fV;
    fd3dLb = (tv3ProdLb - tv3DecayLb).Mag();
    ft3dLb = fd3dLb / fpLb * MB_0 / TMath::Ccgs();
    fIsLbbar = (gc->fID == -5122);
    // L0
    const int gcIdL0 = findDaughter(gc, 3122);
    TGenCand *gcL0 = fpEvt->getGenCand(gcIdL0);
    fpL0 = gcL0->fP.P();
    fptL0 = gcL0->fP.Pt();
    fetaL0 = gcL0->fP.Eta();
    fphiL0 = gcL0->fP.Phi();
    const int gcIdPr = findDaughter(gcL0, 2212);
    const int gcIdPi = findDaughter(gcL0, 211);
    if (gcIdPr < 0 || gcIdPi < 0)
    {
	fL0WarningCounter++;
	const unsigned int L0WarningThreshold(10);
	if (fL0WarningCounter < L0WarningThreshold) cout << "==> lambdaGenReader: WARNING: Problem getting protons/pions - probably no Geant4 data in block?" << endl;
	if (fL0WarningCounter == L0WarningThreshold) cout << "==> lambdaGenReader: Now getting bored of these proton/pion not found warnings - suppressing further warnings. See end of output to get total number of problematic cases." << endl;
    }
    else
    {
	TGenCand *gcPr = fpEvt->getGenCand(gcIdPr);
	TGenCand *gcPi = fpEvt->getGenCand(gcIdPi);
	const TVector3 tv3DecayL0 = gcPr->fV;
	fd3dL0 = (tv3DecayL0 - tv3DecayLb).Mag();
	ft3dL0 = fd3dL0 / fpL0 * MKSHORT / TMath::Ccgs();
	fvtxxL0 = tv3DecayL0.X();
	fvtxyL0 = tv3DecayL0.Y();
	fvtxzL0 = tv3DecayL0.Z();
	fvtxrL0 = tv3DecayL0.XYvector().Mod();
	// Pions
	fppr = gcPr->fP.P();
	fptpr = gcPr->fP.Pt();
	fetapr = gcPr->fP.Eta();
	fphipr = gcPr->fP.Phi();
	fppi = gcPi->fP.P();
	fptpi = gcPi->fP.Pt();
	fetapi = gcPi->fP.Eta();
	fphipi = gcPi->fP.Phi();
    }
    // Muons
    const int gcIdMu1 = findDaughter(gcJpsi, 13, 1);
    const int gcIdMu2 = findDaughter(gcJpsi, 13, 2);
    TGenCand *gcMu1 = fpEvt->getGenCand(gcIdMu1);
    TGenCand *gcMu2 = fpEvt->getGenCand(gcIdMu2);
    fpmu1 = gcMu1->fP.P();
    fptmu1 = gcMu1->fP.Pt();
    fetamu1 = gcMu1->fP.Eta();
    fphimu1 = gcMu1->fP.Phi();
    fpmu2 = gcMu2->fP.P();
    fptmu2 = gcMu2->fP.Pt();
    fetamu2 = gcMu2->fP.Eta();
    fphimu2 = gcMu2->fP.Phi();
    // and finally...
    fTree->Fill();
    fNfilled++;
}

// ----------------------------------------------------------------------
void lambdaGenReader::initVariables()
{
    // Lb
    fpLb = fptLb = fetaLb = fphiLb = fd3dLb = ft3dLb = 9999;
    fOsc = fIsLbbar = false;
    // L0
    fpL0 = fptL0 = fetaL0 = fphiL0 = fd3dL0 = ft3dL0 = 9999;
    fvtxxL0 = fvtxyL0 = fvtxzL0 = fvtxrL0 = 9999; // w.r.t detector origin
    // mu
    fpmu1 = fptmu1 = fetamu1 = fphimu1 = 9999;
    fpmu2 = fptmu2 = fetamu2 = fphimu2 = 9999;
    // pi
    fppr = fptpr = fetapr = fphipr = 9999;
    fppi = fptpi = fetapi = fphipi = 9999;
}

// ----------------------------------------------------------------------
bool lambdaGenReader::checkCuts()
{
    return true;
}

// ----------------------------------------------------------------------
void lambdaGenReader::fillHist()
{

    if (fVerbose > 5) cout << "fillHist()" << endl;
    ((TH1D*)fpHistFile->Get("hpLb"))->Fill(fpLb);
    ((TH1D*)fpHistFile->Get("hptLb"))->Fill(fptLb);
    ((TH1D*)fpHistFile->Get("hetaLb"))->Fill(fetaLb);
    ((TH1D*)fpHistFile->Get("hphiLb"))->Fill(fphiLb);
    ((TH1D*)fpHistFile->Get("hd3dLb"))->Fill(fd3dLb);
    ((TH1D*)fpHistFile->Get("ht3dLb"))->Fill(ft3dLb);

    ((TH1D*)fpHistFile->Get("hpL0"))->Fill(fpL0);
    ((TH1D*)fpHistFile->Get("hptL0"))->Fill(fptL0);
    ((TH1D*)fpHistFile->Get("hetaL0"))->Fill(fetaL0);
    ((TH1D*)fpHistFile->Get("hphiL0"))->Fill(fphiL0);
    ((TH1D*)fpHistFile->Get("hd3dL0"))->Fill(fd3dL0);
    ((TH1D*)fpHistFile->Get("ht3dL0"))->Fill(ft3dL0);

    ((TH1D*)fpHistFile->Get("hppr"))->Fill(fppr);
    ((TH1D*)fpHistFile->Get("hptpr"))->Fill(fptpr);
    ((TH1D*)fpHistFile->Get("hetapr"))->Fill(fetapr);
    ((TH1D*)fpHistFile->Get("hphipr"))->Fill(fphipr);

    ((TH1D*)fpHistFile->Get("hppi"))->Fill(fppi);
    ((TH1D*)fpHistFile->Get("hptpi"))->Fill(fptpi);
    ((TH1D*)fpHistFile->Get("hetapi"))->Fill(fetapi);
    ((TH1D*)fpHistFile->Get("hphipi"))->Fill(fphipi);

    ((TH1D*)fpHistFile->Get("hpmu1"))->Fill(fpmu1);
    ((TH1D*)fpHistFile->Get("hptmu1"))->Fill(fptmu1);
    ((TH1D*)fpHistFile->Get("hetamu1"))->Fill(fetamu1);
    ((TH1D*)fpHistFile->Get("hphimu1"))->Fill(fphimu1);

    ((TH1D*)fpHistFile->Get("hpmu2"))->Fill(fpmu2);
    ((TH1D*)fpHistFile->Get("hptmu2"))->Fill(fptmu2);
    ((TH1D*)fpHistFile->Get("hetamu2"))->Fill(fetamu2);
    ((TH1D*)fpHistFile->Get("hphimu2"))->Fill(fphimu2);
}

// ----------------------------------------------------------------------
void lambdaGenReader::bookHist()
{
    cout << "==> lambdaGenReader: bookHist " << endl;

    TH1D *h;
    h = new TH1D("hpLb", "p(B^{0}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("p(B^{0}) [GeV/c]");
    h = new TH1D("hptLb", "p_{T}(B^{0}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p_{T}(B^{0}) [GeV/c]");
    h = new TH1D("hetaLb", "#eta(B^{0}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(B^{0})");
    h = new TH1D("hphiLb", "#phi(B^{0}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(B^{0})");
    h = new TH1D("hd3dLb", "flightlength (B^{0}) MC generator level", 50, 0, 1.0);
    h->GetXaxis()->SetTitle("d_{3d}(B^{0}) [cm]");
    h = new TH1D("ht3dLb", "lifetime (B^{0}) MC generator level", 75, 0, 15e-12);
    h->GetXaxis()->SetTitle("t(B^{0}) [s]");

    h = new TH1D("hpL0", "p(K_{s}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("p(K_{s}) [GeV/c]");
    h = new TH1D("hptL0", "p_{T}(K_{s}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p_{T}(K_{s}) [GeV/c]");
    h = new TH1D("hetaL0", "#eta(K_{s}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(K_{s})");
    h = new TH1D("hphiL0", "#phi(K_{s}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(K_{s})");
    h = new TH1D("hd3dL0", "flightlength (K_{s}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("d_{3d}(K_{s}) [cm]");
    h = new TH1D("ht3dL0", "lifetime (K_{s}) MC generator level", 75, 0, 15e-10);
    h->GetXaxis()->SetTitle("t(K_{s}) [s]");

    h = new TH1D("hppr", "p(#pi_{1}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p(#pi_{1}) [GeV/c]");
    h = new TH1D("hptpr", "p_{T}(#pi_{1}) MC generator level", 50, 0, 25);
    h->GetXaxis()->SetTitle("p_{T}(#pi_{1}) [GeV/c]");
    h = new TH1D("hetapr", "#eta(#pi_{1}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#pi_{1})");
    h = new TH1D("hphipr", "#phi(#pi_{1}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(#pi_{1})");

    h = new TH1D("hppi", "p(#pi_{2}) MC generator level", 50, 0, 20);
    h->GetXaxis()->SetTitle("p(#pi_{2}) [GeV/c]");
    h = new TH1D("hptpi", "p_{T}(#pi_{2}) MC generator level", 50, 0, 10);
    h->GetXaxis()->SetTitle("p_{T}(#pi_{2}) [GeV/c]");
    h = new TH1D("hetapi", "#eta(#pi_{2}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#pi_{2})");
    h = new TH1D("hphipi", "#phi(#pi_{2}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(#pi_{2})");

    h = new TH1D("hpmu1", "p(#mu_{1}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p(#mu_{1}) [GeV/c]");
    h = new TH1D("hptmu1", "p_{T}(#mu_{1}) MC generator level", 50, 0, 25);
    h->GetXaxis()->SetTitle("p_{T}(#mu_{1}) [GeV/c]");
    h = new TH1D("hetamu1", "#eta(#mu_{1}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#mu_{1})");
    h = new TH1D("hphimu1", "#phi(#mu_{1}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(#mu_{1})");

    h = new TH1D("hpmu2", "p(#mu_{2}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p(#mu_{2}) [GeV/c]");
    h = new TH1D("hptmu2", "p_{T}(#mu_{2}) MC generator level", 50, 0, 25);
    h->GetXaxis()->SetTitle("p_{T}(#mu_{2}) [GeV/c]");
    h = new TH1D("hetamu2", "#eta(#mu_{2}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#mu_{2})");
    h = new TH1D("hphimu2", "#phi(#mu_{2}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(#mu_{2})");

    return;
}

// ----------------------------------------------------------------------
void lambdaGenReader::bookReducedTree()
{
    cout << "==> lambdaGenReader: bookReducedTree" << endl;

    // create the events tree ======================================
    fTree = new TTree("genevents", "genevents");

    // run info
    fTree->Branch("run",     &fRun,     "run/I");
    fTree->Branch("event",   &fEvt,     "event/I");
    fTree->Branch("LS",      &fLS,      "LS/I");

    // Lb
    fTree->Branch("pLb",     &fpLb,     "pLb/D");
    fTree->Branch("ptLb",    &fptLb,    "ptLb/D");
    fTree->Branch("etaLb",   &fetaLb,   "etaLb/D");
    fTree->Branch("phiLb",   &fphiLb,   "phiLb/D");
    fTree->Branch("d3dLb",   &fd3dLb,   "d3dLb/D");
    fTree->Branch("t3dLb",   &ft3dLb,   "t3dLb/D");

    fTree->Branch("Lbosc",   &fOsc,     "Lbosc/O");
    fTree->Branch("Lbbar",   &fIsLbbar, "Lbbar/O");

    // L0
    fTree->Branch("pL0",     &fpL0,     "pL0/D");
    fTree->Branch("ptL0",    &fptL0,    "ptL0/D");
    fTree->Branch("etaL0",   &fetaL0,   "etaL0/D");
    fTree->Branch("phiL0",   &fphiL0,   "phiL0/D");
    fTree->Branch("d3dL0",   &fd3dL0,   "d3dL0/D");
    fTree->Branch("t3dL0",   &ft3dL0,   "t3dL0/D");
    fTree->Branch("vtxxL0",  &fvtxxL0,  "vtxxL0/D");
    fTree->Branch("vtxyL0",  &fvtxyL0,  "vtxyL0/D");
    fTree->Branch("vtxzL0",  &fvtxzL0,  "vtxzL0/D");
    fTree->Branch("vtxrL0",  &fvtxrL0,  "vtxrL0/D");

    // muons
    fTree->Branch("pmu1",    &fpmu1,    "pmu1/D");
    fTree->Branch("ptmu1",   &fptmu1,   "ptmu1/D");
    fTree->Branch("etamu1",  &fetamu1,  "etamu1/D");
    fTree->Branch("phimu1",  &fphimu1,  "phimu1/D");
    fTree->Branch("pmu2",    &fpmu2,    "pmu2/D");
    fTree->Branch("ptmu2",   &fptmu2,   "ptmu2/D");
    fTree->Branch("etamu2",  &fetamu2,  "etamu2/D");
    fTree->Branch("phimu2",  &fphimu2,  "phimu2/D");

    // pion
    fTree->Branch("ppr",    &fppr,    "ppr/D");
    fTree->Branch("ptpr",   &fptpr,   "ptpr/D");
    fTree->Branch("etapr",  &fetapr,  "etapr/D");
    fTree->Branch("phipr",  &fphipr,  "phipr/D");
    fTree->Branch("ppi",    &fppi,    "ppi/D");
    fTree->Branch("ptpi",   &fptpi,   "ptpi/D");
    fTree->Branch("etapi",  &fetapi,  "etapi/D");
    fTree->Branch("phipi",  &fphipi,  "phipi/D");
}

// ----------------------------------------------------------------------
void lambdaGenReader::readCuts(TString filename, int dump)
{
    // set some default cuts
    CUTLb= 0; // indicates non set value
    // read cut file
    fCutFile = filename;
    if (dump) cout << "==> lambdaGenReader: Reading " << fCutFile.Data() << " for cut settings" << endl;
    ifstream infile(fCutFile.Data());

    TString fn(fCutFile.Data());

    if (dump)
    {
        cout << "====================================" << endl;
        cout << "==> lambdaGenReader: Cut file  " << fCutFile.Data() << endl;
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
            if("CUTLb" == key) // The candidate to retrieve signal values
            {
                setCut(CUTLb, value);
                //setCut(CUTLbCandidate, value, hcuts, 1, "Candidate");
                continue;
            }

            cout << "==> lambdaGenReader: ERROR in cutfile: Don't know what to do with " << key << "=" << value << endl;
        }
    }

    if (dump)  cout << "------------------------------------" << endl;

}

// ----------------------------------------------------------------------
// searches for a daughter with a specific pid and gets its id back.
// If more than one such daughter are found, the last one is given.
// -1 means no such daughter found
int lambdaGenReader::findDaughter(TGenCand* gc, int pid)
{
    int ret = -1;
    for(int gcDauIt=gc->fDau1; gcDauIt<=gc->fDau2; gcDauIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcDauIt);
	if (TMath::Abs(gcCur->fID) == pid) ret = gcDauIt;
    }
    return ret;
}

// ----------------------------------------------------------------------
// searches for a daughter with a specific pid and gets its id back.
// number defines which daughter is returned, 1 means first, 2 second and so on.
// -1 means no such daughter found
int lambdaGenReader::findDaughter(TGenCand* gc, int pid, int number)
{
    int ret(-1);
    int counter(0);
    for(int gcDauIt=gc->fDau1; gcDauIt<=gc->fDau2; gcDauIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcDauIt);
	if (TMath::Abs(gcCur->fID) == pid)
	{
	    counter++;
	    if (counter == number) ret = gcDauIt;
	}
    }
    return ret;
}

// ----------------------------------------------------------------------
// counts how many daughters are there with a specific pid
int lambdaGenReader::countDaughters(TGenCand* gc, int pid)
{
    int ret = 0;
    for(int gcDauIt=gc->fDau1; gcDauIt<=gc->fDau2; gcDauIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcDauIt);
	if (TMath::Abs(gcCur->fID) == pid) ret++;
    }
    return ret;
}

// ----------------------------------------------------------------------
// gets number of daughters
int lambdaGenReader::countDaughters(TGenCand* gc)
{
    return gc->fDau2 - gc->fDau1 + 1;
}

// ----------------------------------------------------------------------
// a recursive retriever for Lb, as Lb may come from another Lb due to oscillation
TVector3 lambdaGenReader::getProdVtx(TGenCand* gc, int pid)
{
    TVector3 ret = gc->fV;
    for(int gcMomIt=gc->fMom1; gcMomIt<=gc->fMom2; gcMomIt++)
    {
	TGenCand *gcCur = fpEvt->getGenCand(gcMomIt);
	if (TMath::Abs(gcCur->fID) == pid)
	{
	    ret = getProdVtx(gcCur, pid);
	    fOsc = true;
	}
    }
    return ret;
}

