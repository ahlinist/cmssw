#ifndef PGlobalRecHit_h
#define PGlobalRecHit_h

#include <vector>
#include <memory>

class PGlobalRecHit
{
 public:

  PGlobalRecHit(): nEECalRecHits(0), nEBCalRecHits(0) {}
  virtual ~PGlobalRecHit(){}

  // ECal Info
  struct ECalRecHit
  {
    ECalRecHit(): ADC(0), maxPos(0), AEE(0), SHE(0) {}
    float ADC;
    float maxPos;
    float AEE;
    float SHE;
  };
  typedef std::vector<ECalRecHit> ECalRecHitVector;
  //put functions
  void putEECalRecHits(std::vector<float> adc, std::vector<float> maxpos,
		     std::vector<float> aee, std::vector<float> she);
  void putEBCalRecHits(std::vector<float> adc, std::vector<float> maxpos,
		     std::vector<float> aee, std::vector<float> she);
  //get functions
  int getnEECalRecHits() {return nEECalRecHits;}
  int getnEBCalRecHits() {return nEBCalRecHits;}  
  ECalRecHitVector getEECalRecHits() {return EECalRecHits;}
  ECalRecHitVector getEBCalRecHits() {return EBCalRecHits;}  

 private:

  // ECal info
  int nEECalRecHits;
  ECalRecHitVector EECalRecHits;
  int nEBCalRecHits;
  ECalRecHitVector EBCalRecHits;

}; // end class declaration

#endif //PGlobalRecHitHit_h
