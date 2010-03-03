#ifndef __CSCTT_CSCMATCHLOOKUPTABLE_H
#define __CSCTT_CSCMATCHLOOKUPTABLE_H (1)

////////////////////////////////////////////////////////////////
// Class: CSCTT_CSCMatchLookupTable
// Authors: Ivan Furic, Bobby Scurlock, University of Florida
// Description: Defines the CSC-to-Tracker matching window LUTs
////////////////////////////////////////////////////////////////


class CSCTT_CSCMatchLookupTable {

public:

CSCTT_CSCMatchLookupTable();

void PhiBend(int charge,int station,int pt, int eta);
void Z(int charge,int station,int eta);

static const double p_phiBendMean1[5][33];
static const double p_phiBendSigma1[5][33];

static const double p_phiBendMean2[5][33];
static const double p_phiBendSigma2[5][33];

static const double p_phiBendMean3[5][33];
static const double p_phiBendSigma3[5][33];

static const double n_phiBendMean1[5][33];
static const double n_phiBendSigma1[5][33];

static const double n_phiBendMean2[5][33];
static const double n_phiBendSigma2[5][33];

static const double n_phiBendMean3[5][33];
static const double n_phiBendSigma3[5][33];

static const double p_zBendMean[5][32];
static const double p_zBendSigma[5][32];

static const double n_zBendMean[5][32];
static const double n_zBendSigma[5][32];

double phiMean;
double phiSigma;

double zMean;
double zSigma;

private:

 
};

#endif

