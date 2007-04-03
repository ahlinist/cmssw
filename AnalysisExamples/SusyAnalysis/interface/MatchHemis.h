#ifndef MatchHemis_h
#define MatchHemis_h


/*  \class MatchHemis
*
*  Class which matches the reconstructed Hemispheres to the MC truth
*
*  Authors: Luc Pape & Filip Moortgat      Date: October 2006
*                                          Updated: 
*
*/


#include "MrParticle.h" 
#include <vector>
#include <iostream>
#include <cmath>

class MatchHemis {

public:

// Constructor:
MatchHemis(vector<MrParticle*>&, vector<MrParticle*>&, 
           vector<float>&, vector<float>&);

// Destructor:
virtual ~MatchHemis(){};

// Methods:

virtual int GetMatchedSusyMother1(void); // returns the Susy Mother of hemisphere 1
virtual int GetMatchedSusyMother2(void); // returns the Susy Mother of hemisphere 2
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

private:

virtual bool DoMatch(); // the hemisphere matching algorithm

std::vector<MrParticle*>& RecoData;
std::vector<MrParticle*>& MCData;
vector<float>& vA1; 
vector<float>& vA2;

int DEBUGLVL;

int status;
bool isMatchedOK;
int firstsusymother[2];

};

#endif

