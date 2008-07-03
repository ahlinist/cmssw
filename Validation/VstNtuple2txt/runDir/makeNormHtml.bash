#!/bin/bash
###################################################################
# Purpose: take the norm.txt, shape.txt, and 
# fudgeFactors/fudgeFactorValues.dat produced by Vista 
# and create a html page that presents it in a beautified form.
#
# USAGE: makeNormHtml.bash <dir containing norm.txt shape.txt> 
#   [-o dir where to create html] [-e experiment] [-p QSUB|LOCALLY|CANNED]
#
# Georgios Choudalakis, August 2006
# Update for CMS by Mrenna, 0505, 2008
###################################################################

usage () {
 cat <<EOF 
USAGE: $0 <dir containing norm.txt shape.txt bump.txt> [-o dir where to create html] [-e experiment] [-p QSUB|LOCALLY|CANNED]

EXAMPLE: $0 ~highpt/cvs/current/Quaero/doc/devel/cdf/Vista/current/ -o ~/Public/ -e cdf -p LOCALLY

EOF
 exit 1
}

write () {
 echo $* >> ${target}
}

if [ $# -eq 0 ]; then
 usage
fi

collider=lhc
Experiment=CMS
experiment=cms

sourceDir=$1; shift #to read the options after the mandatory argument
normSource=${sourceDir}/norm.txt
shapeSource=${sourceDir}/shape.txt
bumpSource=${sourceDir}/bump.txt
workingDir=${sourceDir}/
PLOTSPOLICY="LOCALLY"

while getopts o:e:p: OPT; do
 case $OPT in
 o) # Define workingDir, where to put the results
      workingDir=${OPTARG}       
      echo "workingDir=$workingDir"
      ;;
 e) # Define experiment
      experiment=${OPTARG}       
      echo "experiment=$experiment"
      if [ "$experiment" = "lhc" ]; then
	  Experiment=CMS
	  collider=lhc
      fi
      ;;
 p) # Policy for the plots
      if [ "${OPTARG}" = "QSUB" -o "${OPTARG}" = "LOCALLY" -o "${OPTARG}" = "CANNED" ]; then
       PLOTSPOLICY=${OPTARG}
       echo "PLOTSPOLICY=$PLOTSPOLICY"
      else
       usage
      fi
      ;;
  *)
      usage
      ;;
 esac
done

echo ${workingDir}

if [ ! -d ${workingDir} ]; then
    mkdir -p ${workingDir}
fi


target=${workingDir}/"norm.html"
plotsDir=${workingDir}/"plotsDir"

#shift `expr $OPTIND - 1`

if [ ! -d ${sourceDir} ]; then
 echo "You must specify a proper sourceDir."
 exit 1
fi

if [ "$PLOTSPOLICY" = "CANNED" ]; then
 if [ ! -d ${plotsDir} ]; then
  echo "Directory $plotsDir not found. Can't use CANNED plots."
  exit 1
 fi  
fi

date=`head -1 ${normSource}`
niceDate=`date -d "$date" +'%A, %Y/%m/%d'`
numberOfFinalStates=`cat ${normSource} | head -3 | tail -1 | awk '{print $6}'`
numberOfStars=`cat ${normSource} | grep 'Number of disagreeing final states' | awk '{print $NF}'`
numberOfDistributions=`cat ${shapeSource} | grep 'distributions considered' | awk '{print $2}'`
numberOfShapeStars=`cat ${shapeSource} | grep 'score:' | awk '{print $NF}'`
numberOfBumpStars=`cat ${bumpSource} | grep 'bumps:' | awk '{print $NF}'`
if [ -e ${sourceDir}/fudgeFactors/fudgeFactorValues.dat ]; then
  if [ "`printenv multiplicativeFactor`" == "" ]; then
    luminosity=`egrep '[56]001' ${sourceDir}/fudgeFactors/fudgeFactorValues.dat | awk '{print $2}'`
  else
    luminosity=`egrep '[56]001' ${sourceDir}/fudgeFactors/fudgeFactorValues.dat | awk '{print $2*'$multiplicativeFactor'}'`
  fi
