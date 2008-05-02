C-------------------------------------------------------------
	Real*8 Function generate1(
     &     FName,pythiaFName,lumi,collider,
     &     NMax)
C
C
C    Main Pythia driver, called from Quaero
C    Input: File pythiaFName contains pythia commands
C            Lumi is the luminosity to generate
C    Output: File FName will contain the generated events in stdhep format
C    Return value: On success, the cross section in pb, corresponding to
C                  the event sample in the output file;
C                  On error, negative error code
C    Author: Bruce Knuteson, July 2003
C
C-------------------------------------------------------------
C
C...All real arithmetic in double precision.
      IMPLICIT DOUBLE PRECISION(A-H, O-Z)
c	implicit none
C
C...Input variables --------------------------------->
C
	Character*(*) FName
	Character*(*) pythiaFName
	character*(*) collider
	Real*8 lumi    ! units are pb^-1
C
C...PYTHIA Common blocks and parameters ------------->
C
C      INTEGER PYK,PYCHGE,PYCOMP
      PARAMETER (KSUSY1=1000000,KSUSY2=2000000,KEXCIT=4000000)
C...Commonblocks.
C...The event record.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
C...Parameters.
      COMMON/PYDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
C...Particle properties + some flavour parameters.
      COMMON/PYDAT2/KCHG(500,4),PMAS(500,4),PARF(2000),VCKM(4,4)
C...Decay information.
c      COMMON/PYDAT3/MDCY(500,3),MDME(4000,2),BRAT(4000),KFDP(4000,5)
      COMMON/PYDAT3/MDCY(500,3),MDME(8000,2),BRAT(8000),KFDP(8000,5)
C...Selection of hard scattering subprocesses.
      COMMON/PYSUBS/MSEL,MSELPD,MSUB(500),KFIN(2,-40:40),CKIN(200)
C...Parameters. 
      COMMON/PYPARS/MSTP(200),PARP(200),MSTI(200),PARI(200)
C...Supersymmetry parameters.
      COMMON/PYMSSM/IMSS(0:99),RMSS(0:99)
C...Cross-section info.
      COMMON/PYINT5/NGENPD,NGEN(0:500,3),XSEC(0:500,3)
C...Resonance decays
      COMMON/PYINT4/MWID(500),WIDS(500,5)
      External PYDATA
c...Random number
	COMMON/PYDATR/MRPY(6),RRPY(100)
C...HEPEVT commonblock.
      integer nmxhep
      PARAMETER (NMXHEP=4000)
      DOUBLE PRECISION PHEP,VHEP
      integer NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
C---------------------------------------------------->
C...Local arrays, character variables and data.
C
	logical writeOutStdHep, writeOutHepEvt
	parameter (writeOutStdHep = .false.)
	parameter (writeOutHepEvt = .true.)
	real kfactor, xsecGen, lumiGen   ! units are pb, pb^-1
	integer numGen
	logical runII, passes, unlimitedEvents
	Integer NEvents, I, J, M, L, PID
	Integer IEv, NE, NM, NG, NP, NJet
	Real PTMin, ETMin, Tht, Eta, Phi, ThtJ, EtaJ, PhiJ, r
	Real PEl(3,100), PMu(3,100), PPh(3,100), Jet(4,100), PMet(2)
c	real e, costh, px, py, pz, sumpt
c	Integer typeEl(100), typeMu(100)
	logical killThisElectron
	real uncl_x, uncl_y, DeltaR, costhmax
c	Parameter (NMax = 10000) ! Maximum number of tried events
	Parameter (PTMin = 15.)     ! Minimum trigger pT
	Parameter (RISO = 0.5)	! Minimum separation from jet for electron,
				! photon, or muon to be considered isolated
	parameter (costhmax=0.9)

	real*8 nominalecm
	integer iecm, necm
	real*8 ecm(100)

	real*8 nominalecmLEP2
	integer iecmLEP2
	data nominalecmLEP2 /207/
	parameter (NecmLEP2 = 8)
	real*8 ecmLEP2(NecmLEP2)
	data ecmLEP2 /183, 189, 192, 196, 200, 202, 205, 207/

	real*8 nominalecmHERA
	data nominalecmHERA /320/
	parameter (NecmHERA = 2)
	real*8 ecmHERA(NecmHERA)
	data ecmHERA /301,319/

	double precision nominalecmTEV1
	data nominalecmTEV1 /1800/
	parameter (NecmTEV1 = 1)
	double precision ecmTEV1(NecmTEV1)
	data ecmTEV1 /1800/

	real*8 nominalecmTEV2
	data nominalecmTEV2 /1960/
	parameter (NecmTEV2 = 1)
	real*8 ecmTEV2(NecmTEV2)
	data ecmTEV2 /1960/

	real*8 nominalecmLHC
	data nominalecmLHC /14000/
	parameter (NecmLHC = 1)
	real*8 ecmLHC(NecmLHC)
	data ecmLHC /14000/

	character*999 FNAMEifile
	character*5 cifile
	integer lok
	integer nevifile,ifile,nfile
	integer totalEventsPassed, totalEventsTried
	parameter (maxNumberOfEventsPerFile=20000)
