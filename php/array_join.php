#!/usr/bin/php
<?php

require_once "bench.php";

$string = 'abcdefghijklmnopqrstuvwxyz';
global $array;
$array = str_split($string);

cmp_these(
    array(
        't_foreach();',
        't_implode();',
    ), 100000
);

function t_foreach() {
    global $array;
    $string = "";
    foreach ($array as $char) {
        $string .= "<category>{$char}</category>";
    }
}
function t_implode() {
    global $array;
    $string = "<category" . implode("</category><category>", $array) . "</category>";
}
