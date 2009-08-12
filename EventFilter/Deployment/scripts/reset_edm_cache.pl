#!/usr/bin/env perl
use Env;
($#ARGV >= 2) || die "Usage::reset_edm_cache.pl libdir moddir";
$libdir = @ARGV[0];
$moddir = @ARGV[1];
$envfile = @ARGV[2];
my $sourceme = `scramv1 runtime -sh`;
print "---------------------------------------------\n";
print "$sourceme\n";
print "---------------------------------------------\n";
@tok1 = split(/\n/,$sourceme);
while($#tok1 != -1)
{
    my $env =  @tok1[$#tok1];
    $env =~ s/export (.+)/$1/;
    $env =~ s/SCRAMRT_([^";]+)/$ENV{$1}/;
    $env =~ s/;//;
    $env =~ s/"//g;
    $env =~ /(.*)=(.*)/;
#    print "setting $1 to $2\n";
    $ENV{$1}=$2;
    pop(@tok1);
}

$ENV{LD_LIBRARY_PATH}=$libdir;
$ENV{'SEAL_PLUGINS'}=$moddir;
#$ENV{LOG}='stderr';
print "working on $ENV{LD_LIBRARY_PATH}\n";
print "removing $libdir/.edmplugincache\n";
my $resp = `rm -f $libdir/.edmplugincache`;
print "$resp\n";
print "here 1\n";
$resp = `EdmPluginRefresh`;
print $resp;
