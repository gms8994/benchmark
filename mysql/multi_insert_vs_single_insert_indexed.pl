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
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name}_indexed (field_1 int, field_2 int, field_3 text, INDEX (field_1), INDEX (field_2))");

my $insert_string = "(1, 1, 'this is some random text that shall be inserted')";
my $sql_base = "INSERT INTO ${table_name} (field_1, field_2, field_3) VALUES ";
my $sql_base_indexed = "INSERT INTO ${table_name}_indexed (field_1, field_2, field_3) VALUES ";

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

sub insert_one_indexed {
    $dbh->do("TRUNCATE TABLE ${table_name}_indexed");
    for(my $i = 0; $i < abs($repeat); $i++) {
        $dbh->do($sql_base_indexed . $insert_string);
    }
}

sub insert_multiple_indexed {
    $dbh->do("TRUNCATE TABLE ${table_name}_indexed");
    my $multi_insert_string = ($insert_string . ", ") x abs($repeat);
    $multi_insert_string =~ s/, $//;

    $dbh->do($sql_base_indexed . $multi_insert_string);
}

cmpthese($repeat, {
    'Insert One' => 'insert_one',
    'Insert Multiple' => 'insert_multiple',
    'Insert One (Indexed)' => 'insert_one_indexed',
    'Insert Multiple (Indexed)' => 'insert_multiple_indexed'
});

$dbh->disconnect();
