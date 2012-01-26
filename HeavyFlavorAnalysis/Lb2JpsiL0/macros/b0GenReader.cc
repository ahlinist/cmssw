#define TAUCALCULATION_PATCH
#include "b0GenReader.hh"
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
// Run with: ./runTreeReaders -c chains/bg-test -D root -C cuts/b0GenReader.default.cuts
//           ./runTreeReaders -f test.root
// ----------------------------------------------------------------------


//==========================================================================

b0GenReader::b0GenReader(TChain *tree, TString evtClassName): treeReader01(tree, evtClassName)
{
    cout << "==> b0GenReader: constructor..." << endl;
}

// ----------------------------------------------------------------------
b0GenReader::~b0GenReader()
{
    cout << "==> b0GenReader: destructor..." << endl;
}

// ----------------------------------------------------------------------
void b0GenReader::startAnalysis()
{
    cout << "==> b0GenReader: Starting analysis..." << endl;
    if (!fIsMC) cout << "Oooops, probably you'd forgot a -m in the call? This reader works on MC only. But I'll try my best..." << endl;
    // book reduced trees
    bookReducedTree();
    fKsWarningCounter = fNfilled = 0;
}

// ----------------------------------------------------------------------
void b0GenReader::endAnalysis()
{
    cout << "==> b0GenReader: Filled " << fNfilled << " candidates into tree" << endl;
    if (fKsWarningCounter != 0 ) cout << "    WARNING: candidates without Ks->pipi: " << fKsWarningCounter << " of " << fNfilled << endl;
}

