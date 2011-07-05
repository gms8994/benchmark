#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all);
use POSIX;

my $count = 50000000;
my $rand = rand(10);

cmpthese($count, {
        'if' => \&if_test,
        'post-if' => \&postif_test,
});


sub if_test {
    if ($rand > 2) { return; }
}

sub postif_test {
	return if $rand > 2;
}
