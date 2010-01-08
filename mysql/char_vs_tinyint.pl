#!/usr/bin/perl

use warnings;
use strict;

use Benchmark qw(:all :hireswallclock);
use DBI;

my $table_name = $0;
$table_name =~ s/^.*\///;
$table_name =~ s/\.pl//;

my $repeat = 300;
my $runs = 5;

my $dbh = DBI->connect("dbi:mysql:benchmark:localhost", "user", "password");
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name}_ti (field_1 char(1), INDEX (field_1))");
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name}_ci (field_1 tinyint(1), INDEX (field_1))");
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name}_ei (field_1 enum('y', 'n'), INDEX (field_1))");
$dbh->do("CREATE TABLE IF NOT EXISTS ${table_name}_bi (field_1 boolean, INDEX (field_1))");

my $tth = $dbh->prepare("INSERT INTO ${table_name}_ti (field_1) VALUES (?)");
my $cth = $dbh->prepare("INSERT INTO ${table_name}_ci (field_1) VALUES (?)");
my $eth = $dbh->prepare("INSERT INTO ${table_name}_ei (field_1) VALUES (?)");
my $bth = $dbh->prepare("INSERT INTO ${table_name}_bi (field_1) VALUES (?)");

my $tth1 = $dbh->prepare("SELECT * FROM ${table_name}_ti WHERE field_1 = ? LIMIT 10");
my $cth1 = $dbh->prepare("SELECT * FROM ${table_name}_ci WHERE field_1 = ? LIMIT 10");
my $eth1 = $dbh->prepare("SELECT * FROM ${table_name}_ei WHERE field_1 = ? LIMIT 10");
my $bth1 = $dbh->prepare("SELECT * FROM ${table_name}_bi WHERE field_1 = ? LIMIT 10");

sub tinyint {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $tth->execute( rand() > 0.50 ? 1 : 0 );
    }
}

sub char {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $cth->execute( rand() > 0.50 ? 'y' : 'n' );
    }
}

sub enum {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $eth->execute( rand() > 0.50 ? 'y' : 'n' );
    }
}

sub bool {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $bth->execute( rand() > 0.50 ? 1 : 0 );
    }
}

sub tinyint_s {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $tth1->execute( rand() > 0.50 ? 1 : 0 );
    }
}

sub char_s {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $cth1->execute( rand() > 0.50 ? 'y' : 'n' );
    }
}

sub enum_s {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $eth1->execute( rand() > 0.50 ? 'y' : 'n' );
    }
}

sub bool_s {
    for(my $i = 0; $i < abs($repeat); $i++) {
        $bth1->execute( rand() > 0.50 ? 1 : 0);
    }
}

for(my $i = 0; $i < $runs; $i++) {
    cmpthese($repeat, {
        'insert tinyint(1)' => 'tinyint',
        'insert char(1)' => 'char',
        'insert enum(\'y\', \'n\')' => 'enum',
        'insert boolean' => 'bool',
    });
}

for(my $i = 0; $i < $runs; $i++) {
    cmpthese($repeat, {
        'select tinyint(1)' => 'tinyint_s',
        'select char(1)' => 'char_s',
        'select enum(\'y\', \'n\')' => 'enum_s',
        'select boolean' => 'bool_s',
    });
}

$eth->finish();
$eth1->finish();
$cth->finish();
$cth1->finish();
$tth->finish();
$tth1->finish();
$bth->finish();
$bth1->finish();
$dbh->do("DROP TABLE ${table_name}_ti");
$dbh->do("DROP TABLE ${table_name}_ci");
$dbh->do("DROP TABLE ${table_name}_ei");
$dbh->do("DROP TABLE ${table_name}_bi");
$dbh->disconnect();
