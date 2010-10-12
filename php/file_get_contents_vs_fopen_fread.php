#!/usr/bin/php
<?php

require_once "bench.php";

file_put_contents("test.txt", "THIS IS A TEST. THIS IS ONLY A TEST");

cmp_these(
    array(
        'fo();',
        'fg();',
    ), 100000
);

function fo() {
    $f = fopen('test.txt', 'r');
    $txt = fread($f, 8192);
    fclose($f);
}
function fg() {
    $txt = file_get_contents('test.txt');
}
