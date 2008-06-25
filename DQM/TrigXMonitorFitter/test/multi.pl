#!/usr/bin/perl -w

$flag = "%";

if (!@ARGV)
  {
    $prog = $0;
    $prog =~ s|.*/||g;
    print qq(Usage: $prog command oldname$flag newname$flag

ex:
$prog mv $flag.txt $flag.txt.old);
    die "\n";
  }

$command = "@ARGV";
for ($loop = 0; ($loop < @ARGV) && ($ARGV[$loop] !~ /$flag/); ++$loop) {}

if ($loop == @ARGV)
  {
    die "you need to use $flag somewhere\n";
  }	

$ARGV[$loop] =~ /$flag/;
$pre = $`;
$post = $';
$search =  "$pre*$post";
@list = glob($search);
@fixed = ();

foreach $name (@list)
  {
    if ($name =~ /^$pre([\S]*)$post$/)
      {
	push @fixed, $1;
      }
    else
      {
	print "Help: $name\n";
      }	
  }

foreach $name (@fixed)
  {
    $newcmd = $command;
    $newcmd =~ s/$flag/$name/g;
    print "unix> $newcmd\n";
    print `$newcmd`,"\n";;
  }
