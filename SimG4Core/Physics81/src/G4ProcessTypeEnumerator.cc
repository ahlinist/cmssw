#include "SimG4Core/Physics81/interface/G4ProcessTypeEnumerator.h"
#include "SimG4Core/Physics81/interface/ProcessTypeEnumerator.h"

#include "G4VProcess.hh"

#include <iostream>

//#define DEBUG

G4ProcessTypeEnumerator::G4ProcessTypeEnumerator()
{
    mapProcesses["Undefined"] = "Undefined";
    mapProcesses["Unknown"] = "Unknown";
    //
    mapProcesses["Primary"] = "Primary";
    // nuclear stuff
    mapProcesses["AntiNeutronInelastic"] = "Hadronic";
    mapProcesses["PositronNuclear"] = "Hadronic";
    mapProcesses["ElectroNuclear"] = "Hadronic";
    mapProcesses["AntiProtonAnnihilationAtRest"] = "Hadronic";
    mapProcesses["AntiProtonInelastic"] = "Hadronic";
    mapProcesses["ProtonInelastic"] = "Hadronic";
    mapProcesses["PhotonInelastic"] = "Hadronic";
    mapProcesses["DeuteronInelastic"] = "Hadronic";
    mapProcesses["KaonMinusAbsorption"] = "Hadronic";
    mapProcesses["KaonMinusInelastic"] = "Hadronic";
    mapProcesses["KaonPlusInelastic"] = "Hadronic";
    mapProcesses["KaonZeroLInelastic"] = "Hadronic";
    mapProcesses["KaonZeroSInelastic"] = "Hadronic";
    mapProcesses["LCapture"] = "Hadronic";
    mapProcesses["LElastic"] = "Hadronic";
    mapProcesses["hElastic"] = "Hadronic";
    mapProcesses["LambdaInelastic"] = "Hadronic";
    mapProcesses["NeutronInelastic"] = "Hadronic";
    mapProcesses["CHIPSNuclearAbsorptionAtRest"] = "Hadronic";
    mapProcesses["PionMinusAbsorptionAtRest"] = "Hadronic";
    mapProcesses["PionMinusInelastic"] = "Hadronic";
    mapProcesses["PionPlusInelastic"] = "Hadronic";
    mapProcesses["SigmaMinusInelastic"] = "Hadronic";
    mapProcesses["AntiSigmaMinusInelastic"] = "Hadronic";
    mapProcesses["AntiSigmaPlusInelastic"] = "Hadronic";
    mapProcesses["AntiLambdaInelastic"] = "Hadronic";
    mapProcesses["TritonInelastic"] = "Hadronic"; 
    mapProcesses["XiMinusInelastic"] = "Hadronic";
    mapProcesses["XiPlusInelastic"] = "Hadronic";
    mapProcesses["AntiXiZeroInelastic"] = "Hadronic";
    mapProcesses["SigmaPlusInelastic"] = "Hadronic";
    mapProcesses["XiZeroInelastic"] = "Hadronic";
    mapProcesses["AntiXiMinusInelastic"] = "Hadronic";
    // ionizations
    mapProcesses["eIoni"] = "EIoni";
    mapProcesses["hIoni"] = "HIoni";
    mapProcesses["ionIoni"] = "HIoni";
    mapProcesses["muIoni"] = "MuIoni";
    // annihilation
    mapProcesses["annihil"] = "Annihilation";
    // muBrem
    mapProcesses["muBrems"] = "MuBrem";
    // muNucl
    mapProcesses["muMinusCaptureAtRest"] = "MuNucl";
    mapProcesses["MuonMinusCaptureAtRest"] = "MuNucl";
    mapProcesses["MuonPlusCaptureAtRest"] = "MuNucl";
    // conversions
    mapProcesses["conv"] = "Conversions";
    // brems
    mapProcesses["eBrem"] = "EBrem";
    // decay
    mapProcesses["Decay"] = "Decay";
    // pairProd
    mapProcesses["muPairProd"] = "MuPairProd";
    // photon
    mapProcesses["phot"] = "Photon";
    // sync
    mapProcesses["SynchrotronRadiation"] = "SynchrotronRadiation";
    // Compton
    mapProcesses["compt"] = "Compton";
    //
    buildReverseMap();
    //
    theProcessTypeEnumerator = new ProcessTypeEnumerator;
}

G4ProcessTypeEnumerator::~G4ProcessTypeEnumerator()
{
    delete theProcessTypeEnumerator;
    mapProcesses.clear();
    reverseMapProcesses.clear();
}

unsigned int G4ProcessTypeEnumerator::processId(const G4VProcess * process)
{
    if (process == 0) 
    {
	// it is primary!
	std::string temp = "Primary";
#ifdef DEBUG
	edm::LogInfo("SimG4CoreApplication") 
	    << " G4ProcessTypeEnumerator : Primary process, returning " 
	    << theProcessTypeEnumerator->processId(temp);
#endif
	return theProcessTypeEnumerator->processId(temp);
    }
    else
    {
	std::string temp = process->GetProcessName();
#ifdef DEBUG
	edm::LogInfo("SimG4CoreApplication") 
	    << " G4ProcessTypeEnumerator : G4Process "
	    << temp << " mapped to " << processCMSName(temp) << "; returning "
	    << theProcessTypeEnumerator->processId(processCMSName(temp));
#endif
	return theProcessTypeEnumerator->processId(processCMSName(temp));
    }
}

std::string G4ProcessTypeEnumerator::processCMSName(std::string in)
{
    if (mapProcesses[in] == "")
    {
	std::cout <<" NOT FOUND G4ProcessTypeEnumerator: " << in << std::endl;
	return "Unknown";
    }
    return mapProcesses[in];
}

std::vector<std::string> G4ProcessTypeEnumerator::processG4Name(std::string in)
{ return reverseMapProcesses[in]; }

unsigned int G4ProcessTypeEnumerator::numberOfKnownCMSProcesses()
{ return reverseMapProcesses.size(); }

unsigned int G4ProcessTypeEnumerator::numberOfKnownG4Processes()
{ return mapProcesses.size(); }

void G4ProcessTypeEnumerator::buildReverseMap()
{
    for (MapType::const_iterator it = mapProcesses.begin();  
	 it != mapProcesses.end(); it++)
	(reverseMapProcesses[(*it).second]).push_back((*it).first);
}
