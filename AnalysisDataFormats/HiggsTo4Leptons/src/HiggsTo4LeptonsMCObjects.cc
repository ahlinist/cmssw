/*
 * author: Dominique Fortin - UC Riverside
 */

#include <AnalysisDataFormats/HiggsTo4Leptons/interface/HiggsTo4LeptonsMCObjects.h>
#include <CLHEP/Vector/LorentzVector.h>
#include <vector>


HiggsTo4LeptonsMCObjects::HiggsTo4LeptonsMCObjects() :
    theDecayType(),
    theH(),
    theZ(),
    theZdecayType(),
    theL(),
    theLId(),
    theLMother(),
    theLMotherId(),
    theG(),
    theGMother(),
    theGMotherId(),
    theIBG(),
    theIBGMother()
  {}

HiggsTo4LeptonsMCObjects::HiggsTo4LeptonsMCObjects(const int             &DecayType,
                                     const HepLorentzVector              &Higgs, 
                                     const std::vector<HepLorentzVector> &Z, 
                                     const std::vector<int>              &ZdecayType, 
                                     const std::vector<HepLorentzVector> &lepton,
                    	             const std::vector<int>              &leptonId,
                                     const std::vector<int>              &leptonMother,
                                     const std::vector<int>              &leptonMotherId,
                                     const std::vector<HepLorentzVector> &gamma,
                                     const std::vector<int>              &gammaMother,
                                     const std::vector<int>              &gammaMotherId,
                                     const std::vector<HepLorentzVector> &IBgamma,
                                     const std::vector<int>              &IBgammaMother) :
    theDecayType( DecayType ),
    theH( Higgs ),
    theZ( Z ),
    theZdecayType( ZdecayType ),
    theL( lepton ),
    theLId( leptonId ),
    theLMother( leptonMother ),
    theLMotherId( leptonMotherId ),
    theG( gamma ),
    theGMother( gammaMother ),
    theGMotherId( gammaMotherId ),
    theIBG( IBgamma ),
    theIBGMother( IBgammaMother )
  {}


  HiggsTo4LeptonsMCObjects::~HiggsTo4LeptonsMCObjects() {}


