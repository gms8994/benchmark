#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all);
use POSIX;

my $count = 50000000;

cmpthese($count, {
        'sprintf' => \&sprintf_test,
        'POSIX::floor' => \&posix_test,
});


sub sprintf_test {
    sprintf('%d', rand(10) / rand(3));
}

sub posix_test {
    POSIX::floor(rand(10) / rand(3));
}
