#ifndef PGlobalDigi_h
#define PGlobalDigi_h

#include <vector>
#include <memory>

class PGlobalDigi
{
 public:

  PGlobalDigi(): nEBCalDigis(0), nEECalDigis(0) {}
  virtual ~PGlobalDigi(){}

  // ECal Info
  struct ECalDigi
  {
    ECalDigi(): maxPos(0), AEE(0), SHE(0) {}
    int maxPos;
    double AEE; //analog equivalent energy
    float SHE; //simhit energy
  };
  typedef std::vector<ECalDigi> ECalDigiVector;
  //put functions
  void putEBCalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  void putEECalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  //get functions
  int getnEBCalDigis() {return nEBCalDigis;}  
  int getnEECalDigis() {return nEECalDigis;}
  ECalDigiVector getEBCalDigis() {return EBCalDigis;}  
  ECalDigiVector getEECalDigis() {return EECalDigis;}

 private:

  // ECal info
  int nEBCalDigis;
  ECalDigiVector EBCalDigis;
  int nEECalDigis;
  ECalDigiVector EECalDigis;

}; // end class declaration

#endif //PGlobalDigiHit_h
