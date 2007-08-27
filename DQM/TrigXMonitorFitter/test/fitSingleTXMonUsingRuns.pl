#!/usr/bin/env perl

use strict;
use warnings;


my $directory = "./";
my @dirs = qw (./txmon_new ./txmon_old);
my $prefix = "XMon_xsec";


(my $prog = $0) =~ s|.+/||g;
my $help = "Usage: $prog [-flags] triggername [123456]
Options:
-b basename       => use 'basename' to save fit results
-f filename       => use the Run Numbers in 'filename'
-h                => this Help screen
-nc               => do Not Convert eps to gifs
-param 1 2 3 4 5  => use 1 2 3 4 5 as the fit PARAMeters (p0..p3, error)
";

my ($filename, $base, $noconvert);
my @params = ();
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
    if ($arg =~ /^-param/i)
    {
        # the next five arguments belong to the parameters
        for (1..5)
        {
            push @params, shift @ARGV;
        }
        next;
    }
    if ($arg =~ /^-b/i)
    {
        $base = shift;
        next;
    }
    if ($arg =~ /^-nc/i)
    {
        $noconvert = "true";
        next;
    }
    warn "I don't understand '$arg'.\n";
}


my $triggername = shift @ARGV || "empty";
$triggername =~ s/_and_/\&/g;

if ($base && $base !~ m|^/|)
{
    $base = $ENV{PWD}."/$base";
}

if (! $base)
{
    ($base = $triggername) =~ s/\&/_and_/g;
    $base .= ".$$";
    $base = "./singles/$base";
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
    open (SOURCE, $filename) or die "Can't open $filename for input\n";
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

@runs = reverse sort {$a <=> $b} @runs;

die "Usage: $0 Triggername 123456 [123457] ...\n" unless (@runs);

chdir $directory;

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
} # foreach Run

print "files:\n",join("\n", @rootfiles),"\n";

my $fit = "";
if (@params)
{
    $fit = "-f @params";
}

if (@rootfiles)
{
    my $command = "./SingleTXMonFitter.exe \"$triggername\" "
        ."$base $fit @rootfiles";
    print $command,"\n";
    system $command;
    if (!$noconvert)
    {
	system "./convertAllEps.pl -m $base -r --dpi=50";
    }
}


