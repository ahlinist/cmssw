#!/usr/bin/env perl

use strict;
use warnings;



my @dirs = qw (./txmon_new ./txmon_old);
my $prefix = "TXMon_txsec";
# set prefix2 to prefix1 and delete the original prefix 1 when ready


(my $prog = $0) =~ s|.+/||g;
my $help = "Usage: $prog [-flags]
Options:
-f filename       => use the Run Numbers in 'filename'
-h                => this Help screen
-nc               => do Not Convert eps to gifs
";



my ($filename, $noconvert);
while (@ARGV && $ARGV[0] =~ /^-/)
  {
    my $arg = shift @ARGV;

    if ($arg =~ /^-{1,2}h/i)
    {
        die $help;
    }

    if ($arg =~ /^-f/i)
      {
        $filename = shift;
        next;
      }
    if ($arg =~ /^-nc/i)
      {
        $noconvert = "true";
        next;
      }
    warn "I don't understand '$arg'.\n";
  }




my @runs;
foreach (@ARGV)
  {
    if (/^\d{6,}$/)
      {
        push @runs, $_;
      } # if digits
  } # foreach argv





if ($filename)
  {
    open (SOURCE, $filename) or die;
    while (<SOURCE>)
      {
        chomp;
        s/\#.+//;
        s/\s+//g;
        if (/^\d{6,}$/)
	  {
            push @runs, $_;
	  }
      } # while source
  } # if filename

die "Usage: $0 123456 [123457] ...\n" unless (@runs);



@runs = reverse sort {$a <=> $b} @runs;
print "runs @runs\n";
my @rootfiles;
foreach my $run (@runs)
  {
    foreach my $dir (@dirs)
      {
        my @files = glob ("$dir/$prefix$run*root");
        if (@files)
	  {
            push @rootfiles, @files;
            last;
	  } # if found
      } # foreach dir
  } # foreach run



print "files:\n",join("\n", @rootfiles),"\n";
if (@rootfiles)
  {
    my $convert;
    if (!$noconvert)
      {
        $convert = "; ./makeImgs.tcsh";
      }

    my $htmlcommand = "; ./moveHtml.pl ; ./copyRoot.pl -f $filename";

    my $command = "(./AllTXMonFitter.exe @rootfiles $htmlcommand $convert) >& all.out &";
    print $command,"\n";
    system $command;

  }












