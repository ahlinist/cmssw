########################################################
#
#  drawLogLcurve.pl
#
#  Bruce Knuteson, modified from Daniel Whiteson danielw@fnal.gov, 2003
#
#  run like
#  perl drawLogLcurve.pl <requestNumberStart> <nRequests> <example> <sm_x> <axis label>


$edition = "quaero_development";
$nDuplicates = 1;
$requestNumberStart = $ARGV[0];
$nRequests = $ARGV[1];
$example = $ARGV[2];
#LEP II doubly charged Higgs = 98.5
#LEP II charged Higgs = 87.0
#HERA I doubly charged Higgs = 134
#standard model Higgs = 114.0
#mSugra = 150
#H1 single top = 0.25
$sm_x = $ARGV[3];
$xAxisLabel = $ARGV[4];

print LOUT "============================\n";
print LOUT "Scanning for results...";
$n=0;
    for ($i=0; $i<$nRequests; $i++)
    {
	$requestNumber = $requestNumberStart + $i;
	$url = "http://mit.fnal.gov/~knuteson/Quaero/$edition/requests/$requestNumber/answer.txt";
	$x = `links -source $url | grep log10 | awk '{print \$13}'` + 0.001;
	print "$x\t";
	if ( $x != "" ) 
	{ 
	    $xValue[$i] += `links -source $url | grep -i $example | sed -e 's/.*'$example' (//' | sed -e 's/,.*)(.*,.*)//'`;
	    $results{$xValue[$i]} += $x;
	    $nTrials{$xValue[$i]}++;
	    $url = "http://mit.fnal.gov/~knuteson/Quaero/$edition/requests/$requestNumber/.answer.txt";
	    $sensitivityH{$xValue[$i]} += `links -source $url | tail +2 | awk '{print \$4}' | perl -ne 'BEGIN { \$sum = 0; } chomp; \$sum += \$_; END { print "\$sum\n"; }'`;	    
	    $sensitivitySM{$xValue[$i]} += `links -source $url | tail +2 | awk '{print \$6}' | perl -ne 'BEGIN { \$sum = 0; } chomp; \$sum += \$_; END { print "\$sum\n"; }'`;	    
	}
	else
	{
	    print "$requestNumber is empty"
	}
    }
foreach $x (keys(%nTrials)) {
	print "$ntrials ";
	$results{$x} /= $nTrials{$x};
	$sensitivityH{$x} /= $nTrials{$x};
	$sensitivitySM{$x} /= $nTrials{$x};
	$n++;
}
print "\n";
	
open(FOUT,">logL_$$_temp.C");
$settings = `cat SetStyle.C`;

print FOUT " void prodplot_$$()\n{\n";
print FOUT $settings;

print FOUT "double steps[$n] = {";

foreach $x (sort { $a <=> $b } keys(%nTrials)) {
    print FOUT "$x, ";
}	
print FOUT "};\n";
print FOUT "double res[$n] = {";		
foreach $x (sort { $a <=> $b } keys(%nTrials)) {
    print FOUT "$results{$x}, ";
}	
print FOUT "};\n";
print "\n";
print FOUT "double sensitivityH[$n] = {";		
foreach $x (sort { $a <=> $b } keys(%nTrials)) {
    print FOUT "$sensitivityH{$x}, ";
}	
print FOUT "};\n";
print FOUT "double sensitivitySM[$n] = {";		
foreach $x (sort { $a <=> $b } keys(%nTrials)) {
    print FOUT "$sensitivitySM{$x}, ";
}	
print FOUT "};\n";
print FOUT "const int Npoints = $n;\n";

#print FOUT "  TGraphAsymmErrors *g = new TGraphAsymmErrors(Npoints);\n";
#print FOUT "  g->GetXaxis()->SetTitle(\"$xAxisLabel\");\n";

close(FOUT);

`cat plotScriptAnal.C >> logL_$$_temp.C`;

$fdata = `cat logL_$$_temp.C`;
$fdata =~ s/MODEL/model/g;
$fdata =~ s/TAG/tag/g;
$fdata =~ s/SMVALUE/$sm_x/g;
$fdata =~ s/XAXISLABEL/$xAxisLabel/g;

open(FILE,">prodplot_$$.C");
print FILE $fdata;
close(FILE);
	
$rootres = `root -q -l -b prodplot_$$.C`;
print $rootres;










































