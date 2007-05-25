C******************************************************
C*          MadEvent - Pythia interface.              *
C*           Version 4.2-CMSSW 08 May 2007            *
C* dkcira                                             *
C*                                                    *
C*  - add changes proposed by S. Bolognesi to allow   *
C*    reading of Phantom files. Two flags are added   *
C*    that disallow setting the model and reading the *
C*    input cards file SLHA.dat                       *
C*  - Hack for reading input file in present setup    *
C*    of CMSSW                                        *
C*  - add -w flag to buildfile so that no warnings    *
C*    about unused variables are displayed            *
C*                                                    *
C*           Version 4.2, 4 March 2007                *
C*                                                    *
C*  - Improvement of matching routines                *
C*                                                    *
C*  Version 4.1                                       *
C*                                                    *
C*  - Possibility to use several files                *
C*                                                    *
C*  Version 4.0                                       *
C*                                                    *
C*  - Routines for matching of ME and PS              *
C*                                                    *
C*  Version 3.8                                       *
C*                                                    *
C*  - Give the event number in the event file in the  *
C*    new variable IEVNT in UPPRIV                    *
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
C* - IEVNT gives the number of the event in the event *
C*   file                                             *
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
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      DATA LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE/77,6,1,0,0,1/
      SAVE/UPPRIV/

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
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
      CHARACTER EVFILE*1000

C...Format for reading lines.
      CHARACTER*6 STRFMT
      STRFMT='(A000)'
      WRITE(STRFMT(3:5),'(I3)') MAXLEN

c DKC
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

c DKC
c C...Set PDFLIB or LHAPDF pdf number for Pythia
c 
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
     $     IDBMUP(1).EQ.-IDBMUP(2).AND.ktsche.EQ.0)THEN
         ktsche=1
      ELSE IF(ktsche.EQ.0) THEN
         ktsche=4313
      ENDIF

C...Enhance primordial kt
c      CALL PYGIVE('PARP(91)=2.5')
c      CALL PYGIVE('PARP(93)=15')

      IF(ickkw.gt.0) CALL set_matching(LNHIN,npara,param,value)

C...Reset event numbering
      IEVNT=0

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
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      DOUBLE PRECISION PTCLUS
      COMMON/MEMAEV/PTCLUS(20),NLJETS,IEXC,Ifile

C...Local variables
      INTEGER I,J,IBEG,NEX,KP(MAXNUP),MOTH
      DOUBLE PRECISION PSUM,ESUM,PM1,PM2,A1,A2,A3,A4,A5
      DOUBLE PRECISION SCALLOW(MAXNUP),PNONJ(4),PMNONJ!,PT2JETS
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

C...Increment event number
      IEVNT=IEVNT+1

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
        READ(LNHIN,*,END=130,ERR=101) CDUM,(PTCLUS(I),I=1,NEX)
 101    CONTINUE
      ENDIF

      IF(ickkw.gt.0) THEN
c
c   Set up number of jets
c
         NLJETS=0

         do i=3,NUP
            if(ISTUP(i).ne.1) cycle
            if(iabs(IDUP(i)).ge.6.and.IDUP(i).ne.21) cycle
            if(MOTHUP(1,i).gt.2) cycle
            NLJETS=NLJETS+1
         enddo
      
         IDPRUP=LPRUP(NLJETS-MINJETS+1)


         IF(ickkw.eq.1) THEN
c   ... and decide whether exclusive or inclusive
            if(IEXCFILE.eq.0.and.NLJETS.eq.MAXJETS)then
               IEXC=0
            else
               IEXC=1
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
C...GETJET commonblocks
      INTEGER MNCY,MNCPHI,NCY,NCPHI,NJMAX,JETNO,NCJET
      DOUBLE PRECISION YCMIN,YCMAX,DELY,DELPHI,ET,STHCAL,CTHCAL,CPHCAL,
     &  SPHCAL,PCJET,ETJET
      PARAMETER (MNCY=200)
      PARAMETER (MNCPHI=200)
      COMMON/CALORM/DELY,DELPHI,ET(MNCY,MNCPHI),
     $CTHCAL(MNCY),STHCAL(MNCY),CPHCAL(MNCPHI),SPHCAL(MNCPHI),
     $YCMIN,YCMAX,NCY,NCPHI
      DATA NCY,NCPHI/50,60/
      PARAMETER (NJMAX=500)
      COMMON/GETCOMM/PCJET(4,NJMAX),ETJET(NJMAX),JETNO(MNCY,MNCPHI),
     $NCJET
      DOUBLE PRECISION PI
      PARAMETER (PI=3.141593D0)
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
      INTEGER NITER,NFILES
      DOUBLE PRECISION ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX,ASFACT
      COMMON/JETPAR/ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX,ASFACT,
     $     NITER,NFILES

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE

