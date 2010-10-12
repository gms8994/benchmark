#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'ne();',
        'trinary();',
        'ifelse();',
    ), 1000000
);

function ne() {
    $y = rand(0,1);
    $x = $y != -1;
}
function trinary() {
    $y = rand(0,1);
    $x = $y != -1 ? 1 : 0;
}
function ifelse() {
    $y = rand(0,1);
    if ($y != -1) {
        $x = 1;
    } else {
        $x = 0;
    }
}