// ----------------------------------------------------------------------
void b0GenReader::eventProcessing()
{
    initVariables(); // still very crude...
    const bool reqKsStrict(false); // true: Ks needs to decay into exactly 2 charged pi

    if (fVerbose>2) cout << "-------------------------------" << endl;
    const int nGenCands(fpEvt->nGenCands());
    for(int gcit=0; gcit!=nGenCands; gcit++)
    {
        TGenCand *gcCur = fpEvt->getGenCand(gcit);
        if(511==TMath::Abs(gcCur->fID))
	{
	    //fpEvt->dumpGenBlock();
	    const bool isGoodDecay = goodDecay(gcCur, reqKsStrict);
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
bool b0GenReader::goodDecay(TGenCand* gc, bool strict)
{
    bool ret(false);
    //fpEvt->dumpGenBlock();
    const int nB0daughters = countDaughters(gc);
    const int nJpsi = countDaughters(gc, 443);
    const int nK0 = countDaughters(gc, 311);
    if (fVerbose>5) cout << "This B0 has " << nB0daughters << " daughters: " << nJpsi << " Jpsi and " << nK0 << " K0" << endl;
    if (nB0daughters == 2 && nJpsi == 1 && nK0 ==1)
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
	// now check K0
	const int gcIdK0 = findDaughter(gc, 311);
	TGenCand *gcK0 = fpEvt->getGenCand(gcIdK0);
	const int nK0Daughters = countDaughters(gcK0);
	const int nKs = countDaughters(gcK0, 310);
	if (fVerbose>5)
	{
	    cout << "   K0 has " << nK0Daughters << " daughters, " << nKs << " of them are Ks";
	    if (nK0Daughters == 1 && nKs == 1 ) cout << " -- ok" << endl;
	    else cout << " -- not ok" << endl;
	}
	if (nJpsiDaughters == 2 && nMu == 2 && nK0Daughters == 1 && nKs == 1)
	{
	    if (!strict) ret = true;
	    // and now check Ks
	    const int gcIdKs = findDaughter(gcK0, 310);
	    TGenCand *gcKs = fpEvt->getGenCand(gcIdKs);
	    const int nKsDaughters = countDaughters(gcKs);
	    const int nPi = countDaughters(gcKs, 211);
	    if (fVerbose>5)
	    {
		cout << "   Ks has " << nKsDaughters << " daughters, " << nPi << " of them are charged pions";
		if (nKsDaughters == 2 && nPi == 2) cout << " -- ok" << endl;
		else cout << " -- not ok" << endl;
	    }
	    if (nKsDaughters == 2 && nPi == 2)
	    {
		ret = true;
		if (fVerbose>5) cout << "   SUCCESS - we got a B0 -> Jpsi(mu mu) Ks (pi pi)" << endl;
	    }
	}
    }
    return ret;
}

// ----------------------------------------------------------------------
void b0GenReader::fillTree(TGenCand *gc)
{
    // NB: We don't need to do any boundary check here as this code runs only when the decay is checked already
    // B0
    fpB0 = gc->fP.P();
    fptB0 = gc->fP.Pt();
    fetaB0 = gc->fP.Eta();
    fphiB0 = gc->fP.Phi();
    // B0 decay
    const int gcIdJpsi = findDaughter(gc, 443);
    TGenCand *gcJpsi = fpEvt->getGenCand(gcIdJpsi);
    const TVector3 tv3ProdB0 = getProdVtx(gc, 511);
    const TVector3 tv3DecayB0 = gcJpsi->fV;
    fd3dB0 = (tv3ProdB0 - tv3DecayB0).Mag();
    ft3dB0 = fd3dB0 / fpB0 * MB_0 / TMath::Ccgs();
    fIsB0bar = (gc->fID == -511);
    // Ks
    const int gcIdK0 = findDaughter(gc, 311);
    TGenCand *gcK0 = fpEvt->getGenCand(gcIdK0);
    const int gcIdKs = findDaughter(gcK0, 310);
    TGenCand *gcKs = fpEvt->getGenCand(gcIdKs);
    fpKs = gcKs->fP.P();
    fptKs = gcKs->fP.Pt();
    fetaKs = gcKs->fP.Eta();
    fphiKs = gcKs->fP.Phi();
    const int gcIdPi1 = findDaughter(gcKs, 211, 1);
    const int gcIdPi2 = findDaughter(gcKs, 211, 2);
    if (gcIdPi1 < 0 || gcIdPi2 < 0)
    {
	fKsWarningCounter++;
	const unsigned int KsWarningThreshold(10);
	if (fKsWarningCounter < KsWarningThreshold) cout << "==> b0GenReader: WARNING: Problem getting pions - probably no Geant4 data in block?" << endl;
	if (fKsWarningCounter == KsWarningThreshold) cout << "==> b0GenReader: Now getting bored of these pion not found warnings - suppressing further warnings. See end of output to get total number of problematic cases." << endl;
    }
    else
    {
	TGenCand *gcPi1 = fpEvt->getGenCand(gcIdPi1);
	TGenCand *gcPi2 = fpEvt->getGenCand(gcIdPi2);
	const TVector3 tv3DecayKs = gcPi1->fV;
	fd3dKs = (tv3DecayKs - tv3DecayB0).Mag();
	ft3dKs = fd3dKs / fpKs * MKSHORT / TMath::Ccgs();
	fvtxxKs = tv3DecayKs.X();
	fvtxyKs = tv3DecayKs.Y();
	fvtxzKs = tv3DecayKs.Z();
	fvtxrKs = tv3DecayKs.XYvector().Mod();
	// Pions
	fppi1 = gcPi1->fP.P();
	fptpi1 = gcPi1->fP.Pt();
	fetapi1 = gcPi1->fP.Eta();
	fphipi1 = gcPi1->fP.Phi();
	fppi2 = gcPi2->fP.P();
	fptpi2 = gcPi2->fP.Pt();
	fetapi2 = gcPi2->fP.Eta();
	fphipi2 = gcPi2->fP.Phi();
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
void b0GenReader::initVariables()
{
    // B0
    fpB0 = fptB0 = fetaB0 = fphiB0 = fd3dB0 = ft3dB0 = 9999;
    fOsc = fIsB0bar = false;
    // Ks
    fpKs = fptKs = fetaKs = fphiKs = fd3dKs = ft3dKs = 9999;
    fvtxxKs = fvtxyKs = fvtxzKs = fvtxrKs = 9999; // w.r.t detector origin
    // mu
    fpmu1 = fptmu1 = fetamu1 = fphimu1 = 9999;
    fpmu2 = fptmu2 = fetamu2 = fphimu2 = 9999;
    // pi
    fppi1 = fptpi1 = fetapi1 = fphipi1 = 9999;
    fppi2 = fptpi2 = fetapi2 = fphipi2 = 9999;
}

// ----------------------------------------------------------------------
bool b0GenReader::checkCuts()
{
    return true;
}

// ----------------------------------------------------------------------
void b0GenReader::fillHist()
{

    if (fVerbose > 5) cout << "fillHist()" << endl;
    ((TH1D*)fpHistFile->Get("hpB0"))->Fill(fpB0);
    ((TH1D*)fpHistFile->Get("hptB0"))->Fill(fptB0);
    ((TH1D*)fpHistFile->Get("hetaB0"))->Fill(fetaB0);
    ((TH1D*)fpHistFile->Get("hphiB0"))->Fill(fphiB0);
    ((TH1D*)fpHistFile->Get("hd3dB0"))->Fill(fd3dB0);
    ((TH1D*)fpHistFile->Get("ht3dB0"))->Fill(ft3dB0);

    ((TH1D*)fpHistFile->Get("hpKs"))->Fill(fpKs);
    ((TH1D*)fpHistFile->Get("hptKs"))->Fill(fptKs);
    ((TH1D*)fpHistFile->Get("hetaKs"))->Fill(fetaKs);
    ((TH1D*)fpHistFile->Get("hphiKs"))->Fill(fphiKs);
    ((TH1D*)fpHistFile->Get("hd3dKs"))->Fill(fd3dKs);
    ((TH1D*)fpHistFile->Get("ht3dKs"))->Fill(ft3dKs);

    ((TH1D*)fpHistFile->Get("hppi1"))->Fill(fppi1);
    ((TH1D*)fpHistFile->Get("hptpi1"))->Fill(fptpi1);
    ((TH1D*)fpHistFile->Get("hetapi1"))->Fill(fetapi1);
    ((TH1D*)fpHistFile->Get("hphipi1"))->Fill(fphipi1);

    ((TH1D*)fpHistFile->Get("hppi2"))->Fill(fppi2);
    ((TH1D*)fpHistFile->Get("hptpi2"))->Fill(fptpi2);
    ((TH1D*)fpHistFile->Get("hetapi2"))->Fill(fetapi2);
    ((TH1D*)fpHistFile->Get("hphipi2"))->Fill(fphipi2);

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
void b0GenReader::bookHist()
{
    cout << "==> b0GenReader: bookHist " << endl;

    TH1D *h;
    h = new TH1D("hpB0", "p(B^{0}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("p(B^{0}) [GeV/c]");
    h = new TH1D("hptB0", "p_{T}(B^{0}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p_{T}(B^{0}) [GeV/c]");
    h = new TH1D("hetaB0", "#eta(B^{0}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(B^{0})");
    h = new TH1D("hphiB0", "#phi(B^{0}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(B^{0})");
    h = new TH1D("hd3dB0", "flightlength (B^{0}) MC generator level", 50, 0, 1.0);
    h->GetXaxis()->SetTitle("d_{3d}(B^{0}) [cm]");
    h = new TH1D("ht3dB0", "lifetime (B^{0}) MC generator level", 75, 0, 15e-12);
    h->GetXaxis()->SetTitle("t(B^{0}) [s]");

    h = new TH1D("hpKs", "p(K_{s}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("p(K_{s}) [GeV/c]");
    h = new TH1D("hptKs", "p_{T}(K_{s}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p_{T}(K_{s}) [GeV/c]");
    h = new TH1D("hetaKs", "#eta(K_{s}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(K_{s})");
    h = new TH1D("hphiKs", "#phi(K_{s}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(K_{s})");
    h = new TH1D("hd3dKs", "flightlength (K_{s}) MC generator level", 50, 0, 100);
    h->GetXaxis()->SetTitle("d_{3d}(K_{s}) [cm]");
    h = new TH1D("ht3dKs", "lifetime (K_{s}) MC generator level", 75, 0, 15e-10);
    h->GetXaxis()->SetTitle("t(K_{s}) [s]");

    h = new TH1D("hppi1", "p(#pi_{1}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p(#pi_{1}) [GeV/c]");
    h = new TH1D("hptpi1", "p_{T}(#pi_{1}) MC generator level", 50, 0, 25);
    h->GetXaxis()->SetTitle("p_{T}(#pi_{1}) [GeV/c]");
    h = new TH1D("hetapi1", "#eta(#pi_{1}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#pi_{1})");
    h = new TH1D("hphipi1", "#phi(#pi_{1}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
    h->GetXaxis()->SetTitle("#phi(#pi_{1})");

    h = new TH1D("hppi2", "p(#pi_{2}) MC generator level", 50, 0, 50);
    h->GetXaxis()->SetTitle("p(#pi_{2}) [GeV/c]");
    h = new TH1D("hptpi2", "p_{T}(#pi_{2}) MC generator level", 50, 0, 25);
    h->GetXaxis()->SetTitle("p_{T}(#pi_{2}) [GeV/c]");
    h = new TH1D("hetapi2", "#eta(#pi_{2}) MC generator level", 50, -2.5, 2.5);
    h->GetXaxis()->SetTitle("#eta(#pi_{2})");
    h = new TH1D("hphipi2", "#phi(#pi_{2}) MC generator level", 50, -TMath::Pi(), +TMath::Pi());
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
void b0GenReader::bookReducedTree()
{
    cout << "==> b0GenReader: bookReducedTree" << endl;

    // create the events tree ======================================
    fTree = new TTree("genevents", "genevents");

    // run info
    fTree->Branch("run",     &fRun,     "run/I");
    fTree->Branch("event",   &fEvt,     "event/I");
    fTree->Branch("LS",      &fLS,      "LS/I");

    // B0
    fTree->Branch("pB0",     &fpB0,     "pB0/D");
    fTree->Branch("ptB0",    &fptB0,    "ptB0/D");
    fTree->Branch("etaB0",   &fetaB0,   "etaB0/D");
    fTree->Branch("phiB0",   &fphiB0,   "phiB0/D");
    fTree->Branch("d3dB0",   &fd3dB0,   "d3dB0/D");
    fTree->Branch("t3dB0",   &ft3dB0,   "t3dB0/D");

    fTree->Branch("B0osc",   &fOsc,     "B0osc/O");
    fTree->Branch("B0bar",   &fIsB0bar, "B0bar/O");

    // Ks
    fTree->Branch("pKs",     &fpKs,     "pKs/D");
    fTree->Branch("ptKs",    &fptKs,    "ptKs/D");
    fTree->Branch("etaKs",   &fetaKs,   "etaKs/D");
    fTree->Branch("phiKs",   &fphiKs,   "phiKs/D");
    fTree->Branch("d3dKs",   &fd3dKs,   "d3dKs/D");
    fTree->Branch("t3dKs",   &ft3dKs,   "t3dKs/D");
    fTree->Branch("vtxxKs",  &fvtxxKs,  "vtxxKs/D");
    fTree->Branch("vtxyKs",  &fvtxyKs,  "vtxyKs/D");
    fTree->Branch("vtxzKs",  &fvtxzKs,  "vtxzKs/D");
    fTree->Branch("vtxrKs",  &fvtxrKs,  "vtxrKs/D");

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
    fTree->Branch("ppi1",    &fppi1,    "ppi1/D");
    fTree->Branch("ptpi1",   &fptpi1,   "ptpi1/D");
    fTree->Branch("etapi1",  &fetapi1,  "etapi1/D");
    fTree->Branch("phipi1",  &fphipi1,  "phipi1/D");
    fTree->Branch("ppi2",    &fppi2,    "ppi2/D");
    fTree->Branch("ptpi2",   &fptpi2,   "ptpi2/D");
    fTree->Branch("etapi2",  &fetapi2,  "etapi2/D");
    fTree->Branch("phipi2",  &fphipi2,  "phipi2/D");
}

// ----------------------------------------------------------------------
void b0GenReader::readCuts(TString filename, int dump)
{
    // set some default cuts
    CUTB0= 0; // indicates non set value
    // read cut file
    fCutFile = filename;
    if (dump) cout << "==> b0GenReader: Reading " << fCutFile.Data() << " for cut settings" << endl;
    ifstream infile(fCutFile.Data());

    TString fn(fCutFile.Data());

    if (dump)
    {
        cout << "====================================" << endl;
        cout << "==> b0GenReader: Cut file  " << fCutFile.Data() << endl;
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
            if("CUTB0" == key) // The candidate to retrieve signal values
            {
                setCut(CUTB0, value);
                //setCut(CUTB0Candidate, value, hcuts, 1, "Candidate");
                continue;
            }

            cout << "==> b0GenReader: ERROR in cutfile: Don't know what to do with " << key << "=" << value << endl;
        }
    }

    if (dump)  cout << "------------------------------------" << endl;

}

// ----------------------------------------------------------------------
// searches for a daughter with a specific pid and gets its id back.
// If more than one such daughter are found, the last one is given.
// -1 means no such daughter found
int b0GenReader::findDaughter(TGenCand* gc, int pid)
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
int b0GenReader::findDaughter(TGenCand* gc, int pid, int number)
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
int b0GenReader::countDaughters(TGenCand* gc, int pid)
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
int b0GenReader::countDaughters(TGenCand* gc)
{
    return gc->fDau2 - gc->fDau1 + 1;
}

// ----------------------------------------------------------------------
// a recursive retriever for B0, as B0 may come from another B0 due to oscillation
TVector3 b0GenReader::getProdVtx(TGenCand* gc, int pid)
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

