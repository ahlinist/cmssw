c----------------------------------------------------------------------------c
C..FINDJETS V:1.01,   06/09/2006 by Serge Slabospitsky
C  call PYCELL routine from PYTHIA
C  IXPAR(5) = 0/1 NOT/CALL pycell 
      subroutine findjets()
      implicit none
      character*132                            CXpar
      integer         Ixpar
      double precision            RXpar
      common /EXGPAR/ IXpar(100), RXpar(100), CXpar(100)
      save /EXGPAR/
      integer Njets
C...HEPEVT commonblock.
      INTEGER NMXHEP
      PARAMETER (NMXHEP=4000)
      integer       NEVHEP,NHEP,ISTHEP,        IDHEP,
     &JMOHEP,          JDAHEP
      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
      DOUBLE PRECISION PHEP,VHEP
      SAVE /HEPEVT/
      INTEGER         NN, NPAD, K
      DOUBLE PRECISION                      PP,         VV
      COMMON /PYJETS/ NN, NPAD, K(4000,5), PP(4000,5), VV(4000,5)
      INTEGER   I, J 
*
      if(IXpar(5).eq.1) then
        call pycell(njets)
        NN = NN + Njets
      endif
      return
      end
