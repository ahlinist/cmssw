******************************************************
C*          MadEvent - Pythia interface.              *
C*            Version 3.7.2, 4 Oct 2006               *
C*                                                    *
C*  - Set mass of massless outgoing particles to      *
C*    Pythia mass (PMAS(I,1))                         *
C*                                                    *
C*  Version 3.6-CMSSW                                 *
C*  dkcira                                            *
C*                                                    *
C*  - add changes proposed by S. Bolognesi to allow   *
C*    reading of Phantom files. Two flags are added   *
C*    that disallow setting the model and reading the *
C*    input cards file SLHA.dat                       *
C*  - Hack for reading input file in present setup    *
C*    of CMSSW                                        *
C*  - Comment out unused variables to avoid warnings  *
C*    at build time                                   *
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
C* The logical unit LNHIN must be an opened MadEvent  *
C*   output file.                                     *
C*                                                    *
C* The logical unit LNHOUT is by default 6 (std out). *
C* If MODE (giving IDWTUP) is not set, it will be set *
C*   automatically to 3 (unweighted events)           *
C*                                                    *
C* If some other scale is desired for parton showers  *
C*   than the one used as factorization scale by      *
C*   MadEvent, please make the necessary changes      *
C*   to the subroutine PYMASC                         *
C*                                                    *
C* If insertion of mothers is desired, please find    *
C* example code in UPEVNT below                       *
C*                                                    *
C******************************************************

C...UPINIT
C...Routine called by PYINIT to set up user-defined processes.
      
      SUBROUTINE UPINIT
      
      IMPLICIT NONE
c      CHARACTER*132 CHAR_READ

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

C...Extra commonblock to transfer run info.
      INTEGER LNHIN,LNHOUT,MSCAL
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL
      DATA LNHIN,LNHOUT,MSCAL/77,6,1/
      SAVE/UPPRIV/

C...Lines to read in assumed never longer than 200 characters. 
c      INTEGER MAXLEN,IBEG,IPR,I
      INTEGER MAXLEN,IBEG,IPR
      PARAMETER (MAXLEN=200)
      CHARACTER*(MAXLEN) STRING

C...add opening of file as done presently in CMSSW
      CHARACTER EVFILE*500

C...Format for reading lines.
      CHARACTER*6 STRFMT
      STRFMT='(A000)'
      WRITE(STRFMT(3:5),'(I3)') MAXLEN

C...Note that Pythia will not look at PDFGUP and PDFSUP.  
      PDFGUP(1)=4
      PDFSUP(1)=46
      PDFGUP(2)=PDFGUP(1)
      PDFSUP(2)=PDFSUP(1)
      write(*,*) " PDFSUP=",PDFSUP(1), PDFSUP(2),
     &           " PDFGUP=",PDFGUP(1),PDFGUP(2),
     &           " MSTP(51)=",MSTP(51)

C connects unit 22 w file inparm.dat and reads from unit 22, fmt=format, A=
C caracter, 500=500 characters.first read reads one line of max 500 characters
C then read second line into evfile, third line into dummy and fourth into process
      OPEN(22,FILE='MGinput.dat',STATUS='UNKNOWN')
      READ(22,FMT='(A500)') EVFILE
      WRITE(*,*)' ',EVFILE
      OPEN(LNHIN,FILE=EVFILE,STATUS='UNKNOWN')

C...Extract the model parameter card and read it.
      CALL MODELPAR(LNHIN)

C...Loop until finds line beginning with "<init>" or "<init ". 
  100 READ(LNHIN,STRFMT,END=130,ERR=130) STRING
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

c      IF(PDFSUP(1).NE.19070)THEN
c     Not CTEQ5L, which is standard in Pythia
c        MSTP(52)=2
c     The following works for both PDFLIB and LHAPDF (where PDFGUP(1)=0)
c     But note that the MadEvent output uses the LHAPDF numbering scheme
c        MSTP(51)=1000*PDFGUP(1)+PDFSUP(1)
c      ENDIF

C...Initialize widths and partial widths for resonances.
      CALL PYINRE
        
C...Calculate xsec reduction due to non-decayed resonances
C...based on first event only!

      CALL BRSUPP

      REWIND(LNHIN)

      RETURN

C...Error exit: give up if initalization does not work.
  130 WRITE(*,*) ' Failed to read LHEF initialization information.'
      WRITE(*,*) ' Event generation will be stopped.'
      STOP  
      END

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
      INTEGER LNHIN,LNHOUT,MSCAL
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL

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
      
C*********************************************************************
      
C...UPEVNT
C...Routine called by PYEVNT or PYEVNW to get user process event
      
      SUBROUTINE UPEVNT

      IMPLICIT NONE

C...Pythia common blocks
      INTEGER PYCOMP,KCHG
      DOUBLE PRECISION PMAS,PARF,VCKM
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
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
      INTEGER LNHIN,LNHOUT,MSCAL
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL

C...Local variables
      INTEGER I,J,IBEG
c      DOUBLE PRECISION PSUM,ESUM,PM1,PM2,A1,A2,A3,A4,A5
      DOUBLE PRECISION PSUM,ESUM

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
      ESUM=0d0
      PSUM=0d0
      DO 120 I=1,NUP
        READ(LNHIN,*,END=130,ERR=130) IDUP(I),ISTUP(I),
     &  MOTHUP(1,I),MOTHUP(2,I),ICOLUP(1,I),ICOLUP(2,I),
     &  (PUP(J,I),J=1,5),VTIMUP(I),SPINUP(I)
