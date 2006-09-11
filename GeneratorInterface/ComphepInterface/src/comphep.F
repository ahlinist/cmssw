*************************************************************************
*                       M A I N   program                               *
*          for CompHEP 4.10  -  PYTHIA 6.2 interface                    *
* uses unweighted events from CompHEP output as PYTHIA external process *
*************************************************************************
*  25.11.2001         Ilyin V.A.                                        *
*************************************************************************
      SUBROUTINE PEVMAIN
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
*...PYTHIA common blocks
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON /PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      SAVE

      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
      SAVE /HEPRUP/

      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &VTIMUP(MAXNUP),SPINUP(MAXNUP)
      SAVE /HEPEUP/

*...CompHEP-PYTHIA interface
      INTEGER KFB,IS,NEVPY,FIREVT,ISMEAR
      CHARACTER PROCESS*80,EVFILE*500,BEAM*16
      DOUBLE PRECISION SQRTS,RPDTY,BMASS
      COMMON/BEAMS/SQRTS,RPDTY,KFB(2),BMASS(2),BEAM(2)
      COMMON/ISMEAR/ISMEAR
      COMMON/INPARM/IS,NEVPY,FIREVT,PROCESS,EVFILE
      SAVE

****************** read CompHEP output header and input parameters
*...read input parameters from the file (INPARM.DAT)
      CALL RINPAR
*...switch to smear the mass of unstable particles in CompHEP out-state
      ISMEAR=1     ! 0 - no Breit-Wigner smearing, 1- yes
*...set some phys. parameters (masses etc) in PYTHIA according to CompHEP
*...default values
      CALL PHPARS
*...read the number of subprocesses
      CALL SUBNUM
*...read header for event flow
      CALL RHEADER
*...If firevt doesn't equal 1, processing begins from firevt event
       call SHIFT(FIREVT)

*************   USER can change these switches or add others ********

*...Independent(MSTJ(1)=2)/LUND(=1)/NO(=0) Fragmentation Model switch
c      MSTJ(1) = 0
*...switch on/off fragmentation and decays
c      MSTP(111) = 0
*...switch on/off initial QCD and QED state radiation
c      MSTP(61) = 0
*...switch on/off final state showers
c      MSTP(71) = 0
*...switch off multiple interactions
c      MSTP(81) = 0
*...title page off
c      MSTU(12) = 0
*...no initialization and diff.cross-section maximization print-out
c      MSTP(122) = 0

********************************************************************
*...initialize PYTHIA (using CompHEP output for kin.frame)
      CALL PYINIT('USER',' ',' ',0d0)
      CALL PHPARS    ! set again phys.parameters according to
                     ! CompHEP default values
*********************************************************
*...generate events by PYTHIA
c      nnev = 0
c      CALL HEPRUP_PRINT   ! Print HEPRUP Common block
c      DO 200 IEV=1,NEVPY
c           CALL PYEVNT

*...<<<<<<< USER's routines for analysis should be call here >>>>>> 
c      CALL HEPEUP_PRINT   ! Print other parameters from HEPEUP Common block
c        call pylist(7)    ! This is an example of user's routine:
c        call pylist(1)    ! This is an example of user's routine:
c        call PYHEPC(1)      ! Convert PYJETS event to HEPEVT format.
*...<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


*...check end of file with partonic events
c        IF (NUP.eq.0) THEN
c            goto 300
c        ELSE
c            nnev = iev
c        ENDIF

c200   CONTINUE               ! DO 200 IEV=1,NEVPY

c300   CONTINUE
*...cross section table
c      CALL PYSTAT(1)

c      write(*,*) '  Generated events = ',nnev

      END
*************************************************************************
*   25.11.2001    V.A.Ilyin                                             *
*************************************************************************
*      Running ALPHAS                                                   *
*                                                                       *
*   Quark masses thresholds are matched at                              *
*        Mc=1.3 GeV, Mb=4.3 GeV   (MS-bar masses)                       *
*     and Mtop=175 GeV                                                  *
*                                                                       *
*   Input: DSCALE = QCD scale in GeV                                    *
*          QCDL5 =  Lambda in GeV at NF=5                               *
*          NL = 1 (LO), 2(NLL)                                          *
*************************************************************************
      DOUBLE PRECISION FUNCTION ALPHAS2(DSCALE,QCDL5,NL)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      COMMON /QCDPAR/ QCDLF(6),NNL
      COMMON /BETA/ BETA0(6),BETA1(6)
      LOGICAL FIRST
      DATA FIRST/.TRUE./
      SAVE

      IF ((FIRST).OR. (QCDL5.NE. QCDLF(5)) .or. NNL.NE.NL) THEN
         NNL = NL
         QCDLF(5) = QCDL5
         PI4=16.0D0*ATAN(1.d0)
         DO 20 NFF=3,6
           BETA0(NFF)= 11.d0 - 2.d0/3.d0*NFF
           BETA1(NFF)= 51.d0 -19.d0/3.d0*NFF
20       CONTINUE
         XMC = 1.3d0
         XMB = 4.3d0
         XMTOP = 175.d0
* matching t-quark threshold
         QCDLF(6)=TONFB(QCDLF(5),XMTOP,6)
c         write(*,*) 'Lambda(6)=',qcdlf(6)
* matching b-quark threshold 
         QCDLF(4)=TONF(QCDLF(5),XMB,5)
c         write(*,*) 'Lambda(4)=',qcdlf(4)
* matching c-quark threshold 
         QCDLF(3)=TONF(QCDLF(4),XMC,4)
c         write(*,*) 'Lambda(3)',qcdlf(3)
         FIRST=.FALSE.
      ENDIF

      IF     (DSCALE .GT. XMTOP) THEN
          NF = 6
      ELSEIF (DSCALE .GT. XMB) THEN
          NF = 5
      ELSEIF (DSCALE .GT. XMC) THEN
          NF = 4
      ELSE
          NF = 3
      ENDIF

c...LO 
      B0 = BETA0(NF)
      RL = 2.d0*LOG(DSCALE/QCDLF(NF))
      ALPHAS2 = PI4/(B0*RL)
c...NLL 
      IF (NNL.GT.1) THEN
        B1 = BETA1(NF)
        ALPHAS2 = ALPHAS2*(1-2*B1/B0**2*LOG(RL)/RL )
      ENDIF

      RETURN
      END

****************************************************
*  matching  Lambda_QCD   to lower NF=4,5          *      
****************************************************
      DOUBLE PRECISION FUNCTION TONF(QCDL,XMQ,NF)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      COMMON /BETA/ B0(6),B1(6)
      COMMON /QCDPAR/ QCDLF(6),NNL
      SAVE

      RL = LOG(XMQ/QCDL)
      B10N  = B1(NF)/B0(NF)
      B10N1 = B1(NF-1)/B0(NF-1)

      IF (NNL.GT.1) THEN
        XEXP1 = (B10N-B10N1)*LOG(2*RL)-B10N1*LOG(B0(NF)/B0(NF-1))+
     &          (B10N*(B10N-B10N1)*LOG(2.d0*RL)+B10N**2-B10N1**2)
     &          /(B0(NF)*RL)
      ELSE
        XEXP1 = 0d0
      ENDIF

        TONF = QCDL*EXP(-1/B0(NF-1)
     &   *( (B0(NF)-B0(NF-1))*RL+XEXP1-(7.d0/18.d0)/(B0(NF)*RL)
     &    )            )

      RETURN
      END

****************************************************
*   get Lambda_QCD(NF+1) back  from  QCDLF(NF)         *      
****************************************************
      DOUBLE PRECISION FUNCTION TONFB(QCDL,XMQ,NF)
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)

      XLAM = QCDL/2d0
