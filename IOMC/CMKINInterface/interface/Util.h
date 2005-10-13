// -----------------------------------------------------------------------------
//  First steps toward CMKIN++
//  Util.h -> CMKIN kiutil fortran rutines  (libkine_d.a)
// -----------------------------------------------------------------------------
//  Date: 2004/10/26
//  Revision: 0.001 (pre alpha!)
// -----------------------------------------------------------------------------
//  Author: Hector Naves Sordo (Email: naves@mail.cern.ch)
// -----------------------------------------------------------------------------

#ifndef Util_h
#define Util_h

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
#define close_ntuple close_ntuple_
#define io_change_file io_change_file_
#define io_close_file io_close_file_
#define io_defaults io_defaults_
#define io_dir io_dir_
#define io_ffkey io_ffkey_
#define io_file io_file_
#define io_file_find io_file_find_
#define io_file_set io_file_set_
#define io_file_type io_file_type_
#define io_fil_kaddr io_fil_kaddr_
#define io_i_am_fz io_i_am_fz_
#define io_i_am_rz io_i_am_rz_
#define io_init io_init_
#define io_mult_file io_mult_file_
#define io_open_err io_open_err_
#define io_open_file io_open_file_
#define io_open_fz_file io_open_fz_file_
#define io_string_find io_string_find_
#define io_string_len io_string_len_
#define io_string_pad io_string_pad_
#define io_unit io_unit_
#define open_new_ntuple open_new_ntuple_
#define rename_ntuple rename_ntuple_
#define iucomc iucomc_
//
#else
//
#define close_ntuple CLOSE_NTUPLE 
#define io_change_file IO_CHANGE_FILE 
#define io_close_file IO_CLOSE_FILE 
#define io_defaults IO_DEFAULTS 
#define io_dir IO_DIR 
#define io_ffkey IO_FFKEY 
#define io_file IO_FILE 
#define io_file_find IO_FILE_FIND 
#define io_file_set IO_FILE_SET 
#define io_file_type IO_FILE_TYPE 
#define io_fil_kaddr IO_FIL_KADDR 
#define io_i_am_fz IO_I_AM_FZ 
#define io_i_am_rz IO_I_AM_RZ 
#define io_init IO_INIT 
#define io_mult_file IO_MULT_FILE 
#define io_open_err IO_OPEN_ERR 
#define io_open_file IO_OPEN_FILE 
#define io_open_fz_file IO_OPEN_FZ_FILE 
#define io_string_find IO_STRING_FIND 
#define io_string_len IO_STRING_LEN 
#define io_string_pad IO_STRING_PAD 
#define io_unit IO_UNIT 
#define open_new_ntuple OPEN_NEW_NTUPLE 
#define rename_ntuple RENAME_NTUPLE 
#define iucomc IUCOMC
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


  extern "C" void type_of_call close_ntuple(void);    //**KI_PROC** the ntuple is filled



  // Action: Changes the file to the next one in multiple file
  //          environment
  // Input:  C_KEYWORD  = Keyword of file to be changed
  // Output: LUN        = Logical unit on which file was opened
  // Output: ISTAT      = Return code (0 if successful)
  //???????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_change_file(char &C_KEYWORD, bool &LUN, int &ISTAT);

  
  // Action:           Close file corresponding to keyword
  // Input:            C_KEYWORD    keyword of file to be closed
  // Output:           ISTAT        return code (=0 if successful)

  extern "C" int type_of_call io_close_file(char &C_KEYWORD, int &ISTAT);



  // Action:           Set defaults for database keywords, directories, files
  // Input:            None
  // Output:           None

  extern "C" void type_of_call io_defaults(void);



  // Action:  Returns the physical directory name associated to CMDB keyword
  // Input:   C_KEYWORD    = Character keyword of file
  // Output:  CFNAME       = Character name of directory
  // Output:  ISTAT        = 0 if call was successful
  //??????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_dir(char &C_KEYWORD, char &CFNAME, int &ISTAT);



  // Action:           Defines data cards for database
  // Input:            None
  // Output:           None

  extern "C" void type_of_call io_ffkey(void);



  // Action:  Returns the physical file name associated to CMDB keyword
  // Input:   C_KEYWORD     Character keyword of file
  // Output:  C_FNAME       Character name of file
  // Output:  ISTAT         =0 if call was successful
  //??????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_file(char &C_KEYWORD,  char* C_FNAME, int &ISTAT);



  // Action: Finds the file in multiple file system with leaf name
  //         as given for the set keyword
  // Input:  C_KEYWORD    Keyword of file to be queried
  // Input:  C_LEAF       Leaf name of the file to be queried
  // Output: C_FILE       File name of the file (blank if not found)
  //??????????????????????????????????????????????????????????????????????????

  extern "C" char type_of_call io_file_find(char &C_KEYWORD, char &C_LEAF, char* C_FILE);




  // Action: Sets the output file tiple file system with leaf name
  //         as given for the set keyword
  // Input:  C_KEYWORD    Keyword of file to be set
  // Input:  C_FILE       File name to be set
  // Output: ISTAT        Status of setting (0 if no error)

  extern "C" int type_of_call io_file_set(char &C_KEYWORD,  char &C_FILE, int &ISTAT);




  // Action:           Finds file type (extension) from filename
  // Input:            C_KEYWORD   keyword of file to be checked
  // Output:           C_TYPE      file type (extension)
  // Output:           ISTAT       return code (=0 if successful)
  //??????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_file_type(char &C_KEYWORD,  char* C_TYPE, int &ISTAT);



  // Action:           Get K address of file corresponding to C_KEYWORD
  // Input:            C_KEYWORD keyword of file to be opened
  // Output:           K_ADDRESS address in IO arrays corresponding to
  //                   specified C_KEYWORD (=0 if none found)
  //??????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_fil_kaddr(char &C_KEYWORD,int &K_ADDRESS);




  // Action:           Checks whether file is FZ format
  // Input:            C_KEYWORD   keyword of file to be checked
  // Value:           .TRUE./.FALSE. if file is/is not in FZ format

  extern "C" bool type_of_call io_i_am_fz(char &C_KEYWORD);




  // Action:           Checks whether file is RZ format
  // Input:            C_KEYWORD   keyword of file to be checked
  // Value:           .TRUE./.FALSE. if file is/is not in RZ format

  extern "C" bool type_of_call io_i_am_rz(char &C_KEYWORD);


  // Action:           Decodes data cards for database
  // Input:            None
  // Output:           None

  extern "C" void type_of_call io_init(void);



  // Action: Finds if there are multiple files requested for this
  //         keyword
  // Input:  C_KEYWORD    Keyword of file to be queried
  // Output: IO_MULT_FILE Return code (true if multiple files)

  extern "C" bool type_of_call io_mult_file(char &C_KEYWORD);



  // SUBROUTINE IO_OPEN_ERR (C_OPEN_ROUTINE,C_FILE_NAME,I_ERROR)
  // Action:           print message in the event of error opening file
  // Input:            C_OPEN_ROUTINE name of the opening routine
  // Input:            C_FILE_NAME    name of the file requested
  // Input:            I_ERROR        error code (=0 if successful)
  // Output:           None

  extern "C" void type_of_call io_open_err(char &C_OPEN_ROUTINE,char &C_FILE_NAME,int &I_ERROR);



  // Action:           Open file corresponding to keyword
  // Input:            C_KEYWORD    keyword of file to be opened
  // Output:           LUN          logical unit on which file was opened
  // Output:           ISTAT        return code (=0 if successful)
  //?????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call io_open_file(char &C_KEYWORD,bool &LUN, int &ISTAT);



  // Action:           Open FZ event file
  // Input:            C_FNAME      file to be opened
  // Input:            LUN          logical unit on which to open file
  // Input:            C_OPTION     = 'READ' or 'WRIT'
  // Output:           ISTAT        return code (=0 if successful)

  extern "C" int type_of_call io_open_fz_file(char &C_FNAME,bool &LUN, 
						char &C_OPTION,int &ISTAT);



  // Action: Finds a character string (of no more than 4 characters)
  //         in an array of hollerith numbers
  // Input:  C_STRING   = Character string to be searched for
  // Input:  I_HOLL     = Array to be searched
  // Input:  I_HOLL_LEN = Length of array I_HOLL
  // Output: INDEX      = Index number in I_HOLL corresponding to C_STRING

  extern "C" int type_of_call io_string_find(char &C_STRING, char &I_HOLL, 
					       int &I_HOLL_LEN, int &INDEX);



  // Action:           Decodes data cards for database
  // Input:            C_STRING  Character string
  // Output:           None

  extern "C" void type_of_call io_string_len(char &C_STRING);



  // Action:           Remove leading and trailing spaces, tabs, and nulls
  //                   from a string. The first space, tab, or null found in
  //                   the string (after leading ones are removed) is
  //                   considered the end of the string. The end of the string
  //                   is padded with blanks.
  // Input:            C_STRING  Character string
  // Output:           C_STRING  The same character string padded with blanks

  extern "C" char type_of_call io_string_pad(char &C_STRING);



  // Action:  Assigns, checks, or deassigns logical unit numbers
  //          Unit numbers are deliberately NOT available in COMMON
  //          to ensure they are only ever accessed via this routine
  // Input:   C_KEYWORD    = Character keyword of file
  // Input:   C_ACTION     = 'PICK' assign unit for this keyword
  //                         'CHEK' return current unit for this keyword
  //                         'DROP' de-assign unit for this keyword
  // Output:  LOGICAL_UNIT = Integer logical unit number
  // Output:  ISTAT        = 0 if call was successful

  extern "C" int type_of_call io_unit(char &C_KEYWORD, char &C_ACTION, 
					int &LOGICAL_UNIT, int &ISTAT);




  extern "C" int type_of_call open_new_ntuple(void); //return 1 if error



  extern "C" void type_of_call rename_ntuple(int &INUM);

  
  
  // Function    : look for character string CHAR in character           
  //               array CHAR(NV)                                        
  //???????????????????????????????????????????????????????????????????????????

  extern "C" int type_of_call iucomc(char &CHAR, char &CHARV, int &NV);



  // Function comments are from the original fortran code


}

#endif