c	character*5 cgive
c	character*30 cgive0

	character*999 fPythiaFName
	character*999 command
	integer eof1
        integer MSTP111

C---------------------------------------------------->
	NEvents = 0
c	Open(Unit=98,File=FName,Status='Unknown',
c     &     Form='formatted',Err=120)
C---------------------------------------------------->
C
C...Intialize certain PYTHIA parameters, such as collider type & energy 
C

	call pygive('mstp(61)=')  ! initial state radiation on
	call pygive('mstp(71)=')  ! final state radiation on
	call pygive('mstp(81)=')  ! multiple interactions off

c bktemp -- following line
	MSTP(111)=1		! fragmentation and decay [0]-off
	mstp(127)=1		! possibility to continue run even if no requested processes 
				! have non-vanishing cross sections
	mstp(171)=0		! not running with varied energies
c
c	MSTP(125)=2 ! prints out more information
C
C...Select which processes to generate:
C
      MSEL=0 ! generate only the processes selected below
	
C
	xsecGen = -1
	numGen = -1
	kfactor = 1.
	lumiGen = lumi
	unlimitedEvents = .false.

	fpythiaFName = pythiaFName(1:len_trim(pythiaFName))
	open(Unit=1, FILE=fpythiaFName, STATUS='Unknown')
	eof1=0
	Do while(eof1.eq.0)
	   read(1,'(A)',iostat=eof1) command	
	   if (eof1.eq.0) then
	      if (len_trim(command).gt.4) then
		 if (command(1:5).eq."xsec=") then
		    read(command(6:len_trim(command)),*) xsecGen
		 elseif (command(1:8).eq."kfactor=") then
		    read(command(9:len_trim(command)),*) kfactor
		 elseif (command(1:9).eq."unlimited") then
		    unlimitedEvents = .true.
		 elseif (command(1:8).eq."suspect=") then
		    write(*,*) 'implement suspect wrapper  in Pythia'
CC		    call suspectwrapper(command(9:len(command)))
		 else		 
		    call pygive(command(1:len_trim(command)))
		 endif
	      endif
	   end if
	End Do
	close(1)

	if((lumiGen.lt.0).and.(numGen.lt.0)) goto 120

	write(*,*) collider
	if((len_trim(collider).ge.4).and.
     &          (collider(1:4).eq."hera")) then	   
	   necm = 1
	   p(1,1)=0.0
	   p(1,2)=0.0
	   p(1,3)=-27.5
	   p(2,1)=0.0
	   p(2,2)=0.0
	   if(collider(1:len_trim(collider)).eq."hera+820") then
	      nominalecm = 301
	      ecm(1) = 301
	      p(2,3)=820.0
	      call pyinit('3mom','e+','p',0d0)
	   else if(collider(1:len_trim(collider)).eq."hera+920") then
	      nominalecm = 319
	      ecm(1) = 319
	      p(2,3)=920.0
	      call pyinit('3mom','e+','p',0d0)
	   else if(collider(1:len_trim(collider)).eq."hera-920") then
	      nominalecm = 319
	      ecm(1) = 319
	      p(2,3)=920.0
	      call pyinit('3mom','e-','p',0d0)
	   else
	      write(*,*) 'HERA collider ',collider,' not understood.'
	      write(*,*) 'Aborting.'
	   endif
	else
	   if(collider(1:len_trim(collider)).eq."tev2") then
	      CALL PYINIT('CMS','p','pbar',nominalecmTEV2)
	      nominalecm = nominalecmTEV2
	      necm = necmTEV2
	      do i=1,necm
		 ecm(i) = ecmTEV2(i)
	      enddo
	   else if(collider(1:len_trim(collider)).eq."tev1") then
	      CALL PYINIT('CMS','p','pbar',nominalecmTEV1)
	      nominalecm = nominalecmTEV1
	      necm = necmTEV1
	      do i=1,necm
		 ecm(i) = ecmTEV1(i)
	      enddo
	   else if(collider(1:len_trim(collider)).eq."lep2") then
	      mstp(171)=1	! run with varied energies
	      mstp(172)=2	! we are going to generate a fixed luminosity at each center of mass energy
	      CALL PYINIT('CMS','e-','e+',nominalecmLEP2)
	      nominalecm = nominalecmLEP2
	      necm = necmLEP2
	      do i=1,necm
		 ecm(i) = ecmLEP2(i)
	      enddo
	   else if(collider(1:len_trim(collider)).eq."lhc") then
	      CALL PYINIT('CMS','p','p',nominalecmLHC)
	      nominalecm = nominalecmLHC
	      necm = necmLHC
	      do i=1,necm
		 ecm(i) = ecmLHC(i)
	      enddo
	   endif
	endif

	if(msti(53).eq.1) then
	   generate1 = 0.
	   return
	endif