10    CONTINUE
        XLAMx = TONF(XLAM,XMQ,NF)
        EPS = XLAMx-QCDL
        IF (ABS(EPS).GT.1.d-4) THEN
           XLAM = XLAM - EPS/2d0
        ELSE
           TONFB = XLAM
           GOTO 20
        ENDIF
      GOTO 10

20    RETURN
      END
*************************************************************************
*                      interface routines                               *
*          for CompHEP 4.10  -  PYTHIA 6.2 interface                    *
*************************************************************************
*  15.12.99         Ilyin V.A.                                          *  
*  24.12.99         Modified by A. Belyaev    for   PYTHIA 6.1          *
*  18.05.2000  Modified by A.Skachkova for CompHEP version N4           *
*  22.10.2000    SETWID is removed, V.A.Ilyin                           *
*  22.10.2000  Zero masses protected, V.A.Ilyin                         *
*  25.11.2001  updated to PYTHIA 6.2 by V.A.Ilyin                       *
*  10.05.2002  bug on CTEQ5m, improved by A.Sherstnev                   *
*************************************************************************

*****************************************
*...write event (in PYJETS format)
*****************************************
      SUBROUTINE EVOUT
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)      
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      SAVE
      
      WRITE(6,*) N,((K(I,J),J=1,5),I=1,N),((P(I,J),J=1,5),I=1,N),
     .             ((V(I,J),J=1,5),I=1,N)
     
      RETURN
      END

**************************************************
*...read number of subprocesses
*************************************************
      SUBROUTINE SUBNUM
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
*...CompHEP-PYTHIA interface   
      CHARACTER LINE*80,PROCESS*80,EVFILE*500
      INTEGER IS,NEVPY,FIREVT      
      COMMON/INPARM/IS,NEVPY,FIREVT,PROCESS,EVFILE
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .               NEV(100),XM(100,20,3),SIGMA(100),NPROC
  
      IS = 44
1     READ(IS,FMT='(A80)') LINE
      IF (LINE(1:26).EQ.'#Number_of_subprocesses = ') THEN
         READ(LINE,FMT='(26x,i4)') nproc 
      ELSE
         GOTO 1
      ENDIF  
      REWIND IS                  

      RETURN
      END           

*****************************
*...read header of the event file
************************************
      SUBROUTINE RHEADER
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
*...PYTHIA common blocks
      INTEGER PYCOMP
      COMMON /PYPARS/ MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
*...CompHEP-PYTHIA interface
      CHARACTER*80 PROCESS
      CHARACTER*500 EVFILE
      CHARACTER*1000 LINE
      CHARACTER*16 BEAM*16
      CHARACTER*100 EMPTYS
      CHARACTER*100 SPROCESS
      CHARACTER*100 STRF1,STRF2
      INTEGER IS,NEVPY,FIREVT      
      COMMON/INPARM/IS,NEVPY,FIREVT,PROCESS,EVFILE

      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .               NEV(100),XM(100,20,3),SIGMA(100),NPROC
      COMMON/BEAMS/SQRTS,RPDTY,KFB(2),BMASS(2),BEAM(2)
      SAVE
      
*...Which PevLIB version do you use?
      READ(IS,1001) LINE

*...Loop over of subprocesses
      DO 100 NPRC=1,NPROC

*...Which CompHEP version do you use?
        READ(IS,1001) LINE

*...Read particles names for the subprocess and get their KF codes 
        SPROCESS = EMPTYS(100)
        READ(IS,1009) LINE
        READ(LINE,FMT='(A100)') SPROCESS
      
        CALL R_PROC(NPRC,SPROCESS)

*...Omit string "initial state"
        READ(IS,1001) LINE

*...Read CMS energy and rapidity for Lorentz boost 
*...to transform CMS to the collision frame used in CompHEP
        SQRTS0 = SQRTS
        RPDTY0 = RPDTY
        READ(IS,1009) LINE
        READ(LINE,FMT='(G14.6)', ERR=2001) SQRTS
      
*...Avoid conflict with "remnant" approach to ISR for electron beam:
       SQRTS = SQRTS*1.0000001

        READ(IS,1017) LINE
        READ(LINE,FMT='(G14.6)', ERR=2002) RPDTY
        IF (NPRC.GT.1) THEN
          delt = abs(SQRTS-SQRTS0)
          IF (delt.lt.1.d-10) SQRTS=SQRTS0
          delt = abs(RPDTY-RPDTY0)
          IF (delt.lt.1.d-10) RPDTY=RPDTY0
          IF ((SQRTS.NE.SQRTS0).or.(RPDTY.NE.RPDTY0)) THEN
            WRITE(*,*) 'Invalid SQRTS or RPDTY in subprocesses'
            STOP
          ENDIF
        ENDIF

*...read names of beams used in CompHEP and get their KF codes and masses
        KFB1=KFB(1)
        KFB2=KFB(2)
        READ(IS,1010) LINE
        READ(LINE,FMT='(A80)') STRF1
        BEAM(1) = EMPTYS(20)	
        CALL R_BEAM(STRF1,KFB(1),BEAM(1),BMASS(1),PDFGUP(1),PDFSUP(1))
        IF(BEAM(1).EQ.'no') KFB(1)=KFP(nprc,1)
        IF((BEAM(1).EQ.'electron').and.(KFP(nprc,1).lt.0))KFB(1)=-KFB(1)

        READ(IS,1010) LINE
        READ(LINE,FMT='(A80)') STRF2
        BEAM(2) = EMPTYS(20)
        CALL R_BEAM(STRF2,KFB(2),BEAM(2),BMASS(2),PDFGUP(2),PDFSUP(2))
        IF(BEAM(2).EQ.'no') KFB(2)=KFP(nprc,2)
        IF((BEAM(2).EQ.'electron').and.(KFP(nprc,2).lt.0))KFB(2)=-KFB(2)
        CALL PYNAME(KFB(1),BEAM(1))
        CALL PYNAME(KFB(2),BEAM(2))
        IF ((NPRC.GT.1).and.((KFB1.NE.KFB(1)).or.(KFB2.NE.KFB(2)))) THEN
          WRITE(*,*) 'Invalid beams in subprocesses'
          STOP
        ENDIF

*...define beam momenta for PYTHIA 'USER" frame
        CALL BEAMFR
 
*...read particles masses in the subprocess
        READ(IS,1008) LINE
        READ(LINE,FMT='(20(G17.10))',ERR=2003) (XM(NPRC,i,1),
     +  i=1,NNUP(NPRC))

*...set out-particle masses in PYTHIA according to CompHEP events
        DO 30 i=3,NNUP(nprc)
          IF (XM(nprc,i,1).gt.0.d0) THEN
	    PMAS(PYCOMP(KFP(nprc,i)),1)=XM(nprc,i,1)
	  ENDIF
30      CONTINUE

*...read cross section of the subprocess in pb
        READ(IS,1021) LINE
        READ(LINE,FMT='(G13.6)', ERR=2004) SIGMA(NPRC)

*...read number of (unweighted) events in the input
        READ(IS,1023) LINE
        READ(LINE,FMT='(i10)', ERR=2005)  NEV(NPRC)

*...omit string: #----------------------------------------------------------
        READ(IS,1001) LINE

*...end of loop over subprocess headers
100   CONTINUE

*...omit string: #Number_of_subprocesses = xxx
      READ(IS,1001) LINE

*...read total cross section (not used)
      READ(IS,1027) LINE
      READ(LINE,FMT='(G18.3)', ERR=2006) XSIGMAT

*...read number of mixed events (not used)
      READ(IS,1030) LINE
      READ(LINE,FMT='(i10)', ERR=2007) NEVTOT

