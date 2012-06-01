#ifndef GUARD_FINDDAUGHTERS_H
#define GUARD_FINDDAUGHTERS_H

#include <string>

#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "../../../AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"

//==========================================================================
class FindDaughters
{
    public:
	FindDaughters() : nDaughters(0) {};
	FindDaughters(TGenCand* curParticle, TAna01Event* fpEvt);
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

#endif