C...Reset resonance energies
        IF(ISTUP(I).EQ.2) PUP(4,I)=0
        IF(ISTUP(I).EQ.1)THEN
           IF(PUP(5,I).EQ.0D0)THEN
C...Set massless particle masses to Pythia default. Adjust energy. 
              PUP(5,I)=PMAS(IABS(PYCOMP(IDUP(I))),1)
              PUP(4,I)=SQRT(PUP(5,I)**2+PUP(1,I)**2+PUP(2,I)**2+
     $             PUP(3,I)**2)
           ENDIF
           ESUM=ESUM+PUP(4,I)
           PSUM=PSUM+PUP(3,I)
C...Set resonance energy
           IF(MOTHUP(1,I).GT.2)
     $          PUP(4,MOTHUP(1,I))=PUP(4,MOTHUP(1,I))+PUP(4,I)
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

C...Assuming massless incoming particles - otherwise Pythia adjusts
C...the momenta to make them massless
      DO I=1,2
         PUP(3,I)=0.5d0*(PSUM+SIGN(ESUM,PUP(3,I)))
         PUP(4,I)=ABS(PUP(3,I))
         PUP(5,I)=0d0
      ENDDO

C...If you want to use some other scale for parton showering then the 
C...factorisation scale given by MadEvent, please implement the function PYMASC
C...(example function included below) 

      IF(MSCAL.GT.0) CALL PYMASC(SCALUP)

      RETURN

C...Error exit, typically when no more events.
  130 WRITE(*,*) ' Failed to read LHEF event information.'
      WRITE(*,*) ' Will assume end of file has been reached.'
      NUP=0
      RETURN
      END
      
C*********************************************************************
C   PYMASC
C   Implementation of scale used in Pythia parton showers

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
      INTEGER LNHIN,LNHOUT,MSCAL
      COMMON/UPPRIV/LNHIN,LNHOUT,MSCAL

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
C...Local variables      
      CHARACTER*132 buff
      CHARACTER*8 model
      INTEGER iunit

C...add flags for the case when no model is defined in the head of LH and no cards should be read
      INTEGER model_def,cards_def
      
      model_def = 0
      cards_def = 0
      buff='#'
      do 100 while(.true.)
         read(iunit,'(a132)',end=105,err=98) buff
         if(buff.eq.' ') then
            goto 100
         endif
         if(buff(1:5).eq.'<init') goto 105
         if(buff(1:14).eq.'# Begin MODEL')then
            read(iunit,'(a132)',end=99,err=98) buff
            model=buff
            model_def=1
         endif
         if(buff(1:23).eq.'# Begin param_card.dat')then
            open(24,FILE='SLHA.dat',ERR=90)
            do 10 while(.true.)
               read(iunit,'(a132)',end=99,err=98) buff
               cards_def=1
               if(buff(1:21).eq.'# End param_card.dat') goto 15
               write(24,'(a80)') buff
 10         continue
 15         continue
            close(24)
            goto 105
         endif
 100  continue
 105  continue
      REWIND(iunit)
      
      if(model_def.EQ.1) then
        write(*,*) 'Reading model: ',model
      endif

      if(cards_def.EQ.1) then
        open(24,FILE='SLHA.dat',ERR=91)
c     Pick out SM parameters
        CALL READSMLHA(24)
      endif

      if(model.eq.'mssm')then
         IMSS(1) = 11
         IMSS(21)= 24 ! Logical unit number of SLHA spectrum file
c         IMSS(22)= 24 ! Logical unit number of SLHA decay file
      endif

      RETURN

 90   WRITE(*,*)'Could not open file SLHA.dat for writing'
      WRITE(*,*)'Quitting...'
      STOP
 91   WRITE(*,*)'Could not open file SLHA.dat for reading'
      WRITE(*,*)'Quitting...'
      STOP
 98   WRITE(*,*)'Unexpected error reading file'
      WRITE(*,*)'Quitting...'
      STOP
 99   WRITE(*,*)'Unexpected end of file'
      WRITE(*,*)'Quitting...'
      STOP

      END

C*********************************************************************
      
C...READSMLHA
C...Reads the SMLHA file to extract SM parameters
C...as well as all particle masses.

      SUBROUTINE READSMLHA(iunit)

      IMPLICIT NONE

C...Pythia common blocks
      INTEGER PYCOMP,MSTU,MSTJ,KCHG
      DOUBLE PRECISION PARU,PARJ,PMAS,PARF,VCKM
C...Parameters.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)

C...Local variables      
      CHARACTER*132 buff,block_name
c      CHARACTER*8 model
      INTEGER iunit,ivalue
      DOUBLE PRECISION value
      LOGICAL block_found
      
      block_found=.false.

      do 100 while(.true.)
         read(iunit,'(a132)',end=105,err=98) buff
         call case_trap2(buff,132)
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
               read(iunit,'(a132)',end=105,err=98) buff
               if(buff(1:1).eq.'#') goto 10
               if(buff(1:1).ne.' ') then
                  block_found=.false.
                  backspace(iunit)
                  goto 100
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
 100  continue
 105  continue

      REWIND(iunit)

      RETURN
 98   WRITE(*,*)'Unexpected error reading file'
      WRITE(*,*)'Quitting...'
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

