#!/bin/csh
#                                                            26.11.96 / VK
#                                                            16.01.97 / VK
#                                                            10.09.03 / VK
############################################################################
#
# Herwig run script (to create a HEPEVT ntuple)
#
# Contact person:  Veikko Karimaki
#
########################################################################
#setenv LD_LIBRARY_PATH /usr/local/gcc-alt-3.2/lib:/usr/local/lib
#
set DATACARDS=herwig.txt
set DATACARDFILE=datacards/$DATACARDS
                                                                              
if ( ! -f ${DATACARDFILE} ) then
  echo ' '
  echo The file ${DATACARDFILE} does not exist
  exit 1
endif
#
set JOBNAME=herwig
eval `scram runtime -csh`
set EXEFILE=../../../../test/slc3_ia32_gcc323/testCMKINHerwig
                                                                              
#if ( ! -f ${EXEFILE}.exe ) then
#  echo ' '
#  echo The file ${EXEFILE}.exe does not exist
#  echo Run the script ${JOBNAME}.com first
#  exit 1
#endif
                                                                              
#
if (-f ${JOBNAME}.lis) mv ${JOBNAME}.lis ${JOBNAME}.lis_old
echo Starting execution of $EXEFILE.exe at `date`
#${EXEFILE}.exe >${JOBNAME}.lis <<EOF
${EXEFILE}  <<EOF
 LIST
 CFIL 'EVTO' '${DATACARDS:r}.ntpl '              (Output ntuple file)
C
C-------- Start of channel independent control cards ----------------------
C
C CMS energy (GeV)
C
  ECMS 14000.
C -----------
C No. of events to generate
C
  TRIG    10
C -------------
C Activate the 'NTPL 0' card, if no ntuple file wanted
C
C NTPL  0
C -------------
C No. of events selected (written out)
C
C  NSEL    20                  ! this is set in the datacards file                                                                               
C ----------
C
C particle masses               (not always needed)
C --------------
C
C  PMAS   6,1=175.             ! top quark
C  PMAS  23,1=91.187           ! Z
C  PMAS  24,1=80.22            ! W
C
C  MSTJ  22 = 2                ! Decay those unstable particles
C  PARJ  71 = 10.              ! for which c*tau < 10 mm
C
C-------- End of channel independent control cards -----------------------
C
C-------- Start of channel dependent control cards -----------------------
`cat ${DATACARDFILE}`
EOF
#
echo  Job listing is in ${JOBNAME}.lis
if ( $status ) then
  echo  ------------------------------------------------------
  echo $0 finished on `date` with status code 1
  echo  ------------------------------------------------------
  exit 1
else
  echo  ------------------------------------------------------
  echo  $0 finished on `date`
  echo  Ntuple events are in ${DATACARDS:r}.ntpl
  echo  ------------------------------------------------------
endif
rm -f fort.77 fort.88
############################################################################

