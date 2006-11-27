#ifndef PGlobalDigi_h
#define PGlobalDigi_h

#include <vector>
#include <memory>

class PGlobalDigi
{
 public:

  PGlobalDigi(): nEECalDigis(0), nEBCalDigis(0) {}
  virtual ~PGlobalDigi(){}

  // ECal Info
  struct ECalDigi
  {
    ECalDigi(): ADC(0), maxPos(0), AEE(0), SHE(0) {}
    float ADC;
    float maxPos;
    float AEE;
    float SHE;
  };
  typedef std::vector<ECalDigi> ECalDigiVector;
  //put functions
  void putEECalDigis(std::vector<float> adc, std::vector<float> maxpos,
		     std::vector<float> aee, std::vector<float> she);
  void putEBCalDigis(std::vector<float> adc, std::vector<float> maxpos,
		     std::vector<float> aee, std::vector<float> she);
  //get functions
  int getnEECalDigis() {return nEECalDigis;}
  int getnEBCalDigis() {return nEBCalDigis;}  
  ECalDigiVector getEECalDigis() {return EECalDigis;}
  ECalDigiVector getEBCalDigis() {return EBCalDigis;}  

 private:

  // ECal info
  int nEECalDigis;
  ECalDigiVector EECalDigis;
  int nEBCalDigis;
  ECalDigiVector EBCalDigis;

}; // end class declaration

#endif //PGlobalDigiHit_h
