#!/bin/csh
#                                                         26.11.96 / VK
#                                                         12.12.00 / VK
#                                                         19.07.02 / VK
#                                      Updated for cms130 30.01.03 / VK
############################################################################
#
# CMKIN run script (to create a HEPEVT ntuple)
#
# Contact persons:  Lucas Taylor
#                   Veikko Karimäki
#
############################################################################
#
# setenv LD_LIBRARY_PATH /usr/local/gcc-alt-3.2/lib:/usr/local/lib
#
#  set DATACARDFILE=bsmumu.txt
#  set DATACARDFILE=h2taujj200.txt
#  set DATACARDFILE=sm_qq_qqh120_inv.txt
#  set DATACARDFILE=qq_qqh135_2taulj.txt
#  set DATACARDFILE=h2taujj500.txt
#  set DATACARDFILE=h2tau_tauola.txt
#  set DATACARDFILE=minbias.txt
#  set DATACARDFILE=mutau_gg_bbh200_2tau_muX_no_cuts.txt
#  set DATACARDFILE=etau_tt_purelept.txt
#  set DATACARDFILE=etau_wj_80_10.txt
#  set DATACARDFILE=zz_2e2mu.txt
  set DATACARDFILE=hzz_2e2mu.txt
#  set DATACARDFILE=hplus_tau_like.txt
#  set DATACARDFILE=eg_wwzz.txt
#  set DATACARDFILE=eg_gam.txt
#  set DATACARDFILE=mutau_bb1.txt
#  set DATACARDFILE=mutau_bb2.txt
#  set DATACARDFILE=mutau_w_jet.txt
#  set DATACARDFILE=mutau_z_gamma_star.txt
#  set DATACARDFILE=h_eemm.txt
#  set DATACARDFILE=high_pt_jets.txt
#  set DATACARDFILE=jpsik0.txt
#  set DATACARDFILE=msugra1.txt
#  set DATACARDFILE=tt_4l.txt
#  set DATACARDFILE=zbb_4l.txt
#  set DATACARDFILE=zz_2l2nu.txt
#  set DATACARDFILE=zz_4mu.txt
#
 set npyt=6220                 
 set DATACARDFILE=datacards/$DATACARDFILE
 
if ( ! -f ${DATACARDFILE} ) then
  echo ' '
  echo The file ${DATACARDFILE} does not exist
  exit 1
endif
#
 set JOBNAME=pythia
eval `scram runtime -csh`
 set EXEFILE=testCMKINPythiaHepMC
# 
 
#if ( ! -f ${EXEFILE}.exe ) then
#  echo ' '
#  echo The file ${EXEFILE}.exe does not exist
#  echo Run the script ${JOBNAME}.com first
#  exit 1
#endif

if (-f ${JOBNAME}.lis) mv ${JOBNAME}.lis ${JOBNAME}.lis_old
echo
echo 'Starting execution of  ' $EXEFILE 
echo '  using datacard file  ' $DATACARDFILE
echo on `date`

#${EXEFILE} >${JOBNAME}_zz.lis <<EOF
${EXEFILE}  <<EOF
LIST
C
C-------- Start of channel independent control cards ----------------------
C
C CMS energy (GeV)      
C
  ECMS 14000.             
C -----------
C No. of events to generate  
C
  TRIG    9999999
C -------------
C Activate the 'NTPL 0' card, if no ntuple file wanted
C
C NTPL  0
C -------------
C No. of events selected (written out)
C
	  NSEL    10           
C -----------
C particle masses               (not always needed)
C --------------
C
C PMAS   6,1=175.             ! top quark
C PMAS  23,1=91.187           ! Z
C PMAS  24,1=80.22            ! W
C
  MSTJ  22 = 2                ! Decay those unstable particles
  PARJ  71 = 10.              ! for which c*tau < 10 mm
C
C-------- End of channel independent control cards -----------------------
C
C-------- Start of channel dependent control cards -----------------------
`cat ${DATACARDFILE}`
EOF

if ( $status ) then
  echo  ------------------------------------------------------
  echo $0 finished on `date` with status code 1
  echo  ------------------------------------------------------
  exit 1
else
  echo  ------------------------------------------------------
  echo  $0 finished on `date`
  echo  ------------------------------------------------------
endif
############################################################################