C...Inputs for the matching algorithm
C   clfact determines how jet-to parton matching is done
C   kt-jets: default=1
C    clfact >= 0: Max mult. if within clfact*max(qcut,Q(partNmax)) from jet, others within clfact*qcut
C    clfact < 0: Max mult. if within |clfact|*Q(jetNmax) from jet, other within |clfact|*qcut
C   cone-jets: default=1.5
C    Matching if within clfact*RCLMAX 

      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      DOUBLE PRECISION PTCLUS
      COMMON/MEMAEV/PTCLUS(20),NLJETS,IEXC,Ifile

      INTEGER nvarev,nvar2
      PARAMETER (nvarev=57,nvar2=6)

      REAL*4 varev(nvarev)
      COMMON/HISTDAT/varev


C   local variables
      integer i,j,ihep,nmatch,jrmin,KPT(MAXNUP)
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

      IPVETO=0
      IF(ICKKW.NE.1) RETURN

C   Set up vetoed mothers
c      DO I=1,MAXNUP
c        INORAD(I)=0
c      ENDDO
c      DO IHEP=1,NUP-2      
c        if(ISTHEP(ihep).gt.1.and.iabs(IDHEP(ihep)).gt.8) then
c        if(iabs(IDHEP(ihep)).gt.5.and.IDHEP(ihep).ne.21) then
c          INORAD(ihep)=1
c        endif
c      ENDDO

C     Set status for non-clustering partons to 2
      DO ihep=1,NHEP
c         ISTORG(ihep)=ISTHEP(ihep)
         IF(ISTHEP(ihep).EQ.1.AND.iabs(IDHEP(ihep)).GE.6.AND.
     $        IDHEP(ihep).NE.21) ISTHEP(ihep)=2
         IF(ISTHEP(ihep).EQ.1.AND.(iabs(IDHEP(ihep)).lt.6.or.
     $        IDHEP(ihep).eq.21).AND.JMOHEP(1,ihep).GT.0) then
            IMO=JMOHEP(1,ihep)
            DO WHILE(IMO.GT.0)
               IF(iabs(IDHEP(IMO)).GE.6.AND.IDHEP(IMO).NE.21) GOTO 5
               IMO=JMOHEP(1,IMO)
            ENDDO
            cycle
 5          ISTHEP(ihep)=2
         ENDIF
      ENDDO

C     Prepare histogram filling
        DO I=1,4
          var2(1+I)=-1
          varev(46+I)=-1
          varev(50+I)=-1
        ENDDO

C CHECK FOR EVENT ERROR OR ZERO WGT
      I=0
C
c     reconstruct parton-level event

      if(idbg.eq.1) then
        write(LNHOUT,*) ' '
        write(LNHOUT,*) 'new event '
c        CALL PYLIST(1)
        CALL PYLIST(7)
        CALL PYLIST(5)
        write(LNHOUT,*) 'PARTONS'
      endif
      i=0
      do ihep=3,nup
        if(ISTUP(ihep).ne.1.or.
     $     (iabs(IDUP(ihep)).ge.6.and.IDUP(ihep).ne.21)) cycle
        if(MOTHUP(1,ihep).gt.2) cycle
        i=i+1
        do j=1,4
          p(j,i)=pup(j,ihep)
        enddo
        pt(i)=sqrt(p(1,i)**2+p(2,i)**2)
        if(i.LE.4) varev(50+i)=pt(i)
        eta(i)=-log(tan(0.5d0*atan2(pt(i)+tiny,p(3,i))))
        phi(i)=atan2(p(2,i),p(1,i))
        if(idbg.eq.1) then
          write(LNHOUT,*) pt(i),eta(i),phi(i)
        endif
      enddo
      if(i.ne.NLJETS)then
        print *,'Error in UPVETO: Wrong number of jets found ',i,NLJETS
        CALL PYLIST(7)
        CALL PYLIST(2)
        stop
      endif

C Bubble-sort PTs in descending order
      DO I=1,3
         DO J=4,I+1,-1
            IF(varev(50+J).GT.varev(50+I))THEN
               PTJET=varev(50+J)
               varev(50+J)=varev(50+I)
               varev(50+I)=PTJET
            ENDIF
         ENDDO
      ENDDO

      if(idbg.eq.1) then
        do i=1,nhep
          write(LNHOUT,111) i,isthep(i),idhep(i),jmohep(1,i),jmohep(2,i)
     $         ,phep(1,i),phep(2,i),phep(3,i)
        enddo
 111  format(5(i4,1x),3(f12.5,1x))
      endif

      IF(qcut.le.0d0)then

      IF(clfact.EQ.0d0) clfact=1.5d0

