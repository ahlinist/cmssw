// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Kikfcodes.h -> CMKIN++ specification of KF code PARAMETER's
// -----------------------------------------------------------------------------
//  Date: 2004/09/30
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Héctor Naves Sordo (Email: naves@mail.cern.ch)
//          (Original kikfcodes.h by Lucas Taylor 15/02/96)
// -----------------------------------------------------------------------------

#ifndef Kikfcodes_h
#define Kikfcodes_h

// Unknown particle
enum Unknown { KF_ZERO = 0 }; // Unknown Particle

// Quarks
enum Quarks { KF_DQ = 1,   // down
	      KF_UQ = 2,   // up
	      KF_SQ = 3,   // strange
	      KF_CQ = 4,   // charm
	      KF_BQ = 5,   // bottom
	      KF_TQ = 6 }; // top

// Leptons
enum Electrons { KF_E   = 11, KF_NUE   = 12 };   // e, nu_e
enum Muons     { KF_MU  = 13, KF_NUMU  = 14 };   // mu,  nu_mu
enum Taus      { KF_TAU = 15, KF_NUTAU = 16 };   // tau, nu_tau

// Bosons
enum Bosons { KF_GLUON  =    21,   // gluon
              KF_GAMMA  =    22,   // photon
              KF_Z      =    23,   // Z0
	      KF_W      =    24,   // W+-
	      KF_H0     =    25 }; // H0 (SM Higgs)

// Light Unflavored Mesons
enum LUMesons { KF_PIP    =   211,   // pi+
	        KF_PI0    =   111,   // pi0
		KF_ETA    =   221,   // eta
		KF_RHO0   =   113,   // rho0(770)
		KF_RHOP   =   213,   // rho+(770)
		KF_A10    = 20113,   // a1^0(1260)
		KF_A1P    = 20213 }; // a1^+(1260)

// Strange Mesons
enum SMesons { KF_KP     =   321,   // K+	       
	       KF_K0     =   311,   // K0
	       KF_K0S    =   310,   // K0_S
	       KF_K0L    =   130,   // K0_L
	       KF_KST0   =   313,   // K*0(892)
	       KF_KSTP   =   323 }; // K*+(892)

// Charmed Mesons
enum CMesons { KF_DP     =   411,   // D+
	       KF_D0     =   421,   // D0
	       KF_DS     =   431 }; // D_S (Charmed Strange Meson)

// Bottom Mesons
enum BMesons { KF_BP     =   521,   // B+
	       KF_B0     =   511,   // B0
	       KF_B0S    =   510,   // B0_S  ??
	       KF_B0L    =   150,   // B0_L  ??
	       KF_BS     =   531,   // B_S
	       KF_BSS    =   530,   // B_S_S ??
	       KF_BSL    =   350 }; // B_S_L ??

// C NU_C Mesons
enum CCMesons { KF_ETAC   =   441,   // eta_c (1S)
		KF_JPSI   =   443,   // J/psi (1S)
		KF_PSIP   = 20443,   // psi   (2S)
		KF_CHIC0  = 10441,   // chi_c0(1P)
		KF_CHIC1  = 10443,   // chi_c1(1P)
		KF_CHIC2  =   445 }; // chi_c2(1P)

#endif