else
 luminosity="SOMETHING"
fi



rm -f $target >& /dev/null

write '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">'
write '<html>  <head> '
write '<title>Vista overview</title> '
write '<style type="text/css">'
write 'tr.bottomLine > td { border-style: solid; border-width: 0px 0px 2px 0px; }'
write 'tr.header > td { color:red; background:yellow; text-align:center; padding-left:0.5em; padding-right:0.5em;}'
write 'td.smcomposition { font-size:xx-small; padding-right:0.5em;}'
write 'table.distributions { color:red; font-size:x-small; border-style: solid; border-width: 0px 0px 0px 1px; padding-left:0.5em;}'
write 'table.distributions > td { border-style: solid; border-width: 1px 0px 1px 0px;}'
write 'table.bumps { color:blue; font-size:x-small; border-style: solid; border-width: 0px 0px 0px 1px; padding-left:0.5em;}'
write 'table.bumps > td { border-style: solid; border-width: 1px 0px 1px 0px;}'
write 'p.signature { font-size:xx-small; text-align:right; }'
write '</style>'
write '</head> <body>'

write '<h2> Vista comparison </h2>'
write '<h2> '$Experiment
case "${collider}" in
 "lhc" )
  write ' LHC Preliminary '
  ;;
esac
write '</h2>'
write '<h3> Summary </h3>'
write '<p> Integrated luminosity: '${luminosity}' pb<sup>-1</sup>.</p>'
write '<p> Generated on '${niceDate}' as part of Vista@'$Experiment'.</p>'
write '<p> Data detected in '${numberOfFinalStates}' final states. Number of final states with serious population discrepancy: '${numberOfStars}'.</p>'
write '<p> Kinematic distributions considered: '${numberOfDistributions}'.'
write ' Number of kinematic distributions with serious shape discrepancy: '${numberOfShapeStars}'.'
write ' Number of mass distributions with serious bumps: '${numberOfBumpStars}'.</p>'
write '<h3> Description </h3>'
write '<p>The final states are sorted in order of decreasing discrepancy between the observed population and the expected one. Population discrepancies take into account the statistical uncertainties and the trials factor due to examining many final states. The final states that are seriously discrepant are preceded by an asterisk (*). </p>'
write '<p>Plots of relevant kinematic distributions are available.  The uncertainty in the SM expectation of each final state is statistical. All Monte Carlo events are at the generator level, i.e. they have NOT been run through the full '$Experiment' detector simulation. </p>'
write '<p>The last column lists the most discrepant kinematic distributions in each final state, with their discrepancy given in units of standard deviations accounting for the trials factor.</p>'

write '<h3> Table of final states </h3>'
write '<table cellspacing="-1">'
write '<tr class="header"> <td width="0px"></td> <td align="left">Final State</td> <td>Plots</td> <td>Observed</td> <td colspan="3">Expected</td> <td>Discrepancy (&sigma)</td> <td>SM composition</td> <td>Discrepant Distributions (&sigma)</td><td>Bumps</td> </tr>'


