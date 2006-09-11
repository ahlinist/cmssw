* this routine is renamed and modified in order to read parameters from
* charybdis common blocks. srs,  13/08/2006 
* all modification are marked by "srs"
*CDECK  ID>, UPINIT.
*CMZ :-        -17/07/03  18.11.30  by  Peter Richardson
*-- Author :    Chris Harris and Peter Richardson
C----------------------------------------------------------------------
*       subroutine CHARYBDIS_INI    ! srs
       SUBROUTINE UPINIT         
C----------------------------------------------------------------------
C     Les Houches initialisation routine
C----------------------------------------------------------------------
c      INCLUDE 'charybdis1002.inc' 
#include "charybdis1002.inc"      
C--Les Houches run common block
      INTEGER MAXPUP
      PARAMETER(MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON /HEPRUP/ IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &                IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),
     &                XMAXUP(MAXPUP),LPRUP(MAXPUP)
C--event common block
      INTEGER MAXNUP
      PARAMETER (MAXNUP=500)
      INTEGER NUP,IDPRUP,IDUP,ISTUP,MOTHUP,ICOLUP
      DOUBLE PRECISION XWGTUP,SCALUP,AQEDUP,AQCDUP,PUP,VTIMUP,SPINUP
      COMMON/HEPEUP/NUP,IDPRUP,XWGTUP,SCALUP,AQEDUP,AQCDUP,
     &              IDUP(MAXNUP),ISTUP(MAXNUP),MOTHUP(2,MAXNUP),
     &              ICOLUP(2,MAXNUP),PUP(5,MAXNUP),VTIMUP(MAXNUP),
     &              SPINUP(MAXNUP)
      save /HEPRUP/, /HEPEUP/
C--local variables
      INTEGER NSEARCH,I
*      common /CHRBD1/ NSEARCH 
      DOUBLE PRECISION WGTSUM,WGTSQ,SPNZRO,SPNHLF,SPNONE,PTOTAL
      DOUBLE PRECISION PPROBS(6,3)
      DATA PPROBS /1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0, 1.0d0,
     &     0.773d0, 0.776d0, 0.761d0, 0.743d0, 0.726d0, 0.711d0,
     &     0.688d0, 0.827d0, 0.909d0, 0.959d0, 0.990d0, 1.011d0/
C--beam particles and energies (only proton=2212 and antiproton=-2212)
* srs: setting of these parameters is done in other routine of stagen
      IDBMUP(1) = 2212
      IDBMUP(2) = 2212
      EBMUP(1)  = 7000.0D0
      EBMUP(2)  = 7000.0D0
*....

C--Set MPLNCK and define what is meant by it:
C--MSSDEF=1 means M_GT, MSSDEF=2 means M_DL and MSSDEF=3 means M_D
      MPLNCK=1000.0D0
      MSSDEF=2
C--Set number of dimensions (number of EXTRA dimensions is n=D-4)
C--TOTDIM can be in the range 6-11
      TOTDIM=6
C--Use Giddings+Thomas momentum scale for calling pdfs?
C--(See page 12 of hep-ph/0106219)
      GTSCA=.FALSE.
C--Set mass window for black holes produced
      MINMSS=5000.0D0
      MAXMSS=EBMUP(1)+EBMUP(2)
C--Set NBODY decay of BH remnant - NBODY can be in range 2-5.
      NBODY=2
C--Turn time variation of BH Hawking temperature in decay on or off
      TIMVAR=.TRUE.
C--Set which decay products are to be used:
C--MSSDEC=1 gives no heavy particles
C--MSSDEC=2 gives t, W and Z as well
C--MSSDEC=3 gives t, W, Z and Higgs as well  
      MSSDEC=3
C--Turn grey-body factors on/off
      GRYBDY=.TRUE.
C--Turn kinematic cut-off of decay on (rather than M=MPLANCK cut-off)
      KINCUT=.FALSE.
C--print out option (0=no printout, 1 =errors only, 2=errors+info)
      IPRINT = 1
C--parameters for the initial maximum weight search
      NSEARCH=10000
***
       CALL charbd_ini(NSEARCH)  ! to set the parameters given below 
* end of srs-modifications
C--pdf's for the beams (use the generator default)
C--MUST BE THE SAME FOR BOTH BEAMS
      PDFGUP(1) = -1
      PDFGUP(2) = -1
      PDFSUP(1) = -1
      PDFSUP(2) = -1