c       the next lines prints out oodles of parameter information
c	call pystat(2)

C       Compute NOut -- number of events to generate
	if(xsecGen.lt.0) then
	   i=0
	   iecm = 0
           MSTP111=MSTP(111)
           call pygive('mstp(111)=-1')
	   do while (i.le.min(10000,NMax))
	      iecm = iecm + 1
	      if (iecm .gt. Necm) iecm=1
	      if (necm .gt. 1) parp(171)=ecm(iecm)/nominalecm ! change beam energy
	      call pyevnt	! generate Pythia event
	      if ((necm.eq.1).or.(msti(61).ne.1))  
     &	           i = i + 1
c	      call pyevnt	! generate Pythia event
	   enddo
	   call pystat(1)
           if(MSTP111.EQ.1) THEN
              call pygive('mstp(111)=1')
           elseif(MSTP111.EQ.0) THEN
              call pygive('mstp(111)=0')
           endif
	   xsecGen = xsec(0,3)*1d9*kfactor ! units are pb
c	   NOut = idint(xsec(0,3)*kfactor*1d9*lumiGen)
c	   if(NOut.ge.10000) NOut=10000
	endif
	
	if(xsecGen.le.0) goto 120
	
	if(lumiGen.lt.0) lumiGen = numGen/xsecGen
	if(numGen.lt.0) numGen = lumiGen*xsecGen
	
	if(((numGen.lt.0).or.(numGen.gt.NMax)).and.
     &       (.not.unlimitedEvents)) then
	   write(*,*) 'Generating maximum number of events allowed = ',NMax
	   numGen = NMax
	   lumiGen = numGen/xsecGen
c	   generate1 = -xsecGen
c	   write(*,*) 'Too many events requested'
c	   return
	endif

	write(*,*) 'Generating ',numGen,' events...', lumiGen,' pb^-1'
	
	NOut = numGen
	lumi = lumiGen
	
	nfile = NOut/maxNumberOfEventsPerFile+1
	
	write(*,*) ' nfile = ',nfile
	
	totalEventsPassed=0
	totalEventsTried=0

      do ifile = 1,nfile
C.....Opening stdhep file for writing
         WRITE(cifile,*) ifile
         if(ifile.lt.nfile) then
            nevifile = maxNumberOfEventsPerFile
         else
            nevifile = mod(NOut,maxNumberOfEventsPerFile)
         endif
	 if(nfile.eq.1) then
	    fnameifile = fname(1:len_trim(fname))
	 else	 
	    fnameifile = 
     &   	 fname(1:len_trim(fname)) // "_" // cifile(2:5)
	 endif
        print*,fnameifile(1:len_trim(fnameifile))

	if (writeOutStdHep) then
	  lok=-1
cc	   call stdxwinit(fnameifile,'signal',nevifile,istr,lok) ! open output stream
	   if(lok.ne.0) write(lnhout,*) ' Problem opening file '
cc	   call stdflpyxsec(nevifile) ! get run information from Pythia
cc	   call stdxwrt(100,istr,lok) ! write begin run record	   
	   if(lok.ne.0) write(lnhout,*) ' Problem stdhep begin run record'

	endif
	if(writeOutHepEvt) then
	   Open(Unit=90,File=fnameifile,Status='unknown')
	   write(90,*) NOut
	endif
	   