*...omit string: #Nproc ================== Events ==========================
      READ(IS,1001) LINE
      RETURN

 2001 WRITE(*,*) 'Invalid string for the sqrtS in subprocess = ',NPRC
      STOP
 2002 WRITE(*,*) 'Invalid string for the rapidity in subprocess = ',NPRC
      STOP
 2003 WRITE(*,*) 
     + 'Invalid string for the particle masses in subprocess = ',NPRC
      STOP
 2004 WRITE(*,*) 
     + 'Invalid string for the cross section in subprocess = ',NPRC
      STOP
 2005 WRITE(*,*) 
     + 'Invalid string for the events number in subprocess = ',NPRC
      STOP
 2006 WRITE(*,*) 'Invalid string for the full cross section'
      STOP
 2007 WRITE(*,*) 'Invalid string for the full events number'
      STOP

 1001 FORMAT( 1x,A1000)
 1008 FORMAT( 8x,A1000)
 1009 FORMAT( 9x,A1000)
 1010 FORMAT(10x,A1000)
 1017 FORMAT(17x,A1000)
 1021 FORMAT(21x,A1000)
 1023 FORMAT(23x,A1000)
 1027 FORMAT(27x,A1000)
 1030 FORMAT(30x,A1000)
      END


*******************************
* evaluate momenta of beams  
*******************************
      SUBROUTINE BEAMFR
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
*...PYTHIA common blocks
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
*...CompHEP-PYTHIA interface
      CHARACTER BEAM*16
      DIMENSION PBCMS(2,5)
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .              NEV(100),XM(100,20,3),SIGMA(100),NPROC
      COMMON/BEAMS/SQRTS,RPDTY,KFB(2),BMASS(2),BEAM(2)
      SAVE

*...evaluate beams momenta in CMS
        PBCMS(1,1) = 0
        PBCMS(1,2) = 0
        PBCMS(1,3) = sqrt(SQRTS**2/4-BMASS(1)**2)
        PBCMS(1,4) = SQRTS/2
        PBCMS(1,5) = BMASS(1)
        PBCMS(2,1) = 0
        PBCMS(2,2) = 0
        PBCMS(2,3) = -sqrt(SQRTS**2/4-BMASS(2)**2)
        PBCMS(2,4) = SQRTS/2
        PBCMS(2,5) = BMASS(2)
*...evaluate beams momenta in the collison frame using Lorentz boost 
*...given from the CompHEP session
        P(1,1) = PBCMS(1,1)
        P(1,2) = PBCMS(1,2)
        P(1,3) = SINH(RPDTY)*PBCMS(1,4) + COSH(RPDTY)*PBCMS(1,3)
        P(1,4) = COSH(RPDTY)*PBCMS(1,4) + SINH(RPDTY)*PBCMS(1,3)
        P(1,5) = PBCMS(1,5)
        P(2,1) = PBCMS(2,1)
        P(2,2) = PBCMS(2,2)
        P(2,3) = SINH(RPDTY)*PBCMS(2,4) + COSH(RPDTY)*PBCMS(2,3)
        P(2,4) = COSH(RPDTY)*PBCMS(2,4) + SINH(RPDTY)*PBCMS(2,3)
        P(2,5) = PBCMS(2,5)
      
      RETURN
      END

******************************
* parsing the beam strings
******************************
      SUBROUTINE R_BEAM(STRF,KFB,BEAM,BMASS,IPDFGUP,IPDFSUP)
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
      CHARACTER BEAM*16,STRF*80
      COMMON /QCDPAR/ QCDLF(6),NL
      SAVE
      
*** error case
      IF (STRF.eq.'') THEN
         write(*,*) 'Error in STRF'
         stop
      ENDIF

*** default beam mass
      BMASS = 0

      DO 10 i=1,80
        IF (STRF(i:i).ne.' ') goto 20
10    continue

*** OFF case
20    IF(STRF(i:(i+2)).EQ.'OFF') THEN
         KFB = 0
         BEAM = 'no'
         RETURN
	 
*** Proton/AntiProton beam cases
      ELSEIF (STRF(i:(i+3)).EQ.'PDF:') THEN
         BMASS = 0.938    ! mass of proton
         NL=2             ! default NLL
         GOTO 50
	 
*** Electron beam cases
      ELSEIF(STRF(i:(i+2)).EQ.'ISR')THEN
         BEAM = 'electron'
         BMASS = 0.000511   ! mass of electron
         GOTO 99
	 
      ELSEIF(STRF(i:(i+11)).EQ.'Equiv.Photon')THEN
         BEAM = 'electron'
         BMASS = 0.000511   ! mass of electron
         GOTO 99
	 
*** Photon beam case
      ELSEIF(STRF(i:(i+13)).EQ.'Laser photons')THEN
         BEAM = 'gamma'
         GOTO 99
      ENDIF

*** error STRF
      GOTO 999

*** Proton or AntiProton beams 
50    k=i+4
      IF(STRF(k:(k+3)).eq.'MRST') THEN
        IPDFGUP=3
        IF (STRF((k+4):(k+6)).EQ.'c-g')  THEN      ! MRSTc-g(proton)
                    NL = 1           ! LO
                    IPDFSUP=72
                    QCDLF(5)=0.2374
                    K1 = k+8
        ENDIF
        IF (STRF((k+4):(k+5)).EQ.'ht')  THEN       ! MSRTht(proton)
                    NL = 2
                    IPDFSUP=77
                    QCDLF(5)=0.2086
                    K1 = k+7
        ENDIF
        GOTO 55
      ENDIF
           IF((STRF(k:k+3).eq.'cteq').or.(STRF(k:k+3).eq.'CTEQ')) THEN
             IPDFGUP=4
             IF(STRF((k+4):(k+5)).eq.'6l'.or.
     &          STRF((k+4):(k+5)).eq.'6L') THEN         ! CTEQ6L(proton)
                    NL=1                       !LO running alphas
                    IPDFSUP=-1
                    QCDLF(5)=0.146
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+6)).eq.'6l1'.or.
     &          STRF((k+4):(k+6)).eq.'6L1') THEN         ! CTEQ6L1(proton)
                    NL=1                       !LO running alphas
                    IPDFSUP=-1
                    QCDLF(5)=0.146
                    K1 = k+8
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'6m'.or.
     &          STRF((k+4):(k+5)).eq.'6M') THEN        ! CTEQ6m(proton)
                    NL=2                       !NLL running alphas
                    IPDFSUP=-1
                    QCDLF(5)=0.226
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'6d'.or.
     &          STRF((k+4):(k+5)).eq.'6D') THEN        ! CTEQ6m(proton)
                    NL=2                       !NLL running alphas
                    IPDFSUP=-1
                    QCDLF(5)=0.226
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'5l'.or.
     &          STRF((k+4):(k+5)).eq.'5L') THEN         ! CTEQ5L(proton)
                    NL=1                       !LO running alphas
                    IPDFSUP=46
                    QCDLF(5)=0.146
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'5m'.or.
     &          STRF((k+4):(k+5)).eq.'5M') THEN        ! CTEQ5m(proton)
                    NL=2                       !NLL running alphas
                    IPDFSUP=48
                    QCDLF(5)=0.226
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+6)).eq.'5m1'.or.
     &          STRF((k+4):(k+6)).eq.'5M1') THEN        ! CTEQ5m1(proton)
                    NL=2                       !NLL running alphas
                    IPDFSUP=53
                    QCDLF(5)=0.226
                    K1 = k+8
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'4l'.or.
     &          STRF((k+4):(k+5)).eq.'4L') THEN         ! CTEQ4l(proton)
                    NL=1                       !LO running alphas
                    IPDFSUP=32
                    QCDLF(5)=0.181
                    K1 = k+7
             ENDIF
             IF(STRF((k+4):(k+5)).eq.'4m'.or.
     &          STRF((k+4):(k+5)).eq.'4M') THEN         ! CTEQ4m(proton)
                    NL=2                       !NLL running alphas
                    IPDFSUP=34
                    QCDLF(5)=0.202
                    K1 = k+7
             ENDIF
           GOTO 55
           ENDIF
           IF(STRF(k:(k+2)).eq.'GRV') THEN
             IPDFGUP=5
             IF (STRF((k+3):(k+10)).EQ.'98LO')  THEN  ! GRV98LO(proton)
                    NL = 1           ! LO
                    IPDFSUP=12
                    QCDLF(5)=0.1529
                    K1 = k+8
             ENDIF
             IF (STRF((k+3):(k+10)).EQ.'98HO')  THEN
                    NL = 2
                    IPDFSUP=13
                    QCDLF(5)=0.1334
                    K1 = k+8
             ENDIF
           GOTO 55
           ENDIF

           write(*,*) 'Unknown PDF set for proton/antiproton'
           stop
