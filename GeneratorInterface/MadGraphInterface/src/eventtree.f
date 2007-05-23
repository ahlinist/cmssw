
      SUBROUTINE EVENTTREE
C Dorian Kcira, 2007.05.22 - write out an event tree
C copied and hacked from pythia.f routine of the MadGraph package, original code from Johan Alwall
      IMPLICIT NONE
C      include 'stdlun.inc'

C... ugly, but is fortran . . .
        Save            firsttime
        Logical         firsttime
        Data            firsttime   /.true./

C...EXTERNAL statement links PYDATA on most machines.
      EXTERNAL PYDATA

C...The event record.
      INTEGER N,NPAD,K,MSTP,MSTI,MSEL,MSELPD,MSUB,KFIN,MINT
      DOUBLE PRECISION P,V,PARP,PARI,CKIN,VINT
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
      COMMON/PYINT1/MINT(400),VINT(400)
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
      INTEGER LNHIN,LOUT,MSCAL,IEVNT,ICKKW,ISCALE
      COMMON/UPPRIV/LNHIN,LOUT,MSCAL,IEVNT,ICKKW,ISCALE

C...Commonblock to transfer event-by-event matching info
      INTEGER NLJETS,IEXC,Ifile
      DOUBLE PRECISION PTCLUS
      COMMON/MEMAEV/PTCLUS(20),NLJETS,IEXC,Ifile
      DATA Ifile/0/
C...Inputs for the matching algorithm
      double precision etcjet,rclmax,etaclmax,qcut,clfact
      integer maxjets,minjets,iexcfile,ktsche
      common/MEMAIN/etcjet,rclmax,etaclmax,qcut,clfact,
     $   maxjets,minjets,iexcfile,ktsche

C...Inputs for jet clustering and Pythia run
C...IJET=1(getjet)/2(ktclus)
C...NITER=-1 means all available events
C...Inputs for jet clustering and Pythia run
      INTEGER NITER,NFILES
      DOUBLE PRECISION ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX
      COMMON/JETPAR/ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX,
     $     NITER,NFILES
      DATA ETCLUS,ETMIN,RMAX,ETAMAX,ETAJETMAX/10d0,10d0,0.7d0,2.5d0,2d0/
      DATA NITER,NFILES/-1,0/
C...External functions
      DOUBLE PRECISION PYANGL,PSERAP
      EXTERNAL PYP,PYANGL,PSERAP

C...HBOOK parameters
c      character*72 FHBOOK
c      INTEGER NWPAWC,lrec,istat,icycle
c      PARAMETER (NWPAWC = 500000)
c      REAL HMEMOR
c      COMMON /PAWC/ HMEMOR(NWPAWC)
c      REAL HSUM
      INTEGER nvarev,nvar2
      PARAMETER (nvarev=57,nvar2=6)
      CHARACTER*8 htit(nvarev),htit2(nvar2)
      DATA htit/'Npart','Qjet1','Qjet2','Qjet3','Qjet4',
     $   'Ptcjet1','Ptcjet2','Ptcjet3','Ptcjet4',
     $   'Etacjet1','Etacjet2','Etacjet3','Etacjet4',
     $   'Phicjet1','Phicjet2','Phicjet3','Phicjet4',
     $   'Ptjet1','Ptjet2','Ptjet3','Ptjet4',
     $   'Etajet1','Etajet2','Etajet3','Etajet4',
     $   'Phijet1','Phijet2','Phijet3','Phijet4',
     $   'Idres1','Ptres1','Etares1','Phires1',
     $   'Idres2','Ptres2','Etares2','Phires2',
     $   'Ptlep1','Etmiss','Htjets',
     $   'Ptb','Etab','Ptbbar','Etabbar','Ptbj','Etabj',
     $   'Qpar1','Qpar2','Qpar3','Qpar4',
     $   'Ptpar1','Ptpar2','Ptpar3','Ptpar4',
     $   'Ncjets','Njets','Nfile'/
      DATA htit2/'Npart','Qjet1','Qjet2','Qjet3','Qjet4','Nfile'/
      REAL*4 varev(nvarev)
      COMMON/HISTDAT/varev
      CHARACTER*8 ftit(1)
      DATA ftit/'Xsecfact'/
      REAL*4 fvar(1)