c      CALL PYLIST(7)
c      CALL PYLIST(2)
c      CALL PYLIST(5)
c     Start from the partonic system
      IF(NLJETS.GT.0) CALL ALPSOR(pt,nljets,KP,2)  
c     reconstruct showered jets
c     
      YCMAX=ETACLMAX+RCLMAX
      YCMIN=-YCMAX
      CALL CALINIM
      CALL CALDELM(1,1)
      CALL GETJETM(RCLMAX,ETCJET,ETACLMAX)
c     analyse only events with at least nljets-reconstructed jets
      IF(NCJET.GT.0) CALL ALPSOR(ETJET,NCJET,K,2)              
      if(idbg.eq.1) then
        write(LNHOUT,*) 'JETS'
        do i=1,ncjet
          j=k(ncjet+1-i)
          ETAJET=PSERAP(PCJET(1,j))
          PHIJET=ATAN2(PCJET(2,j),PCJET(1,j))
          write(LNHOUT,*) etjet(j),etajet,phijet
        enddo
      endif
      IF(NCJET.LT.NLJETS) THEN
        if(idbg.eq.1)
     $     WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' < ',NLJETS
        GOTO 999
      endif
c     associate partons and jets, using min(delr) as criterion
      NMATCH=0
      DO I=1,NCJET
        KPJ(I)=0
      ENDDO
      DO I=1,NLJETS
        DELRMIN=1D5
        DO 110 J=1,NCJET
          IF(KPJ(J).NE.0) GO TO 110
          ETAJET=PSERAP(PCJET(1,J))
          PHIJET=ATAN2(PCJET(2,J),PCJET(1,J))
          DPHI=ABS(PHI(KP(NLJETS-I+1))-PHIJET)
          IF(DPHI.GT.PI) DPHI=2.*PI-DPHI
          DELR=SQRT((ETA(KP(NLJETS-I+1))-ETAJET)**2+(DPHI)**2)
          IF(DELR.LT.DELRMIN) THEN
            DELRMIN=DELR
            JRMIN=J
          ENDIF
 110    CONTINUE
        IF(DELRMIN.LT.clfact*RCLMAX) THEN
          NMATCH=NMATCH+1
          KPJ(JRMIN)=I
        ENDIF
C     WRITE(*,*) 'PARTON-JET',I,' best match:',k(ncjet+1-jrmin)
c     $           ,delrmin
      ENDDO
      IF(NMATCH.LT.NLJETS)  THEN
        if(idbg.eq.1)
     $     WRITE(LNHOUT,*) 'Failed due to NMATCH ',NMATCH,' < ',NLJETS
        GOTO 999
      endif
C REJECT EVENTS WITH LARGER JET MULTIPLICITY FROM EXCLUSIVE SAMPLE
      IF(NCJET.GT.NLJETS.AND.IEXC.EQ.1)  THEN
        if(idbg.eq.1)
     $     WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' > ',NLJETS
        GOTO 999
      endif
C     VETO EVENTS WHERE MATCHED JETS ARE SOFTER THAN NON-MATCHED ONES
      IF(IEXC.NE.1) THEN
        J=NCJET
        DO I=1,NLJETS
          IF(KPJ(K(J)).EQ.0) GOTO 999
          J=J-1
        ENDDO
      ENDIF

      else                      ! qcut.gt.0

        IF(clfact.EQ.0d0) clfact=1d0

C---FIND FINAL STATE COLOURED PARTICLES
        NN=0
        DO IHEP=1,NHEP
          IF (ISTHEP(IHEP).EQ.1
     &       .AND.(ABS(IDHEP(IHEP)).LT.6.OR.IDHEP(IHEP).EQ.21)) THEN
            PTJET=sqrt(PHEP(1,IHEP)**2+PHEP(2,IHEP)**2)
            ETAJET=ABS(LOG(MIN((SQRT(PTJET**2+PHEP(3,IHEP)**2)+
     $       ABS(PHEP(3,IHEP)))/PTJET,1d5)))
            IF(ETAJET.GT.etaclmax) cycle
            NN=NN+1
            IF (NN.GT.NMAX) then
              CALL PYLIST(2)
              PRINT *, 'Too many particles: ', NN
              NN=NN-1
              GOTO 10
            endif
            DO I=1,4
              PP(I,NN)=PHEP(I,IHEP)
            ENDDO
          ELSE if(idbg.eq.1)THEN
            PRINT *,'Skipping particle ',IHEP,ISTHEP(IHEP),IDHEP(IHEP)
          ENDIF
        ENDDO