C--------------------------------------------------------------------
C     END OF USER VARIABLES DON'T TOUCH ANYTHING BELOW HERE
C--------------------------------------------------------------------
C--probabilities for particles of different spin (arbitrary normalisation)
      IF (GRYBDY) THEN
         SPNZRO = PPROBS(TOTDIM-5,1)
         SPNHLF = PPROBS(TOTDIM-5,2)
         SPNONE = PPROBS(TOTDIM-5,3)
      ELSE
         SPNZRO = 1d0
         SPNHLF = 0.750d0
         SPNONE = 1d0
      ENDIF
C--calculation of probability of emission for different particle types
C--only light SM particles
      IF (MSSDEC.EQ.1) THEN
         PTOTAL = (78.0d0*SPNHLF)+(18.0d0*SPNONE)
         PQUARK = (60.0d0*SPNHLF)/PTOTAL
         PLEPT  = (12.0d0*SPNHLF)/PTOTAL
         PNEUT  = (6.0d0*SPNHLF)/PTOTAL
         PGLUON = (16.0d0*SPNONE)/PTOTAL
         PGAMMA = (2.0d0*SPNONE)/PTOTAL
         PWBOSN = 0.0D0
         PZBOSN = 0.0D0
         PHIGGS = 0.0D0   
C--light SM partcles + top W/Z      
      ELSEIF (MSSDEC.EQ.2) THEN
         PTOTAL = (3.0d0*SPNZRO)+(90.0d0*SPNHLF)+(24.0d0*SPNONE)
         PQUARK = (72.0d0*SPNHLF)/PTOTAL
         PLEPT  = (12.0d0*SPNHLF)/PTOTAL
         PNEUT  = (6.0d0*SPNHLF)/PTOTAL
         PGLUON = (16.0d0*SPNONE)/PTOTAL
         PGAMMA = (2.0d0*SPNONE)/PTOTAL
         PZBOSN = (SPNZRO+(2.0d0*SPNONE))/PTOTAL
         PWBOSN = 2.0d0*PZBOSN
         PHIGGS = 0.0D0 
C--light SM particles +top W/Z and Higgs
      ELSE
         PTOTAL = (4.0d0*SPNZRO)+(90.0d0*SPNHLF)+(24.0d0*SPNONE)
         PQUARK = (72.0d0*SPNHLF)/PTOTAL
         PLEPT  = (12.0d0*SPNHLF)/PTOTAL
         PNEUT  = (6.0d0*SPNHLF)/PTOTAL
         PGLUON = (16.0d0*SPNONE)/PTOTAL
         PGAMMA = (2.0d0*SPNONE)/PTOTAL
         PZBOSN = (SPNZRO+(2.0d0*SPNONE))/PTOTAL
         PWBOSN = 2.0d0*PZBOSN
         PHIGGS = SPNZRO/PTOTAL
      ENDIF
C--what do do with the weights(here are generating weighted events)
      IDWTUP = 1
C--only one process
      NPRUP  = 1
C--communication code
      LPRUP(1) = 1
C--calculate the maximum weight
      WGTSUM = 0.0D0
      WGTSQ = 0.0D0
      XMAXUP(1) = 0.0D0
      DO I=1,NSEARCH
         CALL CHEVNT(.FALSE.)
         WGTSUM = WGTSUM+XWGTUP
         WGTSQ  = WGTSQ+XWGTUP**2
         XMAXUP(1) = MAX(XMAXUP(1),XWGTUP)
      ENDDO
      WGTSUM = WGTSUM/DBLE(NSEARCH)
      WGTSQ  = MAX(WGTSQ/DBLE(NSEARCH)-WGTSUM**2,0.0D0)
      WGTSQ  = SQRT(WGTSQ/ DBLE(NSEARCH))
C--cross section
      XSECUP(1) = wgtsum
C--error on the cross section
      XERRUP(1) = wgtsq
C--output initialisation information
C--header
      WRITE(*,10)
C--beam parameters
      WRITE(*,11) IDBMUP(1),EBMUP(1),IDBMUP(2),EBMUP(2)
C--basic parameters
      WRITE(*,12) MINMSS,MAXMSS,MPLNCK,MSSDEF,TOTDIM,TIMVAR,GRYBDY,
     &     KINCUT
