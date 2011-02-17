#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all);
use POSIX;

my $count = 50000000;

cmpthese($count, {
        'string concat' => \&sc,
        'array join' => \&aj,
});


sub sc {
	'2000'.'-'.'02'.'-'.'04';
}

sub aj {
	join('-', ('2000', '02', '04'));
}
