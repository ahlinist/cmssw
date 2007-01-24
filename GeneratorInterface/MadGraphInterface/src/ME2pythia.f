C******************************************************
C*          MadEvent - Pythia interface.              *
C*            Version 4.1, 13 Dec 2006                *
C*                                                    *
C*  - Possibility to use several files                *
C*                                                    *
C*  Version 4.0                                       *
C*                                                    *
C*  - Routines for matching of ME and PS              *
C*                                                    *
C*  Version 3.7                                       *
C*                                                    *
C*  - Set mass of massless outgoing particles to      *
C*    Pythia mass (PMAS(I,1))                         *
C*                                                    *
C*  Version 3.6                                       *
C*                                                    *
C*  - Removed the 1st # from the event file header    *
C*                                                    *
C*  Version 3.5                                       *
C*                                                    *
C*  - Reads according to the new LH event file format *
C*  - Now only LNHIN, LNHOUT and MSCAL in UPPRIV      *
C*                                                    *
C*  Version 3.4                                       *
C*                                                    *
C*  - Reads particle masses from event file           *
C*                                                    *
C*  Version 3.3                                       *
C*                                                    *
C*  - Added option MSCAL in common block UPPRIV to    *
C*    choose between fix (0) or event-based (1)       *
C*    scale for Pythia parton showering (SCALUP).     *
C*  - Fixed bug in reading the SLHA file              *
C*                                                    *
C*  Version 3.2                                       *
C*                                                    *
C*  - Reading the SLHA format param_card from the     *
C*    banner                                          *
C*  - Added support for lpp1/lpp2 = 2 or 3            *
C*  - Removed again support for different MadEvent    *
C*    processes in different files (no longer         *
C*    necessary with new multiple processes support   *
C*    in MadGraph/MadEvent                            *
C*                                                    *
C*  Version 3.1                                       *
C*  - Added support for different MadEvent processes  *
C*    in different files                              *
C*  - Fixed bug in e+e- collisions                    *
C*                                                    *
C*     Written by J.Alwall, alwall@fyma.ucl.ac.be     *
C*      Earlier versions by S.Mrenna, M.Kirsanov      *
C*                                                    *
C******************************************************
C*                                                    *
C* Instructions:                                      *
C* Please use the common block UPPRIV:                *
C* - The logical unit LNHIN must be an opened         *
C*   MadEvent event file                              *
C* - The output unit LNHOUT is by default 6 (std out) *
C* - Set MSCAL to 1 if a dynamical scale is desired   *
C*   for parton showers rather than the one given as  *
C*   factorization scale by MadEvent (otherwise 0)    *
C* - ICKKW is set automatically depending on whether  *
C*   the events generated are matched or not          *
C*                                                    *
C******************************************************

C*********************************************************************
C...UPINIT
C...Routine called by PYINIT to set up user-defined processes.
C*********************************************************************      
      SUBROUTINE UPINIT
      
      IMPLICIT NONE
      CHARACTER*132 CHAR_READ

C...Pythia parameters.
      INTEGER MSTP,MSTI,MRPY
      DOUBLE PRECISION PARP,PARI,RRPY
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYDATR/MRPY(6),RRPY(100)

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      DATA LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE/77,6,1,0,1/
      SAVE/UPPRIV/

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,qfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,qfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Parameter arrays (local)
      integer maxpara
      parameter (maxpara=100)
      integer npara,iseed
      character*20 param(maxpara),value(maxpara)      

C...Lines to read in assumed never longer than 200 characters. 
      INTEGER MAXLEN,IBEG,IPR,I
      PARAMETER (MAXLEN=200)
      CHARACTER*(MAXLEN) STRING

C...add opening of file as done presently in CMSSW. DKC
      CHARACTER EVFILE*500

C...Format for reading lines.
      CHARACTER*6 STRFMT
      STRFMT='(A000)'
      WRITE(STRFMT(3:5),'(I3)') MAXLEN

C connects unit 22 w file inparm.dat and reads from unit 22, fmt=format, A=
C caracter, 500=500 characters.first read reads one line of max 500 characters
C then read second line into evfile, third line into dummy and fourth into process
      OPEN(22,FILE='MGinput.dat',STATUS='UNKNOWN')
      READ(22,FMT='(A500)') EVFILE
      WRITE(*,*)' ',EVFILE
      OPEN(LNHIN,FILE=EVFILE,STATUS='UNKNOWN')

C...Extract the model parameter card and read it.
      CALL MODELPAR(LNHIN)

c...Read the <init> block information

C...Loop until finds line beginning with "<init>" or "<init ". 
  100 READ(LNHIN,STRFMT,END=130,ERR=130) STRING
C...Pick out random number seed and use for PYR initialization
      IF(INDEX(STRING,'iseed').NE.0)THEN
         READ(STRING,*) iseed
         IF(iseed.gt.0) THEN
            WRITE(LNHOUT,*) 'Initializing PYR with random seed ',iseed
            MRPY(1) = iseed
            MRPY(2) = 0
         ENDIF
      ENDIF
      IBEG=0
  110 IBEG=IBEG+1
C...Allow indentation.
      IF(STRING(IBEG:IBEG).EQ.' '.AND.IBEG.LT.MAXLEN-5) GOTO 110 
      IF(STRING(IBEG:IBEG+5).NE.'<init>'.AND.
     &STRING(IBEG:IBEG+5).NE.'<init ') GOTO 100

C...Read first line of initialization info.
      READ(LNHIN,*,END=130,ERR=130) IDBMUP(1),IDBMUP(2),EBMUP(1),
     &EBMUP(2),PDFGUP(1),PDFGUP(2),PDFSUP(1),PDFSUP(2),IDWTUP,NPRUP

C...Read NPRUP subsequent lines with information on each process.
      DO 120 IPR=1,NPRUP
        READ(LNHIN,*,END=130,ERR=130) XSECUP(IPR),XERRUP(IPR),
     &  XMAXUP(IPR),LPRUP(IPR)
  120 CONTINUE

C...Set PDFLIB or LHAPDF pdf number for Pythia

c       IF(PDFSUP(1).NE.19070)THEN
c c     Not CTEQ5L, which is standard in Pythia
c         MSTP(52)=2
c c     The following works for both PDFLIB and LHAPDF (where PDFGUP(1)=0)
c c     But note that the MadEvent output uses the LHAPDF numbering scheme
c         MSTP(51)=1000*PDFGUP(1)+PDFSUP(1)
c       ENDIF

C...Initialize widths and partial widths for resonances.
      CALL PYINRE
        
C...Calculate xsec reduction due to non-decayed resonances
C...based on first event only!

      CALL BRSUPP

      REWIND(LNHIN)

C...Extract cuts and matching parameters
      CALL read_params(LNHIN,npara,param,value)

      call get_integer(npara,param,value," ickkw ",ickkw,0)

C
C...Set kt clustering scheme (if not already set)
C
      IF(ABS(IDBMUP(1)).EQ.11.AND.ABS(IDBMUP(2)).EQ.11.AND.
     $     IDBMUP(1).EQ.-IDBMUP(2))THEN
         ktsche=1
      ELSE
         ktsche=4313
      ENDIF

C...Enhance primordial kt
      CALL PYGIVE('PARP(91)=2.5')
      CALL PYGIVE('PARP(93)=15')

      CALL set_matching(LNHIN,npara,param,value)

      RETURN

C...Error exit: give up if initalization does not work.
  130 WRITE(*,*) ' Failed to read LHEF initialization information.'
      WRITE(*,*) ' Event generation will be stopped.'
      STOP  
      END

C*********************************************************************      
C...UPEVNT
C...Routine called by PYEVNT or PYEVNW to get user process event
C*********************************************************************
      SUBROUTINE UPEVNT

      IMPLICIT NONE

C...Pythia parameters.
      INTEGER MSTP,MSTI
      DOUBLE PRECISION PARP,PARI
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)
C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)
C...Pythia common blocks
      INTEGER PYCOMP,KCHG,MINT,NPART,NPARTD,IPART,MAXNUR
      DOUBLE PRECISION PMAS,PARF,VCKM,VINT,PTPART
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
      COMMON/PYINT1/MINT(400),VINT(400)
      PARAMETER (MAXNUR=1000)
      COMMON/PYPART/NPART,NPARTD,IPART(MAXNUR),PTPART(MAXNUR)

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,qfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,qfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      COMMON/MEMAEV/NLJETS,IEXC,Ifile

