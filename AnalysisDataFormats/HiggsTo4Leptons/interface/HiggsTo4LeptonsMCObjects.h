#ifndef AnalysisDataFormats_HiggsTo4LeptonsMCObjects_H
#define AnalysisDataFormats_HiggsTo4LeptonsMCObjects_H

/** \class HiggsTo4LeptonsMCObjects
 *
 * Class to store generator MC level information for the H-->ZZ(*)-->2e2mu analysis
 *
 * author: Dominique Fortin - UC Riverside
 *
 * DecayType: 
 * 0:  H -> ZZ  -> 4l
 * 1:  H -> ZZ  -> 4e
 * 2:  H -> ZZ  -> 4mu
 * 3:  H -> ZZ  -> 4tau
 * 4:  H -> ZZ  -> 2e + 2mu
 * 5:  H -> ZZ  -> 2e + 2tau
 * 6:  H -> ZZ  -> 2mu + 2tau
 * 7:  H -> other -> 4l
 * 10: ZZ/gamma -> 4l
 * 11: ZZ/gamma -> 4e
 * 12: ZZ/gamma -> 4mu
 * 13: ZZ/gamma -> 4tau
 * 14: ZZ/gamma -> 2e + 2mu
 * 15: ZZ/gamma -> 2e + 2tau  
 * 16: ZZ/gamma -> 2mu + 2tau 
 * 17: ZZ/gamma -> other -> 4l
 * 20: ttbar -> 4l
 * 21: ttbar -> 4e
 * 22: ttbar -> 4mu
 * 23: ttbar -> 4tau
 * 24: ttbar -> 2e + 2mu
 * 25: ttbar -> 2e + 2tau
 * 26: ttbar -> 2mu + 2tau
 * 27: ttbar -> other -> 4l
 * 30: Zbbar -> 4l
 * 31: Zbbar -> 4e
 * 32: Zbbar -> 2mu
 * 33: Zbbar -> 4tau
 * 34: Zbbar -> 2e + 2mu
 * 35: Zbbar -> 2e + 2 tau
 * 36: Zbbar -> 2mu + 2 tau
 * 37: Zbbar -> other -> 4l
 * 40: other -> 4l
 * 41: other -> 4e
 * 42: other -> 2mu
 * 43: other -> 4tau
 * 44: other -> 2e + 2mu
 * 45: other -> 2e + 2 tau
 * 46: other -> 2mu + 2 tau
 * 47: other -> other -> 4l
 * 
 * ZdecayType:
 * 0: Z -> 2 e
 * 1: Z -> 2 mu
 * 2: Z -> 2 tau
 * 3: Z -> other -> 2l
 */

#include <CLHEP/Vector/LorentzVector.h>
#include <vector>
#include <iosfwd>


//class HepLorentzVector;

class HiggsTo4LeptonsMCObjects 
{
 public:

  HiggsTo4LeptonsMCObjects();
  
  HiggsTo4LeptonsMCObjects(const int                    &DecayType,
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
                    const std::vector<int>              &IBgammaMother);

  ~HiggsTo4LeptonsMCObjects();

  int DecayType() { return theDecayType; }

  HepLorentzVector Higgs() { return theH; }

  std::vector<HepLorentzVector> Z() { return theZ; }
  HepLorentzVector Z(int idx) { return theZ[idx]; }

  std::vector<int> ZdecayType() { return theZdecayType; }
  int ZdecayType(int idx) { return theZdecayType[idx]; }

  std::vector<HepLorentzVector> lepton() { return theL; }
  HepLorentzVector lepton(int idx) { return theL[idx]; }

  std::vector<int> leptonId() { return theLId; }
  int leptonId(int idx) { return theLId[idx]; }

  std::vector<int> leptonMother() { return theLMother; }
  int leptonMother(int idx) { return theLMother[idx]; }

  std::vector<int> leptonMotherId() { return theLMotherId; }
  int leptonMotherId(int idx) { return theLMotherId[idx]; }

  std::vector<HepLorentzVector> gamma() { return theG; }
  HepLorentzVector gamma(int idx) { return theG[idx]; }

  std::vector<int> gammaMother() { return theGMother; }
  int gammaMother(int idx) { return theGMother[idx]; }

  std::vector<int> gammaMotherId() { return theGMotherId; }
  int gammaMotherId(int idx) { return theGMotherId[idx]; }

  std::vector<HepLorentzVector> IBgamma() { return theIBG; }
  HepLorentzVector IBgamma(int idx) { return theIBG[idx]; }

  std::vector<int> IBgammaMother() { return theGMother; }
  int IBgammaMother(int idx) { return theGMother[idx]; }

 private:

  int idx;
  int theDecayType;
  HepLorentzVector theH;
  std::vector<HepLorentzVector> theZ;
  std::vector<int> theZdecayType;
  std::vector<HepLorentzVector> theL;
  std::vector<int> theLId;
  std::vector<int> theLMother;
  std::vector<int> theLMotherId;
  std::vector<HepLorentzVector> theG;
  std::vector<int> theGMother;
  std::vector<int> theGMotherId;
  std::vector<HepLorentzVector> theIBG;
  std::vector<int> theIBGMother;

};

#endif