55    CONTINUE
      
      kk = 1
      DO 65 j=K1,80
        IF (STRF(j:j).eq.' '.or.STRF(j:j).eq.')') goto 99
        BEAM(kk:kk)=STRF(j:j)
        kk=kk+1
65    continue

*...get PYTHIA's KF code for the beam      
99    KFB = KFBEAM(BEAM)
      RETURN

999   WRITE(*,*) 'Unknown STRF'
      RETURN
      END

*********************************
* parsing the subprocess string
*********************************
      SUBROUTINE R_PROC(NPRC,PROCESS)
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .              NEV(100),XM(100,20,3),SIGMA(100),NPROC
      CHARACTER*100 PROCESS
      CHARACTER*3 PNAME(1:20)
      INTEGER II
      SAVE

      NNUP(NPRC) = 0
      II = 0

5     CONTINUE
       II = II + 1

      IF (PROCESS(II:II).EQ.' ') GOTO 15
      IF (PROCESS(II:II).EQ.'-') THEN
        NIN = NNUP(NPRC)
        GOTO 5
c 	write(*,*) 'Warning: the interface version analyses initial ',
c     .'state in process strings only'
c	RETURN
      ENDIF
      IF (PROCESS(II:II).EQ.'>') GOTO 5
      NNUP(NPRC) = NNUP(NPRC)+1
      IF (NNUP(NPRC).GT.10) THEN
        write(*,*) 'Warning: more than 10 partons in final state'
      ENDIF

      PNAME(NNUP(NPRC)) = PROCESS(II:II+4)
      KFP(NPRC,NNUP(NPRC)) = KFPART(PNAME(NNUP(NPRC)))

      II = II + 3

      IF (KFP(NPRC,NNUP(NPRC)).eq.0) THEN
        write(*,*) 'II = ', II
        write(*,*) 'Unknown PYTHIA particle name: ',PNAME(NNUP(NPRC))
        STOP
      ENDIF

 15   IF (II.GE.80) goto 10
      GOTO 5
 10   continue

      IF (NNUP(NPRC).EQ.0) THEN
        WRITE(*,*) 'No process string in event file.'
        STOP
      ELSEIF(NIN.EQ.0) THEN
        WRITE(*,*) 'No IN particles in the process string.'
        STOP
      ELSEIF((NNUP(NPRC)-NIN).EQ.0) THEN
        WRITE(*,*) 'No OUT particles in the process string.'
        STOP
      ELSEIF(NIN.EQ.1) THEN
        WRITE(*,*) 'Decay process is not allowed.'
        STOP
      ENDIF

      RETURN
      END


*######################################################
*  Some service routines
*######################################################

********************
* make empty string
********************
      FUNCTION EMPTYS(N)
      CHARACTER*(*) EMPTYS
      SAVE

      DO 15 k=1,N
           EMPTYS(k:k)=' '
15    CONTINUE
      RETURN
      END

*******************************
* delete blanks in string
*******************************
      FUNCTION CMPRS(STR)
      CHARACTER*500 STR,XSTR,CMPRS
      CHARACTER*500 EMPTYS
      SAVE

      j=1
      XSTR = EMPTYS(500)
      DO 10 i=1,80
        IF(STR(i:i).ne.' ') THEN
          XSTR(j:j)=STR(i:i)
          j=j+1
        ENDIF
10    CONTINUE
      CMPRS = XSTR
      RETURN
      END

*************************************************************************
*                         KFBEAM routine                                *
*          for CompHEP 41.10  -  PYTHIA 6.2 interface                   *
*************************************************************************
*  25.11.2001  Ilyin V.A.                                               *
*************************************************************************
      INTEGER FUNCTION kfbeam(bname)
      CHARACTER*16 bname

      kfbeam = 0
      IF (bname.eq.'Proton'.or.bname.eq.'proton'.or.
     . bname.eq.'p+') THEN 
            kfbeam=2212
      ELSEIF (bname.eq.'AntiProton'.or.bname.eq.'antiProton'.or.
     . bname.eq.'anti-proton'.or.bname.eq.'pbar-') THEN 
            kfbeam=-2212
      ELSEIF (bname.eq.'electron'.or.bname.eq.'e-') THEN 
            kfbeam=11
      ELSEIF (bname.eq.'positron'.or.bname.eq.'e+') THEN 
            kfbeam=-11
      ELSEIF (bname.eq.'gamma') THEN 
            kfbeam=22
      ELSE
            WRITE(*,*) 'Unknown for CompHEP/PYTHIA beam name: ',bname
      ENDIF

      RETURN
      END

