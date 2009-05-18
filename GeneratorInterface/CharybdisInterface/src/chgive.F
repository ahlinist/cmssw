c----------------------------------------------------------------------------c
C...CHGIVE.F: Based on : TXGIVE.F V:1.03,   06/09/2006 by Serge Slabospitsky
C...Sets values of common block variables for communication with Charybdis 1.003
C...I: Index in the array
C...IVAL: If nonzero, it's value goes to Ipar(I)
C...RVAL: If nonzero, it's value goes to Rpar(I)
c----------------------------------------------------------------------------c

      SUBROUTINE CHGIVE(I,IVAL,RVAL)
      implicit none 
      Integer           Ipar     ! global TopRex integer parameters  
      double precision  Rpar     ! global TopRex real    parameters
      common /TXPAR/Ipar(200), Rpar(200)
      save   /TXPAR/
***
      double precision  RVAL
      integer I, IVAL
      IF(I.eq.0) THEN     
         print *,
     &   '[CHGIVE] Whoops, something`s wrong with the data you sent:'
         print *,'[CHGIVE] I=',I,", IVAL=", IVAL,", RVAL=",RVAL
      ELSE 
c	 print *, I, IVAL, RVAL
	 IF(I.gt.0) THEN ! index is positive: integer or bool
	    Ipar(I)=IVAL
	 ELSE
	    Rpar(-I)=RVAL
	 ENDIF
      ENDIF
      END

