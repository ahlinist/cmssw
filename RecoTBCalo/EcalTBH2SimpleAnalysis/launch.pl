#!/usr/bin/perl  

use Cwd;
$TBPATH = getcwd;
print "working in: ".$TBPATH."\n" ;
$LISTA = $ARGV[0];
$CONFDIR = $ARGV[1];
$OUTDIR = $ARGV[2];
print $LISTA."\n" ;

@counter = 0;

#inizia il loop sui run
open STUFF, $LISTA or die "Cannot open $LISTA to read the run numbers :$!";
while (<STUFF>)
{
    @linea = $_;
    $counter ++ ;

    $RUN = $linea[0] ;
    $RUN =~s/\n// ;
    print ("analizing the run number: $RUN\n") ;
    print ("-------------------------------\n") ;

    $NUM = (split /\.000/, $RUN)[1];
    $NUM = (split /\.root/, $NUM)[0];
    $CONFIGFILEName = $TBPATH."/$CONFDIR/config".$NUM.".cfg" ;
    $OUTPUTROOTFILE = "\$ENV{WORKDIR}/".$NUM.".root";
    $OUTPUTFILE = $OUTDIR."/".$NUM.".log";
    $OUTPUTFILE2 = "\$ENV{WORKDIR}/out_".$NUM.".log";
    $SCRIPTName = "$TBPATH/script/script_$NUM.pl";
    print "$CONFIGFILEName\n";
    print "$OUTPUTROOTFILE\n";
    print "$OUTPUTFILE\n";
    print "$OUTPUTFILE2\n";
    print "$SCRIPTName\n";

    open (SCRIPT, ">".$SCRIPTName) or die "Can't open config file ".$SCRIPTName." for output: $!" ;
    print SCRIPT "#!/usr/bin/perl\n";
    print SCRIPT "\n";
    print SCRIPT "system(\"cp $TBPATH/prefile.txt $CONFIGFILEName\");\n";
    print SCRIPT "open (CONFIGFILE, \">> $CONFIGFILEName\") or die \"Can't open config file $CONFIGFILEName for output: \$\!\" ;\n";
    print SCRIPT "print CONFIGFILE \"        untracked vstring fileNames = { 'rfio:$RUN'}\n\";\n";
    print SCRIPT "system(\"cat $TBPATH/middlefile.txt >> $CONFIGFILEName\");\n";
    print SCRIPT "print CONFIGFILE \"	untracked string rootfile = \\\"$OUTPUTROOTFILE\\\"\n\";\n";
    print SCRIPT "system(\"cat $TBPATH/postfile.txt >> $CONFIGFILEName\");\n";
    print SCRIPT "system(\"cmsRun $CONFIGFILEName > $OUTPUTFILE2 \");\n";
    print SCRIPT "system(\"rfcp $OUTPUTROOTFILE  /castor/cern.ch/user/d/delre/TBH206_pi0new_cal\");\n";
    print SCRIPT "system(\"rfcp $OUTPUTFILE2  /castor/cern.ch/user/d/delre/TBH206_pi0new_cal\");\n";

    system("chmod a+x $SCRIPTName");

    system("bsub -q 8nh -o $OUTPUTFILE $SCRIPTName");

} # loop sui run

