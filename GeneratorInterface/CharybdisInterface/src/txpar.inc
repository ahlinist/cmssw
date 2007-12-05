*......................................................................................*
*......................................................................................*
*.......... GLOBAL parameters for talking to Charybdis                                 *
*                                       Charybdis                                      *
* Parameter      Type      Default      Variable   Description                         *
* -------------------------------------------------------------------------------------*
* IPAR(140)      Int         10000      NCHSRCH    NSearch                             *
* IPAR(141)      Int             2      MSSDEF     Definition of Planck mass           *
* IPAR(142)      Int             7      TOTDIM     Total number of dimensions (6-11)   *
* IPAR(143)      Int             2      NBODY      To how many particles the remnant   *
*                                                  will decay (2-5)                    *
* IPAR(144)      Int             3      MSSDEC     Decay to heavy particles (h,t,W,Z)? *
*                                                  = 1 No                              *
*                                                  = 2 all but higgs                   *
*                                                  = 3 all SM particles                *
*......................................................................................*
* Ipar(145)      Bool        False      GTSCA      Use Giddings Thomas momentum scale? *
* Ipar(146)      Bool        False      TIMVAR     Time variation of BH Hawking        *
*                                                  temperature in decay on/off         *
* Ipar(147)      Bool        False      GRYBDY     Grey-body factors on/off            *
* Ipar(148)      Bool        False      KINCUT     Kinematic cut-off of decay on/off   *
* Ipar(149)      Bool        False      YRCSEC     Use Yoshino-Rychkov factors in      *
*                                                  cross-section                       *
* Ipar(150)      Bool        False      RMBOIL     Remnant decay model: boiling at     *
*                                                  Maximum Hawking temperature         *
*......................................................................................*
* Rpar(141)      Double       1000      MPLNCK     Planck mass (GeV)                   *
* Rpar(142)      Double       5000      MINMSS     Minimum BH mass (GeV)               *
* Rpar(143)      Double      14000      MINMSS     Maximum BH mass (GeV)               *
* Rpar(144)      Double       1000      THWMAX     Maximum Hawking temperature (GeV)   *
* Rpar(145)      Double       1000      RMMINM     Min BH mass, ends boiling (GeV)     *
*......................................................................................*
*......................................................................................*
      Integer           Ipar     ! global interface integer parameters  
      double precision  Rpar     ! global interface real    parameters
      common /TXPAR/Ipar(200), Rpar(200)
      save   /TXPAR/
**********
