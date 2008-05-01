#!/bin/tcsh
#####################
# RPV SUSY
#
setenv example rpvSusy
cd $RELEASE_DIR/Quaero/tests/
rm tmp_$example.txt >& /dev/null
setenv dir $RELEASE_DIR/Quaero/experiments/hera/h1/desy/signal/rpvSusy/
setenv r 1
echo "print 0.42/(0.013**2), 0.39/(0.030**2), 0.35/(0.050**2), 0.34/(0.100**2), 0.59/(0.300**2)" > tmp.py; setenv xsecLambdaEqOne `python tmp.py`; rm tmp.py
setenv MsqTuple "150 200 225 250 275"
setenv lambdaTuple "0.015 0.03 0.05 0.1 0.3"
foreach i ( 1 2 3 4 5 )
  foreach j ( 0.5 1.0 1.5 2.0 )
    echo $lambdaTuple | awk -v z=`expr $i` '{print $z}' > /tmp/tmp_$$.txt
    setenv lambda `cat /tmp/tmp_$$.txt`
    rm /tmp/tmp_$$.txt
    echo "print ($lambda * $j)" > tmp.py; setenv lambda `python tmp.py`; rm tmp.py
    echo $MsqTuple | awk -v z=`expr $i` '{print $z}' > /tmp/tmp_$$.txt
    setenv Msq `cat /tmp/tmp_$$.txt`
    rm /tmp/tmp_$$.txt
    echo 'Msq = ' $Msq '  lambda = ' $lambda 
    echo $xsecLambdaEqOne | awk -v z=`expr $i` '{print $z}' > /tmp/tmp_$$.txt
    setenv xsec `cat /tmp/tmp_$$.txt`
    rm /tmp/tmp_$$.txt
    echo "print ($lambda **2)*$xsec*10000/50000" > tmp.py; setenv f `python tmp.py`; rm tmp.py
    $RELEASE_DIR/bin/extractRandomSubsetOfHepevtEvents $r $f $dir/sq${Msq}.hepevt $dir/sq${Msq}_lambda${lambda}.hepevt
    hepevt2stdhep $dir/sq${Msq}_lambda${lambda}.hepevt $dir/sq${Msq}_lambda${lambda}.stdhep
    perl runTestJob.pl hera h1 dis "rpvSusy" $Msq $lambda 0 0   | tail -1 | awk '{print $2}' >> tmp_$example.txt
    rm $dir/sq${Msq}_lambda${lambda}.hepevt $dir/sq${Msq}_lambda${lambda}.stdhep
    sleep 60
  end
end
echo 'All jobs submitted'
echo 'Now keeping tabs on submitted jobs'
./keepTabsOnQuaeroJobs.tcsh `cat tmp_$example.txt`
source setupCDF.tcsh
python drawLogLcurve_2d.py "rpvSusy" `head -1 tmp_$example.txt` `wc tmp_$example.txt | awk '{print $1}'` > h1_${example}_logL.txt
setenv example h1_$example
root drawLogLcurve_2d.C
mv model_tag.eps $RELEASE_DIR/Quaero/doc/results_h1/${example}_logL_2d.eps
echo "$example is done" > tmp_$$.txt; /usr/lib/sendmail knuteson@fnal.gov < tmp_$$.txt; rm tmp_$$.txt;
#####################