*************************************************************************
*                         KFPART routine                                *
*          for CompHEP 41.10  -  PYTHIA 6.2 interface                   *
*************************************************************************
*  25.11.2001  Ilyin V.A.                                               *
*************************************************************************
      INTEGER FUNCTION kfpart(pname)
      CHARACTER*3 pname

      IF     (pname.eq.'e1 ') THEN 
            kfpart=11
      ELSEIF (pname.eq.'E1 ') THEN 
            kfpart=-11
      ELSEIF (pname.eq.'e  ') THEN 
            kfpart=11
      ELSEIF (pname.eq.'E  ') THEN 
            kfpart=-11
      ELSEIF (pname.eq.'e2 ') THEN 
            kfpart=13
      ELSEIF (pname.eq.'E2 ') THEN 
            kfpart=-13
      ELSEIF (pname.eq.'m  ') THEN 
            kfpart=13
      ELSEIF (pname.eq.'M  ') THEN 
            kfpart=-13
      ELSEIF (pname.eq.'e3 ') THEN 
            kfpart=15
      ELSEIF (pname.eq.'E3 ') THEN 
            kfpart=-15
      ELSEIF (pname.eq.'l  ') THEN 
            kfpart=15
      ELSEIF (pname.eq.'L  ') THEN 
            kfpart=-15
      ELSEIF (pname.eq.'e4 ') THEN 
            kfpart=17
      ELSEIF (pname.eq.'E4 ') THEN 
            kfpart=-17
      ELSEIF (pname.eq.'n1 ') THEN 
            kfpart=12
      ELSEIF (pname.eq.'N1 ') THEN 
            kfpart=-12
      ELSEIF (pname.eq.'ne ') THEN 
            kfpart=12
      ELSEIF (pname.eq.'Ne ') THEN 
            kfpart=-12
      ELSEIF (pname.eq.'n2 ') THEN 
            kfpart=14
      ELSEIF (pname.eq.'N2 ') THEN 
            kfpart=-14
      ELSEIF (pname.eq.'nm ') THEN 
            kfpart=14
      ELSEIF (pname.eq.'Nm ') THEN 
            kfpart=-14
      ELSEIF (pname.eq.'n3 ') THEN 
            kfpart=16
      ELSEIF (pname.eq.'N3 ') THEN 
            kfpart=-16
      ELSEIF (pname.eq.'nl ') THEN 
            kfpart=16
      ELSEIF (pname.eq.'Nl ') THEN 
            kfpart=-16
      ELSEIF (pname.eq.'n4 ') THEN 
            kfpart=18
      ELSEIF (pname.eq.'N4 ') THEN 
            kfpart=-18

      ELSEIF (pname.eq.'d# ') THEN
            kfpart=1
      ELSEIF (pname.eq.'D# ') THEN
            kfpart=-1
      ELSEIF (pname.eq.'u# ') THEN
            kfpart=2
      ELSEIF (pname.eq.'U# ') THEN
            kfpart=-2
      ELSEIF (pname.eq.'q# ') THEN
            kfpart=2
      ELSEIF (pname.eq.'Q# ') THEN
            kfpart=-2

      ELSEIF (pname.eq.'d  ') THEN 
            kfpart=1
      ELSEIF (pname.eq.'D  ') THEN 
            kfpart=-1
      ELSEIF (pname.eq.'u  ') THEN 
            kfpart=2
      ELSEIF (pname.eq.'U  ') THEN 
            kfpart=-2
      ELSEIF (pname.eq.'s  ') THEN 
            kfpart=3
      ELSEIF (pname.eq.'S  ') THEN 
            kfpart=-3
      ELSEIF (pname.eq.'c  ') THEN 
            kfpart=4
      ELSEIF (pname.eq.'C  ') THEN 
            kfpart=-4
      ELSEIF (pname.eq.'b  ') THEN 
            kfpart=5
      ELSEIF (pname.eq.'B  ') THEN 
            kfpart=-5
      ELSEIF (pname.eq.'t  ') THEN 
            kfpart=6
      ELSEIF (pname.eq.'T  ') THEN 
            kfpart=-6
      ELSEIF (pname.eq.'b4 ') THEN 
            kfpart=7
      ELSEIF (pname.eq.'B4 ') THEN 
            kfpart=-7
      ELSEIF (pname.eq.'t4 ') THEN 
            kfpart=8
      ELSEIF (pname.eq.'T4 ') THEN 
            kfpart=-8

      ELSEIF (pname.eq.'G  ') THEN 
            kfpart=21
      ELSEIF (pname.eq.'g  ') THEN 
            kfpart=21
      ELSEIF (pname.eq.'A  ') THEN 
            kfpart=22
      ELSEIF (pname.eq.'gamma') THEN 
            kfpart=22
      ELSEIF (pname.eq.'Z  ') THEN 
            kfpart=23
      ELSEIF (pname.eq.'Z0 ') THEN 
            kfpart=23
      ELSEIF (pname.eq.'W+ ') THEN 
            kfpart=24
      ELSEIF (pname.eq.'W- ') THEN 
            kfpart=-24
      ELSEIF (pname.eq.'H  ') THEN 
            kfpart=25
      ELSEIF (pname.eq.'h  ') THEN 
            kfpart=25

      ELSEIF (pname.eq.'h0 ') THEN 
            kfpart=25
      ELSEIF (pname.eq.'H0 ') THEN 
            kfpart=35
      ELSEIF (pname.eq.'A0 ') THEN 
            kfpart=36
      ELSEIF (pname.eq.'H3 ') THEN 
            kfpart=36
      ELSEIF (pname.eq.'H+ ') THEN 
            kfpart=37
      ELSEIF (pname.eq.'H- ') THEN 
            kfpart=-37

      ELSEIF (pname.eq.'Gr ') THEN                  ! Graviton
            kfpart=39
      ELSEIF (pname.eq.'R0 ') THEN 
            kfpart=41
      ELSEIF (pname.eq.'LQ ') THEN 
            kfpart=42

      ELSEIF (pname.eq.'~g ') THEN                  ! gluino
            kfpart= 1000021      

      ELSEIF (pname.eq.'~dL') THEN
            kfpart= 1000001
      ELSEIF (pname.eq.'~DL') THEN
            kfpart=-1000001
      ELSEIF (pname.eq.'~dR') THEN
            kfpart= 2000001
      ELSEIF (pname.eq.'~DR') THEN
            kfpart=-2000001
      ELSEIF (pname.eq.'~uL') THEN
            kfpart= 1000002 
      ELSEIF (pname.eq.'~UL') THEN
            kfpart=-1000002 
      ELSEIF (pname.eq.'~uR') THEN
            kfpart= 2000002
      ELSEIF (pname.eq.'~UR') THEN
            kfpart=-2000002
      ELSEIF (pname.eq.'~sL') THEN
            kfpart= 1000003  
      ELSEIF (pname.eq.'~SL') THEN
            kfpart=-1000003 
      ELSEIF (pname.eq.'~sR') THEN
            kfpart= 2000003 
      ELSEIF (pname.eq.'~SR') THEN
            kfpart=-2000003
      ELSEIF (pname.eq.'~cL') THEN
            kfpart= 1000004
      ELSEIF (pname.eq.'~CL') THEN
            kfpart=-1000004
      ELSEIF (pname.eq.'~cR') THEN
            kfpart= 2000004 
      ELSEIF (pname.eq.'~CR') THEN
            kfpart=-2000004
      ELSEIF (pname.eq.'~b1') THEN
            kfpart= 1000005 
      ELSEIF (pname.eq.'~B1') THEN
            kfpart=-1000005
      ELSEIF (pname.eq.'~b2') THEN
            kfpart= 2000005
      ELSEIF (pname.eq.'~B2') THEN
            kfpart=-2000005
      ELSEIF (pname.eq.'~t1') THEN
            kfpart= 1000006
      ELSEIF (pname.eq.'~T1') THEN
            kfpart=-1000006
      ELSEIF (pname.eq.'~t2') THEN
            kfpart= 2000006 
      ELSEIF (pname.eq.'~T2') THEN
            kfpart=-2000006 
      ELSEIF (pname.eq.'~eL') THEN
            kfpart= 1000011
      ELSEIF (pname.eq.'~EL') THEN
            kfpart=-1000011
      ELSEIF (pname.eq.'~eR') THEN
            kfpart= 2000011 
      ELSEIF (pname.eq.'~ER') THEN
            kfpart=-2000011
      ELSEIF (pname.eq.'~ne') THEN
            kfpart= 1000012
      ELSEIF (pname.eq.'~Ne') THEN
            kfpart=-1000012   
      ELSEIF (pname.eq.'~mL') THEN
            kfpart= 1000013 
      ELSEIF (pname.eq.'~ML') THEN
            kfpart=-1000013 
      ELSEIF (pname.eq.'~mR') THEN
            kfpart= 2000013
      ELSEIF (pname.eq.'~MR') THEN
            kfpart=-2000013
      ELSEIF (pname.eq.'~nm') THEN
            kfpart= 1000014 
      ELSEIF (pname.eq.'~Nm') THEN
            kfpart=-1000014
      ELSEIF (pname.eq.'~l1') THEN
            kfpart= 1000015
      ELSEIF (pname.eq.'~L1') THEN 
            kfpart=-1000015
      ELSEIF (pname.eq.'~l2') THEN
            kfpart= 2000015 
      ELSEIF (pname.eq.'~L2') THEN
            kfpart=-2000015
      ELSEIF (pname.eq.'~nl') THEN
            kfpart= 1000016  
      ELSEIF (pname.eq.'~Nl') THEN
            kfpart=-1000016
 
      ELSEIF (pname.eq.'~o1') THEN                  ! neutralino chi_10
            kfpart= 1000022
      ELSEIF (pname.eq.'~o2') THEN                  ! neutralino chi_20
            kfpart= 1000023 
      ELSEIF (pname.eq.'~o3') THEN                  ! neutralino chi_30
            kfpart= 1000025 
      ELSEIF (pname.eq.'~o4') THEN                  ! neutralino chi_40
            kfpart= 1000035 
      ELSEIF (pname.eq.'~1+') THEN                  ! chargino chi_1+
            kfpart= 1000024
      ELSEIF (pname.eq.'~1-') THEN                  ! chargino chi_1-
            kfpart=-1000024
      ELSEIF (pname.eq.'~2+') THEN                  ! chargino chi_2+
            kfpart= 1000037
      ELSEIF (pname.eq.'~2-') THEN                  ! chargino chi_2-
            kfpart=-1000037