C...Cluster event to find values of Y including jet matching but not veto of too many jets
C...Only used to fill the beforeveto Root tree
 10     ECUT=1
        IF (NN.GT.1) then
          CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)
          if(idbg.eq.1)
     $       WRITE(LNHOUT,*) 'Clustering values:',
     $       (SQRT(Y(i)),i=1,MIN(NN,3))

C       Print out values in the case where all jets are matched at the
C       value of the NLJETS:th clustering
        var2(1)=NLJETS
        var2(6)= Ifile

        if(NLJETS.GT.MINJETS)then
          YCUT=Y(NLJETS)
          CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
     $       NCJET,NSUB,*999)        

C     Cluster jets with first hard parton
          DO I=1,NLJETS
            DO J=1,4
              PPM(J,I)=PJET(J,I)
            ENDDO
          ENDDO
          
          NJETM=NLJETS
          DO IHARD=1,NLJETS
            NNM=NJETM+1
            DO J=1,4
              PPM(J,NNM)=p(J,IHARD)
            ENDDO
            CALL KTCLUS(KTSCHE,PPM,NNM,ECUT,YM,*999)
            IF(YM(NNM).GT.YCUT) THEN
C       Parton not clustered
              GOTO 90
            ENDIF
            
C       Find jet clustered with parton

            IP1=HIST(NNM)/NMAXKT
            IP2=MOD(HIST(NNM),NMAXKT)
            IF(IP2.NE.NNM.OR.IP1.LE.0)THEN
              GOTO 90
            ENDIF
            DO I=IP1,NJETM-1
              DO J=1,4
                PPM(J,I)=PPM(J,I+1)
              ENDDO
            ENDDO
            NJETM=NJETM-1
          ENDDO                 ! IHARD=1,NLJETS
        endif                   ! NLJETS.GT.MINJETS

        DO I=1,MIN(NN,4)
          var2(1+I)=SQRT(Y(I))
        ENDDO
C DKC        WRITE(15,4001) (var2(I),I=1,nvar2)

 90     CONTINUE

C   Now perform jet clustering at the value chosen in qcut

        CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)

        YCUT=qcut**2
        NCJET=0
          
C     Reconstruct jet momenta
          CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
     $       NCJET,NSUB,*999)        

        ELSE IF (NN.EQ.1) THEN

          Y(1)=PP(1,1)**2+PP(2,1)**2
          IF(Y(1).GT.YCUT)THEN
            NCJET=1
            DO I=1,4
              PJET(I,1)=PP(I,1)
            ENDDO
          ENDIF
        endif

        if(idbg.eq.1) then
          write(LNHOUT,*) 'JETS'
          do i=1,ncjet
            PTJET =SQRT(PJET(1,i)**2+PJET(2,i)**2)
            ETAJET=PSERAP(PJET(1,i))
            PHIJET=ATAN2(PJET(2,i),PJET(1,i))
            write(LNHOUT,*) ptjet,etajet,phijet
          enddo
        endif

        IF(NCJET.LT.NLJETS) THEN
          if(idbg.eq.1)
     $       WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' < ',NLJETS
          GOTO 999
        endif

C...Right number of jets - but the right jets?        
C     For max. multiplicity case, count jets only to the NHARD:th jet
        IF(IEXC.EQ.0)THEN
           IF(NLJETS.GT.0)THEN
              YCUT=Y(NLJETS)
              CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,PJET,JET,
     $             NCJET,NSUB,*999)
              IF(clfact.GE.0d0) THEN
                 CALL ALPSOR(PTCLUS,nljets,KPT,2)
                 YCUT=MAX(qcut,PTCLUS(KPT(1)))**2
              ENDIF
           ENDIF
        ELSE IF(NCJET.GT.NLJETS) THEN
           if(idbg.eq.1)
     $       WRITE(LNHOUT,*) 'Failed due to NCJET ',NCJET,' > ',NLJETS
           GOTO 999
        ENDIF

C     Cluster jets with hard partons, one at a time
        DO I=1,NLJETS
          DO J=1,4
            PPM(J,I)=PJET(J,I)
          ENDDO
        ENDDO

        NJETM=NLJETS
        IF(clfact.NE.0) YCUT=clfact**2*YCUT
