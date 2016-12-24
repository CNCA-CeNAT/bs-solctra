#!/usr/bin/perl



use strict;
use warnings;
use Getopt::Long qw(GetOptions);


my $column_width = 4;


my $log_file;
my $out;
GetOptions('log=s' => \$log_file, 'out=s' => \$out) or die "Usage: $0 -log LOG FILE -out OUTPUT\n";
if (!$log_file)
{
    die "-log is empty!!";
}
if (!$out)
{
    die "-out is empty!!";
}

my @file_content = `cat $log_file`;

my $bounding = {};
my $bounding_reverse = {};
my $mapping = {};
my $procs = {};
my @threads_per_core = ();
push(@threads_per_core, 0);
push(@threads_per_core, 1);
push(@threads_per_core, 2);
push(@threads_per_core, 3);
my $current_key = "A";
my $pids = {};

foreach my $current_line (@file_content)
{
	if($current_line =~ /OMP: Info #\d+: KMP_AFFINITY: pid (\d+) thread (\d+) bound to OS proc set \{(\d+(,\d+)?)\}/)
	{
		my $pid = $1;
		my $pid_thread = $2;
		my $proc = $3;
		if(!defined($bounding->{$pid}))
		{
			$bounding->{$pid} = {};
                        $pids->{$pid} = $current_key++;
		}
		$bounding->{$pid}->{$pid_thread} = $proc;
                $bounding_reverse->{$proc} = [];
		push(@{$bounding_reverse->{$proc}}, $pid);
		push(@{$bounding_reverse->{$proc}}, $pid_thread);
	}
	elsif($current_line =~ /OMP: Info #\d+: KMP_AFFINITY: OS proc (\d+) maps to package \d core (\d+) thread (\d+)/)
	{
		my $proc = $1 - 0;
		my $core = $2 - 0;
		my $thread = $3 - 0;
		if(!defined($mapping->{$core}))
		{
			$mapping->{$core} = {};
		}
		$mapping->{$core}->{$thread} = $proc;
		$procs->{$proc} = [];
		push(@{$procs->{$proc}}, $core);
		push(@{$procs->{$proc}}, $thread);
#                print "$core,$thread,$proc\n";
	}
}

open(my $fh, ">$out/thread_mapping.csv") || die "CANNOT open $out/thread_mapping.csv";
open(my $fh_per_pid, ">$out/thread_mapping_per_pid.csv") || die "CANNOT open $out/thread_mapping.csv";

my $thread_per_rank_table;
printf "|%2s|", " ";
$thread_per_rank_table = sprintf "|%2s|", " ";
foreach my $core (sort {$a <=> $b} keys %{$mapping})
{
	printf "%${column_width}s|", $core;
	$thread_per_rank_table .= sprintf "%${column_width}s|", $core;
        print $fh ",$core";
        printf $fh_per_pid ",%04s", $core;
}
printf "\n====";
printf "=====" x (scalar keys %{$mapping});
$thread_per_rank_table .= "\n====";
$thread_per_rank_table .= "=====" x (scalar keys %{$mapping});

foreach my $thread (sort @threads_per_core)
{
	printf "\n|%2s|", $thread;
	$thread_per_rank_table .= sprintf "\n|%2s|", $thread;
        print $fh "\n$thread";
        print $fh_per_pid "\n$thread";
	foreach my $core (sort {$a <=> $b} keys %{$mapping})
	{
	        printf "%${column_width}s|", $mapping->{$core}->{$thread};
	        printf $fh ",$mapping->{$core}->{$thread}";
		if(defined($bounding_reverse->{$mapping->{$core}->{$thread}}) && @{$bounding_reverse->{$mapping->{$core}->{$thread}}} == 2)
		{
                	my $pid = $bounding_reverse->{$mapping->{$core}->{$thread}}->[0];
                	my $pid_thread = $bounding_reverse->{$mapping->{$core}->{$thread}}->[1];
	        	printf $fh_per_pid ",$pids->{$pid}%03s", $pid_thread;
	        	$thread_per_rank_table .= sprintf "$pids->{$pid}%03s|", $pid_thread;
		}
		else
		{
			printf $fh_per_pid ",X000";
			$thread_per_rank_table .= sprintf "X000|";
		}
	}
}

print "\n";
print "\n";
print "\n";
print $thread_per_rank_table;

close($fh);
close($fh_per_pid);
