#ifndef __CSCTT_MYSTUB_H
#define __CSCTT_MYSTUB_H (1)


////////////////////////////////////////////////////////////////
// Class: CSCTT_MyStub
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Re-defines a stub used in CSCTT algorithm 
////////////////////////////////////////////////////////////////

#include <math.h>

class CSCTT_MyStub {

public:

CSCTT_MyStub(double ax, double ay, double az, double aeta, double aphi, int astNo, bool ainDz, bool ainDphi);

//float Cot();

double x,y,z,r,phi,eta,cot;
int layer;
bool inDz; 
bool inDphi; 
void SetInDz(bool isInDz) {inDz = isInDz;}
void SetInDphi(bool isInDphi) {inDphi = isInDphi;}

private:
 
};

#endif

