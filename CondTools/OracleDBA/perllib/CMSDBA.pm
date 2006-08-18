#!/usr/bin/perl

package CMSDBA;

use XML::Simple qw(:strict);
use POSIX qw(strftime);

sub parse_authentication {
    my $authfile = shift @_;
    my $auth = XMLin( $authfile,
		      forcearray => [qw(connection parameter)],
		      keyattr => { connection => 'name', parameter => 'name' }
		      );

    my $newauth = {};
    foreach my $connect (keys %{$auth->{connection}}) {
	my $user = $auth->{connection}->{$connect}->{parameter}->{user}->{value};
	my $pass = $auth->{connection}->{$connect}->{parameter}->{password}->{value};
	
	$newauth->{$connect}->{user} = $user;
	$newauth->{$connect}->{pass} = $pass;
    }



    return $newauth;
}


sub parse_dbconfiguration {
    my $configfile = shift @_;
    my $config = XMLin( $configfile,  
			forcearray => [qw(detector)], 
			keyattr => {detector => 'name'},
			);
    return $config;
}

sub parse_o2oconfiguration {
    my $configfile = shift @_;
    my $config = XMLin( $configfile, 
			forcearray => [qw(object detector)], 
			keyattr => [qw(name)]
			);
    return $config;
}

sub get_sqlplus_cmd {
    my %args = @_;

    if  (!($args{user} && $args{pass} and $args{db})
	 || (exists $args{sql} && exists $args{file}) ) {
	return 0;
    }

    my $sqlplus = "sqlplus -L -S $args{user}/$args{pass}\@$args{db}";
    
    if (exists $args{file}) {
	$sqlplus = "cat $args{file} | ".$sqlplus;
    } elsif (exists $args{sql}) {
	$sqlplus = "echo \"".$args{sql}.";\" | ".$sqlplus;
    }
   
    return $sqlplus;
}



sub check_db_connection {
    my %args = @_;
    if  (!(exists $args{user} && exists $args{pass} and exists $args{db})) {
	return 0;
    }

    unless (my $sqlplus = get_sqlplus_cmd(%args)) {
	return "ERROR: check_db_connection had bad parameters";
    }

    my $result = `echo 'exit;' | $sqlplus 2>&1;`;
    if ($?) {
	return $result;
    } else { return 0; }
}



sub connection_test {
    my ($auth, $connect) = @_;

    my ($db) = ($connect =~ m'^oracle://(\w+)/\w+$');
    unless ($db) {
	die "Connect string $connect is malformed\n";
    }

    # Parse out the login info
    unless (exists $auth->{$connect}) {
	die "Connection $connect is not defined in $authfile\n";
    }

    my $user = $auth->{$connect}->{user};
    my $pass = $auth->{$connect}->{pass};

    print "[INFO]   Setting up connection $connect\n";

    # Check the connection
    print "[INFO]   Checking connection...";
    my $errormsg = check_db_connection('user' => $user, 'pass' => $pass, 'db' => $db);
    if ($errormsg) {
	print "\nConnection Test Result:  \"".$errormsg."\"\n\n";
	die "Connection to $connect with sqlplus failed.  Check the parameters in authentication.xml";
    } else {
	print "OK\n";
    }

    return ($user, $pass);
}



sub check_files {
    foreach (@_) {
	die "Needed file $_ does not exist" unless -e $_;
    }
}


sub execute_commands {
    my %args = @_;

    if (!exists $args{cmd_array}) {
	return 0;
    }
    
    my $fake = 0;
    if (exists $args{fake}) { $fake = $args{fake}; }
    
    my $debug = 0;
    if (exists $args{debug}) { $debug = $args{debug}; }

    my $log = 0;
    if (exists $args{log}) { $log = $args{log}; }

    my $totaltime = 0;
    
    my $status = 0;
    open LOG, '>>', $log if $log;
    foreach (@{$args{cmd_array}}) {
	print "[INFO]   ".$_->{info}."...";
	print "\n[DEBUG]  ".$_->{cmd}."\n" if $debug;
	print LOG strftime("%Y-%m-%d %H:%M:%S", localtime()), " ",$_->{info} if $log;
	my $t1 = time();
	my $output = `$_->{cmd} 2>&1` unless $fake;
	my $status = $?;
	my $dt = time() - $t1;
	$totaltime += $dt;
	$dt = "($dt s)";
	if ($status) {
	    my $msg = "[ERROR]  Previous command returned error.  Stopping.";
	    my $report = "Command:\n". $_->{cmd}. "\nOutput:\n". $output;
	    warn $msg, "\n", $report, "\n";
	    print LOG strftime("%Y-%m-%d %H:%M:%S", localtime()), 
	    " ", $msg, "\n", $report, "\n", $dt, "\n" if $log;
	    return 0;
	} else {
	    print $output, "\n" if $debug;
	    print "OK $dt\n";
	    print LOG " $dt\n" if $log;
	}
    }
    close LOG if $log;

    print "[INFO]   Total time: $totaltime s\n";

    return 1;
}

1;
