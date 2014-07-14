#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'expl();',
        'gcsv();' 
    ), 100000
);

function expl() {
	explode("1,2,3,4", ",");
}
function gcsv() {
	str_getcsv("1,2,3,4", ",");
}
