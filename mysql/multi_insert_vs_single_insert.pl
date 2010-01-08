#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all :hireswallclock);
use DBI;

my $table_name = $0;
$table_name =~ s/^.*\///;
$table_name =~ s/\.pl//;

my $repeat = 2600;

my $dbh = DBI->connect("dbi:mysql:benchmark:localhost", "user", "password");
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name} (field_1 int, field_2 int, field_3 text)");

my $insert_string = "(1, 1, 'this is some random text that shall be inserted')";
my $sql_base = "INSERT INTO ${table_name} (field_1, field_2, field_3) VALUES ";

sub insert_one {
    $dbh->do("TRUNCATE TABLE ${table_name}");
    for(my $i = 0; $i < abs($repeat); $i++) {
        $dbh->do($sql_base . $insert_string);
    }
}
sub insert_multiple {
    $dbh->do("TRUNCATE TABLE ${table_name}");
    my $multi_insert_string = ($insert_string . ", ") x abs($repeat);
    $multi_insert_string =~ s/, $//;

    $dbh->do($sql_base . $multi_insert_string);
}

cmpthese($repeat, {
    'Insert One' => 'insert_one',
    'Insert Multiple' => 'insert_multiple'
});

$dbh->disconnect();