c... other lebel for chargino (from extra-dimention theories)
      ELSEIF (pname.eq.'KG') THEN                  ! chargino chi_2-
            kfpart= 1000037

      ELSEIF (pname.eq.'~G') THEN                   ! gravitino
            kfpart=1000039
      ELSEIF (pname.eq.'~sS') THEN                  ! sgoldstino S
            kfpart=1000040
      ELSEIF (pname.eq.'~sP') THEN                  ! sgoldstino P
            kfpart=1000041


      ELSE 
            kfpart=0
      ENDIF

      RETURN
      END

*************************************************************************
*          Routine to set some physical parameters in PYTHIA            *
*           CompHEP 4.10  -  PYTHIA 6.2 interface                       *
*************************************************************************
*  06.11.99        A.Skachkova,                                         *
*  25.11.2001      Ilyin V.A.                                           *
*************************************************************************
      SUBROUTINE PHPARS
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      COMMON /PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYINT1/MINT(400),VINT(400)
*...sinW
c      write(*,*) sqrt(PARU(102))
      sinW=0.47547
      PARU(102)=sinW**2
                                   
*...masses, widths and width bands (CompHEP default values)

*....Z0 
c      write(*,*) PMAS(23,1)
       PMAS(23,1)=91.187
c      write(*,*) PMAS(23,2)
       PMAS(23,2)=2.49444
c      write(*,*) PMAS(23,3)
       PMAS(23,3)=10.*PMAS(23,2)

*...W
c      write(*,*) PMAS(24,1)
       PMAS(24,1)=PMAS(23,1)*sqrt(1.-sinW**2)
c      write(*,*) PMAS(24,2)
       PMAS(24,2)=2.08895
c      write(*,*) PMAS(24,3)
       PMAS(24,3)=10.*PMAS(24,2)
       
*...t-quark
c      write(*,*) PMAS(6,1)
       PMAS(6,1)=175.000
c      write(*,*) PMAS(6,2)
       PMAS(6,2)=1.59
c      write(*,*) PMAS(6,3)
       PMAS(6,3)=10.*PMAS(6,2)

*...b-quark (pole mass)
c      write(*,*) PMAS(5,1)
      PMAS(5,1)= 4.850 
*...c-quark (pole mass)
c      write(*,*) PMAS(4,1)
      PMAS(4,1)= 1.420
*...s-quark
c      write(*,*) PMAS(3,1)
      PMAS(3,1)= 0.200
C...   MU   MASS VALUE  
c      write(*,*) PMAS(13,1)
      PMAS(13,1)=  0.1057
C...   TAU LEPTON   MASS VALUE  
c      write(*,*) PMAS(15,1)
       PMAS(15,1)=  1.777    

c      read(*,*)

      RETURN
      END
*****************************************
*...read input parameters from DAT file
*   15.12.99  V.Ilyin
*****************************************
      SUBROUTINE RINPAR
      COMMON/INPARM/IS,NEVPY,FIREVT,PROCESS,EVFILE
      INTEGER IS,NEVPY,FIREVT
      CHARACTER EVFILE*500,PROCESS*80,XXX*500,CMPRS*500
      SAVE
      character*132                            CXpar
      integer         Ixpar 
      double precision            RXpar
      common /EXGPAR/ IXpar(100), RXpar(100), CXpar(100)
C
*      OPEN (UNIT=22, ERR=3000, FILE='INPARM.DAT', STATUS='OLD')

*      READ (UNIT=22, FMT='(A500)', ERR=1000) XXX
*      READ (UNIT=22, FMT='(A500)', ERR=1000) XXX
*      EVFILE = CMPRS(XXX)
       EVFILE = CXpar(1) ! '../data/CompHEP_events.PEV'
*      READ (UNIT=22, FMT='(A500)', ERR=1000) XXX
*      READ (UNIT=22, FMT='(A500)', ERR=1000) XXX
*      PROCESS = CMPRS(XXX)
       PROCESS = CXpar(2) !  ! p,p -> p,p'
       WRITE(*,*) PROCESS   

c      READ (UNIT=22, FMT='(A500)', ERR=1000) XXX
c      READ (UNIT=22, FMT='(i10)', ERR=1000) NEVPY

*      READ(UNIT=22,FMT='(A500)', ERR=1000) XXX
*      READ(UNIT=22,FMT='(i10)', ERR=1000) FIREVT
       FIREVT = IXpar(1)
      if (FIREVT.LT.1) then
        write(*,*)'***Warning! First event parameter in INPARM.DAT file'
	write(*,*)'    does not have to less than 1. So it is set to 1.'
	FIREVT=1
      endif

*      CLOSE(UNIT=22)

*... input channel for unweighted events 
      IS = 44
      OPEN(IS,FILE=EVFILE,STATUS='OLD',ERR=3001)
      RETURN

 1000 WRITE(*,*)'Error: I can not read INPARM.DAT! Stop!'
      STOP

 3000 WRITE(*,*)'Error: I can not open INPARM.DAT! Stop!'
      STOP

 3001 WRITE(*,*)
     + 'Error: I can not open a file with events (',EVFILE,')! Stop!'
      STOP

      END
*************************************************
*   Shift of event read from event file         *
*                                               *
*  FIRST  -- event number for processing        *
*                                               *
*************************************************
      SUBROUTINE SHIFT(FIRST)
      INTEGER FIRST
      INTEGER N,NEV
      INTEGER IS,NEVPY,FIREVT 
      CHARACTER PROCESS*80,EVFILE*500 
      COMMON /INPARM/ IS,NEVPY,FIREVT,PROCESS,EVFILE
      SAVE

      N = 0
       IF (FIRST.GT.1) THEN         
         DO 15 I=1,FIRST-1
           READ(IS,FMT='(I2)') nev
	   N=N+1
  15	 CONTINUE
       ENDIF
       RETURN
      END
*************************************************************************
*        Routine to smear the resonance mass according  to Breit-Wigner *
*                  CompHEP 4.10  -  PYTHIA 6.2 interface                *
*************************************************************************
*  15.12.99         A. Belyaev, V.Ilyin                                 *
*  06.11.2000       some corrections (XM -> PMAS), V.Ilyin              *
*  25.11.2001       update to Pythia 6.2 by V.Ilyin                     *
*************************************************************************
      SUBROUTINE SMEARMASS
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      INTEGER KCp
      INTEGER PYCOMP     ! PYTHIA FUNCTION
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &VTIMUP(MAXNUP),SPINUP(MAXNUP)
*...CompHEP-PYTHIA interface
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .               NEV(100),XM(100,20,3),SIGMA(100),NPROC
      SAVE

      REAL*8 PX,PX1(20,5),PX2(20,5),PX3(20,5),PXX(20,5)

*...some values in lab frame
      PLAB=PUP(3,1)+PUP(3,2)
      ELAB=PUP(4,1)+PUP(4,2)
      SHAT=(ELAB-PLAB)*(ELAB+PLAB)

*...loop over the particles in final state
      DO I=3,NUP
      
*...if unstable particle then smear its mass
        KCp=PYCOMP(IDUP(I))
        IF(PMAS(ABS(KCp),2).gt.0.) THEN

*...transfer out-momenta to REAL*8
        DO 100 j=1,NUP
        DO 100 k=1,5
100       PX1(j,k)=PUP(k,j)

*...invariant mass of the cluster recoiled the Ith particle
         IF(NUP.EQ.4) THEN              ! 2->2 case
           IF(I.eq.3) PM2=PX1(4,5)
           IF(I.eq.4) PM2=PX1(3,5)
         ELSE
           PX = sqrt(PX1(I,1)**2+PX1(I,2)**2+(PLAB-PX1(I,3))**2)
           PM2=(ELAB-PX1(I,4)-PX)*(ELAB-PX1(I,4)+PX)
           IF(PM2.lt.0.) PM2=abs(PM2)
           PM2=sqrt(PM2)
         ENDIF
 