C...Variables for the kT-clustering
      INTEGER NMAX
      PARAMETER (NMAX=512)
      INTEGER NN,IHEP,I,NSUB,JET(NMAX),NCJET
      DOUBLE PRECISION ECUT,YCUT,RAD
      DOUBLE PRECISION PCJET(4,NMAX),Y(NMAX),PP(4,NMAX),ETJETC(NMAX)
C...HEPEVT commonblock.
      INTEGER NMXHEP,NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      DOUBLE PRECISION PHEP,VHEP
      PARAMETER (NMXHEP=4000)
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      SAVE /HEPEVT/
C...GETJET commonblocks
      INTEGER MNCY,MNCPHI,NCY,NCPHI,NJMAX,JETNO,NJJET
      DOUBLE PRECISION YCMIN,YCMAX,DELY,DELPHI,ET,STHCAL,CTHCAL,CPHCAL,
     &  SPHCAL,PJJET,ETJETJ
      PARAMETER (MNCY=200)
      PARAMETER (MNCPHI=200)
      COMMON/CALOR/DELY,DELPHI,ET(MNCY,MNCPHI),
     $CTHCAL(MNCY),STHCAL(MNCY),CPHCAL(MNCPHI),SPHCAL(MNCPHI),
     $YCMIN,YCMAX,NCY,NCPHI
      DATA YCMIN,YCMAX/-2.5d0,2.5d0/
      DATA NCY,NCPHI/50,60/
      PARAMETER (NJMAX=500)
      COMMON/GETCOM/PJJET(4,NJMAX),ETJETJ(NJMAX),JETNO(MNCY,MNCPHI),
     $NJJET

C   Local variables
      INTEGER IEV,istrstd,lok,IEXCSAVE,Ifstart
      CHARACTER*5 CGIVE,BEAM(2)
      CHARACTER*30 CGIVE0
      CHARACTER*80 pythia_card,output_file,input_file
      DOUBLE PRECISION Ptres1,Etares1,Phires1,Ptres2,Etares2,Phires2
      DOUBLE PRECISION Ptlep1,Etmiss,PT,AETA,PTJET(NMAX),ETA
      DOUBLE PRECISION Pxmiss,Pymiss,ETACJET(NJMAX),ETAJJET(NJMAX)
      DOUBLE PRECISION Ptb,Etab,Phib,Ptbbar,Etabbar,Phibbar,Ptbj,Etabj
      INTEGER J,KSORTJ(NMAX),KSORTC(NMAX),ib,ibbar,idres1,idres2,IMOT
      LOGICAL internal
      DATA internal/.false./

C...Set pi0 stable to trim event listings.
C      CALL PYGIVE('MDCY(C111,1)=0')
C      CALL PYGIVE('MDCY(C211,1)=0')

C...Set tau stable to use tauola for decay
C      CALL PYGIVE('MDCY(C15,1)=0')

C...
      if(firsttime) then
        OPEN (10, FILE='events.tree')
        IEV=0
        write(*,*)"Called EVENTTREE SUBROUTINE"
C...Print root tree event file header
       WRITE(CGIVE0,'(a,I2,a)') '('//char(39)//'# '//char(39)//',',
     $  nvarev-1,'(a,'//char(39)//':'//char(39)//'),a)'
       WRITE(10,'(a)') '# File with ntuple events with the variables:'
       WRITE(10,CGIVE0) (htit(I)(1:len_trim(htit(I))),I=1,nvarev)
        firsttime=.false.
      endif

C...Event loop
        IEV=IEV+1

C...If event generation failed, quit loop
        IF(MSTI(51).EQ.1) THEN
          GOTO 135 
        ENDIF

