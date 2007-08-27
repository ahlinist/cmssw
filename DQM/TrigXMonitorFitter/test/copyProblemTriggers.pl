#!/usr/bin/env perl

use strict;
use warnings;

my @matches;
my $moveThings = "";
while (@ARGV)
  {
    my $arg = shift @ARGV;
    if ($arg =~ /^-/)
      {
	if ($arg =~ /^-m/i)
	  {
	    $moveThings = "true";
	  }
	next;
      }
    push @matches, $arg if ($arg =~ /\w/);
  }

die "Nothing to match\n" unless (@matches);

my @dirs = qw (L1 L2 L3 Ln);
my $base = "./";
my @toBeMoved;
foreach my $dir (@dirs)
  {
    chdir "$base/$dir";
    my @files = glob "*.html";
    foreach my $file (@files)
      {
	$file =~ s/\.html$//;
	foreach my $match (@matches)
	  {
	    if ($file =~ /$match/i)
	      {
		push @toBeMoved, "$dir/$file";
		last;
	      }
	  }
      }
  } # foreach dir

print "matched\n", join("\n", @toBeMoved),"\n\n";

my $problemDir = "./ProblemTriggers/bad";

if ($moveThings)
  {
    chdir $base;
    foreach my $move (@toBeMoved)
      {
	system "mv $move* $problemDir/";
      }
    system "./makeHtml";
    system "cd ProblemTriggers; ./makeHtml";
  }