*...boost of all momenta to partonic CMS
         GAMMA = 1.d0/(sqrt(ELAB-PLAB)*sqrt(ELAB+PLAB))
         DO j=1,NUP
           PX2(j,1)=PX1(j,1)
           PX2(j,2)=PX1(j,2)
           PX2(j,3) = GAMMA*(-PLAB*PX1(j,4)+ELAB*PX1(j,3))
           PX2(j,4) = GAMMA*( ELAB*PX1(j,4)-PLAB*PX1(j,3))
           PX2(j,5)=PX1(j,5)
         ENDDO

*...defs for Breit-Wigner measure
         PMAV = PX2(I,5)
         WIDTH = PMAS(ABS(ABS(KCp)),2)
         WBAND = PMAS(ABS(ABS(KCp)),3)
         PMUP = WBAND+PMAV
         PMDN =-WBAND+PMAV
         ATL=ATAN((PMDN**2-PMAV**2)/(PMAV*WIDTH))
         ATU=ATAN((PMUP**2-PMAV**2)/(PMAV*WIDTH))
       
*...BW smearing of the mass of Ith particle
555      PM1=sqrt(PMAV**2+PMAV*WIDTH*TAN(ATL+PYR(0)*(ATU-ATL)))
         PX3(I,5)=PM1                         ! new mass of Ith particle

*...new energy of the Ith particle in partonic CMS
         E1N = (PM1**2-PM2**2 + SHAT)/2.d0/SQRT(SHAT)
         IF(E1N.le.PM1) goto 555   !check masses
         PX3(I,4)=E1N
*...old and new energy of the recoiled cluster in partonic CMS
         E2  = sqrt(SHAT)-PX2(I,4)
         E2N = sqrt(SHAT)-E1N
         IF(E2N.le.PM2) goto 555   !check masses

*...old space momenta of the Ith particle and recoiled cluster in partonic CMS
         PMOD=sqrt(PX2(I,1)**2+ PX2(I,2)**2+PX2(I,3)**2)	  
*...new space momenta of the Ith particle and recoiled cluster in partonic CMS
         PMODN=sqrt(E1N-PM1)*sqrt(E1N+PM1)
*...new space components of the Ith momentum in partonic CMS via rescaling
         DO k=1,3
           PXX(I,k)=PX2(I,k)/PMOD
           PX3(I,k)=PXX(I,k)*PMODN
         ENDDO

*...new momentum of the recoiled particle in 2->2 case
         IF(NUP.eq.4) THEN
           IF(I.eq.3) I2=4
           IF(I.eq.4) I2=3
           PX3(I2,1)=-PX3(I,1)
           PX3(I2,2)=-PX3(I,2)
           PX3(I2,3)=-PX3(I,3)
           PX3(I2,4)= E2N
           PX3(I2,5)= PM2
           GOTO 333
         ENDIF

*...new momenta for other out particles (neq Ith)
         GAMMA = 1.d0/sqrt((E2-PMOD)*(E2N+PMODN))
     &        *1.d0/sqrt((E2+PMOD)*(E2N-PMODN))
         DO j=3,NUP
           IF(j.ne.I) then
             PJL=-(PX2(j,1)*PXX(I,1)+PX2(j,2)*PXX(I,2)
     &           +PX2(j,3)*PXX(I,3))
             PJLN=GAMMA*(-(E2N*PMOD-E2*PMODN)*PX2(j,4)
     &            +(E2*E2N-PMOD*PMODN)*PJL)
             PX3(j,4)=GAMMA*((E2*E2N-PMOD*PMODN)*PX2(j,4)
     &                -(E2N*PMOD-E2*PMODN)*PJL)
             PX3(j,1)=PX2(j,1)-(PJLN-PJL)*PXX(I,1)
             PX3(j,2)=PX2(j,2)-(PJLN-PJL)*PXX(I,2)
             PX3(j,3)=PX2(j,3)-(PJLN-PJL)*PXX(I,3)
             PX3(j,5)=PX2(j,5) 
           ENDIF
         ENDDO

333      CONTINUE
*...boost from partonic CMS to the lab frame
         GAMMA = 1.d0/(sqrt(ELAB-PLAB)*sqrt(ELAB+PLAB))
         DO j=3,NUP
           PUP(1,j)=PX3(j,1)
           PUP(2,j)=PX3(j,2)
           PUP(3,j) = GAMMA*(PLAB*PX3(j,4)+ELAB*PX3(j,3))
           PUP(4,j) = GAMMA*(ELAB*PX3(j,4)+PLAB*PX3(j,3))
           PUP(5,j)=PX3(j,5)
         ENDDO
      ENDIF                     ! IF(PMAS(ABS(KCp),2).gt.0.)
      ENDDO                     ! DO I=3,NUP

      RETURN
      END
*************************************************************************
*                          UPEVNT routine                               *
*          for CompHEP 41.10  -  PYTHIA 6.2 interface                   *
*************************************************************************
*  25.11.2001       Ilyin V.A.                                          *
*************************************************************************
      SUBROUTINE UPEVNT
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &VTIMUP(MAXNUP),SPINUP(MAXNUP)
      SAVE /HEPEUP/

      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)

*...CompHEP-PYTHIA interface
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .               NEV(100),XM(100,20,3),SIGMA(100),NPROC
      COMMON/BEAMS/SQRTS,RPDTY,KFB(2),BMASS(2),BEAM(2)

      INTEGER IS,NEVPY,FIREVT      
      REAL*8 PP(20,4)
      CHARACTER EVFILE*500,PROCESS*80,LINE*500
      CHARACTER BEAM*16
      character chain*60, K1*1, K2*1, XN*2      
      COMMON/CCHAIN/ICC(100,2,20)
      COMMON/ISMEAR/ISMEAR
      COMMON/INPARM/IS,NEVPY,FIREVT,PROCESS,EVFILE
      COMMON/QCDPAR/QCDLF(6),NNL
      COMMON/RANDM0/IDUM
      LOGICAL FIRST
      DATA FIRST/.TRUE./
      SAVE
       
       IPSGN = 1
*...if BEAM(1)=BEAM(2) the simmetrization Z->-Z is made
      IF (KFB(1).eq.KFB(2)) THEN
       IF (FIRST) THEN
         FIRST = .FALSE.
         IDUM = -1
       ENDIF
       XRND = pyr(0)
       IF (XRND.lt.0.5d0) IPSGN=-1
       IF (XRND.lt.0.d0 .or. XRND.ge.1.d0) THEN
            write (*,*) 'RAN2 out of [0,1)'
            stop
       ENDIF
      ENDIF

*...read current event
      PP(1,1)=0d0
      PP(1,2)=0d0
      PP(2,1)=0d0
      PP(2,2)=0d0
      READ(IS,FMT='(A500)',END=1000,ERR=1000) LINE  
      READ(LINE,FMT='(i6)') NPRC
      Num=3*(NNUP(NPRC)-2)+2
      CALL ITOCHR(XN,NUM)
      READ(LINE,FMT='(i6,'//XN//'G18.10,G12.3,1x,A)',END=1000,
     .ERR=1000)  NPRC,PP(1,3),PP(2,3),((PP(N,J),J=1,3),N=3,NNUP(NPRC)),
     .QUP,chain

*... random phi generation 
      PI      = DACOS(-1d0)
      PHI_RND = 2D0*PI*PYR(0)
      DO N=3,NNUP(NPRC)
        XX=PP(N,1)
	YY=PP(N,2)
	PP(N,1)=XX*COS(PHI_RND)-YY*SIN(PHI_RND)
	PP(N,2)=XX*SIN(PHI_RND)+YY*COS(PHI_RND)
      ENDDO

*...the number of particle entries in the current event
      NUP =  NNUP(NPRC)
