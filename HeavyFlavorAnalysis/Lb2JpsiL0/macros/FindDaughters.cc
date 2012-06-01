#include <string>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"

#include "FindDaughters.hh"
#include "Lb2JpsiL0_utils.h"

FindDaughters::FindDaughters(TGenCand* curParticle, TAna01Event* fpEvt) : nDaughters(0)
{
    strDaughters = "";
    strDauGrdau = toString(abs(curParticle->fID)) + " -> ";
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
	    const unsigned int id = abs(gcDau->fID);
	    // check if these particles come from the right mother
	    if(gcDau->fMom1==mother->fNumber&&gcDau->fMom2==mother->fNumber)
	    {
		nDaughters++;
		strDaughters += toString(id) + " ";
		FindDaughters grdau(gcDau,fpEvt);
		strDauGrdau += toString(id) + " (" + grdau.getDaughters() + ") " ;
		strDauGrdaus += toString(id) + " (" + grdau.getDauGrdau() + ") " ;
		nDauGrdau += grdau.getNDaughters();
	    }
	    // else: particle comes from hadronic interactions or decays in flight
	    // discard this for the moment
	}
    }
}