C--choice of outgoing particles
      IF(MSSDEC.EQ.1) THEN
         WRITE(*,13)
      ELSEIF(MSSDEC.EQ.2) THEN
         WRITE(*,14)
      ELSEIF(MSSDEC.EQ.3) THEN
         WRITE(*,15)
      ENDIF
C--choice of scale
      IF(GTSCA) THEN
         WRITE(*,17)
      ELSE
         WRITE(*,18)
      ENDIF
C--information on the cross section
      WRITE(*,19) XSECUP(1),XERRUP(1),XMAXUP(1)
C--particle production probabilites
      WRITE(*,20) PQUARK,PLEPT,PNEUT,PGLUON,PGAMMA,PZBOSN,PWBOSN,PHIGGS
      RETURN
C--format statements for the output
* 10   FORMAT(//10X,'       CHARYBDIS 1.000    ?? July 2003        ',//,
 10   FORMAT(//10X,'       CHARYBDIS 1.002       July 2006        ',//,
     &         10X,'Please reference: C.M. Harris, P. Richardson &',/,
     &         10X,'                  B.R. Webber                 ',/,
     &         10X,'                 hep-ph/0307???               '/)
 11   FORMAT(/10X,'INPUT CONDITIONS FOR THIS RUN'//
     &        10X,'BEAM 1 (',I8,') ENG. =',F10.2/
     &        10X,'BEAM 2 (',I8,') ENG. =',F10.2/)
 12   FORMAT(
     &     10X,'MINIMUM BH MASS    =',F11.2/
     &     10X,'MAXIMUM BH MASS    =',F11.2/
     &     10X,'PLANCK MASS        =',F11.2/
     &     10X,'DEFN OF PLANCK MASS=',I5/
     &     10X,'NO. OF DIMENSIONS  =',I5/
     &     10X,'TIME VARIATION     =',L5/
     &     10X,'GREY BODY EFFECTS  =',L5/
     &     10X,'KINEMATIC CUT      =',L5)
 13   FORMAT(
     &     10X,'ONLY LIGHT SM PARTICLES PRODUCED')
 14   FORMAT(
     &     10X,'ALL SM PARTICLES BUT HIGGS PRODUCED')
 15   FORMAT(
     &     10X,'ALL SM PARTICLES PRODUCED')
      WRITE(*,16) NBODY
 16   FORMAT(
     &     10X,'PRODUCING ',I1,' PARTICLES IN REMNANT DECAY')
 17   FORMAT(
     &     10X,'USING BLACK RADIUS AS SCALE')
 18   FORMAT(
     &     10X,'USING BLACK MASS AS SCALE')
 19   FORMAT(
     &     10X,'CROSS SECTION (PB) =',G12.4/
     &     10X,'ERROR IN C-S  (PB) =',G12.4/
     &     10X,'MAXIMUM WEIGHT     =',E12.4)
 20   FORMAT(/10X,'PARTICLE PRODUCTION PROBABILITIES'//
     &        10X,'QUARK                  =',F10.4/
     &        10X,'CHARGED LEPTON         =',F10.4/
     &        10X,'NEUTRINO               =',F10.4/
     &        10X,'GLUON                  =',F10.4/
     &        10X,'PHOTON                 =',F10.4/
     &        10X,'Z BOSON                =',F10.4/
     &        10X,'W BOSON                =',F10.4/
     &        10X,'HIGGS BOSON            =',F10.4)
      END

c----------------------------------------------------------------------------c
       subroutine charbd_ini(NSEARCH)
*      ---------------------
*      implicit none      
#include "charybdis1002.inc"      
#include "txpar.inc"   
      INTEGER MAXPUP
      PARAMETER(MAXPUP=100)
      INTEGER IDBMUP,PDFGUP,PDFSUP,IDWTUP,NPRUP,LPRUP
      DOUBLE PRECISION EBMUP,XSECUP,XERRUP,XMAXUP
      COMMON /HEPRUP/ IDBMUP(2),EBMUP(2),PDFGUP(2),PDFSUP(2),
     &                IDWTUP,NPRUP,XSECUP(MAXPUP),XERRUP(MAXPUP),
     &                XMAXUP(MAXPUP),LPRUP(MAXPUP)
* legend:
* Ipar(140) = NSEARCH - parameters for the initial maximum weight search (def = 10000)
* Ipar(141) = MSSDEF : MSSDEF=1 means M_GT, MSSDEF=2 means M_DL and MSSDEF=3 means M_D
* Ipar(142) = TOTDIM can be in the range 6-11 
* Ipar(143) = NBODY the num. of part. produced in the decay of the BH hole remnant (2-5)
* Ipar(144) = MSSDEC the type of particles to  produce in the decay, 
*    1= only light SM  particles,
*    2= light SM  particles + top W/Z and 
*    3= all SM particles.
***
* Ipar(145) <==> GTSCA - use Giddings+Thomas momentum scale for calling pdfs?
* Ipar(145) = 0 ==> .false. 
* Ipar(145) = 1 ==> .true.  
*
* Ipar(146) <==> TIMVAR time variation of the BH Hawking temperature 
* Ipar(146) = 0 ==> .false. is OFF
* Ipar(146) = 1 ==> .true.  is ON
* 
* Ipar(147) <==> GRYBDY , controls the use of the gray body  factors
* Ipar(147) = 0 ==>  GRYBDY=.false. the black body spectrum is used
* Ipar(147) = 1 ==>  GRYBDY=.true.  the correct  D-dimensional gray body factors are used
* 
* Ipar(148) <==> KINCUT Controls  the  termination  of the  Schwarzschild  phase
* Ipar(148) = 0 ==>  KINCUT=.false.
* Ipar(148) = 1 ==>  KINCUT=.true.
*
************************
* float parameters
* Rpar(141) = MPLNCK Planck mass
* Rpar(142) = MINMSS low bound of mass window for black holes produced
* Rpar(143) = MAXMSS up  bound of mass window for black holes produced
*    if(MAXMSS < minMSS) => maxMSS = E1 + E2
*..............
* CHARYBDIS common blocks
      integer         NSEARCH 
*      common /CHRBD1/ NSEARCH
*      save /CHRBD1/ 
*
C--parameters for the initial maximum weight search
      if(Ipar(140).ne.Nsearch) then
         NSEARCH = Ipar(140) 
         if(Nsearch.le.0) NSEARCH = 10000 ! default
      endif
        print*,' CHARYBDIS:  Nsearch = ',Nsearch
* 
      if(Ipar(141).ge.1.and.Ipar(141).le.3) MSSDEF = Ipar(141)
*
C--Set number of dimensions (number of EXTRA dimensions is n=D-4)
C--TOTDIM can be in the range 6-11
      if(Ipar(142).ge.6.and.Ipar(142).le.11) TOTDIM = Ipar(142) 
*
C--Set NBODY decay of BH remnant - NBODY can be in range 2-5.
      if(Ipar(143).ge.2.and.Ipar(143).le.5)  NBODY  = Ipar(143) 
*
C--Set which decay products are to be used:
C--MSSDEC=1 gives no heavy particles
C--MSSDEC=2 gives t, W and Z as well
C--MSSDEC=3 gives t, W, Z and Higgs as well  
      if(Ipar(144).ge.1.and.Ipar(144).le.3)  MSSDEC=ipar(144)

**** LOGICAL parameters
C--Use Giddings+Thomas momentum scale for calling pdfs? (See page 12 of hep-ph/0106219)
      if(Ipar(145).eq.1)  GTSCA = .true.
C--Turn time variation of BH Hawking temperature in decay on or off
      if(Ipar(146).eq.0)  TIMVAR = .false. 
C--Turn grey-body factors on/off
      if(Ipar(147).eq.0)  GRYBDY = .false.
C--Turn kinematic cut-off of decay on (rather than M=MPLANCK cut-off)
      if(Ipar(148).eq.1) KINCUT=.true.
*...
C
*  FLOAT parameters
      if(Rpar(141).ge.1.d0) MPLNCK=rpar(141) 
C--Set mass window for black holes produced
      if(Rpar(142).ge.1.d0) MINMSS=rpar(142)

      if(Rpar(143).le.MINMSS) then
         MAXMSS=EBMUP(1)+EBMUP(2)
      else
         MAXMSS=Rpar(143)
      endif
      return
      end
c----------------------------------------------------------------------------c
C----------------------------------------------------------------------
      SUBROUTINE UPEVNT
C----------------------------------------------------------------------
C     Les Houches event routine
C----------------------------------------------------------------------
      IMPLICIT NONE
      CALL CHEVNT(.TRUE.)
      END

