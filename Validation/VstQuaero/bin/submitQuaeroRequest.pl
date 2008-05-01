#!/usr/bin/perl5.6.1
###############################################
# Submit Quaero request
# Return immediately unless resultFileName is given, 
#   in which case block until Quaero has finished, 
#   and put logL in resultFileName
#
# Usage:  perl submitQuaeroRequest.pl <QuaeroEdition> <InputType> <InputFileName> \
#                                     <ModelDescription> <TargetTime> <Password> [resultFileName]
###############################################

require LWP::UserAgent;
use HTTP::Request::Common;
use HTTP::Cookies;

my $ctServer = "http://fcdfwww.fnal.gov/";
my $ctAction = "/cgi-bin/$ARGV[0].py";
my $ua = LWP::UserAgent->new();

$req = HTTP::Request::Common::POST($ctServer.$ctAction, 
				      Content_Type => 'form-data',
				      Content => [
						  "inputType_all" => "$ARGV[1]",
						  "inputFile_all" => [$ARGV[2]],
						  RequestorEmail => "knuteson\@mit.edu",
						  ModelDescription => "$ARGV[3]",
						  targetTime => "$ARGV[4]",
						  Password => "$ARGV[5]"
						  ]     );

my $r = $ua->request($req);
($reqID) = ($r->content =~ /Quaero\srequest\s\#(\d+)/);
print "$reqID\n";
`sleep 2`;

if(($ARGC eq 7) and ($ARGV[6] ne ""))
{
    $done = 0;
    $res = "";
    while (!$done)
    {
	$url = "http://mit.fnal.gov/~knuteson/Quaero/${edition}/requests/${reqID}/answer.txt";
	$logL = `links -source $url | grep -i 'log10' | awk '{print $13}'`;
	$failed = `links -source $url | grep -i 'failed'`;
	if (($res ne "") or ($failed ne ""))
	{
	    $done=1;
	}
	unless ($done) { `sleep 30`;} ;
    }
    open(FOUT,">${ARGV[6]}");
    print FOUT "${res}\n";
}



