#The plots must be there somehow:
if [ "$PLOTSPOLICY" = "QSUB" ]; then
 # First spawn a job that will generate all the plots. Also, all the composistions (in txt files).
 rm -rf $plotsDir >& /dev/null
 mkdir $plotsDir
 counter=0
 i=0
 n=81
 echo $counter > /tmp/counter.tmp
 cat ${normSource} | tail +9 \
    | grep -v 'CDT\|Number\|CST' \
    | while read line
 do
  if [ "$line" = "" ]; then
   continue
  fi
  counter=`echo ${counter} + 1 | bc`
  echo $counter > /tmp/counter.tmp
  i=`echo ${counter} % ${n} | bc`
  starred=0
  if [ "`echo "$line" | awk '{print $1}'`" = "*" ]; then
   line=`echo "$line" | sed 's/* //'`
   starred=1
  fi
  fs=`echo "$line" | awk '{print $1}'`
  echo "checkDistributionsWithCuts -root Vista_$experiment/files/ bkg data $collider $experiment $fs -jargonReductionFile=./Vista_$experiment/files/mc/nicerTitlesMergedLegend.txt -showBumps=true > $plotsDir/checkDistributionsWithCuts_$fs.output ; mv plots_withCuts_$fs.ps $plotsDir/ ; gzip $plotsDir/plots_withCuts_$fs.ps ; rm plots_withCuts_${fs}.C ; " >> command_$i.tcsh
 done
 for i in `printRange 0 $n` 
 do
   $RELEASE_DIR/bin/qsub "cd ~/cvs/current; source Quaero_utils/utilities/setup.tcsh; source command_$i.tcsh" > /dev/null
 done
 counter=`cat /tmp/counter.tmp`
 segments=${counter}
 echo "Submitted $segments jobs"
 chmod a+rwx ${plotsDir}


 # Wait until all the compositions and plots are produced, but don't wait longer than some time given below.
 minutesSlept=0
 filesReturned=0
 while (( ${filesReturned} < ${segments} && ${minutesSlept} < 30 ))
 do
  sleep 60
  let minutesSlept++
  filesReturned=`ls -1 ${plotsDir}/checkDistributionsWithCuts_*.output 2> /dev/null | wc -l`
  echo "After last submission, slept for ${minutesSlept} minutes. Files returned : ${filesReturned}"
 done
 #OK, plots generated now, or time ran out.
 rm command_[0-9]*.tcsh
fi
if [ "$PLOTSPOLICY" = "LOCALLY" ]; then
 # generate the plots....without qsub.
 rm -rf $plotsDir >& /dev/null
 mkdir $plotsDir
 cat ${normSource} | tail +9 \
     | grep -v 'CDT\|Number\|CST' \
     | while read line
 do
  if [ "$line" = "" ]; then
   continue
  fi
  starred=0
  if [ "`echo "$line" | awk '{print $1}'`" = "*" ]; then
   line=`echo "$line" | sed 's/* //'`
   starred=1
  fi
  fs=`echo "$line" | awk '{print $1}'`

  echo $fs 
  scriptDir=${CMSSW_BASE}/bin/slc4_ia32_gcc345/

  echo $scriptDir

  ${scriptDir}/checkDistributionsWithCuts -root $PWD bkg sig ${collider} ${experiment} $fs > ${plotsDir}/checkDistributionsWithCuts_${fs}.output 

#  checkDistributionsWithCuts -root $RELEASE_DIR/Vista/results/baseDirectory bkg data ${collider} ${experiment} $fs -jargonReductionFile=$RELEASE_DIR/Vista_$experiment/files/mc/nicerTitlesMergedLegend.txt  > ${plotsDir}/checkDistributionsWithCuts_${fs}.output 

 ps2pdf -dPDFSETTINGS=/screen plots_withCuts_${fs}.ps
 mv plots_withCuts_${fs}.pdf $plotsDir/ 
# gzip $plotsDir/plots_withCuts_${fs}.ps 
 rm plots_withCuts_${fs}.C  
 rm plots_withCuts_${fs}.ps
 done
 #OK, plots generated now.
 chmod a+rwx ${plotsDir}
fi




# Now loop again over the norm.txt and construct the html table.
cat ${normSource} | tail +9 \
    | grep -v 'CDT\|Number\|CST' \
    | while read line
do
 if [ "$line" = "" ]; then
  continue
 fi

 write '<tr class="bottomLine" valign="top">'

 starred=0
 if [ "`echo "$line" | awk '{print $1}'`" = "*" ]; then
  line=`echo "$line" | sed 's/* //'`
  starred=1
 fi

 fs=`echo "$line" | awk '{print $1}'`
 #echo $fs
 discrepancy=`echo "$line" | awk '{print $3}'`
 data=`echo "$line" | awk '{print $4}'`
 bkg=`echo "$line" | awk '{print $5}'`
 dbkg=`echo "$line" | awk '{print $7}'`

# pageNumber=`echo "$line" | awk -F'[' '{print $2}' | awk -F']' '{print $1}'`

 if [ -e ${plotsDir}/checkDistributionsWithCuts_$fs.output ]; then
