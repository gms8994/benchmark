#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all);
use JSON::PP;
use JSON::XS;
use POSIX;

my $count = 6000;

my @data = ();
for (my $i = 32; $i <= 126; $i++) {
	$data[$i] = [];
	$data[$i][0] = chr($i)x$i;
}

cmpthese($count, {
	'json-encode-xs' => \&json_encode_xs,
	'json-encode-pp' => \&json_encode_pp
});

sub json_encode_xs {
	JSON::XS::encode_json(\@data);
}

sub json_encode_pp {
	JSON::PP::encode_json(\@data);
}