C...Perform event analysis to fill hbook ntuple
        Idres1=0
        Ptres1=-1
        Etares1=-20
        Phires1=-1
        Idres2=0
        Ptres2=-1
        Etares2=-20
        Phires2=-1
        Ptlep1=-1
        Pxmiss=0
        Pymiss=0
        Ptb=-1
        Etab=-20
        Phib=-1
        Ptbbar=-1
        Etabbar=-20
        Phibbar=-1
        Ptbj=-1
        Etabj=-20
        ib=0
        ibbar=0
        DO I=7,N
          IF(K(I,1).GT.20.AND.K(I,3).LT.7.AND.
     $       ((IABS(K(I,2)).GT.5.AND.IABS(K(I,2)).LT.11).OR.
     $       (IABS(K(I,2)).GT.21.AND.IABS(K(I,2)).LT.90).OR.
     $       IABS(K(I,2)).GT.100000))THEN
            IF(Idres1.EQ.0)THEN
              Idres1=K(I,2)
              Ptres1=sqrt(P(I,1)**2+P(I,2)**2)
              IF(Ptres1.gt.0d0.AND.P(I,3).NE.0d0)THEN
                Etares1=SIGN(LOG(MIN((SQRT(Ptres1**2+P(I,3)**2)+
     $             ABS(P(I,3)))/Ptres1,1d5)),P(I,3))
              ENDIF
              Phires1=PYANGL(P(I,1),P(I,2))
            ELSE IF(Idres2.EQ.0)THEN
              Idres2=K(I,2)
              Ptres2=sqrt(P(I,1)**2+P(I,2)**2)
              IF(Ptres2.gt.0d0.AND.P(I,3).NE.0d0)THEN
                Etares2=SIGN(LOG(MIN((SQRT(Ptres2**2+P(I,3)**2)+
     $             ABS(P(I,3)))/Ptres2,1d5)),P(I,3))
              ENDIF
              Phires2=PYANGL(P(I,1),P(I,2))
            ENDIF
          ELSE IF(IABS(K(I,2)).EQ.11.AND.IABS(K(K(I,3),2)).EQ.24)THEN
            Ptlep1=sqrt(P(I,1)**2+P(I,2)**2)
          ELSE IF(K(I,1).LT.10.AND.(IABS(K(I,2)).EQ.12.OR.
     $         IABS(K(I,2)).EQ.14.OR.IABS(K(I,2)).EQ.16))THEN
            Pxmiss=Pxmiss+P(I,1)
            Pymiss=Pymiss+P(I,2)
          ELSE IF(K(I,2).EQ.5.AND.ib.eq.0.AND.K(I,3).LT.7)THEN
            Ptb=sqrt(P(I,1)**2+P(I,2)**2)
            Etab=SIGN(LOG(MIN((SQRT(Ptb**2+P(I,3)**2)+
     $       ABS(P(I,3)))/Ptb,1d5)),P(I,3))
            Phib=PYANGL(P(I,1),P(I,2))
            ib=I
          ELSE IF(K(I,2).EQ.-5.AND.ibbar.eq.0.AND.K(I,3).LT.7)THEN
            Ptbbar=sqrt(P(I,1)**2+P(I,2)**2)
            Etabbar=SIGN(LOG(MIN((SQRT(Ptbbar**2+P(I,3)**2)+
     $       ABS(P(I,3)))/Ptbbar,1d5)),P(I,3))
            Phibbar=PYANGL(P(I,1),P(I,2))
            ibbar=I
          ENDIF
        ENDDO

        Etmiss=sqrt(Pxmiss**2+Pymiss**2)

C---FIND FINAL STATE COLOURED PARTICLES
        NN=0
C---CLUSTER THE EVENT (DEFINING ECUT AS 1, SO Y IS MEASURED IN GEV**2)
C   USING THE PT-SCHEME
        ECUT=1
        YCUT=ETCLUS**2
        NCJET=0
        NJJET=0
        NHEP=N
        DO I=1,N
c            DO J=1,5
c              PHEP(J,I)=P(I,J)
c            ENDDO
C...Make only interesting jet particles active
          IF(K(I,1).LT.10.AND.
     $          (ABS(K(I,2)).LT.6 .OR. K(I,2).EQ.21 .OR. K(I,2).EQ.22
     $          .OR.(ABS(K(I,2)).GT.100.AND.ABS(K(I,2)).LT.10000)))THEN
             ISTHEP(I)=1
             GOTO 21
          ELSE
             ISTHEP(I)=2
 21       ENDIF
          
C...Prepare for kT clustering
          IF(ISTHEP(I).EQ.1)THEN
            PT=sqrt(P(I,1)**2+P(I,2)**2)
            AETA=ABS(LOG(MIN((SQRT(PT**2+P(I,3)**2)+
     $         ABS(P(I,3)))/PT,1d5)))
            IF(AETA.GT.ETAMAX) goto 22
            IF(PT.EQ.0) goto 22
            NN=NN+1
            IF (NN.GT.NMAX) then
              PRINT *, 'Too many particles for kt clustering, ',NN,
     $              ', skipping extra particles.'
              NN=NN-1
              GOTO 23
            endif
            DO J=1,4
              PP(J,NN)=P(I,J)
            ENDDO
          ENDIF
 22     ENDDO
 23     IF (NN.GT.1) then
