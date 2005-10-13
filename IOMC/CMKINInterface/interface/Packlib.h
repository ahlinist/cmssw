// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Packlib.h -> some CERNLIB packlib fortran rutines  
// -----------------------------------------------------------------------------
//  Date: 2005/05/24
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Packlib_h
#define Packlib_h

#ifdef _WIN32
# pragma once
# ifdef DLLEXPORT
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllexport)
#  endif
# else
#  ifndef DLLLINKAGE
#   define DLLLINKAGE __declspec(dllimport)
#  endif
# endif
#endif
#ifndef DLLLINKAGE
# define DLLLINKAGE
#endif

#ifndef _WIN32
//
#define hlimit hlimit_
#define hbset hbset_
#define exitf exitf_
#define hrin hrin_
#define hexist hexist_
#define hprint hprint_
#define hnoent hnoent_
#define hnbptr hnbptr_
#define hdelet hdelet_
#define hcdir hcdir_
#define hgnt hgnt_
#define hgntf hgntf_
#define ffinit ffinit_
#define ffset ffset_
//
#else
//
#define hlimit HLIMIT
#define hbset HBSET
#define exitf EXITF
#define hrin HRIN
#define hexist HEXIST
#define hprint HPRINT
#define hnoent HNOENT
#define hnbptr HNBPTR
#define hdelet HDELET
#define hcdir HCDIR
#define hgnt HGNT
#define hgntf HGNTF
#define ffinit FFINIT
#define ffset FFSET
//
#endif

//From ROOT file TCallf77
//(From Pythia6Functions.h, I.Gonzalez Caballero)
#ifndef WIN32
# define type_of_call
#else
# define type_of_call  _stdcall
#endif