c        YCUT=qcut**2
c        YCUT=(1.5*qcut)**2

        DO 120 IHARD=1,NLJETS
          NN=NJETM+1
          DO J=1,4
            PPM(J,NN)=p(J,IHARD)
          ENDDO
          CALL KTCLUS(KTSCHE,PPM,NN,ECUT,Y,*999)

          IF(Y(NN).GT.YCUT) THEN
C       Parton not clustered
          if(idbg.eq.1)
     $       WRITE(LNHOUT,*) 'Failed due to parton ',IHARD,
     $         ' not clustered.'
            GOTO 999
          ENDIF
          
C       Find jet clustered with parton

          IP1=HIST(NN)/NMAXKT
          IP2=MOD(HIST(NN),NMAXKT)
          IF(IP2.NE.NN.OR.IP1.LE.0)THEN
          if(idbg.eq.1)
     $       WRITE(LNHOUT,*) 'Failed due to parton ',IHARD,
     $         ' not clustered.'
            GOTO 999
          ENDIF
C     Remove jet clustered with parton
          DO I=IP1,NJETM-1
            DO J=1,4
              PPM(J,I)=PPM(J,I+1)
            ENDDO
          ENDDO
          NJETM=NJETM-1
 120    CONTINUE

      endif

C...Cluster particles with |eta| < etamax for histograms
      NN=0
      DO IHEP=1,NHEP
         IF (ISTHEP(IHEP).EQ.1
     &        .AND.(ABS(IDHEP(IHEP)).LT.6.OR.IDHEP(IHEP).EQ.21)) THEN
            PTJET=sqrt(PHEP(1,IHEP)**2+PHEP(2,IHEP)**2)
            ETAJET=ABS(LOG(MIN((SQRT(PTJET**2+PHEP(3,IHEP)**2)+
     $           ABS(PHEP(3,IHEP)))/PTJET,1d5)))
c            IF(ETAJET.GT.ETAMAX) cycle
cDKC - recommended from Johan for doing check plots for the matching
            IF(ETAJET.GT.etaclmax) cycle
            NN=NN+1
            IF (NN.GT.NMAX) then
               CALL PYLIST(2)
               PRINT *, 'Too many particles: ', NN
               NN=NN-1
               GOTO 20
            ENDIF
            DO I=1,4
               PP(I,NN)=PHEP(I,IHEP)
            ENDDO
         ELSE if(idbg.eq.1)THEN
            PRINT *,'Skipping particle ',IHEP,ISTHEP(IHEP),IDHEP(IHEP)
         ENDIF
      ENDDO
      
 20   ECUT=1
      IF (NN.GT.1) THEN
         CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)
      ELSE IF(NN.EQ.1) THEN
         Y(1)=SQRT(PP(1,NN)**2+PP(2,NN)**2)
      ENDIF

      DO I=1,MIN(NN,4)
         varev(46+I)=SQRT(Y(I))
      ENDDO


      RETURN
 4001 FORMAT(50E15.6)
c HERWIG/PYTHIA TERMINATION:
 999  IPVETO=1
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
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE

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

C...add flags for the case when no model is defined in the head of LH and no cards should be read. DKC
      INTEGER model_def,cards_def
      model_def = 0
      cards_def = 0

      buff=' '
      do 100 while(buff(1:21).ne.'# End param_card.dat')
        read(iunit,'(a132)',end=105,err=98) buff
        
        if(buff(1:23).eq.'# Begin param_card.dat')then
c       Write out the SLHA file to unit 24
          open(24,status='scratch')
          do while(.true.)
            read(iunit,'(a132)',end=99,err=98) buff
            cards_def=1 ! DKC
            if(buff(1:21).eq.'# End param_card.dat') goto 105
            write(24,'(a80)') buff
          end do
        endif
        
        call case_trap2(buff,len_trim(buff))
c     Find and store model used
        if(buff(1:14).eq.'# begin model')then
          read(iunit,'(a132)',end=99,err=98) buff
          model_def=1 ! DKC
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
      
      if(model_def.EQ.1) then ! DKC
      write(*,*) 'Reading model: ',model
      endif

c      open(24,FILE='SLHA.dat',ERR=91)
c     Pick out SM parameters
c      CALL READSMLHA(iunit)

      if(model.ne.'sm')then
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
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE

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
      INTEGER LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL,IEVNT,ICKKW,ISCALE