C
C...  Main event loop
C

	NEvents = 0
	IEv = 0
	iecm = 0
	Do While (NEvents .lt. nevifile)
	   iecm = iecm + 1
	   if (iecm .gt. Necm) iecm=1
	   if (necm.gt.1) parp(171)=ecm(iecm)/nominalecm ! change beam energy
	   call pyevnt		! generate Pythia event
	   if((necm.eq.1).or.(msti(61).ne.1)) then
	      IEv = IEv + 1

c       Check to see whether the event is worth bothering with
	      
	      passes = .true.
	      if (lumi.lt.0) then
		 passes = .false.
		 do j=1,n
		    if(k(j,1).eq.21) then
		       pt = sqrt(p(j,1)**2+p(j,2)**2)
		       pid = abs(k(j,2))
		       if(pt.gt.ptmin) then
			  passes = .true.
		       endif
		    endif
		 enddo
		 call PJets(.true.,NP,NJet,Jet)
		 do j=1,NJet
		    if(sqrt(jet(1,j)**2+jet(2,j)**2).gt.ptmin) then
		       passes = .true.
		    endif
		 enddo
	      endif
	      if (.not.passes) goto 103

c		 call pylist(1) ! prints out the event 

	      NEvents = NEvents + 1
	      if(mod(NEvents,1000).eq.0) 
     &  	write(*,*) 'working on event # ',NEvents

	      if (writeOutStdHep) then
CC		 call lunhep(1)	! fill hepevt common block
   	         call pyhepc(1)	! fill hepevt common block
cc		 call stdxwrt(1,istr,lok) ! write event record
	      endif
	      
	      if(writeOutHepEvt) then
		 call pyhepc(1)	! fill hepevt common block
		 write(90,*) NHEP
		 DO ii=1,NHEP
		    write (90,*) ISTHEP(ii),IDHEP(ii),
     & 	          (JMOHEP(J,ii),J=1,2), (JDAHEP(L,ii),L=1,2)
		    write (90,*) (PHEP(J,ii),J=1,5)
		    write (90,*) (VHEP(L,ii),L=1,5)
		 END DO         	     
	      endif
	      
 103	      continue
	   endif
	End Do			!Nevents
	
c	CALL PYSTAT(1)
C       Fill Stdhep common block 1 with run information

	if(writeOutStdHep) then
cc         call stdflpyxsec(nevifile) ! get run formation from Pythia
cc         call stdxwrt(200,istr,lok) ! write end run record
cc         call stdxend(istr)   ! close output stream
	endif

	if(writeOutHepEvt) then
	   Close (Unit=90)
	endif

	totalEventsPassed = totalEventsPassed + Nevents
	totalEventsTried = totalEventsTried + IEv

	enddo			! ifile

C
C...  Print a summary of cross-sections.
C
  110 Continue
	call pystat(1)
	if (totalEventsPassed.eq.0) then
	   generate1 = 0.
	else
	   generate1 = xsecGen*
     &        totalEventsPassed/totalEventsTried
	endif
	write(*,*) totalEventsPassed,
     &      ' events written in file out of',
     &      totalEventsTried,' tried'
 	write(*,*) 'Cross section of produced events:',generate1,' pb'
c	if(NOut.eq.NMax) generate1 = -generate1	
 
	Return
 
  120	generate1 = -999.
	Return
      END

C---------------------------------------------------------------------
C  The following subroutines are historical, and currently not used
C---------------------------------------------------------------------

	Subroutine PJets(FSR,NP,NJ,Jet)
	Implicit None
C	INTEGER PYK,PYCHGE,PYCOMP
C...Commonblocks.
C...The event record.
      COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
	COMMON/PJETP/ IPrint
	Real PJET(8,100), Jet(4,100), PTJ(100), RSep, M13, M23
	Real EtaMax, ETMin, DR0, DR, DeltaR, Mass2, Tmp
	Double Precision P, V
	Integer NP,J,NJ,N,NPad,K,L,IJ(100),Ind(100),I1,I2,IPrint,IT
	Logical FSR
      External PYDATA
	Data RSep /0.5/, EtaMax /3.0/, ETMin /7./
	Save RSep, EtaMax, ETMin
