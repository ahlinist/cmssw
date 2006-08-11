 
C*********************************************************************
 
C...PYEVWT
C...Dummy routine, which the user can replace in order to multiply the
C...standard PYTHIA differential cross-section by a process- and
C...kinematics-dependent factor WTXS. For MSTP(142)=1 this corresponds
C...to generation of weighted events, with weight 1/WTXS, while for
C...MSTP(142)=2 it corresponds to a modification of the underlying
C...physics.
 
      SUBROUTINE PYEVWT(WTXS)
 
C...Double precision and integer declarations.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYINT1/MINT(400),VINT(400)
      COMMON/PYINT2/ISET(500),KFPR(500,2),COEF(500,20),ICOL(40,4,2)
C      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      SAVE /PYDAT1/,/PYINT1/,/PYINT2/
 
C...Set default weight for WTXS.

C      write(*,*) 'Reweighting event ...'
      WTXS=1D0
 
C...Read out subprocess number.
      ISUB=MINT(1)
      ISTSB=ISET(ISUB)
 
C...Read out tau, y*, cos(theta), tau' (where defined, else =0).
      TAU=VINT(21)
      YST=VINT(22)
      CTH=0D0
      IF(ISTSB.EQ.2.OR.ISTSB.EQ.4) CTH=VINT(23)
      TAUP=0D0
      IF(ISTSB.GE.3.AND.ISTSB.LE.5) TAUP=VINT(26)
 
C...Read out x_1, x_2, x_F, shat, that, uhat, p_T^2.
      X1=VINT(41)
      X2=VINT(42)
      XF=X1-X2
      SHAT=VINT(44)
      THAT=VINT(45)
      UHAT=VINT(46)
      PT2=VINT(48)

      PTHAT = SQRT(PT2)  
 
C...Weights for QCD dijet sample

      IF (ISUB.EQ.11.OR.ISUB.EQ.68.OR.ISUB.EQ.28.OR.ISUB.EQ.53) THEN 

       IF(PTHAT.GE.0.AND.PTHAT.LT.15) WTXS = 0.025 
       IF(PTHAT.GE.15.AND.PTHAT.LT.20) WTXS = 1.8405931
       IF(PTHAT.GE.20.AND.PTHAT.LT.30) WTXS = 8.60482502 
       IF(PTHAT.GE.30.AND.PTHAT.LT.50) WTXS = 35.4135551 
       IF(PTHAT.GE.50.AND.PTHAT.LT.80) WTXS = 263.720733 
       IF(PTHAT.GE.80.AND.PTHAT.LT.120) WTXS = 936.023193
       IF(PTHAT.GE.120.AND.PTHAT.LT.170) WTXS = 5525.80176
       IF(PTHAT.GE.170.AND.PTHAT.LT.230) WTXS = 27337.9121
       IF(PTHAT.GE.230.AND.PTHAT.LT.300) WTXS = 115738.633
       IF(PTHAT.GE.300.AND.PTHAT.LT.380) WTXS = 432008.344
       IF(PTHAT.GE.380.AND.PTHAT.LT.470) WTXS = 1461105.62
       IF(PTHAT.GE.470.AND.PTHAT.LT.600) WTXS = 3999869.75
       IF(PTHAT.GE.600.AND.PTHAT.LT.800) WTXS = 8180579.5
       IF(PTHAT.GE.800.AND.PTHAT.LT.1000) WTXS = 46357008.
       IF(PTHAT.GE.1000.AND.PTHAT.LT.1400) WTXS = 152645456.
       IF(PTHAT.GE.1400.AND.PTHAT.LT.1800) WTXS = 1.56872026D9
       IF(PTHAT.GE.1800.AND.PTHAT.LT.2200) WTXS = 1.14387118D10
       IF(PTHAT.GE.2200.AND.PTHAT.LT.2600) WTXS = 6.9543682D10
       IF(PTHAT.GE.2600.AND.PTHAT.LT.3000) WTXS = 3.86604466D11
       IF(PTHAT.GE.3000.AND.PTHAT.LT.3500) WTXS = 1.96279625D12
       IF(PTHAT.GE.3500.AND.PTHAT.LT.4000) WTXS = 1.70783513D13

      ENDIF

C... Fit function form
C      WTXS = (150.564d0*(PT2/25.0d0)**(6.28335d0)*
C     & exp(-6.28335d0*(PT2/25.0d0))
C     & + 0.035313d0*PT2-0.00628d0*log(PT2+1)*log(PT2+1))/
C     & (1.04992d0*exp(-0.245*PT2)) 


C...Weights for EWK sample
 
      IF (ISUB.EQ.2) WTXS=0.2
      IF (ISUB.EQ.102) WTXS=400.    
      IF (ISUB.EQ.123) WTXS=400.    
      IF (ISUB.EQ.124) WTXS=400.    
      

      RETURN
      END