C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
     $   maxjets,minjets,iexcfile,ktsche
      DATA ktsche/0/
      DATA qcut,clfact/0d0,0d0/

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      DOUBLE PRECISION PTCLUS
      COMMON/MEMAEV/PTCLUS(20),NLJETS,IEXC,Ifile

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
c        XSECTOT=XSECTOT+XSTOT(I+1)
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

        if(qcut.lt.xqcut) qcut=xqcut*1.2
        if(xqcut.le.0)then
           write(*,*) 'Warning! ME generation QCUT = 0. QCUT set to 0!'
           qcut=0
        endif

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

        ETCJET=MAX(PTJMIN+5,1.2*PTJMIN)
        RCLMAX=DRJMIN
        ETACLMAX=ETAJMAX
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

C-----------------------------------------------------------------------
C----Calorimeter simulation obtained from Frank Paige 23 March 1988-----
C
C          USE
C
C     CALL CALINIM
C     CALL CALSIMM
C
C          THEN TO FIND JETS WITH A SIMPLIFIED VERSION OF THE UA1 JET
C          ALGORITHM WITH JET RADIUS RJET AND MINIMUM SCALAR TRANSVERSE
C          ENERGY EJCUT
C            (RJET=1., EJCUT=5. FOR UA1)
C          USE
C
C     CALL GETJETM(RJET,EJCUT)
C
C
C-----------------------------------------------------------------------
C 
C          ADDED BY MIKE SEYMOUR: PARTON-LEVEL CALORIMETER. ALL PARTONS
C          ARE CONSIDERED TO BE HADRONS, SO IN FACT RESEM IS IGNORED
C
C     CALL CALPARM
C
C          HARD PARTICLE CALORIMETER. ONLY USES THOSE PARTICLES WHICH
C          CAME FROM THE HARD PROCESS, AND NOT THE UNDERLYING EVENT
C
C     CALL CALHARM
C
C-----------------------------------------------------------------------
      SUBROUTINE CALINIM
C                
C          INITIALIZE CALORIMETER FOR CALSIMM AND GETJETM.  NOTE THAT
C          BECAUSE THE INITIALIZATION IS SEPARATE, CALSIMM CAN BE
C          CALLED MORE THAN ONCE TO SIMULATE PILEUP OF SEVERAL EVENTS.
C
      IMPLICIT NONE
C...GETJET commonblocks
      INTEGER MNCY,MNCPHI,NCY,NCPHI,NJMAX,JETNO,NCJET
      DOUBLE PRECISION YCMIN,YCMAX,DELY,DELPHI,ET,STHCAL,CTHCAL,CPHCAL,
     &  SPHCAL,PCJET,ETJET
      PARAMETER (MNCY=200)
      PARAMETER (MNCPHI=200)
      COMMON/CALORM/DELY,DELPHI,ET(MNCY,MNCPHI),
     $CTHCAL(MNCY),STHCAL(MNCY),CPHCAL(MNCPHI),SPHCAL(MNCPHI),
     $YCMIN,YCMAX,NCY,NCPHI
      PARAMETER (NJMAX=500)
      COMMON/GETCOMM/PCJET(4,NJMAX),ETJET(NJMAX),JETNO(MNCY,MNCPHI),
     $     NCJET

      INTEGER IPHI,IY
      DOUBLE PRECISION PI,PHIX,YX,THX
      PARAMETER (PI=3.141593D0)
      LOGICAL FSTCAL
      DATA FSTCAL/.TRUE./
C
C          INITIALIZE ET ARRAY.
      DO 100 IPHI=1,NCPHI
      DO 100 IY=1,NCY
100   ET(IY,IPHI)=0.
C
      IF (FSTCAL) THEN
C          CALCULATE TRIG. FUNCTIONS.
        DELPHI=2.*PI/FLOAT(NCPHI)
        DO 200 IPHI=1,NCPHI
        PHIX=DELPHI*(IPHI-.5)
        CPHCAL(IPHI)=COS(PHIX)
        SPHCAL(IPHI)=SIN(PHIX)
200     CONTINUE
        DELY=(YCMAX-YCMIN)/FLOAT(NCY)
        DO 300 IY=1,NCY
        YX=DELY*(IY-.5)+YCMIN
        THX=2.*ATAN(EXP(-YX))
        CTHCAL(IY)=COS(THX)
        STHCAL(IY)=SIN(THX)
300     CONTINUE
        FSTCAL=.FALSE.
      ENDIF
      END
C
      SUBROUTINE CALSIMM
C                
C          SIMPLE CALORIMETER SIMULATION.  ASSUME UNIFORM Y AND PHI
C          BINS
C...HEPEVT commonblock.
      INTEGER NMXHEP,NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      PARAMETER (NMXHEP=4000)
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      DOUBLE PRECISION PHEP,VHEP
      SAVE /HEPEVT/