#  composition=`cat ${plotsDir}/checkDistributionsWithCuts_${fs}.output | sed -e 's/^[^(]*( //' | sed -e 's/, )//g' | sed -e 's/->/\&rarr;/g' | sed -e 's/>/ > /g' | sed -e 's/</ < /g' | sed -e 's/mu/\&#956;/g' | sed -e 's/tau/\&#964;/g' `
  composition=`cat ${plotsDir}/checkDistributionsWithCuts_${fs}.output | sed -e 's/^[^(]*( //' | sed -e 's/, )//g' | sed -e 's/#rightarrow/\&rarr;/g' | sed -e 's/>/ > /g' | sed -e 's/</ < /g' | sed -e 's/#mu/\&#956;/g' | sed -e 's/#tau/\&#964;/g' | sed -e 's/#nu/\&#957;/g' | sed -e 's/#gamma/\&#947;/g' | sed -e 's/#bar{t}/tbar/g' `
 else
  composition=`cat ${normSource} | grep ' '${fs}' ' | sed -e 's/^[^(]*( //' | sed -e 's/, )//g' | sed -e 's/->/\&rarr;/g' | sed -e 's/>/ > /g' | sed -e 's/</ < /g' | sed -e 's/mu/\&#956;/g' | sed -e 's/tau/\&#964;/g' `
 fi
 if [ "$composition" = ")" ]; then
     composition="&nbsp"
 fi
   
 pageLink=""
#modify to be pdf
# if [ -e ${plotsDir}/plots_withCuts_$fs.ps.gz ]; then
#  pageLink=${pageLink}'<a href="plotsDir/'plots_withCuts_${fs}.ps.gz'">plots</a>'
# fi

 if [ -e ${plotsDir}/plots_withCuts_$fs.pdf ]; then
  pageLink=${pageLink}'<a href="plotsDir/'plots_withCuts_${fs}.pdf'">plots</a>'
 fi