C...Perform jet clustering with ktclus
          CALL KTCLUS(KTSCHE,PP,NN,ECUT,Y,*999)
          CALL KTRECO(MOD(KTSCHE,10),PP,NN,ECUT,YCUT,YCUT,
     $       PCJET,JET,NCJET,NSUB,*999)            
          DO I=1,NCJET
            ETJETC(I)=SQRT(PCJET(1,I)**2+PCJET(2,I)**2)
          ENDDO

        ELSE IF (NN.EQ.1) THEN
          Y(1)=PP(1,1)**2+PP(2,1)**2
          IF(Y(1).GT.YCUT)THEN
            NCJET=1
            DO I=1,4
              PCJET(I,1)=PP(I,1)
            ENDDO
          ENDIF
        ENDIF

        CALL ALPSOR(ETJETJ,NJJET,KSORTJ,2)
        CALL ALPSOR(ETJETC,NCJET,KSORTC,2)

        DO I=1,NJMAX
          IF(I.LE.NCJET) THEN
            ETACJET(I)=PSERAP(PCJET(1,KSORTC(NCJET+1-I)))
          ELSE
            ETACJET(I)=-10
          ENDIF
          IF(I.LE.NJJET) THEN
            ETAJJET(I)=PSERAP(PJJET(1,KSORTJ(NJJET+1-I)))
          ELSE
            ETAJJET(I)=-10
          ENDIF
        ENDDO

        DO I=1,nvarev
           if(i.lt.47.or.i.gt.54) varev(I)=-1
        ENDDO

C...Fill hbook ntuple with jet rates        
        varev(1)=NLJETS
        DO I=1,MIN(NN,4)
          varev(1+I)=SQRT(Y(I))
        ENDDO
        DO I=1,MIN(NCJET,4)
          IF(ABS(ETACJET(I)).LE.ETAJETMAX)
     $       varev(5+I)=ETJETC(KSORTC(NCJET+1-I))
        ENDDO
        DO I=1,4
          IF(ABS(ETACJET(I)).LE.ETAJETMAX)THEN
            varev(9+I)=ETACJET(I)
          ELSE
            varev(9+I)=-10
          ENDIF
        ENDDO
        DO I=1,MIN(NCJET,4)
          IF(ABS(ETACJET(I)).LE.ETAJETMAX)
     $       varev(13+I)=PYANGL(PCJET(1,KSORTC(NCJET+1-I)),
     $       PCJET(2,KSORTC(NCJET+1-I)))
        ENDDO
        DO I=1,MIN(NJJET,4)
          IF(ABS(ETAJJET(I)).LE.ETAJETMAX)
     $       varev(17+I)=ETJETJ(KSORTJ(NJJET+1-I))
        ENDDO
        DO I=1,4
          IF(ABS(ETAJJET(I)).LE.ETAJETMAX)THEN
            varev(21+I)=ETAJJET(I)
          ELSE
            varev(21+I)=-10
          ENDIF
        ENDDO
        DO I=1,MIN(NJJET,4)
          IF(ABS(ETAJJET(I)).LE.ETAJETMAX)
     $    varev(25+I)=PYANGL(PJJET(1,KSORTJ(NJJET+1-I)),
     $       PJJET(2,KSORTJ(NJJET+1-I)))
        ENDDO
        varev(30)=Idres1
        varev(31)=Ptres1
        varev(32)=Etares1
        varev(33)=Phires1
        varev(34)=Idres2
        varev(35)=Ptres2
        varev(36)=Etares2
        varev(37)=Phires2
        varev(38)=Ptlep1
        varev(39)=Etmiss
        varev(40)=0
        DO I=1,NJJET
          IF(ABS(ETAJJET(I)).LE.ETAJETMAX)
     $       varev(40)=varev(40)+ETJETJ(I)
        ENDDO
        if(abs(Etab).lt.ETAMAX)then
          varev(41)=Ptb
          varev(42)=Etab
        else
          varev(41)=-1
          varev(42)=-20
        endif
        if(abs(Etabbar).lt.ETAMAX)then
          varev(43)=Ptbbar
          varev(44)=Etabbar
        else
          varev(43)=-1
          varev(44)=-20
        endif