C...Local variables
      INTEGER I,J,IBEG,NEX,KP(MAXNUP),MOTH
      DOUBLE PRECISION PSUM,ESUM,PM1,PM2,A1,A2,A3,A4,A5
      DOUBLE PRECISION SCALLOW(MAXNUP),PTCLUS(MAXNUP),PNONJ(4),PMNONJ!,PT2JETS
C...Lines to read in assumed never longer than 200 characters. 
      INTEGER MAXLEN
      PARAMETER (MAXLEN=200)
      CHARACTER*(MAXLEN) STRING

C...Format for reading lines.
      CHARACTER*6 STRFMT
      CHARACTER*1 CDUM

      STRFMT='(A000)'
      WRITE(STRFMT(3:5),'(I3)') MAXLEN

C...Loop until finds line beginning with "<event>" or "<event ". 
  100 READ(LNHIN,STRFMT,END=130,ERR=130) STRING
      IBEG=0
  110 IBEG=IBEG+1
C...Allow indentation.
      IF(STRING(IBEG:IBEG).EQ.' '.AND.IBEG.LT.MAXLEN-6) GOTO 110 
      IF(STRING(IBEG:IBEG+6).NE.'<event>'.AND.
     &STRING(IBEG:IBEG+6).NE.'<event ') GOTO 100

C...Read first line of event info.
      READ(LNHIN,*,END=130,ERR=130) NUP,IDPRUP,XWGTUP,SCALUP,
     &AQEDUP,AQCDUP

C...Read NUP subsequent lines with information on each particle.
      ESUM=0d0
      PSUM=0d0
      NEX=2
      DO 120 I=1,NUP
        READ(LNHIN,*,END=130,ERR=130) IDUP(I),ISTUP(I),
     &  MOTHUP(1,I),MOTHUP(2,I),ICOLUP(1,I),ICOLUP(2,I),
     &  (PUP(J,I),J=1,5),VTIMUP(I),SPINUP(I)
C...Reset resonance momentum to prepare for mass shifts
        IF(ISTUP(I).EQ.2) PUP(3,I)=0
        IF(ISTUP(I).EQ.1)THEN
          NEX=NEX+1
           IF(PUP(5,I).EQ.0D0.AND.IABS(IDUP(I)).GT.3.AND.IDUP(I).NE.21)
     $       THEN
C...Set massless particle masses to Pythia default. Adjust z-momentum. 
              PUP(5,I)=PMAS(IABS(PYCOMP(IDUP(I))),1)
              PUP(3,I)=SIGN(SQRT(MAX(0d0,PUP(4,I)**2-PUP(5,I)**2-
     $           PUP(1,I)**2-PUP(2,I)**2)),PUP(3,I))
           ENDIF
           PSUM=PSUM+PUP(3,I)
C...Set mother resonance momenta
           MOTH=MOTHUP(1,I)
           DO WHILE (MOTH.GT.2)
             PUP(3,MOTH)=PUP(3,MOTH)+PUP(3,I)
             MOTH=MOTHUP(1,MOTH)
           ENDDO
        ENDIF
  120 CONTINUE

C..Adjust mass of resonances
      DO I=1,NUP
         IF(ISTUP(I).EQ.2)THEN
            PUP(5,I)=SQRT(PUP(4,I)**2-PUP(1,I)**2-PUP(2,I)**2-
     $             PUP(3,I)**2)
         ENDIF
      ENDDO

C...Adjust energy and momentum of incoming particles
C...In massive case need to solve quadratic equation
c      PM1=PUP(5,1)**2
c      PM2=PUP(5,2)**2
c      A1=4d0*(ESUM**2-PSUM**2)
c      A2=ESUM**2-PSUM**2+PM2-PM1
c      A3=2d0*PSUM*A2
c      A4=A3/A1
c      A5=(A2**2-4d0*ESUM**2*PM2)/A1
c
c      PUP(3,2)=A4+SIGN(SQRT(A4**2+A5),PUP(3,2))
c      PUP(3,1)=PSUM-PUP(3,2)
c      PUP(4,1)=SQRT(PUP(3,1)**2+PM1)
c      PUP(4,2)=SQRT(PUP(3,2)**2+PM2)

      ESUM=PUP(4,1)+PUP(4,2)

C...Assuming massless incoming particles - otherwise Pythia adjusts
C...the momenta to make them massless
      IF(IDBMUP(1).GT.100.AND.IDBMUP(2).GT.100)THEN
        DO I=1,2
          PUP(3,I)=0.5d0*(PSUM+SIGN(ESUM,PUP(3,I)))
          PUP(5,I)=0d0
        ENDDO
        PUP(4,1)=ABS(PUP(3,1))
        PUP(4,2)=ESUM-PUP(4,1)
      ENDIF
        
C...If you want to use some other scale for parton showering then the 
C...factorisation scale given by MadEvent, please implement the function PYMASC
C...(example function included below) 

      IF(ickkw.eq.0.AND.MSCAL.GT.0) CALL PYMASC(SCALUP)

C...Read FSR scale for all FS particles (as comment in event file)
      IF(ickkw.eq.1)THEN
c        DO I=1,NLJETS
c          SCALLOW(I)=0
c        ENDDO
c        READ(LNHIN,*,END=130,ERR=101) CDUM,(SCALLOW(I),I=1,NLJETS)
        READ(LNHIN,*,END=130,ERR=101) CDUM,(PTPART(I),I=1,NEX)
 101    CONTINUE
c        CALL ALPSOR(SCALLOW,nljets,KP,1)
      ENDIF

      NLJETS=0
c
c   Set up number of jets and find invariant mass of non-jets
c
c      do j=1,4
c        PNONJ(j)=0d0
c      enddo
c      PT2JETS=0d0
      NPART=0
      do i=3,NUP
        if(ISTUP(i).ne.1) cycle
        NPART=NPART+1
        IPART(NPART)=i
        if(iabs(IDUP(i)).ge.6.and.IDUP(i).ne.21) then
c          do j=1,4
c            PNONJ(j)=PNONJ(j)+PUP(j,i)
c          enddo
          cycle
        endif
        if((MOTHUP(1,i).eq.1.and.MOTHUP(2,i).eq.2).or.
     $     (ABS(IDBMUP(1)).eq.11.AND.ABS(IDBMUP(2)).eq.11.and.
     $     IDBMUP(1).eq.-IDBMUP(2).AND.
     $     (IDUP(MOTHUP(1,I)).eq.22.or.IDUP(MOTHUP(1,I)).eq.23).and.
     $     MOTHUP(1,MOTHUP(1,I)).eq.1.and.MOTHUP(2,MOTHUP(1,I)).eq.2))
     $     then
          NLJETS=NLJETS+1
          SCALLOW(NLJETS)=PTPART(NPART)
c          PT2JETS=PT2JETS+PUP(1,i)**2+PUP(2,i)**2
c        else
c          do j=1,4
c            PNONJ(j)=PNONJ(j)+PUP(j,i)
c          enddo
        endif
      enddo
      CALL ALPSOR(SCALLOW,nljets,KP,1)

      IDPRUP=LPRUP(NLJETS-MINJETS+1)
c      PMNONJ=PNONJ(4)**2-PNONJ(3)**2
c      IF(PMNONJ.GT.0) SCALUP=SQRT(PMNONJ)

      IF(ickkw.eq.1) THEN
c   ... and decide whether exclusive or inclusive
        if(IEXCFILE.eq.0.and.NLJETS.eq.MAXJETS)then
          IEXC=0
        else
          IEXC=1
        endif

