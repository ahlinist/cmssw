
# run like

# perl analysisBot.pl <low-weight> <high-weight> <Nsteps> <tag> <signalfile>

# signal file should be a normal quaero file, but with the word "weight" instead of the weight

$low = $ARGV[0];
$high = $ARGV[1];
$nsteps = $ARGV[2];

$tag = $ARGV[3];
$file = $ARGV[4];

$newWeight = $low;
$weightStep = ($high-$low)/($nsteps-1);


$noZ = ($ARGV[5] eq "noz");
$noWW = ($ARGV[5] eq "noww");
$noTop = ($ARGV[5] eq "notop");

printf " No top? $noTop No WW?  $noWW No Z? $noZ\n";


for ($i = 0;$i < $nsteps;$i++)
{

    $inFile = $file;


    printf "From file $inFile\n";
#    $inFile = $file.".split_".$i;

    $newFile = $file."_".$tag."_".$i;

    print "Step $i weight = $newWeight file = $newFile\n";

    $stepVal[$i] = $newWeight;
    $stepFile[$i] = $newFile;

    $fdata = `cat $inFile`;
    $fdata =~ s/weight/$newWeight/g;
        
    open(FILE,">$newFile");
    print FILE $fdata;
    close(FILE);
   
    $newWeight += $weightStep;
}



for ($i = 0;$i < $nsteps;$i++)
{

    if ($noTop) 
    { $reqRes = `perl quaeroBotNoTop.pl $stepFile[$i] $tag 1`; }
    else { if ($noZ) { $reqRes = `perl quaeroBotNoZ.pl $stepFile[$i] $tag 1`; } 
	   else { if ($noWW) { $reqRes = `perl quaeroBotNoWW.pl $stepFile[$i] $tag 1`;}
		  else { $reqRes = `perl quaerobot.pl $stepFile[$i] $tag 1`; } } }
    ($reqID) = ($reqRes =~ /Request\s(\d+)/);
    $stepReq[$i] = $reqID;
    print "File $stepFile[$i] Step $stepVal[$i] request $stepReq[$i]\n";
    $stepDone[$i] = 0;
}

$allDone = 0;

for ($i = 0;$i < $nsteps;$i++)
{
    $stepDone[$i] = 0;
}



print "All done? $allDone\n";
while (!$allDone)
{
	print "============================\n";
	print "Scanning for results...";
    for ($i = 0;$i < $nsteps;$i++)
    {

	print "[ Step $i status $stepDone[$i] ";
	if (!$stepDone[$i])
	{

	    $url = "http://hep.uchicago.edu/cdf/Knuteson/Quaero/quaero/requests/$stepReq[$i]/requests/$stepReq[$i]";
	   # print "Querying step $i @ $url\n";
	    $reqRes = `lynx -source $url`;
	  #  print "Res: $reqRes\n\n";


	    ($res) = ($reqRes =~ /model\)\s\]\s\=\s(.*)/);

	    if ($res ne "")
	    {
		$stepDone[$i]= 1;
		$stepRes[$i] = $res;
		print " res = $stepRes[$i] ] ";
	    }
	    else
	    {
		print " no result ] "
	    }
	}
	
    }
	
	print " ( Status: ";
    $allDone = 1;
    for ($i = 0;$i < $nsteps;$i++)
    {
	$allDone *= $stepDone[$i];
	print "$stepDone[$i] ";
    }
    print " =>  $allDone)\n";

    unless ($allDone) { `sleep 30`;}
 
}

print "All results completed\n";


for ($i = 0;$i < $nsteps;$i++)
{
 
    print "File $stepFile[$i] Step $stepVal[$i] request $stepReq[$i] result $stepRes[$i]\n";
 
}

open(FOUT,">$tag.C");

print FOUT " void ${tag}()\n{\n";
print FOUT "double steps[$nsteps] = {";


for ($i = 0;$i < $nsteps;$i++)
{
    print FOUT $stepVal[$i];
    if ($i+1 != $nsteps) { print FOUT ", " };
}

print FOUT "};\n";

print FOUT "double res[$nsteps] = {";


for ($i = 0;$i < $nsteps;$i++)
{
    print FOUT $stepRes[$i];
    if ($i+1 != $nsteps) { print FOUT ", " };
}

print FOUT "};\n";

print FOUT " TGraphAsymmErrors *g = new TGraphAsymmErrors($nsteps);\n";

print FOUT " for (int i = 0;i < $nsteps;i++)\n{\n";
print FOUT " g->SetPoint(i,steps[i],res[i]);\n }\n";
print FOUT " g->SetTitle(\"Quaero results for file $file, tag $tag\");\n";

print FOUT " g->SetMarkerStyle(22);\n g->SetMarkerStyle(4);\n";
print FOUT " TCanvas *cg = new TCanvas(\"cg\",\"\",400,400);\n";
print FOUT " g->Draw(\"ALP\");\n";
print FOUT " g->GetXaxis()->SetTitle(\"Weight\");\n ";
print FOUT " g->GetYaxis()->SetTitle(\"log10(L)\");\n ";
print FOUT " cg->Print(\"${tag}.ps\");}\n";


print "Script written to ${tag}.C . Running... \n";
$rootres = `root -q -l -b ${tag}.C`;
print $rootres;


`cp ${tag}.ps /home/danielw/public_html/quaero/`;
`cd /home/danielw/public_html/quaero/; ./makeIndex.pl`;













































