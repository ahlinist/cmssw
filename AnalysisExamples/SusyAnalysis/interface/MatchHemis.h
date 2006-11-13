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
~MatchHemis(){};

// Methods:

int GetMatchedSusyMother1(void); // returns the Susy Mother of hemisphere 1
int GetMatchedSusyMother2(void); // returns the Susy Mother of hemisphere 2
void SetDebug(int debug)  {DEBUGLVL  = debug; } 

private:

bool DoMatch(); // the hemisphere matching algorithm

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