C...Need lower scale for final state radiation in e+e-
        VINT(370)=0d0
        IF(IABS(IDBMUP(1)).EQ.11.AND.IABS(IDBMUP(2)).EQ.11.AND.
     $     NLJETS.gt.2) then
          VINT(370)=SCALLOW(1)
        ELSE IF(NLJETS.gt.0)THEN
          if(ISCALE.EQ.1)then
            VINT(370)=SCALLOW(NLJETS)
          else if(ISCALE.EQ.0)then
            VINT(370)=SCALUP
          endif
        ENDIF
      ENDIF

      RETURN

C...Error exit, typically when no more events.
  130 WRITE(*,*) ' Failed to read LHEF event information.'
      WRITE(*,*) ' Will assume end of file has been reached.'
      NUP=0
      RETURN
      END

C*********************************************************************
C...UPVETO
C...Subroutine to implement the MLM jet matching criterion
C*********************************************************************
      SUBROUTINE UPVETO(IPVETO)

      IMPLICIT NONE

C...GUP Event common block
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,
     &              IDUP(MAXNUP),ISTUP(MAXNUP),MOTHUP(2,MAXNUP),
     &              ICOLUP(2,MAXNUP),PUP(5,MAXNUP),VTIMUP(MAXNUP),
     &              SPINUP(MAXNUP)
C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)
C...HEPEVT commonblock.
      INTEGER NMXHEP,NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      PARAMETER (NMXHEP=4000)
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      DOUBLE PRECISION PHEP,VHEP
      SAVE /HEPEVT/
      INTEGER IPVETO
C     CALSIM VARIABLES
      INTEGER NCY,NCPHI
      DOUBLE PRECISION PI,DELY,DELPHI,ET,CTHCAL,STHCAL,CPHCAL,SPHCAL,
     $   YCMIN,YCMAX
      PARAMETER (NCY=50)
      PARAMETER (NCPHI=60,PI=3.141593D0)
      COMMON/CALOR/DELY,DELPHI,ET(NCY,NCPHI),
     $   CTHCAL(NCY),STHCAL(NCY),CPHCAL(NCPHI),SPHCAL(NCPHI),
     $   YCMIN,YCMAX
C     JET VARIABLES                             
      INTEGER NJMAX,JETNO,NCJET
      DOUBLE PRECISION PCJET,ETJET
      PARAMETER (NJMAX=500)
      COMMON/GETCOM/PCJET(4,NJMAX),ETJET(NJMAX),JETNO(NCY,NCPHI),NCJET
C     
      DOUBLE PRECISION PSERAP
      INTEGER K(NJMAX),KP(NJMAX),kpj(njmax)

C...Variables for the kT-clustering
      INTEGER NMAX,NN,NJET,NSUB,JET,NJETM,IHARD,IP1,IP2
      DOUBLE PRECISION PP,PJET
      DOUBLE PRECISION ECUT,Y,YCUT,RAD
      PARAMETER (NMAX=512)
      DIMENSION JET(NMAX),Y(NMAX),PP(4,NMAX),PJET(4,NMAX),
     $   PJETM(4,NMAX)
      INTEGER NNM
      DOUBLE PRECISION YM(NMAX),PPM(4,NMAX),PJETM

C...kt clustering common block
      INTEGER NMAXKT,NUM,HIST
      PARAMETER (NMAXKT=512)
      DOUBLE PRECISION PPP,KT,ETOT,RSQ,KTP,KTS,KTLAST
      COMMON /KTCOMM/ETOT,RSQ,PPP(9,NMAXKT),KTP(NMAXKT,NMAXKT),
     $   KTS(NMAXKT),KT(NMAXKT),KTLAST(NMAXKT),HIST(NMAXKT),NUM

C...Inputs for jet clustering and Pythia run
      INTEGER KTPYSC,NITER,NFILES
      DOUBLE PRECISION ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX,ASFACT
      COMMON/JETPAR/ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX,ASFACT,
     $     KTPYSC,NITER,NFILES

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,qfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,qfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      COMMON/MEMAEV/NLJETS,IEXC,Ifile

      INTEGER nvarev,nvar2
      PARAMETER (nvarev=57,nvar2=6)

      REAL*4 varev(nvarev)
      COMMON/HISTDAT/varev


C   local variables
      integer i,j,ihep,nmatch,jrmin
      double precision etajet,phijet,delr,dphi,delrmin,ptjet
      double precision p(4,10),pt(10),eta(10),phi(10)
      integer idbg
      data idbg/0/
      REAL*4 var2(nvar2)
c
      double precision tiny
      parameter (tiny=1d-3)
      integer icount
      data icount/0/
      INTEGER ISTOLD(NMXHEP),IST,IMO
c      LOGICAL FOUND

