#ifndef __CSCTT_PHIWIN_H
#define __CSCTT_PHIWIN_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_PhiWin
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Defines Delta-Phi(Layer_i, Layer_j) filter window LUTs
////////////////////////////////////////////////////////////////

class CSCTT_PhiWin {

public:

CSCTT_PhiWin(float genPt, int from, int to, float dphi);


bool inDphi; 

private:
 
};

#endif