C...GETJET commonblocks
      INTEGER MNCY,MNCPHI,NCY,NCPHI,NJMAX,JETNO,NCJET
      DOUBLE PRECISION YCMIN,YCMAX,DELY,DELPHI,ET,STHCAL,CTHCAL,CPHCAL,
     &  SPHCAL,PCJET,ETJET
      PARAMETER (MNCY=200)
      PARAMETER (MNCPHI=200)
      COMMON/CALORM/DELY,DELPHI,ET(MNCY,MNCPHI),
     $CTHCAL(MNCY),STHCAL(MNCY),CPHCAL(MNCPHI),SPHCAL(MNCPHI),
     $YCMIN,YCMAX,NCY,NCPHI
      PARAMETER (NJMAX=500)
      COMMON/GETCOMM/PCJET(4,NJMAX),ETJET(NJMAX),JETNO(MNCY,MNCPHI),
     $     NCJET

      INTEGER IHEP,ID,IY,IPHI
      DOUBLE PRECISION PI,YIP,PSERAP,PHIIP,EIP
      PARAMETER (PI=3.141593D0)
C
C          FILL CALORIMETER
C
      DO 200 IHEP=1,NHEP
      IF (ISTHEP(IHEP).EQ.1) THEN
        YIP=PSERAP(PHEP(1,IHEP))
        IF(YIP.LT.YCMIN.OR.YIP.GT.YCMAX) GOTO 200
        ID=ABS(IDHEP(IHEP))
C---EXCLUDE TOP QUARK, LEPTONS, PROMPT PHOTONS
        IF ((ID.GE.11.AND.ID.LE.16).OR.ID.EQ.6.OR.ID.EQ.22) GOTO 200
C
        PHIIP=ATAN2(PHEP(2,IHEP),PHEP(1,IHEP))
        IF(PHIIP.LT.0.) PHIIP=PHIIP+2.*PI
        IY=INT((YIP-YCMIN)/DELY)+1
        IPHI=INT(PHIIP/DELPHI)+1
        EIP=PHEP(4,IHEP)
C            WEIGHT BY SIN(THETA)
        ET(IY,IPHI)=ET(IY,IPHI)+EIP*STHCAL(IY)
      ENDIF
  200 CONTINUE
  999 END
      SUBROUTINE GETJETM(RJET,EJCUT,ETAJCUT)
C                
C          SIMPLE JET-FINDING ALGORITHM (SIMILAR TO UA1).
C
C     FIND HIGHEST REMAINING CELL > ETSTOP AND SUM SURROUNDING
C          CELLS WITH--
C            DELTA(Y)**2+DELTA(PHI)**2<RJET**2
C            ET>ECCUT.
C          KEEP JETS WITH ET>EJCUT AND ABS(ETA)<ETAJCUT
C          THE UA1 PARAMETERS ARE RJET=1.0 AND EJCUT=5.0
C                  
      IMPLICIT NONE
C...GETJET commonblocks
      INTEGER MNCY,MNCPHI,NCY,NCPHI,NJMAX,JETNO,NCJET
      DOUBLE PRECISION YCMIN,YCMAX,DELY,DELPHI,ET,STHCAL,CTHCAL,CPHCAL,
     &  SPHCAL,PCJET,ETJET
      PARAMETER (MNCY=200)
      PARAMETER (MNCPHI=200)
      COMMON/CALORM/DELY,DELPHI,ET(MNCY,MNCPHI),
     $CTHCAL(MNCY),STHCAL(MNCY),CPHCAL(MNCPHI),SPHCAL(MNCPHI),
     $YCMIN,YCMAX,NCY,NCPHI
      PARAMETER (NJMAX=500)
      COMMON/GETCOMM/PCJET(4,NJMAX),ETJET(NJMAX),JETNO(MNCY,MNCPHI),
     $     NCJET

      INTEGER IPHI,IY,J,K,NPHI1,NPHI2,NY1,
     &  NY2,IPASS,IYMX,IPHIMX,ITLIS,IPHI1,IPHIX,IY1,IYX
      DOUBLE PRECISION PI,RJET,
     &  ETMAX,ETSTOP,RR,ECCUT,PX,EJCUT
      PARAMETER (PI=3.141593D0)
      DOUBLE PRECISION ETAJCUT,PSERAP
C
C          PARAMETERS
      DATA ECCUT/0.1D0/
      DATA ETSTOP/1.5D0/
      DATA ITLIS/6/