c      if(NLJETS.GT.0)then
c        idbg=1
c      else
c        idbg=0
c      endif
c 
c       IPVETO=0
c       IF(ICKKW.EQ.0) RETURN
c 
c C   Set up vetoed mothers
c c      DO I=1,MAXNUP
c c        INORAD(I)=0
c c      ENDDO
c c      DO IHEP=1,NUP-2      
c c        if(ISTHEP(ihep).gt.1.and.iabs(IDHEP(ihep)).gt.8) then
c c        if(iabs(IDHEP(ihep)).gt.5.and.IDHEP(ihep).ne.21) then
c c          INORAD(ihep)=1
c c        endif
c c      ENDDO
c 
c C     Prepare histogram filling
c         DO I=1,4
c           var2(1+I)=-1
c           varev(46+I)=-1
c           varev(50+I)=-1
c         ENDDO
c 
c C CHECK FOR EVENT ERROR OR ZERO WGT
c       I=0
c C
c c     reconstruct parton-level event
c 
c       if(idbg.eq.1) then
c         write(LNHOUT,*) ' '
c         write(LNHOUT,*) 'new event '
c c        CALL PYLIST(1)
c         CALL PYLIST(7)
c         CALL PYLIST(5)
c         write(LNHOUT,*) 'PARTONS'
c       endif
c       i=0
c       do ihep=3,nup
c         if(ISTUP(ihep).ne.1.or.
c      $     (iabs(IDUP(ihep)).ge.6.and.IDUP(ihep).ne.21)) cycle
c         if(.NOT.((MOTHUP(1,ihep).eq.1.and.MOTHUP(2,ihep).eq.2).or.
c      $     (ABS(IDBMUP(1)).eq.11.AND.ABS(IDBMUP(2)).eq.11.and.
c      $     IDBMUP(1).eq.-IDBMUP(2).AND.
c      $     (IDUP(MOTHUP(1,IHEP)).eq.22.or.
c      $     IDUP(MOTHUP(1,IHEP)).eq.23).and.
c      $     MOTHUP(1,MOTHUP(1,IHEP)).eq.1.and.
c      $     MOTHUP(2,MOTHUP(1,IHEP)).eq.2)))
c      $     cycle
c         i=i+1
c         do j=1,4
c           p(j,i)=pup(j,ihep)
c         enddo
c         pt(i)=sqrt(p(1,i)**2+p(2,i)**2)
c         if(i.LE.4) varev(50+i)=pt(i)
c         eta(i)=-log(tan(0.5d0*atan2(pt(i)+tiny,p(3,i))))
c c        if(abs(eta(i)).gt.2.5d0)then
c c          write(LNHOUT,*)'Jet with eta > 2.5: ',eta(i)
c c          write(LNHOUT,*)'Skipping event'
c c          goto 999
c c        endif
c         phi(i)=atan2(p(2,i),p(1,i))
c         if(idbg.eq.1) then
c           write(LNHOUT,*) pt(i),eta(i),phi(i)
c         endif
c       enddo
c       if(i.ne.NLJETS)then
c         print *,'Error in UPVETO: Wrong number of jets found ',i,NLJETS
c         CALL PYLIST(7)
c         CALL PYLIST(2)
c         stop
c       endif
c 
c C Bubble-sort PTs in descending order
c       DO I=1,3
c          DO J=4,I+1,-1
c             IF(varev(50+J).GT.varev(50+I))THEN
c                PTJET=varev(50+J)
c                varev(50+J)=varev(50+I)
c                varev(50+I)=PTJET
c             ENDIF
c          ENDDO
c       ENDDO
c 
c       if(idbg.eq.1) then
c         do i=1,nhep
c           write(LNHOUT,111) i,isthep(i),idhep(i),jmohep(1,i),jmohep(2,i)
c      $         ,phep(1,i),phep(2,i),phep(3,i)
c         enddo
c  111  format(5(i4,1x),3(f12.5,1x))
c       endif
c 
c       IF(qcut.le.0d0)then
c c      CALL PYLIST(7)
c c      CALL PYLIST(2)
c c      CALL PYLIST(5)
c c     Start from the partonic system
c       IF(NLJETS.GT.0) CALL ALPSOR(pt,nljets,KP,2)  
c c     reconstruct showered jets
c c     
c       YCMAX=ETACLMAX+RCLMAX
c       YCMIN=-YCMAX
c       CALL CALINI
c       CALL CALDEL(1,1)
c       CALL GETJET(RCLMAX,ETCJET,ETACLMAX)
c c     analyse only events with at least nljets-reconstrcuted jets
c       IF(NCJET.GT.0) CALL ALPSOR(ETJET,NCJET,K,2)              
c       if(idbg.eq.1) then
c         write(LNHOUT,*) 'JETS'
c         do i=1,ncjet
c           j=k(ncjet+1-i)
c           ETAJET=PSERAP(PCJET(1,j))
c           PHIJET=ATAN2(PCJET(2,j),PCJET(1,j))
c           write(LNHOUT,*) etjet(j),etajet,phijet
c         enddo
c       endif
c       IF(NCJET.LT.NLJETS) THEN
c         if(idbg.eq.1)
c      $     WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' < ',NLJETS
c         GOTO 999
c       endif
c c     associate partons and jets, using min(delr) as criterion
c       NMATCH=0
c       DO I=1,NCJET
c         KPJ(I)=0
c       ENDDO
c       DO I=1,NLJETS
c         DELRMIN=1D5
c         DO 110 J=1,NCJET
c           IF(KPJ(J).NE.0) GO TO 110
c           ETAJET=PSERAP(PCJET(1,J))
c           PHIJET=ATAN2(PCJET(2,J),PCJET(1,J))
c           DPHI=ABS(PHI(KP(NLJETS-I+1))-PHIJET)
c           IF(DPHI.GT.PI) DPHI=2.*PI-DPHI
c           DELR=SQRT((ETA(KP(NLJETS-I+1))-ETAJET)**2+(DPHI)**2)
c           IF(DELR.LT.DELRMIN) THEN
c             DELRMIN=DELR
c             JRMIN=J
c           ENDIF
c  110    CONTINUE
c         IF(DELRMIN.LT.1.5*RCLMAX) THEN
c           NMATCH=NMATCH+1
c           KPJ(JRMIN)=I
c         ENDIF
c C     WRITE(*,*) 'PARTON-JET',I,' best match:',k(ncjet+1-jrmin)
c c     $           ,delrmin
c       ENDDO
c       IF(NMATCH.LT.NLJETS)  THEN
c         if(idbg.eq.1)
c      $     WRITE(LNHOUT,*) 'Failed due to NMATCH ',NMATCH,' < ',NLJETS
c         GOTO 999
c       endif
c C REJECT EVENTS WITH LARGER JET MULTIPLICITY FROM EXCLUSIVE SAMPLE
c       IF(NCJET.GT.NLJETS.AND.IEXC.EQ.1)  THEN
c         if(idbg.eq.1)
c      $     WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' > ',NLJETS
c         GOTO 999
c       endif
c       RETURN
c 
c       else                      ! qcut.gt.0
c 
c C---FIND FINAL STATE COLOURED PARTICLES
c         NN=0
c         DO IHEP=1,NHEP
c           IF (ISTHEP(IHEP).EQ.1
c      &       .AND.(ABS(IDHEP(IHEP)).LT.6.OR.IDHEP(IHEP).EQ.21)) THEN
c             PTJET=sqrt(PHEP(1,IHEP)**2+PHEP(2,IHEP)**2)
c             ETAJET=ABS(LOG(MIN((SQRT(PTJET**2+PHEP(3,IHEP)**2)+
c      $       ABS(PHEP(3,IHEP)))/PTJET,1d5)))
c             IF(ETAJET.GT.etaclmax) cycle
c             NN=NN+1
c             IF (NN.GT.NMAX) then
c               CALL PYLIST(2)
c               PRINT *, 'Too many particles: ', NN
c               STOP
c             endif
c             DO I=1,4
c               PP(I,NN)=PHEP(I,IHEP)
c             ENDDO
c           ELSE if(idbg.eq.1)THEN
c             PRINT *,'Skipping particle ',IHEP,ISTHEP(IHEP),IDHEP(IHEP)
c           ENDIF
c         ENDDO
c 
c C---CLUSTER THE EVENT (DEFINING ECUT AS 1, SO Y IS MEASURED IN GEV**2)
c C   USING THE PT-SCHEME
c         ECUT=1
c         IF (NN.GT.1) then
c           CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)
c           if(idbg.eq.1)
c      $       WRITE(LNHOUT,*) 'Clustering values:',
c      $       (SQRT(Y(i)),i=1,MIN(NN,3))
c 
c C       Print out values in the case where all jets are matched at the
c C       value of the NLJETS:th clustering
c         var2(1)=NLJETS
c         var2(6)= Ifile
c 
c         if(NLJETS.GT.MINJETS)then
c           YCUT=Y(NLJETS)
c           CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
c      $       NCJET,NSUB,*999)        
c 
c C     Cluster jets with first hard parton
c           DO I=1,NLJETS
c             DO J=1,4
c               PPM(J,I)=PJET(J,I)
c             ENDDO
c           ENDDO
c           
c           NJETM=NLJETS
c           DO IHARD=1,NLJETS
c             NNM=NJETM+1
c             DO J=1,4
c               PPM(J,NNM)=p(J,IHARD)
c             ENDDO
c             CALL KTCLUS(KTSCHE,PPM,NNM,ECUT,YM,*999)
c             IF(YM(NNM).GT.YCUT) THEN
c C       Parton not clustered
c               GOTO 90
c             ENDIF
c             
c C       Find jet clustered with parton
c 
c             IP1=HIST(NNM)/NMAXKT
c             IP2=MOD(HIST(NNM),NMAXKT)
c             IF(IP2.NE.NNM.OR.IP1.LE.0)THEN
c               GOTO 90
c             ENDIF
c             DO I=IP1,NJETM-1
c               DO J=1,4
c                 PPM(J,I)=PPM(J,I+1)
c               ENDDO
c             ENDDO
c             NJETM=NJETM-1
c           ENDDO                 ! IHARD=1,NLJETS
c         endif                   ! NLJETS.GT.MINJETS
c 
c         DO I=1,MIN(NN,4)
c           var2(1+I)=SQRT(Y(I))
c         ENDDO
c         WRITE(15,4001) (var2(I),I=1,nvar2)
c 
c  90     CONTINUE
c 
c C   Now perform jet clustering at the value chosen in qcut
c 
c         CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)
c 
c         YCUT=qcut**2
c         NCJET=0
c           
c C---RECONSTRUCT THE MOMENTA OF THE 4 HARDEST JETS
c C   USING THE E-SCHEME (IE SIMPLE VECTOR ADDITION)
c C---NOTE THAT THE VALUE OF NJET SHOULD BE CHECKED AFTERWARDS, AS ONE
c C   OCCASIONALLY ENCOUNTERS NON-MONOTIC SEQUENCES OF Y().
c C   ALTHOUGH THESE ARE EXTREMELY RARE, WE HAVEN'T DETERMINED EXACTLY
c C   WHAT BEST TO DO ABOUT THEM YET!
c C---THE VALUES OF JET AND NSUB, TOGETHER WITH THE SECOND Y-SCALE
c C   CAN BE IGNORED FOR SIMPLE ANALYSES
c           CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
c      $       NCJET,NSUB,*999)        
c         ELSE IF (NN.EQ.1) THEN
c           Y(1)=PP(1,1)**2+PP(2,1)**2
c           IF(Y(1).GT.YCUT)THEN
c             NCJET=1
c             DO I=1,4
c               PJET(I,1)=PP(I,1)
c             ENDDO
c           ENDIF
c         endif
c 
c         if(idbg.eq.1) then
c           write(LNHOUT,*) 'JETS'
c           do i=1,ncjet
c             PTJET =SQRT(PJET(1,i)**2+PJET(2,i)**2)
c             ETAJET=PSERAP(PJET(1,i))
c             PHIJET=ATAN2(PJET(2,i),PJET(1,i))
c             write(LNHOUT,*) ptjet,etajet,phijet
c           enddo
c         endif
c 
c         IF(NCJET.LT.NLJETS) THEN
c           if(idbg.eq.1)
c      $       WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' < ',NLJETS
c           GOTO 999
c         endif
c 
c C...Right number of jets - but the right jets?        
c C     For max. multiplicity case, count jets only to the NHARD:th jet
c         IF(IEXC.EQ.0)THEN
c c          YCUT=Y(NLJETS)
c c          CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
c c     $       NCJET,NSUB,*999)
c         ELSE IF(NCJET.GT.NLJETS) THEN
c           if(idbg.eq.1)
c      $       WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' > ',NLJETS
c           GOTO 999
c         ENDIF
c 
c C     Cluster jets with first hard parton
c         DO I=1,NLJETS
c           DO J=1,4
c             PPM(J,I)=PJET(J,I)
c           ENDDO
c         ENDDO
c 
c         NJETM=NLJETS
c         DO 120 IHARD=1,NLJETS
c c          IC=K(ICMSHOW+IHARD,2)
c c          IF(IABS(MOD(IC,1000000)).GT.6.AND.
c c     $     MOD(IC,1000000).NE.21) GOTO 110
c           NN=NJETM+1
c           DO J=1,4
c             PPM(J,NN)=p(J,IHARD)
c           ENDDO
c           CALL KTCLUS(KTSCHE,PPM,NN,ECUT,Y,*999)
c c          YCUT=(1.5*qcut)**2
c           YCUT=qcut**2
c           IF(Y(NN).GT.YCUT) THEN
c C       Parton not clustered
c           if(idbg.eq.1)
c      $       WRITE(LNHOUT,*) 'Failed due to parton ',IHARD,
c      $         ' not clustered.'
c             GOTO 999
c           ENDIF
c           
c C       Find jet clustered with parton
c 
c           IP1=HIST(NN)/NMAXKT
c           IP2=MOD(HIST(NN),NMAXKT)
c           IF(IP2.NE.NN.OR.IP1.LE.0)THEN
c           if(idbg.eq.1)
c      $       WRITE(LNHOUT,*) 'Failed due to parton ',IHARD,
c      $         ' not clustered.'
c             GOTO 999
c           ENDIF
c           DO I=IP1,NJETM-1
c             DO J=1,4
c               PPM(J,I)=PPM(J,I+1)
c             ENDDO
c           ENDDO
c           NJETM=NJETM-1
c  120    CONTINUE
c 
c       endif
c 
c C---FIND FINAL STATE COLOURED PARTICLES
c       NN=0
c       DO IHEP=1,NHEP
c          IF (ISTHEP(IHEP).EQ.1
c      &        .AND.(ABS(IDHEP(IHEP)).LT.6.OR.IDHEP(IHEP).EQ.21)) THEN
c             PTJET=sqrt(PHEP(1,IHEP)**2+PHEP(2,IHEP)**2)
c             ETAJET=ABS(LOG(MIN((SQRT(PTJET**2+PHEP(3,IHEP)**2)+
c      $           ABS(PHEP(3,IHEP)))/PTJET,1d5)))
c             IF(ETAJET.GT.ETAMAX) cycle
c             NN=NN+1
c             IF (NN.GT.NMAX) then
c                CALL PYLIST(2)
c                PRINT *, 'Too many particles: ', NN
c               STOP
c            endif
c            DO I=1,4
c               PP(I,NN)=PHEP(I,IHEP)
c            ENDDO
c         ELSE if(idbg.eq.1)THEN
c            PRINT *,'Skipping particle ',IHEP,ISTHEP(IHEP),IDHEP(IHEP)
c         ENDIF
c       ENDDO
c 
c C---CLUSTER THE EVENT (DEFINING ECUT AS 1, SO Y IS MEASURED IN GEV**2)
c C   USING THE PT-SCHEME
c       ECUT=1
c       IF (NN.GT.1) THEN
c          CALL KTCLUS(KTPYSC,PP,NN,ECUT,Y,*999)
c       ELSE IF(NN.EQ.1) THEN
c          Y(1)=SQRT(PP(1,NN)**2+PP(2,NN)**2)
c       ENDIF
c 
c       DO I=1,MIN(NN,4)
c          varev(46+I)=SQRT(Y(I))
c       ENDDO
c 
c 
c       RETURN
c  4001 FORMAT(50E15.6)
c c HERWIG/PYTHIA TERMINATION:
c  999  IPVETO=1
      END

