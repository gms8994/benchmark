#!/usr/bin/php
<?php

$blah = 123;

require_once "bench.php";

cmp_these(
    array(
        'single_quotes();',
        'double_quotes();',
        'double_quotes_interp();',
        'sprint();'
    ), 1000000
);

function single_quotes() {
    $var = 'Word: '.$blah.' ';
}
function double_quotes() {
    $var = "Word: ".$blah." ";
}
function double_quotes_interp() {
    $var = "Word: $blah ";
}
function sprint() {
    $var = sprintf("Word: %s ", $blah);
}