C
C          INITIALIZE
C
      DO 100 IPHI=1,NCPHI
      DO 100 IY=1,NCY
100   JETNO(IY,IPHI)=0
      DO 110 J=1,NJMAX
      ETJET(J)=0.
      DO 110 K=1,4
110   PCJET(K,J)=0.
      NCJET=0
      NPHI1=RJET/DELPHI
      NPHI2=2*NPHI1+1
      NY1=RJET/DELY
      NY2=2*NY1+1
      IPASS=0
C
C          FIND HIGHEST CELL REMAINING
C
1     ETMAX=0.
      DO 200 IPHI=1,NCPHI
      DO 210 IY=1,NCY
      IF(ET(IY,IPHI).LT.ETMAX) GOTO 210
      IF(JETNO(IY,IPHI).NE.0) GOTO 210
      ETMAX=ET(IY,IPHI)
      IYMX=IY
      IPHIMX=IPHI
210   CONTINUE
200   CONTINUE
      IF(ETMAX.LT.ETSTOP) RETURN
C
C          SUM CELLS
C
      IPASS=IPASS+1
      IF(IPASS.GT.NCY*NCPHI) THEN
        WRITE(ITLIS,8888) IPASS
8888    FORMAT(//' ERROR IN GETJETM...IPASS > ',I6)
        RETURN
      ENDIF
      NCJET=NCJET+1
      IF(NCJET.GT.NJMAX) THEN
        WRITE(ITLIS,9999) NCJET
9999    FORMAT(//' ERROR IN GETJETM...NCJET > ',I5)
        RETURN
      ENDIF
      DO 300 IPHI1=1,NPHI2
      IPHIX=IPHIMX-NPHI1-1+IPHI1
      IF(IPHIX.LE.0) IPHIX=IPHIX+NCPHI
      IF(IPHIX.GT.NCPHI) IPHIX=IPHIX-NCPHI
      DO 310 IY1=1,NY2
      IYX=IYMX-NY1-1+IY1
      IF(IYX.LE.0) GOTO 310
      IF(IYX.GT.NCY) GOTO 310
      IF(JETNO(IYX,IPHIX).NE.0) GOTO 310
      RR=(DELY*(IY1-NY1-1))**2+(DELPHI*(IPHI1-NPHI1-1))**2
      IF(RR.GT.RJET**2) GOTO 310
      IF(ET(IYX,IPHIX).LT.ECCUT) GOTO 310
      PX=ET(IYX,IPHIX)/STHCAL(IYX)
C          ADD CELL TO JET
      PCJET(1,NCJET)=PCJET(1,NCJET)+PX*STHCAL(IYX)*CPHCAL(IPHIX)
      PCJET(2,NCJET)=PCJET(2,NCJET)+PX*STHCAL(IYX)*SPHCAL(IPHIX)
      PCJET(3,NCJET)=PCJET(3,NCJET)+PX*CTHCAL(IYX)
      PCJET(4,NCJET)=PCJET(4,NCJET)+PX
      ETJET(NCJET)=ETJET(NCJET)+ET(IYX,IPHIX)
      JETNO(IYX,IPHIX)=NCJET
310   CONTINUE
300   CONTINUE
C
C          DISCARD JET IF ET < EJCUT.
C
      IF(ETJET(NCJET).GT.EJCUT.AND.ABS(PSERAP(PCJET(1,NCJET))).LT
     $     .ETAJCUT) GOTO 1
      ETJET(NCJET)=0.
      DO 400 K=1,4
400   PCJET(K,NCJET)=0.
      NCJET=NCJET-1
      GOTO 1
      END
C-----------------------------------------------------------------------
      SUBROUTINE CALDELM(ISTLO,ISTHI)
C     LABEL ALL PARTICLES WITH STATUS BETWEEN ISTLO AND ISTHI (UNTIL A
C     PARTICLE WITH STATUS ISTOP IS FOUND) AS FINAL-STATE, CALL CALSIMM
C     AND THEN PUT LABELS BACK TO NORMAL
C-----------------------------------------------------------------------
      IMPLICIT NONE
      INTEGER MAXNUP
      PARAMETER(MAXNUP=500)
C...HEPEVT commonblock.
      INTEGER NMXHEP,NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      PARAMETER (NMXHEP=4000)
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      DOUBLE PRECISION PHEP,VHEP
      SAVE /HEPEVT/
      INTEGER ISTOLD(NMXHEP),IHEP,IST,ISTLO,ISTHI,ISTOP,IMO,icount


      CALL CALSIMM
      END
