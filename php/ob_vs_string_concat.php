#!/usr/bin/php
<?php

$subloop = 100;
$repeat = 100;

require_once "bench.php";

cmp_these(
    array(
        'ob();',
        'sc();',
        'tc();',
    ), 1000000
);

function sc() {
    global $subloop, $repeat;
    $string = '';
    for($i = 0; $i <= $subloop; $i++) {
        $string .= str_repeat(' ', $repeat);
    }
    $string = '';
    return false;
}

function ob() {
    global $subloop, $repeat;
    ob_start();
    for($i = 0; $i <= $subloop; $i++) {
        echo str_repeat(' ', $repeat);
    }
    ob_end_clean();
    return false;
}

function tc() {
    global $subloop, $repeat;
    try {
        for($i = 0; $i <= $subloop; $i++) {
            echo str_repeat(' ', $repeat);
        }
        throw new Exception('Empty output');
    } catch (Exception $e) {
    }
    return false;
}