C*********************************************************************
C   PYMASC
C   Implementation of scale used in Pythia parton showers
C*********************************************************************
      SUBROUTINE PYMASC(scale)
      IMPLICIT NONE

C...Arguments
      REAL*8 scale

C...Functions
      REAL*8 SMDOT5

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)
C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE

C...Local variables
      INTEGER ICC1,ICC2,IJ,IDC1,IDC2,IC,IC1,IC2
      REAL*8 QMIN,QTMP

C   Just use the scale read off the event record
      scale=SCALUP

C   Alternatively:

C...  Guesses for the correct scale
C     Assumptions:
C     (1) if the initial state is a color singlet, then
C     use s-hat for the scale
C     
C     (2) if color flow to the final state, use the minimum
C     of the dot products of color connected pairs
C     (times two for consistency with above)

        QMIN=SMDOT5(PUP(1,1),PUP(1,2))
        ICC1=1
        ICC2=2
C     
C     For now, there is no generic way to guarantee the "right"
C     scale choice.  Here, we take the HERWIG pt. of view and
C     choose the dot product of the colored connected "primary"
C     pairs.
C     

        DO 101 IJ=1,NUP
          IF(MOTHUP(2,IJ).GT.2) GOTO 101
          IDC1=ICOLUP(1,IJ)
          IDC2=ICOLUP(2,IJ)
          IF(IDC1.EQ.0) IDC1=-1
          IF(IDC2.EQ.0) IDC2=-2
          
          DO 201 IC=IJ+1,NUP
            IF(MOTHUP(2,IC).GT.2) GOTO 201
            IC1=ICOLUP(1,IC)
            IC2=ICOLUP(2,IC)
            IF(ISTUP(IC)*ISTUP(IJ).GE.1) THEN
              IF(IDC1.EQ.IC2.OR.IDC2.EQ.IC1) THEN
                QTMP=SMDOT5(PUP(1,IJ),PUP(1,IC))
                IF(QTMP.LT.QMIN) THEN
                  QMIN=QTMP
                  ICC1=IJ
                  ICC2=IC
                ENDIF
              ENDIF
            ELSEIF(ISTUP(IC)*ISTUP(IJ).LE.-1) THEN
              IF(IDC1.EQ.IC1.OR.IDC2.EQ.IC2) THEN
                QTMP=SMDOT5(PUP(1,IJ),PUP(1,IC))          
                IF(QTMP.LT.QMIN) THEN
                  QMIN=QTMP
                  ICC1=IJ
                  ICC2=IC
                ENDIF
              ENDIF
            ENDIF
 201      CONTINUE
 101    CONTINUE

        scale=QMIN

      RETURN
      END

