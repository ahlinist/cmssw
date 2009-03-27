c----------------------------------------------------------------------------c
C PYTHIA ROUTINE should be putted explicitly
      SUBROUTINE UPINIT
 
C...Double precision and integer declarations.
      IMPLICIT double precision(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
 
C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      double precision EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
      SAVE /HEPRUP/
      EXTERNAL TXDATA
      EXTERNAL SPLINT
      Integer           Ipar     ! global TopRex integer parameters
      double precision  Rpar     ! global TopRex real    parameters
      common /TXPAR/Ipar(200), Rpar(200)

      RETURN
      END

*----------------------------------------------------------------------------*
*...UPEVNT PYTHIA                                                            * 
*............................................................................*

      subroutine UPEVNT
      implicit none
       print*,'TX_UPEVENT ===================== '
       call TX_UPEVNT
      return
      end
*----------------------------------------------------------------------------*
c PYTHIA
 
      SUBROUTINE UPVETO(IVETO)
      implicit none
      integer IVETO 
      call TX_UPVETO(IVETO)
      return
      end


*----------------------------------------------------------------------------*


      BLOCK DATA TXDATA
 
C...double precision and integer declarations.
      IMPLICIT NONE 
*............................................................................*
*..... GLOBAL     TopReX    parameters                                       *
*. . . . . .  INTEGER parametrs . . . . . . . . . . . . . . . . . . . . . .  *
*..Ipar(1)      ! internal TopReX code of the process                        *
*  -1 : PYTHIA process                                                       *
*  Ipar(1) =  1  :  EW top : W g(2->2)                                       *
*  Ipar(1) =  2  :  EW top : W g(2->3)                                       *
*  Ipar(1) =  3  :  EW top : t W                                             *
*  Ipar(1) =  4  :  EW top : W* -> t bbar                                    *
*  Ipar(1) =  5  :  W Q Qbar                                                 *
*  Ipar(1) =  6  :  W+-  -> tau nu                                           *
*  Ipar(1) =  7  :  W+- (-> tau nu) JET                                      *
*  Ipar(1) =  8  :  H+-  -> t bbar                                           *
*  Ipar(1) =  9  :  H+-  -> tau nu                                           *
*  Ipar(1) = 10  :  H+- (-> tau nu) JET                                      *
*  Ipar(1) = 11  :  EW top : W g(2->2) and W g(2->3)                         *
*  Ipar(1) = 20  :  gg+qq~ -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 21  :  gg     -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 22  :     qq~ -> tt~,  'on-shel ',  S. Slabospitsky             *
*  Ipar(1) = 31  :  q q~   -> H+ H-                                          *
*  Ipar(1) = 50  :  gg+qq~ -> tt~,  'on-shel ',  W. Bernreuther et al        *
*  Ipar(1) = 51  :  gg     -> tt~,  'on-shel ',  W. Bernreuther et al        *
*  Ipar(1) = 52  :     qq~ -> tt~,  'on-shel ',  W. Bernreuther et al        *
*............................................................................*
*................                                                            *
*  Ipar(2)      : num. of current event                                      *
*..Ipar(3)      : evolution scale type for PDF                               *
*..Ipar(4)      : evolution scale type for alpha_S                           *
*  Ipar(5)      : 0(1) quark masses are taken from PYTHIA(PDG)               *
*  Ipar(6)      : 0(1) s,c,b,t mass value are QCD-evaluated(fixed)           *
*               : Ipar(6) is used only for evaluation of H+- couplings       *
*..Ipar(7)      : KF code of Q-quark for WQQbar                              *
*..Ipar(8)      : INIT flag                                                  *
*..Ipar(9)      : continuation flag                                          *
*..Ipar(10)     : 1(0) (no)print  sigma for each q, qbar contribution        *
*..Ipar(11)     : 0/1 to NOT/print output from single top production         *
*..Ipar(12)     : 0/1 print information about hard process                   *
*..Ipar(16)     : flag for first/second order in alpha-s evaluation          *
*..Ipar(17)     : check max. diff. X-section, during run                     *
*      D=0 : always check, 1 : events with diff.sig. > sigmax NOT accepted   *
*..Ipar(18)     : 1/0 evaluate PDF uncertainty                               *
*..Ipar(21)     : W-exchange: > 0 tb, <0 tau nu, 1 W, 2 H, 3 (W+H+Int)       *
*..Ipar(22)     : 0: gg + qqbar + g q, 1: gg,   2: q qbar, 3: g q            *
*..Ipar(23)     : Zqq, 0: gam* + Z,   1: gam*, 2: Z                          *
*..Ipar(24)     : gg->ZZ, 0: SM box, 1: box + Higgs, 2: Higgs                *
*..Ipar(25)     : number etimates X-sections for Ipar(1) = 11    (D=1000)    *
*..Ipar(26)     : bW 0 SM + FCNC, 1 - SM, 2 - FCNC                           *
*  Ipar(31)     : 1(0) running (fixed) top-mass according top width          *
*..Ipar(32)     : 1/0 finite/zero width of the W-boson in decay chain        *
*  ipar(33)     : 1(0) take (not) into account spin of produced top quarks   *
*..Ipar(41)     : number of the particles in the hard process                *
*..Ipar(42)     : sig*10**(Ipar(42))                                         *
*  ipar(50)     : number of current color                                    *
*..Ipar(51)      : num. of events used for estimation of SigMax               *
*..ipar(52)      : num. of events used for estimation of M**2                 *
*..Ipar(53)     : num. of MC entries  for estimates  of H+- and T decays     *
*..Ipar(100)    : current process number = Ipar(1)                           *
*..Ipar(101)    : current process number in /HEPRUP/ list, i.e. 1,2,3..NPRUP *
*..Ipar(120)    : final leptons for ADD:                                     *
*. . . . . .  double precision parameters . . . . . . . . . . . . . . . . .  *
*..Rpar(1)      : Ecm                                                        *
*..Rpar(2)      : S = Ecm**2                                                 *
*..Rpar(3)      : Evolution scale for PDF                                    *
*..Rpar(4)      : Evolution scale for alpha_s                                *
*..Rpar(5)      : parameter for Q-scale evaluation for PDF                   *
*..Rpar(6)      : parameter for Q-scale evaluation for alpha_s               *
*
*.............. couplings ...................................................*
*..Rpar(10)     : 0.38939d0     transformation 1/GeV**2 to mb                *
*..Rpar(11)     : alpha_QED = (1/128) is electromagnetic coupling            *
*..Rpar(12)     : 4.*pi*rpar(11)  = (electric charge)**2                     *
*..Rpar(13)     : sqrt(4.*pi*rpar(11)) is electric charge                    *
*..Rpar(14)     : G_Fermi                                                    *
*..Rpar(15)     : alpha_QCD = QCD coupling used for event                    *
*..Rpar(16)     : 4*pi * Rpar(15) = (QCD charge)**2                          *
*..Rpar(17)     : sqrt(4*pi * Rpar(15)) = QCD charge                         *
*............................                                                *
*..Rpar(21)     : sin(theta_W)                                               *
*..Rpar(22)     : cos(theta_W)                                               *
*..Rpar(23)     : sin(theta_W)**2                                            *
*..Rpar(24)     : cos(theta_W)**2                                            *
*..Rpar(25)     : sin(2*theta_W) = 2 * sin(theta_W) * cos(theta_W)           *
*..rpar(26)     : g = g_weak  = e / sin(theta_W)                             *
*..Rpar(27)     : g_z         = e / sin(2*theta_W)                           *
*..Rpar(28)     : v vacuum expectation value * 246/sqrtr(2) GeV              *
*.............. top quark parameters ........................................*
*..Rpar(30)     : width of t-> d  W decay at tree level (SM case)            *
*..Rpar(31)     : width of t-> s  W decay at tree level (SM case)            *
*..Rpar(32)     : width of t-> b  W decay at tree level (SM case)            *
*..Rpar(33)     : width of t-> b4 W decay at tree level (SM case)            *
*..Rpar(34)     : total t-quark decay width                                  *
*..Rpar(35)     : width of t -> b H+- decay channel                          *
*..Rpar(36)     : M(top) * width(total top)                                  *
*..rpar(37)     : current value of M(top) generated with B.W.                *
*.............. W-boson and Z-boson parameters  .............................*
*..Rpar(41)     : W-boson mass                                               *
*..Rpar(42)     : Rpar(41)**2                                                *
*..Rpar(43)     : W-boson width                                              *
*..Rpar(44)     : M(W) * Width(W)                                            *
*...                                                                         *
*..Rpar(45)     : Z-boson mass                                               *
*..Rpar(46)     : Rpar(45)**2                                                *
*..Rpar(47)     : Z-boson width                                              *
*..Rpar(48)     : M(Z) * Width(Z)                                            *
*............................................................................*
*..Rpar(50)     : New Physics scale (1 TeV), FCNC                            *
*.............. charged Higgs parameters  ...................................*
*..Rpar(51)     : charged Higgs mass                                         *
*..Rpar(52)     : Rpar(51)**2                                                *
*..Rpar(53)     : total charged Higgs decay width                            *
*..Rpar(54)     : M(H+-) * Width(H+-)                                        *
*..Rpar(55)     : tan(beta)                                                  *
*..Rpar(56)     : (1/tan(beta))                                              *
*............................................................................*
*..Rpar(57)     : neutral Higgs (SM) mass                                    *
*..Rpar(58)     : total Higgs decay width                                    *
*..Rpar(59)     : M(H) * Width(H)                                            *
*............................................................................*
*..Rpar(61:63) are for processes Ipar(1) = 50                                *
*..Rpar(61)    : gVV, relative coupling of Higgs boson(H0) to W and Z bosons *
*..Rpar(62)    : a, scalar Yukawa coupling of Higgs boson(H0) to top quarks  *
*..Rpar(63)    : a~, pseudoscalar Yukawa coupling of H0 boson to top quarks  *
*............................................................................* 
*..Rpar(64)    : total X-section for the process, evaluated for each event   *
*..Rpar(65)    : differential cross section (in PB) for current event        *
*..Rpar(67)    : 2.d0,  PTmin(Z), IPAR(1) = 24, ZZ-production                *
*..Rpar(68)    : 20.d0, PTmin(q)    ==> Rpar(81), IPAR(1) = 11, T-channel    *
*..Rpar(69)    : 10.d0, PTmin(Bbar) ==> Rpar(82), IPAR(1) = 11, T-channel    *
*..Rpar(70)    : MW_min  for final W*/Z*                                     *
*............................................................................* 
*..Rpar(71)    : Shat       for hard process                                 *
*..Rpar(72)    : That       for hard process                                 *
*..Rpar(73)    : Uhat       for hard process                                 *
*..Rpar(74)    : sqrt(Shat) for hard process                                 *
*..Rpar(75)    : PThat      for hard process                                 *
*..Rpar(76)    : PThat**2   for hard process                                 *
*..Rpar(77)    : M(QQbar)_MIN  for W/Z Q Qbar production                     *
*..Rpar(78)    : M(QQbar)_MAX  for W/Z Q Qbar production                     *
*..Rpar(79)    : M(ffbar)_MIN  for gamma* or Z*                              *
*..Rpar(80)    : M(ffbar)_MAX  for gamma* or Z*                              *
*..Rpar(81)    : kinematic cut,  >=3 final state, PTmin(q) for txpro(11)     *
*..Rpar(82)    : kinematic cut,  >=3 final state, PTmin(Bbar) for txpro(11)  *
*..Rpar(83:99) : kinematic cuts, >=3 final state                             *
*............................................................................* 
*..Rpar(101)   : CKIN(1) = sqrt(Shat_MIN) for hard process                   *
*..Rpar(102)   : CKIN(2) = sqrt(Shat_MAX) for hard process                   *
*..Rpar(103)   : CKIN(3) = PThat_MIN                                         *
*..Rpar(104)   : CKIN(4) = PThat_MAX                                         *
*............................................................................*
*...........  Graviton* production,  ADD scenario                            *
*..Rpar(121)     : M_exdim,                         ADD scenario             *
*..Rpar(122)     : n_exdim,  Graviton* production,  ADD scenario             *
*..Rpar(123)     : mKK mass of KK-excitation,       ADD scenario             *
*..Rpar(124)     : recommended PThat_MIN            ADD scenario             *
*..Rpar(125)     : factor_ADD                                                *
*............................................................................*
*...........  Black Hole (BH) production                                     *
*  Rpar(131)     : Planck scale, GeV                                         *
*  Rpar(132)     : n extra dimensions                                        *
*  Rpar(133)     : BR(BH -> q qbar X)                                        *
*  Rpar(134)     : BR(BH -> g g    X) / 2                                    *
*  Rpar(135)     : BR(BH -> l l'   X)                                        *
*  Rpar(136)     : BR(BH -> gamma,W,Z X) + BR(BH -> Higgs  X)                *
*  Rpar(137)     : BR(BH -> Higgs  X)                                        *
*............................................................................*
      double precision         Rpar     ! global TopReX real    parameters
      Integer       Ipar                ! global TopReX integer parameters  
      common /TXPAR/Ipar(200), Rpar(200)
      save   /TXPAR/
*.......
      integer ndch   ! maximum number of decay channels
      parameter (ndch=50)
      integer        MID(0:ndch,4),             FID(4,ndch,5) 
      double precision   wid(0:ndch,4),brf(ndch,4),     BRS(2,4)
      common /TXdec/ MID, WID,         BRF,     FID,    brs
      save   /TXdec/
*
      integer  maxpro 
      parameter (maxpro=500)  ! max.num. of TopReX process
*
      integer         txmup, txsel, txpro,         txext
      common /trxpro/ txmup, txsel, txpro(maxpro), txext(maxpro)
      save   /trxpro/
*
      integer dectop(ndch), dechpm(ndch) 
      common /TXdecRE/dectop, dechpm 
      double complex ei,e1,e0
      common /cunits/ e0, e1, ei
      data e0/(0.d0,0.d0)/,e1/(1.d0,0.d0)/,ei/(0.d0,1.d0)/
      save /cunits/
*...........
      data Ipar/0,   !  1: internal TopReX code of the process
     &          0,   !  2: num. of current event
     &          1,   !  3: default Q-scale type for PDF 
     &          0,   !  4: Q-scale for alpha_s <== Q-scale(PDF)
     &          0,   !  5:  0(1) quark masses are taken from PYTHIA(PDG)
     &          0,   !  6:  0(1) s,c,b,t mass value are QCD-evaluated(fixed)
*                    Ipar(6) is used only for evaluation of H+- couplings
     &          5,   !  7: KF code of Q-quark for W Q Qbar production
     &          0,   !  8:  INIT flag 
     &          1,   !  9: continuation flag
     &          0,   ! 10: no print X-sections for each q qbar contr
     &          0,   ! 11:  0/1 to NOT/print output from single top production
     &          0,   ! 12: 0/1 print information about hard process
     &          0,   ! 13: free
     &          0,   ! 14: free
     &          0,   ! 15: free
     &          2,   ! 16: first/second order in alpha-s evaluation (PYTHIA)
     &          0,   ! 17: check max. diff. X-section, during run 
*        D=0 : always check, 1 : events with diff.sig. > sigmax NOT accepted 
     &          0,   ! 18: free
     &          0,   ! 19: free
     &          0,   ! 20: free
     &          0,   ! 21: W-exc.: > 0 tb, <0 tau nu, 1 W, 2 H, 3 (W+H+Int)
     &          0,   ! 22: Zqq, 0: gg + qqbar, 1: gg,   2: qqbar 
     &          0,   ! 23: Zqq, 0: gam* + Z,   1: gam*, 2: Z  
     &          0,   ! 24: gg->ZZ, 0: SM box, 1: box + Hiigs, 2: Higgs   
     &          0,   ! 25: free
     &          0,   ! 26: bW: 0 SM + FCNC, 1 - SM, 2 - FCNC
     &        4*0,   ! 27:30 free
     &          1,   ! 31: running (0 - fixed) top-mass according top width 
     &          1,   ! 32: 1/0 finite/zero width of W-boson in decay chain
     &          1,   ! 33: take(0:not) into account spin of produced top quark
     &        7*0,   ! 34-40 not used
     &          0,   ! 41: number of the particles in the hard process 
     &          9,   ! 42: sig*10**(Ipar(42))    
     &          0,   ! 43: free
     &          0,   ! 44: free 
     &          0,   ! 45: free 
     &          0,   ! 46: free 
     &          0,   ! 47: free 
     &          0,   ! 48: free 
     &          0,   ! 49: free 
     &          0,   ! 50: current color (see colorf_xxx)  
     &     100000,   ! 51: num. of MC calls to estimate SigMax
     &     100000,   ! 52: num. of MC calls to estimate ME**2
     &      10000,   ! 53: num. of MC calls to estimate of H+- and T width
     &          0,   ! 54: free 
     &          0,   ! 55: free 
     &          0,   ! 56: free 
     &          0,   ! 57: free 
     &          0,   ! 58: free 
     &          0,   ! 59: free 
     &          0,   ! 60: free 
     &      140*0/   ! 42 - 200 not used
*... double precision parameters
      data Rpar/0.d0, !  1: Ecm
     &          0.d0, !  2: S = Ecm**2                                   
     &        100.d0, !  3: Evolution scale for PDF 
     &          0.d0, !  4: Evolution scale for alpha_s 
     &          0.d0, !  5: parameter for Q-scale evaluation for PDF 
     &          0.d0, !  6: parameter for Q-scale evaluation for alpha_s 
     &        3*0.d0, !  7 - 9: not used
* almost all these parameters are evaluated or readed in TopReX
     &     0.38939d0, ! 10: transformation 1/GeV**2 to mb
     &          0.d0, ! 11: alpha_QED = (1/128) is electromagnetic coupling
     &          0.d0, ! 12:      4.*pi*rpar(11) = (electric charge)**2   
     &          0.d0, ! 13: sqrt(4.*pi*rpar(11)) is electric charge   
     &          0.d0, ! 14: G_Fermi
     &          0.d0, ! 15: alpha_QCD = coupling used for event
     &          0.d0, ! 16: 4*pi * Rpar(15) = (QCD charge)**2
     &          0.d0, ! 17: sqrt(4*pi * Rpar(15)) = QCD charge
     &        3*0.d0, ! 18-20: not used 
     &        4*0.d0, ! 21-24 sin(theta_W), cos(cos(theta_W)), sin^2, cos^2  
     &          0.d0, ! 25: sin(2*theta_W) = 2 * sin(theta_W) * cos(theta_W)
     &          0.d0, ! 26: g = g_weak  = e / sin(theta_W)                  
     &          0.d0, ! 27: g_z         = e / sin(2*theta_W)                
     &          0.d0, ! 28: v is vacumm expectation                
     &          0.d0, ! 28-29: not used
     &          0.d0, ! 30: width of t-> d  W decay at tree level (SM case)
     &          0.d0, ! 31: width of t-> s  W decay at tree level (SM case)
     &          0.d0, ! 32: width of t-> b  W decay at tree level (SM case)
     &          0.d0, ! 33: width of t-> b4 W decay at tree level (SM case)
     &          0.d0, ! 34: total t-quark decay width
     &          0.d0, ! 35: width of t -> b H+- decay channel 
     &          0.d0, ! 36: M(top) * width(total)_top
     &          0.d0, ! 37: current value of M(top) generated with B.W.       
     &        3*0.d0, ! 38-40: not used
     &        8*0.d0, ! 41=MW, 42=MW**2, 43=Width(W), 44= MW * Width(W)   
*                     ! 45=MZ, 46=MZ**2, 47=Width(Z), 48= MZ * Width(Z)   
     &          0.d0, ! 49- not used
     &       1000.d0, ! 50: New Physics scale (1 TeV) 
     &        150.d0, ! 51: charged Higgs mass, default value
     &          0.d0, ! 52: M(H+-)**2
     &          0.d0, ! 53: Width(H+-)
     &          0.d0, ! 54: M(H+-) * Width(H+-)
     &         0.3d0, ! 55: tan(beta)
     &          0.d0, ! 56: (1/tan(beta))
     &        115.d0, ! 57: neutral Higgs mass, default value
     &          0.d0, ! 58: Width(H)
     &          0.d0, ! 59: M(H) * Width(H)
     &          0.d0, ! 60 - not used
     &        3*0.d0, ! 61 - 63: for processes Ipar(1) = 50, 51, 52 only 
     &          0.d0, ! 64: total X-section (process), evaluated for event
     &          0.d0, ! 65: diff. X-section (in PB) for current event
     &          0.d0, ! 66 - not used
     &          2.d0, ! 67 PT(Z)min for ZZ-production, Ipar(1) = 24
     &         20.d0, ! 68: PTmin(q)    ==> Rpar(81), IPAR(1) = 11, T-channel 
     &         10.d0, ! 69: PTmin(Bbar) ==> Rpar(82), IPAR(1) = 11, T-channel 
     &         15.d0, ! 70: min value for W*/Z* decay
*  hard process kinematics:  2 -> 2
     &        6*0.d0, !71-76: Shat, That, Uhat, sqrt(Shat), PThat, PTHAT**2
     &         -1.d0, !77: M(QQ)MIN, WQQbar, 
     &         -1.d0, !78: M(QQ)MAX  WQQbar, if(eq.-1) => from LIPS 
     &         -1.d0, !79: M(ff)MIN, ZQQbar 
     &         -1.d0, !80: M(ff)MAX, ZQQbar, if(eq.-1) => from LIPS 
     &       19*0.d0, !81-99: basic  kinematic cuts
     &          0.d0, !100 - not used
     &         20.d0, !101: CKIN(1) = sqrt(Shat_MIN) for hard process
     &          0.d0, !102: CKIN(2) = sqrt(Shat_MAX) for hard process
     &          0.d0, !103: CKIN(3) = PThat_MIN  
     &          0.d0, !104: CKIN(4) = PThat_MAX
     &       96*0.d0/ !111 - 200: not used
*............................................................................*
      data MID/    18, !  0,   1: number of top-quark decay channels
     &              1, !  1,   1: SM decay channel, t -> d  W   
     &              1, !  2,   1: SM decay channel, t -> s  W   
     &              1, !  3,   1: SM decay channel, t -> b  W   
     &             -1, !  4,   1: SM decay channel, t -> b4 W   
     &             -1, !  5,   1: t -> b H+ 
     &             -1, !  6,   1: t -> b H+, H+ -> t* bbar
     &             -1, !  7,   1: is not used
     &             -1, !  8,   1: is not used
     &             -1, !  9,   1: is not used
     &             -1, ! 10,   1: is not used
     &             -1, ! 11,   1: FCNC:  t gluon  Up
     &             -1, ! 12,   1: FCNC:  t gluon  Charm
     &             -1, ! 13,   1: FCNC:  t photon Up
     &             -1, ! 14,   1: FCNC:  t photon Ccharm
     &             -1, ! 15,   1: FCNC:  t Z      Up    :  gamma^mu
     &             -1, ! 16,   1: FCNC:  t Z      Charm :  gamma^mu
     &             -1, ! 17,   1: FCNC:  t Z      Up    :  sigma^mu^nu
     &             -1, ! 18,   1: FCNC:  t Z      Charm :  sigma^mu^nu
     &         32*0 , ! 19-50,1: is not used
*       W-boson
     &              0, !  0,   2: number of W-boson decay channels
     &         ndch*0, ! 1-50, 2: will be readed from PYTHIA
*   charged Higgs decay parameters
     &             19, !  0,   3: number of H+- decay channels
     &              1, !  1,   3: d U 
     &              1, !  2,   3: d C 
     &             -1, !  3,   3: d T
     &             -1, !  4,   3: d Tprime 
     &              1, !  5,   3: s U 
     &              1, !  6,   3: s C
     &             -1, !  7,   3: s T 
     &             -1, !  8,   3: s Tprime
     &              1, !  9,   3: b U
     &              1, ! 10,   3: b C
     &             -1, ! 11,   3: b T
     &             -1, ! 12,   3: b Tprime
     &             -1, ! 13,   3: bprime U
     &             -1, ! 14,   3: bprime C
     &             -1, ! 15,   3: bprime T
     &             -1, ! 16,   3: bprime Tprime
     &             -1, ! 17,   3: e   nue
     &              1, ! 18,   3: mu  numu
     &              1, ! 19,   3: tau nutau
     &         31*-1,! 20-50 3: is not used
*       Z-boson
     &              0, !  0,   4: number of W-boson decay channels
     &         ndch*0/ ! 1-50, 4: will be readed from PYTHIA

*
      data txmup /0/, txsel/0/,  txpro/500*0/, txext/500*0/
      data dectop/
     &              1, !  1,   1: SM decay channel, t -> d  W   
     &              1, !  2,   1: SM decay channel, t -> s  W   
     &              1, !  3,   1: SM decay channel, t -> b  W   
     &             -1, !  4,   1: SM decay channel, t -> b4 W   
     &             -1, !  5,   1: t -> b H+ 
     &             -1, !  6,   1: t -> b H+, H+ -> t* bbar
     &             -1, !  7,   1: is not used
     &             -1, !  8,   1: is not used
     &             -1, !  9,   1: is not used
     &             -1, ! 10,   1: is not used
     &             -1, ! 11,   1: FCNC:  t gluon  Up
     &             -1, ! 12,   1: FCNC:  t gluon  Charm
     &             -1, ! 13,   1: FCNC:  t photon Up
     &             -1, ! 14,   1: FCNC:  t photon Ccharm
     &             -1, ! 15,   1: FCNC:  t Z      Up    :  gamma^mu
     &             -1, ! 16,   1: FCNC:  t Z      Charm :  gamma^mu
     &             -1, ! 17,   1: FCNC:  t Z      Up    :  sigma^mu^nu
     &             -1, ! 18,   1: FCNC:  t Z      Charm :  sigma^mu^nu
     &         32*0 /  ! 19-50,1: is not used

      data  dechpm /
     &              1, !  1,   3: d U 
     &              1, !  2,   3: d C 
     &             -1, !  3,   3: d T
     &             -1, !  4,   3: d Tprime 
     &              1, !  5,   3: s U 
     &              1, !  6,   3: s C
     &             -1, !  7,   3: s T 
     &             -1, !  8,   3: s Tprime
     &              1, !  9,   3: b U
     &              1, ! 10,   3: b C
     &             -1, ! 11,   3: b T
     &             -1, ! 12,   3: b Tprime
     &             -1, ! 13,   3: bprime U
     &             -1, ! 14,   3: bprime C
     &             -1, ! 15,   3: bprime T
     &             -1, ! 16,   3: bprime Tprime
     &             -1, ! 17,   3: e   nue
     &              1, ! 18,   3: mu  numu
     &              1, ! 19,   3: tau nutau
     &         31*-1 / ! 20-50 3: is not used
      END
*----------------------------------------------------------------------------*