extern "C" {


  //*.==========>
  //*.            Initialization routine for HBOOK
  //*.            IF(LIMIT>0) CALL MZEBRA and MZPAW
  //*.            IF(LIMIT=0) no calls to MZEBRA and MZPAW
  //*.            IF(LIMIT<0) CALL MZPAW
  //*..=========> ( R.Brun )

  extern "C" void type_of_call hlimit(int &LIMIT);



 
 //*.==========>
  //*.           N-tuple option setting
  //*.
  //*.
  //*..=========> ( A.A.Rademakers )

  extern "C" void type_of_call hbset(char &OPTION, int &IVAL, int &IERROR);




  extern "C" void type_of_call exitf (int &ICODE);




  //*.==========>
  //*.           Read histogram IDD from current directory (RZ or GLOBAL)
  //*.           The histogram stored in memory will be IDD+IOFSET
  //*..=========> ( R.Brun )

  extern "C" void type_of_call hrin(int &IDD, int &ICYCLE, int &KOFSET);



  //*.==========>
  //*.           HEXIST=.TRUE. IF ID2 PRESENTLY EXIST
  //*..=========> ( R.Brun )

  extern "C" bool type_of_call hexist(int &ID2);



  //*.==========>
  //*.           PRINTS HIST ID1
  //*.           IF(ID1=0) PRINTS ALL HIST IN THE BOOKING ORDER
  //*..=========> ( R.Brun )

  extern "C" void type_of_call hprint(int &ID1);



  //*.==========>
  //*.           RETURNS THE NUMBER OF ENTRIES OF IDD
  //*..=========> ( R.Brun )

  extern "C" int type_of_call hnoent(int &IDD,int &NUMB);



  //*.==========>
  //*.
  //*.           Return the LBLOK of the block with name BLKNAM
  //*.
  //*..=========> ( A.A.Rademakers )

  extern "C" void type_of_call hnbptr(char &BLKNA1);
  


  //*.==========>
  //*.           DELETE ID FROM MEMORY
  //*..=========> ( R.Brun )

  extern "C" void type_of_call hdelet(int &ID1);



  //*.==========>
  //*.      To set,read or print the Current Working Directory
  //*. Input:
  //*.   *CHPATH* Character  variable  specifying  the  pathname  of  the  CWD
  //*.            (default option).
  //*.            The top level directory is //PAWC.  When  the pathname does
  //*.            not start with a '/', the pathname is prefixed with the path
  //*.            of the CWD.
  //*.   CHOPT    Character variable specifying the option
  //*.            'R'   Read the CWD pathname into CHPATH
  //*.            'P'   Print the CWD
  //*.            ' '   Set the CWD
  //*. Output:
  //*.   *CHPATH* Character variable  containing the complete pathname  of the
  //*.            current working directory (with 'R' option only).
  //*..=========> ( R.Brun )

  extern "C" char type_of_call hcdir(char &CHPATH,char &CHOPT);


  
  //*.==========>
  //*.
  //*.           Describing the variables to be stored in the new
  // *.           (variable row length) n-tuple.
  //*.           ADDRES must be the address of a integer or real variable.
  //*.           For the data-structure description see routine HBNT.
  //*.
  //*..=========> ( A.A.Rademakers )
  
  extern "C" void type_of_call hbname(int &ID, char &BLKNAM, int &ADDRES, char &FORM);


  
  
  //*.==========>
  //*.
  //*.           Return in the preset addresses (set by HBNAME)
  //*.           the variables of event (row) IDNEVT.
  //*.
  //*.  This routine calls HGNT1. HGNT1 checks the IDD and scans over all
  //*.  blocks, calling for each block HGNT2. HGNT2 does the actual retrieval.
  //*.
  //*..=========> ( A.A.Rademakers )

  extern "C" void type_of_call hgnt(int &IDN,int &IDNEVT,int &IERROR);
  

  //*.==========>
  //*.
  //*.           Return in the preset addresses (set by HBNAME)
  //*.           the values of the variables as used in the last call
  //*.           to HGNT, HGNTB, or HGNTV for the event (row) IDNEVT.
  //*.
  //*.  This routine does the actual reading of the variables in LTMP1.
  //*.
  //*..=========> ( A.A.Rademakers )

  extern "C" void type_of_call hgntf(int &IDN,int &IDNEVT,int &IERROR);



  //
  //      FUNCTIONAL DESCRIPTION:
  //
  //              This routine intializes the FFREAD package.
  //              If any keys had been previously specified,
  //              they are erased with all associated information.
  //
  //      DUMMY ARGUMENTS:
  //
  //              NW specifies the number of words allocated by the
  //              user in common CFREAD. This can be used to increase
  //              the number of available keys.
  //
  //      IMPLICIT INPUTS:
  //
  //              NONE
  //
  //      IMPLICIT OUTPUTS:
  //
  //              The common CFREAD is initialized with default information.
  //
  //      SIDE EFFECTS:
  //
  //              Any information set up by previous calls to FFREAD routines
  //              is destroyed. Users of FFREAD (which calls FFINIT with
  //              NW=0) can call FFINIT(NW) before calling FFREAD.
  //
  
  extern "C" void type_of_call ffinit(int& NW);



  //
  //      FUNCTIONAL DESCRIPTION:
  //
  //              This sets some values which are optional within FFREAD.
  //
  //      DUMMY ARGUMENTS:
  //
  //              CHOPT  - Character argument specifying which item to change.
  //                       Valid are:
  //                         LENG - set # of significant characters on data cards
  //                         LINP - change input  LUN
  //                         LOUT - change output LUN
  //                         SIZE - set key size in number of characters.
  //                                This must be at least 4 and cannot be
  //                                greater than NCHMAX.
  //                       The last option, SIZE, can only be specified
  //                       after a call to FFINIT and before the first
  //                       call to FFKEY/FFGO.
  //              IVALUE - Value of the desired argument
  //
  //      IMPLICIT INPUTS:
  //
  //              NONE
  //
  //      IMPLICIT OUTPUTS:
  //
  //              The appropriate value in common CFREAD is changed.
  //
  //      SIDE EFFECTS:
  //
  //              NONE
  

  extern "C" void type_of_call ffset(const char* CHOPT, int* IVALUE);




  // Function comments are from the original fortran code


}

#endif



