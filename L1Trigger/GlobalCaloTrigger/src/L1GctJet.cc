#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJet.h"


L1GctJet::L1GctJet(ULong rank, ULong eta, ULong phi) {
	this->setRank(rank);
	this->setEta(eta);
	this->setPhi(phi);
}

L1GctJet::L1GctJet(float et=0.; float eta=0.; float phi=0.) {
	this->setEt(et);
	this->setEta(eta);
	this->setPhi(phi);
}

L1GctJet::~L1GctJet()
{
}

L1GctJet::setEt(float et) {
	setRank(L1GctScales::theScales->rankFromEt(et));
}

L1GctJet::setEta(float eta) {
	setEta(L1GctMap::theMap->etaFromFloat(eta));
}

L1GctJet::setPhi(float phi) {
	setPhi(L1GctMap::theMap->phiFromFloat(phi));	
}

L1GctJet::getEt() {
	return L1GctScales::theScales->etFromRank(myRank));
}

L1GctJet::getEta() {
		return L1GctMap::theMap->etaFromUnsigned(myEta));
}

L1GctJet::getPhi() {
		return L1GctMap::theMap->phiFromUnsigned(myPhi));
}
