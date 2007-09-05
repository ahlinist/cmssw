#!/usr/bin/perl

# Replace stable particles in Pythia setup

$workdir = ".";

$originalcfg = "NoInhibDecay.cfg";
$finalcfg = "InhibDecay.cfg";
$partlist = "/afs/cern.ch/user/c/covarell/scratch0/evtgen/CMSSW_1_3_1/src/GeneratorInterface/EvtGenInterface/data/kclund_partlist.txt";

# create cfg file
system("cp ${workdir}/$originalcfg ${workdir}/$finalcfg");

open(PARTLIST,"$partlist") or die "cannot open $partlist";;
@part=<PARTLIST>;
close(PARTLIST);
$repl="";

# Check not to double commands
$previous = 0;

foreach $number (@part) {
  my @pFields = split( /\s+/, $number );
  if ($pFields[2] > 0 && $pFields[2] != $previous) {
      $repl=$repl . "       'MDCY(" . $pFields[2] . ",1) = 0', \n ";
      $previous = $pFields[2] 
  }
}
 
open(INFILE,"$finalcfg") or die "cannot open $finalcfg";;
@log=<INFILE>;
close(INFILE);

system("rm -f tmp");
open(OUTFILE,">tmp");

foreach $line (@log) {
  if ($line =~ /REPLACEME/) { print OUTFILE $repl; }
  else { print OUTFILE $line; }
}

close(OUTFILE);
system("mv tmp $finalcfg");