*...the identity of the current subprocess
      IDPRUP = LPRUP(NPRC)
*...the event weight
      XWGTUP = 1.d0
*...scale Q of the event (factorization scale)
      SCALUP = QUP
*...the QED coupling used
      AQEDUP = -1    ! not QED process
*...the QCD coupling used for this event (ME)
      AQCDUP = ALPHAS2(SCALUP,QCDLF(5),NNL)  ! the same as factorization scale
*...particle identity code
      DO 100 i=1,NUP
        IDUP(i) = KFP(nprc,i)
100   CONTINUE
*...status code of particle
      ISTUP(1) = -1
      ISTUP(2) = -1
      DO 110 i=3,NUP
        ISTUP(i) = 1
110   CONTINUE
*...position of first and last mother of particle
       MOTHUP(1,1) = 0 
       MOTHUP(2,1) = 0 
       MOTHUP(1,2) = 0 
       MOTHUP(2,2) = 0 
      DO 120 i=3,NUP
       MOTHUP(1,i) = 1 
       MOTHUP(2,i) = 2
120   CONTINUE
*...the particle momenta
      DO 200 NN=1,NUP
        PUP(1,NN)=PP(NN,1)
        PUP(2,NN)=PP(NN,2)
        PUP(3,NN)=PP(NN,3)*IPSGN
        PUP(4,NN)=SQRT(PP(NN,1)**2+PP(NN,2)**2+PP(NN,3)**2
     .                 +XM(nprc,NN,1)**2)
        PUP(5,NN)=XM(nprc,NN,1)
200   CONTINUE
*...invariant lifetime
      DO 300 i=1,NUP
       VTIMUP(i) = 0.d0
300   CONTINUE
*...spin information
      DO 400 i=1,NUP
       SPINUP(i) = 9
400   CONTINUE


*...swap Z axis for momenta if BEAM(1)=BEAM(2)
        IF (IPSGN.eq.-1) THEN
          IDUPx   = IDUP(1)
          IDUP(1) = IDUP(2)
          IDUP(2) = IDUPx
          DO 135 I=1,5
             XPUP     = PUP(I,1)
             PUP(I,1) = PUP(I,2)
135          PUP(I,2) = XPUP
        ENDIF

*...Breit-Wigner smearing of masses for unstable particles in OUT state if yes
       IF (ISMEAR.eq.1) CALL SMEARMASS

*...set color flows
      DO 10 I=1,30
       IF (CHAIN(I:I+1).EQ.') ') LENGTH=I
10    CONTINUE       
      ITAG = 500
      DO 111 I=1,NUP
        ICOLUP(1,i) = 0
        ICOLUP(2,i) = 0
111   CONTINUE
      DO 222 I=1,LENGTH,5
        K1=CHAIN(I+1:I+1)
        N1=NUMBER(K1)
        K2=CHAIN(I+3:I+3)
        N2=NUMBER(K2)
        IF (IPSGN.eq.-1) THEN
          IF (N1.eq.1) THEN
                 N1=2
          ELSEIF (N1.eq.2) THEN
                 N1=1
          ENDIF
          IF (N2.eq.1) THEN
                 N2=2
          ELSEIF (N2.eq.2) THEN
                 N2=1
          ENDIF
        ENDIF
        IF (N1.le.2) THEN
          ICOLUP(1,N1) = ITAG
        ELSE
          ICOLUP(2,N1) = ITAG
        ENDIF
        IF (N2.le.2) THEN
          ICOLUP(2,N2) = ITAG
        ELSE
          ICOLUP(1,N2) = ITAG
        ENDIF
        ITAG = ITAG+1
222   CONTINUE

      RETURN

*...EOF or error in reading events from CompHEP output
1000  NUP = 0

      RETURN
      END


      FUNCTION NUMBER(PARTON)
      CHARACTER PARTON*1
      IF (PARTON.EQ.'1') THEN 
      NUMBER=1
       ELSEIF(PARTON.EQ.'2') THEN        
       NUMBER=2
        ELSEIF(PARTON.EQ.'3') THEN         
	NUMBER=3
         ELSEIF(PARTON.EQ.'4') THEN          
	 NUMBER=4
          ELSEIF(PARTON.EQ.'5') THEN 	  
	  NUMBER=5
	   ELSEIF(PARTON.EQ.'6') THEN
	   NUMBER=6
	    ELSEIF(PARTON.EQ.'7') THEN
	    NUMBER=7
	     ELSEIF(PARTON.EQ.'8') THEN
	     NUMBER=8
	      ELSEIF(PARTON.EQ.'9') THEN
              NUMBER=9  
	      ELSE
	      NUMBER=10
      ENDIF
      END 
	 

*******************************************************
*                    FUNCTION                         *
* Conversion of integer number into character string  *
*******************************************************
*   Input:  ICHR - output string,                     *
*           JJ - integer number under the conversion; *
*   Output: ITOCHR - the character string,            *
*             declare ICHR at least as                *
*                CHARACTER*LENINT(JJ)                 *
*******************************************************
      SUBROUTINE ITOCHR(ICHR,JJ)
      IMPLICIT REAL*8(A-H,O-Z),INTEGER(I-N) 
      CHARACTER ICHR*(*)
      CHARACTER XFMT*10,XCC

      ICHR=' '
      WRITE(XCC,FMT='(I1)') LENINT(JJ)
      XFMT='(I'//XCC//')'
      WRITE(ICHR,FMT=XFMT,err=1) JJ

      goto 9000
1     write(*,*) 'Error in ITOCHR.'
      read(*,*)

9000  RETURN
      END

********************************************
*           FUNCTION                       *
*     Length of the integer number         *
********************************************
      INTEGER FUNCTION LENINT(JJ)
      IMPLICIT REAL*8(A-H,O-Z),INTEGER(I-N)
      KK=1
10    CONTINUE
      IF (JJ .LT. 10**KK) THEN
         LLL=KK
         GOTO 20
      ENDIF
      KK=KK+1 
      GOTO 10

20    LENINT=LLL
      RETURN
      END


*************************************************************************
*                          UPINIT routine                               *
*          for CompHEP 41.10  -  PYTHIA 6.2 interface                   *
*************************************************************************
*  25.11.2001         Ilyin V.A.                                        *
*************************************************************************

      SUBROUTINE UPINIT
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
      IMPLICIT INTEGER(I-N)
C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &LPRUP(MAXPUP)
      SAVE /HEPRUP/
*...CompHEP-PYTHIA interface
      CHARACTER BEAM*16
      COMMON/BEAMS/SQRTS,RPDTY,KFB(2),BMASS(2),BEAM(2)
      COMMON/SUBPROC/NNUP(100),KFP(100,20),
     .               NEV(100),XM(100,20,3),SIGMA(100),NPROC

*...PDG codes for beams
      IDBMUP(1)=KFB(1)     
      IDBMUP(2)=KFB(2)     
*...energies of beams (in PYTHI beams are massless !)
      EBMUP(1) = SQRTS/2d0*(COSH(RPDTY)+SINH(RPDTY))
      EBMUP(2) = SQRTS/2d0*(COSH(RPDTY)-SINH(RPDTY))
*...PDFLIB group and set of the parton distributions
***     already done
*...events weighting
      IDWTUP = 3     !unweighted input
*...number of subprocesses in input
      NPRUP = nproc
*...cross section for each subprocess
      DO 100 i=1,NPRUP
       XSECUP(i) = SIGMA(i)
100   CONTINUE
*...statistcal error on the cross sections
      DO 110 i=1,NPRUP
       XERRUP(i) = 0.d0
110   CONTINUE
*...maximum event weight
      DO 120 i=1,NPRUP
       XMAXUP(i) = 1.d0
120   CONTINUE
*...unique integer identifier for each subprocess
      DO 130 i=1,NPRUP
       LPRUP(i) = i
130   CONTINUE

      RETURN
      END