c     Check if there is a b going in the direction of one of the jets
        IF(ib.NE.0.OR.ibbar.NE.0)THEN
          DO I=1,NJJET
            IF(ib.NE.0.AND.SQRT((PSERAP(PJJET(1,I))-Etab)**2+
     $         (PYANGL(PJJET(1,I),PJJET(2,I))-Phib)**2).LT.RMAX .AND.
     $         SQRT(PJJET(1,I)**2+PJJET(2,I)**2).GT.Ptb*0.8) THEN
              Ptbj=SQRT(PJJET(1,I)**2+PJJET(2,I)**2)
              Etabj=PSERAP(PJJET(1,I))
            ENDIF
            IF(ibbar.NE.0.AND.SQRT((PSERAP(PJJET(1,I))-Etabbar)**2+
     $         (PYANGL(PJJET(1,I),PJJET(2,I))-Phibbar)**2).LT.RMAX.AND.
     $         SQRT(PJJET(1,I)**2+PJJET(2,I)**2).GT.Ptb*0.8) THEN
              Ptbj=SQRT(PJJET(1,I)**2+PJJET(2,I)**2)
              Etabj=PSERAP(PJJET(1,I))
            ENDIF
          ENDDO
        ENDIF
        varev(45)=Ptbj
        varev(46)=Etabj
        varev(55)=NCJET
        varev(56)=NJJET
        varev(57)=Ifile

c        varev(13)=LOG10(PTPART(1)) ! i.e. PTGEN from pyshow
c        varev(14)=LOG10(PTMAX)  !LOG10(SQRT(VINT(56)))

c        CALL HFN(10,varev)        
C...Write event into ntuple 
        WRITE(10,4001) (varev(I),I=1,nvarev)

C...Print first 10 events

C 125    IF(IEV.LE.10) THEN
C          PRINT *
C          PRINT *,'Event number: ',IEV
C          CALL PYLIST(7)
C          CALL PYLIST(2)
C          IF(ickkw.gt.0) PRINT *,NLJETS,'-jet event (parton level)'
C          PRINT *,'SCALUP: ',SCALUP
C          IF(NN.GT.0) PRINT *,'Jet clustering values:'
C          PRINT *,(SQRT(Y(J)),J=1,MIN(4,NN))
C          PRINT *,'Jet momenta (ktclus):'
C          DO I=1,NCJET
C            PRINT *,(PCJET(J,I),J=1,4)
C          ENDDO
C          PRINT *,'Jet Pts:',(ETJETC(KSORTC(NCJET+1-I)),I=1,NCJET)
C          PRINT *,'Jet momenta (getjet):'
C          DO I=1,NJJET
C            PRINT *,(PJJET(J,KSORTJ(NJJET+1-I)),J=1,4)
C          ENDDO
C          PRINT *,'Jet Pts:',(ETJETJ(KSORTJ(NJJET+1-I)),I=1,NJJET)
Cc          call heplst(1)
C          CALL FLUSH()
C        ENDIF

        IF(MOD(IEV,10000).EQ.0)THEN
          CALL PYSTAT(1)
C...Save hbook histograms and ntuples to file
c          CALL HROUT(0,icycle,'T')
c          PRINT *, 'Histograms saved to ',FHBOOK
          CALL FLUSH()
        ENDIF

 130  CONTINUE

 135  CONTINUE

C...Save conversion factor per event (total xsec/number of events)
C...(for unweighted events)
      fvar(1)=PARI(2)*1d9
c      CALL HFN(20,fvar)
C...Write into ntuple 
C      WRITE(20,4001) fvar(1)      

      RETURN
 4000 FORMAT(//A15,A1)
 4001 FORMAT(60E15.6)
 999  WRITE(*,*) 'Error in one of the KTCLUS routines'
      STOP
 90   WRITE(*,*) 'Error: Could not open MadEvent event file'
      WRITE(*,*) 'Quitting...'
      END
      
C************************************************
C      subroutine remove_comments(line,len)
C
C      implicit none
C      character*(*) line
C      integer i,len
C
C      logical comment
C
C      comment = .false.
C      if(line(1:1).eq.'#') comment = .true.
C      do i=1,len
C        if(line(i:i).eq.'!') comment = .true.
C        if(comment) line(i:i) = ' ' 
C      enddo
C
C      return
C      end