#  composition="`echo $composition | sed -e 's/</ < /g'`"
#  compositionA="`echo $composition | sed -e 's/\([^,]*,[^,]*,[^,]*,[^,]*,[^,]*\).*/\1/'`"
#  number="`echo $composition | sed -e 's/\([^,]*,[^,]*,[^,]*,[^,]*,[^,]*\)//' | sed -e 's/[^,]*=/ /' | sed -e 's/,[^,]*=/ /g'`"
#  numberA="`echo $compositionA | sed -e 's/\([^,]*,[^,]*,[^,]*,[^,]*,[^,]*\)//' | sed -e 's/[^,]*=/ /' | sed -e 's/,[^,]*=/ /g'`"
#  number=`accumulate $number`
#  numberA=`accumulate $numberA`
#  numberB=`accumulate $number "-$numberA"`
#  composition="$compositionA , Other = $numberB"
#  composition=`echo $composition | sed -e 's/,/<br>/g'` 

 # read the most discrepant shapes from shape.txt
 echo '<table class="distributions">' > /tmp/normHtml.tmp
 grep -A 50 ' '${fs}' ' ${shapeSource} | tail +2 \
    | grep -v runHistory | grep -v zvtx | grep -v generatedSumPt \
    | while read distribution
 do
  numberOffields=`echo "$distribution" | wc -w`
  if [ ${numberOffields} -ne 3 ]; then
   break
  fi
  kinVariable=`echo "$distribution" | awk '{print $2}'`
  kinSigma=`echo "$distribution" | awk '{print $3}'`
  echo '<tr><td>'${kinVariable}'</td><td>'${kinSigma}'</td></tr>' >> /tmp/normHtml.tmp
 done
 echo '</table>' >> /tmp/normHtml.tmp
 distributions=`cat /tmp/normHtml.tmp`

 # read the most discrepant bumps from bump.txt
 echo '<table class="bumps">' > /tmp/normHtmlBump.tmp
 grep -A 50 ' '${fs}' ' ${bumpSource} | tail +2 \
    | while read distribution
 do
  numberOffields=`echo "$distribution" | wc -w`
  if [ ${numberOffields} -ne 4 ]; then
   break
  fi
  bumpVariable=`echo "$distribution" | awk '{print $2}'`
  bumpSigma=`echo "$distribution" | awk '{print $3}'`
  bumpRange=`echo "$distribution" | awk '{print $4}'`
  rangeLeft=`echo "${bumpRange}" | sed -e 's/\[\|\]//g' | awk -F',' '{print $1}'`
  rangeRight=`echo "${bumpRange}" | sed -e 's/\[\|\]//g' | awk -F',' '{print $2}'`
  rangeLeftRounded=`echo "print round($rangeLeft,0)" | python | sed -e 's/\.0//'`
  rangeRightRounded=`echo "print round($rangeRight,0)" | python | sed -e 's/\.0//'`
  bumpRange='['${rangeLeftRounded}','${rangeRightRounded}']'
  echo '<tr><td>'${bumpVariable}'</td><td>'${bumpSigma}'</td><td>'${bumpRange}'</td></tr>' >> /tmp/normHtmlBump.tmp
 done
 echo '</table>' >> /tmp/normHtmlBump.tmp
 bumps=`cat /tmp/normHtmlBump.tmp`

 # write the table row
 if [ ${starred} -eq 1 ]; then 
  write '<td align="right"><b>* </b></td>'
 else
  write '<td>&nbsp</td>'
 fi
 write '<td align="left"> &nbsp '$fs' &nbsp </td>'
 write '<td align="center">['${pageLink}']</td>'
 write '<td align="center">'${data}'</td>'
 write '<td align="right">'${bkg}'</td><td>+-</td><td align="left">'${dbkg}'</td>'
 write '<td align="center">'${discrepancy}'</td>'
 write '<td class="smcomposition">'${composition}'</td>'
 if [ `cat /tmp/normHtml.tmp | wc -l` -gt 2 ]; then
  write '<td>'${distributions}'</td>'
 else
  write '<td>&nbsp</td>'
 fi
 if [ `cat /tmp/normHtmlBump.tmp | wc -l` -gt 2 ]; then
  write '<td>'${bumps}'</td>'
 else
  write '<td>&nbsp</td>'
 fi
 write '</tr>'
done

write '</table>'
write '<hr>'
if [ "$experiment" == "cms" ]; then
  write '<p class=signature>Generated by the <a href="http://home.fnal.gov/~mrenna" target="_top">LPC SIM group</a>.</p>'
fi
if [ "$experiment" == "d0" ]; then
  write '<p class=signature>Generated by <a href="http://www-clued0.fnal.gov/~piperj">Joel</a>, <a href="http://www-clued0.fnal.gov/~nayeem/">Nayeem</a>, <a href="http://www-clued0.fnal.gov/~oleksiy/">Oleksiy</a>, <a href="http://www-clued0.fnal.gov/~serban/">Serban</a>, and <a href="http://home.fnal.gov/~meyera/">Arnd</a>, with technical assistance from <a href="http://mit.fnal.gov/~knuteson/">Bruce</a>.</p>'
fi
if [ "$experiment" == "l3" ]; then
  write '<p class=signature>Generated by <a href="http://wwweth.cern.ch/~holzner/">Andre Holzner</a> and <a href="http://mit.fnal.gov/~knuteson">Bruce Knuteson</a>.</p>'
fi
if [ "$experiment" == "aleph" ]; then
  write '<p class=signature>Generated by <a href="http://thinktank.rootnode.com/Wiki/Kyle%20Cranmer">Kyle Cranmer</a> and <a href="http://mit.fnal.gov/~knuteson">Bruce Knuteson</a>.</p>'
fi
if [ "$experiment" == "h1" ]; then
  write '<p class=signature>Generated by <a href="http://wwwhep.physik.uni-freiburg.de/~scaron">Sascha Caron</a> and <a href="http://mit.fnal.gov/~knuteson">Bruce Knuteson</a>.</p>'
fi
write '</body> </html>'

rm /tmp/normHtml.tmp /tmp/normHtmlBump.tmp
rm /tmp/counter.tmp