C...SMDOT5
C   Helper function

      FUNCTION SMDOT5(V1,V2)
      IMPLICIT NONE
      REAL*8 SMDOT5,TEMP
      REAL*8 V1(5),V2(5)
      INTEGER I

      SMDOT5=0D0
      TEMP=V1(4)*V2(4)
      DO I=1,3
        TEMP=TEMP-V1(I)*V2(I)
      ENDDO

      SMDOT5=SQRT(ABS(TEMP))

      RETURN
      END

C*********************************************************************
      
C...modelpar
C...Checks if model is mssm and extracts SLHA file
C...Reads all particle masses and SM parameters in any case

      SUBROUTINE MODELPAR(iunit)

      IMPLICIT NONE

C...Three Pythia functions return integers, so need declaring.
      INTEGER IMSS
      DOUBLE PRECISION RMSS
C...Supersymmetry parameters.
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
C...Pythia common blocks
      INTEGER PYCOMP,MSTU,MSTJ,KCHG
      DOUBLE PRECISION PARU,PARJ,PMAS,PARF,VCKM
C...Parameters.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)

C...Local variables      
      CHARACTER*132 buff,block_name
      CHARACTER*8 model
      INTEGER iunit,ivalue
      DOUBLE PRECISION value
      LOGICAL block_found
      
      buff=' '
      do 100 while(buff(1:21).ne.'# End param_card.dat')
        read(iunit,'(a132)',end=105,err=98) buff
        
        if(buff(1:23).eq.'# Begin param_card.dat')then
c       Write out the SLHA file to unit 24
          open(24,status='scratch')
          do while(.true.)
            read(iunit,'(a132)',end=99,err=98) buff
            if(buff(1:21).eq.'# End param_card.dat') goto 105
            write(24,'(a80)') buff
          end do
        endif
        
        call case_trap2(buff,len_trim(buff))
c     Find and store model used
        if(buff(1:14).eq.'# begin model')then
          read(iunit,'(a132)',end=99,err=98) buff
          model=buff
        endif
 100  continue
 105  continue
      REWIND(iunit)
      REWIND(24)


C...Read the SLHA file
      block_found=.false.
      do 200 while(.true.)
        read(24,'(a132)',end=205,err=98) buff
c      Look for "block" to find SM and mass parameters
         if(buff(1:1).eq.'b')then
            block_name=buff(7:)
            block_found=.true.
c         elseif(buff(1:1).eq.'d') then
c            read(buff(6:),*) ivalue,value
c            print *,'Reading decay ',ivalue,value
c            PMAS(PYCOMP(ivalue),2)=value            
         endif
         if (block_found) then
            do 10 while(.true.)
               read(iunit,'(a132)',end=205,err=98) buff
               if(buff(1:1).eq.'#') goto 10
               if(buff(1:1).ne.' ') then
                  block_found=.false.
                  backspace(iunit)
                  goto 200
               endif
               if(block_name(1:8).eq.'sminputs')then
                  read(buff,*) ivalue,value
                  print *,'Reading parameter ',block_name(1:8),
     $                 ivalue,value
                  if(ivalue.eq.1) PARU(103)=1d0/value
                  if(ivalue.eq.2) PARU(105)=value
                  if(ivalue.eq.4) PMAS(23,1)=value
                  if(ivalue.eq.6) PMAS(6,1)=value
                  if(ivalue.eq.7) PMAS(15,1)=value
               else if(block_name(1:9).eq.'mgsmparam')then
                  read(buff,*) ivalue,value
                  print *,'Reading parameter ',block_name(1:9),
     $                 ivalue,value
                  if(ivalue.eq.1) PARU(102)=value
                  if(ivalue.eq.2) PMAS(24,1)=value
               else if(block_name(1:4).eq.'mass')then
                  read(buff,*) ivalue,value
                  print *,'Reading parameter ',block_name(1:4),
     $                 ivalue,value
                  PMAS(PYCOMP(ivalue),1)=value                  
               endif
 10         continue
         endif
 200  continue
 205  continue
      REWIND(24)
      
      write(*,*) 'Reading model: ',model

c      open(24,FILE='SLHA.dat',ERR=91)
c     Pick out SM parameters
c      CALL READSMLHA(iunit)

      if(model.eq.'mssm')then
         IMSS(1) = 11
         IMSS(21)= 24 ! Logical unit number of SLHA spectrum file
c         IMSS(22)= 24 ! Logical unit number of SLHA decay file
      endif

      RETURN

 90   WRITE(*,*)'Could not open file SLHA.dat for writing'
      WRITE(*,*)'Quitting...'
      STOP
 98   WRITE(*,*)'Unexpected error reading file'
      WRITE(*,*)'Quitting...'
      STOP
 99   WRITE(*,*)'Unexpected end of file'
      WRITE(*,*)'Quitting...'
      STOP

      END

cC*********************************************************************
c      
cC...READSMLHA
cC...Reads the SMLHA file to extract SM parameters
cC...as well as all particle masses.
c
c      SUBROUTINE READSMLHA(iunit)
c
c      IMPLICIT NONE
c
cC...Pythia common blocks
c      INTEGER PYCOMP,MSTU,MSTJ,KCHG
c      DOUBLE PRECISION PARU,PARJ,PMAS,PARF,VCKM
cC...Parameters.
c      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
cC...Particle properties + some flavour parameters.
c      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
c
cC...Local variables      
c      CHARACTER*132 buff,block_name
c      CHARACTER*8 model
c      INTEGER iunit,ivalue
c      DOUBLE PRECISION value
c      LOGICAL block_found
c      
c      block_found=.false.
c
c      do 100 while(.true.)
c         read(iunit,'(a132)',end=105,err=98) buff
c         call case_trap2(buff,132)
c         if(buff(1:1).eq.'b')then
c            block_name=buff(7:)
c            block_found=.true.
cc         elseif(buff(1:1).eq.'d') then
cc            read(buff(6:),*) ivalue,value
cc            print *,'Reading decay ',ivalue,value
cc            PMAS(PYCOMP(ivalue),2)=value            
c         endif
c         if (block_found) then
c            do 10 while(.true.)
c               read(iunit,'(a132)',end=105,err=98) buff
c               if(buff(1:1).eq.'#') goto 10
c               if(buff(1:1).ne.' ') then
c                  block_found=.false.
c                  backspace(iunit)
c                  goto 100
c               endif
c               if(block_name(1:8).eq.'sminputs')then
c                  read(buff,*) ivalue,value
c                  print *,'Reading parameter ',block_name(1:8),
c     $                 ivalue,value
c                  if(ivalue.eq.1) PARU(103)=1d0/value
c                  if(ivalue.eq.2) PARU(105)=value
c                  if(ivalue.eq.4) PMAS(23,1)=value
c                  if(ivalue.eq.6) PMAS(6,1)=value
c                  if(ivalue.eq.7) PMAS(15,1)=value
c               else if(block_name(1:9).eq.'mgsmparam')then
c                  read(buff,*) ivalue,value
c                  print *,'Reading parameter ',block_name(1:9),
c     $                 ivalue,value
c                  if(ivalue.eq.1) PARU(102)=value
c                  if(ivalue.eq.2) PMAS(24,1)=value
c               else if(block_name(1:4).eq.'mass')then
c                  read(buff,*) ivalue,value
c                  print *,'Reading parameter ',block_name(1:4),
c     $                 ivalue,value
c                  PMAS(PYCOMP(ivalue),1)=value                  
c               endif
c 10         continue
c         endif
c 100  continue
c 105  continue
c
c      REWIND(iunit)
c
c      RETURN
c 98   WRITE(*,*)'Unexpected error reading file'
c      WRITE(*,*)'Quitting...'
c      STOP
c      END

