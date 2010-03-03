#ifndef __CSCTT_STUBLUT_H
#define __CSCTT_STUBLUT_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_StubLUT
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Defines returns z_0 and cot(theta) given a pair of stubs
////////////////////////////////////////////////////////////////

class CSCTT_StubLUT {

public:

CSCTT_StubLUT();

void Cot(int stationfrom,int stationto,int deltaz);

void Zcorr(int stationfrom,int stationto,int deltaz);

static const int cotlookup[7][2000];

static const int zcorrlookup[7][2000];

double cot0x;
double zcorr;

private:
 
};

#endif