C
C...New initial seed algorithm
C
C       Call VZero(Jet,400)
        DO J=1,4
           DO L=1,100
              JET(J,L)=0D0
           ENDDO
        ENDDO
	NP = 0
	IT = 0
	Do 115 J = 1,N
	  If (K(J,1) .eq. 21) Go To 115
	  If (K(J,2) .eq. 21 .or. abs(K(J,2)) .le. 5) Then
	    NP = NP + 1
	    If (NP .gt. 100) Then
	      NP = 100
		  write(*,*) 'PJET Warning: Jet array size exceeded!'
	    End If
		Do L = 1,4
		  PJET(L,NP) = P(J,L)
	    End Do
	    PJET(5,NP) = SQRT(PJET(1,NP)**2 + PJET(2,NP)**2)
		If (PJET(5,NP) .gt. 0) Then
	      Call Angles(PJET(1,NP),PJET(5,NP),
     &				   PJET(6,NP),PJet(7,NP),PJet(8,NP))		! Theta, Phi, Eta
		Else
		  NP = NP -1
	    End If
	  End If
 115	Continue
	NJ = -1
	Do While (NJ .ne. 0)
	  NJ = 0
	  DR0 = 999.
	  Do J = 1,NP-1
	    Do L = J+1,NP
	      DR = DeltaR(PJet(7,J),PJet(8,J),PJet(7,L),PJet(8,L))
	      If (DR .le. DR0) Then
			DR0 = DR
			I1 = J
			I2 = L
	      End If
	     End Do
	  End Do
	  If (DR0 .le. RSep) Then
	    NJ = NJ + 1
	    Do J = 1,4
	      PJet(J,I1) = PJet(J,I1) + PJet(J,I2)
	    End Do
	    PJET(5,I1) = SQRT(PJET(1,I1)**2 + PJET(2,I1)**2)
	    Call Angles(PJet(1,I1),PJet(5,I1),
     &		    	PJet(6,I1),PJet(7,I1),PJet(8,I1))		! Theta, Phi, Eta
		Do J = I2,NP-1
		  Do L = 1,8
			PJet(L,J) = PJet(L,J+1)
	      End Do
		End Do
		NP = NP - 1
	  End If
	End Do
C
	NJ = 0
	Do J = 1,NP
	  If (PJet(5,J) .ge. ETMin .and. Abs(PJet(8,J)) .lt. EtaMax) Then
	    NJ = NJ + 1
	    PTJ(NJ) = PJet(5,J)
		IJ(NJ) = J
	  End If
	End Do
CCall SortZV(PTJ,Ind,NJ,1,1,0)
        print*, 'SORTZV is not implemented; code should not be used'
C
	Do J = 1,NJ
	  Do L = 1,4
	    Jet(L,J) = PJet(L,IJ(Ind(J)))
	  End Do
	End Do
C
C...  Simple FSR merging
C
	if(.false.) then		! don't do this
	If (NJ .ge. 3 .and. FSR) Then
	  M13 = Mass2(Jet(1,1),Jet(1,3))
	  M23 = Mass2(Jet(1,2),Jet(1,3))
	  If (Min(M13,M23) .lt. 90.) Then
	    If (M23 .lt. M13) Then
		  Do L = 1,4
	        Jet(L,2) = Jet(L,2) + Jet(L,3)
	      End Do
	    Else
		  Do L = 1,4
	        Jet(L,1) = Jet(L,1) + Jet(L,3)
	      End Do
	    End If
		Do J = 3,NJ-1
		  Do L = 1,4
		    Jet(L,J) = Jet(L,J+1)
	      End Do
	    End Do
		Do J = 1,4
	      Jet(J,NJ) = 0.
		End Do
	    NJ = NJ - 1
	  End If
	  If (Jet(1,1)**2+Jet(2,1)**2 .lt. Jet(1,2)**2+Jet(2,2)**2) Then
		Do L = 1,4
		  Tmp = Jet(L,1)
	      Jet(L,1) = Jet(L,2)
		  Jet(L,2) = Tmp
	    End Do
	  End If
	End If
	endif
C
	Return
	End
C---------------------------------------------------------->>>
C
C...  Swaps values of pT and 4-vectors for two particles
C
      Subroutine Swap4T(PT1,PT2,P1,P2)
	Real PT1, PT2, P1(4), P2(4)
C
      TMP = PT1
	PT1 = PT2
	PT2 = TMP