C*********************************************************************

      subroutine BRSUPP

      IMPLICIT NONE

C...Three Pythia functions return integers, so need declaring.
      INTEGER PYCOMP,MWID
      DOUBLE PRECISION WIDS

C...Resonance width and secondary decay treatment.
      COMMON/PYINT4/MWID(500),WIDS(500,5)

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)
C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE

      INTEGER I,J,IBEG
      REAL SUPPCS

C...Lines to read in assumed never longer than 200 characters. 
      INTEGER MAXLEN
      PARAMETER (MAXLEN=200)
      CHARACTER*(MAXLEN) STRING

C...Format for reading lines.
      CHARACTER*6 STRFMT
      STRFMT='(A000)'
      WRITE(STRFMT(3:5),'(I3)') MAXLEN

C...Loop until finds line beginning with "<event>" or "<event ". 
  100 READ(LNHIN,STRFMT,END=130,ERR=130) STRING
      IBEG=0
  110 IBEG=IBEG+1
C...Allow indentation.
      IF(STRING(IBEG:IBEG).EQ.' '.AND.IBEG.LT.MAXLEN-6) GOTO 110 
      IF(STRING(IBEG:IBEG+6).NE.'<event>'.AND.
     &STRING(IBEG:IBEG+6).NE.'<event ') GOTO 100

C...Read first line of event info.
      READ(LNHIN,*,END=130,ERR=130) NUP,IDPRUP,XWGTUP,SCALUP,
     &AQEDUP,AQCDUP

C...Read NUP subsequent lines with information on each particle.
      DO 120 I=1,NUP
        READ(LNHIN,*,END=130,ERR=130) IDUP(I),ISTUP(I),
     &  MOTHUP(1,I),MOTHUP(2,I),ICOLUP(1,I),ICOLUP(2,I),
     &  (PUP(J,I),J=1,5),VTIMUP(I),SPINUP(I)
  120 CONTINUE


      SUPPCS=1.
      do I=3,NUP
        if (ISTUP(I).EQ.1.AND.(IABS(IDUP(I)).GT.23.OR.
     $     (IABS(IDUP(I)).GE.6.AND.IABS(IDUP(I)).LE.8)))
     $     THEN
          WRITE(LNHOUT,*) 'Resonance ',IDUP(I), ' has BRTOT ',
     $       wids(PYCOMP(IDUP(I)),2)
          if(wids(PYCOMP(IDUP(I)),2).lt.0.95) then
            write(*,*) 'Decreasing cross section!'
            SUPPCS=SUPPCS*wids(PYCOMP(IDUP(I)),2)
          endif
        endif
      enddo
      if(SUPPCS.gt.0)then
         write(*,*)'Multiplying cross section by ',SUPPCS
         XSECUP(1)=XSECUP(1)*SUPPCS
      else
         write(*,*) 'Warning! Got cross section suppression 0'
         write(*,*) 'No cross section reduction done'
      endif
      RETURN

C...Error exit, typically when no more events.
  130 WRITE(*,*) ' Failed to read LHEF event information.'
      STOP
      END
      
      subroutine case_trap2(name,n)
c**********************************************************
c   change the string to lowercase if the input is not
c**********************************************************
      implicit none
c   
c   ARGUMENT
c   
      character(*) name
      integer n
c   
c   LOCAL
c   
      integer i,k

      do i=1,n
        k=ichar(name(i:i))
        if(k.ge.65.and.k.le.90) then !upper case A-Z
          k=ichar(name(i:i))+32   
          name(i:i)=char(k)        
        endif
      enddo

      return
      end

c----------------------------------------------------------------------
c   READ_PARAMS
c   Read the parameters from the run_card in the MadEvent event file
c----------------------------------------------------------------------

      subroutine read_params(iunit,npara,param,value)
      implicit none

c   
c   parameters
c   
      integer maxpara
      parameter (maxpara=100)
c   
c   arguments
c   
      integer iunit
      character*20 param(maxpara),value(maxpara)
      integer npara

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)
C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)

c   
c   local
c   
      logical fopened
      character*20 ctemp
      integer k,i,l1,l2,j,jj
      character*132 buff
      integer NPARTS,KNTEV,NJETS,MAXNJ,NREAD
      parameter(MAXNJ=6)
      double precision WTEVNT,XSTOT(MAXNJ),DUMMY,WTMAX
c   
c----------
c   start
c----------

      NREAD=0

c   
c   read the input-card.dat
c   
      npara=1
      param(1)=' '
      value(1)=' '
      WTMAX=0D0
c   
c   read in values
c   
      buff=' '
      do while(index(buff,'Begin run_card.dat').ne.0)
        read(iunit,'(a132)',end=99,err=99) buff
      enddo
      do 10 while(index(buff,'End run_card.dat').eq.0)
        read(iunit,'(a132)',end=99,err=99) buff
        if(buff.eq.' ') then
          goto 10
        endif
        if(index(buff,'=').ne.0) then
          l1=index(buff,'=')
          l2=index(buff,'!')
          if(l2.eq.0) l2=l1+20  !maybe there is no comment...
c       
          value(npara)=buff(1:l1-1)
          ctemp=value(npara)
          call case_trap2(ctemp,20)
          value(npara)=ctemp
c       
          param(npara)=" "//buff(l1+1:l2-1)
          ctemp=param(npara)
          call case_trap2(ctemp,20)
          param(npara)=ctemp
c       
          npara=npara+1
        endif
 10   continue

      REWIND(iunit)

      return

 99   WRITE(*,*)'Unexpected error reading file'
      WRITE(*,*)'Quitting...'
      STOP
      end

C*********************************************************************      
C...set_matching
C...Sets parameters for the matching, i.e. cuts and jet multiplicities
C*********************************************************************      

      SUBROUTINE set_matching(iunit,npara,param,value)
      implicit none
c   
c   arguments
c   
      integer iunit,npara
      character*20 param(*),value(*)

C...User process initialization commonblock.
      INTEGER MAXPUP
      PARAMETER (MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON/HEPRUP/IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &   IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),XMAXUP(MAXPUP),
     &   LPRUP(MAXPUP)

C...User process event common block.
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,IDUP(MAXNUP),
     &   ISTUP(MAXNUP),MOTHUP(2,MAXNUP),ICOLUP(2,MAXNUP),PUP(5,MAXNUP),
     &   VTIMUP(MAXNUP),SPINUP(MAXNUP)

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,ICKKW,ISCALE

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,qfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,qfact,
     $   maxjets,minjets,iexcfile,ktsche
      DATA ktsche/0/
      DATA qcut/0d0/

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      COMMON/MEMAEV/NLJETS,IEXC,Ifile

C     CALSIM VARIABLES
      INTEGER NCY,NCPHI
      DOUBLE PRECISION PI,DELY,DELPHI,ET,CTHCAL,STHCAL,CPHCAL,SPHCAL,
     $   YCMIN,YCMAX
      PARAMETER (NCY=50)
      PARAMETER (NCPHI=60,PI=3.141593D0)
      COMMON/CALOR/DELY,DELPHI,ET(NCY,NCPHI),
     $   CTHCAL(NCY),STHCAL(NCY),CPHCAL(NCPHI),SPHCAL(NCPHI),
     $   YCMIN,YCMAX

C...Local variables
      INTEGER I,MAXNJ,NREAD,MINJ,MAXJ
      parameter(MAXNJ=6)
      DOUBLE PRECISION XSTOT(MAXNJ),XSECTOT
      DOUBLE PRECISION ptjmin,etajmax,drjmin,ptbmin,etabmax,xqcut

C...Need lower scale for final state radiation in e+e-
      IF(IABS(IDBMUP(1)).EQ.11.AND.IABS(IDBMUP(2)).EQ.11) then
        CALL PYGIVE('PARP(71)=1')
c      ELSE
c        CALL PYGIVE('PARP(71)=4')        
      ENDIF
