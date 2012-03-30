
#ifndef __HepevtBlock__
#define __HepevtBlock__

#include <iostream>
#include <vector>
#include "CLHEP/Vector/LorentzVector.h"

class HepevtParticle
{
  friend std::istream& operator>>(std::istream&, HepevtParticle&);
  friend std::ostream& operator<<(std::ostream&, const HepevtParticle&);
public:
  HepevtParticle();
  CLHEP::HepLorentzVector getFourVector() const;

public:
  int isthep, idhep;
  std::vector<int> jmohep, jdahep; // each length 2
  std::vector<double> phep;  // length 5
  std::vector<double> vhep;  // length 4
};




class HepevtEvent
{
  friend std::istream& operator>>(std::istream&, HepevtEvent&);
  friend std::ostream& operator<<(std::ostream&, const HepevtEvent&);
public:
  HepevtEvent();
public:
  int nevhep;
  std::vector<HepevtParticle> particles;
};


#endif