C
      Do I = 1,4
	  TMP = P1(I)
	  P1(I) = P2(I)
	  P2(I) = TMP
	End Do
C
	Return
	End
C---------------------------------------------------------->>>
C
C...  Returns angles Phi, Theta, as well as pseudorapidity
C...  for a particle with four-vector P and pT PT
C
      Subroutine Angles(P,PT,Tht,Phi,Eta)
      Real P(4), PT, Tht, Phi, Eta
C
      PT = sqrt(p(1)*p(1)+p(2)*p(2))
      Tht = ATAN2(PT,P(3))
      Phi = ATAN2(P(2),P(1))
      If (Tan(Tht/2.) .le. 0.) Then
		write(*,*) 'Error:',P,PT,Tht,Phi
		Eta = 0.
		Return
      End If
	Eta = -ALog(Tan(Tht/2.))
C
      Return
	End
C
C---------------------------------------------------------->>>
C
C...  Returns Delta R = sqrt(Delta(eta)**2 + Delta(Phi)**2)
C...  given Phi1,2, Theta1,2
C
      Real Function DeltaR(Phi1,Eta1,Phi2,Eta2)
      Real Phi1, Phi2, Eta1, Eta2, Pi, TwoPi
	Parameter (   Pi = 3.1415926536)
	Parameter (TwoPi = 6.2831853072)
C
      If (ABS(Phi1-Phi2) .le. Pi) Then
	  DeltaR = sqrt((Eta1-Eta2)**2 + (Phi1-Phi2)**2)
	Else
	  DeltaR = sqrt((Eta1-Eta2)**2 + (TwoPi-ABS(Phi1-Phi2))**2)
	End If
C
      Return
	End
C---------------------------------------------------------->>>
C
C...  Returns invariant mass of an object decaying into two
C...  particles with 4-vectors P1, P2
C
      Real Function Mass2(P1,P2)
      Real P1(4), P2(4)
C
      Mass2 = (P1(4)+P2(4)+P1(3)+P2(3))*(P1(4)+P2(4)-P1(3)-P2(3)) -
     &        (P1(2)+P2(2))**2 - (P1(1)+P2(1))**2
	If (Mass2 .lt. 0.) Then
		write(*,*) 'Mass2: ',P1,P2,Mass2
	End If
	Mass2 = sqrt(max(1.E-6,Mass2))
C
      Return
	End
C-------------------------------------------------------------




C-------------------------------------------------------------
	Real*8 Function generatehera(
     &     FName,pythiaFName,
     &     lumi1,lumi2,lumi3,
     &     xsec1, xsec2, xsec3,
     &     NMax )
	IMPLICIT DOUBLE PRECISION(A-H, O-Z)
	Character*(*) FName
	character*999 fname1, fname2, fname3
	Character*(*) pythiaFName
        character*999 collider1, collider2, collider3
	Real*8 lumi1, lumi2, lumi3
        Real*8 xsec1, xsec2, xsec3, generate1
	integer NMax
	
	fname1 = FName // "_1"
	fname2 = FName // "_2"
	fname3 = FName // "_3"
	collider1 = "hera+820"
	collider2 = "hera+920"
	collider3 = "hera-920"
	xsec1 = generate1(fname1,pythiaFName,lumi1,collider1,NMax)
	xsec2 = generate1(fname2,pythiaFName,lumi2,collider2,NMax)
	xsec3 = generate1(fname3,pythiaFName,lumi3,collider3,NMax)
	 
c H1 luminosities:
c  pe+ 301 GeV  36.4 pb^-1
c  pe- 319 GeV  13.8 pb^-1
c  pe+ 319 GeV  66.4 pb^-1

	generate_hera = 0

	return
	end

C-------------------------------------------------------------
	Real*8 Function generate(
     &     FName,pythiaFName,lumi,xsec,collider,
     &     NMax)
	IMPLICIT DOUBLE PRECISION(A-H, O-Z)
	Character*(*) FName
C	character*999 fname1, fname2, fname3
	Character*(*) pythiaFName
	character*(*) collider
	Real*8 lumi, generate1, xsec
	integer NMax
	

	if(collider(1:len_trim(collider)).eq."hera") then
	   write(*,*) 'This function does not do hera!'
	   stop
	else
	   xsec = generate1(FName,pythiaFName,lumi,collider,NMax)
	endif

	generate = 0

	return
	end