C     
C...Check jet multiplicities and set processes
C
      MINJETS=0
      MAXJETS=0

      DO I=1,MAXNJ
        XSTOT(I)=0D0
      ENDDO
      MINJ=MAXNJ
      MAXJ=0
      NREAD=0
      
      DO WHILE(.true.)
        CALL UPEVNT()
        IF(NUP.eq.0) goto 20
        if(NLJETS.GT.MAXJ) MAXJ=NLJETS
        if(NLJETS.LT.MINJ) MINJ=NLJETS
c        XSTOT(NLJETS+1)=XSTOT(NLJETS+1)+XWGTUP
        XSTOT(NLJETS+1)=XSTOT(NLJETS+1)+1
        NREAD=NREAD+1
      ENDDO

 20   continue

      REWIND(iunit)

      MINJETS=MINJ
      MAXJETS=MAXJ
      write(LNHOUT,*) 'Minimum number of jets: ',MINJETS
      write(LNHOUT,*) 'Maximum number of jets: ',MAXJETS
      NPRUP=1+MAXJETS-MINJETS
c      XSECTOT=0d0
      XSECTOT=XSECUP(1)
      DO I=MINJETS,MAXJETS
c        XSECUP(1+I-MINJETS) = XSTOT(I+1)
        XSECUP(1+I-MINJETS) = XSECTOT*XSTOT(I+1)/NREAD
        XMAXUP(1+I-MINJETS) = XMAXUP(1)
        LPRUP(1+I-MINJETS)  = 661+I-MINJETS
        XSECTOT=XSECTOT+XSTOT(I+1)
      ENDDO

      WRITE(LNHOUT,*) ' Number of Events Read:: ',NREAD
      WRITE(LNHOUT,*) ' Total cross section (pb):: ',XSECTOT
      WRITE(LNHOUT,*) ' Process   Cross Section (pb):: '
      DO I=1,NPRUP
        WRITE(LNHOUT,'(I5,E23.5)') I,XSECUP(I)
      ENDDO
      CALL FLUSH()

      if(ickkw.eq.1) then

C...Run PYPTFS instead of PYSHOW
c        CALL PYGIVE("MSTJ(41)=12")

c***********************************************************************
c   Read in jet cuts
c***********************************************************************

        call get_real   (npara,param,value," ptj " ,ptjmin,7d3)
        call get_real   (npara,param,value," etaj " ,etajmax,7d3)
        call get_real   (npara,param,value," ptb " ,ptbmin,7d3)
        call get_real   (npara,param,value," etab " ,etabmax,7d3)
        call get_real   (npara,param,value," drjj " ,drjmin,7d3)
        call get_real   (npara,param,value," xqcut " ,xqcut,0d0)
        call get_real   (npara,param,value," xqfact " ,qfact,1.5d0)

        if(qcut.lt.xqcut) qcut=xqcut*qfact
        
        etajmax=min(etajmax,etabmax)
        ptjmin=max(ptjmin,ptbmin)

c      IF(ICKKW.EQ.1) THEN
c        WRITE(*,*) ' '
c        WRITE(*,*) 'INPUT 0 FOR INCLUSIVE JET SAMPLE, 1 FOR EXCLUSIVE'
c        WRITE(*,*) '(SELECT 0 FOR HIGHEST PARTON MULTIPLICITY SAMPLE)' 
c        WRITE(*,*) '(SELECT 1 OTHERWISE)'
c        READ(*,*) IEXCFILE
c      ENDIF
        
C     INPUT PARAMETERS FOR CONE ALGORITHM

        ETCJET=PTJMIN
        RCLMAX=DRJMIN
        ETACLMAX=ETAJMAX+DRJMIN
        IF(qcut.le.0)THEN
          WRITE(*,*) 'JET CONE PARAMETERS FOR MATCHING:'
          WRITE(*,*) 'ET>',ETCJET,' R=',RCLMAX
          WRITE(*,*) 'DR(PARTON-JET)<',1.5*RCLMAX
          WRITE(*,*) 'ETA(JET)<',ETACLMAX
        ELSE
          WRITE(*,*) 'KT JET PARAMETERS FOR MATCHING:'
          WRITE(*,*) 'QCUT=',qcut
          WRITE(*,*) 'PT(JET)>',PTJMIN
          WRITE(*,*) 'ETA(JET)<',ETACLMAX
          WRITE(*,*) 'Note that in ME generation, qcut = ',xqcut
        ENDIF
      endif
      return
      end

      subroutine get_real(npara,param,value,name,var,def_value)
c----------------------------------------------------------------------------------
c   finds the parameter named "name" in param and associate to "value" in value 
c----------------------------------------------------------------------------------
      implicit none

c   
c   parameters
c   
      integer maxpara
      parameter (maxpara=100)
c   
c   arguments
c   
      integer npara
      character*20 param(maxpara),value(maxpara)
      character*(*)  name
      real*8 var,def_value
c   
c   local
c   
      logical found
      integer i
c   
c   start
c   
      i=1
      found=.false.
      do while(.not.found.and.i.le.npara)
        found = (index(param(i),name).ne.0)
        if (found) read(value(i),*) var
c     if (found) write (*,*) name,var
        i=i+1
      enddo
      if (.not.found) then
        write (*,*) "Warning: parameter ",name," not found"
        write (*,*) "         setting it to default value ",def_value
        var=def_value
      else
        write(*,*),'Found parameter ',name,var
      endif
      return

      end
c   

      subroutine get_integer(npara,param,value,name,var,def_value)
c----------------------------------------------------------------------------------
c   finds the parameter named "name" in param and associate to "value" in value 
c----------------------------------------------------------------------------------
      implicit none
c   
c   parameters
c   
      integer maxpara
      parameter (maxpara=100)
c   
c   arguments
c   
      integer npara
      character*20 param(maxpara),value(maxpara)
      character*(*)  name
      integer var,def_value
c   
c   local
c   
      logical found
      integer i
c   
c   start
c   
      i=1
      found=.false.
      do while(.not.found.and.i.le.npara)
        found = (index(param(i),name).ne.0)
        if (found) read(value(i),*) var
c     if (found) write (*,*) name,var
        i=i+1
      enddo
      if (.not.found) then
        write (*,*) "Warning: parameter ",name," not found"
        write (*,*) "         setting it to default value ",def_value
        var=def_value
      else
        write(*,*)'Found parameter ',name,var
      endif
      return

      end

C-----------------------------------------------------------------------
      SUBROUTINE ALPSOR(A,N,K,IOPT)
C-----------------------------------------------------------------------
C     Sort A(N) into ascending order
C     IOPT = 1 : return sorted A and index array K
C     IOPT = 2 : return index array K only
C-----------------------------------------------------------------------
      DOUBLE PRECISION A(N),B(5000)
      INTEGER N,I,J,IOPT,K(N),IL(5000),IR(5000)
      IF (N.GT.5000) then
        write(*,*) 'Too many entries to sort in alpsrt, stop'
        stop
      endif
      if(n.le.0) return
      IL(1)=0
      IR(1)=0
      DO 10 I=2,N
      IL(I)=0
      IR(I)=0
      J=1
   2  IF(A(I).GT.A(J)) GOTO 5
   3  IF(IL(J).EQ.0) GOTO 4
      J=IL(J)
      GOTO 2
   4  IR(I)=-J
      IL(J)=I
      GOTO 10
   5  IF(IR(J).LE.0) GOTO 6
      J=IR(J)
      GOTO 2
   6  IR(I)=IR(J)
      IR(J)=I
  10  CONTINUE
      I=1
      J=1
      GOTO 8
  20  J=IL(J)
   8  IF(IL(J).GT.0) GOTO 20
   9  K(I)=J
      B(I)=A(J)
      I=I+1
      IF(IR(J)) 12,30,13
  13  J=IR(J)
      GOTO 8
  12  J=-IR(J)
      GOTO 9
  30  IF(IOPT.EQ.2) RETURN
      DO 31 I=1,N
  31  A(I)=B(I)
 999  END
